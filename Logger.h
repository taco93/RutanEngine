#pragma once
#include "COMException.h"
#include <Windows.h>

class Logger
{
public:
	static void Log(std::string message);
	static void Log(HRESULT hr, std::string message);
	static void LogError(HRESULT hr, std::string message);
	static void LogError(std::string message);
	static void LogError(HRESULT hr, std::wstring message);
	static void Log(COMException& exception);
};
