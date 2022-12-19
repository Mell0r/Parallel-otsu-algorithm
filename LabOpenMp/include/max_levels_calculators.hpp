#pragma once

#include <vector>

#include "utility.hpp"

using std::vector;

vector<int> calc_max_levels_one_thread(const vector<vector<pixel>>& pic, int pic_dim);
vector<int> calc_max_levels_multithread(int threads, const vector<vector<pixel>>& pic, int pic_dim);
vector<int> calc_max_levels_default(const vector<vector<pixel>>& pic, int pic_dim);