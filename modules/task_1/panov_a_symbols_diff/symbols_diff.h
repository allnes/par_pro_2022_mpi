// Copyright 2022 Panov Alexey
#ifndef MODULES_TASK_1_PANOV_A_SYMBOLS_DIFF_SYMBOLS_DIFF_H_
#define MODULES_TASK_1_PANOV_A_SYMBOLS_DIFF_SYMBOLS_DIFF_H_

#include <string>

int getDifferentSymbolsCountSequentially(
    const std::string& first,
    const std::string& second,
    int from,
    int to
);

int getDifferentSymbolsCountParallel(
    const std::string& first,
    const std::string& second
);

#endif  // MODULES_TASK_1_PANOV_A_SYMBOLS_DIFF_SYMBOLS_DIFF_H_
