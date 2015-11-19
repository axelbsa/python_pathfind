#include "common.h"
#include <stdlib.h>
#include <stdio.h>

POINT* ppoints[INIT_HEAP_SIZE];

// Holds the heap in memory
static POINT** heap = NULL;

// Number of elements in heap
static size_t size = 0;

// Maximum heap size
static size_t max_size = INIT_HEAP_SIZE;

POINT* open_search(int id) {
    for (int i=0; i < size; i++) {
        POINT *tmp = heap[i]; 
        if (tmp && tmp->id == id)
            return tmp;
    }
    return NULL;
}

void open_add(POINT *point) {
    if (heap == NULL) {
        heap = (POINT**) malloc(sizeof(POINT*) * max_size);
        heap[0] = NULL;
    }

    // We're running out of space, increase heap size
    if (size == max_size - 1) {
        max_size = max_size * 2;
        heap = (POINT**) realloc((void*) heap, sizeof(POINT*) * max_size);
    }
    
    // Percolate up
    size_t hole = ++size;
    for(; hole > 1 && point->fcost < heap[hole / 2]->fcost; hole /= 2) {
        heap[hole] = heap[hole / 2];
    }

    // Insert node in available space
    heap[hole] = point;
    ppoints[point->id] = point;
}

static void percolateDown(size_t hole) {
    size_t child;
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
    }

    heap[hole] = temp;
}

POINT* open_del() {
    if (size == 0) {
        return NULL;
    }

    POINT* min = heap[1];

    // Move last element to root and let it percolate down
    heap[1] = heap[size--];
    percolateDown(1);

    return min;
}

size_t open_size() {
    return size;
}

void open_destroy() {
    free(heap);
    heap = NULL;
}



#ifdef DEBUG

int main(void) {
    int i;

    for (i = 0; i < 10; ++i) {
        POINT* p = (POINT*) malloc(sizeof(POINT));
        p->id = i;
        p->fcost = rand() % 266;

        open_add(p);
    }

    while (open_size() != 0) {
        POINT* p = open_del();
        printf("%d %d\n", p->id, p->fcost);
        free(p);
    }

    return 0;
}

#endif
