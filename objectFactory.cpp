#include "stdafx.h"
#include "objectFactory.h"




//오브젝트를 팩토리 패턴으로 생성하려면
//오브젝트의 init함수에 딱히 매개인자가 없도록 구현하는게 좋다.
//굳이 팩토리 패턴 때문이 아니라도 오브젝트 초기화는 외부 데이터로 빼는게 좋음.
gameObject* objectFactory::createObject(string name)
{



	return NULL;
}
