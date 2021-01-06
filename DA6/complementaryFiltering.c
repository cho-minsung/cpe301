#define ACCELEROMETER_SENSITIVITY 16384.0
#define GYROSCOPE_SENSITIVITY 16.4
#define dt 0.01

void ComplementaryFilter()
{
	folat pitchAcc, rollAcc;
	
	pitch += ((float)gyrData[0] / GYROSCOPE_SENSITIVITY) * dt;
	roll -= ((float)gyrData[1] / GYROSCOPE_SENSITIVITY) * dt;
	
	int forceMagnitudeApprox = \ abs(accData[0]) + abs(accData[1]) + abs(accData[2]);
	
	if (forceMagnitudeApprox > 8192 && forceMagnitudeApprox < 32768)
	{
		pitchAcc = atan2f((float)accData[1], (float)accData[2]) * 180 / M_PI;
		pitch = pitch * 0.98 + pitchAcc * 0.02;
		
		rollAcc = atan2f((float)accData[0], (float)accData[2]) * 180 /M_PI;
		roll = roll * 0.98 + rollAcc * 0.02;
	}
}