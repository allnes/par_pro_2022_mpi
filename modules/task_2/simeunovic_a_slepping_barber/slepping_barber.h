// Copyright 2022 Aleksandar Simeunovic
#ifndef MODULES_TASK_2_SIMEUNOVIC_A_SLEPPING_BARBER_SLEPPING_BARBER_H_
#define MODULES_TASK_2_SIMEUNOVIC_A_SLEPPING_BARBER_SLEPPING_BARBER_H_
#include<mpi.h>
#include<queue>
void DoBarberLoop(int n, int ProcSize, int ProcRank);
void Cutting_Hair(int client);
void ClientLoop(int ProcRank);
void Sleeping(int* mutex, bool* thread_running);
// void Empty_Buffer(bool* working, int* count);
// void Checking(int ProcRank);
bool AnyWorks(bool* procesess, int ProcSize);
#endif  // MODULES_TASK_2_SIMEUNOVIC_A_SLEPPING_BARBER_SLEPPING_BARBER_H_
