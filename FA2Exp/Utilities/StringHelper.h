#pragma once
#include <vector>
#include <sstream>

namespace utilities
{
	static std::vector<std::string> split_string(const char* pSource, char cSplit = ',')
	{
		std::vector<std::string> ret;
		std::istringstream iss(pSource);
		std::string tmp;

		while (std::getline(iss, tmp, cSplit)) {
			ret.push_back(std::move(tmp));
		}

		return ret;
	}

	static int to_int(const char* input, int def = -1)
	{
		int ret;
		if (sscanf_s(input, "%d", &ret) == 1) {
			return ret;
		}
		return def;
	}

	static void trim_string(CString& str)
	{
		str.TrimLeft();
		str.TrimRight();
	}

	static void trim_index(CString& str)
	{
		trim_string(str);
		int spaceIndex = str.Find(' ');
		if (spaceIndex > 0) {
			str = str.Mid(0, spaceIndex);
		}
	}

}