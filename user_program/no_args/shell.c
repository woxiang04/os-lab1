#include "kernel/syscall.h"
#include "sys_call/io/io.h"
#include "lib/string.h"


#define BUF_SZ 256
#define MAX_PARAMETER_NR 10


int entry() {
    char cmd[BUF_SZ], copy[BUF_SZ];
    char *save[MAX_PARAMETER_NR] = {0};
    int filename = -1;
    int pid, count, res;
    while(1) {
        prompt();
        memset(cmd, 0, BUF_SZ);
        read_line(cmd, BUF_SZ);
        memcpy(copy, cmd, BUF_SZ);
        count = split(copy, ' ', save);
        assert(MAX_PARAMETER_NR >= count);
        if (count <= 1) {
            printf("Unknown command: %s\n", cmd);
            continue;
        }
        filename = to_int(save[0]);
        // TODO check file existence and whether it is executable -- many be checked by exec
		// TODO add redirect
        if((pid = fork()) == 0) {
			// no thread will receive the response of exec, so it failure should be known by waitpid
            exec(filename, save[1] - copy + cmd);
        }
        else {
			// if exec fail, waitpid will fail
            res = waitpid(pid);
            if (res == 0) {
                printf("Unknown command or wrong args: %s\n", cmd);
                continue;
            }

        }
    }
    return 0;
}