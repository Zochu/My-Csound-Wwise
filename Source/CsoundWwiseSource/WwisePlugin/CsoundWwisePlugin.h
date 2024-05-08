#pragma once

#include <windows.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#include <atlstr.h>
#include <fstream>
#include <string>
#include <vector>

#include <AK/Wwise/AudioPlugin.h>
//#include <AK/Wwise/Plugin.h>
#include "../../Common/Utility.hpp"

/// See https://www.audiokinetic.com/library/edge/?source=SDK&id=plugin__dll.html
/// for the documentation about Authoring plug-ins
class CsoundWwisePlugin
    : public AK::Wwise::DefaultAudioPluginImplementation
{
public:
    CsoundWwisePlugin();
    ~CsoundWwisePlugin();

    /// This will be called to delete the plug-in.
    /// The object is responsible for deleting itself when this method is called.
    void Destroy() override;

    /// The property set interface is given to the plug-in through this method.
    /// It is called by Wwise during initialization of the plug-in, before most other calls.
    void SetPluginPropertySet(AK::Wwise::IPluginPropertySet* in_pPSet) override;

    /// This function is called by Wwise to obtain parameters that will be written to a bank.
    /// Because these can be changed at run-time, the parameter block should stay relatively small.
    // Larger data should be put in the Data Block.
    bool GetBankParameters(const GUID& in_guidPlatform, AK::Wwise::IWriteData* in_pDataWriter) const override;

private:
    AK::Wwise::IPluginPropertySet* m_pPSet;
    std::vector<LPCWSTR> m_propertyNames;
    std::string m_CSDpath;

    std::string TrimName(const std::string& str);
    LPCWSTR StringToLPCWSTR(const std::string_view& str);
    void GetPropertyNames();
    void Parse();
    void ProcessLine(const std::string& line);
    std::string_view GetPropertyValue(std::string_view text, std::string_view property, const char& startDelimiter, const char& endDelimiter);
};
