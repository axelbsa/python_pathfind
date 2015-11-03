#include "common.h"
#include <stdlib.h>

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
            if (list[i-1]->id < list[i]->id) {
                POINT* tmp = list[i-1];
                list[i-1] = list[i];
                list[i] = tmp;
                break;
            }
        }
    } while (i != size);
}

static POINT* search(POINT** list, size_t size, int id) {
    size_t i;

    for (i = size / 2; i > 0 && i < size;) {
        if (id < list[i]->id) {
            i /= 2;
        } else if (id > list[i]->id) {
            i += i / 2;
        } else {
            return list[i];
        }
    }

    return id == list[0]->id ? list[0] : NULL;
}

void points_add(POINT* point) {
    POINT* i_node = (POINT*) malloc(sizeof(POINT));
    *i_node = *point;

    if (points == NULL) {
        points = (POINT**) malloc(sizeof(POINT*) * points_size_max);
    }

    if (points_size == points_max_size - 1) {
        points_size_max = points_size_max * 2;
        points = (POINT**) realloc((void*) points, sizeof(POINT*) * points_size_max);
    }

    points[size++] = i_node;
    sort(points, size);
}

POINT* points_find(int id) {
    return search(points, size, id);
}

void points_destroy() {
    for (size_t i = 0; i < size; ++i) {
        free(points[i]);
    }
    free(points);
    points = NULL;
}

void closed_add(POINT* point) {
    if (closed == NULL) {
        closed = (POINT**) malloc(sizeof(POINT*) * points_size_max);
    }

    if (closed_size == closed_max_size - 1) {
        closed_size_max *= 2;
        closed = (POINT**) realloc((void*) closed, sizeof(POINT*) * closed_size_max);
    }

    closed[size++] = point;
}

void closed_destroy() {
    free(closed);
    closed = NULL;
}
