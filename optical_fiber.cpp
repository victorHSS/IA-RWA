#include "optical_fiber.h"

void OpticalFiber::processSignal(unsigned lambda)
{
	double alfa = constantAttenuationFactor / 4.343;
	double attenuation = exp( - alfa * spamLength);

	powerOut[lambda] = powerIn[lambda]*attenuation;
}

OpticalFiber::OpticalFiber(const OpticalFiber &opticalFiber):WDMLinkComponent(0)
{
	*this = opticalFiber;
}

unsigned OpticalFiber::getSpamLength() const
{
	return spamLength;
}

double OpticalFiber::getConstantAttFactor() const
{
	return constantAttenuationFactor;
}


