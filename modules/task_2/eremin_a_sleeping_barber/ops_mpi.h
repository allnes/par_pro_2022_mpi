// Copyright 2022 Eremin Aleksandr
#ifndef MODULES_TASK_2_EREMIN_A_SLEEPING_BARBER_OPS_MPI_H_
#define MODULES_TASK_2_EREMIN_A_SLEEPING_BARBER_OPS_MPI_H_

#include <vector>
#include <string>

void client(int rank);
void queue(int numberOFseats, int numberOFclients);
void Barber(int numberOFseats, int numberOFclients);

#endif  // MODULES_TASK_2_EREMIN_A_SLEEPING_BARBER_OPS_MPI_H_
