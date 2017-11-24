import math


class Point:
    def __init__(self, id_num, x, y):
        self.id_num = id_num
        self.x = x
        self.y = y
        self.visited = False

    def dist_to(self, point):
        x_dist = self.x - point.x
        y_dist = self.y - point.y
        return math.sqrt(x_dist**2 + y_dist**2)
