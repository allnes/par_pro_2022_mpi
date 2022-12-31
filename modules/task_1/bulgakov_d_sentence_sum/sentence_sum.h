// Copyright 2022 Bulgakov Daniil

#ifndef MODULES_TASK_1_BULGAKOV_D_SENTENCE_SUM_SENTENCE_SUM_H_
#define MODULES_TASK_1_BULGAKOV_D_SENTENCE_SUM_SENTENCE_SUM_H_

#include <string>
#include <vector>

std::vector<std::string> parseText(std::string str, int proc_num);

int computeSenteceCount(std::string str);

int parallelSentenceCount(const std::string& str);

#endif  // MODULES_TASK_1_BULGAKOV_D_SENTENCE_SUM_SENTENCE_SUM_H_
