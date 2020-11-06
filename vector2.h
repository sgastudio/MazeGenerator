#pragma once
#include <iostream>

using namespace std;

typedef class Vector2 {
public:
	Vector2();
	Vector2(int val);
	Vector2(int valX, int valY);
	int x;
	int y;
	int Distance(Vector2 end);
	int Distance(Vector2 start, Vector2 end);
	int Get1DIndex(int width);
	static Vector2 Get2DPos(int width, int index);
	void Set(int valX, int valY);
	void LimitMin(Vector2 limit);
	void LimitMax(Vector2 limit);
	static void LimitInt(int* val, Vector2 limit);
	Vector2 operator+ (int val) ;
	Vector2 operator- (int val);
	Vector2 operator+ (Vector2 vec) ;
	Vector2 operator+= (Vector2 vec) ;
	Vector2 operator- (Vector2 vec) ;
	Vector2 operator-= (Vector2 vec) ;
	Vector2 operator* (int val) ;
	Vector2 operator/(int val);
	Vector2 operator=(int val);
	bool operator== (Vector2 vec);
	bool operator!=(Vector2 vec);
	friend ostream& operator<<(ostream& out, Vector2 vec);
	friend istream& operator>>(istream& in, Vector2& vec);
}v2, vec2, vector2, Pos2D, Size2D, Valuation;