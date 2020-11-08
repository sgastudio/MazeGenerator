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
	//if (m_players != NULL)
	//	delete[] m_players;
	m_exit.clear();
	m_players.clear();
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
	//if (m_players != NULL)
	//	delete[] m_players;
	//m_players = NULL;
	InitPlayers();
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
	InitPlayers();
	//if (m_exit != NULL)
	//	delete[] m_exit;
	//m_exit = new int[(m_size.x + m_size.y) * 2];
	m_exit.clear();
	m_pathPoints.clear();
	m_requireExitCount = 0;
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
	InitPlayers();
	m_exit.clear();
	m_pathPoints.clear();
	//m_exitCount = 0;
}

void Maze::InitPlayers()
{
	m_players.clear();
	/*
	if (m_players != NULL)
		delete[] m_players;
	m_players = new Player[m_exit.size()];*/
}

int Maze::LimitDifficult(int val)
{
	if (val < 1)
		return 1;
	else if (val > min(m_size.x, m_size.y))
		return min(m_size.x, m_size.y);
	else
		return val;
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

	unsigned deadCount=0;
	for(unsigned i=0;i<m_exit.size();i++)
	{
		if (m_players[i].path.size() == 0)
			deadCount++;
	}

	if(deadCount==0)
		return MAZE_SOLVABLE_E::Fully;
	if(deadCount>0 && deadCount< m_exit.size())
		return MAZE_SOLVABLE_E::Partially;
	return MAZE_SOLVABLE_E::Not;
}

/// <summary>
/// get player's number
/// </summary>
/// <returns>the amount of players</returns>
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

/// <summary>
/// get all player state
/// </summary>
/// <returns>return false if any player locked(waited)</returns>
bool Maze::GetPlayerState()
{
	for (unsigned i = 0; i < m_exit.size(); i++)
	{
		if (m_players[i].GetLockState() == true)
		{
			return false;
		}
	}
	return true;
}

/// <summary>
/// get exit number
/// </summary>
/// <returns>exit number</returns>
int Maze::GetExitCount()
{
	return m_exit.size();
}

/// <summary>
/// get active player number
/// </summary>
/// <returns>active player number</returns>
int Maze::GetActivePlayerCount()
{
	int counter = 0;
	for (unsigned i = 0; i < m_exit.size(); i++)
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

/// <summary>
/// check if maze have been generated or loaded
/// </summary>
/// <returns>if generated/loaded return true</returns>
bool Maze::CheckGenerated()
{
	if (m_size.x != 0 && m_size.y != 0)
		return true;
	else
		return false;
}

/// <summary>
/// check if there are players already in the maze
/// </summary>
/// <returns>return true if there is player in the maze data</returns>
bool Maze::CheckPlayerPathFinderMode()
{
	if (m_players.size() > 0)
		return true;
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

/// <summary>
/// set data to specific pos in m_data
/// </summary>
/// <param name="pos">data position</param>
/// <param name="inputData">value to be set</param>
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

/// <summary>
/// set maze data to input value in side of sepecific rectangle
/// </summary>
/// <param name="center">center of rectangle</param>
/// <param name="quarterSize">half width and height of rectangle</param>
/// <param name="inputData">value to be set</param>
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

/// <summary>
/// load maze from file
/// </summary>
/// <param name="fileName">file name</param>
/// <returns>true if load success</returns>
bool Maze::LoadFromFile(string fileName)
{
	//openfile
	ifstream inputFile;
	inputFile.open(fileName, ios::in);

	//check avaliable
	if (!inputFile.is_open())
		return false;

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
	inputFile.seekg(0, ios::end);
	calculatedSize.y = (int)((int)inputFile.tellg() / (calculatedSize.x + 1) + 0.5);
	inputFile.seekg(0, ios::beg);

	//allocate data space
	this->Init(calculatedSize);

	//load data
	int emptyCount=0;
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
			if (testString[j] == DEFAULT_DISPLAY_PLAYER_CHAR)
			{
				m_players.push_back(Player(Pos2D(j,i)));
			}
			if (testString[j] == DEFAULT_DISPLAY_ROUTE_CHAR)
				emptyCount++;
			m_data[i * m_size.x + j] = testString[j];
		}
	}

	
	m_difficulty = LimitDifficult((int)(((float)max(m_size.x, m_size.y) / m_size.x / m_size.y) * emptyCount));

	//close file
	inputFile.close();
	return true;
}

/// <summary>
/// save maze data to file
/// </summary>
/// <param name="fileName">file name</param>
/// <returns>true if save success</returns>
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
				outputFile << DEFAULT_DISPLAY_PLAYER_CHAR;
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
/// Recursion function for DFS algorithm maze generation
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

/// <summary>
/// Recursion function for DFS path finding
/// </summary>
/// <param name="pos">start position</param>
/// <returns>true if path found</returns>
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

	if (GetData(pos) == DEFAULT_STORAGE_PATH_CHAR)
		return true;
	if (GetDataCrossCount(pos, ' ') < 1 )
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

	//randomize directions
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
				//this->SetData(pos, DEFAULT_STORAGE_ROUTE_CHAR);
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
	//horziontal
	for (int i = 1; i < (m_size.x - 1); i++)
	{
		if (GetData(Vector2(i, 1)) == DEFAULT_STORAGE_ROUTE_CHAR)
		{
			m_exit.push_back(i);
		}
		if (GetData(Vector2(i, m_size.y - 2)) == DEFAULT_STORAGE_ROUTE_CHAR)
		{
			m_exit.push_back(i + (m_size.y - 1) * m_size.x);
		}
	}
	//vertical
	for (int i = 1; i < (m_size.y - 1); i++)
	{
		if (GetData(Vector2(1, i)) == DEFAULT_STORAGE_ROUTE_CHAR)
		{
			m_exit.push_back(i * m_size.x);
		}
		if (GetData(Vector2(m_size.x - 2, i)) == DEFAULT_STORAGE_ROUTE_CHAR)
		{
			((i + 1) * m_size.x - 1);
		}
	}
	
	//drop uncessary exits
	while (m_exit.size() > m_requireExitCount)
	{
		vector<int>::iterator it = m_exit.begin();
		m_exit.erase(it+rand() % m_exit.size() );
	}
	//insert into maze data
	for (unsigned i = 0; i < m_exit.size(); i++)
	{
		SetData(m_exit[i], DEFAULT_STORAGE_EXIT_CHAR);
	}
}

/// <summary>
/// Clear pathfiner solution of the maze for inner maze data
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
	if (CheckPlayerPathFinderMode())
	{
		for (unsigned i = 0; i < m_players.size(); i++)
		{
			m_data[m_players[i].GetCurrentMazePos().Get1DIndex(m_size.x)] = 'P';
		}
	}
}

/// <summary>
/// run DFS pathfinding for each exit
/// </summary>
void Maze::FindPathDeepFirst()
{
	if (CheckPlayerPathFinderMode())
	{
		for (unsigned i = 0; i < m_players.size(); i++)
		{
			Vector2 exitPos = m_players[i].GetCurrentMazePos();
			_DeepFirstFindRecursion(exitPos);
		}
	}
	else
	{
		for (unsigned i = 0; i < m_exit.size(); i++)
		{
			Vector2 exitPos = Vector2::Get2DPos(m_size.x, m_exit[i]);
			_DeepFirstFindRecursion(GetPosNearEdge(exitPos));
		}
	}
	

	for (int i = 0; i < m_size.x * m_size.y; i++)
	{
		if (m_data[i] == DEFAULT_STORAGE_PATH_CHAR)
		{
			m_pathPoints.push_back(Vector2::Get2DPos(m_size.x, i));
			m_data[i] = DEFAULT_STORAGE_ROUTE_CHAR;
		}
	}
}

/// <summary>
/// run A* pathfinding for each exit
/// </summary>
void Maze::FindPlayerPathAStar()
{
	ClearPath();
	if (CheckPlayerPathFinderMode())
	{
		for (unsigned i = 0; i < m_players.size(); i++)
		{
			if (m_players[i].GetFinished())
				continue;
			Vector2 exitPos = m_players[i].GetCurrentMazePos();
			_PlayerAStarFindProcess(exitPos, m_start, i);
		}
	}
	else
	{
		for (unsigned i = 0; i < m_exit.size(); i++)
		{
			Vector2 exitPos = Vector2::Get2DPos(m_size.x, m_exit[i]);
			m_players.push_back(exitPos);
			_PlayerAStarFindProcess(GetPosNearEdge(exitPos), m_start, i);
		}
	}
	
}

/// <summary>
/// run A* pathfinding for each exit
/// </summary>
void Maze::FindPathAStar()
{
	if (CheckPlayerPathFinderMode())
	{
		for (unsigned i = 0; i < m_players.size(); i++)
		{
			Vector2 exitPos = m_players[i].GetCurrentMazePos();
			_AStarFindProcess(exitPos, m_start);
		}
	}
	else
	{
		for (unsigned i = 0; i < m_exit.size(); i++)
		{
			Vector2 exitPos = Vector2::Get2DPos(m_size.x, m_exit[i]);

			_AStarFindProcess(GetPosNearEdge(exitPos), m_start);
		}
	}
	
}

/// <summary>
/// A* algorithm path finder function
/// </summary>
/// <param name="startPos">postion for start finding</param>
/// <param name="endPos">the end of finding</param>
/// <returns>true if path found</returns>
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

		//check 4 directions of the currentPos
		for (int i = 0; i < 4; i++)
		{
			nextPos = currentPos + direction[i];
			short nextPosData = m_data[nextPos.Get1DIndex(m_size.x)];

			//block reachable and not in closeTable
			if ((nextPosData == ' ' || nextPosData == 'S' || nextPosData == 'P' || nextPosData == 'F') && !AFinder.CheckInCloseTable(nextPos))
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

/// <summary>
/// player's A* path finding algorithm
/// </summary>
/// <param name="startPos">position to start</param>
/// <param name="endPos">position of finish</param>
/// <param name="playerIndex">player index in m_players</param>
/// <returns>true if path found</returns>
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
			if ((nextPosData == ' ' || nextPosData == 'S' || nextPosData == 'P') && !AFinder.CheckInCloseTable(nextPos))
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
				cout << "\033[92m" << DEFAULT_DISPLAY_EXIT_CHAR << "\033[0m";
				break;
			case DEFAULT_STORAGE_PATH_CHAR:
				cout << "\033[32m" << DEFAULT_DISPLAY_PATH_CHAR << "\033[0m";
				break;
			case DEFAULT_STORAGE_ROUTE_CHAR:
				cout << DEFAULT_DISPLAY_ROUTE_CHAR;
				break;
			case DEFAULT_STORAGE_PLAYER_CHAR:
				cout << "\033[96m" << DEFAULT_DISPLAY_PLAYER_CHAR << "\033[0m";
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
/// Print and process player movement, work with plackback feature 
/// </summary>
void Maze::PrintWithPlayers()
{
	//insert players into data structure
	for (unsigned i = 0; i < m_players.size(); i++)
	{
		if (m_players[i].path.size() == 0)
			continue;
		if (m_players[i].GetFinished())
			continue;
		if (!m_players[i].GetFinished())
			SetData(m_players[i].GetCurrentMazePos(), 'P');
	}

	//print maze with players
	this->Print(true);

	
}


void Maze::UpdatePlayers() {
	//clear and update player pos
	for (unsigned i = 0; i < m_players.size(); i++)
	{
		if (m_players[i].path.size() == 0)
			continue;
		if (m_players[i].GetFinished())
			continue;

		//player finished
		if (GetDataCrossCount(m_players[i].GetCurrentMazePos(), 'S') >= 1)
		{
			m_players[i].RemoveLocked();
			SetData(m_players[i].GetCurrentMazePos(), ' ');
			m_players[i].SetFinished();
		}
		//check if next pos can moved to //&& m_players[i].GetCurrentStepIndex() != 0
		if (GetData(m_players[i].GetNextMazePos()) != 'P')
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