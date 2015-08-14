#ifndef __TIME_H__
#define __TIME_H__

#define HZ        100

typedef struct Time {
	int year, month, day;
	int hour, minute, second;
} Time;

enum {
    NEW_TIMER,
};

inline long get_jiffy();

void get_time(Time *tm);

extern int TIMER;

typedef struct {
    int time;
    int pid;
} Timer;

void set_timer(Timer *t, int time, int pid);
// implemented in timer_sys.c

#include "kernel/message.h"

int kwait(Msg *m);
void update_timer();

extern int d_timer;

#endif
