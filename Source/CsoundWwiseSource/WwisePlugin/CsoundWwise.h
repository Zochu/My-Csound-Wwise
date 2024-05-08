#pragma once

#include <afxwin.h>
#include <Windows.h>

class CsoundWwiseApp
    : public CWinApp
{
public:
    CsoundWwiseApp();

    BOOL InitInstance() override;
    DECLARE_MESSAGE_MAP()
};
