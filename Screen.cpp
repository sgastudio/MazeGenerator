#include "Screen.h"

Screen::Screen()
{
	m_msg = "";
	this->hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hOut, &this->bInfo);
}

Screen::~Screen()
{
	CloseHandle(hOut);
}

void Screen::Menu()
{
	PrintLogo();
	PrintMainMenu();
}

void Screen::PrintLogo()
{
	string screenText;
	screenText += "\t+---------------------------------------------+\n";
	screenText += "\t|                                             |\n";
	screenText += "\t| $$\\      $$\\                                |\n";
	screenText += "\t| $$$\\    $$$ |                               |\n";
	screenText += "\t| $$$$\\  $$$$ | $$$$$$\\  $$$$$$$$\\  $$$$$$\\   |\n";
	screenText += "\t| $$\\$$\\$$ $$ | \\____$$\\ \\____$$  |$$  __$$\\  |\n";
	screenText += "\t| $$ \\$$$  $$ | $$$$$$$ |  $$$$ _/ $$$$$$$$ | |\n";
	screenText += "\t| $$ |\\$  /$$ |$$  __$$ | $$  _/   $$   ____| |\n";
	screenText += "\t| $$ | \\_/ $$ |\\$$$$$$$ |$$$$$$$$\\ \\$$$$$$$\\  |\n";
	screenText += "\t| \\__|     \\__| \\_______|\\________| \\_______| |\n";
	screenText += "\t|                                             |\n";
	screenText += "\t+---------------------------------------------+\n\n";
	cout << screenText;
}

void Screen::PrintMainMenu()
{
	string screenText;
	screenText += "\t" + m_msg + "\n";
	m_msg = "";
	screenText += "\t+---------------------------------------------+\n";
	screenText += "\t|                                             |\n";
	screenText += "\t|         1. Generate                         |\n";
	screenText += "\t|                                             |\n";
	screenText += "\t|         2. Load Maze                        |\n";
	screenText += "\t|                                             |\n";
	screenText += "\t|         3. Save Maze                        |\n";
	screenText += "\t|                                             |\n";
	screenText += "\t|         4. Preview                          |\n";
	screenText += "\t|                                             |\n";
	screenText += "\t|         5. Play Back                        |\n";
	screenText += "\t|                                             |\n";
	screenText += "\t|         6. Credits                          |\n";
	screenText += "\t|                                             |\n";
	screenText += "\t|         7. Exit                             |\n";
	screenText += "\t|                                             |\n";
	screenText += "\t+---------------------------------------------+\n";
	screenText += "\tPlease input number to select function you need:\n";
	cout << screenText;
}

void Screen::PrintCredits()
{
	string screenText;
	screenText += "\t+---------------------------------------------+\n";
	screenText += "\t|                                             |\n";
	screenText += "\t|    Code by Flying_Penguin (Tangwei Meng)    |\n";
	screenText += "\t|                 03 Nov 2020                 |\n";
	screenText += "\t|    Email:                                   |\n";
	screenText += "\t|    	t.meng3@newcastle.ac.uk               |\n";
	screenText += "\t|                                             |\n";
	screenText += "\t+---------------------------------------------+\n";
	cout << screenText;
}

/// <summary>
/// display function for playback feature
/// </summary>
/// <param name="size">size of maze</param>
/// <param name="exitCount">exit count number</param>
/// <param name="activePlayerCount">active players number</param>
/// <param name="solveState">if maze solve able</param>
/// <param name="playerState">if play locked</param>
void Screen::PrintMazeInfo(Size2D size, int exitCount, int activePlayerCount, Maze::MAZE_SOLVABLE_E solveState, bool playerState)
{
	string screenText;
	screenText += "Maze\n";
	screenText += to_string(size.x) + "x" + to_string(size.y) +" " + to_string(activePlayerCount) +"/"+ to_string(exitCount) + " Players\n";
	switch (solveState)
	{
	case Maze::MAZE_SOLVABLE_E::Fully:
		screenText += "\033[92mFully Solvable\033[0m\n";
		break;
	case Maze::MAZE_SOLVABLE_E::Partially:
		screenText += "\033[93mPartially Solvable\033[0m\n";
		break;
	case Maze::MAZE_SOLVABLE_E::Not:
	default:
		screenText += "\033[91mNot Solvable\033[0m\n";
	}
	screenText += (playerState ? "\033[92mPlayer Good\033[0m" : "\033[91mPlayer Locked\033[0m");
	screenText += "\n\n";
	cout << screenText;
}

void Screen::Pause()
{
	system("pause");
}

char Screen::PauseEX(string displayText)
{
	cout << displayText;
	return _getch();
}

void Screen::Clear()
{
	system("cls");
}

void Screen::SetColor(string colorText)
{
	string text = "color " + colorText;
	system(text.c_str());
}


void Screen::SetMsg(string text)
{
	m_msg = text;
}

void Screen::SetMsgError(string text)
{
	this->SetMsg("\033[91m"+text+"\033[0m");
}

void Screen::SetMsgWarn(string text)
{
	this->SetMsg("\033[93m" + text + "\033[0m");
}

void Screen::SetMsgGood(string text)
{
	this->SetMsg("\033[92m" + text + "\033[0m");
}

void Screen::SetWndTitle(string text)
{
	SetConsoleTitleA(text.c_str());
}

void Screen::SetWndSize(Vector2 size)
{
	SetWndSize(size.x, size.y);
}

void Screen::SetWndSize(int width, int height)
{
	string strControlText = "mode con cols=" + to_string(width) + " lines=" + to_string(height);
	system(strControlText.c_str());
}

void Screen::SetBufSize(Vector2 size)
{
	SetBufSize(size.x, size.y);
}

void Screen::SetBufSize(int width, int height)
{
	COORD coordSize = { (short)width, (short)height };
	SetConsoleScreenBufferSize(hOut, coordSize); //set buffer size
}