// ChildView.h : интерфейс класса CChildView

#pragma once

// окно CChildView

class CChildView : public CWnd
{
	// Создание
public:
	CChildView();

	// Атрибуты
public:
	POINT targetPoint;
	RECT targetArea;
	bool startSelected;
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
	afx_msg void OnLButtonDown(UINT, CPoint);
	afx_msg void OnRButtonDown(UINT, CPoint);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBmpLoad();
	afx_msg void OnBmpSave();
};