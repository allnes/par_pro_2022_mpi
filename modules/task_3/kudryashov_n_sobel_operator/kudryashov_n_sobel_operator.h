  // Copyright 2022 Kudryashov Nikita
#pragma once
#include <vector>

std::vector<std::vector<int>> generateRandomImage(int height, int width);
int clamp(int value, int min, int max);
std::vector<std::vector<int>> calcSobel(const std::vector<std::vector<int>>& image, int height, int width);
std::vector<std::vector<int>> calcSobelParallel(const std::vector<std::vector<int>>& image, int height, int width);
