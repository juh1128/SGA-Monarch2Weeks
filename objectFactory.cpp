#include "stdafx.h"
#include "objectFactory.h"

//������Ʈ Ŭ����
#include "rock.h"
#include "tree.h"



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


	return newObj;
}
