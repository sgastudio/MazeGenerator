#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <string>

#include "maze.h"
#include "Screen.h"

using namespace std;
enum MENU_SELECTIONS_E {
Generate=1,
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
    int input;
    do {
        screenAgent.Clear();
        screenAgent.Menu();
        cin >> input;
        screenAgent.Clear();
        switch (input)
        {
        case MENU_SELECTIONS_E::Generate:
            mazeAgent.Init(vector2(50, 50));
            mazeAgent.Generate();
            screenAgent.SetMsg("\033[32mMaze 50x50 generated!\033[0m");
            break;
        case MENU_SELECTIONS_E::Load:
            break;
        case MENU_SELECTIONS_E::Save:
            break;
        case MENU_SELECTIONS_E::Preview:
            if (!mazeAgent.CheckGenerated())
            {
                screenAgent.SetMsg("\033[31mPlease generate/load the maze before preview it!\033[0m");
                break;
            }
            if (screenAgent.InputBool("Do you want to see the path to exit? (0 = No, 1 = Yes)"))
            {
                mazeAgent.Print(true);
            }
            else
            {
                mazeAgent.Print(false);
            }
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


   
    //mazeAgent.LoadFromFile("testmaze1.txt");
    mazeAgent.Print(true);
    mazeAgent.FindPath();
    mazeAgent.Print(true);

    mazeAgent.SaveToFile("test3.txt");
    cin;
    //mazeAgent.LoadFromFile("test2.txt");
    //mazeAgent.Print();


}