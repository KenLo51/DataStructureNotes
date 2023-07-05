#include "Maze.h"

const char Maze::ROAD = 0;
const char Maze::BARRIER = 1;

Maze::Maze(unsigned int w, unsigned int h) {
	width = w + 2;
	height = h + 2;
	map = new char[width * height];
	for (unsigned int i = 0; i < width * height; i++)
		map[i] = (i % width == 0) ? Maze::BARRIER :
			(i % width == (width-1)) ? Maze::BARRIER :
			(i / height == 0) ? Maze::BARRIER :
			(i / height == (height - 1)) ? Maze::BARRIER : Maze::ROAD;

}
Maze::Maze(const char* fileName) {
	std::ifstream ifs(fileName, std::ifstream::in);
	ifs >> height >> width;
	width = width + 2;
	height = height + 2;
	map = new char[width * height];
	for (unsigned int i = 0; i < width * height; i++) {
		if ((i % width == 0) ||
			(i % width == (width - 1))||
			(i / width == 0) ||
			(i / width == (height - 1))) {
			map[i] = Maze::BARRIER;
		}
		else {
			ifs >> map[i];
			map[i] = map[i] - '0';
		}
	}
	ifs.close();

	return;
}


void Maze::load(const char* fileName) {
	std::ifstream ifs(fileName, std::ifstream::in);
	ifs >> height >> width;
	width = width + 2;
	height = height + 2;
	if (map) delete[] map;
	map = new char[width * height];
	for (unsigned int i = 0; i < width * height; i++) {
		if ((i % width == 0) ||
			(i % width == (width - 1)) ||
			(i / width == 0) ||
			(i / width == (height - 1))) {
			map[i] = Maze::BARRIER;
		}
		else {
			ifs >> map[i];
			map[i] = map[i] - '0';
		}
	}
	ifs.close();
}

void Maze::save(const char* fileName) {
	std::ofstream ofs(fileName, std::ofstream::out);
	ofs << height-2 << "\t" << width-2 << "\n";
	for (unsigned int row = 1; row < height - 1; row++) {
		for (unsigned int col = 1; col < width - 1; col++) {
			ofs << (int)map[row * width + col] << "\t";
		}
		ofs << "\n";
	}
	ofs.close();
}

void Maze::resize(unsigned int w, unsigned int h) {
	w = w + 2;
	h = h + 2;
	char* newMap = new char[w * h];
	for (unsigned int i = 0; i < w * h; i++) {
		newMap[i] = Maze::BARRIER;
	}

	int max_w = w ;
	if (max_w > width) max_w = width;
	int max_h = h;
	if (max_h > height) max_h = height;

	for (unsigned int row = 0; row < max_h-1; row++) {
		memcpy(newMap + row * w, map + row * width, max_w);
		newMap[row * w] = newMap[(row + 1) * w - 1] = Maze::BARRIER;
	}
	newMap[1 * w + 1] = Maze::ROAD;
	newMap[(h-2) * w + (w-2)] = Maze::ROAD;

	height = h;
	width = w;
	if (map) delete[] map;
	map = newMap;
}

std::vector<glm::ivec2> Maze::Solve(glm::ivec2 startPoint, glm::ivec2 endPoint) {
	startPoint = startPoint;
	endPoint = endPoint;
	int* distance = new int[width * height];
	if (map[startPoint.x + startPoint.y * width] == Maze::BARRIER) {
		std::vector<glm::ivec2> path;
		return path;
	}
	for (unsigned int i = 0; i < width * height; i++)
		distance[i] = INT_MAX;
	distance[startPoint.x + startPoint.y * width] = 0;

	// find the distance between startPoint and any other point bt BFS
	std::queue<glm::ivec2> nextPoints;
	nextPoints.push(startPoint);
	while (1) {
		if (nextPoints.size() == 0) break; // the path does not exist
		if (nextPoints.front() == endPoint) break;// found the path;

		glm::ivec2 currPoint = nextPoints.front();
		nextPoints.pop();
		const glm::ivec2 neighbors[] = { currPoint + glm::ivec2(1, 0),
										currPoint + glm::ivec2(0, 1),
										currPoint + glm::ivec2(-1, 0),
										currPoint + glm::ivec2(0, -1) };
		for (unsigned int i = 0; i < 4; i++) {
			if (map[neighbors[i].x + neighbors[i].y * width] == Maze::BARRIER) continue; // does not have road
			if (distance[neighbors[i].x + neighbors[i].y * width] != INT_MAX) continue; // traveled
			distance[neighbors[i].x + neighbors[i].y * width] = distance[currPoint.x + currPoint.y * width] + 1;
			nextPoints.push(neighbors[i]);
		}
	}

	// get the path from endPoint to startPoint
	std::vector<glm::ivec2> path;
	if (distance[endPoint.x + endPoint.y * width] == INT_MAX) return path; // no path exist
	glm::ivec2 currPoint = endPoint;
	while (1) {
		if (currPoint == startPoint) break;
		path.push_back(currPoint);
		const glm::ivec2 neighbors[] = { currPoint + glm::ivec2(1, 0),
										currPoint + glm::ivec2(0, 1),
										currPoint + glm::ivec2(-1, 0),
										currPoint + glm::ivec2(0, -1) };
		glm::ivec2 minDistPoint = neighbors[0];
		for (unsigned int i = 1; i < 4; i++) {
			int dist1 = distance[minDistPoint.x + minDistPoint.y * width];
			int dist2 = distance[neighbors[i].x + neighbors[i].y * width];
			if (dist2 < dist1) minDistPoint = neighbors[i];
		}
		currPoint = minDistPoint;
	}

	return path;
}