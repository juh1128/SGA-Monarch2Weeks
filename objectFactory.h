#pragma once



class objectFactory
{
private:
	//유닛 이미지 (레드, 화이트, 블루, 그린) 각 3개씩
	static image* _unitImage[12];

public:
	objectFactory() {}
	virtual ~objectFactory() {}

	static void initUnitImage();
	static image* getUnitImage(CountryColor::Enum color, int lv);
	gameObject*	createObject(int xIndex, int yIndex, string name);

private:
	gameObject* createCastle(int xIndex, int yIndex,string name);
};