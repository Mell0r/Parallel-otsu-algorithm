#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <stdint.h>

#include "utility.hpp"

using std::string;

namespace simple_io {
    const int BYTE_SIZE = 8;

    class noexcept_stream {
    protected:
    public:
        std::fstream stream;
        virtual ~noexcept_stream() noexcept;
        virtual maybe_error open(const string& filename) noexcept = 0;
    };

    class noexcept_reader : noexcept_stream {
        std::ifstream stream;
        std::vector<char> raw;
        int cur_ind = 0;
    public:
        maybe_error open(const string& filename) noexcept override;
        
        or_error<char> read_char() noexcept;
        or_error<int> read_int() noexcept;
        void skip_spaces() noexcept;
    };

    class noexcept_writer : noexcept_stream {
        std::ofstream stream;
    public:
        maybe_error open(const string& filename) noexcept override;

        template<typename T>
        maybe_error write(T val) noexcept {
            if (!(stream << val).good())
                return error("Couldn't write requeted type.");
            return ok();
        }

        template<typename T>
        maybe_error writeln(T val) noexcept {
            if (!(stream << val).good())
                return error("Couldn't write requeted type.");
            if (!stream.put(char(10)).good())
                return error("Couldn't write next line.");
            return ok();
        }
    };
}
