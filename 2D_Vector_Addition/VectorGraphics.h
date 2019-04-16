#pragma once

#include "Vector.h"
#include <SDL.h>

class VectorGraphics
{
public:
	VectorGraphics(int IN_WIDTH, int IN_HEIGHT);
	
	void display();

	//customizations
	void displayAxis(bool);
	void setBackgroundColor(int, int, int);
	void setAxisColor(int, int, int);
	void setAxis(int, int);
	void setScale(double, double);
	void displayGrid(bool);
	void setGridColor(int, int, int);

	//vector functions
	void addVector(Vector v);
	void addVector(double magnitude, double theta);
	void addVector(std::vector<double> &components);
	void clearVectors();
	void removeVector(int index);
	void printVectors();
	void setVectorColor(int ,int, int);
	void setResulatantVectorColor(int, int, int);
	void drawResultantVector(bool);

	//misc
	void wait(int millis);
	void wait();
	void draw();

	~VectorGraphics();

private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	const int WIDTH, HEIGHT;
	bool axis;
	bool grid;
	bool drawRv;

	int xr, xg, xb;//axis color
	int gr, gg, gb; //grid color
	int br, bg, bb;//background color
	int vr, vg, vb;//vector colors
	int Rvr, Rvg, Rvb;//resulatant vector color

	int centerx, centery;
	double pixelsPerUnit_x, pixelsPerUnit_y;
	std::vector<Vector> vectors;

	void createWindowAndRenderer();
	void destroyWindowAndRendrer();
	void  clearScreen();
	void drawLine(int, int, int, int);
	void drawLine(int, int, int, int, int);
	void pushBuffer();
	void waitForClose();
	void drawAxis();
	void setColor(int, int, int);
	bool escEvent(SDL_Event&);

	void drawVectors();
	void drawVector(Vector);
};

VectorGraphics::VectorGraphics(int IN_WIDTH, int IN_HEIGHT): WIDTH(IN_WIDTH), HEIGHT(IN_HEIGHT)
{
	drawRv = axis = grid = true; //draw axis by default
	gr = gg = gb = 100; //grid is grey by default
	br = bg = bb = 0; //black background default
	xr = xg = xb = 255; //default axis color is white
	vr = 255; //vector color is red
	vg = vb = 0;
	Rvg = 255; //resultant vector is green
	Rvr = Rvb = 0;
	pixelsPerUnit_x = pixelsPerUnit_y = 20; //10 pixels is 1

	//center for drawing axis
	centerx = WIDTH / 2;
	centery = HEIGHT / 2;

	//start video
	if (SDL_Init(SDL_INIT_VIDEO) == 0)//success
	{
		window = NULL;
		renderer = NULL;
	}
	else
	{
		std::cout << "Error: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
}

//(true) -> displays axis
//(false) -> does not display axis
inline void VectorGraphics::displayAxis(bool setting)
{
	axis = setting;
}

//sets background color using rgb
inline void VectorGraphics::setBackgroundColor(int r, int g, int b)
{
	br = r;
	bg = g;
	bb = b;
}

//sets axis color using rgb
inline void VectorGraphics::setAxisColor(int r, int g, int b)
{
	xr = r;
	xg = g;
	xb = b;
}

//sets where the axis starts, centered at (x, y)
inline void VectorGraphics::setAxis(int x, int y)
{
	centerx = x;
	centery = y;
}

//displays what is storred in the framebuffer
//ie. SDL_rendererDrawLine(0, 0, WIDTH, HEIGHT);
inline void VectorGraphics::display()
{
	createWindowAndRenderer();

	draw();
	waitForClose();

	destroyWindowAndRendrer();
}

//creates the window and renderer
inline void VectorGraphics::createWindowAndRenderer()
{
	//create window and renderer
	if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) != 0)
	{
		std::cout << "Error: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
}

//destroys the window and renderer
inline void VectorGraphics::destroyWindowAndRendrer()
{
	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
	if (window) {
		SDL_DestroyWindow(window);
	}
}

//fills the buffer with the current color on the renderer
inline void VectorGraphics::clearScreen()
{
	SDL_SetRenderDrawColor(renderer, br, bg, bb, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

//draws a 1px line
inline void VectorGraphics::drawLine(int start_x, int start_y, int end_x, int end_y)
{
	SDL_RenderDrawLine(renderer, start_x, start_y, end_x, end_y);
}

//draws thicker lines
inline void VectorGraphics::drawLine(int start_x, int start_y, int end_x, int end_y, int thickness)
{
	//point slope formula
	//useful for determining simple thickness direction spread
	//if |slope| <= .5, use y line spreading
	//if |slope| > .5, use x line spreading

	float slope;
	if (start_x - end_x == 0)
	{
		slope = 0;
	}
	else
	{
		slope = (start_y - end_y) / (start_x - end_x);
	}

	slope = abs(slope);

	if (slope <= .5)
	{
		//spreading the line accross the yaxis
		for (int i = start_y + (thickness / 2); i > start_y - (thickness / 2); i--)
		{
			SDL_RenderDrawLine(renderer, start_x, i, end_x, end_y + (i - start_y));
		}
	}
	else //slope > .5
	{
		//spreading the line accross the yaxis
		for (int i = start_x + (thickness / 2); i > start_x - (thickness / 2); i--)
		{
			SDL_RenderDrawLine(renderer, i, start_y, end_x + (i - start_x), end_y);
		}
	}

}

//pushes the buffer onto the screen
inline void VectorGraphics::pushBuffer()
{
	SDL_RenderPresent(renderer);
}

//waits for the screen to be closed
inline void VectorGraphics::waitForClose()
{
	SDL_Event event;
	bool done = false;

	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) 
			{
				done = true;
				break;
			}
		}
	}
}

//sets the scale for the axis and vectors
inline void VectorGraphics::setScale(double x_scale, double y_scale)
{
	pixelsPerUnit_x = x_scale;
	pixelsPerUnit_y = y_scale;
}

inline void VectorGraphics::displayGrid(bool b)
{
	grid = b;
}

inline void VectorGraphics::setGridColor(int r, int g, int b)
{
	gr = r;
	gg = g;
	gb = b;
}

inline void VectorGraphics::addVector(Vector v)
{
	vectors.push_back(v);
}

inline void VectorGraphics::addVector(double magnitude, double theta)
{
	vectors.push_back(Vector(magnitude, theta));
}

inline void VectorGraphics::addVector(std::vector<double>& components)
{
	vectors.push_back(Vector(components));
}

inline void VectorGraphics::clearVectors()
{
	vectors.clear();

	if (vectors.size() > 0)
	{
		vectors.resize(0);
	}
}

inline void VectorGraphics::removeVector(int index)
{
	if (index >= vectors.size())
	{
		return;
	}

	vectors.erase(vectors.begin() + index);
}

//draws the axis
inline void VectorGraphics::drawAxis()
{
	if (grid)
	{
		//draw scale/grid
		SDL_SetRenderDrawColor(renderer, gr, gg, gb, SDL_ALPHA_TRANSPARENT);
		for (int i = 0; i < WIDTH; i++)
		{
			if (fmod((i - centerx), pixelsPerUnit_x) == 0)
			{
				drawLine(i, 0, i, HEIGHT);
			}
		}

		for (int i = 0; i < HEIGHT; i++)
		{
			if (fmod((i - centery), pixelsPerUnit_y) == 0)
			{
				drawLine(0, i, WIDTH, i);
			}
		}
	}

	if (axis)
	{
		//set color of axis
		SDL_SetRenderDrawColor(renderer, xr, xg, xb, SDL_ALPHA_OPAQUE);

		//draw the 2 axis(1 line padded for thickness)
		drawLine(centerx + 1, 0, centerx + 1, HEIGHT);
		drawLine(centerx, 0, centerx, HEIGHT);				//x axis
		drawLine(centerx - 1, 0, centerx - 1, HEIGHT);

		drawLine(0, centery + 1, WIDTH, centery + 1);
		drawLine(0, centery, WIDTH, centery);				//y-axis
		drawLine(0, centery - 1, WIDTH, centery - 1);
	}
	
	
}

inline void VectorGraphics::setColor(int r, int g, int b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
}

inline bool VectorGraphics::escEvent(SDL_Event &event)
{
	SDL_PollEvent(&event);

	if (event.type == SDL_WINDOWEVENT_CLOSE)
	{
		destroyWindowAndRendrer();
		return true;
	}
	else if (event.type == SDL_WINDOWEVENT_FOCUS_LOST)
	{
		while (true)
		{
			SDL_PollEvent(&event);

			if (event.type == SDL_WINDOWEVENT_FOCUS_GAINED)
			{
				break;
			}
		}
		
	}

	return false;
}

//pushes all vectors to the buffer
inline void VectorGraphics::drawVectors()
{
	double Rv_x = 0, Rv_y = 0;

	setColor(vr, vg, vb);
	for (int i = 0; i < vectors.size(); i++)
	{
		drawVector(vectors[i]);
		Rv_x += vectors[i].getFx();
		Rv_y += vectors[i].getFy();
	}

	setColor(Rvr, Rvg, Rvb);
	if (drawRv)
	{
		drawVector(Vector(std::vector<double>{Rv_x, Rv_y}));
	}
}

//pushes a vector onto the buffer
inline void VectorGraphics::drawVector(Vector v)
{

	v = Vector(v.getMagnitude(), v.getTheta());
	double Fx_scaled = v.getFx() * pixelsPerUnit_x;
	double Fy_scaled = v.getFy() * pixelsPerUnit_y;

	double endx = centerx + Fx_scaled;
	double endy = centery + Fy_scaled;

	
	//flip across the x axis, since 
	/*
	actual creen renders like so:

	(0,0)--------->
	|		|
	|	q3	|	q4
	|		|
	|------(x, y)-->
	|		|
	|	q2	|	q1
	|		|
	v		v
	
	*/
	int q = v.getQ();
	
	
	switch (q)
	{
		case 1:
			endy = centery - Fy_scaled;
			break;
		case 2:
			endy = centery - Fy_scaled;
			break;
		case 3:
			endy = centery + abs(Fy_scaled);
			break;
		case 4:
			endy = centery + abs(Fy_scaled);
			break;
	default:
		break;
	}
	
	drawLine(centerx, centery, endx, endy, 5);
}

//prints out vectors contained on the object and their positions
inline void VectorGraphics::printVectors()
{
	std::cout << "\nVectors:\n\n";

	Vector resultantVector(0, 0);

	for (int i = 0; i < vectors.size(); i++)
	{
		resultantVector = resultantVector + vectors[i];
		std::cout << "Vector: [" << i << "]" << std::endl;
		std::cout << vectors[i] << std::endl;
	}

	if (drawRv == true)
	{
		std::cout << "Resultant Vector: \n" << resultantVector << std::endl;
	}
}

inline void VectorGraphics::setVectorColor(int r, int g, int b)
{
	vr = r;
	vg = g;
	vb = b;
}

inline void VectorGraphics::setResulatantVectorColor(int r, int g, int b)
{
	Rvg = r;
	Rvg = g;
	Rvb = b;
}

inline void VectorGraphics::drawResultantVector(bool b)
{
	drawRv = b;
}










//waits time specified in milliseconds
inline void VectorGraphics::wait(int millis)
{
	int start = SDL_GetTicks();
	int temp = start;

	while (start <= temp + millis)
	{
		start = SDL_GetTicks();
	}
}

inline void VectorGraphics::wait()
{
	SDL_Event keyPress;

	while (SDL_PollEvent(&keyPress))
	{
		if (keyPress.type == SDL_KEYDOWN)
		{
			break;
		}
	}

}

inline void VectorGraphics::draw()
{

	SDL_Event event;
	bool esc = escEvent(event);

	if (esc == true)
	{
		return;
	}

	if (window == NULL || renderer == NULL)
	{
		createWindowAndRenderer();
	}

	

	//draw background
	clearScreen();

	//draw axis
	if (axis == true)
	{
		drawAxis();
	}

	//draw vectors
	drawVectors();

	pushBuffer();
}











VectorGraphics::~VectorGraphics()
{
	destroyWindowAndRendrer();
	SDL_Quit();
}