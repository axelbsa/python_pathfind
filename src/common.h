#ifndef COMMON_H_   /* Include guard */
#define COMMON_H_

#include <stdlib.h>

#define INIT_HEAP_SIZE (1024*1024)

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



POINT* points_add(POINT* point);

POINT* points_find(int id);

void points_destroy();



void open_add(POINT *point);

POINT * open_del();

size_t open_size();

void open_destroy();

int open_search(POINT *point);



void closed_add(POINT* point);

void closed_destroy();


#endif // FOO_H_
