#include <stdio.h>
#include "uthash.h"

int items_added = 0;

int path[10][10] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
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

struct Points *point = NULL;

void add_user(int user_id, int x, int y) {
    struct Points *p = NULL;

    p = (struct Points*)malloc(sizeof(struct Points));
    p->id = user_id;
    p->x = 22;
    p->y = 22;
    HASH_ADD_INT( point, id, p );  /* id: name of key field */
}

void add_items() {
    int i,j;
    for(i=0; i<10; i++){
        for(j=0; j<10; j++){
            printf("%i ", path[i][j]);
            add_user(i+j, j, i);
            items_added++;
        }
        printf("\n");
    }
}

int main() {
    printf("Hello world\n");

    add_items();
    return 0;
}
