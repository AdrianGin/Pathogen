#include "Oscillator.h"
#include <math.h>


Oscillator::Oscillator(Wavetable* wt) : wt(wt)
{	
	updateWavetable(wt);

}


Oscillator::~Oscillator()
{
}




void Oscillator::updateWavetable(Wavetable* wave)
{
	this->wt = wave;

	updateWavetable();
}


void Oscillator::updateWavetable(void)
{


	phaseIncrement = 1;
	phase = 0;

	
	startPoint = 0;

	endPoint = 0;
	loopPoint = 0;

	loopMode = ONE_SHOT_PING_PONG;
	loopPhase = INC;

	retrigFlag = 0;
	triggerHandled = 0;

	if (wt)
	{
		endPoint = wt->getFrameCount();
		loopPoint = (endPoint - 20000);
		smoothLoopPoints(RISING);
	}
}

void Oscillator::trigger(uint8_t velocity)
{
	if (velocity)
	{
		retrigFlag = 1;
		phase = startPoint;
		if (loopMode == REVERSE)
		{
			phase = endPoint - 1;
		}
		loopPhase = INC;
	}
}

//Find the first zero crossing between start and end in the direction of DIR
uint32_t Oscillator::FindZeroCrossing(uint8_t dir, uint32_t start, uint32_t end)
{
	double lastSample = getSample(LEFT_CHANNEL, start);
	double curSample = getSample(LEFT_CHANNEL, start);

	for (uint32_t i = start; i < end; ++i)
	{
		lastSample = curSample;
		curSample = getSample(LEFT_CHANNEL, i);

		if ((dir == RISING) &&
			(curSample < 0.0) &&
			(lastSample >= 0.0) &&
			(curSample != lastSample))
		{
			return i;
		}

		if ((dir == FALLING) &&
			(curSample >= 0.0) &&
			(lastSample < 0.0) &&
			(curSample != lastSample))
		{
			return i;
		}



	}

	return 0;
}

//Find the zero crossings
uint8_t Oscillator::smoothLoopPoints(uint8_t dir)
{
	const uint32_t SEEK_LIMIT = 4096*3;
	uint32_t tempLoopPoint[2];

	tempLoopPoint[0] = FindZeroCrossing(dir, loopPoint - SEEK_LIMIT, loopPoint + SEEK_LIMIT);
	tempLoopPoint[1] = FindZeroCrossing(dir, endPoint - SEEK_LIMIT, endPoint);

	if ( (tempLoopPoint[0] != 0) &&
		 (tempLoopPoint[1] != 0) && (tempLoopPoint[1] > tempLoopPoint[0]))
	{
		loopPoint = tempLoopPoint[0];
		endPoint = tempLoopPoint[1];

		return 1;
	}

	return 0;

}


double Oscillator::getSample(uint8_t chIndex, uint32_t ph)
{
	double* samples = wt->getLeftSamples();
	if (chIndex == Oscillator::RIGHT_CHANNEL)
	{
		samples = wt->getRightSamples();
	}

	if (samples)
	{
		return samples[ph];
	}

	return 0.0;
}

double Oscillator::getSample(uint8_t chIndex)
{
	if (loopPhase == ONESHOT_FINISHED)
	{
		return 0.0;
	}

	double* samples = wt->getLeftSamples();
	if (chIndex == Oscillator::RIGHT_CHANNEL)
	{
		samples = wt->getRightSamples();
	}

	if (samples)
	{
		if (loopPhase == DEC)
		{
			return -samples[phase];
		}

		return samples[phase];
	}

	return 0.0;
}

void Oscillator::updatePhase(void)
{
	

	switch (loopMode)
	{
		case FORWARD:
		{
			phase += phaseIncrement;
			if (phase >= endPoint)
			{
				phase = (phase - endPoint) + startPoint;
			}
		}
		break;

		case REVERSE:
		{
			phase -= phaseIncrement;
			//For reverse
			if (phase < startPoint)
			{
				phase = phase + endPoint - startPoint;
			}
		}
		break;

		case PING_PONG:
		{
			if (loopPhase == INC)
			{
				phase += phaseIncrement;
				while (phase >= endPoint)
				{
					phase = phase - phaseIncrement;
					loopPhase = DEC;
				}
			}
			else if (loopPhase == DEC)
			{
				phase -= phaseIncrement;
				while (phase < startPoint)
				{
					loopPhase = INC;
					phase += phaseIncrement;
				}
			}
		}
		break;

		case ONE_SHOT:
		{
			phase += phaseIncrement;
			if (phase >= endPoint)
			{
				loopPhase = ONESHOT_FINISHED;
				phase = startPoint;
			}
		}
		break;

		case ONE_SHOT_FORWARD:
		{
			phase += phaseIncrement;
			if (phase >= endPoint)
			{
				phase = (phase - endPoint) + loopPoint;
			}
		}
		break;

		case ONE_SHOT_PING_PONG:
		{
			if (loopPhase == INC)
			{
				phase += phaseIncrement;
				while (phase >= endPoint)
				{
					phase = phase - phaseIncrement;
					loopPhase = DEC;
				}
			}
			else if (loopPhase == DEC)
			{
				phase -= phaseIncrement;
				while (phase < loopPoint)
				{
					loopPhase = INC;
					phase += phaseIncrement;
				}
			}
		}
		break;



	}


}