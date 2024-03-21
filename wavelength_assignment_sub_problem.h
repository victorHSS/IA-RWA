#ifndef WAVELENGTHASSIGNMENTSUBPROBLEM_H
#define WAVELENGTHASSIGNMENTSUBPROBLEM_H

#include "path.h"
#include <iostream>
#include "lambda_control.h"


class WavelengthAssignmentSubProblem {

public:
	WavelengthAssignmentSubProblem();
	virtual ~WavelengthAssignmentSubProblem(){}

	virtual bool searchLambda(Path &path);
	void setStateNetwork(LambdaControl *lambdaControl);
	bool hasFindedLambda() const;
	unsigned getLastLambdaFinded() const;

protected:
	bool findedLambda;
	unsigned lastLambdaFinded;

	LambdaControl *lambdaControl;

	bool firstFitHeuristic(Path &path);

};

#endif // WAVELENGTHASSIGNMENTSUBPROBLEM_H
