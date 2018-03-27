
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
	unsigned int RandData(unsigned int max,unsigned int min);                                  //生成随机数
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
	afx_msg void OnOkBtnClickedButton2();                      //确定
	afx_msg void OnAutoModelBtnClickedRadio1();                //自动模式
	afx_msg void OnManualModelBnClickedRadio2();               //手动模式
	afx_msg void OnRandomChangeBtnClickedButton1();            //随机修改偏移量
	afx_msg void OnRandomIntervalBtnClickedButton3();          //随机间隔
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CEdit   m_OffsetCtl;
	CButton m_RandomOffsetBtn;
	CEdit   m_IntervalCtl;
	CButton m_RandomIntervalBtn;
	CDateTimeCtrl m_ManualTimeCtl;                              //年月日控件
	CDateTimeCtrl m_ManalTimeSubCtl;                            //时分秒控件
	/*************************************************************/
	bool  modelflag;                                            //模式标记 false-手动 true-自动
	bool  autostartflag;                                        //自动修改开始标志
	bool  offsetfalg;                                           //偏移量随机标记
	bool  intervalfalg;                                         //间隔随机标记
	int   m_OffSetCtlVal;                                       //偏移量变量     
	CTime m_ManaulSetCtlVal;                                    //年月日变量
	CTime m_MaunalTimeSubCtlVal;                                //时分秒变量
	UINT  m_IntervalCtlVal;                                     //间隔变量          
	
	
	CButton m_OkBtn;                                            //确定,开始，结束按钮
	BOOL m_Radio2;                                              //必须是BOOL类型且一个组只能设置一个变量就是Group为TRUE的那个Ctrl+D进行分组Tab顺序，分好之后再进行设置http://blog.csdn.net/c_cyoxi/article/details/23868979；http:www.169it.com/tech-cpp/article-8083971887749772398.html
	CWinThread* pPlayerThread;
};
