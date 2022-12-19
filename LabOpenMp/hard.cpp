#include <iostream>
#include <vector>
#include <string>
#include <omp.h>

#include "utility.hpp"
#include "simple_io.hpp"
#include "max_levels_calculators.hpp"

using std::cerr;
using std::cout;
using std::vector;
using std::string;
using std::to_string;
using namespace simple_io;

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
	writer.writeln("P5");
	writer.writeln(to_string(pic.size()) + " " + to_string(pic[0].size()));
	writer.writeln("255");
	for (auto& row : pic) {
		for (auto& pix : row) {
			auto tmp = writer.write(pix);
			if (is_error(tmp))
				return get_error(tmp);
		}
	}
	return ok();
}

void apply_levels(vector<vector<pixel>>& pic, const vector<int>& levels, const vector<pixel>& maps_to) {
	for (auto& row : pic)
		for (auto& pix : row)
			pix = maps_to[lower_bound(levels.begin(), levels.end(), pix) - levels.begin()];
}

vector<int> ocu_one_thread(vector<vector<pixel>>& pic) {
	vector<int> max_levels = calc_max_levels_one_thread(pic, 256);
	apply_levels(pic, max_levels, {0, 84, 170, 255});
	return max_levels;
}

vector<int> ocu_multithread(int threads, vector<vector<pixel>>& pic) {
	vector<int> max_levels = calc_max_levels_multithread(threads, pic, 256);
	apply_levels(pic, max_levels, { 0, 84, 170, 255 });
	return max_levels;
}

vector<int> ocu_default(vector<vector<pixel>>& pic) {
	vector<int> max_levels = calc_max_levels_default(pic, 256);
	apply_levels(pic, max_levels, { 0, 84, 170, 255 });
	return max_levels;
}

int main(int argc, char* argv[])
{
	if (argc != 4) {
#ifdef _DEBUG
		char* test[] = { 
			"", 
			"0",
			"C:/Users/JazzM/source/repos/LabOpenMp/LabOpenMp/test_data/in.pgm",
			"C:/Users/JazzM/source/repos/LabOpenMp/LabOpenMp/test_data/out.pgm"
		};
		argc = 4;
		argv = test;
#else
		cerr << "Wrong number of arguments.";
		return 0;
#endif
	}

	/*noexcept_reader reader;
	unwrap_or_print_error(reader.open(argv[2]));
	noexcept_writer writer;
	unwrap_or_print_error(writer.open(argv[3]));

	vector<vector<pixel>> pic = unwrap_or_print_error(parse_pgm(reader));

	vector<int> max_levels;
	if (std::stoi(argv[1]) == -1)
		max_levels = ocu_one_thread(pic);
	else if (std::stoi(argv[1]) == 0)
		max_levels = ocu_default(pic);
	else
		max_levels = ocu_multithread(std::stoi(argv[1]), pic);

	unwrap_or_print_error(write_pgm(writer, pic));

	for (int i = 0; i < max_levels.size(); i++) {
		if (i > 0)
			cout << ' ';
		cout << max_levels[i];
	}
	cout << '\n';
	return 0;*/

	int cnt = 0;
#pragma omp parallel for collapse(2)
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 10; j++) {
#pragma omp critical
				{
					if (j > i)
						cnt++;
				}
			}
		}
	cout << cnt;
}
