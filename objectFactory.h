#pragma once



class objectFactory
{
private:
	gameObject*		_object;

public:
	objectFactory() : _object(NULL) {}
	virtual ~objectFactory() {}

	gameObject*	createObject(string name);
};