#pragma once
#include "tileMap.h"
#include "country.h"

#define MAX_LAYER 5

class unit;

//�� ������ ������Ʈ���� �Ѱ��ϴ� Ŭ������.
class world
{
private:
	//������Ʈ ����Ʈ(��, ����, ��å, ����, ��)
	vector<gameObject*> _objectList[MAX_LAYER];

	//���� ����Ʈ (���� ����)
	country*			_country[CountryColor::END];

	//Ÿ�ϸ� ����
	tileMap*			_tileMap;

public:
	world() {}
	~world() {}

	void init();
	void update();
	void render();
	void release();

	//������Ʈ�� �߰��Ѵ�. (2��° ���ڴ� ������ �켱����)
	void addObject(gameObject* obj, int renderPriority = 1);
	//name�� ��ġ�ϴ� ������Ʈ�� ã�Ƽ� ��ȯ�Ѵ�.
	gameObject* findObject(string name);
	//name�� ��ġ�ϴ� ������Ʈ�� ��� ã�Ƽ� ���ͷ� ��ȯ�Ѵ�.
	vector<gameObject*> findObjects(string name);
	
	vector<gameObject*> findObjects(string name, int priorityNum);


	//===============================================
	// ## ����, ���� ���� ##
	//===============================================
	void addUnit(unit* newUnit, CountryColor::Enum countryColor);
	country* getCountry(CountryColor::Enum color);

	void destroyCountryBuilding(CountryColor::Enum color);


	//===============================================
	// ## Ÿ�ϸ� ���� ##
	//===============================================

	//Ÿ�� ��� ����
	void releaseTiles();
	//Ÿ�� ����
	void loadTiles(string directory);
	//�� ������
	vector2D getMapSize();
	//Ÿ���� ����
	POINT getTileCount();

	//Ÿ�� ����Ʈ ���� (�ش� �ε��� Ÿ���� �̵� ���� ���θ� ��ȯ�Ѵ�.)
	bool isPossibleMove(int xIndex, int yIndex);
	//Ÿ�� ����Ʈ ���� (�ش� �ε��� Ÿ���� �̵��ӵ����� ��ȯ�Ѵ�.)
	float getTileMoveRatio(int xIndex, int yIndex);
	
	//Ÿ�ϸ� ��ȯ
	tileMap* getMap() { return _tileMap; }

};