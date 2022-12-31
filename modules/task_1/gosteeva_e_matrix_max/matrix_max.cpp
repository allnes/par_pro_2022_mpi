  // Copyright 2022 Gosteeva Ekaterina

#include "../../../modules/task_1/gosteeva_e_matrix_max/matrix_max.h"
std::vector<int> GetRandomMatrix(int matrix_size) {
    std::random_device rand_dev;
    std::mt19937 random(rand_dev());
    std::vector<int> matrix(matrix_size);
    for (int i = 0; i < matrix_size; i++) {
        matrix.at(i) = random() % 100;
    }
    return matrix;
}

int GetMatrixMaxSequential(const std::vector<int> &matrix) {
    int max = matrix.at(0);
    for (size_t i = 1; i < matrix.size(); i++) {
        if (matrix.at(i) > max) {
            max = matrix.at(i);
        }
    }
    return max;
}

int GetMatrixMaxParralel(const std::vector<int> &matrix, const int matrix_size) {
    int MPI_size, MPI_rank, vec_max, matrix_max = 0;
    MPI_Status MPI_status;
    MPI_Comm_size(MPI_COMM_WORLD, &MPI_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &MPI_rank);
    int int_quan = matrix_size / MPI_size;
    int rem_quan = matrix_size % MPI_size;
    int all_quan = int_quan + rem_quan;
    std::vector<int> vector(int_quan);

    if (MPI_rank == 0) {
        for (int proc_rank = 1; proc_rank <= MPI_size - 1; proc_rank++) {
            MPI_Send(matrix.data() + rem_quan + proc_rank * int_quan, int_quan, MPI_INT, proc_rank, 0, MPI_COMM_WORLD);
        }
    }
    if (MPI_rank == 0) {
        vector = std::vector<int>(
            matrix.begin(), matrix.begin() + all_quan);
    } else {
        MPI_Recv(vector.data(), int_quan, MPI_INT, 0, 0, MPI_COMM_WORLD, &MPI_status);
    }
    vec_max = GetMatrixMaxSequential(vector);
    MPI_Reduce(&vec_max, &matrix_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    return matrix_max;
}
