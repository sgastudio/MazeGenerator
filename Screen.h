#pragma once
#include "vector2.h"
#include "maze.h"

#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <Windows.h>
#include <string>

using namespace std;

class Screen
{
protected:
	string m_msg;
	HANDLE hOut;
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
public:
	Screen();
	~Screen();

	void Menu();
	void PrintLogo();
	void PrintMainMenu();
	void PrintCredits();
	void PrintMazeInfo(Size2D, int, int, Maze::MAZE_SOLVABLE_E, bool);

	void Pause();
	char PauseEX(string);

	void Clear();

	void SetColor(string);
	void SetMsg(string);
	void SetMsgError(string);
	void SetMsgWarn(string);
	void SetMsgGood(string);
	void SetWndTitle(string);
	void SetWndSize(Vector2);
	void SetWndSize(int, int);
	void SetBufSize(Vector2);
	void SetBufSize(int, int);

	template <typename T>
	void Input(T*, string);
	template<typename T>
	void InputEX(T*, string);
};

template<typename T>
inline void Screen::Input(T* container, string text)
{
	string screenText;
	screenText += "\t" + text;
	cout << screenText;
	cin.clear();
	cin.sync();
	cin >> *container;
}

template<typename T>
inline void Screen::InputEX(T* container, string text)
{
	string screenText;
	screenText += "\t" + text;
	cout << screenText;
	*container = _getch();
}
