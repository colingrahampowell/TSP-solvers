from point import Point


class Graph:
    def __init__(self, filename):
        self.__create_points(filename)
        self.__create_graph()
        self.__create_nth_references()
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

    def get_nth_edge(self, point_num, n):
        if n > 2:
            return None
        else:
            return self.nth_references[point_num][n]

    def distance(self, p1, p2):
        return self.matrix[p1][p2]

    def __create_nth_references(self):
        self.nth_references = [[None] * 2 for _ in self.points]
        for i in range(len(self.matrix)):
            sorted_row = sorted(self.matrix[i])
            self.nth_references[i][0] = sorted_row[0]
            self.nth_references[i][1] = sorted_row[1]
