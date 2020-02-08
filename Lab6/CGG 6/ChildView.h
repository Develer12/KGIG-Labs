#pragma once
#include "CMatrix.h"
#include "LibGraph.h"
#include "LibSurface.h"

// окно CChildView

class CChildView : public CWnd
{
// Создание
public:
	CChildView();
	double R, Phi, Q;
	CRect WRect;
	int Index;
	CPlot3D Graph1, Graph2, Graph3;

// Атрибуты
public:

// Операции
public:

// Переопределение
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Реализация
public:
	virtual ~CChildView();

	// Созданные функции схемы сообщений
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFunctionsF1();
	afx_msg void OnFunctionsF2();
	afx_msg void OnFunctionsF3();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};