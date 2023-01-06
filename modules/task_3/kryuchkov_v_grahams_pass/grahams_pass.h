// Copyright 2022 Kryuchkov Vladimir
#ifndef MODULES_TASK_3_KRYUCHKOV_V_GRAHAMS_PASS_GRAHAMS_PASS_H_
#define MODULES_TASK_3_KRYUCHKOV_V_GRAHAMS_PASS_GRAHAMS_PASS_H_

#include <stack>
#include <vector>

int rotate(const std::vector<int>& a, const std::vector<int>& b,
           const std::vector<int>& c);
int distance(const std::vector<int>& a, const std::vector<int>& b);
int compare(const void* a, const void* b);
std::vector<int> get_points(int count_p, int centre_x, int centre_y, int r);
std::vector<int> next_to_top(std::stack<std::vector<int>> st);
std::vector<int> grahams_sequential(const std::vector<int>& p, int count_points,
                                    int* size_shell);
std::vector<int> grahams_parallel(const std::vector<int>& points,
                                  int count_points, int* size_shell);

#endif  // MODULES_TASK_3_KRYUCHKOV_V_GRAHAMS_PASS_GRAHAMS_PASS_H_
