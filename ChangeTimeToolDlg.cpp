
// ChangeTimeToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChangeTimeTool.h"
#include "ChangeTimeToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT autoChangeSysTimeThread(LPVOID* aParam);                       //自动修改系统时间
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChangeTimeToolDlg dialog




CChangeTimeToolDlg::CChangeTimeToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChangeTimeToolDlg::IDD, pParent)
	, m_OffSetCtlVal(0)
	, m_ManaulSetCtlVal(0)
	, m_IntervalCtlVal(0)
	, m_MaunalTimeSubCtlVal(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CChangeTimeToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_OffSetCtlVal);
	DDV_MinMaxInt(pDX, m_OffSetCtlVal, 60, 86400);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_ManaulSetCtlVal);
	DDX_Text(pDX, IDC_EDIT3, m_IntervalCtlVal);
	DDV_MinMaxUInt(pDX, m_IntervalCtlVal, 1, 60);
	DDX_Control(pDX, IDC_EDIT2, m_OffsetCtl);
	DDX_Control(pDX, IDC_BUTTON1, m_RandomOffsetBtn);
	DDX_Control(pDX, IDC_EDIT3, m_IntervalCtl);
	DDX_Control(pDX, IDC_BUTTON3, m_RandomIntervalBtn);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_ManualTimeCtl);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_MaunalTimeSubCtlVal);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_ManalTimeSubCtl);
	DDX_Control(pDX, IDC_BUTTON2, m_OkBtn);



	DDX_Radio(pDX, IDC_RADIO1, m_Radio2);
}

BEGIN_MESSAGE_MAP(CChangeTimeToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO1, &CChangeTimeToolDlg::OnAutoModelBtnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CChangeTimeToolDlg::OnManualModelBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON1, &CChangeTimeToolDlg::OnRandomChangeBtnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CChangeTimeToolDlg::OnOkBtnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CChangeTimeToolDlg::OnRandomIntervalBtnClickedButton3)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CChangeTimeToolDlg message handlers

BOOL CChangeTimeToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	pPlayerThread=NULL;
	m_OffSetCtlVal=60;
	m_IntervalCtlVal=1;                                    //初始化时间间隔
	modelflag=false;
	autostartflag=false;
	offsetfalg=false;

	m_Radio2=1;                                            //默认选择手动
	m_OffsetCtl.EnableWindow(false);
	m_RandomOffsetBtn.EnableWindow(false);
	m_IntervalCtl.EnableWindow(false);
	m_RandomIntervalBtn.EnableWindow(false);

	offsetfalg=false;                                           
	intervalfalg=false; 

	m_ManaulSetCtlVal=CTime::GetCurrentTime();            //初始化本地时间
	m_MaunalTimeSubCtlVal=CTime::GetCurrentTime();

	this->UpdateData(false);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChangeTimeToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChangeTimeToolDlg::OnPaint()
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
HCURSOR CChangeTimeToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CChangeTimeToolDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)  
	{  
		switch(pMsg->wParam)  
		{  
		case VK_ESCAPE:           //Esc按键事件  
			return true;  
		case VK_RETURN:           //Enter按键事件 
			OnOkBtnClickedButton2();
			return true;
		case VK_F1:               //开启帮助信息
			return true;
		default:  
			;  
		}  
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CChangeTimeToolDlg::OnAutoModelBtnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	m_Radio2=0;
	modelflag=true;
	m_OffsetCtl.EnableWindow(true);
	m_RandomOffsetBtn.EnableWindow(true);
	m_IntervalCtl.EnableWindow(true);
	m_RandomIntervalBtn.EnableWindow(true);

	m_ManualTimeCtl.EnableWindow(false);
	m_ManalTimeSubCtl.EnableWindow(false);
	m_OkBtn.SetWindowText(_T("开    始"));
	this->UpdateData(false);
}


void CChangeTimeToolDlg::OnManualModelBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	if(autostartflag)
	{
		::MessageBox(NULL,_T("请先结束自动修改任务."),_T("提示"),0);
		m_Radio2=0;
		this->UpdateData(false);
		return ;
	}
	m_Radio2=1;
	modelflag=false;
	offsetfalg=false;
	intervalfalg=false;
	m_OffsetCtl.EnableWindow(false);
	m_RandomOffsetBtn.EnableWindow(false);
	m_IntervalCtl.EnableWindow(false);
	m_RandomIntervalBtn.EnableWindow(false);
	m_ManualTimeCtl.EnableWindow(true);
	m_ManalTimeSubCtl.EnableWindow(true);
	GetDlgItem(IDC_STATESTATIC)->SetWindowText(_T(""));
	m_OkBtn.SetWindowText(_T("确    定"));
	this->UpdateData(false);
}

void CChangeTimeToolDlg::OnRandomChangeBtnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if(!offsetfalg)
	{
		offsetfalg=true;
		m_OffsetCtl.EnableWindow(false);
		m_RandomOffsetBtn.SetWindowText(_T("关闭随机"));
	}
	else
	{
		offsetfalg=false;
		m_OffsetCtl.EnableWindow(true);
		m_RandomOffsetBtn.SetWindowText(_T("随机偏移"));
	}
	this->UpdateData(false);
}

void CChangeTimeToolDlg::OnRandomIntervalBtnClickedButton3()
{
	// TODO: Add your control notification handler code here
	if(!intervalfalg)
	{
		intervalfalg=true;
		m_IntervalCtl.EnableWindow(false);
		m_RandomIntervalBtn.SetWindowText(_T("关闭随机"));
	}
	else
	{
		intervalfalg=false;
		m_IntervalCtl.EnableWindow(true);
		m_RandomIntervalBtn.SetWindowText(_T("随机间隔"));
	}
	this->UpdateData(false);
}
/*
typedef struct _SYSTEMTIME {
    WORD wYear;  实际值
    WORD wMonth;  1~12
    WORD wDayOfWeek; 0~6
    WORD wDay;     1~31
    WORD wHour;   0~23
    WORD wMinute; 0~59
    WORD wSecond; 0~59
    WORD wMilliseconds; 0~999
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;
*/
void CChangeTimeToolDlg::OnOkBtnClickedButton2()
{
	// TODO: Add your control notification handler code here
	this->UpdateData(true);
	if(!modelflag)                              //手动
	{
		SYSTEMTIME   st;
		memset(&st,0,sizeof(st));
		GetSystemTime(&st);
		/*
		CTime ct=CTime::GetCurrentTime();
		st.wYear=ct.GetYear();
		
		st.wMonth=ct.GetMonth();
		st.wDayOfWeek=ct.GetDayOfWeek();
		st.wDay=ct.GetDay();
		st.wHour=ct.GetHour();
		st.wMinute=ct.GetMinute();
		st.wSecond=ct.GetSecond();
		*/
		/*
		CString msg1;
		msg1.Format(_T("Cur Y:%d M:%d D:%d H:%d::M:%d S:%d"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
		::MessageBox(NULL,msg1,_T("提示"),0);
		*/
		st.wYear=m_ManaulSetCtlVal.GetYear();
		st.wMonth=m_ManaulSetCtlVal.GetMonth();
		st.wDay=m_ManaulSetCtlVal.GetDay();

		st.wHour=m_MaunalTimeSubCtlVal.GetHour();

		if(m_MaunalTimeSubCtlVal.GetHour()>=8)
		{
			st.wHour=m_MaunalTimeSubCtlVal.GetHour()-8;
		}
		else
		{
			st.wHour=m_MaunalTimeSubCtlVal.GetHour()+16;
		}

		st.wMinute=m_MaunalTimeSubCtlVal.GetMinute();
		st.wSecond=m_MaunalTimeSubCtlVal.GetSecond();
		/*
		CString msg;
		msg.Format(_T("Cur Y:%d M:%d D:%d H:%d::M:%d S:%d"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
		::MessageBox(NULL,msg,_T("提示"),0);
		*/
		SetSystemTime(&st);
	}
	else                                       //自动
	{
		if(!autostartflag)
		{
			if(!offsetfalg&&(this->m_OffSetCtlVal< 60||this->m_OffSetCtlVal> 86400))  return ;
			if(!intervalfalg&&(this->m_IntervalCtlVal<1||this->m_IntervalCtlVal>60)) return ;

			autostartflag=true;
			//终止上一个自动修改时间线程
			if(pPlayerThread!=NULL)
			{
				TerminateThread(pPlayerThread->m_hThread,0);//强行终止线程
				pPlayerThread=NULL;
			}
			pPlayerThread =AfxBeginThread((AFX_THREADPROC)autoChangeSysTimeThread,this);
			//IDC_STATESTATIC显示状态的控件
			m_OffsetCtl.EnableWindow(false);
			m_RandomOffsetBtn.EnableWindow(false);
			m_IntervalCtl.EnableWindow(false);
			m_RandomIntervalBtn.EnableWindow(false);
			m_OkBtn.SetWindowText(_T("结    束"));
		}
		else
		{
			m_OkBtn.SetWindowText(_T("开    始"));
			GetDlgItem(IDC_STATESTATIC)->SetWindowText(_T(""));
			autostartflag=false;
			if(offsetfalg)
			{
				m_OffsetCtl.EnableWindow(false);
				m_RandomOffsetBtn.EnableWindow(true);
			}
			else
			{
				m_OffsetCtl.EnableWindow(true);
				m_RandomOffsetBtn.EnableWindow(false);
			}
			if(intervalfalg)
			{
				m_IntervalCtl.EnableWindow(false);
				m_RandomIntervalBtn.EnableWindow(true);
			}
			else
			{
				m_IntervalCtl.EnableWindow(true);
				m_RandomIntervalBtn.EnableWindow(false);
			}
		}
	}
	this->UpdateData(false);
}
/*生成随机数*/
unsigned int CChangeTimeToolDlg::RandData(unsigned int max,unsigned int min)
{
	CTime ct=CTime::GetCurrentTime();
	::srand(ct.GetSecond());
	unsigned int tval=::rand()%max;
	while(tval<min)
	{
		tval=::rand()%max;
		Sleep(1);
	}
	return tval;
}
/*自动修改时间线程*/
UINT autoChangeSysTimeThread(LPVOID* aParam)                        //自动修改系统时间
{
	CChangeTimeToolDlg* dlg=(CChangeTimeToolDlg*)aParam;
	SYSTEMTIME   st;
	unsigned offset=0,interval=0;
	/******************************************************************************/
	unsigned  int TmpSval=0;
	unsigned  int _tmp=0;                                                //中间变量
	unsigned long long _cnt=0;
	while(dlg->modelflag&&dlg->autostartflag)
	{
		if(dlg->offsetfalg)                                                  //随机偏移
			offset=dlg->RandData(86400,60);
		else
			offset=dlg->m_OffSetCtlVal;
		if(dlg->intervalfalg)                                                //随机间隔
			interval=dlg->RandData(60,1);
		else
			interval=dlg->m_IntervalCtlVal;

		memset(&st,0,sizeof(st));
		GetSystemTime(&st);
		TmpSval =st.wSecond;
		TmpSval+=offset;
		if(TmpSval>=60)
		{
			st.wSecond=TmpSval%60;
			_tmp=st.wMinute+TmpSval/60;
			if(_tmp>=60)
			{
				st.wMinute=_tmp%60;
				_tmp=st.wHour+_tmp/60;
				if(_tmp>=24)
				{
					st.wHour=_tmp%24;
					//////////////////////////////////
					unsigned int _dw=_tmp/24;                       //天数偏移
					_tmp=st.wDay-1+_tmp/24;

					unsigned int MaxDay=0;                          //保存一个月的最大天数
					switch(st.wMonth)
					{
					case 1:
					case 3:
					case 5:
					case 7:
					case 8:
					case 10:
					case 12:
						{
							MaxDay=31;
							break;
						}
					case 2:
						{
							if (st.wYear % 4 == 0)              //如果这个年份能被4整除  
							{  
								if (st.wYear % 100 == 0)        //如果这个年份能被100整除  
								{  
									if (st.wYear % 400 == 0)    //如果这个年份能被400整除  
										MaxDay = 29;  
									else  
										MaxDay = 28;            //这个数不能被400整除则将leap置为0  
								}  
								else  
									MaxDay = 29;                //这个数不能被100整除则将leap置为1  
							}  
							else  
								MaxDay = 28;                    //这个数不能被这三个数中的任何一个数整除 
							break;
						}
					case 4:
					case 6:
					case 9:
					case 11:
						{
							MaxDay=30;
							break;
						}
					}

					if(_tmp>=MaxDay)                                    //31天的1,3,5,7,8,10,12  
					{
						st.wDay=_tmp%MaxDay+1;
						if((st.wDayOfWeek+_dw)>=7)
						{
							st.wDayOfWeek=(st.wDayOfWeek+_dw)%7;
						}
						else
						{
							st.wDayOfWeek+=_dw;
						}
						_tmp=st.wMonth-1+_tmp/MaxDay;

						if(_tmp>=12)
						{
							st.wMonth=_tmp%12+1;
							st.wYear+=_tmp/12;
						}
						else
							st.wMonth=_tmp+1;
					}
					else
					{
						st.wDay=_tmp+1;
						if((st.wDayOfWeek+_dw)>=7)
						{
							st.wDayOfWeek=(st.wDayOfWeek+_dw)%7;
						}
						else
						{
							st.wDayOfWeek+=_dw;
						}
					}
				}
				else
				{
					st.wHour=_tmp;
				}
			}
			else
				st.wMinute=_tmp;
		}
		else
			st.wSecond=TmpSval;
	
		SetSystemTime(&st);
		CString CntStr;
		CntStr.Format(_T("%ld"),++_cnt);
		dlg->GetDlgItem(IDC_STATESTATIC)->SetWindowText(CntStr);
		Sleep(interval*60*1000);
		//Sleep(10*1000);                                                          //测试用
	}
	dlg->autostartflag=false;
	dlg->m_OkBtn.SetWindowText(_T("开    始"));
	dlg->GetDlgItem(IDC_STATESTATIC)->SetWindowText(_T(""));
	return 0;
}

void CChangeTimeToolDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	modelflag=false;
	Sleep(30);
	CDialogEx::OnClose();
}
