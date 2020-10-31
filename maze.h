#pragma once
#include "vector2.h"
#include <string>
#include <iostream>
#include <fstream>
class Maze {
protected:
	Vector2 m_size;
	short* m_data;
	Vector2 m_start;
	Vector2 m_exit;
public:
	Maze();
	Maze(Vector2 sizeLimit);
	~Maze();
	void Init();
	void Init(Vector2 sizeLimit);
	short GetData(Vector2 pos);
	int GetDataCrossCount(Vector2 pos, short data);
	short* GetDataPointer(Vector2 pos);
	bool CheckOnEdge(Vector2 pos);
	void SetData(Vector2 pos, short inputData);
	void SetDataAll(short inputData);
	void SetDataSurrounding(short wallData);
	bool LoadFromFile(string fileName);
	bool SaveToFile(string fileName);
	void Generate();
	void GenerateRandomPrime();
	void GenerateDeepFisrt();
	void _DeepFirstRecursion(Vector2 pos);
	void _InsertStartPoint();
	void _PinExitPoints();
	void FindPath();
	void FindPathDeepFirst();
	void FindPathAStar();
	void Print();
};