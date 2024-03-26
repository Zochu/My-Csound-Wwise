#include "CsoundWwisePlugin.h"
#include "../SoundEnginePlugin/CsoundWwiseSourceFactory.h"

#include <AK/Tools/Common/AkAssert.h>


CsoundWwisePlugin::CsoundWwisePlugin()
    : m_pPSet(nullptr){}

CsoundWwisePlugin::~CsoundWwisePlugin(){}

void CsoundWwisePlugin::Destroy()
{
    delete this;
}

void CsoundWwisePlugin::SetPluginPropertySet(AK::Wwise::IPluginPropertySet* in_pPSet)
{
    m_pPSet = in_pPSet;
}

//TODO: get Bank data from CSD 
bool CsoundWwisePlugin::GetBankParameters(const GUID& in_guidPlatform, AK::Wwise::IWriteData* in_pDataWriter) const
{
    // Write bank data here TODO, Loop Params
    CComVariant varProp;

    m_pPSet->GetValue(in_guidPlatform, L"Duration", varProp);
    in_pDataWriter->WriteReal32(varProp.fltVal);

    m_pPSet->GetValue(in_guidPlatform, L"Gain", varProp);
    in_pDataWriter->WriteReal32(varProp.fltVal);

    m_pPSet->GetValue(in_guidPlatform, L"Frequency", varProp);
    in_pDataWriter->WriteReal32(varProp.fltVal);

    //in_pDataWriter->WriteReal32(m_propertySet.GetReal32(in_guidPlatform, "Duration"));

    return true;
}

//DEFINE_AUDIOPLUGIN_CONTAINER(ParamTest);											// Create a PluginContainer structure that contains the info for our plugin
//EXPORT_AUDIOPLUGIN_CONTAINER(ParamTest);											// This is a DLL, we want to have a standardized name
//ADD_AUDIOPLUGIN_CLASS_TO_CONTAINER(                                             // Add our CLI class to the PluginContainer
//    ParamTest,        // Name of the plug-in container for this shared library
//    ParamTestPlugin,  // Authoring plug-in class to add to the plug-in container
//    ParamTestSource   // Corresponding Sound Engine plug-in class
//);
//DEFINE_PLUGIN_REGISTER_HOOK
//
//DEFINEDUMMYASSERTHOOK;							// Placeholder assert hook for Wwise plug-ins using AKASSERT (cassert used by default)
