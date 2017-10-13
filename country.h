#pragma once


namespace CountryColor
{
	enum Enum
	{
		RED, WHITE, BLUE, GREEN, END
	};
}

class country
{
private:
	CountryColor::Enum			_countryColor;
	

public:
	country() {}
	~country() {}

	void init();
	void release();
	void update();
	void render();
};