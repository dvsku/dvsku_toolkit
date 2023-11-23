#include "systems/sys_crypt.hpp"
#include "systems_bundle.hpp"

#include "libdvsku_crypt/libdvsku_crypt.hpp"

#include <thread>
#include <algorithm>

using namespace dvsku_toolkit;

static std::filesystem::path construct_output_path(const std::filesystem::path& file,
    const std::filesystem::path& input, const std::filesystem::path& output);

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

sys_crypt::sys_crypt(systems_bundle& systems) 
    : m_systems(systems) {}

void sys_crypt::set_start_callback(callback_start callback) {
    m_start_callback = callback;
}

void sys_crypt::set_finish_callback(callback_finish callback) {
    m_finish_callback = callback;
}

void sys_crypt::set_update_callback(callback_update callback) {
    m_update_callback = callback;
}

void sys_crypt::set_error_callback(callback_error callback) {
    m_error_callback = callback;
}

void sys_crypt::set_cancel_token(cancel_token token) {
    m_cancel_token = token;
}

void sys_crypt::encrypt(const FOLDER_PATH& input, const FOLDER_PATH& output, int filter, const std::string& key, uint8_t iv) {
    std::thread t([this, input, output, filter, &key, iv]() {
        if (m_start_callback)
            m_start_callback();

        libdvsku::crypt::libdvsku_crypt::set_key(key.c_str());
        libdvsku::crypt::libdvsku_crypt::set_iv(iv);

        auto  files       = m_systems.evp.get_filtered_files(input, filter);
        float prog_change = 100.0f / files.size();

        for (auto& file : files) {
            if (m_cancel_token && *m_cancel_token) {
                if (m_finish_callback)
                    m_finish_callback(false);

                return;
            }

            // Only check when there is no filter
            if (filter != 0 && !should_be_encrypted(file))
                continue;

            std::filesystem::path output_path = output.empty() ? "" : construct_output_path(file, input, output);

            libdvsku::crypt::libdvsku_crypt::encrypt_file(file, output_path);

            if (m_update_callback)
                m_update_callback(prog_change);
        }

        if (m_finish_callback)
            m_finish_callback(true);

        reset();
    });
    t.detach();
}

void sys_crypt::encrypt(BUFFER& buffer, const std::string& key, uint8_t iv) {
    libdvsku::crypt::libdvsku_crypt::set_key(key.c_str());
    libdvsku::crypt::libdvsku_crypt::set_iv(iv);
    libdvsku::crypt::libdvsku_crypt::encrypt_buffer(buffer);
}

void sys_crypt::decrypt(const FOLDER_PATH& input, const FOLDER_PATH& output, const std::string& key, uint8_t iv) {
    std::thread t([this, input, output, &key, iv]() {
        if (m_start_callback)
            m_start_callback();

        libdvsku::crypt::libdvsku_crypt::set_key(key.c_str());
        libdvsku::crypt::libdvsku_crypt::set_iv(iv);

        auto  files       = m_systems.evp.get_filtered_files(input, 0);
        float prog_change = 100.0f / files.size();

        for (auto& file : files) {
            if (m_cancel_token && *m_cancel_token) {
                if (m_finish_callback)
                    m_finish_callback(false);

                return;
            }

            std::filesystem::path output_path = output.empty() ? "" : construct_output_path(file, input, output);

            libdvsku::crypt::libdvsku_crypt::decrypt_file(file, output_path);

            if (m_update_callback)
                m_update_callback(prog_change);
        }

        if (m_finish_callback)
            m_finish_callback(true);

        reset();
    });
    t.detach();
}

void sys_crypt::decrypt(BUFFER& buffer, const std::string& key, uint8_t iv) {
    libdvsku::crypt::libdvsku_crypt::set_key(key.c_str());
    libdvsku::crypt::libdvsku_crypt::set_iv(iv);
    libdvsku::crypt::libdvsku_crypt::decrypt_buffer(buffer);
}

bool sys_crypt::should_be_encrypted(const FILE_PATH& file) {
    // Extensions to ignore
    const std::vector<std::string> ignore {
        ".wav", ".ogg", ".db", ".ifl"
    };

    std::string fext = file.extension().string();

    return std::none_of(ignore.begin(), ignore.end(), [&](const std::string& ext) {
        return ext == fext;
    });
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

void sys_crypt::reset() {
    m_start_callback  = nullptr;
    m_finish_callback = nullptr;
    m_update_callback = nullptr;
    m_error_callback  = nullptr;

    m_cancel_token = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

std::filesystem::path construct_output_path(const std::filesystem::path& file, const std::filesystem::path& input, 
    const std::filesystem::path& output) 
{
    std::string relative_path(file.generic_string());

    size_t index = relative_path.find(input.generic_string());
    relative_path.erase(index, input.u8string().size());

    std::replace(relative_path.begin(), relative_path.end(), '/', '\\');

    if (relative_path[0] == '\\')
        relative_path.erase(0, 1);

    std::filesystem::path final_path(output);
    final_path.append(relative_path);

    return final_path;
}
