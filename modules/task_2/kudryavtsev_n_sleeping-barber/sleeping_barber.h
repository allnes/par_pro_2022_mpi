
#ifndef MODULES_TASK_2_KUDRYAVTSEV_N_SLEEPING-BARBER_SLEEPING_BARBER_H_
#define MODULES_TASK_2_KUDRYAVTSEV_N_SLEEPING-BARBER_SLEEPING_BARBER_H_

#define WROOM_REQUEST 1
#define WROOM_RESPONSE 2
#define HAIRCUT 3

void barb(int client_count);
void client();
void wait(int seat_count, int runs);

#endif  
