// Copyright 2022 Gosteeva Ekaterina

#ifndef MODULES_TASK_2_GOSTEEVA_E_PRODUCERS_CONSUMERS_PRODUCERS_CONSUMERS_H_
#define MODULES_TASK_2_GOSTEEVA_E_PRODUCERS_CONSUMERS_PRODUCERS_CONSUMERS_H_

#define default_tag 0
#define peq_from_prod_tag 1
#define req_from_con_tag 3
#define con_expect_tag 2
#define con_on_tag 4
#define con_off_tag 5

#include <mpi.h>

int GetAccess(int num);
void Producer();
void Consumer();
bool is_close(int value);

#endif  // MODULES_TASK_2_GOSTEEVA_E_PRODUCERS_CONSUMERS_PRODUCERS_CONSUMERS_H_
