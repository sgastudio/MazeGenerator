#include "vector2.h"

ostream& operator<<(ostream& out, vector2 A)
{
	out << "(" << A.x << "," << A.y << ")";
	return out;
}

istream& operator>>(istream& in, vector2& A)
{
	in >> A.x >> A.y;
	return in;
}

vector2::vector2()
{
	x = 0;
	y = 0;
}

vector2::vector2(int valX, int valY)
{
	x = valX;
	y = valY;
}

int vector2::Set(int valX, int valY)
{
	x = valX; y = valY;
}

vector2 vector2::operator+(int val)
{
	return vector2(x + val, y + val);
}

vector2 vector2::operator-(int val)
{
	return vector2(x - val, y - val);
}

vector2 vector2::operator+(vector2 v2)
{
	return vector2(x + v2.x, y + v2.y);
}

vector2 vector2::operator+=(vector2 v2)
{
	return vector2(x + v2.x, y + v2.y);
}

vector2 vector2::operator-(vector2 v2)
{
	return vector2(x - v2.x, y - v2.y);
}

vector2 vector2::operator-=(vector2 v2)
{
	return vector2(x - v2.x, y - v2.y);
}

vector2 vector2::operator*(int val)
{
	return vector2(x * val, y * val);
}

bool vector2::operator==(vector2 v2)
{
	return x == v2.x && y == v2.y;
}
