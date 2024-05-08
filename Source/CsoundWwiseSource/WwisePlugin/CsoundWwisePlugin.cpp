#include "CsoundWwisePlugin.h"
#include "../SoundEnginePlugin/CsoundWwiseSourceFactory.h"

#include <AK/Tools/Common/AkAssert.h>


CsoundWwisePlugin::CsoundWwisePlugin()
    : m_pPSet(nullptr){}

CsoundWwisePlugin::~CsoundWwisePlugin(){}

void CsoundWwisePlugin::Destroy()
{
    for (auto i = m_propertyNames.begin(); i != m_propertyNames.end(); ++i)
    {
        delete[] * i;
    }
    delete this;
}

void CsoundWwisePlugin::SetPluginPropertySet(AK::Wwise::IPluginPropertySet* in_pPSet)
{
    m_pPSet = in_pPSet;
    GetPropertyNames();
}

bool CsoundWwisePlugin::GetBankParameters(const GUID& in_guidPlatform, AK::Wwise::IWriteData* in_pDataWriter) const
{
    CComVariant varProp;
    for (auto i = m_propertyNames.begin(); i != m_propertyNames.end(); ++i)
    {
        in_pDataWriter->WriteReal32(m_pPSet->GetValue(in_guidPlatform, *i, varProp));
        in_pDataWriter->WriteReal32(varProp.fltVal);
    }

    return true;
}

LPCWSTR CsoundWwisePlugin::StringToLPCWSTR(const std::string_view& str) {
    // Calculate the required length
    int length = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.length()), nullptr, 0);

    // Allocate memory for the wide character buffer
    wchar_t* buffer = new wchar_t[length + 1]; // +1 for null terminator

    // Convert the UTF-8 encoded string to a wide character string
    MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.length()), buffer, length);

    // Add null terminator
    buffer[length] = L'\0';

    // Return pointer to the allocated buffer
    return buffer;
}


void CsoundWwisePlugin::GetPropertyNames()
{
    m_CSDpath = Utility::GetPluginPath() + ".csd";
    Parse();
}

void CsoundWwisePlugin::Parse()
{
    std::ifstream csd(m_CSDpath);

    if (csd.is_open())
    {
        std::string line;

        while (std::getline(csd, line))
        {
            ProcessLine(line);

            //break when we reach the closing cabbage tag
            if (line.find("</Cabbage>") != std::string::npos)
                break;
        }
    }
    else 
    {
        return;
    }
}

void CsoundWwisePlugin::ProcessLine(const std::string& line)
{
    //Find first word, use space as delimeter
    std::string_view first(line.c_str(), line.find(' '));

    // check if it is a valid widget
    // if the line is a property - create property object and add to the properties class
    first.remove_prefix(1);
    if (first == "slider")
    {
        std::string_view name = GetPropertyValue(line, "channel", '\"', '\"');

        m_propertyNames.push_back(StringToLPCWSTR(name));
    }
}

std::string_view CsoundWwisePlugin::GetPropertyValue(std::string_view text, std::string_view property, const char& startDelimiter, const char& endDelimiter)
{
    size_t startIndex = text.find(startDelimiter, text.find(property)) + 1;
    size_t EndIndex = text.find(endDelimiter, startIndex + 1);

    return  text.substr(startIndex, EndIndex - startIndex);
}
