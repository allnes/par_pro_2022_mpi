// Copyright 2022 Panov Alexey
#include <mpi.h>
#include <cmath>
#include <iostream>
#include <string>
#include <algorithm>
#include "../../../modules/task_1/panov_a_symbols_diff/symbols_diff.h"


int getDifferentSymbolsCountSequentially(
    const std::string& first,
    const std::string& second,
    int from,
    int to
) {
    int diffCount = 0;

    for (int i = from; i < to; i++) {
        if (first[i] != second[i]) diffCount++;
    }

    return diffCount;
}

int getDifferentSymbolsCountParallel(
    const std::string& first,
    const std::string& second
) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int minStringSize = std::min(first.size(), second.size());

    int step = minStringSize / size + ((minStringSize % size) != 0);

    int localFrom = step * rank;
    int localTo = std::min(step * (rank + 1), minStringSize);

    int localDiff = getDifferentSymbolsCountSequentially(first, second, localFrom, localTo);

    int sum = 0;
    MPI_Reduce(&localDiff, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    int stringSizeDiff = std::abs(static_cast<int>(first.size()) - static_cast<int>(second.size()));

    return sum + stringSizeDiff;
}
