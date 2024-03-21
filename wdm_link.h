#ifndef WDMLINK_H
#define WDMLINK_H


#include "wdm_link_component.h"
#include "physical_layer.h"
#include "power.h"
#include "optical_amplifier.h"
#include "optical_fiber.h"
#include <vector>

class PhysicalLayer;
class OpticalAmplifier;
class OpticalFiber;
class WDMLinkComponent;

class WDMLink {

public:
    WDMLink():numSpams(0){}
	WDMLink(OpticalFiber &standardOpticalFiber, OpticalAmplifier &standardOpticalAmplifier, unsigned length = 0);
	virtual ~WDMLink();

	unsigned getTotalComponents() const;

	WDMLinkComponent &getComponent(unsigned i) const;
	WDMLinkComponent &getLastComponent();
	Power processAllComponentBySignal(const Power &power, unsigned lambda);

	friend std::ostream &operator<<(std::ostream &os, const WDMLink &link);

protected:

	std::vector <WDMLinkComponent*> component;
	unsigned numSpams;

};

#endif // WDMLINK_H
