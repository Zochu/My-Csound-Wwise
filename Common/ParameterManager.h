#pragma once

#include <vector>
#include "Parameter.hpp"
#include "CsoundManager.hpp"
//#include "csound.hpp"
#include "C:\Program Files\Csound6_x64\include\csound\csound.hpp"

#include <AK/SoundEngine/Common/IAkPlugin.h>
#include <AK/Plugin/PluginServices/AkFXParameterChangeHandler.h>
#include "CsoundParameterChangeHandler.hpp"
#include "CsoundParameterChangeHandler.cpp"

static const AkPluginParamID PARAM_DURATION_ID = 0;
static const AkUInt32 NUM_PARAMS = 1;


struct RTPCParams
{
	AkReal32 fDuration;
};

class ParameterManager : public AK::IAkPluginParam
{
public:
	ParameterManager();
	ParameterManager(const ParameterManager& in_rParams);

	~ParameterManager();

	// Inherited via IAkPluginParam
	virtual IAkPluginParam* Clone(AK::IAkPluginMemAlloc* in_pAllocator);
	virtual AKRESULT Init(AK::IAkPluginMemAlloc* in_pAllocator, const void* in_pParamsBlock, AkUInt32 in_uBlockSize);
	virtual AKRESULT Term(AK::IAkPluginMemAlloc* in_pAllocator);
	virtual AKRESULT SetParamsBlock(const void* in_pParamsBlock, AkUInt32 in_uBlockSize);
	virtual AKRESULT SetParam(AkPluginParamID in_paramID, const void* in_pValue, AkUInt32 in_uParamSize);

	void HandleParameterChange(std::shared_ptr<CsoundManager>& csoundManager);
	void SetParameters(std::vector<Parameter>& params);
	bool IsCompiled() { return m_Compiled; }
	std::shared_ptr<CsoundManager>& GetCsoundManager() { return m_csoundManager; };

	AkReal32 fDuration;
	RTPCParams RTPC;

private:
	bool m_Compiled;
	CsoundParameterChangeHandler<NUM_PARAMS> m_ParamChangeHandler;
	std::vector<Parameter> m_Parameters;
	std::shared_ptr<CsoundManager> m_csoundManager;
};