#include "connection.h"

Connection::Connection():path(Path())
{
	timeStarted = duraction = QoT = 0.0;
	lambdaAllocated = -1;
	update = false;
	tT = UNIDIRECTIONAL;
}

void Connection::markToUpdate()
{
	update = true;
}

void Connection::cleanUpdate()
{
	update = false;
}

bool Connection::haveToUpdate() const
{
	return update;
}

void Connection::setPath(const Path &path)
{
	this->path = path;
}

const Path &Connection::getPath() const
{
	return path;
}

bool Connection::hasEnded(double actualTime) const
{
	return ( actualTime >= (timeStarted + duraction) );
}

double Connection::getQoTThereshold() const
{
    return QoTThereshold;
}

void Connection::setQoTThereshold(double QoT)
{
    QoTThereshold = QoT;
}

double Connection::getQoT() const
{
	return QoT;
}

void Connection::setQoT(double QoT)
{
	this->QoT = QoT;
}

void Connection::setStartTime(double timeStarted)
{
	this->timeStarted = timeStarted;
}

void Connection::setDuraction(double duraction)
{
	this->duraction = duraction;
}

void Connection::setLambda(unsigned lambda)
{
	this->lambdaAllocated = lambda;
}

unsigned Connection::getLambda() const
{
	return lambdaAllocated;
}


void Connection::setTransmissionType(Connection::TRANSMISSION_TYPE tT)
{
    this->tT = tT;
}

Connection::TRANSMISSION_TYPE Connection::getTransmissionType() const
{
    return tT;
}
