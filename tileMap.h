#pragma once

#include "terrainTile.h"

#define MAX_TILEX 100
#define MAX_TILEY 100
#define MAX_HEIGHT 10

class tileMap
{
public:
	static vector2D			_tileSize;

private:
	string				_name;
	string				_background;
	POINT				_tileCount;
	vector2D			_totalMapSize;

	terrainTile*			_terrainTiles[MAX_TILEY][MAX_TILEX];

	//���� ������
	BYTE					_heightTable[MAX_TILEY * 2 + MAX_HEIGHT][MAX_TILEX * 2 + MAX_HEIGHT];

	image*				_backgroundImage;

	terrainTile*		_pickedTile;
	vector2D			_rbDownPos;

public:
	tileMap():_name(""),_background("") {}
	~tileMap() {}

	void init(string directory);
	void release();
	void update();
	void render();

	//��
	string getName() { return _name; }
	string getBackgroundKey() { return _background; }
	POINT getTileCount() { return _tileCount; }
	vector2D getMapSize() { return _totalMapSize; }

	//���� �� ���� �Լ�
	void updateHeightTable();

	//���� ����� Ÿ�� ũ�⸦ ��ȯ�Ѵ�.
	static vector2D getTileSize();
	//��ŷ ����
	static vector2D getTileIndexFromPos(vector2D pos, int height = 0);
	static vector2D getTilePosFromIndex(vector2D index, int height = 0);

	terrainTile* getTile(int x, int y) { return _terrainTiles[y][x]; }
	terrainTile* getPickedTile() { return _pickedTile; }
	
	terrainTile* getTileFromPos(vector2D pos);

private:
	static vector2D getHeightTableIndexFromPos(vector2D pos);
	static vector2D getHeightTablePosFromIndex(vector2D index);

	void load(string directory);

	static bool _isLoadedTile;
	static void loadTileResource();

	//���� ���콺 ��ġ�� �ִ� Ÿ���� ��ȯ�Ѵ�.
	terrainTile* getTileFromMousePos();
};