#ifndef __PIPE_H__
#define __PIPE_H__

#define LEN 8
#define 256

typedef struct {
    int front, rear;
    pid_t read_wait_queue[LEN], write_wait_queue[LEN];
    uint8_t buf[BUF_LEN];
} Pipe;

int pipe(int pipefd[2]);

#endif /* __PIPE_H__ */