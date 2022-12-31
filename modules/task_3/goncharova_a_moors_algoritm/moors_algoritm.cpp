// Copyright Anna Goncharova
#include <mpi.h>
#include <random>

#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include "../../../modules/task_3/goncharova_a_moors_algoritm/moors_algoritm.h"

static int offset = 0;
const int INF = 2000000000;

std::vector<int> getRandomGraph(int size) {
    std::mt19937 gen;
    std::uniform_int_distribution<int> gen2(0, 10000);
    gen.seed((unsigned)time(0) + ++offset);
    std::vector<int> g(size * size);
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            g[j + i * size] = gen2(gen);
            if (g[j + i * size] == 0) g[j + i * size] = INF;
            if (i == j) g[j + i * size] = 0;
        }
    return g;
}

std::vector<int> Transpose(const std::vector<int>& g, int n) {
    std::vector<int> res(n * n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            res[i * n + j] = g[j * n + i];
    return res;
}

std::vector<int> ParallelMoor(const std::vector<int>& g, int source,
                                                        int* flag) {
    const int minus_inf = -1000000000;

    int size, rank;
    int n = static_cast<int>(sqrt(static_cast<int>(g.size())));
    std::vector<int> d(n, INF);
    d[source] = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = n / size;
    const int rem = n % size;
    int tmp = delta + (rank < rem ? 1 : 0);
    std::vector<int> loc(tmp * n);
    std::vector<int> loc_d(tmp, INF);

    if (loc.size() == 0)
        loc.resize(1);

    std::vector<int> sendcounts(size);
    std::vector<int> displs(size);
    std::vector<int> sendcounts_d(size);
    std::vector<int> displs_d(size);

    displs[0] = displs_d[0] = 0;
    for (int i = 0; i < size; ++i) {
        sendcounts[i] = (delta + (i < rem ? 1 : 0)) * n;
        sendcounts_d[i] = delta + (i < rem ? 1 : 0);
        if (i > 0) {
            displs[i] = (displs[i - 1] + sendcounts[i - 1]);
            displs_d[i] = displs_d[i - 1] + sendcounts_d[i - 1];
        }
    }

    int root = -1;
    for (int i = 0; i < size - 1; ++i)
        if (source >= displs[i] / n)
            root++;
    if (rank == root)
        loc_d[source - displs[rank] / n] = 0;

    std::vector<int> send;
    if (rank == 0)
        send = Transpose(g, n);

    MPI_Scatterv(send.data(), sendcounts.data(), displs.data(),
        MPI_INT, &loc[0], tmp * n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < n - 1; ++i) {
        for (int k = 0; k < sendcounts_d[rank]; ++k) {
            for (int j = 0; j < n; ++j)
                if ((d[j] < INF) && (loc[k * n + j] < INF))
                    if (loc_d[k] > d[j] + loc[k * n + j])
                        loc_d[k] = std::max(d[j] + loc[k * n + j], minus_inf);
        }
        MPI_Allgatherv(loc_d.data(), tmp, MPI_INT, d.data(),
            sendcounts_d.data(), displs_d.data(), MPI_INT, MPI_COMM_WORLD);
    }

    if (flag) {
        int flag2 = 0;
        *flag = 0;
        for (int k = 0; k < sendcounts_d[rank]; ++k)
            for (int j = 0; j < n; ++j)
                if ((d[j] < INF) && (loc[k * n + j] < INF)) {
                    if (loc_d[k] > d[j] + loc[k * n + j]) {
                        loc_d[k] = minus_inf;
                        flag2 = 1;
                    }
                }
        MPI_Reduce(&flag2, flag, 1, MPI_INT, MPI_LOR, 0, MPI_COMM_WORLD);
        MPI_Allgatherv(loc_d.data(), tmp, MPI_INT, d.data(),
            sendcounts_d.data(), displs_d.data(), MPI_INT, MPI_COMM_WORLD);
    }
    return d;
}

std::vector<int> Moors_algorithm(const std::vector<int>& g, int source,
                                                            int* flag) {
    const int minus_inf = -1000000000;

    int n = static_cast<int>(sqrt(static_cast<int>(g.size())));
    if (source < 0 || source >= n)
        throw - 1;
    std::vector<int> d(n, INF);
    d[source] = 0;

    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                if ((d[j] < INF) && (g[k + j * n] < INF))
                    if (d[k] > d[j] + g[k + j * n])
                        d[k] = std::max(d[j] + g[k + j * n], minus_inf);

    if (flag) {
        *flag = 0;
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                if ((d[j] < INF) && (g[k + j * n] < INF)) {
                    if (d[k] > d[j] + g[k + j * n]) {
                        d[k] = minus_inf;
                        *flag = 1;
                    }
                }
    }
    return d;
}
