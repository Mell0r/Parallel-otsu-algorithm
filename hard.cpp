#include <iostream>
#include <vector>
#include <string>
#include <omp.h>

#include "utility.hpp"
#include "otsu.hpp"
#include "measurements.hpp"
#include "pgm_parsing.hpp"

using std::cerr;
using std::cout;
using std::vector;
using std::string;
using std::to_string;
using namespace noexcept_io;

void normal_start(vector<vector<pixel>>& pic, int threads_num, noexcept_writer& writer) {
	double start = omp_get_wtime();
	vector<int> max_levels;
	if (threads_num == -1)
		max_levels = calc_max_levels_one_thread(pic, 256);
	else if (threads_num == 0)
		max_levels = calc_max_levels_default(256, pic, 256);
	else
		max_levels = calc_max_levels_multithread(threads_num, pic, 256);
	apply_levels(pic, max_levels, { 0, 84, 170, 255 });
	double finish = omp_get_wtime();

	unwrap_or_print_error(write_pgm(writer, pic));

	cout << "Time (" << threads_num << " thread(s)): " << (finish - start) * 1000 << " ms\n";

	for (int i = 0; i < max_levels.size(); i++) {
		if (i > 0)
			cout << ' ';
		cout << max_levels[i];
	}
	cout << '\n';
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

	noexcept_reader reader;
	unwrap_or_print_error(reader.open(argv[2]));
	noexcept_writer writer;
	unwrap_or_print_error(writer.open(argv[3]));

	vector<vector<pixel>> pic = unwrap_or_print_error(parse_pgm(reader));

	normal_start(pic, std::stoi(argv[1]), writer);
	//measure_threads_num(pic);
	//measure_dynamic_blocks(pic);
	//measure_one_thread(pic);
}