#include "stdafx.h"
#include "tileMap.h"
#include <io.h>
#include "objectFactory.h"
#include "unit.h"
#include "mncObjectBase.h"

vector2D tileMap::_tileSize = vector2D(128, 64);
bool tileMap::_isLoadedTile = false;

void tileMap::init(string directory)
{
	ZeroMemory(_terrainTiles, sizeof(_terrainTiles));
	//Ÿ�� ���ҽ� �ε�
	loadTileResource();

	//�� ���� �ε�
	load(directory);

	//���� ���̺� �ʱ�ȭ
	updateHeightTable();

	_backgroundImage = IMAGEMANAGER->findImage(_background);
	_totalMapSize.x = _tileSize.x * _tileCount.x;
	_totalMapSize.y = _tileSize.y * _tileCount.y;
	_pickedTile = NULL;
}

void tileMap::release()
{
	//��� Ÿ�� ����
	for (int i = 0; i < _tileCount.y; ++i)
	{
		for (int j = 0; j < _tileCount.x; ++j)
		{
			_terrainTiles[i][j]->release();
			delete _terrainTiles[i][j];
			_terrainTiles[i][j] = NULL;
		}
	}
}

void tileMap::update()
{
}

void tileMap::render()
{
	//��� ������
	_backgroundImage->setSizeOption(vector2D(WINSIZEX, WINSIZEY));
	_backgroundImage->render(0, 0, Pivot::LEFT_TOP, false);

	bool isStopedWorld = SCENEMANAGER->getNowScene()->isStopedWorld();
	for (int i = 0; i < _tileCount.y; ++i)
	{
		for (int j = 0; j < _tileCount.x; ++j)
		{
			if (_terrainTiles[i][j])
				_terrainTiles[i][j]->render(isStopedWorld);
		}
	}

	//��ŷ ������Ʈ ���� ǥ��
	if (_pickedTile)
	{
		vector2D tilePos = CAMERA->getRelativeVector2D(_pickedTile->_pos);
		tilePos.y -= _pickedTile->getHeight()*this->getTileSize().y*0.5f;
		tilePos.y += this->getTileSize().y - 10;
		//������ ���� ��� ���� ���� ǥ��
		vector<unit*> unitList = _pickedTile->getUnitOnTile();
		if (unitList.size() > 0)
		{
			char buf[128] = "";
			wsprintf(buf, "[%s] DF %d", unitList[0]->_name.c_str(), unitList[0]->getHealth()); 
			IMAGEMANAGER->fillRectangle(RectMakeCenter(tilePos.x + 12, tilePos.y + 16, strlen(buf) * 10, 24), D2D1::ColorF::FloralWhite, 0.7f);
			IMAGEMANAGER->drawText(tilePos.x - strlen(buf) * 9, tilePos.y, UTIL::string_to_wstring(buf), 20, DefaultBrush::black);
		}
		//������Ʈ�� ���� ��� ������Ʈ�� ���� ǥ��
		else if (_pickedTile->getObjectOnTile())
		{
			char buf[128] = "";
			mncObjectBase* obj = (mncObjectBase*)_pickedTile->getObjectOnTile();
			if (obj->_name != "��")
			{
				wsprintf(buf, "[%s] DF %d", obj->_name.c_str(), obj->getHp());
				IMAGEMANAGER->fillRectangle(RectMake(tilePos.x - strlen(buf) * 5 - 8, tilePos.y + 4, strlen(buf) * 10, 24), D2D1::ColorF::FloralWhite, 0.7f);
				IMAGEMANAGER->drawText(tilePos.x - strlen(buf) * 9, tilePos.y, UTIL::string_to_wstring(buf), 20, DefaultBrush::black);
			}
		}
	}

	//���� ���̺� ������
	if (_isDebugMode)
	{
		image* heightMap = IMAGEMANAGER->findImage("���̸�");
		vector2D pos;
		float zoom = CAMERA->getZoom();
		for (int i = 0; i < _tileCount.y * 2 + MAX_HEIGHT; ++i)
		{
			for (int j = 0; j < _tileCount.x * 2 + MAX_HEIGHT; ++j)
			{
				pos = this->getHeightTablePosFromIndex(vector2D(j, i));
				heightMap->setScaleOption(vector2D(0.5f*zoom, 0.5f*zoom));
				heightMap->frameRender(pos.x, pos.y, (int)_heightTable[i][j], 0, Pivot::TOP);
			}
		}

		//���� ���̺� ��ŷ
		vector2D mousePos = CAMERA->getMousePos();
		vector2D index = this->getHeightTableIndexFromPos(mousePos);

		//���� ���̺��� ���콺 ��ġ�� ���̰��� ���´�.
		int height = _heightTable[(int)index.y][(int)index.x];

		vector2D top = this->getHeightTablePosFromIndex(vector2D(index.x, index.y));
		vector2D left = this->getHeightTablePosFromIndex(vector2D(index.x, index.y + 1));
		vector2D right = this->getHeightTablePosFromIndex(vector2D(index.x + 1, index.y));
		vector2D bottom = this->getHeightTablePosFromIndex(vector2D(index.x + 1, index.y + 1));

		top = CAMERA->getRelativeVector2D(top);
		left = CAMERA->getRelativeVector2D(left);
		right = CAMERA->getRelativeVector2D(right);
		bottom = CAMERA->getRelativeVector2D(bottom);

		IMAGEMANAGER->drawLine(top, left, DefaultBrush::yello);
		IMAGEMANAGER->drawLine(left, bottom, DefaultBrush::yello);
		IMAGEMANAGER->drawLine(bottom, right, DefaultBrush::yello);
		IMAGEMANAGER->drawLine(right, top, DefaultBrush::yello);
	}
}

void tileMap::load(string directory)
{
	Json json;
	json.read(directory);
	Document& d = json.document();

	//��Ʈ�� ����
	//��Ʈ -> ���, ���� ���̺�, ������Ʈ ���̺�, (���̾�1, ���̾�2, ���̾�3), ������Ʈ �̷��� �޷�����.

	//1. ����� �о�´�.
	//�� �̸�, Ÿ�� ������, Ÿ���� XY����, �� Ÿ�� ����
	_name = d["���"]["mapName"].GetString();
	_tileSize.x = d["���"]["tileSizeX"].GetInt();
	_tileSize.y = d["���"]["tileSizeY"].GetInt();
	_tileCount.x = d["���"]["tileCntX"].GetInt();
	_tileCount.y = d["���"]["tileCntY"].GetInt();
	_background = d["���"]["background"].GetString();

	//2. Ÿ�� �Ӽ� ���̺� �����
	vector<terrainTile>	_terrainAtt;
	for (auto iter = d["�������̺�"].Begin(); iter != d["�������̺�"].End(); ++iter)
	{
		terrainTile tile;
		Value obj(kObjectType);
		obj = *iter;
		tile.init(0, 0, obj["walkable"].GetBool(), obj["moveRatio"].GetFloat(), obj["key"].GetString());
		tile.setHeight(obj["height"].GetInt());
		_terrainAtt.emplace_back(tile);
	}

	struct objectInfo
	{
		POINT index;
		string key;
	};
	vector<objectInfo>	_objAtt;
	for (auto iter = d["������Ʈ���̺�"].Begin(); iter != d["������Ʈ���̺�"].End(); ++iter)
	{
		objectInfo mapObj;
		Value obj(kObjectType);
		obj = *iter;

		mapObj.index = PointMake(0, 0);
		mapObj.key = obj["key"].GetString();
		mapObj.key = mapObj.key.substr(0, mapObj.key.length() - 4);

		_objAtt.emplace_back(mapObj);
	}

	//3. ���� ������ �о�´�.
	int index = -1;
	for (int i = 0; i < _tileCount.y; ++i)
	{
		for (int j = 0; j < _tileCount.x; ++j)
		{
			index = d["����"][i][j].GetInt();
			terrainTile* tile = NULL;
			if (index == -1)
			{
				//�� ����Ÿ�� ����
				tile = _terrainTiles[i][j] = new terrainTile();
				tile->init(j, i, true, 1.0f);
			}
			else
			{
				//�Ӽ� ���̺��� ���� ����
				tile = _terrainTiles[i][j] = new terrainTile(_terrainAtt[index]);
				tile->setIndex(j, i);
				tile->setHeight(_terrainAtt[index].getHeight());
			}
		}
	}

	//4. ������Ʈ ������ �о�´�.
	objectFactory objFactory;
	for (int i = 0; i < _tileCount.y; ++i)
	{
		for (int j = 0; j < _tileCount.x; ++j)
		{
			index = d["������Ʈ"][i][j].GetInt();
			if (index != -1)
			{
				gameObject* newObject = objFactory.createObject(j, i, _objAtt[index].key);
				if(newObject)
					WORLD->addObject(newObject);
			}
		}
	}
}

void tileMap::updateHeightTable()
{
	auto updateTable = [this](terrainTile* tile)
	{
		//1. screen��ǥ�迡�� Ÿ�� ��ġ�� ���ϱ�.
		vector2D screenPos = this->getTilePosFromIndex(tile->getIndex(), tile->getHeight());

		//2. screenPos�� �������� ���� ���̺��� �ε����� ���ϱ�.
		vector2D index = this->getHeightTableIndexFromPos(screenPos);

		//3. ���� ���̺� �����ϱ�.
		int height = tile->getHeight();
		_heightTable[(int)index.y][(int)index.x] = height;
		_heightTable[(int)index.y][(int)index.x + 1] = height;
		_heightTable[(int)index.y + 1][(int)index.x] = height;
		_heightTable[(int)index.y + 1][(int)index.x + 1] = height;
	};

	ZeroMemory(_heightTable, sizeof(_heightTable));
	for (int i = 0; i < _tileCount.y; ++i)
	{
		for (int j = 0; j < _tileCount.x; ++j)
		{
			updateTable(_terrainTiles[i][j]);
		}
	}
}

vector2D tileMap::getTileSize()
{
	return _tileSize*CAMERA->getZoom();
}

vector2D tileMap::getTileIndexFromPos(vector2D pos, int height)
{
	vector2D index;
	vector2D tileSize = getTileSize();

	pos.y += height*(tileSize.y*0.5f);

	index.x = floor(((pos.x / (tileSize.x*0.5f)) + (pos.y / (tileSize.y*0.5f))) * 0.5f);
	index.y = floor(((pos.y / (tileSize.y*0.5f)) - (pos.x / (tileSize.x*0.5f))) * 0.5f);

	return index;
}

vector2D tileMap::getTilePosFromIndex(vector2D index, int height)
{
	vector2D pos;
	vector2D tileSize = getTileSize();

	pos.x = (tileSize.x*0.5f)*(index.x - index.y);
	pos.y = (tileSize.y*0.5f)*(index.x + index.y) - (height*(tileSize.y*0.5f));

	return pos;
}

terrainTile * tileMap::getTileFromMousePos()
{
	//���콺 ��ŷ �� �ϴ� �ش� ��ġ�� heightMap������ �ε����� ��
	//	���콺�� �ִ� ��ġ�� ���̰��� ���ϰ�, �װ� �������� Ÿ���� ã�´�.
	vector2D mousePos = CAMERA->getMousePos();
	vector2D index = getHeightTableIndexFromPos(mousePos);

	//���� ���̺��� ���콺 ��ġ�� ���̰��� ���´�.
	int height = _heightTable[(int)index.y][(int)index.x];

	//���콺 ��ġ�� Ÿ���� ã�´�.
	vector2D tileIndex = this->getTileIndexFromPos(mousePos, height);
	if (tileIndex.x < 0 || tileIndex.y < 0 || tileIndex.x >= _tileCount.x || tileIndex.y >= _tileCount.y)
		return NULL;

	terrainTile* tile = _terrainTiles[(int)tileIndex.y][(int)tileIndex.x];

	//Ÿ���� ���̰� ����
	if (tile->getHeight() != height)
		return NULL;

	return tile;
}

vector2D tileMap::getHeightTableIndexFromPos(vector2D pos)
{
	vector2D index;
	vector2D tileSize = getTileSize();

	//���� Ÿ���� 4����� �������̺��� �ε����� ���Ѵ�.
	index.x = floor(((pos.x / (tileSize.x*0.25)) + (pos.y / (tileSize.y*0.25))) * 0.5 + 0.05f);
	index.y = floor(((pos.y / (tileSize.y*0.25)) - (pos.x / (tileSize.x*0.25))) * 0.5 + 0.05f);

	//���� ���̺� index ����
	index.x += MAX_HEIGHT;
	index.y += MAX_HEIGHT;

	return index;
}

vector2D tileMap::getHeightTablePosFromIndex(vector2D index)
{
	vector2D pos;
	vector2D tileSize = getTileSize();

	//���� ���̺� index ����
	index.x -= MAX_HEIGHT;
	index.y -= MAX_HEIGHT;

	pos.x = (tileSize.x*0.25f)*(index.x - index.y);
	pos.y = (tileSize.y*0.25f)*(index.x + index.y);

	return pos;
}

void tileMap::get8Tiles(terrainTile** array8, int xIndex, int yIndex)
{
	int i = 0;
	for (int y = -1; y <= 1; ++y)
	{
		for (int x = -1; x <= 1; ++x)
		{		
			if (x == 0 && y == 0)
			{
				continue;
			}
			if (xIndex + x < 0 || yIndex + y < 0 || xIndex + x >= _tileCount.x || yIndex + y >= _tileCount.y)
			{
				array8[i++] = NULL;
				continue;
			}
			array8[i++] = _terrainTiles[y + yIndex][x + xIndex];
		}
	}
}

void tileMap::setMousePickTile()
{
	//���콺 ��ŷ
	terrainTile* tile = this->getTileFromMousePos();
	if (_pickedTile)
		_pickedTile->setPicked(false);
	if (tile)
	{
		_pickedTile = tile;
		_pickedTile->setPicked(true);
	}
}


void tileMap::loadTileResource()
{
	if (!_isLoadedTile)
	{
		char* folders[] = { "background", "dirt", "row", "grass", "stone", "wood", "dessert" , "snow", "brick", "water" };
		for (int i = 0; i < 10; ++i)
		{
			char path[128] = "";
			wsprintf(path, "resource/tiles/%s/*.png", folders[i]);

			struct _finddata_t fd;
			intptr_t handle;
			if ((handle = _findfirst(path, &fd)) == -1L)
			{
				continue;
			}

			do
			{
				wstring directory = L"resource/tiles/" + UTIL::string_to_wstring(folders[i]) + L'/' + UTIL::string_to_wstring(fd.name);
				IMAGEMANAGER->addImage(fd.name, directory);
			} while (_findnext(handle, &fd) == 0);
			_findclose(handle);
		}
		
		IMAGEMANAGER->addFrameImage("���̸�", L"resource/tiles/heightMap.png", 10, 1);
	}

	_isLoadedTile = true;
}