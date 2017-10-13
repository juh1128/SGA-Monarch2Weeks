#include "stdafx.h"
#include "world.h"

void world::init()
{
	_tileMap = NULL;
}

void world::update()
{
	if (_tileMap)
		_tileMap->update();

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

}

void world::render()
{
	//≈∏¿œ∏  ∑ª¥ı∏µ
	if(_tileMap)
		_tileMap->render();

	//ø¿∫Í¡ß∆Æ ∑ª¥ı∏µ
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		unsigned int size = _objectList[i].size();
		for (unsigned int j = 0; j<size; ++j)
		{
			gameObject* object = _objectList[i][j];
			if (object->isActiveObject())
			{
				object->render();
			}
		}
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

void world::release()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		for (int j = (int)_objectList[i].size() - 1; j >= 0; --j)
		{
			_objectList[i][j]->release();
			SAFE_DELETE(_objectList[i][j]);
			_objectList[i].pop_back();
		}
	}
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
