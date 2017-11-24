from point import Point


class Graph:
    def __init__(self, filename):
        self.__create_points(filename)
        self.__create_graph()
        self.graph_width = len(self.matrix)

    def __create_graph(self):
        self.matrix = [[0] * len(self.points) for _ in self.points]
        for i in range(len(self.points)):
            for j in range(len(self.points)):
                if i != j:
                    self.matrix[i][j] = self.points[i].dist_to(self.points[j])

    def __create_points(self, filename):
        read_buf = []
        with open(filename, 'r') as r:
            for line in r:
                line = line.rstrip()
                if not line: continue
                read_buf.append(line)

        # convert array of strings to an array of points
        self.points = [Point(x[0], x[1], x[2]) for x in [[int(y) for y in line.split(' ')] for line in read_buf]]

    def is_visited(self, point_num):
        return self.points[point_num].visited

    def set_visited(self, point_num, value):
        self.points[point_num].visited = value

    def get_points(self, is_visited=None):
        if is_visited is None:
            return self.points
        else:
            return [x for x in self.points if x.visited == is_visited]

    def get_nth_edge(self, point_num, n):
        if n > self.graph_width:
            return None
        else:
            return sorted(self.matrix[point_num])[n]

    def get_starting_bound(self):
        bound = 0
        for point in self.points:
            bound = bound + ((self.get_nth_edge(point.id_num, 1) + self.get_nth_edge(point.id_num, 2)) / 2)
        return bound