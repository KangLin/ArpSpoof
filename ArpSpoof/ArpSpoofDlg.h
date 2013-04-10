// ArpSpoofDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "ArpSpoofLib.h"

// CArpSpoofDlg �Ի���
class CArpSpoofDlg : public CDialog
{
// ����
public:
	CArpSpoofDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ARPSPOOF_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CString szLocalMac;
	CString szHostMac;
	CString szGatewayIp;
	CString szHostIp;
	CString szGatewayMac;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	CComboBoxEx m_cmbInterfaceList;
	int InitInterfaceList();
public:
	static int ListInterface(pcap_if_t * d, void * pPara);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonHost();
	afx_msg void OnBnClickedButtonGateway();
	afx_msg void OnCbnSelchangeCmbInterfaceList();
};
