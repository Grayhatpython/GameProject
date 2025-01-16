#include "pch.h"
#include "DataManager.h"
#include <fstream>
#include <filesystem>

DataManager* GDataManager = nullptr;

void DataManager::LoadData()
{
	LoadMapData();
}

void DataManager::LoadMapData()
{
	string filePath;

	//	TEMP
#ifdef _DEBUG
	filePath = "../Common/Data/Map/map.txt";
#else
	filePath = "../../Common/Data/Map/map.txt";
#endif // _DEBUG

	std::ifstream mapFile(filePath);
	
#ifdef _DEBUG
	ASSERT_CRASH(mapFile.is_open());
#else
	try
	{
		if (mapFile.is_open() == false)
			throw std::runtime_error("Map File Open Error.");
	}
	catch (const std::exception& e)
	{
		//	TEMP
	}
#endif // _DEBUG

	std::string line;

	mapFile.close();
}
