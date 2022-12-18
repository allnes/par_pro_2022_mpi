// Copyright 2022 Zorin Oleg
#ifndef MODULES_TASK_1_ZORIN_O_COUNTING_NON_MATCHING_CHAR_MPI_COUNTING_NON_MATCHING_CHAR_H_
#define MODULES_TASK_1_ZORIN_O_COUNTING_NON_MATCHING_CHAR_MPI_COUNTING_NON_MATCHING_CHAR_H_

#include <string>

const char *getRandomString(int len);

int countNonMatchingCharSequential(const char *str, int len,
                                   const char *compare_str, int compare_len);

int countNonMatchingCharParallel(const char *global_str, int len, const char *global_compare_str, int compare_len);

#endif  // MODULES_TASK_1_ZORIN_O_COUNTING_NON_MATCHING_CHAR_MPI_COUNTING_NON_MATCHING_CHAR_H_
