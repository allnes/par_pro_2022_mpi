// Copyright 2022 Muhin Vadim
#ifndef MODULES_TASK_1_MUHIN_V_CHECK_LEX_ORDER_STRINGS_CHECK_LEX_ORDER_STRINGS_H_
#define MODULES_TASK_1_MUHIN_V_CHECK_LEX_ORDER_STRINGS_CHECK_LEX_ORDER_STRINGS_H_
#include <vector>

std::vector<char> getRandomString(std::vector<char>::size_type size);
int getParallelOperations(const std::vector<char>&str1, const std::vector<char>&str2,
                          std::vector<char>::size_type global_size);
int getSequentialOperations(const std::vector<char>& str1, const std::vector<char>& str2);

#endif  //  MODULES_TASK_1_MUHIN_V_CHECK_LEX_ORDER_STRINGS_CHECK_LEX_ORDER_STRINGS_H_
