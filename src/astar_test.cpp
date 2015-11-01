#include <stdio.h>
#include <math.h>
#include "uthash.h"

bool first = false;
int items_added = 0;

int mapWidth = 10;
int mapHeight = 10;

int path[10][10] = {
    {0,0,0,0,0,0,1,1,1,1},
    {0,0,1,1,1,0,1,1,1,1},
    {0,1,1,1,1,0,1,1,1,1},
    {0,1,1,1,1,0,1,1,1,1},
    {0,0,0,0,1,0,1,1,1,1},
    {1,1,1,0,1,0,1,1,1,1},
    {1,1,0,0,1,0,1,1,1,1},
    {1,1,0,1,0,0,0,0,1,1},
    {1,1,0,1,1,0,1,0,0,1},
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
typedef struct closed_list CLOSED_LIST;
typedef struct Points POINT;

struct Points *point = NULL;
struct open_list *olist = NULL;
struct closed_list *clist = NULL;

int START_NODE = 0;
int END_NODE = 0;

struct Points *add_node(int _id, int x, int y, struct Points *parent) {

    struct Points *p = NULL;
    p = (POINT*)malloc(sizeof(POINT));
    p->id = _id;
    p->x = x;
    p->y = y;

    if(parent)
        p->parent = parent;

    HASH_ADD_INT(point, id, p);  /* id: name of key field */
    //printf("Adding nodeid:%d\n", _id);
    return p;
}

struct Points *find_node(int _id) {
    struct Points *s;
    HASH_FIND_INT( point, &_id, s );  
    return s;
}

void add_items(int sy=0, int sx=0, int dy=0, int dx=0) {
	START_NODE = (10*sy) + sx;
	END_NODE = (10*dy) + dx; 
    struct Points *parent = NULL;
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

void find_adj(int sy, int sx, int successors[]){

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
                successors[i] = start_node;
                break;

            case 1:
                start_node = (10*(sy+1))+sx;
                s_node = find_node(start_node);
                if(!s_node)
                    break;
                successors[i] = start_node;
            break;

            case 2:
                start_node = (10*sy)+sx-1;
                s_node = find_node(start_node);
                if(!s_node)
                    break;
                successors[i] = start_node;
            break;

            case 3:
                start_node = (10*(sy-1))+sx;
                s_node = find_node(start_node);
                if(!s_node)
                    break;
                successors[i] = start_node;
            break;

            case 4:
                start_node = (10*(sy-1))+sx+1;
                s_node = find_node(start_node);
                if(!s_node)
                    break;
                successors[i] = start_node;
            break;

            case 5:
                start_node = (10*(sy+1))+sx+1;
                s_node = find_node(start_node);
                if(!s_node)
                    break;
                successors[i] = start_node;
            break;

            case 6:
                start_node = (10*(sy+1))+sx-1;
                s_node = find_node(start_node);
                if(!s_node)
                    break;
                successors[i] = start_node;
            break;

            case 7:
                start_node = (10*(sy-1))+sx-1;
                s_node = find_node(start_node);
                if(!s_node)
                    break;
                successors[i] = start_node;
            break;
        }
    }

   // HASH_FIND_INT( point, &star_node, s );
}

void list_open(){
    struct open_list *s, *tmp;
    for(s=olist; s != NULL; s=(OPEN_LIST*)(s->hh.next)) {
    	printf("Node: x:%d y:%d - distance: %d\n", s->p->y, s->p->x);
    }
}

void list_points(){
    struct Points *s;

    for(s=point; s != NULL; s=(struct Points*)(s->hh.next)) {
        printf("S %d\n", &s->hh.next);
        printf("Node: id %d: x:%d y:%d\n", s->id, s->x, s->y);
    }
}

int manhatten(int sy, int sx, int dy, int dx) {
    int tx = abs(sx - dx);
    int ty = abs(sy - dy);
    return (2 * (tx + ty));
}

int find_lowest(){
    int lowest = 99999;
    int _id = 0;
    struct open_list *s, *tmp;

    //s = (OPEN_LIST*)malloc(sizeof(OPEN_LIST));
    //HASH_ITER(hh, olist, s, tmp) {
        //printf("Node: id %d: x:%d y:%d\n", s->id, s->p->x, s->p->y);
        //printf("We have now cycled through %d times \n", iter++);
    //}

    for(s=olist; s != NULL; s=(OPEN_LIST*)(s->hh.next)) {
        //printf("S %d\n", &s->hh.next);
        int cur = manhatten(s->p->y, s->p->x, END_NODE/10, END_NODE%10);
        printf("Node: x:%d y:%d - distance: %d\n", s->p->y, s->p->x, cur);
        if (cur < lowest){
        	lowest = cur;
        	_id = (10*s->p->y)+s->p->x;
        }
    }

    //OPEN_LIST *s;
    //for(s=olist; s != NULL; s=s->hh.next) {
    //    printf("user id %d: x:%d y:%d\n", s->id, s->p->x, s->p->y);
    //}
    
    return _id;
}

void search(int sy, int sx, int dy, int dx) {
    int lowest = 0;
    int start_node = (10*sy)+sx;
    struct Points *s_node = NULL;

    s_node = (struct Points*)malloc(sizeof(struct Points));
    s_node = find_node(start_node);

    //printf("Node: id %d: y:%d x:%d\n", s_node->id, s_node->y, s_node->x);

    struct open_list *o = NULL;
    struct closed_list *c = NULL;
    c = (CLOSED_LIST*)malloc(sizeof(CLOSED_LIST));

    int _id = s_node->id;
    o = (OPEN_LIST*)malloc(sizeof(OPEN_LIST));
    o->id = _id;
    o->p = s_node;
    o->p->fcost = 0;
    HASH_ADD_INT(olist, id, o);

    int successors[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    
    while(HASH_COUNT(olist) > 0){
    	lowest = find_lowest();
    	s_node = find_node(lowest);
    	
    	int fcost = s_node->fcost;
    	int gcost = s_node->gcost;

    	struct open_list *del = (OPEN_LIST*)malloc(sizeof(OPEN_LIST));
    	HASH_FIND_INT( olist, &s_node->id, del );
    	HASH_DEL(olist, del);
	    
	    find_adj(s_node->y, s_node->x, successors);	    
	    printf("Lowest found was: %d\n", lowest);

	    for(int i=0; i<8; i++){
	    	s_node = find_node(successors[i]);
	    	if ((10*s_node->y)+s_node->x == END_NODE)
	    		break;
	        printf("Inner loop: y:%d x:%d\n", s_node->y, s_node->x);
	        o->id = s_node->id;
    		o->p = s_node;
    		o->p->fcost = 0;
    		HASH_ADD_INT(olist, id, o);
	    }
	    //c->id = s_node->id

		//HASH_ADD_INT(olist, id, o);
		list_open();
		printf("Hashcount before end: %d \n", HASH_COUNT(olist));
		memset(successors, 0, sizeof(successors));
		break;
	}

    free(o);
    printf("Finished :)");
}

int main() {
    add_items(1,1,8,8);
    search(1,1,8,8);
    return 0;
}
