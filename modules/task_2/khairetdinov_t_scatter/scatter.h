// Copyright 2022 Khairetdinov Timur
#ifndef MODULES_TASK_2_KHAIRETDINOV_T_SCATTER_SCATTER_H_
#define MODULES_TASK_2_KHAIRETDINOV_T_SCATTER_SCATTER_H_

#include <mpi.h>
#include <string.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <functional>

int Scatter(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data,
            int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator);
int FasterScatter(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data,
                  int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator);
int MyTest(std::vector<int> mas, std::function<int(void* , int , MPI_Datatype , void* ,
           int , MPI_Datatype , int , MPI_Comm)> func);

#endif  // MODULES_TASK_2_KHAIRETDINOV_T_SCATTER_SCATTER_H_
