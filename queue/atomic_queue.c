#include "atomic_queue.h"
#include "list.h"

/*
 * Atomic queue implementation - valid only for one producer and one consumer
 *
 * The queue stores envelops. These contain a pointer to the element enqueued
 * by the user and a struct list_head to keep the list pointers. When the
 * queue is first initialised it will contain an empty envelop, in which the
 * elem_ member points to NULL
 *
 * Atomic_queue_init(&q):
 *  +---------------------------------------+
 *  |                            env_1      |
 *  |                       +------------+  |
 *  |                       |    elem_---|--|-->NULL
 *  |                       |            |  |
 *  |       head_           |    head_   |  |
 *  |    +--------+         | +--------+ |  |
 *  +--->|  next  |---------|>|  next  |-|--+
 *       +--------+         | +--------+ |
 *  +----|  prev  |<--------|-|  prev  |<|--+
 *  |    +--------+         | +--------+ |  |
 *  |                       +------------+  |
 *  +---------------------------------------+
 *
 *
 * Atomic_queue_push(q, new):
 *                                                                    +-------+
 *                                           +----------------------->|  new  |
 *                                           |                        +-------+
 *  +----------------------------------------|-------------------+
 *  |                            env_1       |        env_2      |
 *  |                       +------------+   |   +------------+  |
 *  |                       |    elem_---|---+   |    elem_---|--|-->NULL
 *  |                       |            |       |            |  |
 *  |       head_           |    head_   |       |    head_   |  |
 *  |    +--------+         | +--------+ |       | +--------+ |  |
 *  +--->|  next  |---------|>|  next  |-|-------|>|  next  |-|--+
 *       +--------+         | +--------+ |       | +--------+ |
 *  +----|  prev  |<--------|-|  prev  |<|-------|-|  prev  |<|--+
 *  |    +--------+         | +--------+ |       | +--------+ |  |
 *  |                       +------------+       +------------+  |
 *  +------------------------------------------------------------+
 *
 *  A new Atomic_queue_pop(q) will remove env_1, that is:
 *   - q->head_.next     = env_2->head_
 *   - env_2->head_.prev = q->head_
 *
 *  A concurrent Atomic_queue_push(q, new) will add a new envelop (env_3) and make
 *  env_2 elem_ point to 'new':
 *   - env_2->head_.next = env_3->head_
 *   - env_3->head_.pre  = env_2->head_
 *   - env_3->head_.next = q->head_
 *   - q->head_.prev     = env_3->head_
 *   - env_2->elem_      = new
 *
 *  The push operation involves neither env_2->head_.prev nor
 *  head_.next (modified by the pop) and therefore does not produce
 *  any inconsistency in the queue.
 */

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

        struct envelop *empty;
        *q = (struct atomic_queue *)malloc(sizeof(struct atomic_queue));
        INIT_LIST_HEAD(&((*q)->head_));

        /* add empty envelop */
        empty = (struct envelop*)malloc(sizeof(struct envelop));
        list_add_tail(&(empty->head_), &(*q)->head_);
        (*q)->size_ = 0;
}

void Atomic_queue_fini(atomic_queue_t *q) {

        struct envelop *env;

        CHECK_PTR(*q);

        /* free dummy element */
        env = list_entry((*q)->head_.prev, struct envelop, head_);
        free(env);

        /* free queue */
        free(*q);
}

void Atomic_queue_push(atomic_queue_t q, void *elem) {

        struct envelop *empty;

        CHECK_PTR(q);

        /* alloc mem for new envelop */
        empty = (struct envelop *)malloc(sizeof(struct envelop));

        /* add empty envelop at the tail of the list */
        list_add_tail(&(empty->head_), &(q->head_));

        /* get oldest empty envelop */
        empty = list_entry(q->head_.prev->prev, struct envelop, head_);

        /* update pointer to element */
        empty->elem_ = elem;

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

void *Atomic_queue_front(atomic_queue_t q) {

        struct envelop *env;

        /* if empty return NULL */
        if (Atomic_queue_empty(q))
                return NULL;

        /* return the front element */
        env = list_entry(q->head_.next, struct envelop, head_);
        return env->elem_;
}

void *Atomic_queue_back(atomic_queue_t q) {

        struct envelop *env;

        /* if empty return NULL */
        if (Atomic_queue_empty(q))
                return NULL;

        /* return the front element */
        env = list_entry(q->head_.prev, struct envelop, head_);
        return env->elem_;
}
