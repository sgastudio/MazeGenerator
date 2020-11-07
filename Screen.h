#pragma once
#include "vector2.h"

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
	void Pause();
	char PauseEX(string displayText);
	void Clear();
	void SetColor(string colorText);
	void SetFont();
	void SetMsg(string text);
	void SetMsgError(string text);
	void SetMsgWarn(string text);
	void SetMsgGood(string text);
	void SetWndTitle(LPCWSTR text);
	void SetWndSize(Vector2 size);
	void SetWndSize(int width, int height);
	void SetBufSize(Vector2 size);
	void SetBufSize(int width, int height);
	template <typename T>
	void Input(T* container, string text);
	string InputString(string text);
	bool InputBool(string text);
	int InputInt(string text);
	Vector2 InputVector2(string text);
};

template<typename T>
inline void Screen::Input(T* container, string text)
{
	string screenText;
	screenText += "\t" + text + "\n\t";
	cout << screenText;
	cin >> *container;
}
