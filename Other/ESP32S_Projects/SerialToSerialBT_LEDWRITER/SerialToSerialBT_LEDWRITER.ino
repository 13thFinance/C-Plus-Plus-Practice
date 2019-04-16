#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

//analogMaker
////////////////////////////////////////////////////////////////////////////
#define min(a,b) ((a)<(b)?(a):(b))
// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0

// use 13 bit precission for LEDC timer
#define LEDC_TIMER_13_BIT  13

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000

// fade LED PIN (replace with LED_BUILTIN constant for built-in LED)
#define LED_PIN            18

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
////////////////////////////////////////////////////////////////////////////

//these are the pins I tested
//under nodemcu_32s pinout sheet, your board may differ
#define ANALOG_OUT 18 //<<<<<<<<<<<<<<<<<<<<<
#define DIGITAL_OUT 13 //<<<<<<<<<<<<<<<<<<<<
byte sigma = 0;
uint8_t command;

void setup() 
{
  //analog//////////////////////////////////////////////////////////////////
  // Setup timer and attach timer to a led pin
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL_0);
  //////////////////////////////////////////////////////////////////////////
  
  pinMode(DIGITAL_OUT, OUTPUT);
  
  Serial.begin(115200); //begins the serial terminal
  SerialBT.begin("[MAINLINE]"); //Bluetooth device name
  Serial.println("DEVICE STARTED SUCCESFULLY!\n");
}

void loop() 
{
  //computer >> esp32 >> phone
  if (Serial.available()) 
  {
    SerialBT.write(Serial.read());
  }

  //phone >> esp32 >> computer
  if (SerialBT.available()) 
  {
    command = SerialBT.read();

    if(command == 't')
    {
      analogTest();
    }
    
    if(command == 'd')
    {
      digitalTest();
    }
    
    Serial.write(command);
  }
  delay(20);
}

// Arduino like analogWrite
// value has to be between 0 and valueMax
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
}

void analogTest()
{
  for(int i = 0; i < 100; i++)
  {
     // set the brightness on LEDC channel 0
    ledcAnalogWrite(LEDC_CHANNEL_0, brightness);
  
    // change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;
  
    // reverse the direction of the fading at the ends of the fade:
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    // wait for 30 milliseconds to see the dimming effect
    delay(10);
  }
  
  Serial.println("ANALOG_TEST FINISHED");
}

void digitalTest()
{
  //blink 3 times
  for(int i = 0; i < 3; i++)
  {
    //blink pin
    digitalWrite(DIGITAL_OUT, HIGH);
    delay(100);
    digitalWrite(DIGITAL_OUT, LOW);
    delay(100);
  }
  
  Serial.println("DIGITAL_TEST FINISHED");
}
