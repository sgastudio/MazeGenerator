#include <windows.h>
#include <string>

#include "maze.h"
#include "Screen.h"

using namespace std;

enum MENU_SELECTIONS_E {
	Generate = '1',
	Load,
	Save,
	Preview,
	PlayBack,
	Credit,
	Exit,
};

int main(void)
{
	Screen screenAgent;
	Maze mazeAgent;
	string fileAddr;
	char input;
	int exitCount;
	int density;
	int densityMax;
	bool needOfPathfinding = false;
	int playbackLoopTime=0;
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
			density = screenAgent.InputInt("How dense should it be?\n\t(min=1 max=" + to_string(densityMax) + ")");
			Vector2::LimitInt(&density, Vector2(1, densityMax));
			mazeAgent.Init(mazeSize, densityMax - density + 1, exitCount);
			mazeAgent.Generate();
			screenAgent.SetMsgGood("Maze " + to_string(mazeSize.x) + "x" + to_string(mazeSize.y) + " " + to_string(density) + " level " + to_string(exitCount) + " exits generated.");
			break;
		
		case MENU_SELECTIONS_E::Load:
			screenAgent.PrintLogo();
			fileAddr = screenAgent.InputString("Please the filename you want to load:");
			if (fileAddr.empty() || fileAddr == "")
			{
				screenAgent.SetMsgWarn("Filename cannot be empty!");
				break;
			}
			if (mazeAgent.LoadFromFile(fileAddr))
				screenAgent.SetMsgGood("Maze loaded.You can preview it now.");
			else
				screenAgent.SetMsgError("Maze load fail.");
			break;
		
		case MENU_SELECTIONS_E::Save:
			if (!mazeAgent.CheckGenerated())
			{
				screenAgent.SetMsgWarn("Please generate/load the maze before save it!");
				break;
			}
			screenAgent.PrintLogo();
			fileAddr = screenAgent.InputString("Please the filename you want to save:");
			if (fileAddr.empty() || fileAddr == "")
			{
				screenAgent.SetMsgWarn("Filename cannot be empty!");
				break;
			}
			if (mazeAgent.SaveToFile(fileAddr))
				screenAgent.SetMsgGood("Maze saved.");
			else
				screenAgent.SetMsgError("Maze save fail.");
			break;
		
		case MENU_SELECTIONS_E::Preview:
			if (!mazeAgent.CheckGenerated())
			{
				screenAgent.SetMsgWarn("Please generate/load the maze before preview it!");
				break;
			}
			screenAgent.PrintLogo();
			needOfPathfinding = screenAgent.InputBool("Do you want to see the path to exit? \n\t(0 = No, 1 = Yes)");
			mazeAgent.ClearPath();
			if(needOfPathfinding)
				mazeAgent.FindPath(screenAgent.InputInt("Which Kind of pathfinding method do you want? \n\t(0 = A* Algorithm, 1 = DFS Algorithm)"));
			screenAgent.SetBufSize(mazeAgent.GetSize() + 2);
			screenAgent.SetWndSize(mazeAgent.GetSize() + 2);
			mazeAgent.Print();
			screenAgent.PauseEX("Press any key to continue");
			break;
		
		case MENU_SELECTIONS_E::PlayBack:
			if (!mazeAgent.CheckGenerated())
			{
				screenAgent.SetMsgWarn("Please generate/load the maze before playback!");
				break;
			}
			screenAgent.PrintLogo();
			playbackLoopTime = screenAgent.InputInt("How many mazes do you want to check? \n\t(at least 1)");
			Vector2::LimitInt(&playbackLoopTime, Vector2(1, INT_MAX));
			screenAgent.SetBufSize(mazeAgent.GetSize() + 3);
			screenAgent.SetWndSize(mazeAgent.GetSize() + 3);
			while (playbackLoopTime > 0)
			{
				mazeAgent.InitPlayers();
				mazeAgent.FindPlayerPathAStar();
				do
				{
					screenAgent.Clear();
					mazeAgent.PrintWithPlayers();
				} while (screenAgent.PauseEX("Press 'e' to exit\nother key to continue") != 'e' && mazeAgent.GetActivePlayerCount() > 0);
				mazeAgent.Generate();
				playbackLoopTime--;
			}
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
			screenAgent.SetMsgError("Invalid input");
			input = 0;
			break;
		}
	} while (true);
}