
// DlgProxy.h: header file
//

#pragma once

class CVirtualMachineTestDlg;


// CVirtualMachineTestDlgAutoProxy command target

class CVirtualMachineTestDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CVirtualMachineTestDlgAutoProxy)

	CVirtualMachineTestDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CVirtualMachineTestDlg* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation
protected:
	virtual ~CVirtualMachineTestDlgAutoProxy();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CVirtualMachineTestDlgAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

