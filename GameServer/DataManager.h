#pragma once

/*
	Game Data °ü¸®
*/

class DataManager
{
public:
	static void LoadData();

private:
	static void LoadMapData();

};

extern DataManager* GDataManager;

