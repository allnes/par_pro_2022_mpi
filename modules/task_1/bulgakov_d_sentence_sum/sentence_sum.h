// Copyright 2022 Bulgakov Daniil

#ifndef MODULES_TASK_1_BULGAKOV_D_SENTENCE_SUM_SENTENCE_SUM_H_
#define MODULES_TASK_1_BULGAKOV_D_SENTENCE_SUM_SENTENCE_SUM_H_

#include "string"

std::string genRandomString(int len);

std::string getRandomText(int word_count);

int getParallelOperations(std::string global_str, int str_len);

int getSequentialOperations(std::string str);

#endif  // MODULES_TASK_1_BULGAKOV_D_SENTENCE_SUM_SENTENCE_SUM_H_
