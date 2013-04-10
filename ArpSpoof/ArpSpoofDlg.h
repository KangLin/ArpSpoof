// ArpSpoofDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "ArpSpoofLib.h"

// CArpSpoofDlg 对话框
class CArpSpoofDlg : public CDialog
{
// 构造
public:
	CArpSpoofDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ARPSPOOF_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
