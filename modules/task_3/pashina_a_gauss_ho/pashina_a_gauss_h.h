// Copyright 2022 Pashina Alina

#pragma once
#include <iostream>
#include <string>
#include <vector>

std::vector<unsigned char> generateNewImage(int rows_num, int columns_num);
unsigned char getNewColor(const std::vector<unsigned char> &myimg, int startX,
                          int startY, int rows_n, int columns_n);
std::vector<unsigned char> gauss_filt(
    const std::vector<unsigned char>& mystart_image,
                                      int rows_num, int columns_num);
std::vector<unsigned char> DoParallel_fGauss(
    std::vector<unsigned char> mystart_im, int rows_num, int columns_num);

