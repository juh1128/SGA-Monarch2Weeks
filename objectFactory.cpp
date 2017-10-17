#include "stdafx.h"
#include "objectFactory.h"

//������Ʈ Ŭ����
#include "rock.h"
#include "tree.h"
#include "castle.h"



//������Ʈ�� ���丮 �������� �����Ϸ���
//������Ʈ�� init�Լ��� ���� �Ű����ڰ� ������ �����ϴ°� ����.
//���� ���丮 ���� ������ �ƴ϶� ������Ʈ �ʱ�ȭ�� �ܺ� �����ͷ� ���°� ����.
gameObject* objectFactory::createObject(int xIndex, int yIndex, string name)
{
	gameObject* newObj = NULL;
	if (name == "tree")
	{
		newObj = new tree;
		((tree*)newObj)->init(xIndex, yIndex);
	}
	else if (name == "rock")
	{
		newObj = new rock;
		((rock*)newObj)->init(xIndex, yIndex);
	}
	else
	{
		newObj = createCastle(xIndex, yIndex, name);
	}


	return newObj;
}

gameObject * objectFactory::createCastle(int xIndex, int yIndex, string name)
{
	gameObject* _castle = NULL;
	//�� ����� ( ���� ���� + King + ���� )
	//0: left, 1: down, 2: right, 3: up


	//���
	if (name == "blueKing0")
	{
		_castle = new castle;
		((castle*)_castle)->init(xIndex, yIndex, CountryColor::BLUE, Direction::Left);
	}
	else if (name == "blueKing1")
	{
		_castle = new castle;
		((castle*)_castle)->init(xIndex, yIndex, CountryColor::BLUE, Direction::Down);
	}
	else if (name == "blueKing2")
	{
		_castle = new castle;
		((castle*)_castle)->init(xIndex, yIndex, CountryColor::BLUE, Direction::Right);
	}
	else if (name == "blueKing3")
	{
		_castle = new castle;
		((castle*)_castle)->init(xIndex, yIndex, CountryColor::BLUE, Direction::Up);
	}


	//����
	if (name == "redKing0")
	{
		_castle = new castle;
		((castle*)_castle)->init(xIndex, yIndex, CountryColor::RED, Direction::Left);
	}
	else if (name == "redKing1")
	{
		_castle = new castle;
		((castle*)_castle)->init(xIndex, yIndex, CountryColor::RED, Direction::Down);
	}
	else if (name == "redKing2")
	{
		_castle = new castle;
		((castle*)_castle)->init(xIndex, yIndex, CountryColor::RED, Direction::Right);
	}
	else if (name == "redKing3")
	{
		_castle = new castle;
		((castle*)_castle)->init(xIndex, yIndex, CountryColor::RED, Direction::Up);
	}

	
	return _castle;
}
