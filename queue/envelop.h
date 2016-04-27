#ifndef __ENVELOP_H__
#define __ENVELOP_H__

#include "list.h"

struct envelop {
        void *elem_;
        struct list_head head_;
};
#endif
