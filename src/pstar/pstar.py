import os
import sys
import time

from Queue import PriorityQueue

D = 1.0
infinity = 512000

cost_lut = {}
cost_lut['.'] = 1.0
cost_lut['G'] = 1.0
cost_lut['@'] = infinity
cost_lut['O'] = infinity
cost_lut['T'] = infinity
cost_lut['S'] = 5.0
cost_lut['W'] = 15.0


def read_metadata(filename):
    with open(filename) as f:
        map_type = f.readline()
        assert map_type  # This field isn't used but assert it anyway
        height = int(f.readline().split()[1])
        width = int(f.readline().split()[1])
        f.readline()
        s_map = [[0 for x in range(width)] for y in range(height)]
        for i, line in enumerate(f):
            for j, char in enumerate(line):
                if j == width:
                    break
                s_map[i][j] = char

    return height, width, s_map


def reconstruct(rev_path, current, s_map, start, end):
    path = []
    node = rev_path[current]
    while node != 0:
        path.append(node)
        node = rev_path[node]

    return path


def write_map(s_map, start, end, path):
    with open("dump", 'w') as f:
        for i, line in enumerate(s_map):
            for j, char in enumerate(line):
                point = (j * width) + i
                point_found = 0
                for x in path:
                    if x == point:
                        point_found = 1
                if point == end:
                    f.write('E')
                elif point == start:
                    f.write('S')
                elif point_found:
                    f.write('v')
                else:
                    f.write(s_map[i][j])
            f.write("\n")


def find_neighbours(ux, uy, width, height, neighbours):
    neighbour_count = 0
    #print "Finding neighbours x:%d y:%d" % (ux, uy)

    # find left and right neighbours
    if ux > 0:
        neighbours[neighbour_count] = uy * width + (ux - 1)
        neighbour_count += 1

    if ux < width - 1:
        neighbours[neighbour_count] = uy * width + (ux + 1)
        neighbour_count += 1

    if uy > 0:
        neighbours[neighbour_count] = (uy - 1) * width + ux
        neighbour_count += 1

    if uy < height - 1:
        neighbours[neighbour_count] = (uy + 1) * width + ux;
        neighbour_count += 1

    # print neighbours, neighbour_count
    return neighbour_count


def octile(sx, sy, dx, dy):
    D2 = D
    gx = abs(sx - dx)
    gy = abs(sy - dy)
    return D * (gx + gy) + (D2 - 2 * D) * min(gx, gy)


def manhattan(sx, sy, dx, dy):
    gx = abs(sx - dx)
    gy = abs(sy - dy)
    return D * (gx + gy)


def search(s_map, sx, sy, dx, dy, start, end, g_cost, f_cost, height, width):
    print height, width

    # rev_path become closed_list somehow??? and marked???
    closed_list = []

    map_size = width * height
    open_list = PriorityQueue()
    open_list.put((0, start))
    marked = [0] * map_size
    rev_path = [0] * map_size
    neighbours = [0 for x in range(4)]
    g_cost[start] = 0
    f_cost[start] = 0  # manhattan(sx, sy, dx, dy)
    debug = 0

    while open_list.qsize():
        current = open_list.get()[1]
        ux = current % width
        uy = current // width
        # print "Current node:%d x:%d y:%d" % (current, ux, uy)

        if current == end:
            print "Path FOUND! current y:%d x:%d" % (ux, uy)
            path = reconstruct(rev_path, current, s_map, start, end)
            return path

        closed_list.append(current)
        marked[current] = 1

        if current < 0:
            debug = 1

        neighbour_count = find_neighbours(ux, uy, width, height, neighbours)
        for i in range(neighbour_count):
            next_node = neighbours[i]

            vx = next_node % width
            vy = next_node // width

            # print "\tNext node x:%d y:%d" % (vx, vy)

            s_value = s_map[vx][vy]
            lut = cost_lut[s_value]
            tentative_score = g_cost[current] + (1.0 * lut)
            if debug:
                print "\t\tG_cost: %d | tentative_cost: %d" % (g_cost[next_node], tentative_score)

            if marked[next_node] == 1:
                continue
            elif tentative_score < g_cost[next_node]:
                rev_path[next_node] = current
                g_cost[next_node] = tentative_score
                man_cost = manhattan(vx, vy, dx, dy)
                # man_cost = octile(vx, vy, dx, dy)
                f_cost[next_node] = g_cost[next_node] + man_cost
                open_list.put((f_cost[next_node], next_node))
                marked[next_node] = 1
                # print "\t\tHeuristic: %d, Ending f_cost: %d" % (man_cost,
                                                                  # f_cost[next_node])

    return "FAIL"


def usage():
    return "./program <mapfile> <start_x> <start_y> <end_x> <end_y>"


if __name__ == '__main__':
    if len(sys.argv) < 6:
        print usage()
        sys.exit()

    filename = sys.argv[1]
    sx = int(sys.argv[2])
    sy = int(sys.argv[3])
    dx = int(sys.argv[4])
    dy = int(sys.argv[5])

    height, width, s_map = read_metadata(filename)

    start = sx * width + sy
    end = dx * width + dy

    print "Finding path for y:%d x:%d" % (dy, dx)
    print "START: %d END:%d" % (start, end)

    g_cost = [(width + height + 1) for x in range( (width + 1) * (height + 1) )]
    f_cost = [(width + height + 1) for x in range( (width + 1) * (height + 1) )]

    start_tick = int(round(time.time()*1000))
    path = search(s_map, sx, sy, dx, dy, start, end, g_cost, f_cost, height, width)
    print "Time took, Msec: %d" % (int(round(time.time()*1000)) - start_tick)

    write_map(s_map, start, end, path)
#    print [("Y:%d X:%d") % (x//width,x%width) for x in path]
