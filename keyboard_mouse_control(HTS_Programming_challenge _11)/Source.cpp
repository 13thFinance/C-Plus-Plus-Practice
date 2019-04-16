//shifts a string's char values by a given number
//taken from a website using windows.h library
//to control mouse and keyboard input
//The program should be able to do this in under 3 seconds

#include <iostream>
#include <string.h>
#include <string>
#include <Windows.h>
#include <vector>
#include <ctype.h>
#include <stdlib.h>

using namespace std;

void toClipboard(const std::string &s);
INPUT letGoLeftClick();
INPUT holdLeftClick();
vector<INPUT> enter();
INPUT positionMouse(int x, int y);
void ctrl_c(vector<INPUT> &inputs);
void ctrl_v(vector<INPUT> &inputs);
void pasteAndEnter();

void getText();
void getShift();
int scaleX(int);
int scaleY(int);

int main()
{
	while (true)
	{
		system("pause");//allows setup of terminal to right hand side, website on left
		string s = "";
		int shift = 0;
		//send enter press
		vector<INPUT> v = enter();
		INPUT a[2] = { 0 };
		int index = 0;
		for (INPUT i : v)
		{
			a[index++] = i;
		}

		getText();
		cin >> s;

		SendInput(2, a, sizeof(INPUT));

		getShift();
		cin >> shift;
		//shift = shift % 26;
		shift = shift;
		SendInput(2, a, sizeof(INPUT));

		string numbers = "";
		string code = "";

		//fix this later/////////
		index = 0;
		for (char c : s)
		{
			if (isdigit(c))
			{
				numbers.push_back(c);
			}
			else
			{
				char c_ = (char)(atoi(numbers.c_str()) - shift);//default +shift is left shift
				code.push_back(c_);
				numbers = "";
			}
		}
		//////////////////////////////

		cout << code << endl;
		//Program is fast enough but is pasting garbage
		//must look into OR Paste and enter
		toClipboard(code.c_str());///////////////////////////////////////////////////////CHECK

		Sleep(20);

		pasteAndEnter();/////////////////////////////////////////////////////////////////check 

		system("pause");
	}
	return 0;
}

void getText()
{
	const int INPUTS = 17;
	INPUT in[INPUTS] = { 0 };

	//highlight text
	in[0] = positionMouse(560, 630);
	in[1] = holdLeftClick();
	in[2] = positionMouse(900, 630);
	in[3] = letGoLeftClick();

	//get ctrl + c INPUTS
	vector<INPUT> v;
	ctrl_c(v);

	int index = 4;
	for (INPUT i : v)
	{
		in[index++] = i;
	}

	in[index++] = positionMouse(1700, 400);//on program ready to paste
	v.clear();
	ctrl_v(v);

	in[index++] = holdLeftClick();
	in[index++] = letGoLeftClick();


	for (INPUT i : v)
	{
		in[index++] = i;
	}

	v = enter();

	for (INPUT i : v)
	{
		in[index++] = i;
	}

	//push buffer of inputs
	SendInput(INPUTS, in, sizeof(INPUT));
}

void getShift()
{
	const int INPUTS = 17;
	INPUT in[INPUTS] = { 0 };

	in[0] = positionMouse(480, 665);
	in[1] = holdLeftClick();
	in[2] = positionMouse(800, 665);
	in[3] = letGoLeftClick();

	vector<INPUT> v;
	ctrl_c(v);

	int index = 4;

	for (INPUT i : v)
	{
		in[index++] = i;
	}

	in[index++] = positionMouse(1700, 400);
	in[index++] = holdLeftClick();
	in[index++] = letGoLeftClick();

	v.clear();

	ctrl_v(v);
	for (INPUT i : v)
	{
		in[index++] = i;
	}

	v = enter();

	for (INPUT i : v)
	{
		in[index++] = i;
	}

	SendInput(INPUTS, in, sizeof(INPUT));
}

void pasteAndEnter()
{
	const int INPUTS = 10;//10
	INPUT in[INPUTS];

	in[0] = positionMouse(480, 750);
	in[1] = holdLeftClick();
	in[2] = letGoLeftClick();

	vector<INPUT> v;
	ctrl_v(v);

	int index = 3;
	for (INPUT i : v)
	{
		in[index++] = i;
	}

	in[index++] = positionMouse(600, 800);
	in[index++] = holdLeftClick();
	in[index++] = letGoLeftClick();

	SendInput(INPUTS, in, sizeof(INPUT));
}

//copies text to clipboard using Windows.h calls
void toClipboard(const std::string &s) {
	OpenClipboard(0);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size());
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size());
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

INPUT positionMouse(int x, int y)
{
	INPUT in;
	//position mouse
	in.type = INPUT_MOUSE;
	in.mi.dx = scaleX(x); // desired X coordinate
	in.mi.dy = scaleY(y); // desired Y coordinate
	in.mi.time = 0;
	in.mi.mouseData = 0;
	in.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

	return in;
}

INPUT holdLeftClick()
{
	INPUT in;
	//leftclick
	in.type = INPUT_MOUSE;
	in.mi.time = 0;
	in.mi.mouseData = 0;
	in.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	return in;
}

INPUT letGoLeftClick()
{
	INPUT in;
	//leftclick
	in.type = INPUT_MOUSE;
	in.mi.time = 0;
	in.mi.mouseData = 0;
	in.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	return in;
}

//returns windows inputs which click ctrl + c
void ctrl_c(vector<INPUT> &inputs)
{
	//press down ctrl
	INPUT in;
	in.type = INPUT_KEYBOARD;
	in.ki.wScan = 0;
	in.ki.time = 0;
	in.ki.wVk = VK_CONTROL;
	in.ki.dwFlags = 0;
	in.ki.time = 0;
	inputs.push_back(in);

	//press down v
	in.ki.wVk = 'C';
	in.ki.dwFlags = 0;
	inputs.push_back(in);

	Sleep(10);

	//release v key
	in.ki.wVk = 'C';
	in.ki.dwFlags = KEYEVENTF_KEYUP;
	inputs.push_back(in);

	//release ctrl key
	in.ki.wVk = VK_CONTROL;
	in.ki.dwFlags = KEYEVENTF_KEYUP;
	inputs.push_back(in);
}

//returns windows inputs which click ctrl + c
void ctrl_v(vector<INPUT> &inputs)
{
	//press down ctrl
	INPUT in;
	in.type = INPUT_KEYBOARD;
	in.ki.wScan = 0;
	in.ki.time = 0;
	in.ki.wVk = VK_CONTROL;
	in.ki.dwFlags = 0;
	in.ki.time = 0;
	inputs.push_back(in);

	//press down v
	in.ki.wVk = 'V';
	in.ki.dwFlags = 0;
	inputs.push_back(in);

	//release v key
	in.ki.wVk = 'V';
	in.ki.dwFlags = KEYEVENTF_KEYUP;
	inputs.push_back(in);

	//release ctrl key
	in.ki.wVk = VK_CONTROL;
	in.ki.dwFlags = KEYEVENTF_KEYUP;
	inputs.push_back(in);
}

vector<INPUT> enter()
{
	vector<INPUT> v;
	INPUT in;
	in.type = INPUT_KEYBOARD;
	in.ki.wScan = 0;
	in.ki.time = 0;
	in.ki.wVk = VK_RETURN;
	in.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
	
	v.push_back(in);

	in.ki.dwFlags = KEYEVENTF_KEYUP;
	v.push_back(in);
	return v;
}

//monitor is 1920x1080
//can use GetMonitorInfo() to scale
int scaleX(int pos)
{
	return 65535 * (pos / 1920.0);
}

int scaleY(int pos)
{
	return 65535 * (pos / 1080.0);
}

