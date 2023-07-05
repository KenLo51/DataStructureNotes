#include "Maze.h"
#include <vector>
#include <glm/glm.hpp>
int main() {
	Maze maze("maze.txt");
	std::vector<glm::ivec2> path = maze.Solve(glm::ivec2(1, 1), glm::ivec2(maze.w()-2, maze.h()-2));
	if (path.size() == 0) {
		printf("The path does not exit\n");
		return 0;
	}
	printf("The shortest path from begin to exit is:\n");
	printf("(%d, %d)\n", 0, 0);
	for (unsigned int i = 0; i < path.size(); i++) {
		printf("(%d, %d)\n", path[path.size()-i-1].y-1, path[path.size() - i - 1].x-1);
	}
	getchar();
	return 0;
}