// Copyright 2021 Musin Alexandr
#include "../../../modules/task_2/musin_a_dining_philosophers/dining_philosophers.h"

#include <mpi.h>

#include <list>

bool is_can_eat(const bool *forks, const int philosoph, int ph_num) {
    return !(forks[philosoph - 1] || forks[philosoph % ph_num]);
}

bool is_cache_free(const bool *cache, int size) {
    for (int i = 0; i < size; i++) {
        if (cache[i]) {
            return false;
        }
    }
    return true;
}

void send_permission(bool *forks, int philosoph, int philosoph_num, bool mark) {
    forks[philosoph - 1] = mark;
    forks[philosoph % philosoph_num] = mark;
    const int permission = 1;
    if (mark) MPI_Send(&permission, 1, MPI_INT, philosoph, 0, MPI_COMM_WORLD);
}

int critical_code(int n, int process) {
    MPI_Status status;
    int param;
    MPI_Recv(&param, 1, MPI_INT, process, 0, MPI_COMM_WORLD, &status);
    return n += param;
}

void philosoph(int rank, int param) {
    MPI_Barrier(MPI_COMM_WORLD);
    int permission = 0;
    int readiness = 1;
    MPI_Status status;
    MPI_Send(&readiness, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(&permission, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    if (permission == 1) {
        MPI_Send(&param, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        int send_end = -1;
        MPI_Send(&send_end, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
}

int manager(int size) {
    if (size == 1) {
        return 0;
    }
    int philosoph_num = size - 1;
    bool *forks = new bool[philosoph_num];
    for (int j = 0; j < philosoph_num; j++) forks[j] = false;
    std::list<int> turn;
    int i = 0;
    int n = 0;
    MPI_Status status;
    int flag = 0;
    MPI_Barrier(MPI_COMM_WORLD);
    do {
        MPI_Recv(&flag, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        const int th_source = status.MPI_SOURCE;

        if (flag == 1) {
            i++;
            if (is_can_eat(forks, th_source, philosoph_num)) {
                send_permission(forks, th_source, philosoph_num, true);
                n = critical_code(n, th_source);
            } else {
                turn.push_back(th_source);
            }
        } else if (flag == -1) {
            send_permission(forks, th_source, philosoph_num, false);
            std::list<int> enabled_treads;
            for (int ph : turn) {
                if (is_can_eat(forks, ph, philosoph_num)) {
                    send_permission(forks, ph, philosoph_num, true);
                    n = critical_code(n, ph);
                    enabled_treads.push_back(ph);
                    break;
                }
            }
            for (int enabled_tread : enabled_treads) turn.remove(enabled_tread);
        }
    } while (i < philosoph_num || !turn.empty() ||
             !is_cache_free(forks, philosoph_num));
    return n;
}
