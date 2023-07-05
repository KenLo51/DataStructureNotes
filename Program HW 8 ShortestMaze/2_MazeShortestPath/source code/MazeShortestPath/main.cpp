#include "Maze.h"
int main() {
	Maze maze("maze.txt");
	int pathLen;
	ivec2 startPoint(1, 1);
	ivec2 endPoint(maze.w()-2, maze.h() - 2);
	ivec2* path = maze.Solve(startPoint, endPoint, &pathLen);
	if (path == NULL) {
		printf("The path does not exit\n");
		return 0;
	}
	printf("The shortest path from begin to exit is:\n");
	printf("(%d, %d)\n", 0, 0);
	for (unsigned int i = 0; i < pathLen; i++) {
		printf("(%d, %d)\n", path[pathLen - i - 1].y - 1, path[pathLen - i - 1].x - 1);
	}
	getchar();
	return 0;
}