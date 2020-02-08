
// CGG 8.h : главный файл заголовка для приложения CGG 8
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CCGG8App:
// О реализации данного класса см. CGG 8.cpp
//

class CCGG8App : public CWinApp
{
public:
	CCGG8App();


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCGG8App theApp;
