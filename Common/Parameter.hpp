#pragma once
#include <AK\SoundEngine\Common\AkTypes.h>
#include <string>

struct Range
{
	Range(float min, float max, float defaultVal, float skew, const std::string& stepSize)
		: m_Min(min)
		, m_Max(max)
		, m_DefaultVal(defaultVal)
		, m_Skew(skew)
		, m_StepSize(stepSize) {}

	float m_Min;
	float m_Max;
	float m_DefaultVal;
	float m_Skew;
	std::string m_StepSize;

	std::size_t GetHash()
	{
		return
			std::hash<float>{}(m_Min) ^
			(std::hash<float>{}(m_Max) << 1) >> 1 ^
			(std::hash<float>{}(m_DefaultVal) << 2) >> 2 ^
			(std::hash<float>{}(m_Skew) << 3) >> 3 ^
			(std::hash<std::string>{}(m_StepSize) << 4) >> 4;
	}
};

class Parameter
{
public:
	Parameter(const std::string& name, Range range, const std::string& type = "Real32");
	~Parameter();

	AkPluginParamID GetID();
	char* GetName();

	AkReal32 GetValue();
	Range GetRange();
	const std::string& GetType();
	void SetValue(AkReal32 value);
	void SetID(AkPluginParamID id);
	std::size_t GetHash();

private:
	AkPluginParamID m_ID;

	std::string m_Name;
	std::string m_Type;

	Range m_Range;
};