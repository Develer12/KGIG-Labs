
// CGG 6.h : главный файл заголовка для приложения CGG 6
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CCGG6App:
// О реализации данного класса см. CGG 6.cpp
//

class CCGG6App : public CWinApp
{
public:
	CCGG6App();


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCGG6App theApp;
