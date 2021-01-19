#pragma once

#include <CTileSetDialogBar.h>
#include <vector>
#include <string>

class TerrainSort {
private:
	std::vector<std::pair<std::string, std::string>> Data;
	std::string Name;
public:
	TerrainSort();
	TerrainSort(std::vector<std::string> init);
	std::pair<std::string, std::string>* operator[](int index);
	int Count();
	void Resize(int size);
	std::string GetName();
};

class NOVTABLE CTileSetDialogBarExt : public CTileSetDialogBar
{
public:
	static CTileSetDialogBar* Instance;
	static std::vector<TerrainSort> TerrainSorts;

	static void LoadTerrainGroups(std::string Theater);
	static void ReloadTileSets(const std::string& Theater);
	static void OnGroupComboboxSelectionChanged();
	static void OnSubTypeComboboxSelectionChanged();

	void ProgramStarupInit();


};