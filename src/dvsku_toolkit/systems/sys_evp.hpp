#pragma once

#include <functional>
#include <string>

namespace dvsku_toolkit {
    class systems_bundle;

    class sys_evp {
    public:
        typedef std::function<void()>                   callback_start;
        typedef std::function<void(bool)>               callback_finish;
        typedef std::function<void(float)>              callback_update;
        typedef std::function<void(const std::string&)> callback_error;

        typedef bool* cancel_token;

        typedef std::string FILE_PATH;
        typedef std::string FOLDER_PATH;
    
    public:
        sys_evp() = delete;
        sys_evp(systems_bundle& systems);

        void set_start_callback(callback_start callback);
        void set_finish_callback(callback_finish callback);
        void set_update_callback(callback_update callback);
        void set_error_callback(callback_error callback);

        void set_cancel_token(cancel_token token);

        void pack(const FOLDER_PATH& input, const FILE_PATH& output, int filter = 0, 
            bool encrypt = false, const std::string& key = "", uint8_t iv = 0);

        void unpack(const FILE_PATH& input, const FOLDER_PATH& output,
            bool decrypt = false, const std::string& key = "", uint8_t iv = 0);

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
