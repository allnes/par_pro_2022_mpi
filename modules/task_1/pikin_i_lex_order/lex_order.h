// Copyright 2022 Pikin Ilya
#ifndef MODULES_TASK_1_PIKIN_I_LEX_ORDER_LEX_ORDER_H_
#define MODULES_TASK_1_PIKIN_I_LEX_ORDER_LEX_ORDER_H_

#include <vector>
#include <string>

enum class Result { less_than, equal, greater_than };

std::string getStrFromRes(Result result);
std::string getRandomString(const int strSize);
std::string changeSomeChars(std::string str, int numChars);
Result getSequentialCompare(const std::string& a, const std::string& b);
Result getParallelCompare(const std::string& a, const std::string& b);

#endif  // MODULES_TASK_1_PIKIN_I_LEX_ORDER_LEX_ORDER_H_
