#include "CsoundWwise.h"
#include "CsoundWwisePlugin.h"
#include "../CsoundWwiseConfig.h"

#include <AK/Wwise/Utilities.h>
#include <AK/Tools/Common/AkAssert.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CsoundWwiseApp, CWinApp)
END_MESSAGE_MAP()

CsoundWwiseApp::CsoundWwiseApp(){}

CsoundWwiseApp theApp;

BOOL CsoundWwiseApp::InitInstance()
{
    CWinApp::InitInstance();
    AK::Wwise::RegisterWwisePlugin();
    return TRUE;
}

AK::Wwise::IPluginBase* __stdcall AkCreatePlugin(unsigned short in_companyID, unsigned short in_pluginID)
{
    return new CsoundWwisePlugin;
}

DEFINEDUMMYASSERTHOOK;
DEFINE_PLUGIN_REGISTER_HOOK;
