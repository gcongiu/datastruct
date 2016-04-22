#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "atomic_queue.h"

void *thread(void *ptr) {

        atomic_queue_t q = (atomic_queue_t)ptr;
        queue_element_t *e;

        for(;;) {
                while (Atomic_queue_empty(q));

                /* get front element */
                e = Atomic_queue_front(q);

                /* print element */
                fprintf(stdout, "received: %d\n", e->integer_);

                /* check term command */
                if (e->integer_ == -1)
                        break;

                /* pop element */
                Atomic_queue_pop(q);

                free(e);
        }

        pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

        int i;
        pthread_t tid;
        pthread_attr_t attr;
        atomic_queue_t q;
        queue_element_t *e;

        /* init queue */
        Atomic_queue_init(&q);

        /* start thread */
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_create(&tid, &attr, thread, q);
        pthread_attr_destroy(&attr);

        /* push new elements in the queue */
        for (i = 0; i < 15; i++) {
                e = (queue_element_t *)malloc(sizeof(queue_element_t));
                e->integer_ = i;
                Atomic_queue_push(q, e);
        }

        /* submit end request : integet = -1 */
        e = (queue_element_t *)malloc(sizeof(queue_element_t));
        e->integer_ = -1;
        Atomic_queue_push(q, e);

        pthread_join(tid, NULL);

        /* fini queue */
        Atomic_queue_fini(&q);

        return 0;
}
