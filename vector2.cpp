#include "Vector2.h"

ostream& operator<<(ostream& out, Vector2 vec)
{
	out << "(" << vec.x << "," << vec.y << ")";
	return out;
}

istream& operator>>(istream& in, Vector2& vec)
{
	in >> vec.x >> vec.y;
	return in;
}

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(int valX, int valY)
{
	x = valX;
	y = valY;
}

void Vector2::Set(int valX, int valY)
{
	x = valX; y = valY;
}

Vector2 Vector2::operator+(int val)
{
	return Vector2(x + val, y + val);
}

Vector2 Vector2::operator-(int val)
{
	return Vector2(x - val, y - val);
}

Vector2 Vector2::operator+(Vector2 vec)
{
	//x += vec.x;
	//y += vec.y;
	//return Vector2(x, y);
	return Vector2(x + vec.x, y + vec.y);
}

Vector2 Vector2::operator+=(Vector2 vec)
{
	//x += vec.x;
	//y += vec.y;
	//return Vector2(x, y);
	return Vector2(x + vec.x, y + vec.y);
}

Vector2 Vector2::operator-(Vector2 vec)
{
	return Vector2(x - vec.x, y - vec.y);
}

Vector2 Vector2::operator-=(Vector2 vec)
{
	return Vector2(x - vec.x, y - vec.y);
}

Vector2 Vector2::operator*(int val)
{
	return Vector2(x * val, y * val);
}

Vector2 Vector2::operator/(int val)
{
	if (val == 0)
		return Vector2(x, y);
	return Vector2(x / val, y / val);
}

bool Vector2::operator==(Vector2 vec)
{
	return x == vec.x && y == vec.y;
}
