//Felix Ferreira
//9/18/2018
//generates all n character ascii passwords and stores them in a text document
//for 8 characters: 208827064576
//assume lowercase

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <sstream>

//arbitrary base counter
class counter
{
public:
	counter(int, std::vector<char>);
	void tick();
	std::string getString();
	void reset();
	bool getOverflowFlag();
	~counter();

private:
	std::vector<char> letters;
	int size;
	bool overflow;
	std::vector<char> string;
	int position;
	void tickR(int next); //recursive tick
};

//returns true if the counter has overflown
bool counter::getOverflowFlag()
{
	return overflow;
}

//param int s: number of "digits" for the counter
//std::vector<char> l: vector of valid characters in order used for counting
counter::counter(int s, std::vector<char> l)
{
	size = s;
	letters = l;
	position = 0;
	overflow = false;
	string.resize(size);

	reset();
}

//resets the counter
void counter::reset()
{
	for (int i = 0; i < string.size(); i++)//fill with 0 base
	{
		string[i] = letters[0];
	}
	overflow = false;
}

//increments the counter
void counter::tick()
{
	
	if (string[string.size() - 1] == letters[letters.size() - 1])//increment next place
	{
		string[string.size() - 1] = letters[0];//reset first counter
		tickR(string.size() - 2);
	}
	else if (string[string.size() - 1] == letters[0])
	{
		position = (position + 1) % (int)letters.size();
		string[string.size() - 1] = letters[1 % letters.size()];
	}
	else
	{
		position = (position + 1) % (int)letters.size();
		string[string.size() - 1] = letters[position];//ticks the last letter/number up
	}

	return;
}

//helper function to tick the counter
void counter::tickR(int next)
{
	if (next == -1)//overflow into 0, loop counter
	{
		reset();
		overflow = true;
		return;
	}

	if (string[next] == letters[letters.size() - 1])//increment next place
	{
		string[next] = letters[0];//reset this counter
		tickR(next - 1);
	}
	else
	{
		int rPosition = 0;
		for (char c : letters)
		{
			if (c == string[next])
			{
				break;
			}
			rPosition++;
		}
		string[next] = letters[++rPosition];//ticks the last letter/number up
	}
}

//gets the counter as a std::string
std::string counter::getString()
{
	std::string s;
	for (char c : string)
	{
		s.push_back(c);
	}

	return s;
}

counter::~counter()
{
}


std::string getString(std::vector<char> password);

int main()
{
	//make password space
	const int SIZE = 8;

	std::vector<char> password;

	for (int i = 0; i < SIZE; i++)
	{
		password.push_back('0');
	}

	//make a vector for our combinations
	std::vector<char> allLetters;

	//fill vector with numbers
	for (int i = 48; i < 58; i++)//0-9
	{
		allLetters.push_back(i);
	}

	//fill vector with lower case letters
	for (int i = 97; i <= 122; i++)//a-z
	{
		allLetters.push_back(i);
	}

	//open some file to write to
	std::ofstream fs;
	fs.open("all8lowercaseAlphanumeric.txt");
	if (!fs)
	{
		std::cout << "File could not be opened!\n";
		system("pause");
		exit(-1);
	}

	counter c(password.size(), allLetters);

	std::string end = "zzzzzzzz";
	while (c.getString() != end)//while the counter has not overflown
	{
		std::string temp = c.getString();
		fs << c.getString() << std::endl;
		c.tick();//increment counter
	}

	fs.close();
	system("pause");
	return 0;
}

//converts a std::vector<char> into a std::string
std::string getString(std::vector<char> password)
{
	std::string string;

	for (int i = 0; i < password.size(); i++)
	{
		string.push_back(password[i]);
	}

	return string;
}