#include "stdafx.h"
#include "terrainTile.h"
#include "unit.h"

HRESULT terrainTile::init(int xIndex, int yIndex, bool walkable, float moveRatio, string key)
{
	gameObject::init("지형타일", key, vector2D(0,0), Pivot::TOP);

	_walkable = walkable;
	_moveRatio = moveRatio;

	setIndex(xIndex, yIndex);

	_isPicked = false; 
	_onUnit = NULL;

	return S_OK;
}

void terrainTile::setIndex(int x, int y)
{
	_index.x = x;
	_index.y = y;
}

vector2D terrainTile::getCenterPos()
{
	return std::move(vector2D(_pos.x, _pos.y + (tileMap::getTileSize().y*0.5f)));
}

vector<unit*> terrainTile::getUnitOnTile()
{
	return _onUnitList; 
}

void terrainTile::deleteUnitOnTile(unit * onUnit)
{
	for (size_t i = 0; i < _onUnitList.size(); ++i)
	{
		if (onUnit == _onUnitList[i])
		{
			_onUnitList.erase(_onUnitList.begin() + i);
			break;
		}
	}
}

void terrainTile::addUnitOnTile(unit * onUnit)
{
	_onUnitList.push_back(onUnit);
}


void terrainTile::release()
{
	gameObject::release();
}

void terrainTile::update()
{
}

void terrainTile::render()
{
	//타일 렌더링
	if (_image)
	{
		_pos = tileMap::getTilePosFromIndex(vector2D(_index.x, _index.y), 0);
		vector2D renderPosition = _pos;
		//128, 64 규격
		_image->setScaleOption(vector2D(CAMERA->getZoom(), CAMERA->getZoom()));
		_image->render(renderPosition.x, renderPosition.y, Pivot::TOP);

		//높이가 있는 타일일 경우
		if (_height > 0)
		{
			float heightUnit = tileMap::getTileSize().y * 0.5f;
			int tileLength = (int)((_image->getHeight() - tileMap::_tileSize.y) / (tileMap::_tileSize.y*0.5f));
			int cnt = (int)(((_height - 1) * heightUnit) / (heightUnit*tileLength)) - 1;

			//높이만큼 깔아주는 타일
			for (int i = cnt; i >= 0; --i)
			{
				_image->setScaleOption(vector2D(CAMERA->getZoom(), CAMERA->getZoom()));
				_image->render(renderPosition.x,
					(renderPosition.y - _height * heightUnit) + ((i + 1)*(heightUnit*tileLength)),
					Pivot::TOP);
			}
			//가장 위에 있을 타일
			_image->setScaleOption(vector2D(CAMERA->getZoom(), CAMERA->getZoom()));
			_image->render(renderPosition.x, renderPosition.y - (_height * heightUnit), Pivot::TOP);
		}
	}



	//마우스 오버 시
	if (_isPicked)
	{
		vector2D top = tileMap::getTilePosFromIndex(vector2D(_index.x, _index.y), _height);
		vector2D left = tileMap::getTilePosFromIndex(vector2D(_index.x, _index.y + 1), _height);
		vector2D right = tileMap::getTilePosFromIndex(vector2D(_index.x + 1, _index.y), _height);
		vector2D bottom = tileMap::getTilePosFromIndex(vector2D(_index.x + 1, _index.y + 1), _height);

		top = CAMERA->getRelativeVector2D(top);
		left = CAMERA->getRelativeVector2D(left);
		right = CAMERA->getRelativeVector2D(right);
		bottom = CAMERA->getRelativeVector2D(bottom);

		IMAGEMANAGER->drawLine(top, left, DefaultBrush::red, 2);
		IMAGEMANAGER->drawLine(left, bottom, DefaultBrush::red, 2);
		IMAGEMANAGER->drawLine(bottom, right, DefaultBrush::red, 2);
		IMAGEMANAGER->drawLine(right, top, DefaultBrush::red, 2);
	}
}
