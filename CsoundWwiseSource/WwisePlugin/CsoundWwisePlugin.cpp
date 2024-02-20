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

    m_pPSet->GetValue(in_guidPlatform, L"Gain", varProp);
    in_pDataWriter->WriteReal32(varProp.fltVal);

    m_pPSet->GetValue(in_guidPlatform, L"Frequency", varProp);
    in_pDataWriter->WriteReal32(varProp.fltVal);

    return true;
}
