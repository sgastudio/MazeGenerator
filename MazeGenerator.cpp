#include <windows.h>
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
    string fileAddr;
    int input;
    int exitCount;
    int density;
    int densityMax;
    Vector2 mazeSize;

    screenAgent.SetWndTitle(TEXT("Maze Generator"));
    screenAgent.SetMsg("Set the 8x8 dot Font to gain better experience");
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
            mazeSize = screenAgent.InputVector2("Please input how big the maze should be? \n\tAt least 10x10(e.g. 10 10)");
            mazeSize.LimitMin(Vector2(10, 10));
            exitCount = screenAgent.InputInt("Please input how many exits do you need ?\n\t(min=1 max=" + to_string((mazeSize.x + mazeSize.y) * 2 - 4)+")");
            Vector2::LimitInt(&exitCount, Vector2(1, (mazeSize.x + mazeSize.y) * 2 - 4));
            densityMax = max(mazeSize.x, mazeSize.y) / 10;
            density = screenAgent.InputInt("How dense should it be?\n\t(min=1 max="+to_string(densityMax)+")");
            Vector2::LimitInt(&density, Vector2(1, densityMax));
            mazeAgent.Init(mazeSize, densityMax - density + 1, exitCount);
            mazeAgent.Generate();
            //mazeAgent.FindPath();
            screenAgent.SetMsg("\033[32mMaze " + to_string(mazeSize.x) + "x" + to_string(mazeSize.y) + " "+to_string(density)+" level " + to_string(exitCount) + " exits generated.\033[0m");
            break;
        case MENU_SELECTIONS_E::Load:
            fileAddr = screenAgent.InputString("Please the filename you want to load:");
            if (fileAddr.empty() || fileAddr == "")
            {
                screenAgent.SetMsg("\033[31mFilename cannot be empty!\033[0m");
                break;
            }
            if (mazeAgent.LoadFromFile(fileAddr))
                screenAgent.SetMsg("\033[32mMaze loaded.\033[0m");
            else
                screenAgent.SetMsg("\033[33mMaze load fail.\033[0m");
            break;
        case MENU_SELECTIONS_E::Save:
            if (!mazeAgent.CheckGenerated())
            {
                screenAgent.SetMsg("\033[31mPlease generate/load the maze before save it!\033[0m");
                break;
            }
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
            screenAgent.SetWndSize(mazeSize+1);
            //screenAgent.SetBufSize(mazeSize);
            if (screenAgent.InputBool("Do you want to see the path to exit? \n\t(0 = No, 1 = Yes)"))
            {
                screenAgent.Clear();

                mazeAgent.Print(true);
            }
            else
            {
                screenAgent.Clear();
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

    /*
    HANDLE hOut;

    CONSOLE_SCREEN_BUFFER_INFO bInfo; // 存储窗口信息

    COORD pos = { 0, 0 };

    // 获取标准输出设备句柄

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // 获取窗口信息

    GetConsoleScreenBufferInfo(hOut, &bInfo);
    SetConsoleTitle("Maze Generator");

    printf("\n\nThe soul selects her own society\n");

    printf("Then shuts the door\n");

    printf("On her devine majority\n");

    printf("Obtrude no more\n\n");
    COORD size = { 80, 25 };
    SetConsoleScreenBufferSize(hOut, size); // 重新设置缓冲区大小
    SMALL_RECT rc = { 0,0, 80 - 1, 25 - 1 }; // 重置窗口位置和大小



    _getch();

    // 向窗口中填充字符以获得清屏的效果

    FillConsoleOutputCharacter(hOut, ' ', bInfo.dwSize.X * bInfo.dwSize.Y, pos, NULL);

    // 关闭标准输出设备句柄

    CloseHandle(hOut);

    return 0;
    */
}