import sys
import os
import time

import pathfinding as p


class ApsAstar():
    """
    Start X (sx), Start Y (sy),
    Destination X (dx), Destination Y (dy)
    World represents a 1D array (list) of the world
    factor? have no idea!

    Width and Height of the map for calculation of 1d array
    """

    astar = p.Astar()

    def __init__(self, sx, sy, dx, dy, world, width, height, factor=None):
        self.factor = factor
        self.world = world
        self.sx, self.sy = sx, sy
        self.dx, self.dy = dx, dy
        self.width = width
        self.height = height

    def load_map_from_file(self, path):
        with open(path) as f:
            for line in f.readline():
                pass
                # Do something with f

    def set_astar_params(self):
        # Not implemented
        astar.set_params()

    def search(self, ALLOW_DIAGONAL=1):
        path = self.astar.search(self.sx, self.sy, self.dx, self.dy,
                            self.world, self.width, self.height,
                            ALLOW_DIAGONAL)
        print "----"
        print "Path: ",
        print [(x % width, x // width) for x in path][::-1]
        print "----"


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
        height = int(f.readline().split()[1])
        width = int(f.readline().split()[1])
        f.readline()
        #s_map = [[0 for x in range(width)] for y in range(height)]
        s_map = [0] * (width * height)
        i = 0
        for k, line in enumerate(f):
            for j, char in enumerate(line):
                if j == 512:
                    break
                s_map[i] = char
                i += 1

    return height, width, s_map


def usage():
    return "./program <mapfile> <start_y> <start_x> <end_y> <end_x>"


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

    #def __init__(sx, sy, dx, dy, world, width, height, factor=None):
    astar = ApsAstar(sx, sy, dx, dy, s_map, width, height)
    astar.search()

    #start_tick = int(round(time.time()*1000))
    #path = search(s_map, sx, sy, dx, dy, start, end, g_cost, f_cost, height, width)
    #print "Time took, Msec: %d" % (int(round(time.time()*1000)) - start_tick)

    #write_map(s_map, start, end, path)
    #print [("Y:%d X:%d") % (x//width,x%width) for x in path]
