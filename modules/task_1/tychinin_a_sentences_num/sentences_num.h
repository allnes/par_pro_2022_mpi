// Copyright 2022 Tychinin Alexey

#ifndef MODULES_TASK_1_TYCHININ_A_SENTENCES_NUM_SENTENCES_NUM_H_
#define MODULES_TASK_1_TYCHININ_A_SENTENCES_NUM_SENTENCES_NUM_H_

#include <string>

namespace task1 {
    std::string generateSentence(const size_t chNum);

    int countSentences(const std::string& str, const bool toParallel = false, const size_t chNum = 0);
}

#endif  // MODULES_TASK_1_TYCHININ_A_SENTENCES_NUM_SENTENCES_NUM_H_
