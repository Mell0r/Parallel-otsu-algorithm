#pragma once

#include <vector>

#include "utility.hpp"

using std::vector;

void measure_threads_num(vector<vector<pixel>>& pic);
void measure_dynamic_blocks(vector<vector<pixel>>& pic);
void measure_one_thread(vector<vector<pixel>>& pic);