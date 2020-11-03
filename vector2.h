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
	bool operator== (Vector2 vec);
	bool operator!=(Vector2 vec);
	friend ostream& operator<<(ostream& out, Vector2 vec);
	friend istream& operator>>(istream& in, Vector2& vec);
}v2, vec2, vector2;

typedef class Vector2List {
public:
	Vector2 data;
	Vector2List* next;
	Vector2List();
	Vector2List(Vector2 vec);
	~Vector2List();
	void Swap(Vector2List* nodeA, Vector2List* nodeB);
	void Swap(Vector2List* nodeB);

	void Push(Vector2 vec);
	void Push(int x, int y);
	Vector2 Pop();
	void Insert(int index, Vector2 vec);
	void Remove(int index);
	void Remove();
}Vector2Node;