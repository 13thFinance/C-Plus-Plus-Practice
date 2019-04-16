/*
Programmer: Felix Ferreira
Date: 11/10/2018

OS: Windows 10 64bit
IDE: Visual Studio 2017 Community ver.
*/


//necessary includes for xbox 360 controller
#pragma comment(lib, "XInput.lib") //need this line so the linker knows what's up
#include <Windows.h> //we need this for xinput to work
#include <Xinput.h> //we need this to get controller states

//includes for output
#include <iostream> //for std::out
#include <string>
#include <chrono>//for delay
#include <thread>//for delay

#define DELAY_MS 100
#define MAX_CONTROLLERS 4
#define DEAD_ZONE 10000

//index definitions for players
#define PLAYER_ONE 0
#define PLAYER_TWO 1
#define PLAYER_THREE 2
#define PLAYER_FOUR 3

//rename to make more sense
typedef _XINPUT_STATE Controller;

#define BUTTONS_STRING_SIZE 16
const static std::string buttonStrings[] = { "[DPAD_UP]" , 
												"[DPAD_DOWN]", 
												"[DPAD_LEFT]", 
												"[DPAD_RIGHT]", 
												"[START]", 
												"[BACK]", 
												"[LEFT_THUMB]", 
												"[RIGHT_THUMB]", 
												"LEFT_SHOULDER", 
												"[RIGHT_SHOULDER]",
												"",//skip these two to match bitmask(makes programming later parts easier)(not used by controller)
												"",
												"[A]", 
												"[B]", 
												"[X]", 
												"[Y]"};

//returns state of all xbox 360 controllers
void getControllerStates(Controller[], bool[]);

//prints out the state of the controller to std::out
void printControllerState(Controller);

/**
	return a trigger value whose value has been checked to be outside the deadzone
	@param triggerValue raw trigger value read from controller using Xinput
	@return processed deadzone trigger value
*/
int processDeadZone(int triggerValue);

//delays program for a certain amount of time in milliseconds
void delay(int);

/*
	checks if a bit is enabled in a DWORD
	@param bits bits to be checked
	@param index index of the bit in bits from right to left
	@return true if a bit is stored inside of a DWORD
*/
bool isBitSet(DWORD bits, DWORD index);

int main()
{
	Controller controllers[MAX_CONTROLLERS];
	bool connectedControllers[MAX_CONTROLLERS];
	
	while (true)
	{
		//get the state of all controllers
		getControllerStates(controllers, connectedControllers);

		//print out state of all connected controllers
		for (int i = 0; i < MAX_CONTROLLERS; i++)
		{
			//if connected
			if (connectedControllers[i] == true)
			{
				std::cout << "Controller [" + std::to_string(i) + "]:\n";

				//print out info
				printControllerState(controllers[i]);
				std::cout << std::endl << std::endl;
			}	
			else //not connected
			{
				std::cout << "Controller [" + std::to_string(i) + "]:\n not connected...\n\n";
			}
		}

		//wait a little
		delay(DELAY_MS);
		system("cls");//clear output, I hear I shouldnt do this(?). Im already locked to windows though  ¯\_('-')_/¯
	}

	return 0;
}

/*
	gets the state of all controllers and updates the passed in controller array
	with corresponding values
*/
void getControllerStates(Controller controllers[], bool connectedControllers[])
{
	DWORD dwResult;

	//for number of controllers
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		Controller state;//creates a controller, may be filled with garbage, could just use controllers[] directly but not necessary
		ZeroMemory(&state, sizeof(Controller));//uses ZeroMemory to fill struct with all zeros

		//get state from Xinput
		dwResult = XInputGetState(i , &state);

		//dwResult lets us know if a contoller is connected or not
		if (dwResult == ERROR_SUCCESS)//controller is connected
		{
			controllers[i] = state;
			connectedControllers[i] = true;
		}
		else //controller is not connected
		{
			connectedControllers[i] = false;
			ZeroMemory(&controllers[i], sizeof(Controller));
		}
	}
}

/*
	prints out the state of a given controller
*/
void printControllerState(Controller c)
{
	/*
		this is what a Controller is:

		typedef struct _XINPUT_GAMEPAD {
		  WORD  wButtons;
		  BYTE  bLeftTrigger; //from 0-255
		  BYTE  bRightTrigger; //from 0-255
		  SHORT sThumbLX; //from -32768-32767 negative are down or to the left
		  SHORT sThumbLY; //from -32768-32767
		  SHORT sThumbRX; //from -32768-32767
		  SHORT sThumbRY; //from -32768-32767
		} XINPUT_GAMEPAD, *PXINPUT_GAMEPAD;
	*/

	/*
		for wButtons

		Bitmask of the device digital buttons, as follows. 
		A set bit indicates that the corresponding button is pressed.

										
		Device button					Bitmask						Binary		
		XINPUT_GAMEPAD_DPAD_UP----------0x0001 ->		                  1			
		XINPUT_GAMEPAD_DPAD_DOWN--------0x0002			                 10
		XINPUT_GAMEPAD_DPAD_LEFT--------0x0004			                100
		XINPUT_GAMEPAD_DPAD_RIGHT-------0x0008			               1000
		XINPUT_GAMEPAD_START------------0x0010			             1 0000
		XINPUT_GAMEPAD_BACK-------------0x0020			            10 0000
		XINPUT_GAMEPAD_LEFT_THUMB-------0x0040			           100 0000
		XINPUT_GAMEPAD_RIGHT_THUMB------0x0080			          1000 0000
		XINPUT_GAMEPAD_LEFT_SHOULDER----0x0100			        1 0000 0000
		XINPUT_GAMEPAD_RIGHT_SHOULDER---0x0200			       10 0000 0000
		XINPUT_GAMEPAD_A----------------0x1000			   1 0000 0000 0000
		XINPUT_GAMEPAD_B----------------0x2000			  10 0000 0000 0000
		XINPUT_GAMEPAD_X----------------0x4000			 100 0000 0000 0000
		XINPUT_GAMEPAD_Y----------------0x8000			1000 0000 0000 0000

		notice the small skip
	*/

	std::printf("%10s %10s %10s %10s %10s %10s\n", "L_TRIGGER"
													, "R_TRIGGER"
													, "LX_THUMB"
													, "LY_THUMB"
													, "RX_TRIGGER"
													, "RY_TRIGGER");
	std::printf("%10d %10d %10d %10d %10d %10d\n", c.Gamepad.bLeftTrigger
													, c.Gamepad.bRightTrigger
													, processDeadZone(c.Gamepad.sThumbLX)
													, processDeadZone(c.Gamepad.sThumbLY)
													, processDeadZone(c.Gamepad.sThumbRX)
													, processDeadZone(c.Gamepad.sThumbRY));
	//print out wButton
	DWORD bits = c.Gamepad.wButtons;
	bool isSet[BUTTONS_STRING_SIZE]; //matches buttonsString array

	for (int i = 0; i < BUTTONS_STRING_SIZE; i++)
	{
		isSet[i] = isBitSet(bits, i);
	}

	//print out buttons that are set, could do it on above loop but may use later as condition checking?, may come back and optimize
	for (int i = 0; i < BUTTONS_STRING_SIZE; i++)
	{
		if (isSet[i] == true)
		{
			std::cout << buttonStrings[i];
		}
	}

	std::cout << std::endl;
}

//delays program for a certain amount of time
void delay(int delayMillis)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis));
}

/**
	return a trigger value whose value has been checked to be outside the deadzone
	@param triggerValue raw trigger value read from controller using Xinput
	@return processed deadzone trigger value
*/
int processDeadZone(int triggerValue)
{
	if (triggerValue >= 0)
	{
		return (triggerValue >= DEAD_ZONE ? triggerValue : 0);
	}
	else
	{
		return (triggerValue <= (DEAD_ZONE * -1) ? triggerValue : 0);
	}

}

/*
	checks if a bit is enabled in a DWORD
	@param bits bits to be checked
	@param index index of the bit in bits from right to left
	@return true if a bit is stored inside of a DWORD
*/
bool isBitSet(DWORD bits, DWORD index)
{
	//roll the bits to the right according to the index we are looking for
	//if there is a 1 bit at rightmost index the number is not divisible by 4
	//we can use this as a quick check

	DWORD shiftedBits = bits >> index;

	if (shiftedBits % 2 == 0)
	{
		return false;
	}

	return true;
}