#pragma once

#include <climits>
#include <string>
#include <fstream>
#include <iostream>

#include "List.h"

struct ivec2{
	int x;
	int y;

	ivec2() : x(0), y(0) {};
	ivec2(int x, int y) : x(x), y(y) {};
};
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
	ivec2* Solve(ivec2 startPoint, ivec2 endPoint, int *pathLen);
	void Delete() { delete[] map; };
};