// Copyright 2022 Elistratov Vladimir
#ifndef MODULES_TASK_1_ELISTRATOV_V_LETTER_COUNT_LETTER_COUNT_H_
#define MODULES_TASK_1_ELISTRATOV_V_LETTER_COUNT_LETTER_COUNT_H_

#include <algorithm>
#include <string>

std::string CreateRandomString(int elems_count);
int calculateCountSequental(std::string a);
int calculateCountParallel(const std::string &a, int elements_count);

#endif  // MODULES_TASK_1_ELISTRATOV_V_LETTER_COUNT_LETTER_COUNT_H_
