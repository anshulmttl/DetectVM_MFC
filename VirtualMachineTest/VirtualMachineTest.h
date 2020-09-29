
// VirtualMachineTest.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CVirtualMachineTestApp:
// See VirtualMachineTest.cpp for the implementation of this class
//

class CVirtualMachineTestApp : public CWinApp
{
public:
	CVirtualMachineTestApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	bool IsVirtualMachine();
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CVirtualMachineTestApp theApp;
