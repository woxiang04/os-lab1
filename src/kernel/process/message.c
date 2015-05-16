#include "kernel/message.h"
#include "kernel/process.h"
#include "kernel/semaphore.h"
#include "lib/string.h"



void add_message(PCB* p, Msg* m) {
    list_add_after(&(p->mes), &(m->list));
}

static int has_message(PCB* p, pid_t id) {
    ListHead* head = &(p->mes);
    ListHead* ptr = NULL;
    Msg* tmp = NULL;
    list_foreach(ptr, head) {
        assert(ptr != NULL);
        tmp = list_entry(ptr, Msg, list);
        assert(tmp != NULL);
        if (tmp->src == id || id == ANY) {
            break;
        }
    }
    return tmp != NULL;
}

static Msg* find_message(PCB* p, pid_t id) {
    ListHead* head = &(p->mes);
    ListHead* ptr = NULL;
    Msg* tmp = NULL;
    list_foreach(ptr, head) {
        assert(ptr != NULL);
        tmp = list_entry(ptr, Msg, list);
        assert(tmp != NULL);
        if (tmp->src == id || id == ANY) {
            list_del(&(tmp->list));
            break;
        }
    }
    return tmp;
}
/**
   get a message from process p's message list, with the right
   id, then copy to m.
 */
void get_message(PCB* p, pid_t id, Msg* m) {
    // find and delete aim message
    Msg *tmp = find_message(p, id);
    assert(tmp != NULL);
    // copy to m
    memcpy(m, tmp, sizeof(Sem));
}

void send(pid_t dest, Msg *m) {
    /**
       TODO add semaphore or lock
     */
    PCB* de = fetch_pcb(dest);
    Sem* s = &(de->mes_lock);
    P(s);
    add_message(de, m);
    V(s);
    wake_up(de);
    //V(&full);
}

void receive(pid_t src, Msg *m) {
    Sem* s = &(current->mes_lock);
    if (!has_message(current, src)) {// no such message
        // go to sleep
        sleep();
    }
    P(s);
    get_message(current, src, m);
    V(s);
}