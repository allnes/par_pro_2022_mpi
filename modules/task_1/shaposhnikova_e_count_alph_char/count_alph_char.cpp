// Copyright 2022 Shaposhnikova Ekaterina
#include <mpi.h>

#include <string>

#include "../../../modules/task_1/shaposhnikova_e_count_alph_char/count_alph_char.h"


int sequentialCharCount(const std::string& procStr) {
  int countStr = 0;
  for (int i = 0; i < procStr.size(); i++)
  {
    if (std::isalpha(procStr[i]))
    {
      countStr++;
    }
  }

  return countStr;
}

int parallelCharCount(const std::string& naturalStr) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int procLen = naturalStr.length() / size;
    int rem = naturalStr.length() % size;

    std::string localStr;
    int gl_count = 0;
   
    if (rank == 0) {
        for (int proc = 0; proc < size-1; proc++) {
          MPI_Send(naturalStr.c_str() +proc * procLen, procLen,
            MPI_CHAR, proc + 1, 0, MPI_COMM_WORLD);
        }
    }  

    if (rank == 0) {
      localStr.resize(procLen + rem);
      localStr = std::string(naturalStr.end() - procLen - rem, naturalStr.end());
    }
    else {
      MPI_Status stat;

      char* local_buf = new char[procLen + 1];

      MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
      MPI_Get_count(&stat, MPI_CHAR, &procLen);
      MPI_Recv(local_buf, procLen + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &stat);

      localStr = std::string(local_buf);
      localStr = localStr.substr(0, procLen);
    }

    int loc_count = sequentialCharCount(localStr);

    MPI_Reduce(&loc_count, &gl_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return gl_count;
}
