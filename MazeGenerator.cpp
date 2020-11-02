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
    string fileAddr;
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
            screenAgent.SetMsg("\033[32mMaze 50x50 generated.\033[0m");
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
            fileAddr = screenAgent.InputString("Please the filename you want to load:");
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
            if (screenAgent.InputBool("Do you want to see the path to exit? (0 = No, 1 = Yes)"))
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


   
    //mazeAgent.LoadFromFile("testmaze1.txt");
    mazeAgent.Print(true);
    mazeAgent.FindPath();
    mazeAgent.Print(true);

    mazeAgent.SaveToFile("test3.txt");
    cin;
    //mazeAgent.LoadFromFile("test2.txt");
    //mazeAgent.Print();



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

    SetConsoleWindowInfo(hOut, true, &rc);

    _getch();

    // 向窗口中填充字符以获得清屏的效果

    FillConsoleOutputCharacter(hOut, ' ', bInfo.dwSize.X * bInfo.dwSize.Y, pos, NULL);

    // 关闭标准输出设备句柄

    CloseHandle(hOut);

    return 0;
    */
}