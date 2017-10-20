#include "stdafx.h"
#include "objectFactory.h"

//������Ʈ Ŭ����
#include "rock.h"
#include "tree.h"
#include "castle.h"
#include "town.h"
#include "wall.h"
#include "bridge.h"

#include "unit.h"



//������Ʈ�� ���丮 �������� �����Ϸ���
//������Ʈ�� init�Լ��� ���� �Ű����ڰ� ������ �����ϴ°� ����.
//���� ���丮 ���� ������ �ƴ϶� ������Ʈ �ʱ�ȭ�� �ܺ� �����ͷ� ���°� ����.
gameObject* objectFactory::createObject(int xIndex, int yIndex, string name)
{
	gameObject* newObj = NULL;

	if (name == "redTown")
	{
		newObj = new town;
		((town*)newObj)->init(xIndex, yIndex, CountryColor::RED);
	}
	else if (name == "blueTown")
	{
		newObj = new town;
		((town*)newObj)->init(xIndex, yIndex, CountryColor::BLUE);
	}
	else if (name == "whiteTown")
	{
		newObj = new town;
		((town*)newObj)->init(xIndex, yIndex, CountryColor::WHITE);
	}
	else if (name == "greenTown")
	{
		newObj = new town;
		((town*)newObj)->init(xIndex, yIndex, CountryColor::GREEN);
	}
	else if (name == "bridge")
	{

	}
	else if (name == "wall")
	{
		newObj = new wall;
		((wall*)newObj)->init(xIndex, yIndex);
	}
	else if (name == "tree")
	{
		newObj = new tree;
		((tree*)newObj)->init(xIndex, yIndex);
	}
	else if (name == "rock")
	{
		newObj = new rock;
		((rock*)newObj)->init(xIndex, yIndex);
	}
	else if (name == "blueStartUnit")
	{
		newObj = new unit;
		((unit*)newObj)->init(vector2D(xIndex, yIndex), 0, CountryColor::BLUE);
		((unit*)newObj)->setHp(500);
	}
	else if (name == "greenStartUnit")
	{
		newObj = new unit;
		((unit*)newObj)->init(vector2D(xIndex, yIndex), 0, CountryColor::GREEN);
		((unit*)newObj)->setHp(500);
	}
	else if (name == "redStartUnit")
	{
		newObj = new unit;
		((unit*)newObj)->init(vector2D(xIndex, yIndex), 0, CountryColor::RED);
		((unit*)newObj)->setHp(500);
	}
	else if (name == "whiteStartUnit")
	{
		//newObj = new unit;
		//((unit*)newObj)->init(vector2D(xIndex, yIndex), 0, CountryColor::WHITE);
		//((unit*)newObj)->setHp(500);
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

	//�׸�
	if (name == "greenKing0")
	{
		_castle = new castle;
		((castle*)_castle)->init(xIndex, yIndex, CountryColor::GREEN, Direction::Left);
	}
	else if (name == "greenKing1")
	{
		_castle = new castle;
		((castle*)_castle)->init(xIndex, yIndex, CountryColor::GREEN, Direction::Down);
	}
	else if (name == "greenKing2")
	{
		_castle = new castle;
		((castle*)_castle)->init(xIndex, yIndex, CountryColor::GREEN, Direction::Right);
	}
	else if (name == "greenKing3")
	{
		_castle = new castle;
		((castle*)_castle)->init(xIndex, yIndex, CountryColor::GREEN, Direction::Up);
	}

	//ȭ��Ʈ
	if (name == "whiteKing0")
	{
		_castle = new castle;
		((castle*)_castle)->init(xIndex, yIndex, CountryColor::WHITE, Direction::Left);
	}
	else if (name == "whiteKing1")
	{
		_castle = new castle;
		((castle*)_castle)->init(xIndex, yIndex, CountryColor::WHITE, Direction::Down);
	}
	else if (name == "whiteKing2")
	{
		_castle = new castle;
		((castle*)_castle)->init(xIndex, yIndex, CountryColor::WHITE, Direction::Right);
	}
	else if (name == "whiteKing3")
	{
		_castle = new castle;
		((castle*)_castle)->init(xIndex, yIndex, CountryColor::WHITE, Direction::Up);
	}

	
	return _castle;
}


//static �̹��� �ʱ�ȭ
image* objectFactory::_unitImage[12] = { NULL };

void objectFactory::initUnitImage()
{
	//����, ȭ��Ʈ, ���, �׸�
	//redLv1, redLv2, redLv3 / whiteLv1, whiteLv2, whiteLv3 
	char* colorList[] = { "red", "white", "blue", "greed" };
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 1; j <= 3; ++j)
		{
			char key[128] = "";
			wsprintf(key, "%sLv%d", colorList[i], j);
			_unitImage[i * 3 + (j-1)] = IMAGEMANAGER->findImage(key);
		}
	}
}
image* objectFactory::getUnitImage(CountryColor::Enum color, int lv)
{
	if (color == CountryColor::NATURE) return NULL;
	return _unitImage[color * 3 + (lv-1)];
}