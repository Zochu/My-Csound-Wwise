#include "CsoundWwiseSource.h"
#include "../CsoundWwiseConfig.h"

#include <AK/AkWwiseSDKVersion.h>
#include <AK/AkPlatforms.h>
#include "../../Common/Utility.hpp"
#include "../../Common/Constants.hpp"


AK::IAkPlugin* CreateCsoundWwiseSource(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, CsoundWwiseSource());
}

AK::IAkPluginParam* CreateCsoundWwiseSourceParams(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, ParameterManager());
}

AK_IMPLEMENT_PLUGIN_FACTORY(CsoundWwiseSource, AkPluginTypeSource, CsoundWwiseConfig::CompanyID, Utility::GetIDFromXML())

CsoundWwiseSource::CsoundWwiseSource()
    : m_pParams(nullptr)
    , m_pAllocator(nullptr)
    , m_pContext(nullptr)
    , m_durationHandler()
    , m_csoundManager(nullptr){}

CsoundWwiseSource::~CsoundWwiseSource() {}

AKRESULT CsoundWwiseSource::Init(AK::IAkPluginMemAlloc* in_pAllocator, AK::IAkSourcePluginContext* in_pContext, AK::IAkPluginParam* in_pParams, AkAudioFormat& in_rFormat)
{
    // Initilize ParameterManager
    m_pParams = (ParameterManager*)in_pParams;

    if (!m_pParams->IsCompiled())
        return AK_InvalidFile;

    m_pAllocator = in_pAllocator;
    m_pContext = in_pContext;

    m_csoundManager = m_pParams->GetCsoundManager();
    in_rFormat.channelConfig.SetStandard(m_csoundManager->GetChannelMask());
    m_durationHandler.Setup(Constants::Duration, Constants::LoopCount, in_rFormat.uSampleRate);

    return AK_Success;
}

AKRESULT CsoundWwiseSource::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
    AK_PLUGIN_DELETE(in_pAllocator, this);
    return AK_Success;
}

AKRESULT CsoundWwiseSource::Reset()
{
    return AK_Success;
}

AKRESULT CsoundWwiseSource::GetPluginInfo(AkPluginInfo& out_rPluginInfo)
{
    out_rPluginInfo.eType = AkPluginTypeSource;
    out_rPluginInfo.bIsInPlace = true;
    out_rPluginInfo.uBuildVersion = AK_WWISESDK_VERSION_COMBINED;
    return AK_Success;
}

void CsoundWwiseSource::Execute(AkAudioBuffer* out_pBuffer)
{
    //Check if parameters have changed since last call and inform Csound
    m_pParams->HandleParameterChange(m_csoundManager);

    m_durationHandler.SetDuration(Constants::Duration);
    m_durationHandler.ProduceBuffer(out_pBuffer);

    m_csoundManager->Process(out_pBuffer);
}

AkReal32 CsoundWwiseSource::GetDuration() const
{
    return m_durationHandler.GetDuration() * 1000.0f;
}
