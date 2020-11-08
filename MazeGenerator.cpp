#include <windows.h>
#include <string>

#include "maze.h"
#include "Screen.h"

using namespace std;

const enum MENU_SELECTIONS_E {
	Generate = '1',
	Load,
	Save,
	Preview,
	PlayBack,
	Credit,
	Exit,
};

Screen screenAgent;
Maze mazeAgent;

bool generateMaze()
{
	Vector2 mazeSize;
	int density;
	int densityMax;
	int exitCount;

	screenAgent.PrintLogo();

	//ask for parameters
	//ask for size
	screenAgent.Input(&mazeSize, "How big the maze should be? (e.g. 10 10)\n\tMin edge length should be 10.");
	mazeSize.LimitMin(Vector2(10, 10));
	//ask for exit count
	screenAgent.Input(&exitCount,"How many exits do you want?\n\t(min=1 max=" + to_string((mazeSize.x + mazeSize.y) * 2 - 4) + ")");
	Vector2::LimitInt(&exitCount, Vector2(1, (mazeSize.x + mazeSize.y) * 2 - 4));
	//ask for density
	densityMax = min(mazeSize.x, mazeSize.y);
	screenAgent.Input(&density,"How dense should it be?\n\t(min=1 max=" + to_string(densityMax) + ")");
	Vector2::LimitInt(&density, Vector2(1, densityMax));

	//generate maze
	mazeAgent.Init(mazeSize, densityMax - density + 1, exitCount);
	mazeAgent.Generate();

	//feedback
	screenAgent.SetMsgGood("Maze " + to_string(mazeSize.x) + "x" + to_string(mazeSize.y) + " " + to_string(density) + " level " + to_string(exitCount) + " exits generated.");
	return true;
}

void loadMaze()
{
	string fileAddr;
	screenAgent.PrintLogo();
	//ask and check filename
	screenAgent.Input(&fileAddr, "Please the filename you want to load:");
	if (fileAddr.empty() || fileAddr == "")
	{
		screenAgent.SetMsgWarn("Filename cannot be empty!");
		return;
	}
	//check if load success
	if (mazeAgent.LoadFromFile(fileAddr))
		screenAgent.SetMsgGood("Maze loaded.You can preview it now.");
	else
		screenAgent.SetMsgError("Maze load fail.");
}

void saveMaze()
{
	string fileAddr;
	//check maze available
	if (!mazeAgent.CheckGenerated())
	{
		screenAgent.SetMsgWarn("Please generate/load the maze before save it!");
		return;
	}

	screenAgent.PrintLogo();
	//ask and check filename
	screenAgent.Input(&fileAddr, "Please the filename you want to save:");
	if (fileAddr.empty() || fileAddr == "")
	{
		screenAgent.SetMsgWarn("Filename cannot be empty!");
		return;
	}
	//check if save success
	if (mazeAgent.SaveToFile(fileAddr))
		screenAgent.SetMsgGood("Maze saved.");
	else
		screenAgent.SetMsgError("Maze save fail.");
}

void previewMaze()
{
	bool needOfPathfinding = false;
	int findingAlgorithm = 0;
	//check maze available
	if (!mazeAgent.CheckGenerated())
	{
		screenAgent.SetMsgWarn("Please generate/load the maze before preview it!");
		return;
	}
	screenAgent.PrintLogo();
	//ask if pathfinding is needed
	 screenAgent.InputEX(&needOfPathfinding, "Do you want to see the path to exit? \n\t(0 = No, 1 = Yes)");
	 screenAgent.InputEX(&findingAlgorithm, "Which Kind of pathfinding method do you want? \n\t(0 = A* Algorithm, 1 = DFS Algorithm)");
	//ask for pathfinding algorithm if it is needed
	if (needOfPathfinding)
		mazeAgent.FindPath(findingAlgorithm);
	else
		mazeAgent.ClearPath();
	//set console window size
	screenAgent.SetBufSize(mazeAgent.GetSize() + 2);
	screenAgent.SetWndSize(mazeAgent.GetSize() + 2);
	//print maze
	mazeAgent.Print();
	screenAgent.Pause();
}

void playbackMaze()
{
	int playbackLoopTime = 0;
	char inputChar;
	//check if maze created or loaded
	Vector2 mazeSize;
	if (!mazeAgent.CheckGenerated())
	{
		screenAgent.SetMsgWarn("Please generate/load the maze before playback!");
		return;
	}

	screenAgent.PrintLogo();

	//ask for series generation
	screenAgent.Input(&playbackLoopTime, "How many simliar mazes do you want to check? \n\t(from 0 - unlimited)");
	Vector2::LimitInt(&playbackLoopTime, Vector2(0, INT_MAX));
	
	//set windows size
	mazeSize = mazeAgent.GetSize() + 1;
	mazeSize.y += 9;
	screenAgent.SetBufSize(mazeSize);
	screenAgent.SetWndSize(mazeSize);

	//display loop
	while (playbackLoopTime >= 0)
	{
		mazeAgent.FindPlayerPathAStar();
		do
		{
			screenAgent.Clear();
			screenAgent.PrintMazeInfo(mazeAgent.GetSize(), mazeAgent.GetExitCount(), mazeAgent.GetActivePlayerCount(), mazeAgent.GetSolvable(), mazeAgent.GetPlayerState());
			mazeAgent.PrintWithPlayers();
			inputChar = screenAgent.PauseEX("\nPress\n'e' = exit\nOther key = continue ");
			//exit function
			if (inputChar == 'e')
			{
				return;
			}
		} while (mazeAgent.GetActivePlayerCount() > 0);

		playbackLoopTime--;
		if (playbackLoopTime >= 0)
		{
			mazeAgent.Generate();
			mazeAgent.InitPlayers();
		
		}
	}
}

void displayCredits()
{
	screenAgent.PrintLogo();
	screenAgent.PrintCredits();
	screenAgent.Pause();
}

int main(void)
{
	char input;
	screenAgent.SetWndTitle("Maze Generator");
	screenAgent.SetMsg("Set to 8x8 Dot Font for better experience");

	do {
		screenAgent.Clear();
		screenAgent.SetWndSize(63, 40);
		screenAgent.Menu();
		screenAgent.InputEX(&input,"");

		screenAgent.Clear();
		switch (input)
		{
		case MENU_SELECTIONS_E::Generate:
			generateMaze();
			break;
		case MENU_SELECTIONS_E::Load:
			loadMaze();
			break;
		case MENU_SELECTIONS_E::Save:
			saveMaze();
			break;
		case MENU_SELECTIONS_E::Preview:
			previewMaze();
			break;
		case MENU_SELECTIONS_E::PlayBack:
			playbackMaze();
			break;
		case MENU_SELECTIONS_E::Credit:
			displayCredits();
			break;
		case MENU_SELECTIONS_E::Exit:
			return 0;
			break;
		default:
			screenAgent.SetMsgError("Invalid input");
			input = 0;
			break;
		}
	} while (true);
}