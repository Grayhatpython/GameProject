#pragma once


//	TEST
class Player
{
public:
	Player();
	~Player();

public:
	uint32					_id = 0;
	Protocol::PositionInfo* _positionInfo = nullptr;
};

