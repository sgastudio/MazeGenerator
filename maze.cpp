#include "maze.h"
#include <iostream>
#include <string>
#include <ctime>
#include <windows.h>

using namespace std;

const char DEFAULT_STORAGE_WALL_CHAR = 'X';
const char DEFAULT_STORAGE_PATH_CHAR = 'o';
const char DEFAULT_STORAGE_START_CHAR = 'S';
const char DEFAULT_STORAGE_EXIT_CHAR = 'E';
const char DEFAULT_STORAGE_ROUTE_CHAR = ' ';

const char DEFAULT_DISPLAY_WALL_CHAR = 'X';
const char DEFAULT_DISPLAY_PATH_CHAR = 'o';
const char DEFAULT_DISPLAY_START_CHAR = 'S';
const char DEFAULT_DISPLAY_EXIT_CHAR = 'E';
const char DEFAULT_DISPLAY_ROUTE_CHAR = ' ';

Maze::Maze()
{
	m_size = Vector2();
	m_data = NULL;
	m_start = Vector2();
	m_exit = NULL;
	m_exitCount = 0;
	m_requireExitCount = 0;
}

Maze::Maze(Vector2 sizeLimit)
{
	Init(sizeLimit);
	m_start = Vector2();
	m_exit = NULL;
	m_exitCount = 0;
}

Maze::Maze(Vector2 sizeLimit, int exitCount)
{
	Init(sizeLimit, exitCount);
	m_start = Vector2();
	m_exit = NULL;
	m_exitCount = 0;
}

Maze::~Maze()
{
	if (m_data != NULL)
	{
		delete[] m_data;
	}
	if (m_exit != NULL)
	{
		delete[] m_exit;
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
	Init(m_sizeLimit, 2);
}

void Maze::Init(Vector2 m_sizeLimit, int exitCount)
{
	m_size = m_sizeLimit;
	m_requireExitCount = exitCount;
	Init();
}

Vector2 Maze::GetPosByIndex(int index)
{
	return Vector2(index % (m_size.x), index / (m_size.y));
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
	if (pos.x<0 || pos.x>m_size.x - 1 || pos.y<0 || pos.y>m_size.y - 1)
		return;
	m_data[pos.x + pos.y * m_size.x] = inputData;
}

void Maze::SetData(int index, short inputData)
{
	m_data[index] = inputData;
}

void Maze::SetDataRectangle(Vector2 center, Vector2 quarterSize, short inputData)
{
	for (int i = center.x - quarterSize.x; i <= center.x + quarterSize.x; i++)
	{
		for (int j = center.y - quarterSize.y; j <= center.y + quarterSize.y; j++)
		{
			m_data[i + j * m_size.x] = inputData;
		}
	}
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
			if (testString[j] == DEFAULT_DISPLAY_START_CHAR)
			{
				this->m_start = Vector2(i, j);
			}
			if (testString[j] == DEFAULT_DISPLAY_EXIT_CHAR)
			{
				//this->m_exit = Vector2(i, j);
				//Vector2List.Push();
				//m_exit->Push(Vector2(i, j));
				//for(int count=0;count<m_exitCount;count++)
				m_exit[m_exitCount] = i + j * m_size.x;
				m_exitCount++;
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
			//outputFile << (char)m_data[i * m_size.x + j];
			switch (m_data[i * m_size.x + j])
			{
			case DEFAULT_STORAGE_WALL_CHAR:
				outputFile << DEFAULT_DISPLAY_WALL_CHAR;
				break;
			case DEFAULT_STORAGE_START_CHAR:
				outputFile << DEFAULT_DISPLAY_START_CHAR;
				break;
			case DEFAULT_STORAGE_EXIT_CHAR:
				outputFile << DEFAULT_DISPLAY_EXIT_CHAR;
				break;
				//hide the path to exit
			case DEFAULT_STORAGE_PATH_CHAR:
				outputFile << DEFAULT_DISPLAY_ROUTE_CHAR;
				break;
			case DEFAULT_STORAGE_ROUTE_CHAR:
				outputFile << DEFAULT_DISPLAY_ROUTE_CHAR;
				break;
			default:
				cout << '?';
				break;
			}
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

void Maze::GenerateDeepFisrt()
{
	SetDataAll(DEFAULT_STORAGE_WALL_CHAR);
	//SetDataSurrounding(' ');
	//SetData(this->m_start, ' ');
	this->_DeepFirstGenerateRecursion(this->m_start);
	this->SetDataRectangle(this->m_start, Vector2(1, 1), DEFAULT_STORAGE_ROUTE_CHAR);
	this->_InsertStartPoint();
	this->_InsertExitPoints();
	return;
}



void Maze::_DeepFirstGenerateRecursion(Vector2 pos)
{
	if (CheckOnEdge(pos))
		return;

	//Set Current block to space
	this->SetData(pos, DEFAULT_STORAGE_ROUTE_CHAR);

	//Randomrize direction index
	Vector2 direction[4] = { {0,1},{0,-1},{1,0},{-1,0} };
	for (int i = 0; i < 4; i++)
	{
		int randDirection = rand() % 4;
		Vector2 tempVec = direction[0];
		direction[0] = direction[randDirection];
		direction[randDirection] = tempVec;
	}

	//progress with direction index
	for (int i = 0; i < 4; i++)
	{

		Vector2 nextPos = pos;
		int digRange = 1 + rand() % 4;
		while (digRange > 0)
		{
			nextPos = nextPos + direction[i];
			if (GetData(nextPos) == DEFAULT_STORAGE_ROUTE_CHAR)
				break;
			if (CheckOnEdge(nextPos))
				break;
			if (GetDataCrossCount(nextPos, DEFAULT_STORAGE_ROUTE_CHAR) > 1)
				break;
			digRange--;
			SetData(nextPos, DEFAULT_STORAGE_ROUTE_CHAR);
		}
		if (digRange <= 0)
			this->_DeepFirstGenerateRecursion(nextPos);

		/*
		Vector2 nextPos = pos + direction[i];
		if (GetDataCrossCount(nextPos, ' ') < 2)
		{
			this->_DeepFirstRecursion(nextPos);
		}*/
	}
}

bool Maze::_DeepFirstFindRecursion(Vector2 pos)
{
	if (CheckOnEdge(pos))
		return false;
	if (GetData(pos) != DEFAULT_STORAGE_ROUTE_CHAR)
		return false;
	//Randomrize direction index
	Vector2 direction[4] = { {0,1},{0,-1},{1,0},{-1,0} };
	for (int i = 0; i < 4; i++)
	{
		int randDirection = rand() % 4;
		Vector2 tempVec = direction[0];
		direction[0] = direction[randDirection];
		direction[randDirection] = tempVec;
	}

	//Set Current block to path
	this->SetData(pos, '?');

	//progress with direction index
	for (int i = 0; i < 4; i++)
	{
		Vector2 nextPos = pos + direction[i];
		short nextPosData = GetData(nextPos);
		
		if (GetDataCrossCount(nextPos, DEFAULT_STORAGE_ROUTE_CHAR) >= 1)
		{
			/*if (this->_DeepFirstFindRecursion(nextPos) == false)
			{
				break;
			}*/
			_DeepFirstFindRecursion(nextPos);
		}
		else
		{
			break;
		}
	}

	Sleep(50);
	this->Print(true);
}

void Maze::_InsertStartPoint()
{
	this->SetData(m_start, DEFAULT_STORAGE_START_CHAR);
}

void Maze::_InsertExitPoints()
{
	m_exit = new int[(m_size.x + m_size.y) * 2];
	m_exitCount = 0;
	for (int i = 1; i < (m_size.x - 1); i++)
	{
		if (GetData(Vector2(i, 1)) == DEFAULT_STORAGE_ROUTE_CHAR)
		{
			m_exit[m_exitCount] = i;
			m_exitCount++;
		}
		if (GetData(Vector2(i, m_size.y - 2)) == DEFAULT_STORAGE_ROUTE_CHAR)
		{
			m_exit[m_exitCount] = i + (m_size.y - 1) * m_size.x;
			m_exitCount++;
		}
	}
	for (int i = 1; i < (m_size.y - 1); i++)
	{
		if (GetData(Vector2(1, i)) == DEFAULT_STORAGE_ROUTE_CHAR)
		{
			m_exit[m_exitCount] = i * m_size.x;
			m_exitCount++;
		}
		if (GetData(Vector2(m_size.x - 2, i)) == DEFAULT_STORAGE_ROUTE_CHAR)
		{
			m_exit[m_exitCount] = (i + 1) * m_size.x - 1;
			m_exitCount++;
		}
	}

	for (int i = 0; i < m_exitCount; i++)
	{
		int randIndex = rand() % m_exitCount;
		int tempInt = m_exit[i];
		m_exit[i] = m_exit[randIndex];
		m_exit[randIndex] = tempInt;
	}
	m_exitCount = m_requireExitCount;
	for (int i = 0; i < m_requireExitCount; i++)
	{
		SetData(m_exit[i], DEFAULT_STORAGE_EXIT_CHAR);
	}
}

void Maze::FindPath()
{
	this->FindPathDeepFirst();
}

void Maze::FindPathDeepFirst()
{
	for (int i = 0; i < m_exitCount; i++)
	{
		Vector2 exitPos = GetPosByIndex(m_exit[i]);
		if (exitPos.x == 0)
			_DeepFirstFindRecursion(Vector2(1, exitPos.y));
		if (exitPos.x == m_size.x - 1)
			_DeepFirstFindRecursion(Vector2(m_size.x - 2, exitPos.y));
		if (exitPos.y == 0)
			_DeepFirstFindRecursion(Vector2(exitPos.x, 1));
		if (exitPos.y == m_size.y - 1)
			_DeepFirstFindRecursion(Vector2(exitPos.x, m_size.y - 2));
	}
}

void Maze::Print(bool showPaths/* =FALSE */)
{
	for (int i = 0; i < m_size.y; i++)
	{
		for (int j = 0; j < m_size.x; j++)
		{
			//cout << (char)m_data[j + i * m_size.x];// == 0 ? (<< "X") : (cout << " ");
			switch (m_data[j + i * m_size.x])
			{
			case DEFAULT_STORAGE_WALL_CHAR:
				cout << DEFAULT_DISPLAY_WALL_CHAR;
				break;
			case DEFAULT_STORAGE_START_CHAR:
				cout << "\033[33m" << DEFAULT_DISPLAY_START_CHAR << "\033[0m";
				break;
			case DEFAULT_STORAGE_EXIT_CHAR:
				cout << "\033[32m" << DEFAULT_DISPLAY_EXIT_CHAR << "\033[0m";
				break;
			case DEFAULT_STORAGE_PATH_CHAR:
				if (showPaths)
					cout << "\033[32m" << DEFAULT_DISPLAY_PATH_CHAR << "\033[0m";
				else
					cout << DEFAULT_DISPLAY_ROUTE_CHAR;
				break;
			case DEFAULT_STORAGE_ROUTE_CHAR:
				cout << DEFAULT_DISPLAY_ROUTE_CHAR;
				break;
			default:
				cout << "\033[31m" << '?' << "\033[0m";
				break;
			}
		}
		cout << endl;
	}
}
