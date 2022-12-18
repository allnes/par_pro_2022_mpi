// Copyright 2022 Strogantsev Anton
#include <mpi.h>
#include <list>
#include <deque>
#include <iostream>
#include "../../../modules/task_2/strogantsev_a_dining_philosophers/dining_philosophers.h"


void printForksStatuses(const std::deque<bool>& statuses) {
    for (bool status : statuses) {
        std::cout << (status ? '|' : '0');
    }
    std::cout << '\n';
}

bool isForksAroundAreFree(const std::deque<bool>& statuses, int philosopher, int philosophersCount) {
    return statuses[philosopher % philosophersCount] && statuses[philosopher - 1];
}

void markForksAroundAsLocked(std::deque<bool>* statuses, int philosopher, int philosophersCount) {
    statuses->at(philosopher % philosophersCount) = false;
    statuses->at(philosopher - 1) = false;
}

void markForksAroundAsFree(std::deque<bool>* statuses, int philosopher, int philosophersCount) {
    statuses->at(philosopher % philosophersCount) = true;
    statuses->at(philosopher - 1) = true;
}

void executePhilosopherLoop(int rank, int cycleCount) {
    int inputFlag = 0;
    int outputFlag = 0;
    MPI_Status status;

    // std::cout << "Philosopher " << rank << " is active\n";
    while (cycleCount-- > 0) {
        // std::cout << "Philosopher " << rank <<  " is thinking\n";
        // std::cout << "Philosopher " << rank << " is waiting\n";
        MPI_Send(&outputFlag, 1, MPI_INT, 0, TAG::REQUEST, MPI_COMM_WORLD);
        MPI_Recv(&inputFlag, 1, MPI_INT, 0, TAG::RESPONSE, MPI_COMM_WORLD, &status);
        // std::cout << "Philosopher " << rank << " is eating\n";
        // std::cout << "Philosopher " << rank << " is done eating\n";
        MPI_Send(&outputFlag, 1, MPI_INT, 0, TAG::RELEASE, MPI_COMM_WORLD);
    }
    // std::cout << "Philosopher " << rank << " is not active\n";
}

void executeWaiterLoop(int philosophersCount, int cycleCount) {
    cycleCount *= philosophersCount * 2;

    int inputFlag = 0;
    int outputFlag = 0;
    int handledPhilosopher = 0;
    MPI_Status status;

    std::list<int> waitingPhilosophers;
    std::deque<bool> forkStatuses(philosophersCount, true);

    while (cycleCount-- > 0) {
        MPI_Recv(&inputFlag, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        handledPhilosopher = status.MPI_SOURCE;

        printForksStatuses(forkStatuses);

        switch (status.MPI_TAG) {
        case TAG::REQUEST: {
            std::cout << "Waiter got philosopher " << handledPhilosopher << " request\n";

            if (isForksAroundAreFree(forkStatuses, handledPhilosopher, philosophersCount)) {
                markForksAroundAsLocked(&forkStatuses, handledPhilosopher, philosophersCount);
                MPI_Send(&outputFlag, 1, MPI_INT, handledPhilosopher, TAG::RESPONSE, MPI_COMM_WORLD);
                std::cout << "Waiter gave forks to philosopher " << handledPhilosopher << '\n';
            } else {
                waitingPhilosophers.push_back(handledPhilosopher);
                std::cout << "Philosopher " << handledPhilosopher << " is waiting\n";
            }
        }
            break;
        case TAG::RELEASE: {
            markForksAroundAsFree(&forkStatuses, handledPhilosopher, philosophersCount);

            std::cout << "Waiter got philosopher " << handledPhilosopher << " release\n";

            auto waitingPhilosophersCopy = waitingPhilosophers;
            for (int philosopher : waitingPhilosophersCopy) {
                if (isForksAroundAreFree(forkStatuses, philosopher, philosophersCount)) {
                    markForksAroundAsLocked(&forkStatuses, philosopher, philosophersCount);
                    MPI_Send(&outputFlag, 1, MPI_INT, philosopher, TAG::RESPONSE, MPI_COMM_WORLD);
                    std::cout << "Waiter gave forks to philosopher " << philosopher << '\n';
                    waitingPhilosophers.remove(philosopher);
                }
            }
        }
            break;
        default:
            std::cout << "Waiter got not valid tag";
            break;
        }
    }
}
