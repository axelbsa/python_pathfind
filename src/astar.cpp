#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>

#define D 1.0 
#define NUDGE_FACTOR 0.001
#define ALLOW_DIAGONAL 1

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define abs(x) ((x)<0 ? -(x) : (x))

#define infinity 512000

std::string test() {
    return "A Simple Test";
}

inline int valid(int c)
{
    const char* obstacles = ".G@OTSW";
    for (int i = 0; i < 7; ++i)
    {
        if (c == obstacles[i])
        {
            return 1;
        }
    }

    // ILLIGAL CHARACTER
    return 0;
}

int read_map(uint32_t* file, uint8_t* map, uint32_t width, uint32_t height)
{
    for (uint32_t i = 0, n = width * height; i < n; ++i)
    {
        int c = file[i];

        // Uncomment if we need to check for bad char's
        //while (!valid(c)) {}

        if (c == EOF)
        {
            return 0;
        }
        map[i] = c & 255;

    }

    return 1;
}

inline void heap_insert(uint32_t* heap, uint32_t* heap_size, double* fcosts, uint32_t point)
{
    uint32_t hole;

    for (hole = ++*heap_size; hole > 1 && fcosts[point] < fcosts[heap[hole >> 1]]; hole >>= 1)
    {
        heap[hole] = heap[hole >> 1];
    }

    heap[hole] = point;
}

inline uint32_t heap_remove(uint32_t* heap, uint32_t* heap_size, double* fcosts)
{
    uint32_t removed_point = heap[1];

    uint32_t point = heap[1] = heap[(*heap_size)--];
    uint32_t hole, child;

    for (hole = 1; (hole << 1) <= *heap_size; hole = child)
    {
        child = hole << 1;

        if (child != *heap_size && fcosts[heap[child + 1]] < fcosts[heap[child]])
        {
            ++child;
        }

        if (fcosts[heap[child]] < fcosts[point])
        {
            heap[hole] = heap[child];
        }
        else
        {
            break;
        }
    }

    heap[hole] = point;

    return removed_point;
}

int read_metadata(FILE* file, uint32_t* width, uint32_t* height)
{
    char line[2048];
    char* pos;
    
    int status = 0;

    for (int i = 0; i < 4 && !feof(file); ++i)
    {
        fgets(line, sizeof(line), file);

        for (size_t j = 0; j < sizeof(line); ++j)
        {
            if (line[j] < 0x20 || line[j] > 0x7e)
            {
                line[j] = '\0';
                break;
            }
        }

        if (strncmp("width ", line, 6) == 0)
        {
            pos = NULL;
            *width = strtoul(&line[6], &pos, 0);
            if (pos == NULL || *pos != '\0')
            {
                return 0;
            }
            status |= 1;
        }
        else if (strncmp("height ", line, 7) == 0)
        {
            pos = NULL;
            *height = strtoul(&line[7], &pos, 0);
            if (pos == NULL || *pos != '\0')
            {
                return 0;
            }
            status |= 2;
        }
    }

    return !!(status & 3);
}


uint32_t cross(uint32_t sx, uint32_t sy,
               uint32_t cx, uint32_t cy,
               uint32_t dx, uint32_t dy)
{
    uint32_t gx1 = sx - dx;
    uint32_t gy1 = sy - dy;
    
    uint32_t gx2 = cx - dx;
    uint32_t gy2 = cy - dy;

    return abs(gx1 * gy2 - gx2 * gy1);
}

double octile_distance(uint32_t sx, uint32_t sy,
                       uint32_t dx, uint32_t dy)
{
    double D2 = 1.414;
    uint32_t gx = abs(sx - dx);
    uint32_t gy = abs(sy - dy);

    return D * (gx + gy) + (D2 - 2 * D) * MIN(gx, gy);
}


double manhattan(uint32_t sx, uint32_t sy,
                 uint32_t dx, uint32_t dy)
{
    uint32_t gx = abs(sx - dx);
    uint32_t gy = abs(sy - dy);

    return (D * (gx + gy));
}

uint32_t find_neighbours(int ux, int uy, int width, int height, uint32_t* neighbours, uint32_t num_neighbours = 8){
 
    //void find_adj(int sy, int sx, int* successors){

    /* Representation of adjecent nodes
     * +-----+
     * |7|3|4|
     * |2|S|0|
     * |6|1|5|
     * +-----+
     */
    
    // XXX Check that this actually has correct vals for 1D array and stuff
    // XXX Also do boundry check
    uint32_t start_node;
    for(uint32_t i = 0; i < num_neighbours; i++){

        start_node = 0;

        switch(i) {
            case 0:
                if (ux < width - 1)
                    start_node = (width * uy) + ux + 1;
                break;

            case 1:
                if (uy < height - 1)
                    start_node = (width * (uy + 1)) + ux;
                break;

            case 2:
                if (ux > 0)
                    start_node = (width * uy) + ux - 1;
                break;

            case 3:
                if (uy > 0)
                    start_node = (width * (uy - 1)) + ux;
                break;

            case 4:
                start_node = (width * (uy - 1)) + ux + 1;
                break;

            case 5:
                start_node = (width * (uy + 1)) + ux + 1;
                break;

            case 6:
                start_node = (width * (uy + 1)) + ux - 1;
                break;

            case 7:
                start_node = (width * (uy - 1)) + ux - 1;
                break;
        }
        neighbours[i] = start_node;

    }

    return num_neighbours;
}


uint32_t reconstruct(uint32_t* rev_path, uint32_t* path, uint32_t current, uint32_t width)
{
    uint32_t path_size = 0;
    uint32_t node = rev_path[current];

    while (node != 0) {
        //printf("Node: x=%d, y=%d\n", node % width, node / width);
        //path.append(node);
        path[path_size++] = node;
        node = rev_path[node];
    }

    return path_size;
}

uint32_t search(
        const uint8_t* map, uint32_t width, uint32_t height, 
        double* cost_lut, uint32_t* rev_path, double* f_costs, double* g_costs,
        uint32_t* open_list, uint8_t* closed_list,
        uint32_t start, uint32_t target
        )
{

    uint32_t neighbours[8];
    uint32_t open_list_size = 0;

    uint32_t sx = start % width;
    uint32_t sy = start / width;

    uint32_t tx = target % width;
    uint32_t ty = target / width;

    for (uint32_t i = 0; i < width * height; i++) {
        g_costs[i] = width * height + 1.0;
        f_costs[i] = width * height + 1.0;
    }

    printf("w:%d h:%d start:%d end:%d\n", width, height, start, target);

    cost_lut['.'] = 1.0;
    cost_lut['G'] = 1.0;
    cost_lut['@'] = infinity;
    cost_lut['O'] = infinity;
    cost_lut['T'] = infinity;
    cost_lut['S'] = 5.0;
    cost_lut['W'] = 15.0;

    if (cost_lut[map[target]] == infinity) {
        printf("YOU ARE TRYING TO GET INSIDE A WALL!\n");
        return 0x0;
    }

    double (*heuristic)(uint32_t, uint32_t, uint32_t, uint32_t);
    
    if (ALLOW_DIAGONAL) 
        heuristic = &octile_distance;
    else 
        heuristic = &manhattan;

    g_costs[start] = f_costs[start] = 0; 
    heap_insert(open_list, &open_list_size, f_costs, start);

    while (open_list_size > 0) {

        uint32_t current = heap_remove(open_list, &open_list_size, f_costs);
        uint32_t ux = current % width;
        uint32_t uy = current / width;

        // check if u is our target
        if (current == target) {
            //uint32_t path[height * width];
            return current;
            //return current;
        }

#ifdef DEBUG
        printf("Olist_size=%d Neighbour X:%d Y:%d node:%d\n", open_list_size, ux, uy, current);
#endif
        closed_list[current] = 1;

        uint32_t num = (ALLOW_DIAGONAL == 0) ? 4 : 8;

        uint32_t neighbour_count = find_neighbours(ux, uy, width, height, neighbours, num);
        for(uint32_t i = 0; i < neighbour_count; i++ ) {
            uint32_t next_node = neighbours[i];
            
            if (closed_list[next_node] == 1.0) {
                continue; 
            }
            
            uint32_t vx = next_node % width;
            uint32_t vy = next_node / width;

            uint8_t map_node = map[next_node];
            double lut = cost_lut[map_node];

            double movement_cost = 1.0;

            if(i > 3)
                movement_cost = 1.414; // If diagonal adjust the cost

            //g_costs[next_node] = g_costs[current] + movement_cost;
            double tentative_score = (g_costs[current] + lut + movement_cost);

#ifdef DEBUG
            printf("\tNeighbour X:%d Y:%d\n", vx, vy);
            printf("\tNode score: %g + %g = %g\n", lut, g_costs[current], tentative_score);
            printf("\t Gcost_next: %g \n", g_costs[next_node]);
            printf("\t tentative_score: %g \n", tentative_score);
#endif

            if (tentative_score < g_costs[next_node]) {
                rev_path[next_node] = current;
                g_costs[next_node] = tentative_score;

                //double man_costs = heuristic(vx, vy, tx, ty);
                double man_costs = octile_distance(vx, vy, tx, ty);
                if(!ALLOW_DIAGONAL)
                    man_costs += cross(vx, vy, sx, sy, tx, ty) * NUDGE_FACTOR;
                
                f_costs[next_node] = g_costs[next_node] + man_costs;
                heap_insert(open_list, &open_list_size, f_costs, next_node);

            }else{ /* If you end up here, you are lost */}
        }

    }


    return 0x0;
}

void free_all(double* a, uint8_t* b,
              uint32_t* c, double* d, 
              double* e, uint32_t* f,
              uint8_t* g) {

    free(a); free(b); free(c);
    free(d); free(e); free(f);
    free(g);
}

int init(uint32_t sx, uint32_t sy, uint32_t dx, uint32_t dy,
        uint32_t* world, uint32_t width, uint32_t height)
{
    long coordinates[4] = {-1, -1, -1, -1};

    //if (argc < 6) {
        //fprintf(stderr, "Usage: %s <start-x> <start-y> <target-x> <target-y> <map-file>\n", argv[0]);
        //return 1;
    //}

    //for (int i = 1; i < argc && i < 5; ++i)
    //{
        //char* pos = NULL;
        //coordinates[i - 1] = strtol(argv[i], &pos, 0);

        //if (pos == NULL || *pos != '\0' || coordinates[i-1] < 0)
        //{
            //fprintf(stderr, "'%s' is not a valid map coordinate\n", argv[i]);
            //return 1;
        //}
    //}

    // Parse map file
    //FILE* file = fopen(argv[5], "r");
    //if (file == NULL) {
        //fprintf(stderr, "'%s' is not a valid map\n", argv[5]);
        //return EXIT_FAILURE;
    //}

    //// Extract map metadata (width and height) from file
    //uint32_t width, height;
    //if (!read_metadata(file, &width, &height)) {
        //fclose(file);
        //fprintf(stderr, "'%s' is not a valid map\n", argv[5]);
        //return EXIT_FAILURE;
    //}

    // Parse coordinates

    // Check that given coordinates are legal
    //if (coordinates[0] >= width || coordinates[1] >= height) {
        //fclose(file);
        //fprintf(stderr, "(%ld,%ld) is not a point in the map!\n", coordinates[0], coordinates[1]);
        //return EXIT_FAILURE;
    //} else if (coordinates[0] >= width || coordinates[1] >= height) {
        //fclose(file);
        //fprintf(stderr, "(%ld,%ld) is not a point in the map!\n", coordinates[0], coordinates[1]);
        //return EXIT_FAILURE;
    //}

    size_t map_size = width * height;
    double* cost_lut = (double*) malloc(sizeof(double) * 256);

    uint8_t* map = (uint8_t*) malloc(sizeof(uint8_t) * map_size * 2);
    uint32_t* path_tbl = (uint32_t*) malloc(sizeof(uint32_t) * map_size);

    double* f_costs = (double*) malloc(sizeof(double) * map_size * 8);
    double* g_costs = (double*) malloc(sizeof(double) * map_size * 8);

    uint32_t* olist = (uint32_t*) malloc(sizeof(uint32_t) * map_size * 8);
    uint8_t* clist = (uint8_t*) malloc(sizeof(uint32_t) * map_size);

    memset(map, 1.0, sizeof(uint8_t) * map_size * 2);

    memset(f_costs, map_size + 1, sizeof(double) * map_size * 8);
    memset(g_costs, map_size + 1, sizeof(double) * map_size * 8);

    memset(cost_lut, 1.0, sizeof(double) * 256);
    memset(olist, map_size, sizeof(uint32_t) * map_size * 8);
    memset(clist, -1, sizeof(uint8_t) * map_size);

    // Parse map file
    if (!read_map(world, map, width, height))
    {
        free_all(cost_lut, map, path_tbl, f_costs, g_costs, olist, clist);
        fprintf(stderr, "Couldn't parse map\n");
        return 1;
    }

    //fclose(file);

    // Make a cost look-up table
    //set_cost_lut(cost_lut, width * height + 1);
    //uint32_t start = coordinates[1] * width + coordinates[0];
    //uint32_t target = coordinates[3] * width + coordinates[2];
    uint32_t start = sy * width + sx;
    uint32_t target = dy * width + dx;

    clock_t t = clock();
    uint32_t exit_point = search(map, width, height, cost_lut, path_tbl, f_costs, g_costs, olist, clist, start, target);
    
    t = clock() - t;
    fprintf(stderr, "It took me (%f seconds).\n",((float)t)/CLOCKS_PER_SEC);

    if(exit_point) 
        fprintf(stderr, "PATH FOUND\n");
    else
        fprintf(stderr, "Error, could not find any path\n");


    uint32_t* path = (uint32_t*)malloc(sizeof(uint32_t) * map_size);
    uint32_t path_size = reconstruct(path_tbl, path, exit_point, width);
    int mark = 0;
    for (uint32_t i = 0; i < width * height; i++) {

        if((i % width) == 0) 
            printf("\n");

        for (uint32_t y = 0; y < path_size; y++) {
            if (path[y] == i) {
                mark = 1;
            } 
        }
        if (mark)
            printf("V");
        else
            printf("%c",map[i]);
        mark = 0;

    }


    free_all(cost_lut, map, path_tbl, f_costs, g_costs, olist, clist);

}
