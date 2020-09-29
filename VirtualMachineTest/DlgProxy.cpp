
// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "VirtualMachineTest.h"
#include "DlgProxy.h"
#include "VirtualMachineTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVirtualMachineTestDlgAutoProxy

IMPLEMENT_DYNCREATE(CVirtualMachineTestDlgAutoProxy, CCmdTarget)

CVirtualMachineTestDlgAutoProxy::CVirtualMachineTestDlgAutoProxy()
{
	EnableAutomation();

	// To keep the application running as long as an automation
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CVirtualMachineTestDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CVirtualMachineTestDlg)))
		{
			m_pDialog = reinterpret_cast<CVirtualMachineTestDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CVirtualMachineTestDlgAutoProxy::~CVirtualMachineTestDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != nullptr)
		m_pDialog->m_pAutoProxy = nullptr;
	AfxOleUnlockApp();
}

void CVirtualMachineTestDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CVirtualMachineTestDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CVirtualMachineTestDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_IVirtualMachineTest to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .IDL file.

// {d8fbc4b6-06a4-4717-96d3-3a27ac79ddd3}
static const IID IID_IVirtualMachineTest =
{0xd8fbc4b6,0x06a4,0x4717,{0x96,0xd3,0x3a,0x27,0xac,0x79,0xdd,0xd3}};

BEGIN_INTERFACE_MAP(CVirtualMachineTestDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CVirtualMachineTestDlgAutoProxy, IID_IVirtualMachineTest, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {0e45ba69-9d9a-4b1c-82f5-6546d9a153fe}
IMPLEMENT_OLECREATE2(CVirtualMachineTestDlgAutoProxy, "VirtualMachineTest.Application", 0x0e45ba69,0x9d9a,0x4b1c,0x82,0xf5,0x65,0x46,0xd9,0xa1,0x53,0xfe)


// CVirtualMachineTestDlgAutoProxy message handlers
