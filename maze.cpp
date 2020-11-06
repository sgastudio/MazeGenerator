#include "maze.h"

using namespace std;

const char DEFAULT_STORAGE_WALL_CHAR = 'X';
const char DEFAULT_STORAGE_PATH_CHAR = 'o';
const char DEFAULT_STORAGE_START_CHAR = 'S';
const char DEFAULT_STORAGE_EXIT_CHAR = 'E';
const char DEFAULT_STORAGE_ROUTE_CHAR = ' ';
const char DEFAULT_STORAGE_PLAYER_CHAR = 'P';

const char DEFAULT_DISPLAY_WALL_CHAR = 'X';
const char DEFAULT_DISPLAY_PATH_CHAR = 'o';
const char DEFAULT_DISPLAY_START_CHAR = 'S';
const char DEFAULT_DISPLAY_EXIT_CHAR = 'E';
const char DEFAULT_DISPLAY_ROUTE_CHAR = ' ';
const char DEFAULT_DISPLAY_PLAYER_CHAR = 'P';
const char DEFAULT_DISPLAY_FINISH_CHAR = 'F';
Maze::Maze()
{
	m_size = Vector2();
	m_data = NULL;
	m_start = Vector2();
	m_exit = NULL;
	m_exitCount = 0;
	m_requireExitCount = 0;
	m_difficulty = 0;
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
	m_difficulty = 1;
}

Maze::Maze(Vector2 sizeLimit, int difficulty ,int exitCount)
{
	Init(sizeLimit, exitCount);
	m_start = Vector2();
	m_exit = NULL;
	m_exitCount = 0;
	m_difficulty = difficulty;
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
	m_pathPoints.clear();
}
void Maze::Init(Vector2 m_sizeLimit)
{
	Init(m_sizeLimit, 2);
}

void Maze::Init(Vector2 m_sizeLimit, int difficulty, int exitCount)
{
	m_size = m_sizeLimit;
	m_requireExitCount = exitCount;
	m_difficulty = difficulty;
	Init();
}

short Maze::GetData(Vector2 pos)
{
	return m_data[pos.x + pos.y * m_size.x];
}

short Maze::GetData(int x, int y)
{
	return GetData(Vector2(x,y));
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

	/*if (pos.x<0 || pos.x>m_size.x)
		count++;
	if (pos.y<0 || pos.y>m_size.y)
		count++;*/

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

Vector2 Maze::GetSize()
{
	return m_size;
}

bool Maze::CheckGenerated()
{
	if (m_size.x != 0 && m_size.y != 0)
		return true;
	else
		return false;
}

bool Maze::CheckOnEdge(Vector2 pos)
{
	return pos.x == 0 || pos.x == m_size.x - 1 || pos.y == 0 || pos.y == m_size.y - 1;
}

bool Maze::CheckInsideCenterRectangle(Vector2 pos, Vector2 quarterSize)
{
	bool verticalCheck = pos.x>=m_size.x/2 - quarterSize.x && pos.x <= m_size.x / 2 + quarterSize.x;
	bool horizontalCheck = pos.y >= m_size.y / 2 - quarterSize.y && pos.y <= m_size.y / 2 + quarterSize.y;
	return verticalCheck && horizontalCheck;
}

void Maze::SetData(Vector2 pos, short inputData)
{
	if (pos.x<0 || pos.x>m_size.x - 1 || pos.y<0 || pos.y>m_size.y - 1)
		return;
	m_data[pos.x + pos.y * m_size.x] = inputData;
}

void Maze::SetData(int x, int y, short inputData)
{
	SetData(Vector2(x,y),inputData);
}

/// <summary>
/// set value of specific pos by index
/// </summary>
/// <param name="index">data index in m_data</param>
/// <param name="inputData">value</param>
void Maze::SetData(int index, short inputData)
{
	m_data[index] = inputData;
}

void Maze::SetDataRectangle(Vector2 center, Vector2 quarterSize, short inputData)
{
	quarterSize = quarterSize / 2;
	for (int i = center.x - quarterSize.x; i <= center.x + quarterSize.x; i++)
	{
		for (int j = center.y - quarterSize.y; j <= center.y + quarterSize.y; j++)
		{
			m_data[i + j * m_size.x] = inputData;
		}
	}
}

/// <summary>
/// set every element of m_data to inputData
/// </summary>
/// <param name="inputData">value to be set</param>
void Maze::SetDataAll(short inputData)
{
	for (int i = 0; i < m_size.x * m_size.y; i++)
	{
		m_data[i] = inputData;
	}
}

void Maze::SetDataSurrounding(short blockData)
{
	//set the sround wall to opened prevent drill through the maze
	//horziontal
	for (int i = 0; i < m_size.x; i++)
	{
		m_data[i] = blockData;
		m_data[i + (m_size.y - 1) * m_size.x] = blockData;
	}
	//vertical
	for (int i = 0; i < m_size.y; i++)
	{
		m_data[i * m_size.x] = blockData;
		m_data[i * m_size.x + m_size.x - 1] = blockData;
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
	int widthCheck = -1;
	int i=0;
	while (!inputFile.eof())
	{
		getline(inputFile, testString);
		if (widthCheck != testString.length() && !testString.empty())
		{
			if (widthCheck > 0)
				return false;
			else
				widthCheck = testString.length();
		}
	}

	inputFile.clear();
	inputFile.seekg(0, ios::beg);
	getline(inputFile, testString);
	m_size.x = testString.length();
	int index = 0;
	inputFile.seekg(0, ios::end);
	m_size.y = (int)((int)inputFile.tellg() / (m_size.x + 1) + 0.5);
	inputFile.seekg(0, ios::beg);

	//allocate data space
	this->Init();
	m_exit = new int[(m_size.x + m_size.y) * 2];

	//load data
	for (int i = 0; i < m_size.y; i++)
	{
		getline(inputFile, testString);
		for (int j = 0; j < m_size.x; j++)
		{
			if (testString[j] == DEFAULT_DISPLAY_START_CHAR)
			{
				this->m_start = Vector2(j, i);
			}
			if (testString[j] == DEFAULT_DISPLAY_EXIT_CHAR)
			{
				m_exit[m_exitCount] = i * m_size.x + j ;
				m_exitCount++;
			}
			m_data[i * m_size.x + j] = testString[j];
		}
	}

	//close file
	inputFile.close();

	//this->FindPath();

	return true;
}

//save maze data to file
bool Maze::SaveToFile(string fileName)
{
	//open file
	ofstream outputFile;
	outputFile.open(fileName, ios::out | ios::trunc);

	//check file avaliable
	if (!outputFile.is_open())
		return false;

	//check if the maze had been generated
	if (!this->CheckGenerated())
		return false;

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
				outputFile << '?';
				break;
			}
		}
		outputFile << endl;
	}

	//close file
	outputFile.close();

	return true;
}

/// <summary>
/// generating algorithm selector
/// </summary>
void Maze::Generate()
{
	//set start point
	m_start = (m_size - 1) / 2;
	srand((unsigned)time(NULL));
	this->GenerateDeepFisrt();
	//this->FindPath();
}

/// <summary>
/// DFS algorithm for maze generation
/// </summary>
void Maze::GenerateDeepFisrt()
{
	SetDataAll(DEFAULT_STORAGE_WALL_CHAR);
	//SetDataSurrounding(' ');
	//SetData(this->m_start, ' ');
	this->_DeepFirstGenerateRecursion(this->m_start);
	Vector2 centerAreaSize = Vector2(m_size.x / 10, m_size.y / 10);
	centerAreaSize.LimitMin(Vector2(3,3));
	this->SetDataRectangle(this->m_start, centerAreaSize, DEFAULT_STORAGE_ROUTE_CHAR);
	this->_InsertStartPoint();
	this->_InsertExitPoints();
	return;
}

/// <summary>
/// Recursion style for DFS algorithm maze generation
/// </summary>
/// <param name="pos">position for check</param>
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
		int digRange = 1 + rand() % m_difficulty;
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

	}
}


bool Maze::_DeepFirstFindRecursion(Vector2 pos)
{
	if (GetData(pos) == DEFAULT_STORAGE_START_CHAR)
		return true;
	if (GetDataCrossCount(pos, DEFAULT_STORAGE_START_CHAR) >= 1)
	{
		SetData(pos, DEFAULT_STORAGE_PATH_CHAR);
		return true;
	}
	if (GetDataCrossCount(pos, DEFAULT_STORAGE_PATH_CHAR) >= 1)
	{
		SetData(pos, DEFAULT_STORAGE_PATH_CHAR);
		return true;
	}
	/*
	if (CheckInsideCenterRectangle(pos, m_size/20))
	{
		SetData(pos, DEFAULT_STORAGE_PATH_CHAR);
		return true;
	}
	*/
	if (GetData(pos) == DEFAULT_STORAGE_PATH_CHAR)
		return true;
	if (GetDataCrossCount(pos, ' ') < 1)
		return false;

	//Randomrize direction index
	//up down left right
	Vector2 distance = m_start - pos;
	Vector2 direction[5] = { {0,-1},{0,1},{-1,0},{1,0}, {0, 0}};
	if (max(abs(distance.x) , abs(distance.y))<=min(m_size.x/5,m_size.y/5))
	{
		if (abs(distance.x) > abs(distance.y))
			direction[4] = { distance.x / abs(distance.x),0 };
		else
			direction[4] = { 0, distance.y / abs(distance.y) };
	}
	

	for (int i = 0; i < 4; i++)
	{
		int randDirection = rand() % 4;
		Vector2 tempVec = direction[0];
		direction[0] = direction[randDirection];
		direction[randDirection] = tempVec;
	}

	//Set Current block to path
	this->SetData(pos, '-');

	Vector2 nextPos;
	//progress with direction index
	for (int i = 4; i >= 0; i--)
	{
		nextPos = pos + direction[i];
		short nextPosData = GetData(nextPos);

		if (nextPosData==DEFAULT_STORAGE_ROUTE_CHAR && direction[i] != 0)
		{
			if (_DeepFirstFindRecursion(nextPos)==true)
			{
				this->SetData(pos, DEFAULT_STORAGE_ROUTE_CHAR);
				this->SetData(pos, DEFAULT_STORAGE_PATH_CHAR);
				return true;
			}
		}
	}

	//path not found, set back to ' ' char
	this->SetData(pos, DEFAULT_STORAGE_ROUTE_CHAR);
	return false;
}

/// <summary>
/// set the start point for inner data structure
/// </summary>
void Maze::_InsertStartPoint()
{
	this->SetData(m_start, DEFAULT_STORAGE_START_CHAR);
}

/// <summary>
/// check and set blocks on edges to exits as much as m_requireExitCount
/// </summary>
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
	if(m_requireExitCount < m_exitCount)
		m_exitCount = m_requireExitCount;
	for (int i = 0; i < m_exitCount; i++)
	{
		SetData(m_exit[i], DEFAULT_STORAGE_EXIT_CHAR);
	}
}

/// <summary>
/// Clear pathfiner solution of the maze for inner data storage
/// </summary>
void Maze::ClearPath()
{
	m_pathPoints.clear();
	for (int i = 0; i < m_size.x * m_size.y; i++)
	{
		if (m_data[i] == DEFAULT_STORAGE_PATH_CHAR)
			m_data[i] = DEFAULT_STORAGE_ROUTE_CHAR;
	}
}

/// <summary>
/// path finding algorithm selector
/// </summary>
/// <param name="methodIndex">algorithm selector</param>
void Maze::FindPath(int algorithmIndex)
{
	ClearPath();
	switch (algorithmIndex)
	{
	case 1:
		this->FindPathDeepFirst();
		break;
	case 0:
	default:
		this->FindPathAStar();
	break;
	}

	//Display found path with char 'o'
	while (m_pathPoints.size() > 0)
	{
		m_data[m_pathPoints.back().Get1DIndex(m_size.x)] = 'o';
		m_pathPoints.pop_back();
	}
}

/// <summary>
/// run DFS pathfinding for each exit
/// </summary>
void Maze::FindPathDeepFirst()
{
	for (int i = 0; i < m_exitCount; i++)
	{
		Vector2 exitPos = Vector2::Get2DPos(m_size.x, m_exit[i]);
		if (exitPos.x == 0)
			_DeepFirstFindRecursion(Vector2(1, exitPos.y));
		if (exitPos.x == m_size.x - 1)
			_DeepFirstFindRecursion(Vector2(m_size.x - 2, exitPos.y));
		if (exitPos.y == 0)
			_DeepFirstFindRecursion(Vector2(exitPos.x, 1));
		if (exitPos.y == m_size.y - 1)
			_DeepFirstFindRecursion(Vector2(exitPos.x, m_size.y - 2));
	}

	for (int i = 0; i < m_size.x * m_size.y; i++)
	{
		if (m_data[i] == 'o')
		{
			m_pathPoints.push_back(Vector2::Get2DPos(m_size.x, i));
			m_data[i] = ' ';
		}
	}
}

/// <summary>
/// run A* pathfinding for each exit
/// </summary>
void Maze::FindPathAStar()
{
	for (int i = 0; i < m_exitCount; i++)
	{
		Vector2 exitPos = Vector2::Get2DPos(m_size.x,m_exit[i]);
		if (exitPos.x == 0)
			_AStarFindProcess(Vector2(1, exitPos.y),m_start);
		if (exitPos.x == m_size.x - 1)
			_AStarFindProcess(Vector2(m_size.x - 2, exitPos.y), m_start);
		if (exitPos.y == 0)
			_AStarFindProcess(Vector2(exitPos.x, 1), m_start);
		if (exitPos.y == m_size.y - 1)
			_AStarFindProcess(Vector2(exitPos.x, m_size.y - 2), m_start);
	}
}


bool Maze::_AStarFindProcess(Vector2 startPos, Vector2 endPos)
{
	//allocate workspace
	AStarFinder AFinder(m_size);
	Pos2D currentPos;
	Pos2D nextPos;
	Vector2 direction[4] = { {0,-1},{0,1},{-1,0},{1,0} };

	//Start Finder
	AFinder.StartFind(startPos, endPos);
	
	while (!AFinder.OpenTableEmpty())
	{
		currentPos = AFinder.GetMinIndex();

		//Check if CurrentPos is the point we need
		if (currentPos == endPos)
		{
			AFinder.AddToCloseTable(currentPos.Get1DIndex(m_size.x));
			break;
		}

		for (int i = 0; i < 4; i++)
		{
			nextPos = currentPos + direction[i];
			short nextPosData = m_data[nextPos.Get1DIndex(m_size.x)];

			//block reachable and not in closeTable
			if ((nextPosData == ' ' || nextPosData == 'S') && !AFinder.CheckInCloseTable(nextPos))
			{

				//block not in openTable
				if (!AFinder.CheckInOpenTable(nextPos))
				{
					AFinder.AddToOpenTalbe(nextPos.Get1DIndex(m_size.x));
					AFinder.SetParent(nextPos.Get1DIndex(m_size.x),currentPos);
					AFinder.SetValueG(nextPos.Get1DIndex(m_size.x),AFinder.GetData(currentPos).GetValueG() + 1);
					AFinder.SetValueH(nextPos.Get1DIndex(m_size.x),nextPos.Distance(endPos));
				}
				//block alreay in openTable if the G Value will be better, then change Parent;
				else if(AFinder.CheckInOpenTable(nextPos) && AFinder.GetData(nextPos).GetValueG() > AFinder.GetData(currentPos).GetValueG() + 1)
				{
					AFinder.SetParent(nextPos.Get1DIndex(m_size.x), currentPos);
					AFinder.SetValueG(nextPos.Get1DIndex(m_size.x), AFinder.GetData(currentPos).GetValueG() + 1);
				}
			}
		}
	}
	
	//returen false if there is no path
	if (AFinder.OpenTableEmpty())
		return false;

	//store path to m_pathPoints for display
	do {
		currentPos = AFinder.GetData(currentPos).parent;
		m_data[currentPos.Get1DIndex(m_size.x)] = 'Z';
		m_pathPoints.push_back(currentPos);
	} while (AFinder.GetData(currentPos).parent != currentPos);
	return true;
}

/// <summary>
///	Print the Maze char by char with std::cout
/// </summary>
/// <param name="showPaths">does the solution shows up</param>
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
				cout << "\033[93m" << DEFAULT_DISPLAY_START_CHAR << "\033[0m";
				break;
			case DEFAULT_STORAGE_EXIT_CHAR:
				cout << "\033[32m" << DEFAULT_DISPLAY_EXIT_CHAR << "\033[0m";
				break;
			case DEFAULT_STORAGE_PATH_CHAR:
				if (showPaths==true)
					cout << "\033[92m" << DEFAULT_DISPLAY_PATH_CHAR << "\033[0m";
				else
					cout << DEFAULT_DISPLAY_ROUTE_CHAR;
				break;
			case DEFAULT_STORAGE_ROUTE_CHAR:
				cout << DEFAULT_DISPLAY_ROUTE_CHAR;
				break;
			case DEFAULT_STORAGE_PLAYER_CHAR:
				cout << "\033[36m" << DEFAULT_DISPLAY_PLAYER_CHAR << "\033[0m";
				break;
			default:
				cout << "\033[31m" << 'U' << "\033[0m";
				break;
			}
		}
		cout << endl;
	}
}

/// <summary>
///	Print the Maze char by char with std::cout and clear the screen first
/// </summary>
/// <param name="showPaths">does the solution shows up</param>
void Maze::PrintWithClearScreen(bool showPaths/* =FALSE */)
{
	system("cls");
	this->Print(showPaths);
}
