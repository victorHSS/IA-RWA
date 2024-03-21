#ifndef OPTICALAMPLIFIER_H
#define OPTICALAMPLIFIER_H

#include "wdm_link_component.h" // Base class: WDMLinkComponent
#include "optical_fiber.h"
#include <cmath>

class OpticalAmplifier : public WDMLinkComponent {

public:
	OpticalAmplifier(const OpticalFiber &opticalFiber, unsigned lambda, long double pSat = 0.01,long double nsp = 4.0,
		long double h = 6.63e-13,	long double fc = 193.0,	long double B0 = 50.0, long double bitRate = 10.0,
		long double tol =	1.0e-12);
	OpticalAmplifier(const OpticalAmplifier &opticalAmplifier, const OpticalFiber &opticalFiber);

	virtual ~OpticalAmplifier(){}

	virtual void processSignal(unsigned lambda);

	long double getGain() const;
	long double getMaxGain() const;

	long double getBe() const;
	long double getB0() const;

private:
	Power getASENoise(unsigned lambda);

	long double nominalGainFunction();

	long double gain(long double pn);
	long double gainDerived(long double pn);

	long double PN(long double pn);

	//constante para calculo numerico do ganho
	long double TOL;


protected:
	//variaveis do ruído ASE
	long double pSat;
	long double nsp;
	long double h;
	long double fc;
	long double B0;
	long double Be; // 0.8 x Tx de Bits
	long double bitRate;

	//variaveis do ganho
	long double maxGain;
	long double nominalGain;

};

#endif // OPTICALAMPLIFIER_H
