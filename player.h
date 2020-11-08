#pragma once

#include "vector2.h"
#include <vector>

class Player {
	bool m_locked;
	bool m_finished;
	Pos2D m_start;
	int m_currentIndex;
public:
	vector<Pos2D> path;
	Player()
	{
		m_locked = false;
		m_finished = false;
		m_currentIndex = 0;
		m_start = 0;
	};
	~Player()
	{
		path.clear();
	};
	void ResetIndex()
	{
		m_currentIndex = path.size() - 1;
		m_finished = false;
	};
	void Progress()
	{
		if (m_currentIndex > 0)
			m_currentIndex--;
	};
	void SetFinished()
	{
		m_finished = true;
	}
	bool GetFinished()
	{
		return m_finished;
	}
	Vector2 GetCurrentMazePos()
	{
		return path[m_currentIndex];
	}
	Vector2 GetNextMazePos()
	{
		if(m_currentIndex>0)
			return path[m_currentIndex-1];
		else
			return path[0];
	}
	int GetCurrentStepIndex()
	{
		return m_currentIndex;
	}
	void SetStartPos(Pos2D pos)
	{
		m_start = pos;
	}

	void SetLocked()
	{
		m_locked = true;
	}
	void RemoveLocked()
	{
		m_locked = false;
	}
	bool GetLockState()
	{
		return m_locked;
	}
};