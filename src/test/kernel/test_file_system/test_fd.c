#include "kernel/message.h"
#include "kernel/process.h"
#include "kernel/manager/FM.h"
#include "kernel/manager/f_dir.h"

#include "lib/string.h"

#include "error.h"

void test_list(char *name);
int set_name_msg(char *name, int (*f)(Msg *));
static char first_doc[] = "test";
static char second_doc[] = "test2";
static char thi_doc[] = "test3";
static char fou_doc[] = "test4";
void test_create() {
    int res;
    /*
      res = set_name_msg(first_doc, make_dir);
      assert(res == SUCC);
      res = set_name_msg(first_doc, make_dir);
      assert(res == FILE_EXIST);
      res = set_name_msg(second_doc, make_dir);
      assert(res == SUCC);
      res = set_name_msg(thi_doc, make_dir);
      assert(res == SUCC);

      The upper code and the following code all cause
      exception #14, so
      1. it seems have something to do with number of file
      in a directory
      2. problem may in make_empty_file
     */
    res = set_name_msg(first_doc, create_file);
    assert(res == SUCC);
    res = set_name_msg(first_doc, create_file);
    assert(res == FILE_EXIST);
    res = set_name_msg(second_doc, create_file);
    assert(res == SUCC);
    res = set_name_msg(thi_doc, create_file);
    assert(res == SUCC);
    res = set_name_msg(fou_doc, create_file);
    assert(res == SUCC);
}

void test_delfile() {
    int res;
    res = set_name_msg(first_doc, delete_file);
    assert(res == SUCC);
    res = set_name_msg(first_doc, delete_file);
    assert(res == NO_SUCH);
    res = set_name_msg(second_doc, delete_file);
    assert(res == SUCC);
    res = set_name_msg(thi_doc, delete_file);
    assert(res == SUCC);
    res = set_name_msg(fou_doc, delete_file);
    assert(res == SUCC);
}

void test_create_del() {
    int count = 100000;
    int i;
    for (i = 0; i < count; i++) {
        test_create();
        test_delfile();
    }
}

static int set_fd_msg(int fd, int (*f)(Msg *)) {
    Msg m;
    m.ret = FM_ERR;
    m.buf = current;
    m.dev_id = fd;
    int res = f(&m);
    test_list(NULL);
    if (m.ret == SUCC) {
        return res;
    }
    assert(res == INVALID_FD_I);
    return m.ret;
}

static int test_open(char *name) {
    int fd;
    Msg m;
    m.ret = FM_ERR;
    m.buf = current;
    m.dev_id = (int)name;
    fd = open_file(&m);
    test_list(NULL);

    return fd;
}

static int test_close(int fd) {
    int res;
    res = set_fd_msg(fd, close_file);
    return res;
}


static int test_dup2(int old_fd, int new_fd) {
    int dup_fd;
    Msg m;
    m.ret = FM_ERR;
    m.i[2] = (int)current;
    m.i[0] = (int)old_fd;
    m.i[1] = (int)new_fd;
    dup_fd = dup2_file(&m);
    assert(m.ret == SUCC);
    test_list(NULL);

    return dup_fd;
}

void test_lseek(int fd) {
}

void test_read(int fd) {
}

void test_write(int fd) {
    // read from ram then write to shell.out, exit.out
}

void test_open_close() {
    int dup_fd;
    int fd = test_open(first_doc);
    // fd should be 4
    // 0 -- stdin, 1 -- stdout, 2 -- stderr, 3 -- cwd
    assert(fd == CWD + 1);
    dup_fd = set_fd_msg(fd, dup_file);
    assert(dup_fd == fd + 1);
    int res;
    res = test_dup2(fd, dup_fd + 1);
    assert(res == dup_fd + 1);
    // do some operations: read and write to file
    // close the duplicated file descriptor
    res = test_close(dup_fd);
    assert(res == dup_fd);
    res = test_close(dup_fd + 1);
    assert(res == dup_fd + 1);
    res = test_close(fd);
    assert(res == fd);
}