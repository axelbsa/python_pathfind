#include <stdio.h>
#include <math.h>
#include <memory>
#include "uthash.h"

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

int allow_diagonal = 0;

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
    struct Points *s_node = NULL;
    for(int i=0; i<8; i++){
        s_node = (struct Points*)malloc(sizeof(struct Points));

        _id = 0;
        start_node = 0;

        switch(i){
            case 0:
                start_node = (10*sy)+sx+1;
                s_node = find_node(start_node);
                if(s_node && path[sy][sx] == 0)
                	successors[i] = start_node;
                break;

            case 1:
                start_node = (10*(sy+1))+sx;
                s_node = find_node(start_node);
                if(s_node && path[sy][sx] == 0)
                	successors[i] = start_node;
            break;

            case 2:
                start_node = (10*sy)+sx-1;
                s_node = find_node(start_node);
                if(s_node && path[sy][sx] == 0)
                	successors[i] = start_node;
            break;

            case 3:
                start_node = (10*(sy-1))+sx;
                s_node = find_node(start_node);
                if(s_node && path[sy][sx] == 0)
                	successors[i] = start_node;
            break;

            case 4:
                start_node = (10*(sy-1))+sx+1;
                s_node = find_node(start_node);
                if(s_node && path[sy][sx] == 0)
                	successors[i] = start_node;
            break;

            case 5:
                start_node = (10*(sy+1))+sx+1;
                s_node = find_node(start_node);
                if(s_node && path[sy][sx] == 0)
                	successors[i] = start_node;
            break;

            case 6:
                start_node = (10*(sy+1))+sx-1;
                s_node = find_node(start_node);
                if(s_node && path[sy][sx] == 0)
                	successors[i] = start_node;
            break;

            case 7:
                start_node = (10*(sy-1))+sx-1;
                s_node = find_node(start_node);
                if(s_node && path[sy][sx] == 0)
                	successors[i] = start_node;
            break;
        }
    }
}

void list_closed(){
    struct closed_list *s;
    for(s=clist; s != NULL; s=(CLOSED_LIST*)(s->hh.next)) {
    	printf("Node: x:%d y:%d \n", s->p->y, s->p->x);
    }
}

void list_open(){
    struct open_list *s;
    for(s=olist; s != NULL; s=(OPEN_LIST*)(s->hh.next)) {
    	printf("Node: x:%d y:%d\n", s->p->y, s->p->x);
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
    int lowest = 999999999;
    int _id = 0;
    struct open_list *s, *tmp;

    for(s=olist; s != NULL; s=(OPEN_LIST*)(s->hh.next)) {
        //printf("S %d\n", &s->hh.next);
        int cur = manhatten(s->p->y, s->p->x, END_NODE/10, END_NODE%10);
        //printf("Node: x:%d y:%d - distance: %d\n", s->p->y, s->p->x, cur);
        if (cur < lowest){
        	lowest = cur;
        	_id = (10*s->p->y)+s->p->x;
        }
    }

    return _id;
}

void search(int sy, int sx, int dy, int dx) {
    int lowest = 0;
    int finished = 0;
    int start_node = (10*sy)+sx;
    struct Points *s_node = NULL;

    s_node = (struct Points*)malloc(sizeof(struct Points));
    s_node = find_node(start_node);

    //printf("Node: id %d: y:%d x:%d\n", s_node->id, s_node->y, s_node->x);

    struct open_list *o = NULL;
    struct closed_list *c = NULL;

    int _id = s_node->id;
    o = (OPEN_LIST*)malloc(sizeof(OPEN_LIST));
    o->id = _id;
    o->p = s_node;
    o->p->fcost = 0;
    HASH_ADD_INT(olist, id, o);

    int successors[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
    struct open_list *tmp = NULL;
    while(HASH_COUNT(olist) > 0 && finished == 0){
    	lowest = find_lowest();
    	s_node = find_node(lowest);
    	
    	int fcost = s_node->fcost;
    	int gcost = s_node->gcost;

    	struct open_list *del = (OPEN_LIST*)malloc(sizeof(OPEN_LIST));
    	HASH_FIND_INT( olist, &s_node->id, del );
    	HASH_DEL(olist, del);
	    
	    find_adj(s_node->y, s_node->x, successors);	    
	    printf("Lowest found was: %d\n", lowest);
       
        int successor_count = 4;
        if (allow_diagonal)
            successor_count = 8;

	    for(int i=0; i<successor_count; i++){
	    	if(successors[i] < 0)
	    		continue;
	    	s_node = find_node(successors[i]);
	    	if(!s_node)
	    		continue;
	    	if ((10*s_node->y)+s_node->x == END_NODE){
	    		printf("WE ARE HERE\n");
	    		finished = 1;
	    		break;
	    	}
	        //printf("Inner loop: y:%d x:%d\n", s_node->y, s_node->x);
	        tmp = (OPEN_LIST*)malloc(sizeof(OPEN_LIST));
	        tmp->id = s_node->id;
    		tmp->p = s_node;
    		tmp->p->fcost = 0;
    		HASH_ADD_INT(olist, id, tmp);
	    }
	    c = (CLOSED_LIST*)malloc(sizeof(CLOSED_LIST));
	    c->id = s_node->id;
	    c->p = s_node;
	    HASH_ADD_INT(clist, id, c);
		//list_open();
		//printf("Hashcount before end: %d \n", HASH_COUNT(olist));
		memset(successors, -1, sizeof(successors));
	}
	list_closed();
    free(o);
    free(c);
    free(s_node);
    free(tmp);
    
    printf("Finished :)\n");
}

int main() {
    add_items(1,1,8,8);
    search(1,1,8,8);
    return 0;
}
