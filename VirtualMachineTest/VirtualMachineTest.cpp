
// VirtualMachineTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "VirtualMachineTest.h"
#include "VirtualMachineTestDlg.h"
#include <comdef.h>
#include <Wbemidl.h>
#include <iostream>
#include "ServiceLib/ServiceController.h"
#include "ServiceLib/ServiceEnumerator.h"
#pragma comment(lib, "wbemuuid.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVirtualMachineTestApp

BEGIN_MESSAGE_MAP(CVirtualMachineTestApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVirtualMachineTestApp construction

CVirtualMachineTestApp::CVirtualMachineTestApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CVirtualMachineTestApp object

CVirtualMachineTestApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{0x0c517984,0x922c,0x416b,{0x97,0x2b,0x41,0x0d,0x0e,0x53,0xc4,0x63}};
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CVirtualMachineTestApp initialization

BOOL CVirtualMachineTestApp::InitInstance()
{
	
	bool isVm = IsVirtualMachine();
	if (isVm)
	{
		MessageBox(NULL, _T("Application does not run in Virtual machine"), _T("Message"), NULL);
		return false;
	}
		
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	// Parse command line for automation or reg/unreg switches.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// App was launched with /Embedding or /Automation switch.
	// Run app as automation server.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Register class factories via CoRegisterClassObject().
		COleTemplateServer::RegisterAll();
	}
	// App was launched with /Unregserver or /Unregister switch.  Remove
	// entries from the registry.
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		COleObjectFactory::UpdateRegistryAll(FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
		return FALSE;
	}
	// App was launched standalone or with other switches (e.g. /Register
	// or /Regserver).  Update registry entries, including typelibrary.
	else
	{
		COleObjectFactory::UpdateRegistryAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
		if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister)
			return FALSE;
	}

	CVirtualMachineTestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CVirtualMachineTestApp::ExitInstance()
{
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

bool CVirtualMachineTestApp::IsVirtualMachine()
{
	bool isVm = false;
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
		return true;
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
		return true;
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
		return true;
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
		return true;
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
			isVm = true;
			//CWnd *label = GetDlgItem(IDC_STATIC);
			//label->SetWindowText(L"Virtual");
			//m_StaticText.SetWindowTextW(L"Virtual");
		}
		else
		{
			//CWnd *label = GetDlgItem(IDC_STATIC);
			//label->SetWindowText(L"Not Virtual");
			//m_StaticText.SetWindowTextW(L"Not Virtual");
		}

		
		//wcout << " OS Name : " << vtProp.bstrVal << endl;
		VariantClear(&vtProp);
		VariantClear(&vtModelProp);
		pclsObj->Release();
	}

	// Check virtual box services
	auto services = ServiceEnumerator::EnumerateServices();
	for (auto const & s : services)
	{
		CString serviceName(s.ServiceName.c_str());
		serviceName.MakeLower();
		if (serviceName == "vboxservice")
		{
			isVm = true;
			break;
		}
		/*std::wcout << "Name:    " << s.ServiceName << std::endl
			<< "Display: " << s.DisplayName << std::endl
			<< "Status:  " << ServiceStatusToString(static_cast<ServiceStatus>(s.Status.dwCurrentState)) << std::endl
			<< "--------------------------" << std::endl;*/
	}
	//Check virtual box disk drive
	IEnumWbemClassObject* pEnumeratorDisk = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_DiskDrive"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumeratorDisk);

	if (FAILED(hres))
	{
		CoUninitialize();
		//return 1;               // Program has failed.
		return true;
	}
	
	IWbemClassObject *pclsObjDisk = NULL;
	ULONG uReturnDisk = 0;

	while (pEnumeratorDisk)
	{
		HRESULT hr = pEnumeratorDisk->Next(WBEM_INFINITE, 1,
			&pclsObjDisk, &uReturnDisk);

		if (0 == uReturnDisk)
		{
			break;
		}

		VARIANT vtModelProp1;
		CString model, model_upper;

		hr = pclsObjDisk->Get(L"Model", 0, &vtModelProp1, 0, 0);

		if (vtModelProp1.vt != VT_BSTR)
		{
			model = "";
		}
		else
		{
			model = (char*)(_bstr_t)vtModelProp1;
			model_upper = model;
			model_upper.MakeUpper();
		}
		if ((model_upper.Find(L"VBOX") != -1)		// VIRTUALBOX
			|| (model_upper.Find(L"VMWARE") != -1)	// VMWARE
			|| (model_upper.Find(L"VIRTUAL") != -1)) // HyperV
		{
			isVm = true;
		}

		VariantClear(&vtModelProp1);
		pclsObjDisk->Release();
	}
	
	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();

	return isVm;
}