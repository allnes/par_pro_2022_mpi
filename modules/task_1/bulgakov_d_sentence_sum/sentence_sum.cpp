// Copyright 2022 Bulgakov Daniil

#include "../../../modules/task_1/bulgakov_d_sentence_sum/sentence_sum.h"

#include <mpi.h>
#include <string>
#include <random>

#include <iostream>

// #define DEBUG

std::string getRandomString(int len) {
    std::string res = "";
    static const char chars[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::random_device dev;
    std::mt19937 gen(dev());
    for (int i = 0; i < len; i++) { res += chars[gen() % (sizeof(chars) - 1)]; }
    return res;
}

std::string getRandomText(int word_count) {
    std::string text = "";
    int str_end_prob = 5;
    static const std::string str_split[3] = {"?", "!", "."};

    std::random_device dev;
    std::mt19937 gen(dev());


    bool is_last_ended = false;
    int pr;
    for (int i = 0; i < word_count; i++) {
        pr = gen() % str_end_prob;

        if (i == 0) {text += getRandomString(gen() % 10 + 2); continue;}
        if (i == word_count - 2) {text += " " + getRandomString(gen() % 10 + 2); continue;}
        if (i == word_count - 1) {text += str_split[gen() % 3]; break;}


        if (pr == str_end_prob - 1 && !is_last_ended) {
            text += str_split[gen() % 3];
            is_last_ended = true;
        } else {
            text += " " + getRandomString(gen() % 10 + 2);
            is_last_ended = false;
        }
    }
    return text;
}


int getSequentialOperations(std::string str) {
    int count = 0;
    std::string str_end = ".?!";
    #ifdef DEBUG
    std::string found;
    #endif
    for (int i = 0; i < static_cast<int>(str.length()); i++) {
        if (str_end.find(str[i]) == '.' ||
        str_end.find(str[i]) == '?'||
        str_end.find(str[i]) == '!') {
            count++;
            #ifdef DEBUG
            found += str[i];
            #endif
        }
    }
    #ifdef DEBUG
    std::cout << "FOUND :" << found << std::endl;
    #endif
    return count;
}

int getParallelOperations(std::string global_str, int str_len) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = str_len / size;

    if (rank == 0) {
        #ifdef DEBUG
        std::cout << "DELTA = " << delta << std::endl;
        std::cout << "TEXT_ALL = " << global_str << std::endl;
        std::cout << "RANK : " << 0 << "TEXT " << global_str.substr(0, delta) << std::endl;
        #endif
        for (int proc = 1; proc < size - 1; proc++) {
            MPI_Send(global_str.substr(proc * delta).c_str(), delta, MPI_CHAR, proc, 0 , MPI_COMM_WORLD);
            #ifdef DEBUG
            std::cout << "RANK : " << proc << "TEXT " << global_str.substr(proc * delta, delta) << std::endl; 
            #endif
        }
        if (size > 1) {
            MPI_Send(global_str.substr((size - 1) * delta).c_str(), 
                delta + str_len % size, MPI_CHAR, size - 1, 0 , MPI_COMM_WORLD);    
            #ifdef DEBUG
            std::cout << "RANK : " << (size - 1) << "TEXT " << 
                global_str.substr((size - 1) * delta, delta + str_len % size) << std::endl;
            #endif
        }
    }

    std::string local_str;
    if (rank == 0) {
        local_str = global_str.substr(0, delta);
    } else {
        MPI_Status status;
        // std::cout << "HERE" << std::endl;
        char* buff = new char[delta + 10];
        MPI_Recv(buff, delta + 10, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        local_str = std::string(buff);
    }

    int global_sum = 0;
    int local_sum = getSequentialOperations(local_str);
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_sum;
}



