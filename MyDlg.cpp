
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "DispZad.h"
#include "MyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DISPZAD_DIALOG, pParent)
	, findPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, findPath);
	DDX_Control(pDX, IDC_LIST1, prgList);
	DDX_Control(pDX, IDC_LIST2, actPrgList);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FIND, &MyDlg::OnBnClickedButtonFind)
	ON_BN_CLICKED(IDC_BUTTON_START_NEW_PROCESS, &MyDlg::OnBnClickedButtonStartNewProcess)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL_PROCESS, &MyDlg::OnBnClickedButtonCancelProcess)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	log.open("log.txt");

	timerid = SetTimer(TIMER1, 1000, NULL);
	if (timerid)log << "Timer init: timer is started\n";
	else log << "Timer init: timer is not started\n";

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void MyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void MyDlg::OnBnClickedButtonFind()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	if (!findPath.GetLength())return;
	
	CFileFind finder;
	auto temp = findPath + _T("\\*.exe");
	BOOL bWorking = finder.FindFile(temp);
	if (bWorking)prgList.ResetContent();
	while (bWorking)
	{
		bWorking = finder.FindNextFileW();
		prgList.AddString(finder.GetFileName());
	}
}


void MyDlg::OnBnClickedButtonStartNewProcess()
{
	// TODO: добавьте свой код обработчика уведомлений
	int id = prgList.GetCurSel();
	if (id == LB_ERR)return;

	wchar_t* fileName = new wchar_t[prgList.GetTextLen(id) + 1];
	prgList.GetText(id, fileName);

	wchar_t* path = new wchar_t[prgList.GetTextLen(id) + findPath.GetLength() + 3];
	StrCpy(path, findPath);
	wchar_t temp[2] = _T("\\");
	StrCat(path, temp);
	StrCat(path, fileName);

	STARTUPINFO si = { sizeof(si) };

	PROCESS_INFORMATION pi;
	if (CreateProcess(NULL, path, NULL, NULL, NULL, 0, NULL, NULL, &si, &pi))
	{
		actPrgList.AddString(fileName);
	}

	IdInfo.push_back(pi);

	delete[]path;
	delete[]fileName;
}


template <typename T> 
vector<T> deleteVecItem(vector<T> data, int id)
{
	if (data.size()==1)
	{
		data.clear();
		return data;
	}
	vector<T> res = data;
	for (int i = id; i < res.size() - 1; i++)
	{
		res[i] = res[i + 1];
	}
	res.pop_back();
	return res;
}
void MyDlg::OnBnClickedButtonCancelProcess()
{
	// TODO: добавьте свой код обработчика уведомлений
	int id = actPrgList.GetCurSel();
	if (id == LB_ERR)return;

	/*if (TerminateProcess(IdInfo[IdInfo.size() - id - 1].hProcess, 0))
	{
		IdInfo = deleteVecItem<PROCESS_INFORMATION>(IdInfo, IdInfo.size() - id - 1);
		actPrgList.DeleteString(id);
	}*/
	if (TerminateProcess(IdInfo[id].hProcess, 0))
	{
		IdInfo = deleteVecItem<PROCESS_INFORMATION>(IdInfo, id);
		actPrgList.DeleteString(id);
	}
}




afx_msg void MyDlg::OnTimer(UINT_PTR idEvent)
{
	
	log << "timer is working\n";
	KillTimer(timerid);
	for (int i = 0; i < IdInfo.size(); i++)
	{
		DWORD exitCode = 0;
		GetExitCodeProcess(IdInfo[i].hProcess, &exitCode);
		if (exitCode != STILL_ACTIVE)
		{
			IdInfo = deleteVecItem<PROCESS_INFORMATION>(IdInfo, i);
			actPrgList.DeleteString(i);
		}
	}


	timerid = SetTimer(TIMER1, 1000, NULL);


	CDialogEx::OnTimer(idEvent);
}