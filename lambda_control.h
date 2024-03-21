#ifndef LAMBDACONTROL_H
#define LAMBDACONTROL_H

#include "connection.h"

class LambdaControl {

public:
	LambdaControl(unsigned tNos, unsigned lambda);
	~LambdaControl(){}
	bool canAlloc(const Connection &connection, unsigned lambda, Connection::TRANSMISSION_TYPE tT = Connection::UNIDIRECTIONAL);
	bool canAlloc(const Path &path, unsigned lambda, Connection::TRANSMISSION_TYPE tT = Connection::UNIDIRECTIONAL);
	void allocConnection(Connection &connection, unsigned lambda, Connection::TRANSMISSION_TYPE tT = Connection::UNIDIRECTIONAL);
	void disallocConnection(Connection &connection);
	Connection *getConnection(unsigned from, unsigned to, unsigned lambda); // recebe uma conexao e marka todas as adjacentes
	unsigned getTotalLambdas() const;

	unsigned getTotalLambdasAllocatedIn(unsigned from, unsigned to);
	unsigned getTotalLambdasFreeIn(unsigned from, unsigned to);

	void clear();
private:
	unsigned getMap(unsigned from, unsigned to, unsigned lambda);
	vector<Connection*> matrixAllocation;
	vector<unsigned> lambdaUsed;
	unsigned tNos;
	unsigned tLambdas;
};

#endif // LAMBDACONTROL_H
