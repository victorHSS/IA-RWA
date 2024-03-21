#include "physical_layer.h"

PhysicalLayer::PhysicalLayer(OpticalFiber &opticalFiber, OpticalAmplifier &opticalAmplifier,
    unsigned *lengthMatrix, unsigned tNos, unsigned lambdaa, double bitRatee):
    standardOpticalFiber(opticalFiber),standardOpticalAmplifier(opticalAmplifier),
    totalNos(tNos),lambda(lambdaa),bitRate(bitRatee)
{
	link.reserve(tNos*tNos); //aloca memoria
	for (int i = 0 ; i < tNos * tNos ; i++)
	{
		if (lengthMatrix[i])
			link.push_back(new WDMLink(opticalFiber, opticalAmplifier, lengthMatrix[i]));
		else
			link.push_back(NULL);
	}
}

PhysicalLayer::~PhysicalLayer()
{
	for ( int i = 0 ; i < link.size() ; i++)
		delete link[i];
}

unsigned PhysicalLayer::totalLambdas() const
{
	return lambda;
}

OpticalFiber PhysicalLayer::getDefaultOpticalFiber() const
{
	return standardOpticalFiber;
}

OpticalAmplifier PhysicalLayer::getDefaultOpticalAmplifier() const
{
	return standardOpticalAmplifier;
}

double PhysicalLayer::getBitRate() const
{
	return bitRate;
}

WDMLink &PhysicalLayer::getLink(unsigned from, unsigned to)
{
	return *link[from*totalNos + to];
}


unsigned PhysicalLayer::getTotalNodes() const
{
    return this->totalNos;
}
