#include "stdafx.h"
#include "objectFactory.h"

//오브젝트 클래스
#include "rock.h"
#include "tree.h"
#include "castle.h"



//오브젝트를 팩토리 패턴으로 생성하려면
//오브젝트의 init함수에 딱히 매개인자가 없도록 구현하는게 좋다.
//굳이 팩토리 패턴 때문이 아니라도 오브젝트 초기화는 외부 데이터로 빼는게 좋음.
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
	//성 만들기 ( 국가 색상 + King + 방향 )
	//0: left, 1: down, 2: right, 3: up


	//블루
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


	//레드
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
