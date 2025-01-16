#include "pch.h"
#include "Player.h"

Player::Player()
{
	//	TODO : 메모리풀
	_positionInfo = new Protocol::PositionInfo();
}

Player::~Player()
{
	delete _positionInfo;
}