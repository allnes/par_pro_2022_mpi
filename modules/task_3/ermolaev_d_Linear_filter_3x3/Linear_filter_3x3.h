// Copyright 2022 Ermolaev Danila
#ifndef MODULES_TASK_3_ERMOLAEV_D_LINEAR_FILTER_3X3_LINEAR_FILTER_3X3_H_
#define MODULES_TASK_3_ERMOLAEV_D_LINEAR_FILTER_3X3_LINEAR_FILTER_3X3_H_

#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <algorithm>
#include <iostream>

unsigned char* getRandomMatrix(unsigned char* matrix, int x);

typedef struct {
    unsigned char* data;
    int w;
    int h;
} Image;

void convolution(unsigned char* a, int aw, int ah, unsigned char* b, double* w,
    int ww, int wh);

void mpi_gauss_filter(Image* im, Image* om, int w);

void gauss_filter(Image* im, Image* om, int w);

#endif  // MODULES_TASK_3_ERMOLAEV_D_LINEAR_FILTER_3X3_LINEAR_FILTER_3X3_H_
