// ChildView.cpp : реализаци€ класса CChildView

#include "stdafx.h"
#include "Lab2.h"
#include "ChildView.h"
#include "BmpUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView

CChildView::CChildView()
{
	targetPoint.x = 0;
	targetPoint.y = 0;
	targetArea.top = 0;
	targetArea.left = 0;
	targetArea.bottom = 0;
	targetArea.right = 0;
	startSelected = false;
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_BMP_LOAD, &CChildView::OnBmpLoad)
	ON_COMMAND(ID_BMP_SAVE, &CChildView::OnBmpSave)
END_MESSAGE_MAP()

// обработчики сообщений CChildView

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}

void CChildView::OnPaint()
{
	CPaintDC dc(this); // контекст устройства дл€ рисовани€

					   // TODO: ƒобавьте код обработки сообщений

					   // Ќе вызывайте CWnd::OnPaint() дл€ сообщений рисовани€
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	RedrawWindow();//перезапись окна
	targetPoint = point;
	CDC* displayDc = GetDC();//получаем доступ к окну
	displayDc->Ellipse(targetPoint.x - 5, targetPoint.y - 5, targetPoint.x + 5, targetPoint.y + 5);//рисуем круг дл€ точки рисовани€
	ReleaseDC(displayDc);//возвращаем доступ, чтобы предотвратить учечки из GDI-графический интерфейс устройства

}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (startSelected)
	{
		targetArea.bottom = point.y;
		targetArea.right = point.x;
		CDC* displayDc = GetDC();//получаем доступ к окну
		displayDc->MoveTo(targetArea.left, targetArea.top);//точка начинает новый рисунок. Ёта точка становитс€ новой текущей позицией
		displayDc->LineTo(targetArea.right, targetArea.top);//лини€ выводитс€ от текущей позиции до этой точки, котора€ затем становитс€ новой текущей позицией.
		displayDc->MoveTo(targetArea.right, targetArea.top);
		displayDc->LineTo(targetArea.right, targetArea.bottom);
		displayDc->MoveTo(targetArea.right, targetArea.bottom);
		displayDc->LineTo(targetArea.left, targetArea.bottom);
		displayDc->MoveTo(targetArea.left, targetArea.bottom);
		displayDc->LineTo(targetArea.left, targetArea.top);
		ReleaseDC(displayDc);//возвращаем достум, чтобы предотвратить учечки из GDI-графический интерфейс устройства

		startSelected = false;
	}
	else
	{
		targetArea.top = point.y;
		targetArea.left = point.x;
		startSelected = true;
	}
}

void CChildView::OnBmpLoad() //«агрузка изображени€
{
	CFileDialog fileDialog(TRUE,//-открыть файл
		L"bmp", //ожидаемый файл
		L".bmp");//искомый формат
	INT_PTR result = fileDialog.DoModal();//ќткрытие проводника дл€ поиска файлов
	if (result == IDOK)//если проводник открылс€
	{
		ShowBmp(*this,
				LoadBmp(fileDialog.GetPathName()),
						targetPoint.x,
						targetPoint.y);
	}
	// TODO: добавьте свой код обработчика команд
}

void CChildView::OnBmpSave()// сохранение(сохран€ет только область выделенную пр€маугольником созданным правой кнопкой)
{
	CFileDialog fileDialog(FALSE,//- сохранить файл
		L"bmp", //ожидаемый файл
		L".bmp");//искомый формат
	INT_PTR result = fileDialog.DoModal();//ќткрытие проводника дл€ поиска файлов
	if (result == IDOK)//если проводник открылс€
	{
		SaveBmp(*this, targetArea,//размер задействованной области
			fileDialog.GetPathName());
		RedrawWindow();//перезапись окна
	}
	// TODO: добавьте свой код обработчика команд
}