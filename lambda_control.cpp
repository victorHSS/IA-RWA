#include "lambda_control.h"

LambdaControl::LambdaControl(unsigned tNos, unsigned lambda):matrixAllocation(tNos*tNos*lambda,0),lambdaUsed(tNos*tNos,0)
{
	//matrixAllocation.reserve(tNos*tNos*lambda, NULL);
	this->tNos = tNos;
	tLambdas = lambda;
}

unsigned LambdaControl::getMap(unsigned from, unsigned to, unsigned lambda)
{
	//return from*tNos*tNos + to*tNos + lambda;//<<<ERRADO
	return lambda*tNos*tNos + from*tNos + to;
}

bool LambdaControl::canAlloc(const Connection &connection, unsigned lambda, Connection::TRANSMISSION_TYPE tT)
{
	Path path = connection.getPath();
	path.goStart();
	while ( !path.isEndPath())
	{
		if (matrixAllocation[getMap(path.getActualStep(),path.getNextStep(),lambda)] != NULL &&
            (tT == Connection::BIDIRECTIONAL?(matrixAllocation[getMap(path.getNextStep(),path.getActualStep(),lambda)] != NULL):true ))
			return false;
		path.goAhead();
	}
	return true;
}


bool LambdaControl::canAlloc(const Path &path, unsigned lambda, Connection::TRANSMISSION_TYPE tT)
{
	Path path_ = path;
	path_.goStart();
	while ( !path_.isEndPath())
	{
		if (matrixAllocation[getMap(path_.getActualStep(),path_.getNextStep(),lambda)] != NULL &&
            (tT == Connection::BIDIRECTIONAL?(matrixAllocation[getMap(path_.getNextStep(),path_.getActualStep(),lambda)] != NULL):true ))
			return false;
		path_.goAhead();
	}
	return true;
}

void LambdaControl::allocConnection(Connection &connection, unsigned lambda, Connection::TRANSMISSION_TYPE tT)
{
	Path path = connection.getPath();
	path.goStart();
	while ( !path.isEndPath())
	{
		matrixAllocation[getMap(path.getActualStep(),path.getNextStep(),lambda)] = &connection;
		lambdaUsed[path.getActualStep()*tNos + path.getNextStep()]++;

		if (tT == Connection::BIDIRECTIONAL)
		{
            matrixAllocation[getMap(path.getNextStep(),path.getActualStep(),lambda)] = &connection;
		    lambdaUsed[path.getNextStep()*tNos + path.getActualStep()]++;
		}
		path.goAhead();
	}
}

void LambdaControl::disallocConnection(Connection &connection)
{
    Path path = connection.getPath();
	path.goStart();
	while ( !path.isEndPath())
	{
		matrixAllocation[getMap(path.getActualStep(),path.getNextStep(),connection.getLambda())] = NULL;
		lambdaUsed[path.getActualStep()*tNos + path.getNextStep()]--;

		if (connection.getTransmissionType() == Connection::BIDIRECTIONAL)
		{
            matrixAllocation[getMap(path.getNextStep(),path.getActualStep(),connection.getLambda())] = NULL;
		    lambdaUsed[path.getNextStep()*tNos + path.getActualStep()]--;
		}
		path.goAhead();
	}
}

unsigned LambdaControl::getTotalLambdas() const
{
	return tLambdas;
}

Connection *LambdaControl::getConnection(unsigned from, unsigned to, unsigned lambda)
{
    return matrixAllocation[getMap(from,to,lambda)];
}

void LambdaControl::clear()
{
    for (int i = 0 ; i < matrixAllocation.size() ; i++)
    {
        matrixAllocation[i] = 0;
    }
    for (int i = 0 ; i < lambdaUsed.size() ; i++)
    {
        lambdaUsed[i] = 0;
    }
}

unsigned LambdaControl::getTotalLambdasAllocatedIn(unsigned from, unsigned to)
{
    return lambdaUsed[from*tNos + to];
}

unsigned LambdaControl::getTotalLambdasFreeIn(unsigned from, unsigned to)
{
    return tLambdas - lambdaUsed[from*tNos + to];
}
