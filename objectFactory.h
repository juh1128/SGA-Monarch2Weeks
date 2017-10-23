#pragma once



class objectFactory
{
private:
	//���� �̹��� (����, ȭ��Ʈ, ���, �׸�) �� 3����
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