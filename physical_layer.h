#ifndef PHYSICALLAYER_H
#define PHYSICALLAYER_H


#include "optical_amplifier.h"
#include "optical_fiber.h"
#include "wdm_link.h"

#include <vector>

class WDMLink;
class OpicalFiber;
class OpticalAmplifier;

class PhysicalLayer {

public:
	PhysicalLayer(OpticalFiber &opticalFiber, OpticalAmplifier &opticalAmplifier, unsigned *lengthMatrix, unsigned tNos, unsigned lambdaa, double bitRatee);

	~PhysicalLayer();

	unsigned totalLambdas() const;
	OpticalFiber getDefaultOpticalFiber() const;
	OpticalAmplifier getDefaultOpticalAmplifier() const;
	double getBitRate() const;
	WDMLink &getLink(unsigned from, unsigned to);

	unsigned getTotalNodes() const;

protected:
	OpticalFiber standardOpticalFiber;
	OpticalAmplifier standardOpticalAmplifier;

	unsigned lambda;
	double bitRate;
	unsigned totalNos;

	std::vector<WDMLink *> link;//atribuir NULL ao enlace que não existir :)

};

#endif // PHYSICALLAYER_H
