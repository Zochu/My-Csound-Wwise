#include "CsoundManager.hpp"

#pragma region Initialization

CsoundManager::CsoundManager(const char* CSDPath) :
	m_csound(nullptr),
	m_csoundParams(nullptr),
	m_CSspin(nullptr),
	m_CSspout(nullptr),
	m_CSscale(0),
	m_csCompileResult(-1),
	m_csdKsmps(0),
	m_csIndex(0),
	m_CSDPath(CSDPath) {}

CsoundManager::~CsoundManager() 
{
	m_csound->Stop();
	m_csound->Cleanup();
}

void CsoundManager::Init()
{
	m_csound.reset(new Csound());
	m_csound->SetHostImplementedAudioIO(1, 0);
	m_csound->SetHostData(this);
	m_csound->CreateMessageBuffer(0);

	m_csoundParams.reset(new CSOUND_PARAMS());
	m_csoundParams->displays = 0;

	m_csound->SetOption((char*)"-n");
	m_csound->SetOption((char*)"-d");
	m_csound->SetOption((char*)"-b0");

	
}

bool CsoundManager::Compile()
{
	m_csCompileResult = m_csound->Compile(m_CSDPath);

	//flipping the compile result so false means it failed
	return !m_csCompileResult;
}

void CsoundManager::SetParams()
{
	m_csdKsmps = m_csound->GetKsmps();
	m_CSspout = m_csound->GetSpout();
	m_CSspin = m_csound->GetSpin();
	m_CSscale = m_csound->Get0dBFS();
	m_csIndex = m_csound->GetKsmps();

}

//Gets the number of channels from Csound and returns the appropriate Speaker configuration
AkChannelMask CsoundManager::GetChannelMask()
{
	return AK::ChannelMaskFromNumChannels(m_csound->GetNchnls());	
}

#pragma endregion

#pragma region Processing

void CsoundManager::SetChannel(char* channel, float value)
{
	m_csound->SetControlChannel(channel, value);
}

bool CsoundManager::Ready()
{
	return (m_csIndex >= m_csdKsmps);
}

void CsoundManager::SendEvent(AkReal32 duration)
{
	m_csound->InputMessage(("i1 0 " + std::to_string(duration)).c_str());
}

void CsoundManager::Perform()
{
	m_csound->PerformKsmps();
	m_csIndex = 0;
}

void CsoundManager::Stop()
{
	m_csound->Stop();
	m_csound->Cleanup();
}

void CsoundManager::Process(AkAudioBuffer* out_pBuffer, bool IsEffect)
{
	AkUInt16 channels = out_pBuffer->NumChannels();
	AkUInt16 WwiseSampleIndex = 0;

	while (WwiseSampleIndex < out_pBuffer->uValidFrames)
	{
		if (Ready())
			Perform();

		for (AkUInt8 i = 0; i < channels; ++i)
		{
			if(IsEffect)
				m_CSspin[i + (m_csIndex * channels)] = out_pBuffer->GetChannel(i)[WwiseSampleIndex];

			out_pBuffer->GetChannel(i)[WwiseSampleIndex] = m_CSspout[i + (m_csIndex * channels)];
		}
			
		++WwiseSampleIndex;
		++m_csIndex;
	}
	
}

#pragma endregion
