#include <stdio.h>
#include <math.h>
#include "common.h"
#include "uthash.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

bool first = false;
int items_added = 0;

int mapWidth = 10;
int mapHeight = 10;

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

int path[10][10] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,1,1,1,1},
    {1,0,1,1,1,0,1,1,1,1},
    {1,0,1,1,1,0,1,1,1,1},
    {1,0,0,0,1,0,1,1,1,1},
    {1,1,1,0,1,0,1,1,1,1},
    {1,1,0,0,1,0,1,1,1,1},
    {1,1,0,0,0,0,0,0,1,1},
    {1,1,0,1,1,0,1,0,0,1},
    {1,1,0,0,0,0,1,0,0,0},
};

int START_NODE = 0;
int END_NODE = 0;

int allow_diagonal = 0;

POINT* add_node(int _id, int x, int y, POINT* parent) {

    //struct Points *p = NULL;
    //p = (POINT*)malloc(sizeof(POINT));
    //p->id = _id;
    //p->x = x;
    //p->y = y;


    //HASH_ADD_INT(point, id, p);  /* id: name of key field */
    //printf("Adding nodeid:%d\n", _id);

    POINT p;
    p.id = _id;
    p.x = x;
    p.y = y;
    p.parent = parent;

    return points_add(&p);
}

void add_items(int sy=0, int sx=0, int dy=0, int dx=0) {
	START_NODE = (10*sy) + sx;
	END_NODE = (10*dy) + dx; 
    POINT *parent = NULL;
    int i,j;
    for(i=0; i<10; i++){
        for(j=0; j<10; j++){
        	int c_node = (i*10)+j;
        	
        	if (c_node == START_NODE){
				printf("S ");
        	}else if(c_node == END_NODE){
				printf("E ");
        	}else{
            	printf("%i ", path[i][j]);
        	}

            parent = add_node((i*10)+j, j, i, parent);
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
    //struct Points *s_node = NULL;
    for(int i=0; i<8; i++){
        //s_node = (struct Points*)malloc(sizeof(struct Points));
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
    float D = 1.070;
    int tx = abs(sx - dx);
    int ty = abs(sy - dy);
    return D * (tx + ty) + (D*2 - 2 * D) * MIN(tx, ty);
}

//int find_lowest(){
    //int lowest = 999999999;
    //int _id = 0;
    //struct open_list *s, *tmp;

    ////for(s=olist; s != NULL; s=(OPEN_LIST*)(s->hh.next)) {
        //////printf("S %d\n", &s->hh.next);
        ////int cur = manhatten(s->p->y, s->p->x, END_NODE/10, END_NODE%10);
        //////printf("Node: x:%d y:%d - distance: %d\n", s->p->y, s->p->x, cur);
        ////if (cur < lowest){
            ////lowest = cur;
            ////_id = (10*s->p->y)+s->p->x;
        ////}
    ////}

    //return _id;
//}

void search(int sy, int sx, int dy, int dx) {
    int lowest = 0;
    int finished = 0;
    int start_node = (10*sy)+sx;
    int END_NODE = (10*dy)+dx;

    POINT* s_node = points_find(start_node);

    //printf("Node: id %d: y:%d x:%d\n", s_node->id, s_node->y, s_node->x);

    //int _id = s_node->id;
    //o = (OPEN_LIST*)malloc(sizeof(OPEN_LIST));
    //o->id = _id;
    //o->p = s_node;
    //o->p->fcost = 0;
    //HASH_ADD_INT(olist, id, o);
    printf("im searching\n");
    
    open_add(s_node);
    int successors[] = {-1, -1, -1, -1, -1, -1, -1, -1};
    while(open_size() > 0 && finished == 0){
    	
        //lowest = find_lowest();
        //s_node = find_node(lowest);
        //int fcost = s_node->fcost;
        //int gcost = s_node->gcost;

    	//struct open_list *del = (OPEN_LIST*)malloc(sizeof(OPEN_LIST));
    	//HASH_FIND_INT( olist, &s_node->id, del );
    	//HASH_DEL(olist, del);

        POINT* p = open_del();
	    find_adj(p->y, p->x, successors);	    
	    printf("Lowest found was: %d\n", (10*p->y)+p->x);
       
        int successor_count = 4;
        if (allow_diagonal)
            successor_count = 8;

	    for(int i=0; i<successor_count; i++){
	    	if(successors[i] < 0)
	    		continue;
            POINT* successor;
	    	successor = points_find(successors[i]);
            int sx = successor->x;
            int sy = successor->y;
            successor->gcost++;
            successor->fcost = manhatten(sy, sx, dy, dx) + successor->gcost;

	    	if(!successor)
	    		continue;
	    	if ((10*successor->y)+successor->x == END_NODE){
	    		printf("WE ARE HERE\n");
	    		finished = 1;
	    		break;
	    	}
            open_add(successor);
	        //printf("Inner loop: y:%d x:%d\n", s_node->y, s_node->x);
			//tmp = (OPEN_LIST*)malloc(sizeof(OPEN_LIST));
			//tmp->id = s_node->id;
            //tmp->p = s_node;
            //tmp->p->fcost = 0;
    		//HASH_ADD_INT(olist, id, tmp);
	    }
        fprintf(stderr, "im in ur looopz\n");
        closed_add(p);
		//c = (CLOSED_LIST*)malloc(sizeof(CLOSED_LIST));
		//c->id = s_node->id;
		//c->p = s_node;
	    //HASH_ADD_INT(clist, id, c);
		//list_open();
		//printf("Hashcount before end: %d \n", HASH_COUNT(olist));
		memset(successors, -1, sizeof(successors));
	}
	//list_closed();
    open_destroy();
    
    printf("Finished :)\n");
}

int main() {
    add_items(1,1,8,8);
    search(1,1,8,8);
    points_destroy();
    return 0;
}

//void list_closed(){
    //struct closed_list *s;
    //for(s=clist; s != NULL; s=(CLOSED_LIST*)(s->hh.next)) {
        //printf("Node: x:%d y:%d \n", s->p->y, s->p->x);
    //}
//}

//void list_open(){
    //struct open_list *s;
    //for(s=olist; s != NULL; s=(OPEN_LIST*)(s->hh.next)) {
        //printf("Node: x:%d y:%d\n", s->p->y, s->p->x);
    //}
//}

//void list_points(){
    //struct Points *s;

    //for(s=point; s != NULL; s=(struct Points*)(s->hh.next)) {
        //printf("S %d\n", &s->hh.next);
        //printf("Node: id %d: x:%d y:%d\n", s->id, s->x, s->y);
    //}
//}
