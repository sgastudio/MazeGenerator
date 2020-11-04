#pragma once
#include "stack.h"
#include "vector2.h"

#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <windows.h>


class Maze {
protected:
	short* m_data;

	Vector2 m_size;

	Vector2 m_start;
	int* m_exit;
	int m_exitCount;
	int m_requireExitCount;

	int m_difficulty;

	void _DeepFirstGenerateRecursion(Vector2 pos);
	bool _DeepFirstFindRecursion(Vector2 pos);
	void _InsertStartPoint();
	void _InsertExitPoints();
public:
	Maze();
	Maze(Vector2 sizeLimit);
	Maze(Vector2 sizeLimit, int exitCount);
	Maze(Vector2 sizeLimit, int difficulty, int exitCount);
	~Maze();
	void Init();
	void Init(Vector2 sizeLimit);
	void Init(Vector2 m_sizeLimit, int difficulty, int exitCount = 1);

	Vector2 GetPosByIndex(int index);
	short GetData(Vector2 pos);
	short GetData(int x, int y);
	int GetDataCrossCount(Vector2 pos, short data);
	short* GetDataPointer(Vector2 pos);
	Vector2 GetSize();

	bool CheckGenerated();

	bool CheckOnEdge(Vector2 pos);
	bool CheckInsideCenterRectangle(Vector2 pos, Vector2 quarterSize);
	void SetData(Vector2 pos, short inputData);
	void SetData(int x, int y, short inputData);
	void SetData(int index, short inputData);
	void SetDataRectangle(Vector2 center, Vector2 quarterSize, short inputData);
	void SetDataAll(short inputData);
	void SetDataSurrounding(short wallData);
	bool LoadFromFile(string fileName);
	bool SaveToFile(string fileName);
	void Generate();
	void GenerateRandomPrime();
	void GenerateDeepFisrt();
	void FindPath();
	void ClearPath();
	void FindPathDeepFirst();
	void FindPathAStar();
	void Print(bool showPaths = false);
	void PrintWithClearScreen(bool showPaths = false);
};