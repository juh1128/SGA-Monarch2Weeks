#pragma once


class terrainTile : public gameObject
{
private:
	bool		_walkable;
	float		_moveRatio;

	string		_imageKey;

	POINT		_index;
	int			_height;

	bool		_isPicked;
	
public:
	terrainTile() {}
	virtual ~terrainTile() {}

	virtual HRESULT init(int xIndex, int yIndex, bool walkable, float moveRatio, string key = "");
	virtual void release();
	virtual void update();
	virtual void render();

	bool isWalkable() { return _walkable; }
	float getMoveRatio() { return _moveRatio; }
	float getHeight() { return _height; }
	void setHeight(int set) { _height = set; }
	void setIndex(int x, int y);
	POINT getIndex() { return _index; }

	vector2D getCenterPos();

	void setPicked(bool pick) { _isPicked = pick; }
	bool isPicked() { return _isPicked; }
};