#include "ParameterManager.h"
#include <AK/Tools/Common/AkBankReadHelpers.h>
#include <string>
#include "CSDParser.hpp"
#include "Utility.hpp"
//#include "CsoundParameterChangeHandler.hpp"

ParameterManager::ParameterManager()
{
    // Parse The CSD file to get the Parameters
    std::string csdPath = Utility::GetPluginPath() + ".csd";
    m_csoundManager = std::make_shared<CsoundManager>(&csdPath[0]);

    //Initialize Csound
    m_csoundManager->Init();
    m_Compiled = m_csoundManager->Compile();

    fDuration = 0.0f;

    if (m_Compiled)
    {
        m_csoundManager->SetParams();
        CSDParser parser(csdPath);
        parser.Parse();
        SetParameters(parser.GetParameters());
    }
}

ParameterManager::ParameterManager(const ParameterManager& in_rParams)
{
    fDuration = in_rParams.fDuration;
    *this = in_rParams;
}

ParameterManager::~ParameterManager() {}

AK::IAkPluginParam* ParameterManager::Clone(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, ParameterManager(*this));
}

AKRESULT ParameterManager::Init(AK::IAkPluginMemAlloc* in_pAllocator, const void* in_pParamsBlock, AkUInt32 in_uBlockSize)
{
    if (in_uBlockSize == 0)
    {
        fDuration = 0.0f;
        m_ParamChangeHandler.SetAllParamChanges();
        return AK_Success;
    }

    return SetParamsBlock(in_pParamsBlock, in_uBlockSize);
}

AKRESULT ParameterManager::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
    AK_PLUGIN_DELETE(in_pAllocator, this);
    return AK_Success;
}

AKRESULT ParameterManager::SetParamsBlock(const void* in_pParamsBlock, AkUInt32 in_uBlockSize)
{
    AKRESULT eResult = AK_Success;
    AkUInt8* pParamsBlock = (AkUInt8*)in_pParamsBlock;

    // Read bank data for each parameter
    fDuration = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    for (Parameter& p : m_Parameters)
    {
        p.SetValue(READBANKDATA(AkReal32, pParamsBlock, in_uBlockSize));
    }

    CHECKBANKDATASIZE(in_uBlockSize, eResult);
    m_ParamChangeHandler.SetAllParamChanges();

    return eResult;
}

//Called when the value of a parmeter is changed
AKRESULT ParameterManager::SetParam(AkPluginParamID in_paramID, const void* in_pValue, AkUInt32 in_uParamSize)
{
    AKRESULT eResult = AK_Success;

    /*if (in_paramID == PARAM_DURATION_ID)
    {
        fDuration = *((AkReal32*)in_pValue);
        m_ParamChangeHandler.SetParamChange(PARAM_DURATION_ID);
    }*/

    for (Parameter& p : m_Parameters)
    {
        if (p.GetID() == in_paramID)
        {
            if (in_paramID == PARAM_DURATION_ID)
            {
                fDuration = *((AkReal32*)in_pValue);
            }
            //Set the Parameters new Value
            p.SetValue(*((AkReal32*)in_pValue));

            //Notify parameter change Handler
            m_ParamChangeHandler.SetParamChange(in_paramID);

            eResult = AK_Success;
            break;
        }
        else
        {
            eResult = AK_InvalidParameter;
        }
    }

    return eResult;
}

void ParameterManager::HandleParameterChange(std::shared_ptr<CsoundManager>& csoundManager)
{
    if (m_ParamChangeHandler.HasAnyChanged())
    {
        for (Parameter& p : m_Parameters)
        {
            //Only Notify Csound if the value has changed since last execution
            if (m_ParamChangeHandler.HasChanged(p.GetID()))
            {
                m_ParamChangeHandler.ResetParamChange(p.GetID());
                csoundManager->SetChannel(p.GetName(), p.GetValue());
            }
        }
        m_ParamChangeHandler.ResetAllParamChanges();
    }

}

void ParameterManager::SetParameters(std::vector<Parameter>& params)
{
    m_ParamChangeHandler.SetSize((params.size() / 8) + 1);
    m_ParamChangeHandler.SetAllParamChanges();
    m_Parameters = params;
}
