#ifndef SIMULATION_H
#define SIMULATION_H

#include "client_layer.h"
#include "cac_layer.h"
#include "physical_layer.h"
#include "connection.h"
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Simulation {

public:
	Simulation();
	~Simulation(){}

    void setUpClientLayer(ClientLayer &clientLayer);
    void setUpCACLayer(CACLayer &cacLayer);
    void setUpPhysicalLayer(PhysicalLayer &physicalLayer);
    void setUpActualizations(unsigned actualizations);

    void setConsiderPhyImpairments(bool considerPhyImpairments);
    void setConsiderBlockByQoT(bool considerBlockByQoT);

    bool configure(char *progresso);

    void reset();
    void simulate(unsigned totalCalls);
    void store();

    void run(double la_min, double la_max, double la_step, unsigned totalCalls);
    void updatePhysicalLayer(bool flag = true);
    void updatePhysicalLayerConnection(Connection &con);

    void preComputeGainVariation(Connection &con);
    void posComputeGainVariation(Connection &con, unsigned actualization);

private:

    char *progresso;

    ClientLayer *cL;
    PhysicalLayer *phy;
    CACLayer *cac;
    unsigned actualizations;
    //variaveis de bloqueio (3 tipos)
    unsigned blockByCont;
    unsigned blockByOSNRInnerCon;
    unsigned blockByOSNRAffectedCon;

    unsigned tCalls;//total de chamadas

    unsigned sizeInHops;//somente das con admitidas pela continuidade

    //variacao do ganho
    vector <double> gain_before;
    vector <double> gain_after;
    vector <double> rateGainAffected;
    vector <unsigned> tGainAffected;

    unsigned TVP;
    unsigned CVP;

    bool considerPhyImpairments;
    bool considerBlockByQoT;
};

#endif // SIMULATION_H
