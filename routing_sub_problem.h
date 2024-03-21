#ifndef ROUTINGSUBPROBLEM_H
#define ROUTINGSUBPROBLEM_H

#include "path.h"
#include "lambda_control.h"
#include <vector>
#include <queue>
#include <iostream>
#include <cmath>


using std::vector;
using std::queue;

class RoutingSubProblem {

public:
	RoutingSubProblem(unsigned *matrixLength = NULL, unsigned tNos = 0);
	virtual ~RoutingSubProblem(){}

	virtual bool searchRoute(unsigned from, unsigned to);
	void setMatrixLength(unsigned *matrixLength, unsigned tNos);
	void setStateNetwork(LambdaControl *lambdaControl);
	bool hasFindedRoute() const;
	const Path &getPath() const;

protected:
	bool findedRoute;
	unsigned tNos;
	unsigned *matrixLength;
	Path route;
	LambdaControl *lambdaControl;

	bool dijkstra(unsigned s, unsigned t);
	unsigned getEdge(unsigned s, unsigned t);

};

#endif // ROUTINGSUBPROBLEM_H
