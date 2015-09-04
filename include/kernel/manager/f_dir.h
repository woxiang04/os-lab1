#ifndef __F_DIR_H__
#define __F_DIR_H__

#define MAX_FILENAME_LEN 28

#include "inode.h"

typedef struct {
	char filename[MAX_FILENAME_LEN];
    // inode offset
	inode_t inode_off;
} Dir_entry;


#define MAX_DIR_DEPTH 16

extern const char *const current_dir;
extern const char *const father_dir;
extern const char *const default_cwd_name;

#include "error.h"

// the error must correspond to the order of err
// @see set_error_msg()
#define NO_SUCH 0
#define NOT_DIR 1


uint32_t get_dir_e_off(iNode *dir, inode_t aim);

#include "lib/string.h"
static inline
int invalid_filename(char *name) {
    if (name == NULL || strlen(name) >= MAX_FILENAME_LEN) {
        return 1;
    }
    return 0;
}

inode_t file_path(inode_t cwd, char *name);

#include "kernel/message.h"

int list_dir(Msg *m);
int ch_dir(Msg *m);
size_t write_file(Msg *m);
size_t n_read_file(Msg *m);
int delete_file(Msg *m);
int make_dir(Msg *m);
int create_file(Msg *m);


#endif /* __F_DIR_H__ */