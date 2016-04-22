#include "atomic_queue.h"
#include "list.h"

#define CHECK_PTR(ptr)               \
        do {                     \
                if (ptr == NULL) \
                        return;  \
        } while(0)

struct atomic_queue {

        struct list_head head_;
        int size_;

        void (*push)(atomic_queue_t, queue_element_t *);
        void (*pop)(atomic_queue_t);
        int  (*size)(atomic_queue_t);
        bool (*empty)(atomic_queue_t);
        queue_element_t *(*front)(atomic_queue_t);
        queue_element_t *(*back)(atomic_queue_t);
};

void Atomic_queue_init(atomic_queue_t *q) {

        struct list_head *dummy;
        *q = (struct atomic_queue *)malloc(sizeof(struct atomic_queue));
        INIT_LIST_HEAD(&((*q)->head_));

        /* add dummy element */
        dummy = (struct list_head *)malloc(sizeof(struct list_head));
        list_add_tail(dummy, &(*q)->head_);
        (*q)->size_ = 0;
}

void Atomic_queue_fini(atomic_queue_t *q) {

        CHECK_PTR(*q);

        /* free dummy element */
        free((*q)->head_.prev);

        /* free queue */
        free(*q);
}

void Atomic_queue_push(atomic_queue_t q, queue_element_t *e) {

        struct list_head *dummy;

        CHECK_PTR(q);

        /* allocate space for dummy */
        dummy = (struct list_head *)malloc(sizeof(struct list_head));

        /* replace e->head_ with existing dummy */
        e->head_ = q->head_.prev;

        /* push a new dummy to the queue */
        list_add_tail(dummy, &(q->head_));

        /* updated size */
        q->size_++;
}

void Atomic_queue_pop(atomic_queue_t q) {

        struct list_head *e;

        CHECK_PTR(q);

        /* if empty return */
        if (Atomic_queue_empty(q))
                return;

        /* pop element */
        e = q->head_.next;
        list_del(e);

        /* update size */
        q->size_--;

        /* free memory */
        free(e);
}

int Atomic_queue_size(atomic_queue_t q) {

        CHECK_PTR(q);

        return q->size_;
}

bool Atomic_queue_empty(atomic_queue_t q) {

        CHECK_PTR(q);

        return (q->size_ > 0) ? false : true;
}

queue_element_t *Atomic_queue_front(atomic_queue_t q) {

        CHECK_PTR(q);

        /* if empty return NULL */
        if (Atomic_queue_empty(q))
                return NULL;

        /* return the front element */
        return list_entry(q->head_.next, queue_element_t, head_);
}

queue_element_t *Atomic_queue_back(atomic_queue_t q) {

        CHECK_PTR(q);

        /* if empty return NULL */
        if (Atomic_queue_empty(q))
                return NULL;

        /* return the back element */
        return list_entry(q->head_.prev, queue_element_t, head_);
}
