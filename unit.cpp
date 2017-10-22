#include "stdafx.h"
#include "unit.h"
#include "mncObjectBase.h"
#include "objectFactory.h"

unit::unit()
{
}


unit::~unit()
{
}

HRESULT unit::init(vector2D index, int height,CountryColor::Enum country)
{
	//������ �Ҽӻ���
	_unitColor = country;
	string color = getColorString();

	gameObject::init("unit", color + "Lv1", tileMap::getTilePosFromIndex(index, height), Pivot::CENTER);
	this->setImage(objectFactory::getUnitImage(country, 1), objectFactory::getUnitImage(country, 2),
		objectFactory::getUnitImage(country, 3));

	_pos = _pos / CAMERA->getZoom();
	_index = index;

	_hp = 100;
	_isStarUnit = false;

	terrainTile* onTile = WORLD->getMap()->getTile(_index.x, _index.y);
	_height = onTile->getHeight();
	onTile->addUnitOnTile(this);
	_myTiles.push_back(onTile);

	_imageFrameX = 0;
	_unitDirection = (UnitDirection::DIRECTION)(RND->getFromIntTo(0, 3));

	_isAuto = true;
	_unitState = new unitNoneState;
	_moveSpeed = 1.5f;
	_frameTimer = 0;
	_mergeUnit = NULL;

	_commandDestTile = NULL;
	_commandTargetUnit = NULL;
	
	changeState(new unitCreateMotion);

	//this->addCallback("���", [&](tagMessage msg) {
	//	terrainTile* tile = (terrainTile*)msg.targetList[0];
	//	this->moveAstar(tile->getIndex().x, tile->getIndex().y);
	//	this->reserveState(new unitNoneState);
	//	this->setAuto(false);
	//});

	//this->addCallback("�ڵ�", [&](tagMessage msg) {
	//	terrainTile* tile = (terrainTile*)msg.targetList[0];
	//	this->moveAstar(tile->getIndex().x, tile->getIndex().y);
	//	this->reserveState(new unitNoneState);
	//	this->setAuto(true);
	//});

	//this->addCallback("���� ����", [&](tagMessage msg) {
	//	terrainTile* tile = (terrainTile*)msg.targetList[0];
	//	this->moveAstar(tile->getIndex().x, tile->getIndex().y);
	//	this->reserveState(new unitBuildTown(msg.ptData));
	//	this->setAuto(true);
	//});
	//this->addCallback("��å ����", [&](tagMessage msg) {
	//	terrainTile* tile = (terrainTile*)msg.targetList[0];
	//	this->moveAstar(tile->getIndex().x, tile->getIndex().y);
	//	this->reserveState(new unitNoneState);
	//	this->setAuto(true);
	//});
	//this->addCallback("�ٸ� �Ǽ�", [&](tagMessage msg) {
	//	terrainTile* tile = (terrainTile*)msg.targetList[0];
	//	this->moveAstar(tile->getIndex().x, tile->getIndex().y);
	//	this->reserveState(new unitNoneState);
	//	this->setAuto(true);
	//});

	//==================================================
	// ## ���� ���
	this->addCallback("����", [&](tagMessage msg) {	
		unit* target = (unit*)msg.targetList[0];
		if (target == this)
		{
			return;
		}
		//��󿡰� Merge �޽����� �ߵ��ϸ�, ������ �˷��޶�� ����Ѵ�.
		//������ ������ ����� �ߴ� Ÿ���� ������, �����Ѵ�.
		target->addObserver("Merge", this);

		unit* oldTarget = this->getCommandTarget();
		if (oldTarget)
			oldTarget->removeObserver("Merge", this);

		this->setCommand(NULL, target, "����");

	});
	//==================================================
	// ## ���� ���
	this->addCallback("����", [&](tagMessage msg) {
		unit* target = (unit*)msg.targetList[0];
		if (target == this) return;		

		target->addObserver("Merge", this);
		unit* oldTarget = this->getCommandTarget();
		if (oldTarget)
			oldTarget->removeObserver("Merge", this);

		this->setCommand(NULL, target, "����");
	});
	//==================================================
	// ## �i�ư��� ������ ���յǾ ������� ��� ó��
	this->addCallback("observed_Merge", [this](tagMessage msg)
	{
		//�ڱⰡ �i�ư��� ������ ���յǼ� ������� ���, �ٸ� ������� �����Ѵ�.
		this->setCommand(NULL, (unit*)msg.targetList[0], this->getCommandName());
		msg.targetList[0]->addObserver("Merge", this);
	});

	return S_OK;
}

void unit::release()
{
	//������ ������ �� �ڱⰡ ���ִ� Ÿ�Ͽ� ��� ������.
	for (size_t i = 0; i < _myTiles.size(); ++i)
	{
		_myTiles[i]->deleteUnitOnTile(this);
	}

	gameObject::release();
}

void unit::update()
{

	gameObject::update();

	imageFrame();

	if (_commandTargetUnit)
	{
		if (!_commandTargetUnit->isLive())
		{
			resetCommand();
		}
	}

	_unitState->update(*this);

	//Z���� �� ������ Ÿ�Ͽ��� �������� ��û�Ѵ�.
	requestRender();
	if (_hp <= 0)
	{
		return this->setDestroy();
	}
}

void unit::render()
{
	if (_image)
	{
		float zoom = CAMERA->getZoom();
		float imageHalfHeight = _image->getFrameSize(_imageFrameX).y * zoom * 0.5f;

		_scale = vector2D(zoom, zoom);

		_image->setAlphaOption(_alpha);
		_image->setScaleOption(_scale);
		_image->frameRender(_pos.x*zoom, _pos.y*zoom + imageHalfHeight-10, _imageFrameX, _unitDirection, _pivot);

		vector2D renderPos = _pos*zoom;
		renderPos = CAMERA->getRelativeVector2D(renderPos);

		if (_commandStateName != "")
		{
			IMAGEMANAGER->drawText(renderPos.x, renderPos.y, UTIL::string_to_wstring(_commandStateName), 14, DefaultBrush::white,
				DWRITE_TEXT_ALIGNMENT_LEADING);
		}
		else if (_state == UnitState::Search)
		{
			IMAGEMANAGER->drawText(renderPos.x, renderPos.y, L"Ž��", 14, DefaultBrush::white,
				DWRITE_TEXT_ALIGNMENT_LEADING);
		}
		else if (_state == UnitState::Run)
		{		
			IMAGEMANAGER->drawText(renderPos.x, renderPos.y, L"����", 14, DefaultBrush::white,
				DWRITE_TEXT_ALIGNMENT_LEADING);
		}
		else if (_state == UnitState::Fight)
		{
			IMAGEMANAGER->drawText(renderPos.x, renderPos.y, L"����", 14, DefaultBrush::white,
				DWRITE_TEXT_ALIGNMENT_LEADING);
		}
		else if (_state == UnitState::Stop)
		{
			IMAGEMANAGER->drawText(renderPos.x, renderPos.y, L"���", 14, DefaultBrush::white,
				DWRITE_TEXT_ALIGNMENT_LEADING);
		}
		else if (_state == UnitState::BuildTown)
		{
			IMAGEMANAGER->drawText(renderPos.x, renderPos.y, L"�Ǽ�", 14, DefaultBrush::white,
				DWRITE_TEXT_ALIGNMENT_LEADING);
		}
		else if (_state == UnitState::Merge)
		{
			IMAGEMANAGER->drawText(renderPos.x, renderPos.y, L"����", 14, DefaultBrush::white,
				DWRITE_TEXT_ALIGNMENT_LEADING);
		}

	}
}

void unit::changeState(unitState* newstate)
{
	SAFE_DELETE(_unitState);
	_unitState = newstate;
	_unitState->enter(*this);
}


vector2D unit::getDirectionVector(UnitDirection::DIRECTION dir)
{
	vector2D direction;

	switch (dir)
	{
		case UnitDirection::UNIT_LEFT: direction = vector2D(-1, 0);
		break;
		case UnitDirection::UNIT_UP: direction = vector2D(0, -1);
		break;
		case UnitDirection::UNIT_RIGHT:	direction = vector2D(1, 0);
		break;
		case UnitDirection::UNIT_DOWN: direction = vector2D(0, 1);
		break;
	}
	return direction;
}

void unit::syncIndexFromPos()
{
	float zoom = CAMERA->getZoom();
	_index = tileMap::getTileIndexFromPos(vector2D((_pos.x+0.1f)*zoom, (_pos.y+0.1f)*zoom), _height);
}

void unit::requestRender()
{
	_image = setUnitLvImage(_hp);

	//������ ������ ��� �ٸ� ���� �ʿ����
	if (!_isMove)
	{
		WORLD->getMap()->getTile(_index.x, _index.y)->requestRender(this);
		return;
	}
	
	//�̵� ���� ��� ĳ���� �̹��� LEFT, TOP, RIGHT, BOTTOM �κ� �˻� ��
	//���� ������ Ÿ�Ͽ� �������� ��û�Ѵ�.
	float zoom = CAMERA->getZoom();
	vector2D frameSize = _image->getFrameSize(_imageFrameX);
	float imageHalfHeight = frameSize.y * zoom * 0.5f;
	vector2D zoomedPos = _pos * zoom;
	RECT rc = RectMakeCenter(zoomedPos.x, zoomedPos.y + imageHalfHeight - 10, frameSize.x*zoom, frameSize.y*zoom);

	vector2D top = vector2D(zoomedPos.x, (float)rc.top);
	vector2D right = vector2D((float)rc.right, zoomedPos.y);
	vector2D bottom = vector2D(zoomedPos.x, (float)rc.bottom);
	vector2D left = vector2D((float)rc.left, zoomedPos.y);

	vector2D indexList[4];
	indexList[0] = tileMap::getTileIndexFromPos(top, _height);
	indexList[1] = tileMap::getTileIndexFromPos(right, _height);
	indexList[2] = tileMap::getTileIndexFromPos(bottom, _height);
	indexList[3] = tileMap::getTileIndexFromPos(left, _height);

	vector2D maxIndex = indexList[0];
	for (int i = 1; i < 4; ++i)
	{
		if (maxIndex.x <= indexList[i].x && maxIndex.y <= indexList[i].y)
		{
			maxIndex = indexList[i];
		}
	}

	WORLD->getMap()->getTile(maxIndex.x, maxIndex.y)->requestRender(this);
}

image* unit::setUnitLvImage(int health)
{
	if (health < 1000)
		return  _lv[1];
	else if (health < 10000)
		return  _lv[2];
	else if (health >= 10000)
		return _lv[3];
	else
	{
		cout << "setUnitLvImage���� " << endl;
	}
}
void unit::setImage(image* lv1, image* lv2, image* lv3)
{
	_lv[1] = lv1;
	_lv[2] = lv2;
	_lv[3] = lv3;
}

bool unit::isMoveable(POINT index)
{
	//�������� �� �� ���� ���� ��� NoneState�� ����.
	vector2D tileCount = WORLD->getMap()->getTileCount();

	// - �����÷ο� üũ
	if (index.x < 0 || index.x >= tileCount.x || index.y < 0 || index.y >= tileCount.y)
	{
		return false;
	}

	// - ����, �̵��Ұ��� Ÿ�� üũ
	terrainTile* destTile = WORLD->getMap()->getTile(index.x, index.y);
	int distanceHeight = abs(destTile->getHeight(true) - _height);
	if (!destTile->isWalkable() || distanceHeight > 1)
	{
		return false;
	}

	// - �̵� �Ϸ��� Ÿ�Ͽ� �� ������ �ִ��� üũ
	vector<unit*> vr = destTile->getUnitOnTile();
	if (vr.size() >= 0)
	{
		for (int i = 0; i < vr.size(); ++i)
		{
			if (vr[i]->getCountryColor() != this->getCountryColor())
			{
				return false;
			}
		}
	}

	return true;
}

unit* unit::isCanRun()
{
	vector<unit*>searchedUnit;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			//����ó��
			if (i > 0 && i < 4 && j > 0 && j < 4) continue;
			if (j + _index.x - 2 < 0 || j + _index.x - 2 > WORLD->getMap()->getTileCount().x - 1) continue;
			if (i + _index.y - 2 < 0 || i + _index.y - 2 > WORLD->getMap()->getTileCount().y - 1) continue;

			//�ش� Ÿ�Ͽ� �ִ� ���ֺ���
			vector<unit*> unitOnTile = WORLD->getMap()->getTile(j - 2 + _index.x, i - 2 + _index.y)->getUnitOnTile();


			//Ÿ�Ͽ� ������ �ִٸ�
			if (unitOnTile.size() != NULL)
			{
				//Ÿ�Ͽ��ִ� ���ֺ��͸� ���� ������ ü���� ���ٸ� ����
				for (int k = 0; k <unitOnTile.size(); k++)
				{
					//�� �ڽŰ� ������ ���ٸ� ������� �ʴ´�.
					if (unitOnTile[k]->getCountryColor() == _unitColor) continue;

					//����ü�� + 200 �� Ÿ������ ������ ü�� ���� �������
					if (_hp < unitOnTile[k]->getHealth())
					{
						if (abs(unitOnTile[k]->getHealth() - _hp) > 200)
						{
							searchedUnit.push_back(unitOnTile[k]);
						}
					}
				}
			}
		}
	}

	//����Ž���Ͽ� ���Ϳ� �� ������Ƿ� ������� �ʴٸ� ������ ã�����̴�.
	//�׷��� �������� �Լ��� �����Ѵ�.
	if (searchedUnit.size() != NULL)
	{
		//ü���� ���� ���� ������ ã�Ƴ���
		int maximumHealth = 0;
		int maximumUnitIndex = 0;
		for (int k = 0; k < searchedUnit.size(); k++)
		{
			//ã�������� �̹������� �ִ�ü�º��� ���ٸ�
			if (searchedUnit[k]->getHealth() > maximumHealth)
			{
				//�ִ�ü�°� �ִ�ü���� ���� ������ �ε����� ����
				maximumHealth = searchedUnit[k]->getHealth();
				maximumUnitIndex = k;
			}
		}
	
		return searchedUnit[maximumUnitIndex];
	}
	else return NULL;
}



string unit::getColorString()
{
	string color;
	switch (_unitColor)
	{
	case CountryColor::RED:
		color = "red";
		break;
	case CountryColor::WHITE:
		color = "white";
		break;
	case CountryColor::BLUE:
		color = "blue";
		break;
	case CountryColor::GREEN:
		color = "green";
		break;
	}
	return color;
}
