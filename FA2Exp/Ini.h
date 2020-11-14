#ifndef __FA2COPY_INI__
#define __FA2COPY_INI__

#pragma region Includes

//#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>

#pragma endregion

#pragma region Classes

class TeamTemplate {
private:
	std::string Data[27];
public:
	TeamTemplate();
	TeamTemplate(std::vector<std::string> init);
	std::string* operator[](int index);
};
class ScriptTemplate {
private:
	std::vector<std::pair<std::string, std::string>> Data;
public:
	ScriptTemplate();
	ScriptTemplate(std::vector<std::string> init);
	std::pair<std::string, std::string>* operator[](int index);
	int Count();
	void Resize(int size);
};

class Ini
{
public:
	static Ini ConfigIni; // Ini Config file

	Ini();
	Ini(std::string path);
	bool Write(std::string section, std::string key, std::string value);
	std::string Read(std::string section, std::string key);
	bool Exist();
	std::vector<std::string> Split(std::string str, char ch);
	std::string& Trim(std::string& s);


private:
	std::string Path;
};
#pragma endregion

#endif// __FA2COPY_INI_