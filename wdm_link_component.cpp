#include "wdm_link_component.h"

WDMLinkComponent::WDMLinkComponent(unsigned lambda):powerIn(lambda,Power()),powerOut(lambda,Power())
{
//	powerIn(lambda,Power());
//	powerOut(lambda,Power()); //verificar se a ordem dos params estao corretos
	this->lambda = lambda;
}

void WDMLinkComponent::setPowerIn(const Power &power, unsigned lambda)
{
	this->powerIn[lambda] = power;
}

const Power &WDMLinkComponent::getPowerIn(unsigned lambda) const
{
	return powerIn[lambda];
}

const Power &WDMLinkComponent::getPowerOut(unsigned lambda) const
{
	return powerOut[lambda];
}

unsigned WDMLinkComponent::getTotalLambda() const
{
    return lambda;
}

long double WDMLinkComponent::totalPowerIn()
{
	Power pSum(0.0,0.0);

	for ( unsigned i = 0 ; i < getTotalLambda() ; i++)
		pSum = (pSum + powerIn[i]);

	return pSum.getTotalSignalPlusNoise();
}

long double WDMLinkComponent::totalPowerOut()
{
	Power pSum(0.0,0.0);

	for ( unsigned i = 0 ; i < getTotalLambda() ; i++)
		pSum = (pSum + powerOut[i]);

	return pSum.getTotalSignalPlusNoise();
}
