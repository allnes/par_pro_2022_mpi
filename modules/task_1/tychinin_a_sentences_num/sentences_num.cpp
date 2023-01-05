// Copyright 2022 Tychinin Alexey

#include "../../../modules/task_1/tychinin_a_sentences_num/sentences_num.h"
#include <mpi.h>

#include <cassert>
#include <ctime>
#include <random>
#include <string>
#include <vector>

namespace {
    constexpr char ALPHABET[] = " .,?!123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    constexpr size_t ALPHABET_SIZE = sizeof(ALPHABET) / sizeof(char);

    int countSentencesSeq(const std::string& str) {
        if (str.size() == 0) {
            return 0;
        }

        int n = 0;
        for (const char ch : str) {
            if (ch == '.' || ch == '?' || ch == '!') {
                n++;
            }
        }
        return n;
    }
}  // namespace

namespace task1 {
    std::string generateSentence(const size_t chNum) {
        std::mt19937 r_gen(std::time(nullptr));
        std::string res(chNum, ' ');
        for (char& c : res) {
            c = ALPHABET[r_gen() % ALPHABET_SIZE];
        }
        return res;
    }

    int countSentences(const std::string& str, const bool toParallel, const size_t chNum) {
        if (!toParallel) {
            return countSentencesSeq(str);
        }
        std::string str_cp = str;
        if (chNum == 0 || str.empty()) {
            return 0;
        }
        int proc_count, proc_rank;
        MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
        MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

        const int delta = chNum / proc_count;
        const int remain = chNum - proc_count * delta;
        if (proc_rank == 0) {
            for (int proc = 1; proc < proc_count; proc++) {
                MPI_Send(str_cp.data() + remain + proc * delta, delta, MPI_CHAR, proc, 0, MPI_COMM_WORLD);
            }
        }
        std::string local_str;
        if (proc_rank == 0) {
            local_str = std::string(str_cp.cbegin(),
                                    str_cp.cbegin() + delta + remain);
        } else {
            std::vector<char> part_line(delta);
            MPI_Status stat;
            MPI_Recv(&part_line[0], delta, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &stat);
            local_str = std::string(part_line.cbegin(), part_line.cend());
        }

        int gcnt = 0;
        int lcnt = countSentencesSeq(local_str);
        MPI_Reduce(&lcnt, &gcnt, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        return gcnt;
    }
}  // namespace task1
