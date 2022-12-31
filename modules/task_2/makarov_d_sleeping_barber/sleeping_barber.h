// Copyright 2022 Makarov Danila
#ifndef MODULES_TASK_2_MAKAROV_D_SLEEPING_BARBER_SLEEPING_BARBER_H_
#define MODULES_TASK_2_MAKAROV_D_SLEEPING_BARBER_SLEEPING_BARBER_H_

#define WROOM_REQUEST 1
#define WROOM_RESPONSE 2
#define HAIRCUT 3

void barber(int client_count);
void client();
void waitingRoom(int seat_count, int runs);

#endif  // MODULES_TASK_2_MAKAROV_D_SLEEPING_BARBER_SLEEPING_BARBER_H_
