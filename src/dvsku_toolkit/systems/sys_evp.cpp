#include "systems/sys_evp.hpp"
#include "systems_bundle.hpp"

#include "libevp/libevp.hpp"

using namespace dvsku_toolkit;

sys_evp::sys_evp(systems_bundle& systems) 
    : m_systems(systems) {}

void sys_evp::set_start_callback(callback_start callback) {
    m_start_callback = callback;
}
     
void sys_evp::set_finish_callback(callback_finish callback) {
    m_finish_callback = callback;
}
     
void sys_evp::set_update_callback(callback_update callback) {
    m_update_callback = callback;
}
     
void sys_evp::set_error_callback(callback_error callback) {
    m_error_callback = callback;
}

void sys_evp::set_cancel_token(cancel_token token) {
    m_cancel_token = token;
}

void sys_evp::pack(const FOLDER_PATH& input, const FILE_PATH& output, int filter, 
    bool encrypt, const std::string& key, uint8_t iv) 
{
    if (encrypt)
        libevp::evp::do_buffer_processing([this, &key, iv](const std::filesystem::path& file, std::vector<uint8_t>& buffer) {
            if (!m_systems.crypt.should_be_encrypted(file)) return;
            m_systems.crypt.encrypt(buffer, key, iv);
        });

    libevp::evp::pack_async(
        input,
        output,
        static_cast<libevp::file_filter>(filter),
        m_cancel_token,
        [&]() {
            if (m_start_callback)
                m_start_callback();
        },
        [&](float value) {
            if (m_update_callback)
                m_update_callback(value);
        },
        [&](libevp::evp_result result) {
            if (m_finish_callback)
                m_finish_callback(result.status == libevp::evp_result::e_status::ok);

            reset();
        },
        [&](libevp::evp_result result) {
            if (m_error_callback)
                m_error_callback(result.msg);

            reset();
        }
    );
}

void sys_evp::unpack(const FILE_PATH& input, const FOLDER_PATH& output, 
    bool decrypt, const std::string& key, uint8_t iv) 
{
    if (decrypt)
        libevp::evp::do_buffer_processing([this, &key, iv](const std::filesystem::path& file, std::vector<uint8_t>& buffer) {
            m_systems.crypt.decrypt(buffer, key, iv);
        });

    libevp::evp::unpack_async(
        input,
        output,
        m_cancel_token,
        [&]() {
            if (m_start_callback)
                m_start_callback();
        },
        [&](float value) {
            if (m_update_callback)
                m_update_callback(value);
        },
        [&](libevp::evp_result result) {
            if (m_finish_callback)
                m_finish_callback(result.status == libevp::evp_result::e_status::ok);

            reset();
        },
        [&](libevp::evp_result result) {
            if (m_error_callback)
                m_error_callback(result.msg);

            reset();
        }
    );
}

std::vector<sys_evp::FILE_PATH> sys_evp::get_filtered_files(const FOLDER_PATH& input, int filter) {
    return libevp::evp::get_filtered_files(input, static_cast<libevp::file_filter>(filter));
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

void sys_evp::reset() {
    m_start_callback  = nullptr;
    m_finish_callback = nullptr;
    m_update_callback = nullptr;
    m_error_callback  = nullptr;

    m_cancel_token = nullptr;
}
