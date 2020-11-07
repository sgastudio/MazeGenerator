#pragma once

#include "vector2.h"

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