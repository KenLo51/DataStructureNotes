#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <queue>
#include <climits>
#include <string>
#include <fstream>
#include <iostream>
class Maze {
public:
	static const char ROAD, BARRIER;
public:
	unsigned int width, height;
	char* map;
public:
	Maze(unsigned int w = 1, unsigned int h = 1);
	Maze(const char*);

	void load(const char*);
	void save(const char*);
	void resize(unsigned int w, unsigned int h);
	unsigned int w() { return width; };
	unsigned int h() { return height; };
	char* data() { return map; };
	char get(unsigned int x, unsigned int y) { 
		unsigned int index = x + y * width;
		return (index < width* height) ? map[index] : 0;
	};
	void set(unsigned int x, unsigned int y, char val) {
		unsigned int index = x + y * width;
		if (index >= width * height) return;
		map[index] = val;
	};
	std::vector<glm::ivec2> Solve(glm::ivec2 startPoint, glm::ivec2 endPoint);
	void Delete() { delete[] map; };
};