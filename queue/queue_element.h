#include "list.h"

#ifndef __QUEUE_ELEMENT_H__
#define __QUEUE_ELEMENT_H__
struct queue_element {

        int integer_;
        struct list_head *head_;
};

typedef struct queue_element queue_element_t;
#endif
