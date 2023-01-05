  // Copyright 2022 Mironov Nikita
#ifndef MODULES_TASK_1_MIRONOV_N_COUNT_REQUIRED_CHAR_IN_STR_COUNT_REQUIRED_CHAR_IN_STR_H_
#define MODULES_TASK_1_MIRONOV_N_COUNT_REQUIRED_CHAR_IN_STR_COUNT_REQUIRED_CHAR_IN_STR_H_

#include <mpi.h>
#include <random>
#include <cstring>
#include <string>
#include <iostream>

void getRandomStr(char* str, int strSize);

int countRequiredCharInProcStr(const char* str, char requiredChar);

int countRequiredCharInStr(std::string str, char requiredChar);

#endif  // MODULES_TASK_1_MIRONOV_N_COUNT_REQUIRED_CHAR_IN_STR_COUNT_REQUIRED_CHAR_IN_STR_H_"
