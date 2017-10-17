#pragma once

class unit;

class terrainTile : public gameObject
{
private:
	bool		_walkable;
	float		_moveRatio;

	string		_imageKey;

	POINT		_index;
	int			_height;

	bool		_isPicked;

	//자기 타일 위에 있는 유닛
	vector<unit*>	_onUnitList;
	//자기 타일 위에 있는 오브젝트
	gameObject*		_onObject;

	//유닛 Z오더 전용 벡터
	vector<unit*>	_renderUnitList;
	
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

	vector<unit*> getUnitOnTile();
	void deleteUnitOnTile(unit* onUnit);
	void addUnitOnTile(unit* onUnit);

	void setObjectOnTile(gameObject* obj);
	void removeObjectOnTile();
	gameObject* getObjectOnTile();

	void requestRender(unit* onUnit);
	
};