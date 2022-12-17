// Copyright 2022
#pragma once
#include <vector>
#include <string>
#include <mpi.h>
#include <random>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <ctime>

std::string getRandomString(size_t  size);
int CountingAlphabeticCharParallel(const std::string& str);
int CountingAlphabeticCharSequential(const std::string& str);