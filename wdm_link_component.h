#ifndef WDMLINKCOMPONENT_H
#define WDMLINKCOMPONENT_H

#include "power.h"
#include <vector>

class WDMLinkComponent {

public:
    WDMLinkComponent(unsigned lambda);
	virtual ~WDMLinkComponent(){}

	void setPowerIn(const Power &power, unsigned lambda);
	const Power &getPowerIn(unsigned lambda) const;
	virtual void processSignal(unsigned lambda) = 0;
	const Power &getPowerOut(unsigned lambda) const;
	unsigned getTotalLambda() const;
	long double totalPowerIn();
	long double totalPowerOut();

protected:
	std::vector<Power> powerIn;
	std::vector<Power> powerOut;
	unsigned lambda;
};

#endif // WDMLINKCOMPONENT_H
