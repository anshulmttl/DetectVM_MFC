
// VirtualMachineTestDlg.h : header file
//

#pragma once

class CVirtualMachineTestDlgAutoProxy;


// CVirtualMachineTestDlg dialog
class CVirtualMachineTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVirtualMachineTestDlg);
	friend class CVirtualMachineTestDlgAutoProxy;

// Construction
public:
	CVirtualMachineTestDlg(CWnd* pParent = nullptr);	// standard constructor
	virtual ~CVirtualMachineTestDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIRTUALMACHINETEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	CVirtualMachineTestDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CButton m_StaticText;
};
