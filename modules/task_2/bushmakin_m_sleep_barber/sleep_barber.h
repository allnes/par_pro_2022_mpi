// Copyright 2022 Bushmakin Maksim
#ifndef MODULES_TASK_2_BUSHMAKIN_M_SLEEP_BARBER_SLEEP_BARBER_H_
#define MODULES_TASK_2_BUSHMAKIN_M_SLEEP_BARBER_SLEEP_BARBER_H_

const double busy_time = 0.08;
const double client_cooldown = 0.016;

void wait(double wtime);
void queue(int seat_count, int number_of_clients);
void customer();
void barber(int number_of_clients);

#endif  // MODULES_TASK_2_BUSHMAKIN_M_SLEEP_BARBER_SLEEP_BARBER_H_
