
// MyDlg.h: файл заголовка
//
#define TIMER1 123456
#pragma once
#include<vector>
#include<fstream>
using namespace std;

// Диалоговое окно MyDlg
class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DISPZAD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString findPath;
	CListBox prgList;
	CListBox actPrgList;
	afx_msg void OnBnClickedButtonFind();
	afx_msg void OnBnClickedButtonStartNewProcess();
	afx_msg void OnBnClickedButtonCancelProcess();
	vector<PROCESS_INFORMATION> IdInfo;
	ofstream log;
	afx_msg void OnTimer(UINT_PTR);
	CString MyDlg::MakeUniqueListItem(CString item, CListBox& box);
	UINT_PTR timerid;
};
template<typename T>
vector<T> deleteVecItem(vector<T> data, int id);
