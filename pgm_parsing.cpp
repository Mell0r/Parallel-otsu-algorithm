#include <iostream>
#include <vector>
#include <string>
#include <omp.h>

#include "otsu.hpp"
#include "pgm_parsing.hpp"

using std::to_string;
using namespace noexcept_io;

or_error<vector<vector<pixel>>> parse_pgm(noexcept_reader& reader) {
	auto tmp = reader.read_char();
	char c1;
	if (unwrap_to_or_error(tmp, c1))
		return get_error(tmp);

	tmp = reader.read_char();
	char c2;
	if (unwrap_to_or_error(tmp, c2))
		return get_error(tmp);

	if (c1 != 'P' || c2 != '5')
		return error("Wrong file format: there is no P5 in the beginning.");

	int width;
	auto tmp1 = reader.read_int();
	if (unwrap_to_or_error(tmp1, width))
		return get_error(tmp1);

	int height;
	tmp1 = reader.read_int();
	if (unwrap_to_or_error(tmp1, height))
		return get_error(tmp1);

	int dim;
	tmp1 = reader.read_int();
	if (unwrap_to_or_error(tmp1, dim))
		return get_error(tmp1);
	if (dim != 255)
		return error("Wrong file format: given pixel dimention is not supported.");

	reader.skip_spaces();

	vector<vector<pixel>> pic(height, vector<pixel>(width, 0));
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			auto pix = reader.read_char();
			char ch;
			if (unwrap_to_or_error(pix, ch))
				return get_error(pix);
			if (ch < 0)
				pic[r][c] = (int)ch + 256;
			else
				pic[r][c] = ch;
		}
	}
	return pic;
}

maybe_error write_pgm(noexcept_writer& writer, vector<vector<pixel>>& pic) {
	auto tmp = writer.writeln("P5");
	if (is_error(tmp))
		return get_error(tmp);
	tmp = writer.writeln(to_string(pic.size()) + " " + to_string(pic[0].size()));
	if (is_error(tmp))
		return get_error(tmp);
	tmp = writer.writeln("255");
	if (is_error(tmp))
		return get_error(tmp);
	for (auto& row : pic) {
		for (auto& pix : row) {
			auto tmp = writer.write(pix);
			if (is_error(tmp))
				return get_error(tmp);
		}
	}
	return ok();
}