#pragma once

#include "Utility.hpp"

void Utility::Print(char* message)
{
	AKPLATFORM::OutputDebugMsg("\n\n\n\n**************************************************************\n\n\n\n");
	AKPLATFORM::OutputDebugMsg(message);
	AKPLATFORM::OutputDebugMsg("\n\n\n\n**************************************************************\n\n\n\n");
}

void Utility::Print(std::string message)
{
	AKPLATFORM::OutputDebugMsg("\n\n\n\n**************************************************************\n\n\n\n");
	AKPLATFORM::OutputDebugMsg(&message[0]);
	AKPLATFORM::OutputDebugMsg("\n\n\n\n**************************************************************\n\n\n\n");
}

std::string Utility::GetPluginPath()
{
	std::vector<wchar_t> pathBuf;
	DWORD copied = 0;
	do {
		pathBuf.resize(pathBuf.size() + MAX_PATH);
		copied = GetModuleFileName((HINSTANCE)&__ImageBase, &pathBuf.at(0), pathBuf.size());
	} while (copied >= pathBuf.size());

	pathBuf.resize(copied);

	std::string path(pathBuf.begin(), pathBuf.end());

	size_t lastindex = path.find_last_of(".");

	return path.substr(0, lastindex);
}

AkUInt32 Utility::GetIDFromXML()
{
	std::ifstream xml(Utility::GetPluginPath() + ".xml");

	if (xml.is_open())
	{
		std::string line;

		while (std::getline(xml, line))
		{
			size_t index = line.find("PluginID");

			if (index != std::string::npos)
			{
				size_t startIndex = line.find("\"", index) + 1;
				size_t endIndex = line.find("\"", startIndex + 1);

				return  stoul(line.substr(startIndex, endIndex - startIndex));
			}
		}
	}

	return 0;
}
