// Copyright 2021 Musin Alexandr
#ifndef MODULES_TASK_1_MUSIN_A_CHARS_ON_STR_CHARS_ON_STR_H_
#define MODULES_TASK_1_MUSIN_A_CHARS_ON_STR_CHARS_ON_STR_H_

#include <string>

char *getRandomString(const int size);
int par_sym_on_str(const char *global_str, const int global_str_len,
                   const char sym);
int sym_on_str(const char *str, const int size, const char sym);

#endif  // MODULES_TASK_1_MUSIN_A_CHARS_ON_STR_CHARS_ON_STR_H_
