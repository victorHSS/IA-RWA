#ifndef CONNECTION_H
#define CONNECTION_H

#include "path.h"

class Connection {

public:
	Connection();
	~Connection(){}

	void markToUpdate();
	void cleanUpdate();
	bool haveToUpdate() const;
	void setPath(const Path &path);
	const Path &getPath() const;
	bool hasEnded(double actualTime) const;
	double getQoT() const;
	void setQoT(double QoT);
	double getQoTThereshold() const;
	void setQoTThereshold(double QoT);
	void setStartTime(double timeStarted);
	void setDuraction(double duraction);
	void setLambda(unsigned lambda);
	unsigned getLambda() const;

	enum TRANSMISSION_TYPE {UNIDIRECTIONAL, BIDIRECTIONAL};

	void setTransmissionType(TRANSMISSION_TYPE tT);
	TRANSMISSION_TYPE getTransmissionType() const;

protected:
	double timeStarted;
	double duraction;
	unsigned lambdaAllocated;
	double QoT;
	double QoTThereshold;
	bool update;
	Path path;

	TRANSMISSION_TYPE tT;
};

#endif // CONNECTION_H
