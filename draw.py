import numpy as np 
from scipy.spatial import Voronoi, voronoi_plot_2d 
import matplotlib.pyplot as plt 
import sys

def get_point(line, n=2):
	line_mod = line.replace(",", "")
	line_mod = line_mod.replace("\n", "")

	point = tuple(map(float, line_mod.split()))

	if len(point) != n:
		raise AssertionError(line)
	else:
		return point

def get_edge(line):
	stripped = line.strip("\n").split(" ")
	try:
		first, second = stripped[0], stripped[1]

		first = tuple(map(float, first.split(",")))
		second = tuple(map(float, second.split(",")))

		return (first, second)
	except IndexError:
		print(line)
		print(stripped)
		raise AssertionError




def read_points(filename):
	vertices = list()

	try:
		fp = open(filename, "r")
		
		line = fp.readline()

		while line:
			point = get_point(line)
			vertices.append(point)
			line = fp.readline()

	except FileNotFoundError as e:
		print("FILE NOT FOUND IN POINTS READING")
	finally:
		fp.close()

	return np.array(vertices)


def read_edges(filename):
	edges = list()

	try:
		fp = open(filename, "r")
		
		line = fp.readline()

		while line:
			edges.append(get_edge(line))

			line = fp.readline()

	except FileNotFoundError as e:
		print("FILE NOT FOUND IN EDGE READING")
	finally:
		fp.close()

	return np.array(edges)

def plot_lines(edges):
	for e in edges:
		plt.plot(e[0], e[1], "r-")

def plot_points(points):
	for p in points:
		plt.plot(p[0], p[1], "b+")


def flip_coords(edges):
	new_edges = list()
	for e in edges:
		new_e = [(e[0][0], e[1][0]), (e[0][1], e[1][1])] 
		new_edges.append(new_e)
	return new_edges



if __name__ == "__main__":

	if len(sys.argv) != 3:
		print("DRAWING FAILED\nUsage: ", sys.argv[0], "edges.txt points.txt")
	else:
		points = read_points(sys.argv[2])
		edges = read_edges(sys.argv[1])
		plot_lines(flip_coords(edges))
		plot_points(points)
		plt.show()
