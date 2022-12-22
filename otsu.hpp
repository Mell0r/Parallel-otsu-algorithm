#pragma once

#include <vector>

#include "utility.hpp"

using std::vector;

vector<int> calc_max_levels_one_thread(const vector<vector<pixel>>& pic, int pic_dim);
vector<int> calc_max_levels_multithread(int threads, const vector<vector<pixel>>& pic, int pic_dim);
vector<int> calc_max_levels_default(int dynamic_blocks, const vector<vector<pixel>>& pic, int pic_dim);
void apply_levels(vector<vector<pixel>>& pic, const vector<int>& levels, const vector<pixel>& maps_to);