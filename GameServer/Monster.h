#pragma once
#include "Creature.h"

class Monster : public Creature
{
public:
	enum { ClassId = 'MONS' };
	virtual uint32 GetClassId() const { return ClassId; }
	static MonsterRef CreateInstance() { return MakeShared<Monster>(); }

};

