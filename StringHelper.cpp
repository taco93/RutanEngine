#include "StringHelper.h"
#include <algorithm>
#include <iterator>

std::wstring StringHelper::StringToWide(std::string str)
{
	std::wstring wide_string(str.length(), 0);

	std::transform(str.begin(), str.end(), wide_string.begin(), [] (char c) {
		return (wchar_t)c;
		});

	return wide_string;
}

std::string StringHelper::WideToString(std::wstring wide)
{
	std::string str(wide.length(), 0);

	std::transform(wide.begin(), wide.end(), str.begin(), [](wchar_t c) {
		return (char)c;
		});

	return str;
}

std::string StringHelper::GetDirectoryFromPath(const std::string& filepath)
{
	size_t off1 = filepath.find_last_of('\\');
	size_t off2 = filepath.find_last_of('/');

	if (off1 == std::string::npos && off2 == std::string::npos) {
		return "";
	}

	if (off1 == std::string::npos) {
		return filepath.substr(0, off2 );
	}
	if (off2 == std::string::npos) {
		return filepath.substr(0, off1);
	}

	return filepath.substr(0, std::max(off1, off2));
}

std::string StringHelper::GetFileExtension(const std::string& filepath)
{
	size_t off = filepath.find_last_of('.');

	if (off == std::string::npos) {
		return {};
	}

	return std::string(filepath.substr(off + 1));
}

std::string StringHelper::GetFileName(const std::string& filepath)
{
	size_t off1 = filepath.find_last_of('/');
	size_t off2 = filepath.find_last_of('\\');

	if (off1 == std::string::npos && off2 == std::string::npos) {
		return "";
	}

	if (off1 == std::string::npos) {
		return filepath.substr(off2 + 1, filepath.size());
	}
	if (off2 == std::string::npos) {
		return filepath.substr(off1 + 1, filepath.size());
	}

	return filepath.substr(std::max(off1, off2) + 1, filepath.size());
}
