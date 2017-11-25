""" Path class """


from sys import maxint
import copy


class Path:

    final_path = []

    def __init__(self, graph):
        self.graph = graph
        self.path = [None] * len(graph.points)
        self.level = 0
        self.weight = maxint
        self.__calculate_start_bound()
        self.tries = 0

    def __calculate_start_bound(self):
        bound = 0
        for point in self.graph.points:
            bound = bound + ((self.graph.get_nth_edge(point.id_num, 0) + self.graph.get_nth_edge(point.id_num, 1)) / 2)
        self.bound = bound

    def __update_bound(self):
        if self.level == 0:
            self.__calculate_start_bound()
        elif self.level == 1:
            self.bound -= ((self.graph.get_nth_edge(0, 0) + self.graph.get_nth_edge(1, 0)) / 2)
        else:
            self.bound -= ((self.graph.get_nth_edge(self.level - 1, 1) + self.graph.get_nth_edge(self.level, 0)) / 2)

    def __set_visited_at_level(self, value):
        self.graph.set_visited(self.path[self.level], value)

    def __recursive_bb_helper(self, weight):
        if self.level == self.graph.graph_width:
            temp_weight = weight + self.graph.distance(self.level - 1, 0)
            if temp_weight < self.weight:
                self.weight = temp_weight
                self.final_path = list(self.path)
                print "New path weight: {}".format(self.weight)
            return
        for point in self.graph.points:
            if not point.visited:
                new_weight = weight + self.graph.distance(self.path[self.level - 1], point.id_num)
                save_bound = self.bound
                self.__update_bound()
                if self.bound + new_weight < self.weight:
                    self.path[self.level] = point.id_num
                    self.__set_visited_at_level(True)
                    self.level += 1
                    self.__recursive_bb_helper(new_weight)
                    self.level -= 1
                    self.__set_visited_at_level(False)
                    self.path[self.level] = None
                self.bound = save_bound

    def branch_and_bound_tsp(self):
        self.path[self.level] = self.graph.points[0].id_num
        self.__set_visited_at_level(True)
        self.level += 1
        self.__recursive_bb_helper(0)
        self.print_results()

    def print_results(self):
        print self.weight
        for p in self.final_path:
            print p
