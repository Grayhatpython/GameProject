#pragma once
#include "Creature.h"

class Room;
class ClientSession;

class Player : public Creature
{
public:
	Player();
	virtual ~Player();

public:
	enum { ClassId = 'PLAY' };
	virtual uint32 GetClassId() const { return ClassId; }
	static PlayerRef CreateInstance() { return MakeShared<Player>(); }

public:
	ClientSessionRef			_ownerSession;
};

