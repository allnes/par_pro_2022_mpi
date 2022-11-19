// Copyright 2022 Bulgakov Daniil

#include "../../../modules/task_1/bulgakov_d_sentence_sum/sentence_sum.h"

#include <mpi.h>
#include <string>
#include <random>

#include <iostream>

// #define debug

// Find line split indexes for each proc
// split index shouldn`t be on ?!. chars cause of re-addition to cases
// "!?" "?!" "..." and so on.
//  leftborder < line <= rightborder
std::vector<int> parseTextToIndexes(std::string str, int proc_num) {
    std::vector<int> vectorInd;
    int splitSize = ceil(str.length() / static_cast<float>(proc_num));
    int splitCount = proc_num;
    const int minSplit = 5;

    vectorInd.reserve(proc_num + 1);
    vectorInd.push_back(0);

    if (static_cast<int>(str.length()) < minSplit) {
        vectorInd.push_back(str.length());
        for (int i = 1; i < proc_num; i++)
            vectorInd.push_back(-1);
        return vectorInd;
    }
    if (splitSize <= minSplit) {
        splitSize = minSplit;
        splitCount = str.length() / minSplit;
    }

    for (int i = splitSize; i < static_cast<int>(str.length()) &&
            static_cast<int>(vectorInd.size()) < splitCount; i += splitSize) {
        int currind = i;

        while (currind < static_cast<int>(str.length()) && currind < i + splitSize &&
            (str[currind] == '?' || str[currind] == '!' || str[currind] == '.') ) {
            currind++;
        }

        vectorInd.push_back(currind);
    }
    vectorInd.push_back(str.length());
    for (int i = vectorInd.size(); i < static_cast<int>(vectorInd.capacity()); i++) {
        vectorInd.push_back(-1);
    }

    return vectorInd;
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

int parallelSentenceCount(std::string str) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> vectorIndeces;
    if (rank == 0) {
        vectorIndeces = parseTextToIndexes(str, size);
        for (int proc = 1; proc < size; proc++) {
            if (vectorIndeces[proc + 1] == -1) {
                // In case there were more proccesors than str.length()
                // or each proccees should compute less than 10 chars
                // programm wont use some of them.
                MPI_Send(std::string("NOT USED").c_str(), 9, MPI_CHAR, proc, 0, MPI_COMM_WORLD);
            } else {
                int meslen = vectorIndeces[proc + 1] - vectorIndeces[proc];
                if (meslen == 0) meslen = 1;
                std::string mes = str.substr(vectorIndeces[proc], meslen);

                #ifdef debug
                std::cout  << " SEND rank " << proc << " " << mes << std::endl;
                #endif

                MPI_Send(mes.c_str(), meslen, MPI_CHAR, proc, 0, MPI_COMM_WORLD);
            }
        }
    }
    std::string local_str;

    if (rank == 0) {
        local_str = str.substr(0, vectorIndeces[1]);
    } else {
        MPI_Status status;
        int maxbuffsize = (str.length() / size) * 2 + 1 >= 25 ? (str.length() / size) * 2 + 1 : 25;
        char * buff = new char[maxbuffsize];
        MPI_Recv(buff, maxbuffsize, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        local_str = std::string(buff);
    }
    #ifdef debug
    std::cout  << "GET rank " << rank << " " << local_str << std::endl;
    #endif
    int global_sum = 0;
    int local_sum = computeSenteceCount(local_str);

    #ifdef debug
    if (local_sum > 0) std::cout << "RANKK" << rank << std::endl;
    #endif

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return global_sum;
}


