#pragma once

#include <string>
#include <vector>
#include <functional>
#include <filesystem>
#include <array>

namespace libdvsku::crypt {
    struct crypt_result {
        enum class e_status : unsigned char {
            ok        = 0x0,
            cancelled = 0x1,
            error     = 0x2
        };

        e_status status = e_status::ok;
        std::string msg = "";

        crypt_result() {};

        crypt_result(e_status _status)
            : status(_status) {};

        crypt_result(e_status _status, const std::string& _msg)
            : status(_status), msg(_msg) {};

        explicit operator bool() const {
            return status == e_status::ok;
        }
    };

    class libdvsku_crypt {
        public:
            typedef std::filesystem::path FILE_PATH;
            typedef std::vector<uint8_t>  BUFFER;

            libdvsku_crypt(const libdvsku_crypt&) = delete;
            libdvsku_crypt(libdvsku_crypt&&)      = delete;

            libdvsku_crypt& operator=(const libdvsku_crypt&) = delete;
            libdvsku_crypt& operator=(libdvsku_crypt&&)      = delete;

        public:
            // Set encryption/decryption key
            static void set_key(const char* key);

            // Set iv
            static void set_iv(uint8_t iv);

            // Check if file is encrypted by checking magic
            static bool is_file_encrypted(const FILE_PATH& file);

            // Check if buffer is encrypted by checking magic
            static bool is_buffer_encrypted(uint8_t* buffer_ptr, size_t size);

            // Get MD5 hash
            static std::array<unsigned char, 16> compute_md5(const void* ptr, size_t size);

            // Preforms encryption on the input file and saves the encrypted data to
            // the output file.
            // Encryption is skipped if file is encrypted.
            // If output is empty, result will be saved to input.
            static crypt_result encrypt_file(const FILE_PATH& input, const FILE_PATH& output = "");
            
            // Preforms encryption on the input file and saves the encrypted data to
            // the out buffer.
            // Encryption is skipped if file is encrypted.
            static crypt_result encrypt_file(const FILE_PATH& input, BUFFER& out);

            // Preforms encryption on the buffer.
            // Encryption is skipped if file is encrypted.
            // Buffer content and size will be modified.
            static crypt_result encrypt_buffer(BUFFER& buffer);

            // Preforms decryption on the input file and saves the data to the output file.
            // Decryption is skipped if file is not encrypted.
            // If output is empty, result will be saved to input.
            static crypt_result decrypt_file(const FILE_PATH& input, const FILE_PATH& output = "");
            
            // Preforms decryption on the input file and saves the data to the out buffer.
            // Decryption is skipped if file is not encrypted.
            static crypt_result decrypt_file(const FILE_PATH& input, BUFFER& out);

            // Preforms decryption then decompression on the buffer.
            // Buffer content and size will be modified.
            // Decryption is skipped if file is not encrypted.
            static crypt_result decrypt_buffer(BUFFER& buffer);

            // Preforms decryption on the buffer. Buffer data is replaced with decrypted data
            // if decryption was successful. 
            // Decryption is skipped if file is not encrypted.
            // Magic is replaced with zeroes and new_size contains the new buffer size.
            static crypt_result decrpyt_buffer(uint8_t* buffer_ptr, size_t size, size_t* new_size);

            // Remark: Assummes the buffer is encrypted! Only use on encrypted buffers!
            // Preforms decryption on the buffer. Buffer data is replaced with decrypted data
            // if decryption was successful.
            // Use when you know for sure that data is encrypted and DOESN'T contain magic!
            // Use when a large buffer is split into multiple smaller. 
            // Offset is offset from start of first buffer.
            // Call decrpyt_split_buffer_cleanup() after you finish decrypting.
            static crypt_result decrpyt_split_buffer(uint8_t* buffer_ptr, size_t size, size_t offset);

            // Cleanup current split buffer decryption
            static void decrpyt_split_buffer_cleanup();

        private:
             libdvsku_crypt() = default;
            ~libdvsku_crypt() = default;
    };
}