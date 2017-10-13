#include "stdafx.h"
#include "tileMap.h"
#include <io.h>

vector2D tileMap::_tileSize = vector2D(128, 64);
bool tileMap::_isLoadedTile = false;

void tileMap::init(string directory)
{
	//타일 리소스 로드
	loadTileResource();

	//맵 파일 로드
	load(directory);

	//높이 테이블 초기화
	updateHeightTable();

	_backgroundImage = IMAGEMANAGER->findImage(_background);
	_totalMapSize.x = _tileSize.x * _tileCount.x;
	_totalMapSize.y = _tileSize.y * _tileCount.y;
	_pickedTile = NULL;
	_rbDownPos = vector2D(0, 0);
}

void tileMap::release()
{
	//모든 타일 해제
	for (int i = 0; i < _tileCount.y; ++i)
	{
		for (int j = 0; j < _tileCount.x; ++j)
		{
			_terrainTiles[i][j]->release();
			delete _terrainTiles[i][j];
			_terrainTiles[i][j] = NULL;

			//if (_objectList[i][j])
			//{
			//	_objectList[i][j]->release();
			//	delete _objectList[i][j];
			//	_objectList[i][j] = NULL;
			//}
		}
	}
}

void tileMap::update()
{
	//마우스 피킹
	terrainTile* tile = this->getTileFromMousePos();
	if (_pickedTile)
		_pickedTile->setPicked(false);
	if (tile)
	{
		_pickedTile = tile;
		_pickedTile->setPicked(true);
	}

	//카메라 이동 구현
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		_rbDownPos = _ptMouse;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
	{
		CAMERA->setPos(CAMERA->getPos() + _rbDownPos - vector2D(_ptMouse));
		_rbDownPos = _ptMouse;
	}
}

void tileMap::render()
{
	//배경 렌더링
	_backgroundImage->setSizeOption(vector2D(WINSIZEX, WINSIZEY));
	_backgroundImage->render(0, 0, Pivot::LEFT_TOP, false);

	//타일 렌더링
	for (int i = 0; i < _tileCount.y; ++i)
	{
		for (int j = 0; j < _tileCount.x; ++j)
		{
			if (_terrainTiles[i][j])
				_terrainTiles[i][j]->render();
			//if (_objectList[i][j])
			//	_objectList[i][j]->render();
		}
	}

	//높이 테이블 렌더링
	if (_isDebugMode)
	{
		image* heightMap = IMAGEMANAGER->findImage("높이맵");
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

		//높이 테이블 피킹
		vector2D mousePos = CAMERA->getMousePos();
		vector2D index = this->getHeightTableIndexFromPos(mousePos);

		//높이 테이블에서 마우스 위치의 높이값을 얻어온다.
		int height = _heightTable[(int)index.y][(int)index.x];

		vector2D top = this->getHeightTablePosFromIndex(vector2D(index.x, index.y));
		vector2D left = this->getHeightTablePosFromIndex(vector2D(index.x, index.y + 1));
		vector2D right = this->getHeightTablePosFromIndex(vector2D(index.x + 1, index.y));
		vector2D bottom = this->getHeightTablePosFromIndex(vector2D(index.x + 1, index.y + 1));

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

	//돔트리 구조
	//루트 -> 헤더, 지형 테이블, 오브젝트 테이블, (레이어1, 레이어2, 레이어3), 오브젝트 이렇게 달려있음.

	//1. 헤더를 읽어온다.
	//맵 이름, 타일 사이즈, 타일의 XY개수, 총 타일 개수
	_name = d["헤더"]["mapName"].GetString();
	_tileSize.x = d["헤더"]["tileSizeX"].GetInt();
	_tileSize.y = d["헤더"]["tileSizeY"].GetInt();
	_tileCount.x = d["헤더"]["tileCntX"].GetInt();
	_tileCount.y = d["헤더"]["tileCntY"].GetInt();
	_background = d["헤더"]["background"].GetString();

	//2. 타일 속성 테이블 만들기
	vector<terrainTile>	_terrainAtt;
	for (auto iter = d["지형테이블"].Begin(); iter != d["지형테이블"].End(); ++iter)
	{
		terrainTile tile;
		Value obj(kObjectType);
		obj = *iter;
		tile.init(0, 0, obj["walkable"].GetBool(), obj["moveRatio"].GetFloat(), obj["key"].GetString());
		tile.setHeight(obj["height"].GetInt());
		_terrainAtt.emplace_back(tile);
	}

	//vector<mapObject>	_objAtt;
	//for (auto iter = d["오브젝트테이블"].Begin(); iter != d["오브젝트테이블"].End(); ++iter)
	//{
	//	mapObject mapObj;
	//	Value obj(kObjectType);
	//	obj = *iter;
	//	mapObj.init(PointMake(0, 0), obj["key"].GetString());
	//	_objAtt.emplace_back(mapObj);
	//}

	//3. 지형 정보를 읽어온다.
	int index = -1;
	for (int i = 0; i < _tileCount.y; ++i)
	{
		for (int j = 0; j < _tileCount.x; ++j)
		{
			index = d["지형"][i][j].GetInt();
			terrainTile* tile = NULL;
			if (index == -1)
			{
				//빈 지형타일 생성
				tile = _terrainTiles[i][j] = new terrainTile();
				tile->init(j, i, true, 1.0f);
			}
			else
			{
				//속성 테이블에서 복사 생성
				tile = _terrainTiles[i][j] = new terrainTile(_terrainAtt[index]);
				tile->setIndex(j, i);
				tile->setHeight(_terrainAtt[index].getHeight());
			}
		}
	}

	//4. 오브젝트 정보를 읽어온다.
	//for (int i = 0; i < _tileCnt.y; ++i)
	//{
	//	for (int j = 0; j < _tileCnt.x; ++j)
	//	{
	//		index = d["오브젝트"][i][j].GetInt();
	//		if (index != -1)
	//		{
	//			_objectList[i][j] = new mapObject(_objAtt[index]);
	//			_objectList[i][j]->setIndex(j, i);
	//		}
	//		else
	//		{
	//			_objectList[i][j] = NULL;
	//		}
	//	}
	//}
}

void tileMap::updateHeightTable()
{
	auto updateTable = [this](terrainTile* tile)
	{
		//1. screen좌표계에서 타일 위치를 구하기.
		vector2D screenPos = this->getTilePosFromIndex(tile->getIndex(), tile->getHeight());

		//2. screenPos를 기준으로 높이 테이블의 인덱스를 구하기.
		vector2D index = this->getHeightTableIndexFromPos(screenPos);

		//3. 높이 테이블 수정하기.
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
	//마우스 피킹 시 일단 해당 위치의 heightMap에서의 인덱스를 얻어서
	//	마우스가 있는 위치의 높이값을 구하고, 그걸 기준으로 타일을 찾는다.
	vector2D mousePos = CAMERA->getMousePos();
	vector2D index = getHeightTableIndexFromPos(mousePos);

	//높이 테이블에서 마우스 위치의 높이값을 얻어온다.
	int height = _heightTable[(int)index.y][(int)index.x];

	//마우스 위치의 타일을 찾는다.
	vector2D tileIndex = this->getTileIndexFromPos(mousePos, height);
	if (tileIndex.x < 0 || tileIndex.y < 0 || tileIndex.x >= _tileCount.x || tileIndex.y >= _tileCount.y)
		return NULL;

	terrainTile* tile = _terrainTiles[(int)tileIndex.y][(int)tileIndex.x];

	//타일의 높이값 검증
	if (tile->getHeight() != height)
		return NULL;

	return tile;
}

vector2D tileMap::getHeightTableIndexFromPos(vector2D pos)
{
	vector2D index;
	vector2D tileSize = getTileSize();

	//기존 타일을 4등분한 높이테이블의 인덱스를 구한다.
	index.x = floor(((pos.x / (tileSize.x*0.25)) + (pos.y / (tileSize.y*0.25))) * 0.5 + 0.05f);
	index.y = floor(((pos.y / (tileSize.y*0.25)) - (pos.x / (tileSize.x*0.25))) * 0.5 + 0.05f);

	//높이 테이블 index 보정
	index.x += MAX_HEIGHT;
	index.y += MAX_HEIGHT;

	return index;
}

vector2D tileMap::getHeightTablePosFromIndex(vector2D index)
{
	vector2D pos;
	vector2D tileSize = getTileSize();

	//높이 테이블 index 보정
	index.x -= MAX_HEIGHT;
	index.y -= MAX_HEIGHT;

	pos.x = (tileSize.x*0.25f)*(index.x - index.y);
	pos.y = (tileSize.y*0.25f)*(index.x + index.y);

	return pos;
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
	}
	_isLoadedTile = true;
}