// Copyright 2022
#pragma once
#include <mpi.h>

#include <algorithm>
#include <climits>
#include <ctime>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>
std::string getRandomString(size_t size);
int CountingAlphabeticCharParallel(const std::string& str);
int CountingAlphabeticCharSequential(const std::string& str);
