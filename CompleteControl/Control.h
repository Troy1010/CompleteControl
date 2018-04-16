#pragma once


class Control
{
private:
	int _x;
public:
	Control(void);
	~Control(void);

	PROPERTY(int,x);
	GET(x)
	{
		return _x;
	}
	SET(x)
	{
		_x = value;
	}


};

