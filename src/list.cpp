#include <stdlib.h>
#include <stdio.h>
#include "common.h"

// Our "all points" list
static POINT** points = NULL;

// The size of "all points" list
static size_t points_size = 0;

// Maximum size of points_size
static size_t points_size_max = INIT_HEAP_SIZE;

// Our closed points list
static POINT** closed = NULL;
static size_t closed_size = 0;
static size_t closed_size_max = INIT_HEAP_SIZE;


// Bubble sort is efficient on nearly-sorted list
// TODO: Implement insertion sort instead
static void sort(POINT** list, size_t size) {
    size_t i;
    do {
        for (i = 1; i < size; ++i) {
            if (list[i]->id < list[i-1]->id) {
                POINT* tmp = list[i-1];
                list[i-1] = list[i];
                list[i] = tmp;
                break;
            }
        }
    } while (i != size);
}

// TODO: Implement binary search
static POINT* search(POINT** list, size_t size, int id) {
    for (size_t i = 0; i < size; ++i) {
        if (list[i]->id == id) {
            return list[i];
        }
    }

    return NULL;
}

static POINT* c_search(POINT** list, size_t size, int id) {
    for (size_t i = 0; i < size; ++i) {
        if (list[i]->id == id) {
            return list[i];
        }
    }

    return NULL;
}

void list_closed(size_t size) {
    printf("\t\tPRINTING CLOSED LIST \n");
    for (size_t i = 0; i < size; i++) {
        printf("\t\tCurrent=%d%d Parrent=%d%d\n", 
                closed[i]->y,
                closed[i]->x,
                closed[i]->parent ? closed[i]->parent->y : 0,
                closed[i]->parent ? closed[i]->parent->x : 0
        );
    }
}

POINT* points_add(POINT* point) {
    if (points == NULL) {
        points = (POINT**) malloc(sizeof(POINT*) * points_size_max);
    }

    if (points_size == points_size_max - 1) {
        points_size_max = points_size_max * 2;
        points = (POINT**) realloc((void*) points, sizeof(POINT*) * points_size_max);
    }

    points[points_size] = (POINT*) malloc(sizeof(POINT)); 
    *points[points_size] = *point;
    ++points_size;
    //sort(points, points_size);
    return points[points_size-1];
}

POINT* points_find(int id) {
    return search(points, points_size, id);
}

POINT* closed_find(int id) {
    return search(closed, closed_size, id);
}


void cl_print() {
    list_closed(closed_size);
}



void points_destroy() {
    for (size_t i = 0; i < points_size; ++i) {
        free(points[i]);
    }
    free(points);
    points = NULL;
}

void closed_add(POINT* point) {
    if (closed == NULL) {
        closed = (POINT**) malloc(sizeof(POINT*) * points_size_max);
    }

    if (closed_size == closed_size_max - 1) {
        closed_size_max *= 2;
        closed = (POINT**) realloc((void*) closed, sizeof(POINT*) * closed_size_max);
    }

    closed[closed_size++] = point;
}

void closed_destroy() {
    free(closed);
    closed = NULL;
}
