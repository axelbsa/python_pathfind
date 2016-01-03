import os
import sys
import time

from Queue import PriorityQueue

D = 10

def read_metadata(filename):
    with open(filename) as f:
        map_type = f.readline()
        height = int(f.readline().split()[1])
        width = int(f.readline().split()[1])
        f.readline()
        s_map = [[0 for x in range(width+1)] for x in range(height+1)]
        for i, line in enumerate(f):
            for j, char in enumerate(line):
                s_map[i][j] = char

    return height, width, s_map


def find_neighbours(uy, ux, width, height):
    neighbour_count = 0
    neighbours = [0 for x in range(4)]

    # find left and right neighbours
    if ux > 0:
        neighbours[neighbour_count] = ux * width + (uy - 1)
        neighbour_count += 1

    if ux < width - 1:
        neighbours[neighbour_count] = ux * width + (uy + 1)
        neighbour_count += 1

    if uy > 0:
        neighbours[neighbour_count] = (ux - 1) * width + uy
        neighbour_count += 1

    if uy < height - 1:
        neighbours[neighbour_count] = (ux + 1) * width + uy
        neighbour_count += 1
    return neighbours


def manhattan(sy, sx, dy, dx):
    gx = abs(sx - dx)
    gy = abs(sy - dy)
    return D * (gx + gy)


def reconstruct(rev_path, current):
    path = []
    for test2 in rev_path.keys():
        test = rev_path[test2]
        path.append(test)

    #print path
    return path


def search(s_map, sy, sx, dy, dx, start, end, g_cost, f_cost, height, width):
    print "Start is %d" % start
    open_list = PriorityQueue()
    open_list.put( (0, start) )
    closed_list = [0 for x in range(width * height)]
    marked = [0 for x in range(width * height)]
    rev_path = {}
    g_cost[start] = 0

    while open_list.qsize():
        current = open_list.get()[1]

        ux = current // width
        uy = current % width

        if current == end:
            print "found path"
            print "Current %d %d" % (uy, ux)
            return reconstruct(rev_path ,current)

        closed_list[current] = 1
        #print "Current y:%d x:%d" % (uy, ux)

        for next_node in find_neighbours(uy, ux, width, height):
            if closed_list[next_node] == 1:
                continue

            vx = next_node // width
            vy = next_node % width

            #print "\tNext node y:%d x:%d" % (vy, vx)

            tentative_score = g_cost[current] + 10

            #print "\t\tG_cost: %d | tentative_cost: %d" % (g_cost[next_node],
            #                                              tentative_score)

            if marked[next_node] == 1:
                continue
            if tentative_score < g_cost[next_node]:

                rev_path[next_node] = current
                g_cost[next_node] = tentative_score
                man_cost = manhattan(vy, vx, dy, dx)
                f_cost[next_node] = g_cost[next_node] + man_cost

                open_list.put( (f_cost[next_node], next_node))
                marked[next_node] = 1
                #print "\t\tEnding f_cost: %d" % f_cost[next_node]

def usage():
    return "./program <mapfile> <start_y> <start_x> <end_y> <end_x>"

if __name__ == '__main__':
    if len(sys.argv) < 6:
        print usage()
        sys.exit()

    filename = sys.argv[1]
    sy = int(sys.argv[2])
    sx = int(sys.argv[3])
    dy = int(sys.argv[4])
    dx = int(sys.argv[5])

    height, width, s_map = read_metadata(filename)

    start = (sx * width) + sy
    end = (dx * width) + dy

    print "Finding path for y:%d x:%d" % (sy, sx)

    g_cost = [(width * height + 1) for x in range( (width + 1) * (height + 1) )]
    f_cost = [(width * height + 1) for x in range( (width + 1) * (height + 1) )]

    start_tick = time.time()
    path = search(s_map, sy, sx, dy, dx, start, end, g_cost, f_cost, height, width)
    print "Time took, sec: %d" % (time.time() - start_tick)
