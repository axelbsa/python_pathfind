#ifndef COMMON_H_   /* Include guard */
#define COMMON_H_

#include <stdlib.h>

#define INIT_HEAP_SIZE (256*256)

struct Points {
    int x;
    int y;
    int id;
    int fcost;
    int hcost;
    int gcost;
    struct Points *parent;
};
typedef struct Points POINT;

struct open_list {
    int id;
    POINT *p;
};
typedef struct open_list OPEN_LIST;

struct closed_list {
    int id;
    POINT *p;
};
typedef struct closed_list CLOSED_LIST;


void heap_add(POINT *point);

POINT * heap_del();

size_t heap_size();

void heap_destroy();

#endif // FOO_H_
