#pragma once

#include "C:\Program Files\Csound6_x64\include\csound\csound.hpp"
#include <memory>
#include <string>
#include <AK/SoundEngine/Common/AkCommonDefs.h>

class CsoundManager
{
public:
	CsoundManager(const char* CSDPath);
	~CsoundManager();

	void Init();
	bool Compile();
	void SetParams();
	void SetChannel(char* channel, float value);

	AkChannelMask GetChannelMask();

	bool Ready();
	void Perform();

	void Process(AkAudioBuffer* out_pBuffer, bool IsEffect = false);
private:
	std::unique_ptr<Csound> m_csound;
	std::unique_ptr<CSOUND_PARAMS> m_csoundParams;

	MYFLT* m_CSspin, * m_CSspout;
	MYFLT m_CSscale;

	int m_csCompileResult = -1;
	AkUInt16 m_csdKsmps, m_csIndex;

	const char* m_CSDPath;
};