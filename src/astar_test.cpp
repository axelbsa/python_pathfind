#include <stdio.h>
#include <math.h>
#include "uthash.h"

bool first = false;
int items_added = 0;

int mapWidth = 10;
int mapHeight = 10;

int path[10][10] = {
    {0,0,0,0,0,0,1,1,1,1},
    {0,1,1,1,1,0,1,1,1,1},
    {0,1,1,1,1,0,1,1,1,1},
    {0,1,1,1,1,0,1,1,1,1},
    {0,0,0,0,1,0,1,1,1,1},
    {1,1,1,0,1,0,1,1,1,1},
    {1,1,0,0,1,0,1,1,1,1},
    {1,1,0,1,0,0,0,0,1,1},
    {1,1,0,1,1,0,1,0,1,1},
    {1,1,0,0,0,0,1,0,0,0},
};

struct Points {
    int x;
    int y;
    int id;
    int fcost;
    int hcost;
    int gcost;
    struct Points *parent;
    UT_hash_handle hh;
} POINT;

typedef struct open_list {
    int id;
    struct Points *p;
    UT_hash_handle hh;
} OPEN_LIST;

typedef struct closed_list {
    int id;
    struct Points *p;
    UT_hash_handle hh;
} CLOSED_LIST;

struct Points *point = NULL;
OPEN_LIST *olist = NULL;
CLOSED_LIST *clist = NULL;

void add_node(int _id, int x, int y) {
    struct Points *p = NULL;
    printf("Adding nodeid:%d\n", _id);

    p = (struct Points*)malloc(sizeof(POINT));
    p->id = _id;
    p->x = x;
    p->y = y;
    HASH_ADD_INT(point, id, p);  /* id: name of key field */
}

struct Points *find_node(int _id) {
    struct Points *s;
    HASH_FIND_INT( point, &_id, s );  
    return s;
}

void add_items() {
    int i,j;
    for(i=0; i<10; i++){
        for(j=0; j<10; j++){
            printf("%i ", path[i][j]);
            add_node((i*10)+j, j, i);
            items_added++;
        }
        printf("\n");
    }
}

int manhatten(int sy, int sx, int dy, int dx) {
    int tx = abs(sx - dx);
    int ty = abs(sy - dy);
    return 2 * (dx + dy);
}

void find_adj(struct Points *p){
    OPEN_LIST *s = NULL;
    s = (OPEN_LIST*)malloc(sizeof(OPEN_LIST));

   // HASH_FIND_INT( point, &star_node, s );
}

void find_lowest(){
    OPEN_LIST *s = NULL;
    s = (OPEN_LIST*)malloc(sizeof(OPEN_LIST));
    for(s=olist; s != NULL; s=(OPEN_LIST*)s->hh.next) {
        printf("user id %d: x:%d y:%d\n", s->id, s->p->x, s->p->y);
    }
}

void search(int sy, int sx, int dy, int dx) {
    int start_node = (10*sy)+sx;

    struct Points *s_node = NULL;
    s_node = (struct Points*)malloc(sizeof(POINT));
    s_node = find_node(start_node);

    printf("Node id:%d\n", s_node->id);

    OPEN_LIST *o = NULL;
    o = (OPEN_LIST*)malloc(sizeof(OPEN_LIST));
    o->id = s_node->id;
    o->p = s_node;
    HASH_ADD_INT(olist, id, o);

    find_adj(s_node);

    while(HASH_COUNT(olist)){
        find_lowest();
        //find_adj();
        printf("Distance: %d\n", manhatten(sy, sx, dy, dx));
        printf("Distance: %d\n", manhatten(sy, sx, dy, dx));
    }
}

int main() {
    add_items();
    search(0,0,9,9);
    return 0;
}
