#include <iostream>

#include "simulation.h"
#include "client_layer.h"
#include "cac_layer.h"
#include "physical_layer.h"
#include "optical_fiber.h"
#include "optical_amplifier.h"
#include "version.h"
#include <ctime>
#include <iostream>
#include <cmath>
#include <cstring>

using namespace std;


//unsigned dist[] =
//    {
//      //A     B    C     D     E     F     G     H     I
//        0,  240,   0,  320,    0,    0,    0,    0,    0,  //A
//        240,  0, 480,    0,   80,    0,    0,    0,    0,  //B
//        0,  480,   0,    0,    0,  160,    0,    0,    0,  //C
//        320,  0,   0,    0,  160,    0,  320,    0,    0,  //D
//        0,   80,   0,  160,    0,  480,    0,   80,    0,  //E
//        0,   0,  160,    0,  480,    0,    0,    0,  240,  //F
//        0,   0,    0,  320,    0,    0,    0,  320,    0,  //G
//        0,   0,    0,    0,   80,    0,  320,    0,  160,  //H
//        0,   0,    0,    0,    0,  240,    0,  160,    0   //I
//	};
//
//    int tNos = 9;

    int tNos = 19;
    char nome[] = "Rede americana.";
    unsigned dist[]=
    {
        0,	320,	0,	0,	0,	80,	0,	0,	0,	0,	0,	160,	0,	0,	0,	0,	0,	0,	0,
        320,	0,	80,	0,	0,	0,	240,	160,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
        0,	80,	0,	160,	160,	0,	0,	0,	240,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
        0,	0,	160,	0,	80,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
        0,	0,	160,	80,	0,	0,	0,	0,	0,	0,	80,	0,	0,	0,	0,	0,	0,	0,	0,
        80,	0,	0,	0,	0,	0,	80,	0,	0,	0,	0,	0,	240,	0,	0,	0,	0,	0,	0,
        0,	240,	0,	0,	0,	80,	0,	80,	0,	0,	0,	0,	0,	240,	0,	0,	0,	0,	0,
        0,	160,	0,	0,	0,	0,	80,	0,	80,	0,	0,	0,	0,	0,	160,	0,	0,	0,	0,
        0,	0,	240,	0,	0,	0,	0,	80,	0,	80,	0,	0,	0,	0,	0,	0,	80,	0,	0,
        0,	0,	0,	0,	0,	0,	0,	0,	80,	0,	80,	0,	0,	0,	0,	0,	0,	0,	320,
        0,	0,	0,	0,	80,	0,	0,	0,	0,	80,	0,	0,	0,	0,	0,	0,	0,	0,	0,
        160,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	160,	0,	0,	0,	0,	0,	0,
        0,	0,	0,	0,	0,	240,	0,	0,	0,	0,	0,	160,	0,	160,	0,	0,	0,	0,	0,
        0,	0,	0,	0,	0,	0,	240,	0,	0,	0,	0,	0,	160,	0,	0,	240,	0,	0,	0,
        0,	0,	0,	0,	0,	0,	0,	160,	0,	0,	0,	0,	0,	0,	0,	160,	160,	0,	0,
        0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	240,	160,	0,	0,	0,	0,
        0,	0,	0,	0,	0,	0,	0,	0,	80,	0,	0,	0,	0,	0,	160,	0,	0,	160,	0,
        0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	160,	0,	80,
        0,	0,	0,	0,	0,	0,	0,	0,	0,	320,	0,	0,	0,	0,	0,	0,	0,	80,	0
    };


//posso atualizar todas ou somente as diretamente afetadas (conexoes)
//ver como esta sendo calculado o OSNR. no sim antigo estou vendo o OSNR agregado de todos os enlaces até chegar na entrante.
//fazer teste para ver se quando removo conexoes alguma fica com o OSNR degradada...
//ver o RWA-Cego...
//ver os parametros do 05969826-v2.pdf. Ele considerou somente length e w.
//to achando estranho. Ta demorando a simulação ao considerar a camada fisica...
int main(int argc, char *argv[])
{
    printf("%s %s %s %s %s %s\n\n",argv[0],argv[1],argv[2],argv[3],argv[4],argv[5]);
	srand(time(0));
	unsigned lambda = 16;
	double constantAttFactor = 0.2;
	unsigned spamL = 80;
    long double pSat = 0.01, nsp = 2.0, h = 6.63e-13, fc = 193.0, B0 = 50.0, bitRate = 10;
    double gama = 7.0, gamaTol = 4.0, mii = 1.0;

    unsigned updates = 0;

    double la_min = atof(argv[3]), la_max = atof(argv[4]), la_step = atof(argv[5]);
    unsigned totalConnections = 10000;

    OpticalFiber oF(lambda,constantAttFactor,spamL);
    OpticalAmplifier oA(oF,lambda,pSat,nsp,h,fc,B0,bitRate);


    PhysicalLayer phy(oF,oA,dist,tNos,lambda,bitRate);
    ClientLayer cL(tNos,gama,gamaTol,mii);
    CACLayer cac(dist,tNos,lambda);

    cout << "IA-RWA Simulator: " << AutoVersion::UBUNTU_VERSION_STYLE << " - " << AutoVersion::FULLVERSION_STRING << endl;

    Simulation sim;
    sim.setUpActualizations(updates);
    sim.setUpClientLayer(cL);
    sim.setUpCACLayer(cac);
    sim.setUpPhysicalLayer(phy);

    sim.setConsiderPhyImpairments(true);//se false vira o blind-rwa?
    sim.setConsiderBlockByQoT(true);

    sim.configure(argv[1]);
    printf("Trafego: %g %g %g\n",la_min,la_max,la_step);
    sim.run(la_min,la_max,la_step,totalConnections);
	return 0;
}
