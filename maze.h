#pragma once
#include "vector2.h"
#include "AStarFinder.h"
#include "player.h"

#include <vector>
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

	vector<int> m_exit;
	int m_requireExitCount;

	vector<Pos2D> m_pathPoints;

	int m_difficulty;

	//Player* m_players;
	vector<Player> m_players;
	void _DeepFirstGenerateRecursion(Vector2 pos);

	bool _DeepFirstFindRecursion(Vector2 pos);

	bool _AStarFindProcess(Vector2 pos, Vector2 end);

	bool _PlayerAStarFindProcess(Vector2 startPos, Vector2 endPos, int playerIndex);

	//insert start points after generation
	void _InsertStartPoint();
	//insert exit points after generation
	void _InsertExitPoints();
public:
	enum MAZE_SOLVABLE_E {
		Not = 0,
		Partially,
		Fully,
	};
	Maze();
	Maze(Vector2 sizeLimit, int difficulty, int exitCount = 1);
	~Maze();
	void Init();
	void Init(Vector2 sizeLimit);
	void Init(Vector2 sizeLimit, int difficulty, int exitCount = 1);

	void InitPlayers();

	short GetData(Vector2 pos);
	short GetData(int x, int y);
	int GetDataCrossCount(Vector2 pos, short data);
	short* GetDataPointer(Vector2 pos);
	Vector2 GetPosNearEdge(Vector2 pos);
	Vector2 GetSize();

	MAZE_SOLVABLE_E GetSolvable();
	int GetPlayerDataCount();
	bool GetPlayerState();
	int GetExitCount();

	int GetActivePlayerCount();

	void SetSize(Size2D size);

	bool CheckGenerated();
	bool CheckPlayerPathFinderMode();
	bool CheckOnEdge(Vector2 pos);
	bool CheckInsideCenterRectangle(Vector2 pos, Vector2 quarterSize);
	int LimitDifficult(int val);

	void SetData(Vector2 pos, short inputData);
	void SetData(int x, int y, short inputData);
	void SetData(int index, short inputData);
	void SetDataRectangle(Vector2 center, Vector2 quarterSize, short inputData);
	void SetDataAll(short inputData);
	void SetDataSurrounding(short wallData);
	bool LoadFromFile(string fileName);
	bool SaveToFile(string fileName);
	void Generate();
	//void GenerateRandomPrime();
	void GenerateDeepFisrt();
	void FindPath(int methodIndex);
	void ClearPath();
	void FindPathDeepFirst();
	void FindPlayerPathAStar();
	void FindPathAStar();

	void Print(bool showPaths = false);
	void PrintWithPlayers();
};

