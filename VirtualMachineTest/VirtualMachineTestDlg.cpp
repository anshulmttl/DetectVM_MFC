
// VirtualMachineTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VirtualMachineTest.h"
#include "VirtualMachineTestDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include <comdef.h>
#include <Wbemidl.h>
#include <iostream>
#pragma comment(lib, "wbemuuid.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVirtualMachineTestDlg dialog


IMPLEMENT_DYNAMIC(CVirtualMachineTestDlg, CDialogEx);

CVirtualMachineTestDlg::CVirtualMachineTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIRTUALMACHINETEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = nullptr;
}

CVirtualMachineTestDlg::~CVirtualMachineTestDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to null, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != nullptr)
		m_pAutoProxy->m_pDialog = nullptr;
}

void CVirtualMachineTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_StaticText);
}

BEGIN_MESSAGE_MAP(CVirtualMachineTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CVirtualMachineTestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CVirtualMachineTestDlg message handlers

BOOL CVirtualMachineTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVirtualMachineTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVirtualMachineTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVirtualMachineTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CVirtualMachineTestDlg::OnClose()
{
	if (CanExit())
		CDialogEx::OnClose();
}

void CVirtualMachineTestDlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CVirtualMachineTestDlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL CVirtualMachineTestDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != nullptr)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}



void CVirtualMachineTestDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	HRESULT hres;
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		//cout << "Failed to initialize COM library. Error code = 0x"
		//	<< hex << hres << endl;
		//return 1;                  // Program has failed.
	}

	hres = CoInitializeSecurity(
		NULL,
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
	);

	if (FAILED(hres))
	{

	}

	IWbemLocator *pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hres))
	{
		//cout << "Failed to create IWbemLocator object."
		//	<< " Err code = 0x"
		//	<< hex << hres << endl;
		CoUninitialize();
		//return 1;                 // Program has failed.
		return;
	}

	IWbemServices *pSvc = NULL;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (for example, Kerberos)
		0,                       // Context object 
		&pSvc                    // pointer to IWbemServices proxy
	);

	if (FAILED(hres))
	{
		//cout << "Could not connect. Error code = 0x"
		//	<< hex << hres << endl;
		//pLoc->Release();
		CoUninitialize();
		//return 1;                // Program has failed.
		return;
	}

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
	);

	if (FAILED(hres))
	{
		//cout << "Could not set proxy blanket. Error code = 0x"
		//	<< hex << hres << endl;
		//pSvc->Release();
		//pLoc->Release();
		CoUninitialize();
		//return 1;               // Program has failed.
		return;
	}

	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_ComputerSystem"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		//cout << "Query for operating system name failed."
		//	<< " Error code = 0x"
		//	<< hex << hres << endl;
		//pSvc->Release();
		//pLoc->Release();
		CoUninitialize();
		//return 1;               // Program has failed.
		return;
	}

	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);

		if (0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;
		VARIANT vtModelProp;
		CString manufacturer, manufacturer_lower;
		CString model, model_upper;
		
		
		//string manufacturer = item["Manufacturer"].ToString().ToLower();
		// if((manufacturer == "microsoft corporation" && item["Model"].ToString().ToUpperInvariant().Contains("VIRTUAL"))
		//		|| manufacturer.Contains("vmware")
		//		|| item["Model"].ToString() == "VirtualBox")
		// {
		//		The machine is virtual machine
		// }
		// Get the value of the Name property
		hr = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
		hr = pclsObj->Get(L"Model", 0, &vtModelProp, 0, 0);

		if (vtProp.vt != VT_BSTR)
		{
			manufacturer = "";
		}
		else
		{
			manufacturer = (char*)(_bstr_t)vtProp;
			manufacturer_lower = manufacturer;
			manufacturer_lower.MakeLower();
		}

		if (vtModelProp.vt != VT_BSTR)
		{
			model = "";
		}
		else
		{
			model = (char*)(_bstr_t)vtModelProp;
			model_upper = model;
			model_upper.MakeUpper();
		}

		// Logic
		if ((manufacturer_lower == "microsoft corporation" && model_upper.Find(L"VIRTUAL") != -1)
			|| (manufacturer_lower.Find(L"vmware") != -1)
			|| (model == "VirtualBox") 
			|| (model_upper == "VMWARE")
			|| (model_upper == "KVM")
			|| (manufacturer_lower.Find(L"microsoft") != -1 && model_upper.Find(L"VIRTUAL") != -1)
			|| (manufacturer_lower.Find(L"vmware") != -1))
		{
			CWnd *label = GetDlgItem(IDC_STATIC);
			label->SetWindowText(manufacturer_lower + L" + " + model_upper + L" + " + L"Virtual");
			//m_StaticText.SetWindowTextW(L"Virtual");
		}
		else
		{
			CWnd *label = GetDlgItem(IDC_STATIC);
			label->SetWindowText(manufacturer_lower + L" + " + model_upper + L" + " + L"Not Virtual");
			//m_StaticText.SetWindowTextW(L"Not Virtual");
		}

		//wcout << " OS Name : " << vtProp.bstrVal << endl;
		VariantClear(&vtProp);
		VariantClear(&vtModelProp);
		pclsObj->Release();
	}

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();

	return;
}
