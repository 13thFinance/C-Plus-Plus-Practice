
//clock speeds for power saving <=
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00
#define CPU_8MHz        0x01
#define CPU_4MHz        0x02
#define CPU_2MHz        0x03
#define CPU_1MHz        0x04
#define CPU_500kHz      0x05
#define CPU_250kHz      0x06
#define CPU_125kHz      0x07
#define CPU_62kHz       0x08

//represents a 7segment display
struct Segment {
  int pinA, 
      pinB, 
      pinC, 
      pinD, 
      pinE, 
      pinF, 
      pinG;
};

//dont use LED pin 6
#define MAX_SCROLL_LINE 1000
#define NUMBER_OF_SEGMENTS 4
#define spd 7
#define halfSpd spd * 2
#define LED 6

//Pulse wave modulation on pin 24
#define readPin 24;
#define CLOCKSPEED1MhZ_5v 194999800
//62
Segment seg1 = {0, 1, 2, 3, 4, 5, 7};//first segment
Segment seg2 = {8, 9, 10, 11, 12, 13, 14};//second segment
Segment seg3 = {15, 16, 17, 45, 44, 43, 42};//third segmet
Segment seg4 = {41, 40, 39, 38, 18, 19, 20};//fourth segment
//fifth?
//Segment seg5 = {6, 21, 22, 23, 24, 25, 26};

Segment segments[] = {seg1, seg2, seg3, seg4}; //segment array for four segments

int Time[] = {0, 0, 0, 0};

void setup() 
{
  CPU_PRESCALE(CPU_500kHz);//125khz
  
  //sets up segments for output
  setupSegments(segments);

  //blinks LED to show we started up
  blinkLED();
  delay(5);
}

void loop() 
{
  blinkText(segments, "hapy", 1, 2);
  blinkText(segments, "neWw", 1, 2);
  blinkText(segments, "year", 1, 2);
  
  for(int i = 0; i < 3; i++)
  {
    scrollLine(segments, "    legoland   ", spd + 1);
    spin(segments, 1, 3);
    blinkText(segments, "lego", 1, 3);
  }
}

void segmentScroller(Segment segments[], int repetitions, int delayTime)
{
  clearSegments(segments);
  
  for(int j = 0; j < repetitions; j++)
  {
    for(int i = 0; i < NUMBER_OF_SEGMENTS; i++)
    {
      printSegment(segments[i], '8');
      delay(delayTime);
      //clearSegment(segments[i]);
    }
    clearSegments(segments);
  }
}

//prints out numbers to segments 
//if starting position is 0 itll print starting on the first segment 
//uses printSegment function so uses ASCII code for number, 48-57 0-1 respectively
void printSegmentNumber(Segment segments[] ,int startingPosition ,int number[] ,int NUMBER_SIZE)
{
  //clears segments
  clearSegments(segments);
  
  for(int i = startingPosition; i < NUMBER_SIZE; i++)
  {
    if(number[i] == 1)
    {
      printSegment(segments[i], '1');
    }
    else if(number[i] == 2)
    {
      printSegment(segments[i], '2');
    }
    else if(number[i] == 3)
    {
      printSegment(segments[i], '3');
    }
    else if(number[i] == 4)
    {
      printSegment(segments[i], '4');
    }
    else if(number[i] == 5)
    {
      printSegment(segments[i], '5');
    }
    else if(number[i] == 6)
    {
      printSegment(segments[i], '6');
    }
    else if(number[i] == 7)
    {
      printSegment(segments[i], '7');
    }
    else if(number[i] == 8)
    {
      printSegment(segments[i], '8');
    }
    else if(number[i] == 9)
    {
      printSegment(segments[i], '9');
    }
    else//prints 0
    {
      printSegment(segments[i], '0');
    }
  }

  
}

//sets up an array or 7segm
void setupSegments(Segment segments[])
{
  for(int i = 0; i < NUMBER_OF_SEGMENTS; i++)
  {
    setupSegment(segments[i]);
  }
}

//blinks some text
void blinkText(Segment segments[], char text[], int delayTime, int blinks)
{
  for(int i = 0; i < blinks; i++)
  {
    clearSegments(segments);
    delay(spd);
    printSegmentLine(segments, text, delayTime);
    delay(halfSpd);
  }
  
}

//use only with 4 segments edit otherwise to use more or less I got lazy
void spin(Segment segments[], int delayTime, int spins)
{
  clearSegments(segments);

  for(int j = 0; j < spins; j++)
  {
    for(int i = 0; i < NUMBER_OF_SEGMENTS * 2 + 4; i++)
    {
      if(i <= 3)
      {
        digitalWrite(segments[i].pinA, HIGH);
        digitalWrite(segments[i].pinB, LOW);
        digitalWrite(segments[i].pinC, LOW);
        digitalWrite(segments[i].pinD, LOW);
        digitalWrite(segments[i].pinE, LOW);
        digitalWrite(segments[i].pinF, LOW);
        digitalWrite(segments[i].pinG, LOW);
      }
      else if(i > 3 && i <= 5)
      {
        if(i == 4)
        {
          digitalWrite(segments[i - 1].pinA, LOW);
          digitalWrite(segments[i - 1].pinB, HIGH);
          digitalWrite(segments[i - 1].pinC, LOW);
          digitalWrite(segments[i - 1].pinD, LOW);
          digitalWrite(segments[i - 1].pinE, LOW);
          digitalWrite(segments[i - 1].pinF, LOW);
          digitalWrite(segments[i - 1].pinG, LOW);
        }
        else
        {
          digitalWrite(segments[i - 2].pinA, LOW);
          digitalWrite(segments[i - 2].pinB, LOW);
          digitalWrite(segments[i - 2].pinC, HIGH);
          digitalWrite(segments[i - 2].pinD, LOW);
          digitalWrite(segments[i - 2].pinE, LOW);
          digitalWrite(segments[i - 2].pinF, LOW);
          digitalWrite(segments[i - 2].pinG, LOW);
        }
      }
      else if(i > 5 && i <= 9)
      {
        if(i == 6)
        {
          digitalWrite(segments[i - 3].pinA, LOW);
          digitalWrite(segments[i - 3].pinB, LOW);
          digitalWrite(segments[i - 3].pinC, LOW);
          digitalWrite(segments[i - 3].pinD, HIGH);
          digitalWrite(segments[i - 3].pinE, LOW);
          digitalWrite(segments[i - 3].pinF, LOW);
          digitalWrite(segments[i - 3].pinG, LOW);
          
        }
        else if(i == 7)
        {
          digitalWrite(segments[i - 5].pinA, LOW);
          digitalWrite(segments[i - 5].pinB, LOW);
          digitalWrite(segments[i - 5].pinC, LOW);
          digitalWrite(segments[i - 5].pinD, HIGH);
          digitalWrite(segments[i - 5].pinE, LOW);
          digitalWrite(segments[i - 5].pinF, LOW);
          digitalWrite(segments[i - 5].pinG, LOW);
          
        }
        else if(i == 8)
        {
          digitalWrite(segments[i - 7].pinA, LOW);
          digitalWrite(segments[i - 7].pinB, LOW);
          digitalWrite(segments[i - 7].pinC, LOW);
          digitalWrite(segments[i - 7].pinD, HIGH);
          digitalWrite(segments[i - 7].pinE, LOW);
          digitalWrite(segments[i - 7].pinF, LOW);
          digitalWrite(segments[i - 7].pinG, LOW);
          
        }
        else//9
        {
          digitalWrite(segments[i - 9].pinA, LOW);
          digitalWrite(segments[i - 9].pinB, LOW);
          digitalWrite(segments[i - 9].pinC, LOW);
          digitalWrite(segments[i - 9].pinD, HIGH);
          digitalWrite(segments[i - 9].pinE, LOW);
          digitalWrite(segments[i - 9].pinF, LOW);
          digitalWrite(segments[i - 9].pinG, LOW);
        }
      }
      else
      {
        if(i == 10)
        {
          digitalWrite(segments[i - 10].pinA, LOW);
          digitalWrite(segments[i - 10].pinB, LOW);
          digitalWrite(segments[i - 10].pinC, LOW);
          digitalWrite(segments[i - 10].pinD, LOW);
          digitalWrite(segments[i - 10].pinE, HIGH);
          digitalWrite(segments[i - 10].pinF, LOW);
          digitalWrite(segments[i - 10].pinG, LOW);
        }
        else//11
        {
          digitalWrite(segments[i - 11].pinA, LOW);
          digitalWrite(segments[i - 11].pinB, LOW);
          digitalWrite(segments[i - 11].pinC, LOW);
          digitalWrite(segments[i - 11].pinD, LOW);
          digitalWrite(segments[i - 11].pinE, LOW);
          digitalWrite(segments[i - 11].pinF, HIGH);
          digitalWrite(segments[i - 11].pinG, LOW);
        }
      }
  
      
      delay(delayTime);
      clearSegments(segments);
    }
  }
  
}

//blinks an led
void blinkLED()
{
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);//internal LED to show we started up
  delay((int)spd / 2);
  digitalWrite(LED, LOW);
  delay((int)spd / 2);
}

//turns off segments
void clearSegments(Segment segment[])
{
  for(int i = 0; i < NUMBER_OF_SEGMENTS; i++)
  {
    clearSegment(segment[i]);
  }
}

//scrolls a line of text
void scrollLine(Segment segments[], char string[], int delayTime)
{
  char tempString[MAX_SCROLL_LINE];
  int numberOfSegments = NUMBER_OF_SEGMENTS;
  int startIndex = 0;
  char stringBuffer[numberOfSegments + 1];//+1 for the NULL char '\n'
  int stringBufferSize = sizeof(stringBuffer) - 1;
  
  strcpy(tempString, string);//copy the string in
  strcat(tempString, "  ");

  int count;

  //does it until the whole string is finished being put out
  while(startIndex < strlen(string) )
  {
    for(count = 0; count < stringBufferSize; count++)
    {
      stringBuffer[count] = tempString[startIndex++];
    }
    startIndex = startIndex - (NUMBER_OF_SEGMENTS - 1);
    
    stringBuffer[count] = '\0';//terminates that fucking string

    if(stringBuffer[count - 1] == '\0')
    {
      continue;
    }
    
    printSegmentLine(segments, stringBuffer, delayTime);
  }
}

//prints a line of of text that is equal to or less than the segments available
//otherwise calls the scrollLine function
void printSegmentLine(Segment segment[], char string[], int delayTime)
{
  int segmentAddressMax = NUMBER_OF_SEGMENTS;
  int segmentAddress = 0;
  
  int stringSize = strlen(string);
  int letter = 0;//first letter

  if(stringSize > segmentAddressMax)
  {
    scrollLine(segment, string, delayTime);
    return;
  }
   
  while(stringSize > 0)
  {
    printSegment(segment[segmentAddress++], string[letter++]);
    
    stringSize--;
  }
  
   delay(delayTime);
}

//sets up a segment for output
void setupSegment(Segment segment)
{
  pinMode(segment.pinA, OUTPUT);
  pinMode(segment.pinB, OUTPUT);
  pinMode(segment.pinC, OUTPUT);
  pinMode(segment.pinD, OUTPUT);
  pinMode(segment.pinE, OUTPUT);
  pinMode(segment.pinF, OUTPUT);
  pinMode(segment.pinG, OUTPUT);
  
}

//prints a letter to a segment
void printSegment(Segment segment, char letter)
{
  //clears the current segment
  clearSegment(segment);

  if(letter == 'A' || letter == 'a')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, LOW);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'B' || letter == 'b')
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, LOW);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'C' || letter == 'c')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, LOW);
    digitalWrite(segment.pinC, LOW);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, LOW);
  }
  else if(letter == 'D' || letter == 'd')
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, LOW);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'E' || letter == 'e')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, LOW);
    digitalWrite(segment.pinC, LOW);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'F' || letter == 'f')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, LOW);
    digitalWrite(segment.pinC, LOW);
    digitalWrite(segment.pinD, LOW);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'g')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, LOW);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'G')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, LOW);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'H' || letter == 'h')
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, LOW);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'I' || letter == 'i')
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, LOW);
    digitalWrite(segment.pinE, LOW);
    digitalWrite(segment.pinF, LOW);
    digitalWrite(segment.pinG, LOW);
  }
  else if(letter == 'K' || letter == 'k')
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, LOW);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'J' || letter == 'j')
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, LOW);
    digitalWrite(segment.pinG, LOW);
  }
  else if(letter == 'L' || letter == 'l')
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, LOW);
    digitalWrite(segment.pinC, LOW);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, LOW);
  }
  else if(letter == 'M' || letter == 'm')
  {
    if(letter == 'M')
    {
      digitalWrite(segment.pinA, LOW);
      digitalWrite(segment.pinB, LOW);
      digitalWrite(segment.pinC, HIGH);
      digitalWrite(segment.pinD, LOW);
      digitalWrite(segment.pinE, HIGH);
      digitalWrite(segment.pinF, HIGH);
      digitalWrite(segment.pinG, HIGH);
    }
    else
    {
      digitalWrite(segment.pinA, LOW);
      digitalWrite(segment.pinB, LOW);
      digitalWrite(segment.pinC, HIGH);
      digitalWrite(segment.pinD, LOW);
      digitalWrite(segment.pinE, LOW);
      digitalWrite(segment.pinF, LOW);
      digitalWrite(segment.pinG, HIGH);
    }

  }
  else if(letter == 'N' || letter == 'n')
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, LOW);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, LOW);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, LOW);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'O' || letter == 'o')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, LOW);
  }
  else if(letter == 'P' || letter == 'p')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, LOW);
    digitalWrite(segment.pinD, LOW);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'Q' || letter == 'q')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, LOW);
    digitalWrite(segment.pinE, LOW);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'R' || letter == 'r')
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, LOW);
    digitalWrite(segment.pinC, LOW);
    digitalWrite(segment.pinD, LOW);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, LOW);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'S' || letter == 's')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, LOW);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, LOW);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'T' || letter == 't')
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, LOW);
    digitalWrite(segment.pinC, LOW);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'U' || letter == 'u')
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, LOW);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, LOW);
    digitalWrite(segment.pinG, LOW);
  }
  else if(letter == 'V' || letter == 'v')
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, LOW);
  }
  else if(letter == 'W' || letter == 'w')
  {
    if(letter == 'W')
    {
      digitalWrite(segment.pinA, LOW);
      digitalWrite(segment.pinB, LOW);
      digitalWrite(segment.pinC, HIGH);
      digitalWrite(segment.pinD, HIGH);
      digitalWrite(segment.pinE, HIGH);
      digitalWrite(segment.pinF, HIGH);
      digitalWrite(segment.pinG, LOW);
    }
    else
    {
      digitalWrite(segment.pinA, LOW);
      digitalWrite(segment.pinB, HIGH);
      digitalWrite(segment.pinC, HIGH);
      digitalWrite(segment.pinD, HIGH);
      digitalWrite(segment.pinE, HIGH);
      digitalWrite(segment.pinF, LOW);
      digitalWrite(segment.pinG, LOW);
    }
  }
  else if(letter == 'X' || letter == 'x')
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, LOW);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'Y' || letter == 'y')
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, LOW);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == 'Z' || letter == 'z')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, LOW);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, LOW);
    digitalWrite(segment.pinG, HIGH);
  }
//numbers
  else if(letter == '0')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, LOW);
  }
  else if(letter == '1')
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, LOW);
    digitalWrite(segment.pinE, LOW);
    digitalWrite(segment.pinF, LOW);
    digitalWrite(segment.pinG, LOW);
  }
  else if(letter == '2')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, LOW);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, LOW);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == '3')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, LOW);
    digitalWrite(segment.pinF, LOW);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == '4')
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, LOW);
    digitalWrite(segment.pinE, LOW);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == '5')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, LOW);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, LOW);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == '6')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, LOW);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == '7')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, LOW);
    digitalWrite(segment.pinE, LOW);
    digitalWrite(segment.pinF, LOW);
    digitalWrite(segment.pinG, LOW);
  }
  else if(letter == '8')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, HIGH);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else if(letter == '9')
  {
    digitalWrite(segment.pinA, HIGH);
    digitalWrite(segment.pinB, HIGH);
    digitalWrite(segment.pinC, HIGH);
    digitalWrite(segment.pinD, HIGH);
    digitalWrite(segment.pinE, LOW);
    digitalWrite(segment.pinF, HIGH);
    digitalWrite(segment.pinG, HIGH);
  }
  else
  {
    digitalWrite(segment.pinA, LOW);
    digitalWrite(segment.pinB, LOW);
    digitalWrite(segment.pinC, LOW);
    digitalWrite(segment.pinD, LOW);
    digitalWrite(segment.pinE, LOW);
    digitalWrite(segment.pinF, LOW);
    digitalWrite(segment.pinG, LOW);
  }
  
}
//clears a segment 
void clearSegment(Segment segment)
{
  digitalWrite(segment.pinA, LOW);
  digitalWrite(segment.pinB, LOW);
  digitalWrite(segment.pinC, LOW);
  digitalWrite(segment.pinD, LOW);
  digitalWrite(segment.pinE, LOW);
  digitalWrite(segment.pinF, LOW);
  digitalWrite(segment.pinG, LOW);
}
