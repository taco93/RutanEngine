#pragma once
#include <comdef.h>
#include "StringHelper.h"

#define COM_ERROR_IF_FAILED( hr, msg ) if( FAILED ( hr ) ) throw COMException( hr, msg, __FILE__, __FUNCTION__, __LINE__ )

class COMException
{
public:
	COMException(HRESULT hr, const std::string& msg, const std::string& file, const std::string& function, int line)
	{
		_com_error error(hr);
		whatMsg = L"Msg: " + StringHelper::StringToWide(std::string(msg)) + L"\n";
		whatMsg += error.ErrorMessage();
		whatMsg += L"\nFile: " + StringHelper::StringToWide(file);
		whatMsg += L"\nFunction: " + StringHelper::StringToWide(function);
		whatMsg += L"\nLine: " + StringHelper::StringToWide(std::to_string(line));
	}

	const wchar_t* what() const
	{
		return whatMsg.c_str();
	}

private:
	std::wstring whatMsg;
};