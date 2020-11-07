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
	screenText += "\tPlease input number to select function you need:\n\t";
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

void Screen::SetFont()
{
	/*
	CONSOLE_FONT_INFOEX info;
	GetCurrentConsoleFontEx(hOut, false, &info);
	cout << info.nFont << " " << info.dwFontSize.X << " " << info.dwFontSize.Y << " " << info.FontFamily<<" "<<info.cbSize<<" "<< info.FontWeight<<" "<<info.FaceName;
	Sleep(5);
	info.FontFamily = 0;
	//00D8F7E4
	SetCurrentConsoleFontEx(hOut, false, &info);
	GetCurrentConsoleFontEx(hOut, false, &info);
	cout <<endl<< info.nFont << " " << info.dwFontSize.X << " " << info.dwFontSize.Y << " " << info.FontFamily << " " << info.cbSize << " " << info.FontWeight << " " << info.FaceName;
	*/
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

void Screen::SetWndTitle(LPCWSTR text)
{
	SetConsoleTitle(text);
}

void Screen::SetWndSize(Vector2 size)
{
	SetWndSize(size.x, size.y);
}

void Screen::SetWndSize(int width, int height)
{
	//SMALL_RECT rc = { 0,0, width - 1, height - 1 }; // reset wnd pos and size
	//SetConsoleWindowInfo(hOut, true, &rc);
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
	SetConsoleScreenBufferSize(hOut, coordSize); // Reset buffer size
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

int Screen::InputInt(string text)
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
	string screenText;
	screenText += "\t" + text + "\n\t";
	cout << screenText;
	Vector2 input;
	cin >> input;
	return input;
}
