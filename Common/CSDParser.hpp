#pragma once
#include <string>
#include "Parameter.hpp"
#include <vector>


class CSDParser
{
public:
	CSDParser(const std::string& CSDPath);
	~CSDParser();

	void Parse();
	void ProcessLine(const std::string& line);

	std::string_view GetPropertyValue(std::string_view text, std::string_view property, const char& startDelimiter, const char& endDelimiter);
	Range GetRangeFromCSVString(const std::string& CSV);
	std::vector<Parameter>& GetParameters() { return m_Parameters; };

private:
	std::vector<Parameter> m_Parameters;
	std::string m_CSDPath;
};