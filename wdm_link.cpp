#include "wdm_link.h"

WDMLink::WDMLink(OpticalFiber &standardOpticalFiber, OpticalAmplifier &standardOpticalAmplifier, unsigned length)
{
	//se nao houver o link -- length == 0 -- nao inicia nada...
	if (length == 0)
		return;

	numSpams = (length/80);
	component.reserve(numSpams * 2);

	//contruindo enlace com a fibra e o amplificador padrao (passados para a classe PhysicalLayer)
	for ( int i = 0 ; i < numSpams ; i++)
	{
		component.push_back(new OpticalFiber(standardOpticalFiber));
		component.push_back(new OpticalAmplifier(standardOpticalAmplifier,
            *(static_cast<OpticalFiber*> (component[component.size()-1]) ) ) );
	}

}

WDMLink::~WDMLink()
{
	for ( int i = 0 ; i < 2*numSpams ; i++)
	{
		delete component[i];
	}
}

unsigned WDMLink::getTotalComponents() const
{
	return component.size();
}

Power WDMLink::processAllComponentBySignal(const Power &power, unsigned lambda)
{
	Power power_(power);
	for ( int i = 0 ; i < component.size() ; i++)
	{
		component[i]->setPowerIn(power_,lambda);
		component[i]->processSignal(lambda);
		power_ = component[i]->getPowerOut(lambda);
	}

	return power_;
}

WDMLinkComponent &WDMLink::getComponent(unsigned i) const
{
	//if ( i >= component.size() ) //reportar erro
	//	throw ;
	return *component[i];
}

WDMLinkComponent &WDMLink::getLastComponent()
{
    return *component[component.size()-1];
}

std::ostream &operator<<(std::ostream &os, const WDMLink &link)
{
    for (int i = 0 ; i < link.getTotalComponents() ; i++)
    {
        os << "Componente " << i << ": \n";
        for (int j = 0 ; j < link.getComponent(i).getTotalLambda() ; j++ )
        {
            os << "[" << j << "]" << link.getComponent(i).getPowerIn(j) << " " << link.getComponent(i).getPowerOut(j) << std::endl;
        }
    }
    return os;
}
