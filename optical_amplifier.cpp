#include "optical_amplifier.h"

OpticalAmplifier::OpticalAmplifier(const OpticalFiber &opticalFiber, unsigned lambda, long double pSat, long double nsp,
    long double h,	long double fc,	long double B0, long double bitRate, long double tol):WDMLinkComponent(lambda)
{
	this->pSat = pSat;
	this->nsp = nsp;
	this->h = h;
	this->fc = fc;
	this->B0 = B0;
	this->Be = 0.8 * bitRate;
	this->bitRate = bitRate;
	this->TOL = tol;

	//variaveis do ganho
	nominalGain = maxGain = exp( (opticalFiber.getConstantAttFactor()/4.343)* opticalFiber.getSpamLength());
}

//passo zero para WDMLinkComponent para depois atribuir o valor real
OpticalAmplifier::OpticalAmplifier(const OpticalAmplifier &opticalAmplifier, const OpticalFiber &opticalFiber):WDMLinkComponent(0)
{
	*this = opticalAmplifier;
	//variaveis do ganho
    nominalGain = maxGain = exp( (opticalFiber.getConstantAttFactor()/4.343)* opticalFiber.getSpamLength());
}

void OpticalAmplifier::processSignal(unsigned lambda)
{
	powerOut[lambda] = (powerIn[lambda] * nominalGainFunction()) + getASENoise(lambda);
}

//tem uma referencia de um artigo que diz que o ASE gerado por essa funcao eh pra cada enlace
Power OpticalAmplifier::getASENoise(unsigned lambda)
{
	//testar se a potencia de entrada total nao eh zero
	long double ASE = 2 * nsp * h * fc * (nominalGainFunction() -1) * B0 ;
	return ((powerIn[lambda].getSignal() && totalPowerIn())? Power(0.0 , ASE) : Power(0.0,0.0) );
}

long double OpticalAmplifier::nominalGainFunction()
{
	//testes Pin
	return nominalGain = (totalPowerIn()?PN(maxGain):maxGain);
}

long double OpticalAmplifier::PN(long double pn)
{
	long double pn_1 = pn - gain(pn)/gainDerived(pn);

    if (fabs(pn_1 - pn) < TOL)
        return pn_1;
    else
        return PN(pn_1);
}

long double OpticalAmplifier::gain(long double pn)
{
	return (pn - 1.0f - ( pSat / totalPowerIn() ) * log( maxGain / pn ) );
}

long double OpticalAmplifier::gainDerived(long double pn)
{
	return 1 + pSat / ( pn * totalPowerIn() );
}

long double OpticalAmplifier::getGain() const
{
	return nominalGain;
}

long double OpticalAmplifier::getMaxGain() const
{
	return maxGain;
}

long double OpticalAmplifier::getBe() const
{
    return Be;
}

long double OpticalAmplifier::getB0() const
{
    return B0;
}
