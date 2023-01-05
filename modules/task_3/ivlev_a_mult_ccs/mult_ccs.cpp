// Copyright 2022 Ivlev A
#include <mpi.h>
#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_3/ivlev_a_mult_ccs/mult_ccs.h"

matrix_ccs::matrix_ccs(int m_, int n_, int val_n_):
    m(m_), n(n_), val_n(val_n_) {
    values = new double[val_n];
    rows = new int[val_n];
    index = new int[n];
}

matrix_ccs::matrix_ccs(const matrix_ccs &c):
    m(c.m), n(c.n), val_n(c.val_n) {
    values = new double[val_n];
    rows = new int[val_n];
    index = new int[n];

    for (int i = 0; i < val_n; i++) {
        values[i] = c.values[i];
        rows[i] = c.rows[i];
    }

    for (int i = 0; i < n; i++) {
        index[i] = c.index[i];
    }
}

void matrix_ccs::operator = (const matrix_ccs &c) {
    this->~matrix_ccs();
    n = c.n;
    m = c.m;
    val_n = c.val_n;
    values = new double[val_n];
    rows = new int[val_n];
    index = new int[n];

    for (int i = 0; i < val_n; i++) {
        values[i] = c.values[i];
        rows[i] = c.rows[i];
    }

    for (int i = 0; i < n; i++) {
        index[i] = c.index[i];
    }
}

bool operator== (const matrix_ccs &b, const matrix_ccs &c) {
    for (int i = 0; i < b.val_n; i++) {
        if (b.values[i] != c.values[i] || b.rows[i] != c.rows[i]) {
            return false;
        }
    }

    for (int i = 0; i < b.n; i++) {
        if (b.index[i] != c.index[i]) {
            return false;
        }
    }

    return true;
}

double matrix_ccs::get(int m_, int n_) {
    if (index[n_] == val_n) {
        return(0.);
    } else {
        int r;
        if (n_ == n-1) {
            r = val_n;
        } else {
            r = index[n_+1];
        }

        for (int i = index[n_]; i < r; i++) {
            if (rows[i] == m_) {
                return values[i];
            }
        }

        return(0.);
    }
}

void matrix_ccs::create_rand() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, RAND_MAX);

    int count = 0;
    for (int i = 0; i < val_n; i++) {
        double temp = static_cast<double>(dist(gen))/ RAND_MAX;

        if (i == 0) {
            index[0] = 0;
        } else {
            if (count < n && (rows[i-1] == m-1 || temp > 0.75)) {
                count += 1;
                index[count] = i;
            }
        }

        values[i] = temp*100;

        if (index[count] == i) {
            rows[i] = dist(gen) % m;
        } else {
            if (rows[i-1] == m-2) {
                rows[i] = m - 1;
            } else {
                rows[i] = rows[i-1] + 1 + dist(gen) % (m - rows[i-1] - 1);
            }
        }
    }

    while (count < n-1) {
        count += 1;
        index[count] = val_n;
    }
}

matrix_ccs matrix_ccs::mult(matrix_ccs b) {
    std::vector<double> temp_val;
    std::vector<int> temp_row;
    std::vector<int> temp_index;

    temp_index.push_back(0);
    for (int k = 0; k < b.n; k++) {
        for (int j = 0; j < m; j++) {
            double sum = 0;
            for (int i = 0; i < n; i++) {
                sum += get(j, i) * b.get(i, k);
            }
            if (sum != 0) {
                temp_val.push_back(sum);
                temp_row.push_back(j);
            }
        }
        if (temp_index.back() != static_cast<int>(temp_val.size())) {
            temp_index.push_back(static_cast<int>(temp_val.size()));
        } else {
            temp_index.push_back(temp_index.back());
        }
    }

    matrix_ccs c(m, b.n, static_cast<int>(temp_val.size()));
    for (int i = 0; i < c.val_n; i++) {
        c.values[i] = temp_val[i];
        c.rows[i] = temp_row[i];
    }

    for (int i = 0; i < c.n; i++) {
        c.index[i] = temp_index[i];
    }

    return c;
}

matrix_ccs matrix_ccs::mpi_mult(matrix_ccs b) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int val_n_b, block_size, last_block_size;
    int max_size = size;
    if (m >= size) {
        if (size != 1) {
            block_size = m/(size-1);
            last_block_size = m%(size-1);
            if (last_block_size == 0) {
                block_size -= 1;
                last_block_size = size-1;
            }
        } else {
            return mult(b);
        }

    } else {
        block_size = 1;
        last_block_size = 1;
        max_size = m;
    }

    if (rank == 0) {
        for (int i = 1; i < max_size; i++) {
            MPI_Send(values, val_n, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(rows, val_n, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(index, n, MPI_INT, i, 2, MPI_COMM_WORLD);

            matrix_ccs d = b.get_column(last_block_size
                + (i-1)*block_size, block_size);

            int temp[1] = {d.val_n};
            MPI_Send(temp, 1, MPI_INT, i, 6, MPI_COMM_WORLD);
            MPI_Send(d.values, d.val_n, MPI_DOUBLE, i, 3, MPI_COMM_WORLD);
            MPI_Send(d.rows, d.val_n, MPI_INT, i, 4, MPI_COMM_WORLD);
            MPI_Send(d.index, d.n, MPI_INT, i, 5, MPI_COMM_WORLD);
        }

        matrix_ccs d = this->mult(b.get_column(0, last_block_size));

        for (int i = 1; i < max_size; i++) {
            MPI_Status status;
            int temp1[1] = {0};
            MPI_Recv(temp1, 1, MPI_INT, i, 7, MPI_COMM_WORLD, &status);
            int val_n_c = temp1[0];
            matrix_ccs c(m, block_size, val_n_c);

            MPI_Recv(c.values, val_n_c, MPI_DOUBLE, i, 8,
                MPI_COMM_WORLD, &status);
            MPI_Recv(c.rows, val_n_c, MPI_INT, i, 9, MPI_COMM_WORLD, &status);
            MPI_Recv(c.index, block_size, MPI_INT, i, 10,
                MPI_COMM_WORLD, &status);

            d = d.add_column_matrix(c);
        }

        return d;
    } else {
        if (rank < max_size) {
            MPI_Status status;
            MPI_Recv(values, val_n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(rows, val_n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(index, n, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);

            int temp[1] = {0};
            MPI_Recv(temp, 1, MPI_INT, 0, 6, MPI_COMM_WORLD, &status);
            val_n_b = temp[0];
            matrix_ccs b(n, block_size, val_n_b);

            MPI_Recv(b.values, val_n_b, MPI_DOUBLE, 0, 3,
                MPI_COMM_WORLD, &status);
            MPI_Recv(b.rows, val_n_b, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
            MPI_Recv(b.index, block_size, MPI_INT, 0, 5,
                MPI_COMM_WORLD, &status);

            matrix_ccs c = this->mult(b);

            int temp1[1] = {c.val_n};
            MPI_Send(temp1, 1, MPI_INT, 0, 7, MPI_COMM_WORLD);
            MPI_Send(c.values, c.val_n, MPI_DOUBLE, 0, 8, MPI_COMM_WORLD);
            MPI_Send(c.rows, c.val_n, MPI_INT, 0, 9, MPI_COMM_WORLD);
            MPI_Send(c.index, c.n, MPI_INT, 0, 10, MPI_COMM_WORLD);
        }
        return matrix_ccs(1, 1, 1);
    }
}

matrix_ccs matrix_ccs::get_column(int start, int col) {
    std::vector<double> temp_val;
    std::vector<int> temp_row;
    std::vector<int> temp_index;

    int end = start+col;
    for (int i = start; i < end; i++) {
        temp_index.push_back(index[i] - index[start]);
    }
    if (end == n) {
        end = val_n;
    } else {
        end = index[end];
    }

    for (int i = index[start]; i < end; i++) {
        temp_val.push_back(values[i]);
        temp_row.push_back(rows[i]);
    }

    matrix_ccs c(m, col, static_cast<int>(temp_val.size()));
    for (int i = 0; i < c.val_n; i++) {
        c.values[i] = temp_val[i];
        c.rows[i] = temp_row[i];
    }

    for (int i = 0; i < c.n; i++) {
        c.index[i] = temp_index[i];
    }

    return c;
}

matrix_ccs matrix_ccs::add_column_matrix(matrix_ccs c) {
    matrix_ccs d(m, n+c.n, val_n+c.val_n);

    for (int i = 0; i < d.val_n; i++) {
        if (i < val_n) {
            d.values[i] = values[i];
            d.rows[i] = rows[i];
        } else {
            d.values[i] = c.values[i-val_n];
            d.rows[i] = c.rows[i-val_n];
        }
    }

    for (int i = 0; i < d.n; i++) {
        if (i < n) {
            d.index[i] = index[i];
            d.index[i] = index[i];
        } else {
            d.index[i] = c.index[i-n]+val_n;
            d.index[i] = c.index[i-n]+val_n;
        }
    }

    return d;
}

void matrix_ccs::print() {
    std::cout << "val: ";
    for (int i = 0; i < val_n; i++) {
        std::cout << values[i] << ' ';
    }
    std::cout << '\n';

    std::cout << "row: ";
    for (int i = 0; i < val_n; i++) {
        std::cout << rows[i] << ' ';
    }
    std::cout << '\n';

    std::cout << "ind: ";
    for (int i = 0; i < n; i++) {
        std::cout << index[i] << ' ';
    }
    std::cout << '\n';
}

void matrix_ccs::all_print() {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << round(get(i, j) * 10000) / 10000 << '\t';
        }
        std::cout << '\n';
    }
}

matrix_ccs::~matrix_ccs() {
    delete [] values;
    delete [] rows;
    delete [] index;
}
