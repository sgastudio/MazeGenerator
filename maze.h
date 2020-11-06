#pragma once
#include "vector2.h"
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
	int* m_exit;
	int m_exitCount;
	int m_requireExitCount;

	vector<Pos2D> m_pathPoints;

	int m_difficulty;

	void _DeepFirstGenerateRecursion(Vector2 pos);
	bool _DeepFirstFindRecursion(Vector2 pos);

	bool _AStarFindProcess(Vector2 pos, Vector2 end);

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
	//void GenerateRandomPrime();
	void GenerateDeepFisrt();
	void FindPath(int methodIndex);
	void ClearPath();
	void FindPathDeepFirst();
	void FindPathAStar();

	void Print(bool showPaths = false);
	void PrintWithClearScreen(bool showPaths = false);
};

struct AStarNode {
public:
	Pos2D parent;
	Valuation values;
	AStarNode()
	{
		values = 0;
		parent = 0;
	};
	AStarNode(Pos2D parentIndex, Vector2 inputVals)
	{
		values = inputVals;
		parent = parentIndex;
	};
	int GetValueG() {
		return values.x;
	};
	int GetValueH() {
		return values.y;
	};
	int GetValueF() {
		return values.x + values.y;
	};
};


class AStarFinder {
	Vector2 m_size;
	bool* m_open;
	bool* m_close;
	AStarNode* m_node;
	Pos2D m_current;
	Pos2D m_start;
	Pos2D m_end;
public:
	AStarFinder(Vector2 definedSize)
	{
		m_size = definedSize;
		m_open = new bool[definedSize.x * definedSize.y]();
		m_close = new bool[definedSize.x * definedSize.y]();
		m_node = new AStarNode[definedSize.x * definedSize.y];
		
	};

	~AStarFinder()
	{
		delete[] m_open;
		delete[] m_close;
		delete[] m_node;
	};

	void StartFind(Pos2D startPos, Pos2D endPos)
	{
		SetData(startPos.Get1DIndex(m_size.x), startPos, Vector2(0, startPos.Distance(endPos)));
		AddToOpenTalbe(startPos.Get1DIndex(m_size.x));
		m_start = startPos;
		m_end = endPos;
	};

	Pos2D GetMinIndex()
	{
		int minValIndex = GetOpenTableMinIndex();
		RemoveFromOpenTalbe(minValIndex);
		AddToCloseTable(minValIndex);
		m_current = Vector2::Get2DPos(m_size.x, minValIndex);
		return m_current;
	}

	void SetData(int dataIndex, Pos2D inputParent, Vector2 inputValues)
	{
		m_node[dataIndex].parent = inputParent;
		m_node[dataIndex].values = inputValues;
	};
	void SetParent(int dataIndex, Pos2D inputParent)
	{
		m_node[dataIndex].parent = inputParent;
	};
	void SetValues(int dataIndex, Vector2 inputValues)
	{
		m_node[dataIndex].values = inputValues;
	};
	void SetValueG(int dataIndex, int inputValues)
	{
		m_node[dataIndex].values.x = inputValues;
	};
	void SetValueH(int dataIndex, int inputValues)
	{
		m_node[dataIndex].values.y = inputValues;
	};
	AStarNode GetData(int dataIndex)
	{
		return m_node[dataIndex];
	}
	AStarNode GetData(Pos2D dataPos)
	{
		return m_node[dataPos.Get1DIndex(m_size.x)];
	}

	bool CheckInOpenTable(int index)
	{
		return m_open[index];
	};

	bool CheckInCloseTable(int index)
	{
		return m_close[index];
	};

	bool CheckInOpenTable(Pos2D pos)
	{
		return m_open[pos.Get1DIndex(m_size.x)];
	};

	bool CheckInCloseTable(Pos2D pos)
	{
		return m_close[pos.Get1DIndex(m_size.x)];
	};

	int GetOpenTableMinIndex()
	{
		int valuationIndex = -1;
		int valuationMin = INT_MAX;
		for (int i = 0; i < m_size.x * m_size.y; i++)
		{
			if (CheckInOpenTable(i) && GetData(i).GetValueF() < valuationMin)
			{
				valuationIndex = i;
				valuationMin = GetData(i).GetValueF();
			}
		}
		return valuationIndex;
	};

	int GetOpenTableCount()
	{
		int count = 0;
		for (int i = 0; i < m_size.x * m_size.y; i++)
		{
			if (m_open[i] == true)
			{
				count++;
			}
		}
		return count;
	};
	int GetCloseTableCount()
	{
		int count = 0;
		for (int i = 0; i < m_size.x * m_size.y; i++)
		{
			if (m_close[i] == true)
			{
				count++;
			}
		}
		return count;
	};
	bool CloseTableEmpty()
	{
		return GetCloseTableCount() == 0;
	}
	bool OpenTableEmpty()
	{
		return GetOpenTableCount() == 0;
	}

	void RemoveFromOpenTalbe(int index)
	{
		m_open[index] = false;
	};
	void RemoveFromCloseTable(int index)
	{
		m_close[index] = false;
	};
	void AddToOpenTalbe(int index)
	{
		m_open[index] = true;
	};
	void AddToCloseTable(int index)
	{
		m_close[index] = true;
	};
};