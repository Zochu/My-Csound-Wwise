#include "Parameter.hpp"
#include <stdio.h>
#include <vector>

Parameter::Parameter(const std::string& name, Range range, const std::string& type) :
    m_ID(0),
    m_Name(name),
    m_Range(range),
    m_Type(type) {}

Parameter::~Parameter(){}

#pragma region Getters

AkPluginParamID Parameter::GetID()
{
    return m_ID;
}

char* Parameter::GetName()
{
    return &m_Name[0];
}

AkReal32 Parameter::GetValue()
{
    return m_Range.m_DefaultVal;
}

Range Parameter::GetRange()
{
    return m_Range;
}

const std::string& Parameter::GetType()
{
    return m_Type;
}

std::size_t Parameter::GetHash()
{
    std::size_t h1 = std::hash<long>{}(m_ID);
    std::size_t h2 = std::hash<std::string>{}(m_Name);
    std::size_t h3 = m_Range.GetHash();

    return  h1 ^ (h2 << 1) >> 1 ^ h3 << 1;
}

#pragma endregion

#pragma region Setters

void Parameter::SetValue(AkReal32 value)
{
    m_Range.m_DefaultVal = value;
}

void Parameter::SetID(AkPluginParamID id)
{
    m_ID = id;
}

#pragma endregion