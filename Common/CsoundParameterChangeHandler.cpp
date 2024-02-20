#include "CsoundParameterChangeHandler.hpp"
#include <AK\Tools\Common\AkPlatformFuncs.h>

// Each Parameter occupies one bit of space that corresponds to the parameters ID
//ID = 0 -> 0000 0001,
//ID = 1 -> 0000 0010, 
//ID = 2 -> 0000 0100 and so on....

// Each Index in the ParamBitArray can store the state of 8 parameters, 
// the size of this array is calculated by dividing the total number of parametrs by 8 and adding 1.

// When we want to set or check if a parameter has changed we create a bitmask based off the parameter ID,
// Then the relvent bit is set or checked from this bitmask

template <AkUInt32 T_MAXNUMPARAMS> CsoundParameterChangeHandler <T_MAXNUMPARAMS>::CsoundParameterChangeHandler()
{
    m_uParamBitArray.reserve(1);
    ResetAllParamChanges();
}

template <AkUInt32 T_MAXNUMPARAMS> CsoundParameterChangeHandler <T_MAXNUMPARAMS>::~CsoundParameterChangeHandler(){}

//Sets the relevent bit to 1 based off the parameter ID
template <AkUInt32 T_MAXNUMPARAMS> void CsoundParameterChangeHandler <T_MAXNUMPARAMS>::SetParamChange(AkPluginParamID in_ID)
{
    AKASSERT(in_ID <= T_MAXNUMPARAMS);
    // Find the index in the bitArray, since each index stores the state of 8 params we just divide by 8 to get the index
    // Anything below 8 will be in index 0, between 8 - 16 will be index 2 and so on.
    const AkUInt32 uByteIndex = in_ID / 8;

    // Create Bitmask based off of the ID and the index in the Array,
    // The Bitmask will be the same as the ID
    // e.g if the ID is 2, its index will be 0
    //(2 - 0 * 8) = 2;
    //1 << 2 = 0000 0100
    const AkUInt32 uBitMask = 1 << (in_ID - uByteIndex * 8);

    // The Bitmask is then OR'd with the value stored in the appropirate index of the array
    // this will make sure that the bit reprseting the parameter with the ID of 2, is set.
    m_uParamBitArray[uByteIndex] |= uBitMask;
}

template <AkUInt32 T_MAXNUMPARAMS> bool CsoundParameterChangeHandler <T_MAXNUMPARAMS>::HasChanged(AkPluginParamID in_ID)
{
    AKASSERT(in_ID <= T_MAXNUMPARAMS);
    //Find the byte Index
    const AkUInt32 uByteIndex = in_ID / 8;
    //Create Mask
    const AkUInt32 uBitMask = 1 << (in_ID - uByteIndex * 8);

    //& the value with the bitmask, this will only be true if the relivent bit is set to 1
    return (m_uParamBitArray[uByteIndex] & uBitMask) > 0;
}

// This just checks if any parameters in the array has changed
// if any have they will not be 0
template <AkUInt32 T_MAXNUMPARAMS> bool CsoundParameterChangeHandler <T_MAXNUMPARAMS>::HasAnyChanged()
{
    AkUInt32 uByteIndex = 0;
    do
    {
        if (m_uParamBitArray[uByteIndex] > 0)
            return true;
        ++uByteIndex;
    } while (uByteIndex < (((1)+((8) - 1)) & ~((8) - 1)) / 8);
    return false;
}

template <AkUInt32 T_MAXNUMPARAMS> void CsoundParameterChangeHandler <T_MAXNUMPARAMS>::ResetParamChange(AkPluginParamID in_ID)
{
    AKASSERT(in_ID <= T_MAXNUMPARAMS);
    //Find index
    const AkUInt32 uByteIndex = in_ID / 8;
    //create mask
    const AkUInt32 uBitMask = 1 << (in_ID - uByteIndex * 8);

    //AND value with the inverted bitmask to reset
    m_uParamBitArray[uByteIndex] &= ~uBitMask;
}

template <AkUInt32 T_MAXNUMPARAMS> void CsoundParameterChangeHandler <T_MAXNUMPARAMS>::ResetAllParamChanges()
{
    //sets all values to 0
    memset(&m_uParamBitArray[0], 0, m_uParamBitArray.size() * sizeof m_uParamBitArray[0]);
}

template <AkUInt32 T_MAXNUMPARAMS> void CsoundParameterChangeHandler <T_MAXNUMPARAMS>::SetAllParamChanges()
{
    //sets all values to 1
    memset(&m_uParamBitArray[0], 255, m_uParamBitArray.size() * sizeof m_uParamBitArray[0]);
}

template <AkUInt32 T_MAXNUMPARAMS> void CsoundParameterChangeHandler <T_MAXNUMPARAMS>::SetSize(AkUInt32 size)
{
    m_uParamBitArray.resize(size);
}
