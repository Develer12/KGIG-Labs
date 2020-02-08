#include "stdafx.h"
#include "CGG 6.h"
#include "ChildView.h"
#include "CMatrix.h"
#include "LibGraph.h"
#include "LibSurface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView

CChildView::CChildView()
{
	Index = 0;
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_FUNCTIONS_F1, &CChildView::OnFunctionsF1)
	ON_COMMAND(ID_FUNCTIONS_F2, &CChildView::OnFunctionsF2)
	ON_COMMAND(ID_FUNCTIONS_F3, &CChildView::OnFunctionsF3)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// обработчики сообщений CChildView

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint()//отрисовка плоскостей
{
	CPaintDC dc(this);
	switch (Index)
	{
	case 1:
		Graph1.Draw(dc);
		break;
	case 2:
		Graph2.Draw(dc);
		break;
	case 3:
		Graph3.Draw(dc);
		break;
	}
}

void CChildView::OnFunctionsF1()
{
	double dx = 0.25, dy = 0.25;//шаг отрисовки
	R = 50, Phi = 30, Q = 45;//параметры камеры
	CRectD SpaceRect(-5, 5, 5, -5);
	CRect  WinRect;
	GetClientRect(WinRect);
	WinRect.SetRect(WinRect.left + 50, WinRect.top + 50, WinRect.right - 50, WinRect.bottom - 50);
	Graph1.SetFunction(//задаем функцию
		function1,//передаем формулу по которой будет рисовать
		SpaceRect,//передаем  значения диапазона отрисовки
		dx, dy);//шаг
	Graph1.SetViewPoint(R, Phi, Q);//задаем параметры камеры
	Graph1.SetWinRect(WinRect);//задаем прямоугольник в котором будем рисовать
	Index = 1;//передаем параметр отрисовки
	Invalidate();
}

void CChildView::OnFunctionsF2()
{
	double dx = 0.25, dy = 0.25;//шаг отрисовки
	R = 50, Phi = 30, Q = 45;//параметры камеры
	CRectD SpaceRect(-5, 5, 5, -5);//значения диапазона по х и у (у обоих от -5 до 5)
	CRect  WinRect;
	GetClientRect(WinRect);//извлекаем коорд рабочей области окна в WinRect
	WinRect.SetRect(WinRect.left + 50, WinRect.top + 50, WinRect.right - 50, WinRect.bottom - 50);
	Graph2.SetFunction(//задаем функцию
		function2,//передаем формулу по которой будет рисовать
		SpaceRect,//передаем  значения диапазона отрисовки
		dx, dy);//шаг
	Graph2.SetViewPoint(R, Phi, Q);//задаем параметры камеры
	Graph2.SetWinRect(WinRect);//задаем прямоугольник в котором будем рисовать
	Index = 2;//передаем параметр отрисовки
	Invalidate();
}

void CChildView::OnFunctionsF3()
{
	double dx = 0.2, dy = 0.2;//шаг
	R = 50, Phi = 30, Q = 45;//параметры камеры
	CRectD SpaceRect(-5, 5, 5, -5);//значения диапазона по х и у (у обоих от -5 до 5)
	CRect  WinRect;
	GetClientRect(WinRect);
	WinRect.SetRect(WinRect.left + 200, WinRect.top + 100, WinRect.right - 50, WinRect.bottom - 50);
	Graph3.SetFunction(//задаем функцию
		function3,//передаем формулу по которой будет рисовать
		SpaceRect,//передаем  значения диапазона отрисовки
		dx, dy);//шаг
	Graph3.SetViewPoint(R, Phi, Q);//задаем параметры камеры
	Graph3.SetWinRect(WinRect);//задаем прямоугольник в котором будем рисовать
	Index = 3;//передаем параметр отрисовки
	Invalidate();
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)//вращение фигуры
{
	CMatrix p(3);
	switch (Index)
	{
	case 1:
	{
		p = Graph1.GetViewPoint();
		break;
	}
	case 2:
	{
		p = Graph2.GetViewPoint();
		break;
	}
	case 3:
	{
		p = Graph3.GetViewPoint();
		break;
	}
	}

	R = p(0), Phi = p(1), Q = p(2);
	double deltaPhi = 5, deltaQ = 5;
	switch (nChar)
	{
	case VK_UP:
	{
		double qx = Q - deltaQ;
		if (qx >= 0)
			Q = qx;
		break;
	}
	case VK_DOWN:
	{
		double qx = Q + deltaQ;
		if (qx <= 180)
			Q = qx;
		break;
	}
	case VK_LEFT:
	{
		double fix = Phi - deltaPhi;
		if (fix >= 0)
			Phi = fix;
		break;
	}
	case VK_RIGHT:
	{
		double fix = Phi + deltaPhi;
		if (fix <= 360)
			Phi = fix;
		break;
	}
	}

	switch (Index)
	{
	case 1:
	{
		Graph1.SetViewPoint(R, Phi, Q);
		break;
	}

	case 2:
	{
		Graph2.SetViewPoint(R, Phi, Q);
		break;
	}

	case 3:
	{
		Graph3.SetViewPoint(R, Phi, Q);
		break;
	}
	}
	Invalidate();
}