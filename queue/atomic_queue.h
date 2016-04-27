#ifndef __ATOMIC_QUEUE_H__
#define __ATOMIC_QUEUE_H__

#include "envelop.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct atomic_queue *atomic_queue_t;

void Atomic_queue_init(atomic_queue_t *);
void Atomic_queue_fini(atomic_queue_t *);
void Atomic_queue_push(atomic_queue_t, void *);
void Atomic_queue_pop(atomic_queue_t);
int  Atomic_queue_size(atomic_queue_t);
bool Atomic_queue_empty(atomic_queue_t);
void *Atomic_queue_front(atomic_queue_t);
void *Atomic_queue_back(atomic_queue_t);
#endif
