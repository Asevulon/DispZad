﻿
// DispZad.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CDispZadApp:
// Сведения о реализации этого класса: DispZad.cpp
//

class CDispZadApp : public CWinApp
{
public:
	CDispZadApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CDispZadApp theApp;
