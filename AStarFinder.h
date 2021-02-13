/*
A Start class for maze generate and search
*/
#pragma once

#include "vector2.h"
#include "ASNode.h"

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