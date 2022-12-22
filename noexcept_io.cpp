#include "noexcept_io.hpp"

using std::monostate;

namespace noexcept_io {
    noexcept_stream::~noexcept_stream() {
        stream.close();
    }

    maybe_error noexcept_reader::open(const string& filename) noexcept {
        stream.open(filename, std::ifstream::in | std::ifstream::binary);
        if (!stream.is_open())
            return error("Couldn't open the input file.");
        raw = std::vector<char>(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
        return ok();
    }

    or_error<char> noexcept_reader::read_char() noexcept {
        if (cur_ind >= raw.size())
            return error("Couldn't read byte: EOF.");
        return raw[cur_ind++];
    }

    or_error<int> noexcept_reader::read_int() noexcept {
        while (cur_ind < raw.size() && (raw[cur_ind] < '0' || raw[cur_ind] > '9'))
            cur_ind++;
        if (cur_ind == raw.size())
            return error("Couldn't read int: EOF.");
        int res = 0;
        while (cur_ind < raw.size() && raw[cur_ind] >= '0' && raw[cur_ind] <= '9')
            res = res * 10 + raw[cur_ind++] - '0';
        return res;
    }

    void noexcept_reader::skip_spaces() noexcept {
        while (cur_ind < raw.size() && std::isspace(static_cast<unsigned char>(raw[cur_ind])))
            cur_ind++;
    }

    maybe_error noexcept_writer::open(const string& filename) noexcept {
        stream.open(filename, std::ofstream::out | std::ifstream::binary);
        if (!stream.is_open())
            return error("Couldn't open the output file.");
        return ok();
    }
}