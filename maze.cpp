#include "maze.h"
#include <iostream>
#include <string>

using namespace std;

Maze::Maze()
{
	_size = Vector2();
	_data = NULL;
	_start = Vector2();
	_exit = Vector2();
}

Maze::Maze(Vector2 _sizeLimit)
{
	Init(_sizeLimit);
	_data = NULL;
}

Maze::~Maze()
{
	delete[] _data;
}

void Maze::Init()
{
	if (_data != NULL)
	{
		delete[] _data;
	}

	_data = new short[_size.x * _size.y];
}
void Maze::Init(Vector2 _sizeLimit)
{
	 _size = _sizeLimit;
	 Init();
}

bool Maze::LoadFromFile(string fileName)
{
	ifstream inputFile;
	inputFile.open(fileName, ios::in);
	
	if (!inputFile.is_open())
	{
		return false;
	}
	
	string testString;
	
	getline(inputFile, testString);
	_size.x = testString.length();
	int index = 0;
	//inputFile.seekg(1, ios::cur);
	inputFile.seekg(0, ios::end);
	_size.y = (int)((int)inputFile.tellg() / (_size.x + 1)+0.5);
	//inputFile.unget();
	inputFile.seekg(0, ios::beg);

	this->Init();
	
	for (int i = 0; i < _size.y; i++)
	{
		getline(inputFile, testString);
		for (int j = 0; j < _size.x; j++)
		{
			_data[i * _size.x + j] = testString[j];
		}
	}
	inputFile.close();
	return true;
}

bool Maze::SaveToFile(string fileName)
{
	ofstream outputFile;
	outputFile.open(fileName, ios::out | ios::trunc);

	if (!outputFile.is_open())
	{
		return false;
	}

	for (int i = 0; i < _size.y; i++)
	{
		for (int j = 0; j < _size.x; j++)
		{
			outputFile << (char)_data[i * _size.x + j];
		}
		outputFile << endl;
	}

	outputFile.close();

	return true;
}

void Maze::Generate()
{

}

void Maze::GenerateRandomPrime()
{
}

void Maze::GenerateDeepFisrt()
{
}

void Maze::FindPath()
{
}

void Maze::FindPathDeepFirst()
{
}

void Maze::FindPathAStar()
{
}

void Maze::Print()
{
	for (int i = 0; i < _size.y; i++)
	{
		for (int j = 0; j < _size.x; j++)
		{
			cout << (char)_data[j + i * _size.x];// == 0 ? (<< "X") : (cout << " ");
		}
		cout << endl;
	}
}
