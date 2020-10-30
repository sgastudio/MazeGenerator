#pragma once
#include "vector2.h"
#include <string>
#include <iostream>
#include <fstream>
class Maze {
protected:
	Vector2 _size;
	short* _data;
	Vector2 _start;
	Vector2 _exit;
public:
	Maze();
	Maze(Vector2 sizeLimit);
	~Maze();
	void Init();
	void Init(Vector2 sizeLimit);
	bool LoadFromFile(string fileName);
	bool SaveToFile(string fileName);
	void Generate();
	void GenerateRandomPrime();
	void GenerateDeepFisrt();
	void FindPath();
	void FindPathDeepFirst();
	void FindPathAStar();
	void Print();
};