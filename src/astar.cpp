#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include "common.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#define MAX_MAP_SIZE 1024*1024

bool first = false;
int items_added = 0;

int mapWidth = 20;
int mapHeight = 20;

int closed_size = 0;
POINT* closed_list[MAX_MAP_SIZE];
POINT* ppoints[MAX_MAP_SIZE];

//A Bigger map for later testing
int path[20][20] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,0,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0},
    {0,1,1,0,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0},
    {0,1,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,1,0,0},
    {0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
    {0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
    {0,1,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0},
    {0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,0,0,0},
    {0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0},
    {0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0},
    {0,1,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,1,0,0},
    {0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
    {0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0},
    {0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1},
    {0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

int lut[] = {1, 90};
int START_NODE = 0;
int END_NODE = 0;

int ALLOW_DIAGONAL = 0;

POINT* add_node(int id, int y, int x, POINT* parent) {
    POINT p;
    p.id = id;
    p.x = x;
    p.y = y;
    p.fcost = 0;
    p.hcost = 0;
    p.gcost = 0;
    p.parent = 0;
    ppoints[(y*mapWidth)+x] = &p;
    return points_add(&p);
}

void add_items(int sy=0, int sx=0, int dy=0, int dx=0) {
    START_NODE = (mapWidth * sy) + sx;
    END_NODE = (mapWidth * dy) + dx; 
    POINT *parent = NULL;

    int i,j;

    for(i = 0; i < mapHeight; i++){
        for(j = 0; j < mapWidth; j++){

            int c_node;
            c_node = (i * mapWidth) + j;

            if (c_node == START_NODE){
                printf("S ");
            }else if(c_node == END_NODE){
                printf("E ");
            }else{
                printf("%i ", path[i][j]);
            }

            parent = add_node(c_node, i, j, parent);
            items_added++;
        }
        printf("\n");
    }
}

void find_adj(int sy, int sx, int* successors){

    /* Representation of adjecent
     * nodes
     * +-----+
     * |7|3|4|
     * |2|S|0|
     * |6|1|5|
     * +-----+
     */

    int start_node;
    for(int i=0; i<8; i++){

        POINT* s_node;
        start_node = 0;

        switch(i){
            case 0:
                start_node = (mapWidth * sy) + sx+1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
                break;

            case 1:
                start_node = (mapWidth * (sy + 1)) + sx;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 2:
                start_node = (mapWidth * sy) + sx - 1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 3:
                start_node = (mapWidth * (sy - 1)) + sx;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 4:
                start_node = (mapWidth * (sy - 1)) + sx + 1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 5:
                start_node = (mapWidth * (sy + 1)) + sx + 1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 6:
                start_node = (mapWidth * (sy + 1)) + sx - 1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 7:
                start_node = (mapWidth * (sy - 1)) + sx - 1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;
        }
    }
}

int manhatten(int sy, int sx, int dy, int dx) {
    int tx = abs(sx - dx);
    int ty = abs(sy - dy);
    return (2 * (tx + ty));
}

float chebyshev(int sy, int sx, int dy, int dx ) {
    float D = 14;
    int tx = abs(sx - dx);
    int ty = abs(sy - dy);
    return D * (tx + ty) + (D*2 - 2 * D) * MIN(tx, ty);
}

void create_path(int id){
    int i = 0;
    POINT* p = points_find(START_NODE);
    while(p){
        int sy = p->y;
        int sx = p->x;
        printf("Testing %d%d parent=%d\n", sy,sx, p->parent->id);
        if(i++ > 60)
            break;
        if((mapWidth * sy) + sx == END_NODE)
            break;
        p = p->parent;
    };
}

void search(int sy, int sx, int dy, int dx) {
    int finished = 0;
    int start_node = (mapWidth * sy) + sx;
    int END_NODE = (mapWidth * dy) + dx;

    POINT* s_node = points_find(start_node);
    printf("im searching for %d from %d\n", END_NODE, start_node);

    open_add(s_node);
    int successors[] = {-1, -1, -1, -1, -1, -1, -1, -1};
    while(open_size() > 0 && finished == 0){

        POINT* p = open_del();
        find_adj(p->y, p->x, successors);

        int successor_count = 4;
        if (ALLOW_DIAGONAL)
            successor_count = 8;

        printf("Lowest found was: %d parent=%d \n ",
                p->id, p->parent ? p->parent->id : 0);

        for(int i=0; i < successor_count; i++){
            POINT* successor;
            successor = points_find(successors[i]);

            if(!successor)
                continue;

            int sx = successor->x;
            int sy = successor->y;
            int gcost = successor->gcost;

            gcost += 8 * lut[path[sy][sx]];

            printf("Gcost = %d \n",gcost);
            successor->gcost = gcost;
            successor->fcost = manhatten(sy, sx, dy, dx) + successor->gcost;
            successor->parent = p;
            printf("\tFcost=%d for node=%d%d parent=%d\n", successor->fcost, sy, sx, successor->parent->id);

            open_add(successor);
        }

        closed_add(p);
        closed_list[(p->y*mapWidth)+p->x] = p;
        memset(successors, -1, sizeof(successors));

        if (( mapWidth * p->y ) + p->x == END_NODE ){
            break;
        }
    }

    //POINT* p = closed_list[closed_size-1];
    //create_path(p->id);
    //open_destroy();
    printf("Finished :)\n");
}

void load_map(char* mapfile){
    int i = 0;
    int j = 0;
    FILE *fp;
    char ch;
    char mystring[100];
    fp=fopen(mapfile, "r");

    if (fp == NULL) {
        printf("Cannot open file '%s' : %s\n", mapfile, strerror(errno));
        return;
    }

    fgets (mystring , 100 , fp);
    fgets (mystring , 100 , fp);
    fgets (mystring , 100 , fp);
    fgets (mystring , 100 , fp);

    while( ( ch = fgetc(fp) ) != EOF ){
        if(j % 513 == 0){
            j=0;
            i++;
        }
        path[i][j++] = ch;
    }
}


        
int main() {
    //load_map("maze512-16-0.map");
    add_items(3,3,6,10);
    search(3,3,6,10);
    points_destroy();
    return 0;
}
