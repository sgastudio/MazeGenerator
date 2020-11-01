#include "maze.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

Maze::Maze()
{
	m_size = Vector2();
	m_data = NULL;
	m_start = Vector2();
	m_exit = Vector2();
}

Maze::Maze(Vector2 sizeLimit)
{
	Init(sizeLimit);
	m_start = Vector2();
	m_exit = Vector2();
}

Maze::~Maze()
{
	if (m_data != NULL)
	{
		delete[] m_data;
	}
}

void Maze::Init()
{
	if (m_data != NULL)
	{
		delete[] m_data;
	}

	m_data = new short[(m_size.x) * (m_size.y)];
}
void Maze::Init(Vector2 m_sizeLimit)
{
	m_size = m_sizeLimit;
	Init();
}

short Maze::GetData(Vector2 pos)
{
	return m_data[pos.x + pos.y * m_size.x];
}

int Maze::GetDataCrossCount(Vector2 pos, short data)
{
	int count = 0;

	if (GetData(pos + Vector2(1, 0)) == data)
		count++;
	if (GetData(pos + Vector2(-1, 0)) == data)
		count++;
	if (GetData(pos + Vector2(0, 1)) == data)
		count++;
	if (GetData(pos + Vector2(0, -1)) == data)
		count++;

	if (pos.x<0 || pos.x>m_size.x)
		count++;
	if (pos.y<0 || pos.y>m_size.y)
		count++;
	/*
	for (int j = pos.x - 1; j < pos.x + 2; j++)
	{
		for (int k = pos.y - 1; k < pos.y + 2; k++)
		{
			if (this->GetData(Vector2(j, k)) == 'X' && abs(j - pos.x) + abs(k - pos.y) == 1)
			{
				count++;
			}
		}
	}*/
	return count;
}

short* Maze::GetDataPointer(Vector2 pos)
{
	return &m_data[pos.x + pos.y * m_size.x];
}

bool Maze::CheckOnEdge(Vector2 pos)
{
	return pos.x == 0 || pos.x == m_size.x - 1 || pos.y == 0 || pos.y == m_size.y - 1;
}

void Maze::SetData(Vector2 pos, short inputData)
{
	m_data[pos.x + pos.y * m_size.x] = inputData;
}

void Maze::SetDataAll(short inputData)
{
	for (int i = 0; i < m_size.x * m_size.y; i++)
	{
		m_data[i] = inputData;
	}
}

void Maze::SetDataSurrounding(short wallData)
{
	//set the sround wall to opened prevent drill through the maze
	//horziontal
	for (int i = 0; i < m_size.x; i++)
	{
		m_data[i] = wallData;
		m_data[i + (m_size.y - 1) * m_size.x] = wallData;
	}
	//vertical
	for (int i = 0; i < m_size.y; i++)
	{
		m_data[i * m_size.x] = wallData;
		m_data[i * m_size.x + m_size.x - 1] = wallData;
	}
}

bool Maze::LoadFromFile(string fileName)
{
	//openfile
	ifstream inputFile;
	inputFile.open(fileName, ios::in);

	//check avaliable
	if (!inputFile.is_open())
	{
		return false;
	}

	//calc maze size
	string testString;
	getline(inputFile, testString);
	m_size.x = testString.length();
	int index = 0;
	inputFile.seekg(0, ios::end);
	m_size.y = (int)((int)inputFile.tellg() / (m_size.x + 1) + 0.5);
	inputFile.seekg(0, ios::beg);

	//allocate data sapce
	this->Init();

	//load data
	for (int i = 0; i < m_size.y; i++)
	{
		getline(inputFile, testString);
		for (int j = 0; j < m_size.x; j++)
		{
			if (testString[j] == 'S')
			{
				this->m_start = Vector2(i, j);
			}
			if (testString[j] == 'E')
			{
				this->m_exit = Vector2(i, j);
			}
			m_data[i * m_size.x + j] = testString[j];
		}
	}

	//close file
	inputFile.close();
	return true;
}

bool Maze::SaveToFile(string fileName)
{
	//open file
	ofstream outputFile;
	outputFile.open(fileName, ios::out | ios::trunc);

	//check avaliable
	if (!outputFile.is_open())
	{
		return false;
	}

	//save data to text
	for (int i = 0; i < m_size.y; i++)
	{
		for (int j = 0; j < m_size.x; j++)
		{
			outputFile << (char)m_data[i * m_size.x + j];
		}
		outputFile << endl;
	}

	//close file
	outputFile.close();

	return true;
}

void Maze::Generate()
{
	//set start point
	m_start = (m_size - 1) / 2;
	srand((unsigned)time(NULL));
	this->GenerateDeepFisrt();
}

void Maze::GenerateRandomPrime()
{

}

void Maze::GenerateDeepFisrt()
{
	SetDataAll('X');
	//SetDataSurrounding(' ');
	//SetData(this->m_start, ' ');
	this->_DeepFirstRecursion(this->m_start);
	this->_InsertStartPoint();
	//this->_PinExitPoints();

	return;
}



void Maze::_DeepFirstRecursion(Vector2 pos)
{
	if (CheckOnEdge(pos))
		return;

	//Set Current block to space
	this->SetData(pos, ' ');

	//Randomrize direction index
	Vector2 direction[4] = { {0,1},{0,-1},{1,0},{-1,0} };
	for (int i = 0; i < 4; i++)
	{
		int randDirection = rand() % 4;
		Vector2 tempVec = direction[0];
		direction[0] = direction[randDirection];
		direction[randDirection] = tempVec;
	}

	//process with direction index
	for (int i = 0; i < 4; i++)
	{

		Vector2 nextPos = pos;
		int digRange = 1 + rand() % 1;
		while (digRange > 0)
		{
			nextPos = nextPos + direction[i];
			if (GetData(nextPos) == ' ')
				break;
			if (CheckOnEdge(nextPos))
				break;
			if (GetDataCrossCount(nextPos, ' ') > 1)
				break;
			digRange--;
			SetData(nextPos, ' ');
		}
		if (digRange <= 0)
			this->_DeepFirstRecursion(nextPos);

		/*
		Vector2 nextPos = pos + direction[i];
		if (GetDataCrossCount(nextPos, ' ') < 2)
		{
			this->_DeepFirstRecursion(nextPos);
		}*/
	}
}

void Maze::_InsertStartPoint()
{
	this->SetData(m_start, 'S');
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
	for (int i = 0; i < m_size.y; i++)
	{
		for (int j = 0; j < m_size.x; j++)
		{
			cout << (char)m_data[j + i * m_size.x];// == 0 ? (<< "X") : (cout << " ");
		}
		cout << endl;
	}
}
