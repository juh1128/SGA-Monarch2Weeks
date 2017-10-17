#include "stdafx.h"
#include "objectFactory.h"

//오브젝트 클래스
#include "rock.h"
#include "tree.h"



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


	return newObj;
}
