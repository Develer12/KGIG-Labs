#include "stdafx.h"
#include "CMatrix.h"
#include "LibGraph.h"
#include "LibSurface.h"
#include <cmath>
//формулы
double function1(double x, double y)
{
	return x * x + y * y;
}

double function2(double x, double y)
{
	return x * x - y * y;
}

double function3(double x, double y)
{
	if(y!=0 && x!=0)
		if((1 - x * x - y * y)>=0)
	return sqrt(1 - x * x - y * y);
}

CPlot3D::CPlot3D()//конструктор по умолчанию
{
	_pFunc = NULL;
	_viewport.RedimMatrix(3);
	_rectWindow.SetRect(0, 0, 200, 200);//пр€моугольник
	_viewport(0) = 10;
	_viewport(1) = 30;
	_viewport(2) = 45;
}

void CPlot3D::SetFunction(pfunc2 pF, CRectD RS, double dx, double dy)//очистка и применение функций
{
	_pFunc = pF;//передаем формулу фигуры в пространстве
	_rectSpace.SetRectD(RS.left, RS.top, RS.right, RS.bottom);//размер фигуры в мировых
	_matrixFunction.clear();//очищаем предыдущую фигуру
	_matrixView.clear();//очищаем предыдущий в вид
	_matrixWindow.clear();//очищаем

	CreateMatrixFunction(dx, dy);
	CreateMatrixView();
	CreateMatrixWindow();
}

void CPlot3D::SetViewPoint(double r, double fi, double q)//определение точек
{
	_viewport(0) = r;
	_viewport(1) = fi;
	_viewport(2) = q;
	_matrixView.clear();
	CreateMatrixView();
	_matrixWindow.clear();
	CreateMatrixWindow();
}

CMatrix CPlot3D::GetViewPoint()//задание точек
{
	CMatrix P = _viewport;
	return P;
}

void CPlot3D::SetWinRect(CRect Rect)//пр€моугольник дл€ рисовани€ функций
{
	_rectWindow = Rect;
	_matrixWindow.clear();
	CreateMatrixWindow();
}

void CPlot3D::CreateMatrixFunction(double dx, double dy)//создаем матрицу фигуры по формуле
{
	double xL = _rectSpace.left;//пространство дл€ пр€моугольника
	double xH = _rectSpace.right;
	double yL = _rectSpace.bottom;
	double yH = _rectSpace.top;
	CVecMatrix VecMatrix;
	CMatrix V(4);
	V(3) = 1;
	for (double x = xL; x <= xH; x += dx)
	{
		VecMatrix.clear();
		for (double y = yL; y <= yH; y += dy)
		{
			V(0) = x;
			V(1) = y;
			V(2) = _pFunc(x, y);
			VecMatrix.push_back(V);// ƒобавление элемента в конец вектора
		}
		_matrixFunction.push_back(VecMatrix); // ƒобавление вектора точки в конец вектора матрицы
	}
}

void CPlot3D::CreateMatrixView()//матрицца дл€ рисовани€ плоскостей
{
	CMatrix MV = CreateViewCoord(_viewport(0), _viewport(1), _viewport(2));
	CVecMatrix VecMatrix;//вектор матрицы
	CMatrix VX(4), V(3);
	V(2) = 1;
	//задаем крайние точки
	double xmin = DBL_MAX;
	double xmax = DBL_MIN;
	double ymin = DBL_MAX;
	double ymax = DBL_MIN;

	for (int i = 0; i < _matrixFunction.size(); i++)//заполн€е матрицу
	{
		VecMatrix.clear();
		for (int j = 0; j < _matrixFunction[i].size(); j++)
		{
			VX = _matrixFunction[i][j];
			VX = MV * VX;
			V(0) = VX(0);
			V(1) = VX(1);
			VecMatrix.push_back(V);// ƒобавление элемента в конец вектора

			double x = V(0);
			double y = V(1);
			//переопредел€ем крайние точки крайние точки
			if (x < xmin)
				xmin = x;
			if (x > xmax)
				xmax = x;
			if (y < ymin)
				ymin = y;
			if (y > ymax)
				ymax = y;
		}
		_matrixView.push_back(VecMatrix);// ƒобавление вектора точки в конец вектора матрицы
	}
	_rectView.SetRectD(xmin, ymax, xmax, ymin);
}

void CPlot3D::CreateMatrixWindow()//оконна€ матрица
{
	CMatrix MW = SpaceToWindow(_rectView, _rectWindow);//из пространственных в оконные коорд
	CVecPoint VecPoint;
	CMatrix  V(3);
	for (int i = 0; i < _matrixView.size(); i++)
	{
		VecPoint.clear();//очищаем вектор точки
		for (int j = 0; j < _matrixView[i].size(); j++)
		{
			V = _matrixView[i][j];
			V = MW * V;
			CPoint P((int)V(0), (int)V(1));
			VecPoint.push_back(P);// ƒобавление элемента в конец вектора
		}
		_matrixWindow.push_back(VecPoint);// ƒобавление вектора точки в конец вектора матрицы
	}
}

void CPlot3D::Draw(CDC& dc)//рисуем плоскости по функци€м
{
	if (_matrixWindow.empty())//проверка матрицы функции на пустоту
	{
		TCHAR* error = _T("ћассив данных дл€ рисовани€ в окне пуст! ");
		MessageBox(NULL, error, _T("ќшибка"), MB_ICONSTOP);
		return;
	}
	CPoint pt[4];
	int nRows = _matrixWindow.size();
	int nCols = _matrixWindow[0].size();
	for (int i = 0; i < nRows - 1; i++)//заполн€ем матрицу
	{
		for (int j = 0; j < nCols - 1; j++)
		{
			pt[0] = _matrixWindow[i][j];
			pt[1] = _matrixWindow[i][j + 1];
			pt[2] = _matrixWindow[i + 1][j + 1];
			pt[3] = _matrixWindow[i + 1][j];
			dc.Polygon(//¬ыводит многоугольник, состо€щий из двух или больше вершин,
						//св€занных лин€ими, использу€ текущее перо, в нашем случае выводит фигуру по формуле
				pt, //массив точек, которые определ€ет вершину многоугольника
				4);//число вершин массива (—только, сколько сторон в окне)
		}
	}
}