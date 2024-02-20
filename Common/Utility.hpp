#pragma once

#include <AK\Tools\Win32\AkPlatformFuncs.h>
#include <string>
#include <fstream>
#include <vector>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

namespace Utility
{
	void Print(char* message);
	void Print(std::string message);

	//https://stackoverflow.com/questions/18783087/how-to-properly-use-getmodulefilename
	std::string GetPluginPath();
	AkUInt32 GetIDFromXML();

};