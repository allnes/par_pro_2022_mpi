// Copyright 2022 Zorin Oleg
#ifndef MODULES_TASK_1_ZORIN_O_COUNTING_NON_MATCHING_CHAR_H_
#define MODULES_TASK_1_ZORIN_O_COUNTING_NON_MATCHING_CHAR_H_

#include <string>

std::string getRandomString(size_t size);
int countNonMatchingCharSequential(const std::string &str,
                                   const std::string &compare_str);
int countNonMatchingCharParallel(const std::string &global_str,
                                 const std::string &global_compare_str);

#endif // MODULES_TASK_1_ZORIN_O_COUNTING_NON_MATCHING_CHAR_H_