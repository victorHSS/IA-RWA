#ifndef OPTICALFIBER_H
#define OPTICALFIBER_H

#include <cmath>
#include "wdm_link_component.h" // Base class: WDMLinkComponent

class OpticalFiber : public WDMLinkComponent {

public:
	OpticalFiber(unsigned lambda, double constantAttFactor = 0.2, unsigned spamL = 80):
        WDMLinkComponent(lambda),constantAttenuationFactor(constantAttFactor),spamLength(spamL){}
	OpticalFiber(const OpticalFiber &opticalFiber);

	virtual ~OpticalFiber(){}

	virtual void processSignal(unsigned lambda);

	unsigned getSpamLength() const;
	double getConstantAttFactor() const;

protected:
	double constantAttenuationFactor;
	unsigned spamLength;

};

#endif // OPTICALFIBER_H
