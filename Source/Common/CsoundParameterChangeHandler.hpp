#pragma once

#include <AK/SoundEngine/Common/AkTypes.h>
#include <vector>
#include <string>

template <AkUInt32 T_MAXNUMPARAMS>
class CsoundParameterChangeHandler
{
public:
	CsoundParameterChangeHandler();
	~CsoundParameterChangeHandler();

	void SetParamChange(AkPluginParamID in_ID);
	bool HasChanged(AkPluginParamID in_ID);
	bool HasAnyChanged();
	void ResetParamChange(AkPluginParamID in_ID);
	void ResetAllParamChanges();
	void SetAllParamChanges();
	void SetSize(AkUInt32 size);
private:
	std::vector<AkUInt8> m_uParamBitArray;
};