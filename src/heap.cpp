#include "common.h"
#include <stdlib.h>

// Holds the heap in memory
static POINT** heap = NULL;

// Number of elements in heap
static size_t size = 0;

// Maximum heap size
static size_t max_size = INIT_HEAP_SIZE;

void heap_add(POINT *point) {
    if (heap == NULL) {
        heap = (POINT**) malloc(sizeof(POINT*) * max_size);
    }

    // We're running out of space, increase heap size
    if (size == max_size - 1) {
        max_size = max_size * 2;
        heap = (POINT**) realloc((void*) heap, sizeof(POINT*) * max_size);
    }
    
    // Percolate up
    int hole = ++size;
    for( ; hole > 1 && point->fcost < heap[hole / 2]->fcost; hole /= 2) {
        heap[hole] = heap[hole / 2];
    }

    // Insert node in available space
    heap[hole] = point;
}

static void percolateDown(int hole) {
    int child;
    POINT* temp = heap[hole];

    for (; hole * 2 <= size; hole = child) {
        child = hole * 2;

        // select smallest child
        if (child != size && heap[child+1]->fcost < heap[child]->fcost) {
            ++child;
        }

        // move smallest child upwards and continue to sink
        if (heap[child]->fcost < temp->fcost) {
            heap[hole] = heap[child];
        } else {
            break;
        }

        heap[hole] = temp;
    }
}

POINT* heap_del() {
    if (size == 0) {
        return NULL;
    }

    POINT* min = heap[1];

    // Move last element to root and let it percolate down
    heap[1] = heap[size--];
    percolateDown(1);

    return min;
}

size_t heap_size() {
    return size;
}

void heap_destroy() {
    free(heap);
    heap = NULL;
}



#ifdef DEBUG
#include <stdio.h>

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

struct Points *add_node(int _id, int x, int y, struct Points *parent) {

    struct Points *p = NULL;
    p = (POINT*)malloc(sizeof(POINT));
    p->id = _id;
    p->x = x;
    p->y = y;
    p->fcost = rand() % 20;

    if(parent)
        p->parent = parent;

    //printf("Adding nodeid:%d\n", _id);
    heap_add(p);
    return p;
}

void add_items(int sy=0, int sx=0, int dy=0, int dx=0) {
	int START_NODE = (10*sy) + sx;
	int END_NODE = (10*dy) + dx; 
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
        }
        printf("\n");
    }
}

int main(void) {
    add_items(0, 0, 9, 9);

    while (heap_size() != 0) {
        POINT* p = heap_del();
        printf("%d %d\n", p->id, p->fcost);
    }
    return 0;
}

#endif
