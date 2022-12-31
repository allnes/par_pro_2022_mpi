// Copyright 2022 Gosteeva Ekaterina

#include "../../../modules/task_2/gosteeva_e_producers_consumers/producers_consumers.h"

bool is_close(int value) {
    return value == 0;
}

int GetAccess(int num) {
    int buf = 0, req, res;
    int acc_to_prod = 0, acc_to_cun = 0, num_of_con_now = 0;
    MPI_Status MPI_status;
    while (num > 0) {
        MPI_Recv(&req, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &MPI_status);
        switch (MPI_status.MPI_TAG) {
        case peq_from_prod_tag:
            res = 0;
            if (!(is_close(acc_to_prod))) {
                MPI_Send(&res, 1, MPI_INT, MPI_status.MPI_SOURCE, default_tag, MPI_COMM_WORLD);
            } else {
                acc_to_prod++;
                acc_to_cun++;
                res = 1;
                MPI_Send(&res, 1, MPI_INT, MPI_status.MPI_SOURCE, default_tag, MPI_COMM_WORLD);
            }
            break;
        case con_expect_tag:
            res = 0;
            if (!(is_close(num_of_con_now))) {
                MPI_Send(&res, 1, MPI_INT, MPI_status.MPI_SOURCE, default_tag, MPI_COMM_WORLD);
            } else {
                res = 1;
                MPI_Send(&res, 1, MPI_INT, MPI_status.MPI_SOURCE, default_tag, MPI_COMM_WORLD);
                buf = req;
                acc_to_cun--;
                acc_to_prod--;
                num--;
            }
            break;
        case req_from_con_tag:
            res = 0;
            if (!(is_close(acc_to_cun))) {
                MPI_Send(&res, 1, MPI_INT, MPI_status.MPI_SOURCE, default_tag, MPI_COMM_WORLD);
            } else {
                num_of_con_now++;
                res = 1;
                MPI_Send(&res, 1, MPI_INT, MPI_status.MPI_SOURCE, default_tag, MPI_COMM_WORLD);
                MPI_Send(&buf, 1, MPI_INT, MPI_status.MPI_SOURCE, 4, MPI_COMM_WORLD);
            }
            break;
        case con_off_tag:
            num_of_con_now--;
            num--;
            break;
        default:
            break;
        }
    }
    return num;
}

void Producer() {
    int MPI_rank, buff, req = 0;
    MPI_Status MPI_status;
    MPI_Comm_rank(MPI_COMM_WORLD, &MPI_rank);
    buff = MPI_rank;

    do {
        MPI_Send(&req, 1, MPI_INT, 0, peq_from_prod_tag, MPI_COMM_WORLD);
        MPI_Recv(&req, 1, MPI_INT, 0, default_tag, MPI_COMM_WORLD, &MPI_status);
    } while (!req);
    req = 0;
    do {
        MPI_Send(&buff, 1, MPI_INT, 0, con_expect_tag, MPI_COMM_WORLD);
        MPI_Recv(&req, 1, MPI_INT, 0, default_tag, MPI_COMM_WORLD, &MPI_status);
    } while (!req);
}

void Consumer() {
    int MPI_rank, buff, req = 0;
    MPI_Status MPI_status;
    MPI_Comm_rank(MPI_COMM_WORLD, &MPI_rank);

    do {
        MPI_Send(&req, 1, MPI_INT, 0, req_from_con_tag, MPI_COMM_WORLD);
        MPI_Recv(&req, 1, MPI_INT, 0, default_tag, MPI_COMM_WORLD, &MPI_status);
    } while (!req);
    MPI_Recv(&buff, 1, MPI_INT, 0, con_on_tag, MPI_COMM_WORLD, &MPI_status);
    MPI_Send(&MPI_rank, 1, MPI_INT, 0, con_off_tag, MPI_COMM_WORLD);
}
