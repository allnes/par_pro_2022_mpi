// Copyright 2022 Makarov Danila
#ifndef MODULES_TASK_3_MAKAROV_D_MARKING_ON_BINARY_IMAGE_MARKING_ON_BINARY_IMAGE_H_
#define MODULES_TASK_3_MAKAROV_D_MARKING_ON_BINARY_IMAGE_MARKING_ON_BINARY_IMAGE_H_

#include <vector>

std::vector<int> preset1();
std::vector<int> preset2();
std::vector<int> preset3();
std::vector<int> preset4();
std::vector<int> preset5();
std::vector<int> marking(const std::vector<int> &vecInput, int rows, int colls);
std::vector<int> rowsDistrib(std::vector<int> vecInput, int rows,
                             int colls);
std::vector<int> markingMPI(const std::vector<int> &vecInput, int locrows, int rows,
                            int colls, int rank);

#endif  // MODULES_TASK_3_MAKAROV_D_MARKING_ON_BINARY_IMAGE_MARKING_ON_BINARY_IMAGE_H_
