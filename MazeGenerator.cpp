#include <windows.h>
#include <string>

#include "maze.h"
#include "Screen.h"

using namespace std;

enum MENU_SELECTIONS_E {
	Generate = 1,
	Load,
	Save,
	Preview,
	Credit,
	Exit,
};

int main(void)
{
	Screen screenAgent;
	Maze mazeAgent;
	string fileAddr;
	int input;
	int exitCount;
	int density;
	int densityMax;
	bool needOfPathfinding = false;
	Vector2 mazeSize;

	screenAgent.SetWndTitle(TEXT("Maze Generator"));
	screenAgent.SetMsg("Set to 8x8 Dot Font for better experience");
	do {
		screenAgent.Clear();
		screenAgent.SetWndSize(63, 40);
		//screenAgent.SetBufSize(65, 50);
		screenAgent.Menu();
		cin >> input;
		screenAgent.Clear();

		switch (input)
		{
		case MENU_SELECTIONS_E::Generate:
			screenAgent.PrintLogo();
			mazeSize = screenAgent.InputVector2("How big the maze should be? (e.g. 10 10)\n\tMin edge length should be 10.");
			mazeSize.LimitMin(Vector2(10, 10));
			exitCount = screenAgent.InputInt("How many exits do you want?\n\t(min=1 max=" + to_string((mazeSize.x + mazeSize.y) * 2 - 4) + ")");
			Vector2::LimitInt(&exitCount, Vector2(1, (mazeSize.x + mazeSize.y) * 2 - 4));
			densityMax = max(mazeSize.x, mazeSize.y) / 10;
			density = screenAgent.InputInt("How dense it should be?\n\t(min=1 max=" + to_string(densityMax) + ")");
			Vector2::LimitInt(&density, Vector2(1, densityMax));
			mazeAgent.Init(mazeSize, densityMax - density + 1, exitCount);
			mazeAgent.Generate();
			//mazeAgent.FindPath();
			screenAgent.SetMsg("\033[32mMaze " + to_string(mazeSize.x) + "x" + to_string(mazeSize.y) + " " + to_string(density) + " level " + to_string(exitCount) + " exits generated.\033[0m");
			break;
		case MENU_SELECTIONS_E::Load:
			screenAgent.PrintLogo();
			fileAddr = screenAgent.InputString("Please the filename you want to load:");
			if (fileAddr.empty() || fileAddr == "")
			{
				screenAgent.SetMsg("\033[31mFilename cannot be empty!\033[0m");
				break;
			}
			if (mazeAgent.LoadFromFile(fileAddr))
				screenAgent.SetMsg("\033[32mMaze loaded.You can preview it now.\033[0m");
			else
				screenAgent.SetMsg("\033[33mMaze load fail.\033[0m");
			break;
		case MENU_SELECTIONS_E::Save:
			if (!mazeAgent.CheckGenerated())
			{
				screenAgent.SetMsg("\033[31mPlease generate/load the maze before save it!\033[0m");
				break;
			}
			screenAgent.PrintLogo();
			fileAddr = screenAgent.InputString("Please the filename you want to save:");
			if (fileAddr.empty() || fileAddr == "")
			{
				screenAgent.SetMsg("\033[31mFilename cannot be empty!\033[0m");
				break;
			}
			if (mazeAgent.SaveToFile(fileAddr))
				screenAgent.SetMsg("\033[32mMaze saved.\033[0m");
			else
				screenAgent.SetMsg("\033[33mMaze save fail.\033[0m");
			break;
		case MENU_SELECTIONS_E::Preview:
			if (!mazeAgent.CheckGenerated())
			{
				screenAgent.SetMsg("\033[31mPlease generate/load the maze before preview it!\033[0m");
				break;
			}
			screenAgent.PrintLogo();
			needOfPathfinding = screenAgent.InputBool("Do you want to see the path to exit? \n\t(0 = No, 1 = Yes)");
			if(needOfPathfinding)
				mazeAgent.FindPath(screenAgent.InputInt("Which Kind of pathfinding method do you want? \n\t(0 = A* Algorithm, 1 = DFS Algorithm)"));
			screenAgent.SetBufSize(mazeAgent.GetSize() + 1);
			screenAgent.SetWndSize(mazeAgent.GetSize() + 1);
			mazeAgent.Print(needOfPathfinding);
			screenAgent.Pause();
			break;
		case MENU_SELECTIONS_E::Credit:
			screenAgent.PrintLogo();
			screenAgent.PrintCredits();
			screenAgent.Pause();
			break;
		case MENU_SELECTIONS_E::Exit:
			return 0;
			break;
		default:
			screenAgent.SetMsg("Invalid input");
			break;
		}
	} while (true);
}