// Copyright 2022 Bulgakov Daniil

#include "../../../modules/task_1/bulgakov_d_sentence_sum/sentence_sum.h"

#include <mpi.h>
#include <string>
#include <random>

#include <iostream>

// #define debug

// Split string to pieces by procs
std::vector<std::string> parseText(std::string str, int proc_num) {
    std::vector<std::string> vectorParts;
    int splitSize = static_cast<int>(ceil(str.length() / static_cast<float>(proc_num)));
    int splitCount = proc_num;
    const int minSplit = 5;


    if (static_cast<int>(str.length()) < minSplit) {
        vectorParts.push_back(str);
        return vectorParts;
    }
    if (splitSize < minSplit) {
        splitSize = minSplit;
        splitCount = ceil(str.length() / minSplit);
    }

    int strpartlen = splitSize + 4;

    int pref = 0;
    for (int i = 1; i < splitCount + 1; i++) {
        int curreof = (i) * splitSize > str.length() ? str.length() : (i) * splitSize;
        while (curreof < static_cast<int>(str.length()) && curreof < (i + 1) * splitSize &&
            (str[curreof] == '?' || str[curreof] == '!' || str[curreof] == '.') ) {
            curreof++;
        }
        std::string out = str.substr(pref, curreof - pref);
        if (out.length() < strpartlen) {
            out.append(strpartlen - out.length(), ' ');
        }
        vectorParts.push_back(out);
        pref = curreof;
        if (curreof >= str.length() - 1) break;
    }

    return vectorParts;
}

// isDelimStarted is boolean value that means that we are
// at the end of line and the following special characters before
// space or alphabetic symbol contains in
// delimChatCases are a part of that ending line
int computeSenteceCount(std::string str) {
    int cnt = 0;
    std::string delimCharCases = "?!.";
    bool isDelimStarted = false;

    for (int i = 0; i < static_cast<int>(str.length()); i++) {
        if (delimCharCases.find(str[i]) < delimCharCases.length()) {
            isDelimStarted = true;
        } else {
            if (isDelimStarted) {
                isDelimStarted = false;
                cnt++;
            }
        }
    }

    if (isDelimStarted) cnt++;

    return cnt;
}

int parallelSentenceCount(const std::string& str) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int mes_len, num_used_procs;
    std::vector<std::string> vectorText;

    if (rank == 0) {
        vectorText = parseText(str, size);
        num_used_procs = vectorText.size();
        mes_len = vectorText[0].length();
        for (int proc = 1; proc < vectorText.size(); proc++) {
            std::string mes = vectorText[proc];

            #ifdef debug
            std::cout  << " SEND rank " << proc << " " << mes << std::endl;
            #endif
            MPI_Send(mes.c_str(), mes.length(), MPI_CHAR, proc, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&mes_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&num_used_procs, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::string local_str;

    if (rank == 0) {
        local_str = vectorText[0];
    } else if (rank < num_used_procs) {
        MPI_Status status;
        int maxbuffsize = mes_len + 1;
        char * buff = new char[maxbuffsize];
        MPI_Recv(buff, maxbuffsize, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);

        local_str = std::string(buff);
        local_str = local_str.substr(0, mes_len);
        #ifdef debug
        std::cout  << "GET rank " << rank << " " << local_str << std::endl;
        #endif
    }

    int global_sum = 0;
    int local_sum = computeSenteceCount(local_str);

    #ifdef debug
    if (local_sum > 0) std::cout << "RANKK" << rank << std::endl;
    #endif

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return global_sum;
}

