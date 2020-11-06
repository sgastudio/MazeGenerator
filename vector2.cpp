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

Vector2::Vector2(int val)
{
	x = val;
	y = val;
}

Vector2::Vector2(int valX, int valY)
{
	x = valX;
	y = valY;
}

int Vector2::Distance(Vector2 end)
{
	return this->Distance(*this, end);
}

int Vector2::Distance(Vector2 start, Vector2 end)
{
	return abs((end - start).x) + abs((end - start).y);
}

int Vector2::Get1DIndex(int width)
{
	return width * y + x;
}
Vector2 Vector2::Get2DPos(int width, int index)
{
	return Vector2(index%width,index/width);
}

void Vector2::Set(int valX, int valY)
{
	x = valX; y = valY;
}

void Vector2::LimitMin(Vector2 limit)
{
	if (x < limit.x)
		x = limit.x;
	if (y < limit.y)
		y = limit.y;
}

void Vector2::LimitMax(Vector2 limit)
{
	if (x > limit.x)
		x = limit.x;
	if (y > limit.y)
		y = limit.y;
}

void Vector2::LimitInt(int* val, Vector2 limit) 
{
	if (*val < limit.x)
		*val = limit.x;
	if (*val > limit.y)
		*val = limit.y;
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

Vector2 Vector2::operator=(int val)
{
	return Vector2(val,val);
}

bool Vector2::operator==(Vector2 vec)
{
	return x == vec.x && y == vec.y;
}

bool Vector2::operator!=(Vector2 vec)
{
	return x != vec.x || y != vec.y;
}