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
	Init();
}

/*
Maze::Maze(Vector2 sizeLimit, int exitCount)
{
	Init(sizeLimit, exitCount);
	m_start = Vector2();
	m_exit = NULL;
	m_exitCount = 0;
	m_difficulty = 1;
}*/

Maze::Maze(Vector2 sizeLimit, int difficulty, int exitCount)
{
	Init(sizeLimit, difficulty, exitCount);
}

Maze::~Maze()
{
	if (m_data != NULL)
		delete[] m_data;
	//if (m_exit != NULL)
	//	delete[] m_exit;
	if (m_players != NULL)
		delete[] m_players;
	if (m_pathPoints.size() > 0)
		m_pathPoints.clear();
}

void Maze::Init()
{

	if (m_data != NULL)
		delete[] m_data;
	m_data = NULL;
	//if (m_exit != NULL)
	//	delete[] m_exit;
	//m_exit = NULL;
	m_exit.clear();
	if (m_players != NULL)
		delete[] m_players;
	m_players = NULL;
	m_size = Vector2();
	m_start = Vector2();
	//m_exitCount = 0;
	m_requireExitCount = 0;
	m_difficulty = 1;

}

void Maze::Init(Vector2 sizeLimit)
{
	m_size = sizeLimit;
	if (m_data != NULL)
		delete[] m_data;
	m_data = new short[(m_size.x) * (m_size.y)];
	m_start = Vector2();
	
	//if (m_exit != NULL)
	//	delete[] m_exit;
	//m_exit = new int[(m_size.x + m_size.y) * 2];
	m_exit.clear();
	m_pathPoints.clear();
	//m_exitCount = 0;
}

void Maze::Init(Vector2 sizeLimit, int difficulty, int exitCount)
{
	m_size = sizeLimit;
	m_difficulty = difficulty;
	m_requireExitCount = exitCount;
	if (m_data != NULL)
		delete[] m_data;
	m_data = new short[(m_size.x) * (m_size.y)];
	m_start = Vector2();
	//if (m_exit != NULL)
	//	delete[] m_exit;
	//m_exit = NULL;
	m_exit.clear();
	m_pathPoints.clear();
	//m_exitCount = 0;
}

void Maze::InitPlayers()
{
	if (m_players != NULL)
		delete[] m_players;
	m_players = new Player[m_exit.size()];
}

short Maze::GetData(Vector2 pos)
{
	return m_data[pos.x + pos.y * m_size.x];
}

short Maze::GetData(int x, int y)
{
	return GetData(Vector2(x, y));
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

	return count;
}

short* Maze::GetDataPointer(Vector2 pos)
{
	return &m_data[pos.x + pos.y * m_size.x];
}

Vector2 Maze::GetPosNearEdge(Vector2 pos)
{
	if (pos.x == 0)
		return Vector2(1, pos.y);
	if (pos.x == m_size.x - 1)
		return Vector2(m_size.x - 2, pos.y);
	if (pos.y == 0)
		return Vector2(pos.x, 1);
	if (pos.y == m_size.y - 1)
		return Vector2(pos.x, m_size.y - 2);
	return Vector2(pos);
}

Vector2 Maze::GetSize()
{
	return m_size;
}

Maze::MAZE_SOLVABLE_E Maze::GetSolvable()
{

	int deadCount=0;
	for(int i=0;i<m_exit.size();i++)
	{
		if (m_players[i].path.size() == 0)
			deadCount++;
	}

	if(deadCount==0)
		return MAZE_SOLVABLE_E::Fully;
	if(deadCount>0 && deadCount< m_exit.size())
		return MAZE_SOLVABLE_E::Partially;
	if (deadCount == m_exit.size())
		return MAZE_SOLVABLE_E::Not;
}

int Maze::GetPlayerDataCount()
{
	int count = 0;
	for (int i = 0; i < m_size.x * m_size.y; i++)
	{
		if (GetData(i) == 'P')
			count++;
	}
	return count;
}

bool Maze::GetPlayerState()
{
	for (int i = 0; i < m_exit.size(); i++)
	{
		if (m_players[i].GetLockState() == true)
		{
			return false;
		}
	}
	return true;
}

int Maze::GetExitCount()
{
	return m_exit.size();
}

int Maze::GetActivePlayerCount()
{
	int counter = 0;
	for (int i = 0; i < m_exit.size(); i++)
	{
		if (!m_players[i].GetFinished() && m_players[i].path.size()>0)
			counter++;
	}
	return counter;
}

/// <summary>
/// Set value of maze size control var
/// </summary>
/// <param name="size">size to be set</param>
void Maze::SetSize(Size2D size)
{
	if (size.x > 0 && size.y > 0)
		m_size = size;
}


bool Maze::CheckGenerated()
{
	if (m_size.x != 0 && m_size.y != 0)
		return true;
	else
		return false;
}

/// <summary>
/// check if the point is on the edge of maze
/// </summary>
/// <param name="pos">position to be checked</param>
/// <returns>pos on edge return true</returns>
bool Maze::CheckOnEdge(Vector2 pos)
{
	return pos.x == 0 || pos.x == m_size.x - 1 || pos.y == 0 || pos.y == m_size.y - 1;
}

bool Maze::CheckInsideCenterRectangle(Vector2 pos, Vector2 quarterSize)
{
	bool verticalCheck = pos.x >= m_size.x / 2 - quarterSize.x && pos.x <= m_size.x / 2 + quarterSize.x;
	bool horizontalCheck = pos.y >= m_size.y / 2 - quarterSize.y && pos.y <= m_size.y / 2 + quarterSize.y;
	return verticalCheck && horizontalCheck;
}

void Maze::SetData(Vector2 pos, short inputData)
{
	if (pos.x<0 || pos.x>m_size.x - 1 || pos.y<0 || pos.y>m_size.y - 1)
		return;
	m_data[pos.Get1DIndex(m_size.x)] = inputData;
}

void Maze::SetData(int x, int y, short inputData)
{
	SetData(Vector2(x, y), inputData);
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

/// <summary>
/// set maze data on edge
/// </summary>
/// <param name="blockData">data to be set</param>
void Maze::SetDataSurrounding(short blockData)
{
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

	//validate maze data
	Size2D calculatedSize;
	string testString;
	int widthCheck = -1;
	
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

	//caculate the size
	inputFile.seekg(0, ios::beg);
	getline(inputFile, testString);
	calculatedSize.x = testString.length();
	int index = 0;
	inputFile.seekg(0, ios::end);
	calculatedSize.y = (int)((int)inputFile.tellg() / (calculatedSize.x + 1) + 0.5);
	inputFile.seekg(0, ios::beg);

	//allocate data space
	this->Init(calculatedSize);

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
				//m_exit[m_exitCount] = i * m_size.x + j;
				//m_exitCount++;
				m_exit.push_back(i * m_size.x + j);
				m_requireExitCount++;
			}
			m_data[i * m_size.x + j] = testString[j];
		}
	}

	//close file
	inputFile.close();
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
			case DEFAULT_STORAGE_PLAYER_CHAR:
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
	centerAreaSize.LimitMin(Vector2(3, 3));
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
	Vector2 direction[5] = { {0,-1},{0,1},{-1,0},{1,0}, {0, 0} };
	if (max(abs(distance.x), abs(distance.y)) <= min(m_size.x / 5, m_size.y / 5))
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

		if (nextPosData == DEFAULT_STORAGE_ROUTE_CHAR && direction[i] != 0)
		{
			if (_DeepFirstFindRecursion(nextPos) == true)
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
	//m_exit = new int[(m_size.x + m_size.y) * 2];
	//m_exitCount = 0;
	for (int i = 1; i < (m_size.x - 1); i++)
	{
		if (GetData(Vector2(i, 1)) == DEFAULT_STORAGE_ROUTE_CHAR)
		{
			m_exit.push_back(i);
			//m_exit[m_exitCount] = i;
			//m_exitCount++;
		}
		if (GetData(Vector2(i, m_size.y - 2)) == DEFAULT_STORAGE_ROUTE_CHAR)
		{
			m_exit.push_back(i + (m_size.y - 1) * m_size.x);
			//m_exitCount++;
		}
	}
	for (int i = 1; i < (m_size.y - 1); i++)
	{
		if (GetData(Vector2(1, i)) == DEFAULT_STORAGE_ROUTE_CHAR)
		{
			m_exit.push_back(i * m_size.x);
			//m_exitCount++;
		}
		if (GetData(Vector2(m_size.x - 2, i)) == DEFAULT_STORAGE_ROUTE_CHAR)
		{
			((i + 1) * m_size.x - 1);
			//m_exitCount++;
		}
	}
	
	
	while (m_exit.size() > m_requireExitCount)
	{
		vector<int>::iterator it = m_exit.begin();
		m_exit.erase(it+rand() % m_exit.size() );
	}
	/*
	for (int i = 0; i < m_exit.size(); i++)
	{
		int randIndex = rand() % m_exitCount;
		int tempInt = m_exit[i];
		m_exit[i] = m_exit[randIndex];
		m_exit[randIndex] = tempInt;
	}
	//if (m_requireExitCount < m_exitCount)
	//	m_exitCount = m_requireExitCount;*/
	for (int i = 0; i < m_exit.size(); i++)
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
	for (int i = 0; i < m_exit.size(); i++)
	{
		Vector2 exitPos = Vector2::Get2DPos(m_size.x, m_exit[i]);
		_DeepFirstFindRecursion(GetPosNearEdge(exitPos));
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
void Maze::FindPlayerPathAStar()
{
	ClearPath();
	for (int i = 0; i < m_exit.size(); i++)
	{
		Vector2 exitPos = Vector2::Get2DPos(m_size.x, m_exit[i]);
		_PlayerAStarFindProcess(GetPosNearEdge(exitPos), m_start, i);
	}
}

/// <summary>
/// run A* pathfinding for each exit
/// </summary>
void Maze::FindPathAStar()
{
	for (int i = 0; i < m_exit.size(); i++)
	{
		Vector2 exitPos = Vector2::Get2DPos(m_size.x, m_exit[i]);
		_AStarFindProcess(GetPosNearEdge(exitPos), m_start);
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
					AFinder.SetParent(nextPos.Get1DIndex(m_size.x), currentPos);
					AFinder.SetValueG(nextPos.Get1DIndex(m_size.x), AFinder.GetData(currentPos).GetValueG() + 1);
					AFinder.SetValueH(nextPos.Get1DIndex(m_size.x), nextPos.Distance(endPos));
				}
				//block alreay in openTable if the G Value will be better, then change Parent;
				else if (AFinder.CheckInOpenTable(nextPos) && AFinder.GetData(nextPos).GetValueG() > AFinder.GetData(currentPos).GetValueG() + 1)
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
		//m_data[currentPos.Get1DIndex(m_size.x)] = 'Z';
		m_pathPoints.push_back(currentPos);
	} while (AFinder.GetData(currentPos).parent != currentPos);
	return true;
}

bool Maze::_PlayerAStarFindProcess(Vector2 startPos, Vector2 endPos, int playerIndex)
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
					AFinder.SetParent(nextPos.Get1DIndex(m_size.x), currentPos);
					AFinder.SetValueG(nextPos.Get1DIndex(m_size.x), AFinder.GetData(currentPos).GetValueG() + 1);
					AFinder.SetValueH(nextPos.Get1DIndex(m_size.x), nextPos.Distance(endPos));
				}
				//block alreay in openTable if the G Value will be better, then change Parent;
				else if (AFinder.CheckInOpenTable(nextPos) && AFinder.GetData(nextPos).GetValueG() > AFinder.GetData(currentPos).GetValueG() + 1)
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
	m_players[playerIndex].SetStartPos(startPos);
	do {
		currentPos = AFinder.GetData(currentPos).parent;
		//m_data[currentPos.Get1DIndex(m_size.x)] = 'Z';
		m_players[playerIndex].path.push_back(currentPos);
		//m_pathPoints.push_back(currentPos);
	} while (AFinder.GetData(currentPos).parent != currentPos);
	m_players[playerIndex].ResetIndex();
	return true;
}

/// <summary>
///	Print the Maze char by char with std::cout
/// </summary>
/// <param name="showPaths">does the solution shows up</param>
void Maze::Print(bool showPlayers/* =FALSE */)
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
				if (showPlayers == true)
					cout << "\033[93m" << DEFAULT_DISPLAY_FINISH_CHAR << "\033[0m";
				else
					cout << "\033[93m" << DEFAULT_DISPLAY_START_CHAR << "\033[0m";
				break;
			case DEFAULT_STORAGE_EXIT_CHAR:
				cout << "\033[32m" << DEFAULT_DISPLAY_EXIT_CHAR << "\033[0m";
				break;
			case DEFAULT_STORAGE_PATH_CHAR:
				cout << "\033[92m" << DEFAULT_DISPLAY_PATH_CHAR << "\033[0m";
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

void Maze::PrintWithPlayers()
{
	//insert players into data structure
	for (int i = 0; i < m_exit.size(); i++)
	{
		m_players[i].RemoveLocked();
		if (m_players[i].path.size() == 0)
			continue;
		if (!m_players[i].GetFinished())
			SetData(m_players[i].GetCurrentMazePos(), 'P');
	}

	//print maze with players
	this->Print(true);

	//clear and update player pos
	for (int i = 0; i < m_exit.size(); i++)
	{
		if (m_players[i].path.size() == 0)
			continue;
		if (GetDataCrossCount(m_players[i].GetCurrentMazePos(), 'S') >= 1)
		{
			SetData(m_players[i].GetCurrentMazePos(), ' ');
			m_players[i].SetFinished();
		}
		//check if next pos can be moved to 
		if (GetData(m_players[i].GetNextMazePos()) != 'P' && m_players[i].GetCurrentStepIndex() != 0)
		{
			SetData(m_players[i].GetCurrentMazePos(), ' ');
			m_players[i].Progress();
		}
		else
		{
			m_players[i].SetLocked();
		}

	}
}
