// Copyright 2022 Polyackov Lev
#include "../../../modules/task_3/polyackov_l_alg_fox/alg_fox.h"

std::vector<double> getRandMatrix(int size) {
    double mean = 0.0;
    double standard_deviation = 50.0;
    std::mt19937 gen;
    std::normal_distribution<double> distrib(mean, standard_deviation);
    std::vector<double> tmp_vec;
    tmp_vec.resize(size * size);
    for (auto& elem : tmp_vec) {
        elem = distrib(gen);
        if (((std::fabs(elem)) < std::numeric_limits<double>::epsilon())) {
            elem = 1.0;
        }
    }
    return tmp_vec;
}

void getResOfMatrMult(double* first_matr, double* second_matr, double* res_matr, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            *(res_matr + i * size + j) = 0.0;
            for (int k = 0; k < size; k++) {
                (*(res_matr + i * size + j)) += (*(first_matr + i * size + k)) * (*(second_matr + k * size + j));
            }
        }
    }
}

void updateMatrToNewSize(std::vector<double>* matr, int old_size, int new_size) {
    if (old_size >= new_size) {
        return;
    }
    std::vector<double> tmp_copy(*matr);
    matr->resize(new_size * new_size);
    int k = 0;
    for (int i = 0; i < old_size; i++) {
        for (int j = 0; j < new_size; j++) {
            if (j < old_size) {
                (*matr)[k] = tmp_copy[i * old_size + j];
            } else {
                (*matr)[k] = 0.0;
            }
            k++;
        }
    }
    std::fill(matr->begin() + k, matr->end(), 0);
}

void switchMatrixStorageType(std::vector<double>* main_vec, int block_size, int block_in_row) {
    int iter = 0;
    std::vector<double> tmp_vec;
    tmp_vec.resize(main_vec->size());
    for (int i = 0; i < block_in_row; i++) {
        for (int l = 0; l < block_in_row; l++) {
            for (int j = 0; j < block_size; j++) {
                for (int k = 0; k < block_size; k++) {
                    tmp_vec[iter] = (*main_vec)[i * block_size * block_size * block_in_row +
                        j * block_size * block_in_row +
                        l * block_size + k];
                    iter++;
                }
            }
        }
    }
    std::swap((*main_vec), tmp_vec);
}

std::vector<double> getResOfFoxMult(std::vector<double> first_matr, std::vector<double> second_matr, int matr_size) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Checking if a matrix can be made using blocks
    if (!(std::fabs(sqrt(size) - round(sqrt(size))) < std::numeric_limits<double>::epsilon())) {
        throw std::string("Non-square number of processes");
    }

    // Perform calculations on one process if necessary
    if (matr_size < size || size == 1) {
        if (rank != 0) {
            return std::vector<double>();
        }
        std::vector<double> tmp_vec;
        tmp_vec.resize(matr_size * matr_size);
        getResOfMatrMult(first_matr.data(), second_matr.data(), tmp_vec.data(), matr_size);
        return tmp_vec;
    }

    // Data processing
    int block_size, blocks_in_row, new_matr_size;
    if (rank == 0) {
        block_size = static_cast<int>ceil(sqrt(double(matr_size * matr_size) / size));
        blocks_in_row = static_cast<int>round(sqrt(size));

        int new_matr_size = block_size * blocks_in_row;

        // Adding dummy zeros to matrix
        updateMatrToNewSize(&first_matr, matr_size, new_matr_size);
        updateMatrToNewSize(&second_matr, matr_size, new_matr_size);

        // Transform the matrix into linear blocks of information
        switchMatrixStorageType(&first_matr, block_size, blocks_in_row);
        switchMatrixStorageType(&second_matr, block_size, blocks_in_row);

        // Do not forget to change the size of the matrix
        matr_size = new_matr_size;
    }

    // Broadcast some information to all processes
    int old_matr_size = matr_size;
    MPI_Bcast(&matr_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&block_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&blocks_in_row, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int fixed_space = block_size * block_size;


    /* Creating Cart */


    // Additional data for creating a communicator
    const int ndims = 2;
    int dims[ndims];
    dims[0] = dims[1] = blocks_in_row;
    int periods[ndims];
    periods[0] = periods[1] = 0;

    // Creating a communicator
    MPI_Comm cart_comm;
    MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, false, &cart_comm);

    // Determining the rank of a process by its Cartesian coordinates
    int coords[ndims];
    MPI_Cart_coords(cart_comm, rank, ndims, coords);

    // Communicator for rows
    MPI_Comm row_comm;
    int subdims[] = { 0, 1 };
    MPI_Cart_sub(cart_comm, subdims, &row_comm);

    // Communicator for columns
    MPI_Comm column_comm;
    std::swap(subdims[0], subdims[1]);
    MPI_Cart_sub(cart_comm, subdims, &column_comm);


    /* Fox 's Algorithm */


    // Cij = sum(Ais * Bsi) where s belongs to the segment from 0 to the number of blocks
    std::vector<double> Ais;
    std::vector<double> Bsj;
    std::vector<double> Cij;
    Cij.resize(fixed_space);
    std::fill(Cij.begin(), Cij.end(), 0.0);

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(first_matr.data() + i * fixed_space, fixed_space, MPI_DOUBLE, i, 1, cart_comm);
            MPI_Send(second_matr.data() + i * fixed_space, fixed_space, MPI_DOUBLE, i, 2, cart_comm);
        }

        auto tmp_iter = first_matr.begin();
        Ais = std::vector<double>(tmp_iter, tmp_iter + fixed_space);

        tmp_iter = second_matr.begin();
        Bsj = std::vector<double>(tmp_iter, tmp_iter + fixed_space);
    } else {
        Ais.resize(fixed_space);
        Bsj.resize(fixed_space);
        MPI_Status status;
        MPI_Recv(Ais.data(), fixed_space, MPI_DOUBLE, 0, 1, cart_comm, &status);
        MPI_Recv(Bsj.data(), fixed_space, MPI_DOUBLE, 0, 2, cart_comm, &status);
    }

    for (int i = 0; i < blocks_in_row; i++) {
        std::vector<double> tmp_block(fixed_space);
        std::vector<double> tmp_res(fixed_space);
        int pivot = (coords[0] + i) % blocks_in_row;
        if (coords[1] == pivot) {
            for (int j = 0; j < fixed_space; j++) {
                tmp_block[j] = Ais[j];
            }
        }
        MPI_Bcast(tmp_block.data(), fixed_space, MPI_DOUBLE, pivot, row_comm);
        getResOfMatrMult(tmp_block.data(), Bsj.data(), tmp_res.data(), block_size);
        for (int j = 0; j < fixed_space; j++) {
            Cij[j] += tmp_res[j];
        }
        int dest = coords[0] - 1;
        if (coords[0] == 0) {
            dest = blocks_in_row - 1;
        }
        int source = coords[0] + 1;
        if (coords[0] == blocks_in_row - 1) {
            source = 0;
        }
        MPI_Status status;
        MPI_Sendrecv_replace(Bsj.data(), fixed_space, MPI_DOUBLE, dest, 0, source, 0, column_comm, &status);
    }

    std::vector<double> res(old_matr_size * old_matr_size);

    MPI_Gather(Cij.data(), fixed_space, MPI_DOUBLE, res.data(), fixed_space, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        switchMatrixStorageType(&res, block_size, blocks_in_row);
        return res;
    }

    return std::vector<double>();
}
