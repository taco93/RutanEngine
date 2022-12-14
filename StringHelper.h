#pragma once
#include <string>
class StringHelper
{
public:
	static std::wstring StringToWide(std::string str);
	static std::string WideToString(std::wstring str);
	static std::string GetDirectoryFromPath(const std::string& filepath);
	static std::string GetFileExtension(const std::string& filepath);
	static std::string GetFileName(const std::string& filepath);
};