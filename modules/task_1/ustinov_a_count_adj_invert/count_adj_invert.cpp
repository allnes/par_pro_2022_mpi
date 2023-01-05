// Copyright 2022 Ustinov A.
#include "../../../modules/task_1/ustinov_a_count_adj_invert/count_adj_invert.h"
#include <mpi.h>
#include <random>

using std::vector;
using std::mt19937;
using std::random_device;

int count_adjacent_invertions_parallel(const vector<int> &vec) {
    MPI_Status status;
    int size;           // total number of processes
    int rank;           // ID of current process
    int b_size;         // length of block in current process
    int b_ans = 0;      // answer counted for a block
    int total_ans = 0;  // answer counted for the entire array
    int n = static_cast<int>(vec.size());  // number of elements in vector
    int *local_buffer;                     // buffer for data in each process
    const int *data = vec.data();          // Pointer to data in vector

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    local_buffer = new int[n/size + 2];
    if (rank == 0) {
        int b_offset;  // position of beginning of the sent block

        // send data to all other processes
        // every process compares current element with previous
        // hence element 'data[i*n/size - 1]' has to be sent too
        // that's why there's '-1' in 'b_offset' and '+1' in 'b_size'
        for (int i = 1; i < size; ++i) {
            b_offset = i*n/size - 1;
            b_size = (i+1)*n/size - b_offset;
            MPI_Send(data + b_offset, b_size, MPI_INT, i, 69, MPI_COMM_WORLD);
        }

        b_size = n/size;  // set correct value for block size in process 0
        for (int i = 0; i < b_size; ++i)
            local_buffer[i] = data[i];
    } else {
        b_size = (rank+1)*n/size - rank*n/size + 1;
        MPI_Recv(local_buffer, b_size, MPI_INT, 0, 69, MPI_COMM_WORLD, &status);
    }

    for (int i = 1; i < b_size; ++i)
        if (local_buffer[i] < local_buffer[i-1])
            ++b_ans;
    MPI_Reduce(&b_ans, &total_ans, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    delete[] local_buffer;
    return total_ans;
}

int count_adjacent_invertions_sequential(const vector<int> &vec) {
    int total_ans = 0;
    int n = static_cast<int>(vec.size());
    const int *data = vec.data();

    for (int i = 1; i < n; ++i)
        if (data[i] < data[i-1])
            ++total_ans;

    return total_ans;
}

vector<int> get_random_vector(size_t vector_size) {
    vector<int> vec(vector_size);
    mt19937 generator {random_device {}()};

    for (size_t i = 0; i < vector_size; ++i)
        vec[i] = generator();

    return vec;
}
