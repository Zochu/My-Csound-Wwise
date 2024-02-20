#ifndef CsoundWwiseSource_H
#define CsoundWwiseSource_H

#include "../../Common/ParameterManager.h"
#include "../../Common/CsoundManager.hpp" 
#include <AK/Plugin/PluginServices/AkFXDurationHandler.h>

#include "csound.hpp"
#include <memory>



/// See https://www.audiokinetic.com/library/edge/?source=SDK&id=soundengine__plugins__source.html
/// for the documentation about source plug-ins
class CsoundWwiseSource
    : public AK::IAkSourcePlugin
{
public:
    CsoundWwiseSource();
    ~CsoundWwiseSource();

    /// Plug-in initialization.
    /// Prepares the plug-in for data processing, allocates memory and sets up the initial conditions.
    AKRESULT Init(AK::IAkPluginMemAlloc* in_pAllocator, AK::IAkSourcePluginContext* in_pContext, AK::IAkPluginParam* in_pParams, AkAudioFormat& in_rFormat);

    /// Release the resources upon termination of the plug-in.
    AKRESULT Term(AK::IAkPluginMemAlloc* in_pAllocator);

    /// The reset action should perform any actions required to reinitialize the
    /// state of the plug-in to its original state (e.g. after Init() or on effect bypass).
    AKRESULT Reset();

    /// Plug-in information query mechanism used when the sound engine requires
    /// information about the plug-in to determine its behavior.
    AKRESULT GetPluginInfo(AkPluginInfo& out_rPluginInfo);

    /// Source plug-in DSP execution.
    void Execute(AkAudioBuffer* io_pBuffer);

    /// This method is called to determine the approximate duration (in ms) of the source.
    AkReal32 GetDuration() const;

private:
    ParameterManager* m_pParams;

    AK::IAkPluginMemAlloc* m_pAllocator;
    AK::IAkSourcePluginContext* m_pContext;
    AkFXDurationHandler m_durationHandler;

    std::shared_ptr<CsoundManager> m_csoundManager;

};

#endif // CsoundWwiseSource_H
