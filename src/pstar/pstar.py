import os
import sys

from Queue import PriorityQueue


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
		neighbours[neighbour_count] = uy * width + (ux - 1)
		neighbour_count += 1

	if ux < width - 1:
		neighbours[neighbour_count] = uy * width + (ux + 1)
		neighbour_count += 1

	if uy > 0:
		neighbours[neighbour_count] = (uy - 1) * width + ux
		neighbour_count += 1

	if uy < height - 1:
		neighbours[neighbour_count] = (uy + 1) * width + ux
		neighbour_count += 1

	return neighbours

def search(s_map, sy, sx, dy, dx, start, end, g_cost, f_cost, height, width):
    open_list = PriorityQueue()
    open_list.put(start)
    closed_list = [0 for x in range(width * height)]

    while open_list.qsize():
    	current = open_list.get()
    	print current
    	ux = current // width
    	uy = current % width

    	if current == end:
    		print "found path"
    		return (1, 2)

    	closed_list[current] = 1

    	for next_node in find_neighbours(uy, ux, width, height):
    		print next_node
    		open_list.put(start)

def usage():
	return """
	./program <mapfile> <start_y> <start_x> <end_y> <end_x>
	"""

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
	
	start = (sy * width) + sx
	end = (dy * width) + dx

	g_cost = [[width * height + 1 for x in range(width+1)] for x in range(height+1)]
	f_cost = [[width * height + 1 for x in range(width+1)] for x in range(height+1)]

	path = search(s_map, sy, sx, dy, dx, start, end, g_cost, f_cost, height, width)