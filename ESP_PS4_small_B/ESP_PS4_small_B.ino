#include <TinyPICO.h>
#include <PS4Controller.h>

TinyPICO tp = TinyPICO();

bool right = 0;
bool left = 0;

bool vor = 0;

float speedR = 0;
float speedL = 0;

int speedRout = 0;
int speedLout = 0;

int in0 = 18;
int in1 = 5;
int in2 = 32;
int in3 = 33;


void setup()
{
  Serial.begin(115200);
  PS4.begin("dc:a2:66:97:fc:92");
  Serial.println("Ready.");

  ledcAttachPin(in0, 0);
  ledcAttachPin(in1, 1);
  ledcAttachPin(in2, 2);
  ledcAttachPin(in3, 3);

  ledcSetup(0, 20000, 8);
  ledcSetup(1, 20000, 8);
  ledcSetup(2, 20000, 8);
  ledcSetup(3, 20000, 8);
}

void loop()
{
  // Below has all accessible outputs from the controller
  if (PS4.isConnected()) {
    tp.DotStar_CycleColor(25);


    speedR = 0;
    speedL = 0;

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

    if ( PS4.event.analog_move.stick.lx ) {

      if (PS4.data.analog.stick.lx > 5)
      {
        speedR = speedR * (1 - map(PS4.data.analog.stick.lx, 0, 127, 0, 95) * 0.01);
      }

      if (PS4.data.analog.stick.lx < -5)
      {
        speedL = speedL * (1 - map(PS4.data.analog.stick.lx, 0, -128, 0, 95) * 0.01);
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

    if (PS4.isConnected()) {
      Serial.println("Controller connected");
    }

    speedRout = map(int(speedR), 0, 100, 0, 200);
    speedLout = map(int(speedL), 0, 100, 0, 200);

    Serial.print("Speed R = ");
    Serial.println(speedRout, DEC);
    Serial.print("Speed L = ");
    Serial.println(speedLout, DEC);


    if (vor)
    {
      ledcWrite(0, speedRout);
      ledcWrite(1, 0);

      ledcWrite(2, speedLout);
      ledcWrite(3, 0);
    }
    else
    {
      ledcWrite(1, speedRout);
      ledcWrite(0, 0);

      ledcWrite(3, speedLout);
      ledcWrite(2, 0);
    }

    delay(100);
  }
}
