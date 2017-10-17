#pragma once



class objectFactory
{
private:

public:
	objectFactory() {}
	virtual ~objectFactory() {}

	gameObject*	createObject(int xIndex, int yIndex, string name);
};