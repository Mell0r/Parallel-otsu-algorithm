#include "utility.hpp"
#include "noexcept_io.hpp"

using namespace noexcept_io;

or_error<vector<vector<pixel>>> parse_pgm(noexcept_reader& reader);
maybe_error write_pgm(noexcept_writer& writer, vector<vector<pixel>>& pic);