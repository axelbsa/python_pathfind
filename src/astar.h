#ifndef ASTAR_H_   /* Include guard */
#define ASTAR_H_


#include <stdint.h>

extern uint32_t search(
        const uint8_t* map, uint32_t width, uint32_t height, 
        double* cost_lut, uint32_t* rev_path, double* f_costs, double* g_costs,
        uint32_t* open_list, uint8_t* closed_list,
        uint32_t start, uint32_t target
        );

extern int init(uint32_t sx, uint32_t sy, uint32_t dx, uint32_t dy,
        uint32_t* world, uint32_t width, uint32_t height, uint32_t* path
        );

extern char* test();

#endif // FOO_H_
