#include "AxisController.h"
#include <Servo.h>
#include <SoftwareSerial.h>

const int DEFAULT_PIN_NUM = 11;
const int DEFAULT_DIRECTION = 0;
const int DEFAULT_OUTMIN = 0;
const int DEFAULT_OUTMAX = 179;
const int DEFAULT_RX_PIN = 3;
const int DEFAULT_TX_PIN = 4;
const int MAX_SAMPLES = 1000;
enum DIRECTION
{
	DIR_ORIENT_X,
	DIR_ORIENT_Y,
	DIR_ORIENT_Z,
	DIR_ORIENT_W,
	DIR_ACCEL_X,
	DIR_ACCEL_Y,
	DIR_ACCEL_Z,
	DIR_GYRO_X,
	DIR_GYRO_Y,
	DIR_GYRO_Z
};

const bool DEBUG = true;

AxisController::AxisController()
{

	Servo m_axisServo;
	m_pinNum = DEFAULT_PIN_NUM;
	SoftwareSerial m_output(DEFAULT_RX_PIN, DEFAULT_TX_PIN);
	m_outMin = DEFAULT_OUTMIN;
	m_outMax = DEFAULT_OUTMAX;
	m_axisServo.attach(m_pinNum);
	m_output.begin(9600);
	calibrate(MAX_SAMPLES);

}

AxisController::AxisController(Servo axisServo, int pinNum, 
							   SoftwareSerial output, int outMin=0, int outMax=179)
{

	m_axisServo = axisServo;
	m_pinNum = pinNum;
	SoftwareSerial m_output(DEFAULT_RX_PIN, DEFAULT_TX_PIN);
	m_outMin = outMin;
	m_outMax = outMax;
	m_axisServo.attach(m_pinNum);
	m_output.begin(9600);
	calibrate(MAX_SAMPLES);

}

AxisController::~AxisController()
{
}

void AxisController::calibrate(int maxSamples)
{
	/** Calibrates the axis, which means assigning it a direction and 
	finding the maximum and minimum values the myo will send to the Arduino.
	It does this by having the user hold their arm at the peak of their
	expected range of movements and collecting the average readings from each
	of the 10 sensors on the myo. Then it repeats the process at for the 
	opposite direction, and determines the differences between the max and 
	min	readings to determine which direction the axis should listen for.
	*/

	// create local arrays to hold data
	double data_maxes[10] = { 0.0 };
	double data_mins[10] = { 0.0 };
	double data_diffs[10] = { 0.0 };
	
	// gather readings at maximum position
	m_output.println("Hold arm in position of max rotation (corresponding to a servo position of 179");
	get_calibration_reading(maxSamples, data_maxes);

	// gather readings at minimum position
	m_output.println("Hold arm in position of min rotation (corresponding to a servo position of 0");
	get_calibration_reading(maxSamples, data_mins);

	// calculate differences
	for (int direction = DIR_ORIENT_X; direction <= DIR_GYRO_Z; direction++)
	{
		data_diffs[direction] = data_maxes[direction] - data_mins[direction];
	}

	// determine which direction the axis should listen to
	int max_diff = 0;
	int axis_direction;
	for (int direction = DIR_ORIENT_X; direction <= DIR_GYRO_Z; direction++)
	{
		if (data_diffs[direction] > max_diff)
		{
			max_diff = data_diffs[direction];
			axis_direction = direction;
		}
	}

	m_direction = axis_direction;
	m_inMax = data_maxes[axis_direction];
	m_inMin = data_mins[axis_direction];
}

void AxisController::get_calibration_reading(int maxSamples, double *data_avgs)
{
	// declare local vars to hold Myo output
	double *orient_x = new double[maxSamples];
	double *orient_y = new double[maxSamples];
	double *orient_z = new double[maxSamples];
	double *orient_w = new double[maxSamples];

	double *accel_x = new double[maxSamples];
	double *accel_y = new double[maxSamples];
	double *accel_z = new double[maxSamples];

	double *gyro_x = new double[maxSamples];
	double *gyro_y = new double[maxSamples];
	double *gyro_z = new double[maxSamples];

	// declare local var to count # of samples taken
	int sampleCount = 0;
	while (Serial.available() > 1 && sampleCount <= maxSamples)
	{
		orient_x[sampleCount] = Serial.parseFloat();
		orient_y[sampleCount] = Serial.parseFloat();
		orient_z[sampleCount] = Serial.parseFloat();
		orient_w[sampleCount] = Serial.parseFloat();

		accel_x[sampleCount] = Serial.parseFloat();
		accel_y[sampleCount] = Serial.parseFloat();
		accel_z[sampleCount] = Serial.parseFloat();

		gyro_x[sampleCount] = Serial.parseFloat();
		gyro_y[sampleCount] = Serial.parseFloat();
		gyro_z[sampleCount] = Serial.parseFloat();

		m_output.println("RECEIVED MYO DATA");
		if (DEBUG)
		{
			m_output.print("orient_x = ");
			m_output.println(orient_x[sampleCount]);
			m_output.print("orient_y = ");
			m_output.println(orient_y[sampleCount]);
			m_output.print("orient_z = ");
			m_output.println(orient_z[sampleCount]);
			m_output.print("orient_w = ");
			m_output.println(orient_w[sampleCount]);
			m_output.print("accel_x = ");
			m_output.println(accel_x[sampleCount]);
			m_output.print("accel_y = ");
			m_output.println(accel_y[sampleCount]);
			m_output.print("accel_z = ");
			m_output.println(accel_z[sampleCount]);
			m_output.print("gyro_x = ");
			m_output.println(gyro_x[sampleCount]);
			m_output.print("gyro_y = ");
			m_output.println(gyro_y[sampleCount]);
			m_output.print("gyro_z = ");
			m_output.println(gyro_z[sampleCount]);
		}
		sampleCount++;
	}

	data_avgs[0] = get_average_double(orient_x, sampleCount);
	data_avgs[1] = get_average_double(orient_y, sampleCount);
	data_avgs[2] = get_average_double(orient_z, sampleCount);
	data_avgs[3] = get_average_double(orient_w, sampleCount);

	data_avgs[4] = get_average_double(accel_x, sampleCount);
	data_avgs[5] = get_average_double(accel_y, sampleCount);
	data_avgs[6] = get_average_double(accel_z, sampleCount);

	data_avgs[7] = get_average_double(gyro_x, sampleCount);
	data_avgs[8] = get_average_double(gyro_y, sampleCount);
	data_avgs[9] = get_average_double(gyro_z, sampleCount);

	// delete data arrays
	delete[] orient_x;
	delete[] orient_y;
	delete[] orient_z;
	delete[] orient_w;

	delete[] accel_x;
	delete[] accel_y;
	delete[] accel_z;

	delete[] gyro_x;
	delete[] gyro_y;
	delete[] gyro_z;
}

double AxisController::get_average_double(double * data_list, int list_length)
{
	double sampleSum = 0.0;
	for (int sampleNum = 0; sampleNum < list_length; ++sampleNum)
	{
		sampleSum += data_list[sampleNum];
	}
	return sampleSum / list_length;
}

int AxisController::move_axis(int *myoDataArray)
{
	//myoDataArray is ALWAYS an array of length 10
	//returns position the servo was moved to
	double new_servo_pos = round(map(myoDataArray[m_direction], m_inMin, m_inMax,
							   m_outMin, m_outMax));
	double old_servo_pos = m_axisServo.read();
	if (new_servo_pos != old_servo_pos)
	{
		m_axisServo.write(new_servo_pos);
	}
	return new_servo_pos;
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
