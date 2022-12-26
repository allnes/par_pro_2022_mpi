// Copyright 2022 Yashina Anastasia
#ifndef MODULES_TASK_3_YASHINA_A_CONVEX_BIN_CONVEX_BIN_H_
#define MODULES_TASK_3_YASHINA_A_CONVEX_BIN_CONVEX_BIN_H_

#include <tbb/tbb.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>


class SideClass {
public:
    double length(int* start, int* end);
    double cosinus(int* start, int* end, int* startsecond);
    int** getRandomPic(const int a, const int b);
};
class MainClass {
public:
    std::vector<int*> jarvis_TBB(int** convex_arr, int a);
    std::vector<int*> getComponent_TBB(int** arr, int a, int b, int*** convex_arr,
        int* num, int num_thr);
    std::vector<int*> inside(std::vector<int*> vec, int** convex_arr, int a);
    std::vector<int*> jarvis(int** convex_arr, int a);
    std::vector<int*> getComponent(int** arr, int a, int b, int*** convex_arr,
        int* num);
};

#endif  // MODULES_TASK_3_YASHINA_A_CONVEX_BIN_CONVEX_BIN_H_
