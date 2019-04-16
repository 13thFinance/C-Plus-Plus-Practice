//anima vesta 0.4

//screen for drawing
//[x][y+ buffer for tetronominos][logic level, pixel address]
//screen grows downward
#define y 12 
#define x 5
#define depth 3
#define logicLevel 0
#define addressLevel 1
#define setLevel 2

//tests joystick module
//joystick
#define xStickPin 43
#define yStickPin 44
#define inputClickPin 45

//defines the dead zones of the controller
#define range 37
#define middle 512
#define minX (middle - range)
#define maxX (middle + range)
#define minY (middle - range)
#define maxY (middle + range)

int renderSpeed = 400;
int globalTime = 0;
byte screen[x][y][depth];///////////////////////////////////////////SCREEN
bool tetrinomino[3][3];

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
byte pivot[] = {0, 0}; //pivot point for rotation
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//number are 0-1023, 512 being idle
struct Joystick
{
  byte xInput, yInput, clickInput;
};

Joystick joy;

void setup()
{
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);

  //call setupPins before setupJoystick
  //reason: setup pins sets all pins as outputs, setupJoystick
  //overrides that
  setupPins();
  setupJoystick();
  setupPixelAddress();
  setupSetLevel();
}

void loop() 
{
  game();
}

void pollInput()
{

  //the way read the array data matters, think about it
  if(analogRead(joy.xInput) > maxX)
  {
    if(checkRightSide())
    {
      return;//do nothing since we are at the side
    }
    //reads from the right side of the screen to the left
    //when at x,y if the pixel is off and the pixel to the left is on and not set
    //set pixel at x,y to on and the left to off
    for(int i = x - 1; i > 0; i--)
    {
      for(int j = 0; j < y; j++)
      {
        if(screen[i][j][setLevel] != true)//dont move it if it is already set, also if it is set it is already on
        {
          //check to left, if on move the pixel to the right
          if(screen[i - 1][j][logicLevel] == true && screen[i - 1][j][setLevel] == false)
          {
            screen[i][j][logicLevel] = true;
            screen[i - 1][j][logicLevel] = false;
          }
        }
      }
    }
  }
  else if(analogRead(joy.xInput) < minX)
  {
    //checks if there is a moveable pixels on the left side
    if(checkLeftSide())
    {
      return;//do nothing since we are at the side
    }
    
    //reads from the left side of the screen to the right
    for(int i = 0; i < x - 1; i++)
    {
      for(int j = 0; j < y; j++)
      {
        if(screen[i][j][setLevel] == false)
        {
          if(screen[i + 1][j][logicLevel] == true && screen[i + 1][j][setLevel] == false)
          {
            screen[i + 1][j][logicLevel] = false;
            screen[i][j][logicLevel] = true;
          }
        }
      }
    }
  }

  /*
  //we should only be able to move side to side
  if(analogRead(joy.yInput) > maxY)
  {
    //reads from the top of the screen going down
    for(int j = 0; j < y - 1; j++)
    {
      for(int i = 0; i < x; i++)
      {
        if(screen[i][j][setLevel] == false)
        {
          if(screen[i][j + 1][logicLevel] == true)
          {
            screen[i][j][logicLevel] = true;
            screen[i][j + 1][logicLevel] = false;
          }
        }
      }
    }
  }
  else if(analogRead(joy.yInput) < minY)
  {
    //read from the bottom of the screen going up
    for(int j = y - 1; j > 0; j--)
    {
      for(int i = 0; i < x; i++)
      {
        if(screen[i][j][setLevel] == false)
        {
          if(screen[i][j - 1][logicLevel] == true)
          {
            screen[i][j][logicLevel] = true;
            screen[i][j - 1][logicLevel] = false;
          }
        }
        
      }
    }
  }*/

  //makes it seem like the game is bringing the piece down faster
  //when in reality its just decreasing the wait time and letting the bringPixelsDown funsction
  //handle that
  if(analogRead(joy.yInput) < minY)
  {
    renderSpeed = 100;
  }
  else
  {
    renderSpeed = 500;
  }

  if(digitalRead(joy.clickInput) == LOW)///////////////////////////////////////////////////////////////////////////rotate the piece
  {
    for(int i = 0; i < x; i++)
    {
       for(int j = 0; j < y; j++)
       {
          screen[i][j][logicLevel] = false;
          screen[i][j][setLevel] = false;
       }
    }
  }
}

bool checkRightSide()
{
  int rightSideIndex = x - 1;
  for(int i = 0; i < y - 1; i++ )
  {
    //if it is a "moveable pixel"
    if(screen[rightSideIndex][i][logicLevel] == true && screen[rightSideIndex][i][setLevel] == false)
    {
      return true;
    }
  }

  return false;
}

bool checkLeftSide()
{
  int leftSideIndex = 0;
  for(int i = 0; i < y - 1; i++)
  {
    //if it is a "moveable pixel"
    if(screen[leftSideIndex][i][logicLevel] == true && screen[leftSideIndex][i][setLevel] == false)
    {
      return true;
    }
  }
  return false;
}

void joyMoveTester()
{
   if(globalTime == 1)
  {
    screen[0][6][logicLevel] = true;
  }

  pollInput();
  render(renderSpeed);
  globalTime++;
}

void game()
{
  //affect logic level
  tetris();//function to handle tetris game 

  //tests for game code conditions for debugging
  //test();
  
  //render scene
  render(renderSpeed);
  
  //clear buffer
  screenClear();

  //increments global time
  globalTime++;
}

//place code here to test game code
//turn on corner lights if a condition is met
void test()
{   
    screen[globalTime % x][0][logicLevel] = true;
}

//turns on all LEDS for debugging purposes
void allOn()
{
  for(int i = 0; i < 50; i++)
  {
    digitalWrite(i, HIGH);
  }
}

void tetris()
{
  //spawn only one at the beginning
  //spawnTetronomino();
  if(globalTime == 1)
  {
    spawnPixel();
  }
  if(globalTime == 12)
  {
    spawnTetronomino();
  }
  
  

  //gameplay loop
  dropPixels();
  pollInput();
}

//spawns a single pixel on the 'screen'
//for debugging purposes
void spawnPixel()
{
  screen[2][0][logicLevel] = true;
}

void spawnTetronomino()
{
  //L tetrinomino
  bool temp[3][3] = {{0,1,0},
                     {0,1,0},
                     {0,1,1}};

  //copies the L shaped tetrinomino into the global tetrinomino array
  for(int i = 0; i < 3; i++)
  {
    for(int j = 0; j < 3; j++)
    {
      tetrinomino[i][j] = temp[i][j];
    }
  }

  //spawns the tetrinomino into the buffer screen
  byte counterx = 1;
  byte countery = 0;
  
  for(int i = 0; i < 3; i++)
  {
    for(int j = 0; j < 3; j++)
    {
      screen[counterx][countery][logicLevel] = tetrinomino[i][j];
      counterx++;
    }
    countery++;
    counterx = 1;
  }
}

void dropPixels()
{
  
  //grab the pixels from the bottom going upwards
  //if you copy the pixels from the top you end up copying the 
  //first top row downward, which is usually empty, so it would just clear the screen

  //decrments the column accessor
  for(int j = y - 1; j >= 0; j--)
  {
    //brings down a whole row
    for(int i = x - 1; i >= 0; i--)
    {
      //renedering here eliminates "pulsing behavior I was experiencing"
      //render(0);
      
      //if the setlevel is not set on the pixel to true, bring the pixel down
      if(screen[i][j - 1][setLevel] == false)
      {
        screen[i][j][logicLevel] = screen[i][j - 1][logicLevel];
      }
    }
    
    checkCollision();
  }
}

//checks for collisions and calls a placeTetronominos function if it detects a collision
//this can be optimized
void checkCollision()
{
  //checks if the tetrinomino hit the ground level
  for(int i = 0; i < x; i++)
  {
    if(screen[i][y - 1][logicLevel] == true && screen[i][y - 1][setLevel] == false)
    {
      placeTetrinominos(); //collision == true
      return;
    }
  }
  
  //checks if the tetrinomino hit other things
  for(int i = 0; i < x; i++)
  {
    for(int j = 0; j < y - 1; j++)
    {
      //if the pixel below is 'set' and 'on'and the pixel it is evaluating is 'on' and not 'set'
      if((screen[i][j + 1][setLevel] == true && screen[i][j + 1][logicLevel] == true) && (screen[i][j][logicLevel] == true && screen[i][j][setLevel] == false))
      {
        placeTetrinominos();
        return;
      }
    }
  }
}

//locks all "on" pixels by setting their setLevel to 
//"True" so they are no longer affected by the dropPixels function
void placeTetrinominos()
{
  for(int i = 0; i < x; i++)
  {
    for(int j = 0; j < y; j++)
    {
      if(screen[i][j][logicLevel] == true)
      {
        screen[i][j][setLevel] = true;
      }
    }
  }
}

//sets all pins on the microcontroller to outputs
//will change once I find which pins I want as inputs
//for control
void setupPins()
{
  //setups all pins on teensy++  2.0 as OUTPUT
  for(int i = 0; i < 46; i++)
  {
    pinMode(i, OUTPUT);
  }
}

//sets up the pixel address
void setupPixelAddress()
{
  //sets address' and logic levels to zero
  for(int i =  0; i < x; i++)
  {
    for(int j = 0; j < y; j++)
    {
      for(int k = 0; k < depth; k++)
      {
        if(k == 0)
        {
          screen[i][j][logicLevel] = false;//have all lights be off
        }
        else
        {
          screen[i][j][addressLevel] = 255;//inaccessible pin
        }
      }
    }
  }
  
  //teensy++ 2.0 as of my wire connections
  //top row after buffer
  //row 4
   screen[0][4][addressLevel] = 41;
   screen[1][4][addressLevel] = 24;
   screen[2][4][addressLevel] = 16;
   screen[3][4][addressLevel] = 8;
   screen[4][4][addressLevel] = 0;

   //row 5
   screen[0][5][addressLevel] = 42;
   screen[1][5][addressLevel] = 25;
   screen[2][5][addressLevel] = 17;
   screen[3][5][addressLevel] = 9;
   screen[4][5][addressLevel] = 1;

   //row 6
   screen[0][6][addressLevel] = 33;//
   screen[1][6][addressLevel] = 26;
   screen[2][6][addressLevel] = 18;
   screen[3][6][addressLevel] = 10;
   screen[4][6][addressLevel] = 2;

   //row 7
   screen[0][7][addressLevel] = 34;//
   screen[1][7][addressLevel] = 32;//
   screen[2][7][addressLevel] = 19;
   screen[3][7][addressLevel] = 11;
   screen[4][7][addressLevel] = 3;

   //row 8
   screen[0][8][addressLevel] = 31;
   screen[1][8][addressLevel] = 27;
   screen[2][8][addressLevel] = 20;
   screen[3][8][addressLevel] = 12;
   screen[4][8][addressLevel] = 4;

   //row 9
   screen[0][9][addressLevel] = 30;
   screen[1][9][addressLevel] = 38;
   screen[2][9][addressLevel] = 21;
   screen[3][9][addressLevel] = 13;
   screen[4][9][addressLevel] = 5;

   //row 10
   screen[0][10][addressLevel] = 29;
   screen[1][10][addressLevel] = 39;
   screen[2][10][addressLevel] = 22;
   screen[3][10][addressLevel] = 14;
   screen[4][10][addressLevel] = 6;

   //row 11
   screen[0][11][addressLevel] = 28;
   screen[1][11][addressLevel] = 40;
   screen[2][11][addressLevel] = 23;
   screen[3][11][addressLevel] = 15;
   screen[4][11][addressLevel] = 7;
}

//set up the write protect
//this way we can check if an led has already been set.
void setupSetLevel()
{
  for(int i = 0; i < x; i++)
  {
    for(int j = 0; j < y; j++)
    {
      screen[i][j][setLevel] = false;
    }
  }
}

//draws the screen based on the lowers logic levels
//screen[x][y][logic level]
//at logic level 0, there will be values 0 and 1
//0 being off and 1 being on
//0 being false and 1 being true
//at logic level 1, the space will contain 
//the pixel address/pin address
void render(int renderspd)
{
  for(int i =  0; i < x; i++)
  {
    for(int j = 0; j < y; j++)
    {
      if(screen[i][j][logicLevel] == 0)
      {
        digitalWrite(screen[i][j][addressLevel], LOW);
      }
      else
      {
        digitalWrite(screen[i][j][addressLevel], HIGH);
      }
    }
  }

  delay(renderspd);
}

void screenClear()
{
  for(int i = 0; i < 50; i++)
  {
    digitalWrite(i, LOW);
  }
}

//sets up the joystick
void setupJoystick()
{
  /*
   * Pins:
      xStickPin
      yStickPin
      inputClickPin
   */
  
  //sets the analog pins to read
  pinMode(xStickPin, INPUT);
  pinMode(yStickPin, INPUT);
  pinMode(inputClickPin, INPUT);
  
  joy.xInput = xStickPin;
  joy.yInput = yStickPin;
  joy.clickInput = inputClickPin;
}



















