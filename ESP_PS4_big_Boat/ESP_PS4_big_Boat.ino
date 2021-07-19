// Uses 2 MX1616 + 2 DC motors on a TinyPICO ESP32 board
// https://makerselectronics.com/product/mx1616-dual-motor-driver-board
// https://www.amazon.com/CANADUINO-H-Bridge-Motor-Driver-MX1616/dp/B07PGY9ZFL

// driver 1 --> motor 1
// driver 2 --> motor 2

#include <TinyPICO.h>         // can be ignored on other ESP32 boards
#include <PS4Controller.h>    // e.g. https://github.com/aed3/PS4-esp32


TinyPICO tp = TinyPICO();     // can be ignored on other ESP32 boards

bool right = 0;
bool left = 0;

bool vor = 0;

float speedR = 0;
float speedL = 0;

int speedRout = 0;
int speedLout = 0;

int in0 = 22;
int in1 = 21;
int in2 = 32;
int in3 = 33;

int in42 = 25;
int in52 = 26;
int in62 = 27;
int in72 = 15;


void setup()
{
  Serial.begin(115200);
  PS4.begin("dc:a2:66:97:fc:92");     // enter your PS4 Controller MAC address
  Serial.println("Ready.");
  
  ledcSetup(0, 20000, 8);
  ledcAttachPin(in0, 0);

  ledcSetup(1, 20000, 8);
  ledcAttachPin(in1, 1);

  ledcSetup(2, 20000, 8);
  ledcAttachPin(in2, 2);

  ledcSetup(3, 20000, 8);
  ledcAttachPin(in3, 3);


  ledcSetup(4, 20000, 8);
  ledcAttachPin(in42, 4);

  ledcSetup(5, 20000, 8);
  ledcAttachPin(in52, 5);

  ledcSetup(6, 20000, 8);
  ledcAttachPin(in62, 6);

  ledcSetup(7, 20000, 8);
  ledcAttachPin(in72, 7);
}

void loop()
{
  if (PS4.isConnected()) {
    tp.DotStar_CycleColor(25);      // can be ignored on other ESP32 boards

    if ( PS4.event.analog_move.stick.rx ) {
      if (PS4.data.analog.stick.ry > 5)
      {
        vor = true;
        speedR = map(PS4.data.analog.stick.ry, 0, 127, 0, 100);
        speedL = speedR;
      }
      if (PS4.data.analog.stick.ry < -5)
      {
        vor = false;
        speedR = map(PS4.data.analog.stick.ry, 0, -128, 0, 100);
        speedL = speedR;
      }

      Serial.print("speed: ");
      Serial.println(speedR, DEC);

      Serial.print("Stick R: ");
      Serial.println(PS4.data.analog.stick.ry, DEC);
    }

     if (speedR < 5 && speedR > -5){
      
      ledcWrite(0, 0);
      ledcWrite(1, 0);
      ledcWrite(2, 0);
      ledcWrite(3, 0);
      ledcWrite(4, 0);
      ledcWrite(5, 0);
      ledcWrite(6, 0);
      ledcWrite(7, 0);      
    }

    if ( PS4.event.analog_move.stick.lx ) {

      if (PS4.data.analog.stick.lx > 5)
      {
        speedR = speedR * (1-map(PS4.data.analog.stick.lx, 0, 127, 0, 95)*0.01);
      }

      if (PS4.data.analog.stick.lx < -5)
      {
        speedL = speedL * (1-map(PS4.data.analog.stick.lx, 0, -128, 0, 95)*0.01);
      }

      Serial.print("Stick L: ");
      Serial.println(PS4.data.analog.stick.lx, DEC);
    }

    if (PS4.data.status.charging)
      Serial.println("The controller is charging");
    if (PS4.data.status.audio)
      Serial.println("The controller has headphones attached");
    if (PS4.data.status.mic)
      Serial.println("The controller has a mic attached");

    Serial.print("Battey = ");
    Serial.print(PS4.data.status.battery, DEC);
    Serial.println(" / 16");

    Serial.println();

    if (PS4.isConnected()) {
      Serial.println("Controller connected");
    }

    speedRout=map(int(speedR),0,100,0,210);
    speedLout=map(int(speedL),0,100,0,210);

    if (vor)
    {
      ledcWrite(1, 0);
      ledcWrite(3, 0);
      ledcWrite(5, 0);
      ledcWrite(7, 0);
      
      ledcWrite(0, speedRout);
      ledcWrite(2, speedRout);
      ledcWrite(4, speedLout);
      ledcWrite(6, speedLout);      
      
    }
    else
    {
      ledcWrite(0, 0);
      ledcWrite(2, 0);
      ledcWrite(4, 0);
      ledcWrite(6, 0);
      
      ledcWrite(1, speedRout);
      ledcWrite(3, speedRout);
      ledcWrite(5, speedLout);
      ledcWrite(7, speedLout);      
    }

    speedR = 0;
    speedL = 0;

    delay(100);
  }
}
