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

                /* check term command */
                if (e->integer_ == -1)
                        break;

                /* print content of element */
                fprintf(stdout, "received: %d through: 0x%x\n", e->integer_, e);

                /* pop element */
                Atomic_queue_pop(q);

                //free(e);
        }

        pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

        int i;
        pthread_t tid;
        atomic_queue_t q;
        queue_element_t *e;

        /* init queue */
        Atomic_queue_init(&q);

        /* push new elements in the queue */
        for (i = 0; i < 15; i++) {
                e = (queue_element_t *)malloc(sizeof(queue_element_t));
                e->integer_ = i;
                fprintf(stdout, "send: %d through:0x%x\n", e->integer_, e);
                Atomic_queue_push(q, e);
        }

        /* start thread */
        pthread_create(&tid, NULL, thread, q);

        /* submit end request : integet = -1 */
        e = (queue_element_t *)malloc(sizeof(queue_element_t));
        e->integer_ = -1;
        Atomic_queue_push(q, e);

        /* wait for thread to end */
        pthread_join(tid, NULL);

        /* fini queue */
        Atomic_queue_fini(&q);

        return 0;
}
