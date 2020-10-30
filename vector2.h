#pragma once
#include <iostream>

using namespace std;


typedef class vector2 {
public:
	vector2();
	vector2(int valX, int valY);
	int x;
	int y;
	int Set(int valX, int valY);
	vector2 operator+ (int val) ;
	vector2 operator- (int val);
	vector2 operator+ (vector2 v2) ;
	vector2 operator+= (vector2 v2) ;
	vector2 operator- (vector2 v2) ;
	vector2 operator-= (vector2 v2) ;
	vector2 operator* (int val) ;
	bool operator== (vector2 v2);
	friend ostream& operator<<(ostream& out, vector2 A);
	friend istream& operator>>(istream& in, vector2& A);
}v2, vec2, Vector2;