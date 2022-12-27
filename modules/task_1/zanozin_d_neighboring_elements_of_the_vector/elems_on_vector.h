#ifndef MODULES_TASK_1_POPOV_A_CLOSE_ELEMS_ON_VECTOR_CLOSE_ELEMS_ON_VECTOR_H_
#define MODULES_TASK_1_POPOV_A_CLOSE_ELEMS_ON_VECTOR_CLOSE_ELEMS_ON_VECTOR_H_

#include <vector>

std::vector<int> getRandVect(int size);

int getParallelOps(const std::vector<int>& vect_glob);
int getSeriallOps(const std::vector<int>& vect);

#endif