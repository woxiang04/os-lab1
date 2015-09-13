#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "kernel/message.h"
#include "types.h"
#include "adt/list.h"

typedef struct Message {
	pid_t src, dest;
	union {
		int type;
		int ret;
	};
	union {
		int i[5];
		struct {
			pid_t req_pid;
			int dev_id;
			void *buf;
			off_t offset;
			size_t len;
		};
	};
	ListHead list;
} Msg;

void send(pid_t dest, Msg *m);
void receive(pid_t src, Msg *m);

void init_msg(
    Msg* m,
    pid_t src,
    int type_return,
    pid_t r, int d, void *b, off_t o, size_t l);

#define INVALID_ID -1
//#define FAIL 0
//#define SUCC 1

// the following is a internal error message,
// should not put it in the following enum
#define NOT_DIR 1

// the error must correspond to the order of err in error.c
// e.g. err[- NO_SUCH] == "No such file or directory"
// @see set_error_msg()
typedef enum {
    SUCC = -1,
    FAIL = 0,
    FM_ERR,
    // no such directory or file
    NO_SUCH,
    IS_DIR,
    FILE_EXIST,
    PM_ERR,
    SEGMENTATION,
    MM_ERR,
    NO_MORE_MEMORY,
} Msg_res;

#endif /* __MESSAGE_H__ */
