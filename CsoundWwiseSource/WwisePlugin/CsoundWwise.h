#pragma once

#include <afxwin.h>
//#include "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.16.27023\atlmfc\include\afxwin.h"
#include <Windows.h>

class CsoundWwiseApp
    : public CWinApp
{
public:
    CsoundWwiseApp();

    BOOL InitInstance() override;
    DECLARE_MESSAGE_MAP()
};
