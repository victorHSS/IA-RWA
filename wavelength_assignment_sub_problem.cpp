#include "wavelength_assignment_sub_problem.h"

WavelengthAssignmentSubProblem::WavelengthAssignmentSubProblem()
{
	findedLambda = false;
	lastLambdaFinded = 0;

	lambdaControl = NULL;
}

bool WavelengthAssignmentSubProblem::searchLambda(Path &path)
{
	return findedLambda = firstFitHeuristic(path);
}

void WavelengthAssignmentSubProblem::setStateNetwork(LambdaControl *lambdaControl)
{
	this->lambdaControl = lambdaControl;
}

bool WavelengthAssignmentSubProblem::hasFindedLambda() const
{
	return findedLambda;
}

unsigned WavelengthAssignmentSubProblem::getLastLambdaFinded() const
{
	return this->lastLambdaFinded;
}

bool WavelengthAssignmentSubProblem::firstFitHeuristic(Path &path)
{
	for ( int lambda = 0 ; lambda < lambdaControl->getTotalLambdas() ; lambda++)
	{
		if (lambdaControl->canAlloc(path,lambda))
		{
		    lastLambdaFinded = lambda;
			return true;
		}
	}
	return false;
}
