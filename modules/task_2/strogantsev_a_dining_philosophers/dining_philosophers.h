// Copyright 2022 Strogantsev Anton
#ifndef MODULES_TASK_2_STROGANTSEV_A_DINING_PHILOSOPHERS_DINING_PHILOSOPHERS_H_
#define MODULES_TASK_2_STROGANTSEV_A_DINING_PHILOSOPHERS_DINING_PHILOSOPHERS_H_

#include <deque>

namespace TAG {
    const int REQUEST = 1;
    const int RESPONSE = 2;
    const int RELEASE = 3;
}

void printForksStatuses(const std::deque<bool>& statuses);

bool isForksAroundAreFree(const std::deque<bool>& statuses, int philosopher, int philosophersCount);
void markForksAroundAsLocked(std::deque<bool>* statuses, int philosopher, int philosophesCount);
void markForksAroundAsFree(std::deque<bool>* statuses, int philosopher, int philosophesCount);

void executePhilosopherLoop(int rank, int cycleCount);
void executeWaiterLoop(int philosophersCount, int cycleCount);

#endif  // MODULES_TASK_2_STROGANTSEV_A_DINING_PHILOSOPHERS_DINING_PHILOSOPHERS_H_
