#include <Servo.h>

Servo horizServo;
//Servo vertServo;

const int HORIZ_SERVO_PIN = 5;
////const int VERT_SERVO_PIN = 6;
int limits[2] = { 5, 175 };
int horiz_pos;
int vert_pos;

void setup()
{
	Serial.begin(9600);
	horizServo.attach(HORIZ_SERVO_PIN);
	//vertServo.attach(VERT_SERVO_PIN);
	Serial.print("SERVOS ATTACHED!");
	horizServo.write(90);
	//vertServo.write(90);
}

void loop()
{
	if (Serial.available())
	{
		int horiz_pos = Serial.read();
		//delay(1);
		//vert_pos = Serial.read();

		horizServo.write(horiz_pos);
		//vertServo.write(vert_pos);

		Serial.print("INPUT: ");
		Serial.println(horiz_pos);
		Serial.print("SERVO POS: ");
		Serial.println(horizServo.read());
	}
}
