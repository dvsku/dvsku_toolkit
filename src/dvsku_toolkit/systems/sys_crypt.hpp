#pragma once

#include <vector>
#include <filesystem>
#include <functional>

namespace dvsku_toolkit {
    class systems_bundle;

    class sys_crypt {
    public:
        typedef std::function<void()>                   callback_start;
        typedef std::function<void(bool)>               callback_finish;
        typedef std::function<void(float)>              callback_update;
        typedef std::function<void(const std::string&)> callback_error;

        typedef bool* cancel_token;

        typedef std::filesystem::path FILE_PATH;
        typedef std::filesystem::path FOLDER_PATH;
        typedef std::vector<uint8_t>  BUFFER;

    public:
        sys_crypt() = delete;
        sys_crypt(systems_bundle& systems);

        void set_start_callback(callback_start callback);
        void set_finish_callback(callback_finish callback);
        void set_update_callback(callback_update callback);
        void set_error_callback(callback_error callback);

        void set_cancel_token(cancel_token token);
    
        void encrypt(const FOLDER_PATH& input, const FOLDER_PATH& output, int filter, const std::string& key, uint8_t iv = 0);
        void encrypt(BUFFER& buffer, const std::string& key, uint8_t iv = 0);

        void decrypt(const FOLDER_PATH& input, const FOLDER_PATH& output, const std::string& key, uint8_t iv = 0);
        void decrypt(BUFFER& buffer, const std::string& key, uint8_t iv = 0);

        bool should_be_encrypted(const FILE_PATH& file);

    private:
        systems_bundle& m_systems;

        callback_start  m_start_callback  = nullptr;
        callback_finish m_finish_callback = nullptr;
        callback_update m_update_callback = nullptr;
        callback_error  m_error_callback  = nullptr;

        cancel_token m_cancel_token = nullptr;

    private:
        void reset();
    };
}
