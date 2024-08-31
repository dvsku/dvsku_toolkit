#include "systems/dt_sys_evp.hpp"
#include "models/pack_tree_leaf.hpp"
#include "models/unpack_tree_leaf.hpp"
#include "dt_app.hpp"

using namespace dvsku_toolkit;

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

static bool is_client_dir(const std::filesystem::path& input);
static bool is_server_dir(const std::filesystem::path& input);
static bool is_client_file(const std::filesystem::path& input);
static bool is_server_file(const std::filesystem::path& input);

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dt_sys_evp::dt_sys_evp(dt_app& app)
    : dt_system_base(app)
{
    m_evp_context.cancel = &m_app.systems.core.work_context.cancel;

    m_evp_context.update_callback = [this](float progress_change) {
        m_app.systems.core.work_context.update_progress(progress_change);
    };

    m_evp_context.finish_callback = [this](libevp::evp_result result) {
        bool success = result.status == libevp::evp_result::status::ok ||
            result.status == libevp::evp_result::status::cancelled;

        if (result.status == libevp::evp_result::status::failure && result.message != "") {
            std::lock_guard guard(m_app.systems.core.mutex);
            m_app.systems.core.errors = result.message;
        }

        if (result.status == libevp::evp_result::status::ok) {
            switch (m_current_work) {
                case work_type::packing:   packing_inputs.reset();   break;
                case work_type::unpacking: unpacking_inputs.reset(); break;
                
                default: break;
            }
        }

        m_current_work = work_type::none;

        m_app.systems.core.work_context.stop_working(success);
        m_app.systems.core.work_context.set_progress(0.0f);
    };
};

void dt_sys_evp::pack() {
    if (m_app.systems.core.work_context.is_working())
        return;
    
    libevp::evp::pack_input inputs;
    inputs.base = packing_inputs.input;

    packing_inputs.tree.traverse([&](visual_tree::element_ptr_t element) {
        if (element->type == visual_tree_element::element_type::leaf) {
            auto leaf = std::static_pointer_cast<pack_tree_leaf>(element);

            if (leaf->is_checked())
                inputs.files.push_back(leaf->file);
        }
    });

    m_app.systems.core.errors = "";

    m_app.systems.core.work_context.start_working();
    m_current_work = work_type::packing;
    m_evp.pack_async(inputs, packing_inputs.output, &m_evp_context);
}

void dt_sys_evp::unpack() {
    if (m_app.systems.core.work_context.is_working())
        return;

    libevp::evp::unpack_input inputs;
    inputs.archive = unpacking_inputs.input;

    unpacking_inputs.tree.traverse([&](visual_tree::element_ptr_t element) {
        if (element->type == visual_tree_element::element_type::leaf) {
            auto leaf = std::static_pointer_cast<unpack_tree_leaf>(element);

            if (leaf->is_checked())
                inputs.files.push_back(leaf->file);
        }
    });

    m_app.systems.core.errors = "";

    m_app.systems.core.work_context.start_working();
    m_current_work = work_type::unpacking;
    m_evp.unpack_async(inputs, unpacking_inputs.output, &m_evp_context);
}

void dt_sys_evp::set_pack_files() {
    packing_inputs.tree        = visual_tree(std::make_shared<visual_tree_branch>(""));
    std::filesystem::path base = std::filesystem::absolute(packing_inputs.input);

    if (base == "" || !std::filesystem::is_directory(base))
        return;

    auto files = get_pack_files();
    for (const std::filesystem::path& file : files) {
        visual_tree::branch_ptr_t       branch;
        std::shared_ptr<pack_tree_leaf> leaf;

        if (!file.has_parent_path()) {
            leaf = std::make_shared<pack_tree_leaf>(file.filename().string());
            leaf->file = file.string();

            packing_inputs.tree.add_leaf(leaf);
        }
        else {
            std::filesystem::path dir = file;
            dir.remove_filename();

            if (dir == "" || !dir.is_relative())
                continue;

            visual_tree::branch_ptr_t parent = packing_inputs.tree.root;
            for (auto& segment : dir) {
                if (segment == "") continue;

                branch = packing_inputs.tree.get_branch(segment.string(), parent);
                if (!branch) {
                    branch = std::make_shared<visual_tree_branch>(segment.string());
                    packing_inputs.tree.add_branch(branch, parent);
                }

                parent = branch;
            }

            leaf = std::make_shared<pack_tree_leaf>(file.filename().string());
            leaf->file = file.string();

            packing_inputs.tree.add_leaf(leaf, branch);
        }
    }
}

void dt_sys_evp::set_unpack_files() {
    unpacking_inputs.tree         = visual_tree(std::make_shared<visual_tree_branch>(""));
    std::filesystem::path archive = std::filesystem::absolute(unpacking_inputs.input);

    if (archive == "" || !std::filesystem::is_regular_file(archive))
        return;

    std::vector<libevp::evp_fd> files = {};
    auto result = m_evp.get_archive_fds(archive, files);
    if (!result) {
        m_app.systems.core.errors = result.message;
        return;
    }

    for (const libevp::evp_fd& fd : files) {
        visual_tree::branch_ptr_t         branch;
        std::shared_ptr<unpack_tree_leaf> leaf;

        std::filesystem::path file = fd.file;

        if (!file.has_parent_path()) {
            leaf = std::make_shared<unpack_tree_leaf>(file.filename().string());
            leaf->file = fd;

            unpacking_inputs.tree.add_leaf(leaf);
        }
        else {
            std::filesystem::path dir = file;
            dir.remove_filename();

            if (dir == "" || !dir.is_relative())
                continue;

            visual_tree::branch_ptr_t parent = unpacking_inputs.tree.root;
            for (auto& segment : dir) {
                if (segment == "") continue;

                branch = unpacking_inputs.tree.get_branch(segment.string(), parent);
                if (!branch) {
                    branch = std::make_shared<visual_tree_branch>(segment.string());
                    unpacking_inputs.tree.add_branch(branch, parent);
                }

                parent = branch;
            }

            leaf = std::make_shared<unpack_tree_leaf>(file.filename().string());
            leaf->file = fd;

            unpacking_inputs.tree.add_leaf(leaf, branch);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// PRIVATE

std::vector<std::filesystem::path> dt_sys_evp::get_pack_files() {
    std::vector<std::filesystem::path> retval;

    bool is_client = is_client_dir(packing_inputs.input);
    bool is_server = is_server_dir(packing_inputs.input);

    for (const auto& element : std::filesystem::recursive_directory_iterator(packing_inputs.input)) {
        if (!element.is_regular_file()) continue;
        
        std::filesystem::path relative = std::filesystem::relative(element.path(), packing_inputs.input);
        if (relative == "") continue;

        if (is_client) {
            if (is_client_file(relative)) {
                retval.push_back(relative);
            }

            continue;
        }

        if (is_server) {
            if (is_server_file(relative)) {
                retval.push_back(relative);
            }

            continue;
        }

        retval.push_back(relative);
    }

    return retval;
}

bool is_client_dir(const std::filesystem::path& input) {
    std::filesystem::path tmp;
    bool exist = true;

    tmp = input;
    tmp /= "client.exe";

    if (std::filesystem::exists(tmp))
        return true;

    static const char* files[] = {
        "client_engine.ini", "client_game.ini"
    };

    for (const char* file : files) {
        tmp  = input;
        tmp /= file;

        if (!std::filesystem::exists(tmp) || !std::filesystem::is_regular_file(tmp)) {
            exist = false;
            break;
        }
    }

    if (exist)
        return true;

    static const char* dirs[] = {
        "local", "maps", "model", "script", "ui"
    };

    exist = true;

    for (const char* dir : dirs) {
        tmp  = input;
        tmp /= dir;

        if (!std::filesystem::exists(tmp) || !std::filesystem::is_directory(tmp)) {
            exist = false;
            break;
        }
    }

    return exist;
}

bool is_server_dir(const std::filesystem::path& input) {
    std::filesystem::path tmp;
    bool exist = true;
    
    static const char* files[] = {
        "server_engine.ini", "server_game.ini", "server_user.ini"
    };

    for (const char* file : files) {
        tmp  = input;
        tmp /= file;

        if (!std::filesystem::exists(tmp) || !std::filesystem::is_regular_file(tmp)) {
            return false;
        }
    }

    static const char* dirs[] = {
        "local", "maps", "script"
    };

    for (const char* dir : dirs) {
        tmp  = input;
        tmp /= dir;

        if (!std::filesystem::exists(tmp) || !std::filesystem::is_directory(tmp)) {
            exist = false;
            break;
        }
    }

    return exist;
}

bool is_client_file(const std::filesystem::path& input) {
    static std::string files[] = {
        "client_engine.ini", "client_game.ini"
    };

    static std::string dirs[] = {
        "local", "maps", "model", "model2", "script", "ui", "audio", "music", "scene"
    };

    for (const std::string& dir : dirs) {
        if (dir == *input.begin())
            return true;
    }

    for (const std::string& file : files) {
        if (file == input.filename())
            return true;
    }

    return false;
}

bool is_server_file(const std::filesystem::path& input) {
    static std::string files[] = {
        "server_engine.ini", "server_game.ini", "server_user.ini"
    };

    static std::string dirs[] = {
        "local", "maps", "script"
    };

    for (const std::string& dir : dirs) {
        if (dir == *input.begin())
            return true;
    }

    for (const std::string& file : files) {
        if (file == input.filename())
            return true;
    }

    return false;
}
