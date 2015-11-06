#include <stdio.h>
#include <string.h>
#include <math.h>
#include "common.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

bool first = false;
int items_added = 0;

int mapWidth = 10;
int mapHeight = 10;

int closed_size = 0;
POINT* closed_list[200000];

// A Bigger map for later testing
//int world_map[ 20 * 20 ] = 
//{

//// 0001020304050607080910111213141516171819
	//0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,   // 00
	//0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,   // 00
	//0,1,1,0,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0,   // 02
	//0,1,1,0,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0,   // 03
	//0,1,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,1,0,0,   // 04
	//0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,   // 05
	//0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,   // 06
	//0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,   // 07
	//0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,   // 08
	//0,1,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,   // 01
	//0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,   // 00
	//0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,0,0,0,   // 00
	//0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0,   // 02
	//0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0,   // 03
	//0,1,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,1,0,0,   // 04
	//0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,   // 05
	//0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,   // 06
	//0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,   // 07
	//0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,   // 08
	//0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,   // 01

//};

int lut[] = {1, 9999};
int path[10][10] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,1,1,1,1},
    {1,0,1,1,1,0,1,1,1,1},
    {1,0,1,1,1,0,1,1,1,1},
    {1,0,0,0,1,0,1,1,1,1},
    {1,1,1,0,1,0,0,0,1,1},
    {1,1,0,0,1,0,1,0,1,1},
    {1,1,0,0,0,1,1,1,1,1},
    {1,1,0,1,1,0,1,0,0,1},
    {1,1,0,0,0,0,0,0,0,0},
};

int START_NODE = 0;
int END_NODE = 0;

int ALLOW_DIAGONAL = 0;

POINT* add_node(int _id, int x, int y, POINT* parent) {
    POINT p;
    p.id = _id;
    p.x = x;
    p.y = y;
    p.fcost = 0;
    p.hcost = 0;
    p.gcost = 0;
    p.parent = 0;
    return points_add(&p);
}

void add_items(int sy=0, int sx=0, int dy=0, int dx=0) {
    START_NODE = (10 * sy) + sx;
    END_NODE = (10 * dy) + dx; 
    POINT *parent = NULL;
    int i,j;
    for(i = 0; i < mapHeight; i++){
        for(j = 0; j < mapWidth; j++){

            int c_node = ( i * 10 ) + j;

            if (c_node == START_NODE){
                printf("S ");
            }else if(c_node == END_NODE){
                printf("E ");
            }else{
                printf("%i ", path[i][j]);
            }

            parent = add_node(( i * 10 ) + j, j, i, parent);
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
                start_node = (10*sy)+sx+1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
                break;

            case 1:
                start_node = (10*(sy+1))+sx;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 2:
                start_node = (10*sy)+sx-1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 3:
                start_node = (10*(sy-1))+sx;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 4:
                start_node = (10*(sy-1))+sx+1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 5:
                start_node = (10*(sy+1))+sx+1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 6:
                start_node = (10*(sy+1))+sx-1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 7:
                start_node = (10*(sy-1))+sx-1;
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

    POINT* p = points_find(START_NODE);
    while(p){
        int sy = p->y;
        int sx = p->x;
        printf("Testing %d%d parent=%d\n", sy,sx, p->parent->id);
        if((10*sy)+sx == END_NODE)
            break;
        p = p->parent;
    };
}

void search(int sy, int sx, int dy, int dx) {
    int lowest = 0;
    int finished = 0;
    int start_node = (10*sy)+sx;
    int END_NODE = (10*dy)+dx;

    POINT* s_node = points_find(start_node);
    printf("im searching\n");

    open_add(s_node);
    int successors[] = {-1, -1, -1, -1, -1, -1, -1, -1};
    while(open_size() > 0 && finished == 0){

        POINT* p = open_del();

        find_adj(p->y, p->x, successors);

        int successor_count = 4;
        if (ALLOW_DIAGONAL)
            successor_count = 8;

        for(int i=0; i < successor_count; i++){
            POINT* successor;
            successor = points_find(successors[i]);

            if(!successor)
                continue;

            int sx = successor->x;
            int sy = successor->y;
            
            int gcost = successor->gcost;
            gcost += 10 * lut[path[sy][sx]];
            successor->gcost = gcost;
            successor->fcost = manhatten(sy, sx, dy, dx) + successor->gcost;
            successor->parent = p;
            //printf("\tFcost=%d for node=%d%d parent=%d\n", successor->fcost, sy, sx, successor->parent->id);

            open_add(successor);
        }
        if(p->parent != NULL)
            printf("Lowest found was: %d parent=%d \n ", (10*p->y)+p->x, p->parent->id);
        closed_add(p);
        closed_list[closed_size++] = p;
        memset(successors, -1, sizeof(successors));
        if (( 10 * p->y ) + p->x == END_NODE ){
            break;
        }
    }
    POINT* p = closed_list[closed_size-1];
    create_path(p->id);
    open_destroy();
    printf("Finished :)\n");
}

int main() {
    add_items(1,1,8,8);
    search(1,1,8,8);
    points_destroy();
    return 0;
}
