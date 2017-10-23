#include "stdafx.h"
#include "world.h"
#include "unit.h"
#include "mncObjectBase.h"

void world::init()
{
	_tileMap = NULL;
	for (int i = 0; i < CountryColor::END; ++i)
	{
		_country[i] = NULL;
		_country[i] = new country;
		_country[i]->init(CountryColor::Enum(i));
	}
}

void world::update()
{
	//타일맵 업데이트
	if (_tileMap)
		_tileMap->update();

	//유닛 업데이트
	for (int i = 0; i < CountryColor::END; ++i)
	{
		if(_country[i])
			_country[i]->update();
	}

	//오브젝트 업데이트
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		for (unsigned int j = 0; j < _objectList[i].size(); ++j)
		{	
			gameObject* object = _objectList[i][j];
			if (object->isLive())
			{
				if (object->isActiveObject())
				{
					object->update();
				}			
			}
			else
			{
				object->release();
				delete object;
				object->setDestroy();
				_objectList[i].erase(_objectList[i].begin() + j);
				--j;
			}
		}
	}

	//테스트
	//terrainTile* tile = WORLD->getMap()->getPickedTile();
	//if (tile)
	//{
		//gameObject* obj = tile->getObjectOnTile();
		//if (obj)
		//{
		//	if (obj->_name == "마을" || obj->_name == "성문")
		//	{
		//		((mncObjectBase*)obj)->addHp(-100);
		//	}
		//}
	//}
}

void world::render()
{
	//타일 + 오브젝트 + 유닛 렌더링
	if (_tileMap)
	{
		_tileMap->render();
	}
}

void world::addObject(gameObject* obj, int renderPriority)
{
	_objectList[renderPriority].push_back(obj);
}

gameObject* world::findObject(string name)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		for (unsigned int j = 0; j < _objectList[i].size(); ++j)
		{
			if (_objectList[i][j]->isLive())
			{
				if (_objectList[i][j]->_name == name)
				{
					return _objectList[i][j];
				}
			}
		}
	}
	return nullptr;
}

vector<gameObject*> world::findObjects(string name)
{
	vector<gameObject*> result;
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		unsigned int size = _objectList[i].size();
		for (unsigned int j = 0; j < size; ++j)
		{
			if (_objectList[i][j]->isLive())
			{
				if (_objectList[i][j]->_name == name)
				{
					result.push_back(_objectList[i][j]);
				}
			}
		}
	}
	return std::move(result);
}

vector<gameObject*> world::findObjects(string name, int priorityNum)
{
	if (name == "NONE")
	{
		return std::move(_objectList[priorityNum]);
	}

	vector<gameObject*> result;
	unsigned int size = _objectList[priorityNum].size();
	for (unsigned int i = 0; i < size; ++i)
	{
		if (_objectList[priorityNum][i]->isLive())
		{
			if (_objectList[priorityNum][i]->_name == name)
			{
				result.push_back(_objectList[priorityNum][i]);
			}
		}
	}
	return std::move(result);
}

void world::addUnit(unit * newUnit, CountryColor::Enum countryColor)
{
	_country[countryColor]->addUnit(newUnit);
}

country * world::getCountry(CountryColor::Enum color)
{
	return _country[color];
}

void world::destroyCountryBuilding(CountryColor::Enum color)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		for (unsigned int j = 0; j < _objectList[i].size(); ++j)
		{
			gameObject* object = _objectList[i][j];
			if (object->isLive())
			{
				if (object->_name == "마을" || object->_name == "농장" || object->_name == "군주")
				{
					if(((mncObjectBase*)object)->getCountryColor() == color)
						object->setDestroy();
				}
			}
		}
	}
}

void world::release()
{
	//오브젝트 해제
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		for (int j = (int)_objectList[i].size() - 1; j >= 0; --j)
		{
			_objectList[i][j]->release();
			SAFE_DELETE(_objectList[i][j]);
			_objectList[i].pop_back();
		}
	}

	//국가 해제
	for (int i = 0; i < CountryColor::END; ++i)
	{
		_country[i]->release();
		delete _country[i];
		_country[i] = NULL;
	}


	//타일맵 해제
	releaseTiles();
}



void world::releaseTiles()
{
	_tileMap->release();
	delete _tileMap;
	_tileMap = NULL;
}

void world::loadTiles(string directory)
{
	if (_tileMap)
		releaseTiles();
	_tileMap = new tileMap;
	_tileMap->init(directory);
}

vector2D world::getMapSize()
{
	if (!_tileMap) return vector2D(0, 0);
	return _tileMap->getMapSize();
}

POINT world::getTileCount()
{
	if (!_tileMap) return PointMake(0, 0);
	return _tileMap->getTileCount();
}

bool world::isPossibleMove(int xIndex, int yIndex)
{
	if (!_tileMap) return false;
	return _tileMap->getTile(xIndex,yIndex)->isWalkable();
}

float world::getTileMoveRatio(int xIndex, int yIndex)
{
	if (!_tileMap) return 0.0f;
	return _tileMap->getTile(xIndex, yIndex)->getMoveRatio();
}
