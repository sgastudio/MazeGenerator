#pragma once
#include <iostream>

using namespace std;


typedef class Vector2 {
public:
	Vector2();
	Vector2(int valX, int valY);
	int x;
	int y;
	void Set(int valX, int valY);
	Vector2 operator+ (int val) ;
	Vector2 operator- (int val);
	Vector2 operator+ (Vector2 vec) ;
	Vector2 operator+= (Vector2 vec) ;
	Vector2 operator- (Vector2 vec) ;
	Vector2 operator-= (Vector2 vec) ;
	Vector2 operator* (int val) ;
	bool operator== (Vector2 vec);
	friend ostream& operator<<(ostream& out, Vector2 vec);
	friend istream& operator>>(istream& in, Vector2& vec);
}v2, vec2, vector2;