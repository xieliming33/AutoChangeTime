
// ChangeTimeToolDlg.h : header file
//

#pragma once
#include "atltime.h"


// CChangeTimeToolDlg dialog
class CChangeTimeToolDlg : public CDialogEx
{
// Construction
public:
	CChangeTimeToolDlg(CWnd* pParent = NULL);	                                               // standard constructor
	unsigned int RandData(unsigned int max,unsigned int min);                                  //���������
// Dialog Data
	enum { IDD = IDD_CHANGETIMETOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	                                       // DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOkBtnClickedButton2();                      //ȷ��
	afx_msg void OnAutoModelBtnClickedRadio1();                //�Զ�ģʽ
	afx_msg void OnManualModelBnClickedRadio2();               //�ֶ�ģʽ
	afx_msg void OnRandomChangeBtnClickedButton1();            //����޸�ƫ����
	afx_msg void OnRandomIntervalBtnClickedButton3();          //������
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CEdit   m_OffsetCtl;
	CButton m_RandomOffsetBtn;
	CEdit   m_IntervalCtl;
	CButton m_RandomIntervalBtn;
	CDateTimeCtrl m_ManualTimeCtl;                              //�����տؼ�
	CDateTimeCtrl m_ManalTimeSubCtl;                            //ʱ����ؼ�
	/*************************************************************/
	bool  modelflag;                                            //ģʽ��� false-�ֶ� true-�Զ�
	bool  autostartflag;                                        //�Զ��޸Ŀ�ʼ��־
	bool  offsetfalg;                                           //ƫ����������
	bool  intervalfalg;                                         //���������
	int   m_OffSetCtlVal;                                       //ƫ��������     
	CTime m_ManaulSetCtlVal;                                    //�����ձ���
	CTime m_MaunalTimeSubCtlVal;                                //ʱ�������
	UINT  m_IntervalCtlVal;                                     //�������          
	
	
	CButton m_OkBtn;                                            //ȷ��,��ʼ��������ť
	BOOL m_Radio2;                                              //������BOOL������һ����ֻ������һ����������GroupΪTRUE���Ǹ�Ctrl+D���з���Tab˳�򣬷ֺ�֮���ٽ�������http://blog.csdn.net/c_cyoxi/article/details/23868979��http:www.169it.com/tech-cpp/article-8083971887749772398.html
	CWinThread* pPlayerThread;
};
