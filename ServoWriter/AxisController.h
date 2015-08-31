/*
AxisController.h - Library to control a single axis using a servo from 
Myo data. In theory other direction data could be used in place of a 
Myo, but Myo input was the original purpose of the library. 

Written by James Boggs on 30 August, 2015
MIT License
*/
#pragma once
class AxisController
{
public:
	AxisController();
	AxisController(Servo axisServo, int pinNum, int direction, SoftwareSerial output, int outMin = 0, int outMax = 179);
	~AxisController();
	int move_axis(int *myoDataArray);
	void set_outMin(int newOutMin);
	void set_outMax(int newOutMax);
	int get_outMin();
	int get_outMax();
	int get_pinNum();
	int get_direction();
	int get_inMin();
	int get_inMax();

private:
	Servo m_axisServo;
	int m_pinNum;
	int m_direction;
	SoftwareSerial m_output;
	int m_outMin;
	int m_outMax;
	int m_inMin;
	int m_inMax;
	void calibrate();
};