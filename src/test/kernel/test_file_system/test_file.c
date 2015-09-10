#include "kernel/message.h"
#include "kernel/process.h"
#include "kernel/manager/FM.h"
#include "kernel/manager/f_dir.h"

#include "lib/string.h"

#define SZ 256
/**
   test the initial state of hard-disk
 */
void test_list(char *name) {
    Msg m;
    int size = SZ / sizeof(Dir_entry);
    Dir_entry entry[size];
    init_msg(&m,
        current->pid,
        FM_lsdir,
        (int)entry, (int)name, current, INVALID_ID, SZ);
    m.ret = list_dir(&m);
    assert(m.ret != FAIL);
    size = m.ret;
    assert(strcmp(entry[0].filename, ".") == 0);
    assert(strcmp(entry[1].filename, "..") == 0);
    int i;
    for (i = 0; i < size; i++) {
        printk("%s, ", entry[i].filename);
    }

}

int set_mk_del_msg(char *name, int (*f)(Msg *)) {
    Msg m;
    m.buf = current;
    m.dev_id = (int)name;
    m.ret = f(&m);
    test_list(name);
    return m.ret;
}

void test_mkdir() {
    int res;
    char name[] = "first_dir";
    res = set_mk_del_msg(name, make_dir);
    assert(res != FAIL);
    char name2[] = "second_dir";
    res = set_mk_del_msg(name2, make_dir);
    assert(res != FAIL);
    // make a duplicate directory
    res = set_mk_del_msg(name2, make_dir);
    assert(res == FAIL);
    //char name3[] = "/third_dir";
    //set_mk_del_msg(name3, make_dir);
    test_list(NULL);
}

void test_deldir() {
    int res;
    char name[] = "first_dir";
    res = set_mk_del_msg(name, delete_file);
    assert(res != FAIL);
    res = set_mk_del_msg(name, delete_file);
    assert(res == FAIL);
    test_list(NULL);
}

void test_mk_del() {
    int count = 100000;
    int i;
    for (i = 0; i < count; i++) {
        test_mkdir();
        test_deldir();
    }
}