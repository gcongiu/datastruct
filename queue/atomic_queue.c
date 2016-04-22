#include <stdio.h>
#include "atomic_queue.h"
#include "list.h"

#define CHECK_PTR(ptr)           \
        do {                     \
                if (ptr == NULL) \
                        return;  \
        } while(0)

struct atomic_queue {

        struct list_head head_;
        int size_;
};

void Atomic_queue_init(atomic_queue_t *q) {

        queue_element_t *dummy;
        *q = (struct atomic_queue *)malloc(sizeof(struct atomic_queue));
        INIT_LIST_HEAD(&((*q)->head_));

        /* add dummy element */
        dummy = (queue_element_t *)malloc(sizeof(queue_element_t));
        list_add_tail(&(dummy->head_), &(*q)->head_);
        (*q)->size_ = 0;
}

void Atomic_queue_fini(atomic_queue_t *q) {

        queue_element_t *e;

        CHECK_PTR(*q);

        /* free dummy element */
        e = list_entry((*q)->head_.prev, queue_element_t, head_);
        free(e);

        /* free queue */
        free(*q);
}

void Atomic_queue_push(atomic_queue_t q, queue_element_t *e) {

        queue_element_t *dummy;

        CHECK_PTR(q);

        /* get old dummy element */
        dummy = list_entry(q->head_.prev, queue_element_t, head_);

        /* copy e to dummy */
        dummy->integer_ = e->integer_;

        /* e becomes the new dummy */
        list_add_tail(&(e->head_), &(q->head_));

        /* updated size */
        q->size_++;
}

void Atomic_queue_pop(atomic_queue_t q) {

        CHECK_PTR(q);

        /* if empty return */
        if (Atomic_queue_empty(q))
                return;

        /* pop element */
        list_del(q->head_.next);

        /* update size */
        q->size_--;
}

int Atomic_queue_size(atomic_queue_t q) {

        return q->size_;
}

bool Atomic_queue_empty(atomic_queue_t q) {

        return (q->size_ > 0) ? false : true;
}

queue_element_t *Atomic_queue_front(atomic_queue_t q) {

        /* if empty return NULL */
        if (Atomic_queue_empty(q))
                return NULL;

        /* return the front element */
        return list_entry(q->head_.next, queue_element_t, head_);
}

queue_element_t *Atomic_queue_back(atomic_queue_t q) {

        /* if empty return NULL */
        if (Atomic_queue_empty(q))
                return NULL;

        /* return the back element */
        return list_entry(q->head_.prev, queue_element_t, head_);
}
