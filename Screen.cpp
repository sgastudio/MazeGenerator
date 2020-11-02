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
	screenText += "\t|         5. Credits                          |\n";
	screenText += "\t|                                             |\n";
	screenText += "\t|         6. Exit                             |\n";
	screenText += "\t|                                             |\n";
	screenText += "\t+---------------------------------------------+\n";
	screenText += "\tPlease input number to select functions\n\t";
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

void Screen::Pause()
{
	system("pause");
}

void Screen::Clear()
{
	system("cls");
}

void Screen::SetMsg(string text)
{
	m_msg = text;
}

void Screen::SetWndTitle(LPCWSTR text)
{
	SetConsoleTitle(text);
}

void Screen::SetWndSize(int width, int height)
{
	//string strControlText = "mode con cols=" + width ;
	//system(strControlText);
}

string Screen::InputString(string text)
{
	string screenText;
	screenText += "\t" + text + "\n\t";
	cout << screenText;
	string input;
	cin >> input;
	return input;

}

bool Screen::InputBool(string text)
{
	string screenText;
	screenText += "\t" + text + "\n\t";
	cout << screenText;
	int input;
	cin >> input;
	return input;

}

Vector2 Screen::InputVector2(string text)
{

}
