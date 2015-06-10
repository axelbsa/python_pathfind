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
};

struct open_list {
    int id;
    struct Points *p;
    UT_hash_handle hh;
};

struct closed_list {
    int id;
    struct Points *p;
    UT_hash_handle hh;
};

typedef struct open_list OPEN_LIST;
typedef struct open_list CLOSED_LIST;
typedef struct Points POINT;

struct Points *point = NULL;
struct open_list *olist = NULL;
CLOSED_LIST *clist = NULL;

struct Points *add_node(int _id, int x, int y, struct Points *parent) {

    struct Points *p = NULL;
    p = (POINT*)malloc(sizeof(POINT));
    p->id = _id;
    p->x = x;
    p->y = y;

    if(parent)
        p->parent = parent;

    HASH_ADD_INT(point, id, p);  /* id: name of key field */
    printf("Adding nodeid:%d\n", _id);
    return p;
}

struct Points *find_node(int _id) {
    struct Points *s;
    HASH_FIND_INT( point, &_id, s );  
    return s;
}

void add_items() {
    struct Points *parent = NULL;
    int i,j;
    for(i=0; i<10; i++){
        for(j=0; j<10; j++){
            printf("%i ", path[i][j]);
            parent = add_node((i*10)+j, j, i, parent);
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

void find_adj(int sy, int sx){

    /* Representation of adjecent
     * nodes
     * +-----+
     * |7|3|4|
     * |2|S|0|
     * |6|1|5|
     * +-----+
     */

    int _id;
    int start_node;
    for(int i=0; i<8; i++){

        struct open_list *o = NULL;
        o = (OPEN_LIST*)malloc(sizeof(OPEN_LIST));
        
        struct Points *s_node = NULL;
        s_node = (struct Points*)malloc(sizeof(struct Points));

        _id = 0;
        start_node = 0;

        switch(i){
            case 0:
                start_node = (10*sy)+sx+1;
                s_node = find_node(start_node);
                if(!s_node)
                    break;
                _id = s_node->id;
                o->id = _id;
                o->p = s_node;
                HASH_ADD_INT(olist, id, o);
                break;

            case 1:
                start_node = (10*(sy+1))+sx;
                s_node = find_node(start_node);
                if(!s_node)
                    break;
                _id = s_node->id;
                o->id = _id;
                o->p = s_node;
                HASH_ADD_INT(olist, id, o);
            break;

            case 2:
                start_node = (10*sy)+sx-1;
                s_node = find_node(start_node);
                if(!s_node)
                    break;
                _id = s_node->id;
                o->id = _id;
                o->p = s_node;
                HASH_ADD_INT(olist, id, o);
            break;

            case 3:
                start_node = (10*(sy-1))+sx;
                s_node = find_node(start_node);
                if(!s_node)
                    break;
                _id = s_node->id;
                o->id = _id;
                o->p = s_node;
                HASH_ADD_INT(olist, id, o);
            break;

            case 4:
                start_node = (10*(sy-1))+sx+1;
                s_node = find_node(start_node);
                if(!s_node)
                    break;
                _id = s_node->id;
                o->id = _id;
                o->p = s_node;
                HASH_ADD_INT(olist, id, o);
            break;

            case 5:
                start_node = (10*(sy+1))+sx+1;
                s_node = find_node(start_node);
                if(!s_node)
                    break;
                _id = s_node->id;
                o->id = _id;
                o->p = s_node;
                HASH_ADD_INT(olist, id, o);
            break;

            case 6:
                start_node = (10*(sy+1))+sx-1;
                s_node = find_node(start_node);
                if(!s_node)
                    break;
                _id = s_node->id;
                o->id = _id;
                o->p = s_node;
                HASH_ADD_INT(olist, id, o);
            break;

            case 7:
                start_node = (10*(sy-1))+sx-1;
                s_node = find_node(start_node);
                if(!s_node)
                    break;
                _id = s_node->id;
                o->id = _id;
                o->p = s_node;
                HASH_ADD_INT(olist, id, o);
            break;
        }
    }

   // HASH_FIND_INT( point, &star_node, s );
}

void list_points(){
    struct Points *s;

    for(s=point; s != NULL; s=(struct Points*)(s->hh.next)) {
        printf("S %d\n", &s->hh.next);
        printf("Node: id %d: x:%d y:%d\n", s->id, s->x, s->y);
    }
}

void find_lowest(){
    int iter = 0;
    struct open_list *s, *tmp;

    //s = (OPEN_LIST*)malloc(sizeof(OPEN_LIST));
    //HASH_ITER(hh, olist, s, tmp) {
        //printf("Node: id %d: x:%d y:%d\n", s->id, s->p->x, s->p->y);
        //printf("We have now cycled through %d times \n", iter++);
    //}

    for(s=olist; s != NULL; s=(OPEN_LIST*)(s->hh.next)) {
        printf("S %d\n", &s->hh.next);
        printf("Node: id %d: x:%d y:%d\n", s->id, s->p->x, s->p->y);
    }
}

void search(int sy, int sx, int dy, int dx) {
    int start_node = (10*sy)+sx;

    struct Points *s_node = NULL;
    s_node = (struct Points*)malloc(sizeof(struct Points));
    s_node = find_node(start_node);

    printf("Node: id %d: x:%d y:%d\n", s_node->id, s_node->x, s_node->y);

    int _id = s_node->id;
    struct open_list *o = NULL;
    o = (OPEN_LIST*)malloc(sizeof(OPEN_LIST));
    o->id = _id;
    o->p = s_node;
    HASH_ADD_INT(olist, id, o);

    find_adj(s_node->y, s_node->x);
    find_lowest();

    int hcount = HASH_COUNT(olist);
    printf("HCOUNT: %d\n", hcount);

    //for(int i=0; i<hcount; i++){
        //printf("Distance: %d\n", manhatten(sy, sx, dy, dx));
        //find_lowest();
        ////find_adj();
    //}

    free(o);
}

int main() {
    add_items();
    search(0,0,9,9);
    return 0;
}
