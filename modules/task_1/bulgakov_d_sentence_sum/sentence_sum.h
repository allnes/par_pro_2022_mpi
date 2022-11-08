#ifndef _MODULES_TASK_1_BULGAKOV_D_SENTENCE_SUM_OPS_MPI_H
#define _MODULES_TASK_1_BULGAKOV_D_SENTENCE_SUM_OPS_MPI_H

#include "string"

std::string genRandomString(int len);

std::string getRandomText(int word_count);

int getParallelOperations(std::string global_str, int str_len);

int getSequentialOperations(std::string str);

#endif // _MODULES_TASK_1_BULGAKOV_D_SENTENCE_SUM_OPS_MPI_H