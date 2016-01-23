#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define D 1.0 
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
    return 0;
}

int read_map(FILE* file, uint8_t* map, uint32_t width, uint32_t height)
{
    for (uint32_t i = 0, n = width * height; i < n; ++i)
    {
        int c;

        while ((c = fgetc(file)) != EOF && !valid(c));

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

int main(int argc, char** argv)
{
    long coordinates[4] = {-1, -1, -1, -1};

    if (argc < 6) {
        fprintf(stderr, "Usage: %s <start-x> <start-y> <target-x> <target-y> <map-file>\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc && i < 5; ++i)
    {
        char* pos = NULL;
        coordinates[i - 1] = strtol(argv[i], &pos, 0);

        if (pos == NULL || *pos != '\0' || coordinates[i-1] < 0)
        {
            fprintf(stderr, "'%s' is not a valid map coordinate\n", argv[i]);
            return 1;
        }
    }

    // Parse map file
    FILE* file = fopen(argv[5], "r");
    if (file == NULL) {
        fprintf(stderr, "'%s' is not a valid map\n", argv[5]);
        return EXIT_FAILURE;
    }

    // Extract map metadata (width and height) from file
    uint32_t width, height;
    if (!read_metadata(file, &width, &height)) {
        fclose(file);
        fprintf(stderr, "'%s' is not a valid map\n", argv[5]);
        return EXIT_FAILURE;
    }

    // Parse coordinates

    // Check that given coordinates are legal
    if (coordinates[0] >= width || coordinates[1] >= height) {
        fclose(file);
        fprintf(stderr, "(%ld,%ld) is not a point in the map!\n", coordinates[0], coordinates[1]);
        return EXIT_FAILURE;
    } else if (coordinates[0] >= width || coordinates[1] >= height) {
        fclose(file);
        fprintf(stderr, "(%ld,%ld) is not a point in the map!\n", coordinates[0], coordinates[1]);
        return EXIT_FAILURE;
    }
    
    size_t map_size = width * height;
    double* cost_lut = (double*) calloc(1, sizeof(double) * 256);

    uint8_t* map = (uint8_t*) malloc(sizeof(uint8_t) * map_size);
    uint32_t* path_tbl = (uint32_t*) calloc(1, sizeof(uint32_t) * map_size);

    double* f_costs = (double*) calloc(1, sizeof(double) * 256);
    double* g_costs = (double*) calloc(1, sizeof(double) * 133);

    printf("%d size", sizeof(uint32_t));
     
}
