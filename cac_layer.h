#ifndef CACLAYER_H
#define CACLAYER_H

#include "lambda_control.h"
#include "connection.h"
#include "routing_sub_problem.h"
#include "wavelength_assignment_sub_problem.h"
#include "physical_layer.h"
#include "optical_amplifier.h"
#include "power.h"
#include <list>
#include <iostream>

class CACLayer {

public:
	CACLayer(unsigned *matrixLength, unsigned tNos, unsigned lambda);
	~CACLayer(){}

    enum QUALITY {QoT = 0, QoTThereshold};

	bool tryRequest(unsigned from, unsigned to, double QoS, double QoSThereshold);//continuidade test

	void preAlloc();//atualiza lista de conexoes
	//no simul as conexoes devem ser atualizadas
	bool testConnectionRequest(QUALITY q = QoT);//ver se a relação sinal ruido esta ok.//bloqueio por OSNR entrante
	bool testConnectionsAffected(QUALITY q = QoT);//ver se a relação sinal ruido das outras estao ok.//bloqueio por OSNR compartilhante
	bool testAllConnectionsAffected(QUALITY q = QoT);//ver se a relação sinal ruido de todas as conexoes estao ok

	void allocateResources(double actualTime, double duraction);
	void desAllocPre();

    //remover conexoes que passaram do tempo
	void updateConnectionsByTime(double actualTime);
	void updateConnectionsRemoved();

	void clearToUpdateList();
	void clearMatrixAllocation();
	void clearListOfConnections();
	void setUpConnection(Connection &con, const Power &power);
	void markConnectionsByConnection(Connection &con);

	Connection &getLastConnection();//para pegar a conexão e atualizar a camada física

	void setPhysicalLayer(PhysicalLayer *phy);

	std::list<Connection> &getListOfConnections();
	std::list<Connection*> &getToUpdateList();

	double QoSToQoT(double gama);

private:

    bool testOSNRByConnection(Connection &con, QUALITY q);

    std::list<Connection> conList;
    std::list<Connection*> toUpdateList;
    std::list<Connection> tmpRemoved;

    LambdaControl lc;
    RoutingSubProblem routing;
    WavelengthAssignmentSubProblem wave;

    //para o caso de algum algoritmo de roteamento e/ou atribuição de w usar a camada física. Uso futuro
    PhysicalLayer *phy;
};

#endif // CACLAYER_H
