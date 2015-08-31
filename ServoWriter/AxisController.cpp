#include "AxisController.h"
#include <Servo.h>
#include <SoftwareSerial.h>

const int DEFAULT_PIN_NUM = 11;
const int DEFAULT_DIRECTION = 0;
const int DEFAULT_OUTMIN = 0;
const int DEFAULT_OUTMAX = 179;
const int DEFAULT_RX_PIN = 3;
const int DEFAULT_TX_PIN = 4;

AxisController::AxisController()
{
	Servo m_axisServo;
	m_pinNum = DEFAULT_PIN_NUM;
	m_direction = DEFAULT_DIRECTION;
	SoftwareSerial m_output(DEFAULT_RX_PIN, DEFAULT_TX_PIN);
	m_outMin = DEFAULT_OUTMIN;
	m_outMax = DEFAULT_OUTMAX;
	m_axisServo.attach(m_pinNum);
	calibrate();
}

AxisController::AxisController(Servo axisServo, int pinNum, 
							   int direction, SoftwareSerial output, 
							   int outMin=0, int outMax=179)
{
	m_axisServo = axisServo;
	m_pinNum = pinNum;
	m_direction = direction;
	SoftwareSerial m_output(DEFAULT_RX_PIN, DEFAULT_TX_PIN);
	m_outMin = outMin;
	m_outMax = outMax;
	m_axisServo.attach(m_pinNum);
	calibrate();
}

AxisController::~AxisController()
{
}

void AxisController::calibrate()
{
	m_output.println("Hold arm in position of max rotation (corresponding to a servo position of 180");

}

int AxisController::move_axis(int *myoDataArray)
{
	//myoDataArray is ALWAYS an array of length 10
	return 0;
}

void AxisController::set_outMin(int newOutMin)
{
	m_outMin = newOutMin;
}

void AxisController::set_outMax(int newOutMax)
{
	m_outMax = newOutMax;
}

int AxisController::get_outMin()
{
	return m_outMin;
}

int AxisController::get_outMax()
{
	return m_outMax;
}

int AxisController::get_pinNum()
{
	return m_pinNum;
}

int AxisController::get_direction()
{
	return m_direction;
}

int AxisController::get_inMin()
{
	return m_inMin;
}

int AxisController::get_inMax()
{
	return m_inMax;
}
