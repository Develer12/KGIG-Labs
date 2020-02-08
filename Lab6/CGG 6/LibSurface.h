#pragma once

#include <vector>

using namespace std;

typedef vector<CMatrix> CVecMatrix;
typedef vector<CVecMatrix> CMasMatrix;

typedef vector<CPoint> CVecPoint;
typedef vector<CVecPoint> CMatrPoint;

double function1(double x, double y);
double function2(double x, double y);
double function3(double x, double y);

class CPlot3D
{
private:
	pfunc2 _pFunc;
	CRectD _rectSpace;
	CMasMatrix _matrixFunction;
	CMasMatrix _matrixView;
	CRectD _rectView;
	CRect _rectWindow;
	CMatrix _viewport;
	CMatrPoint _matrixWindow;

public:
	CPlot3D();
	~CPlot3D() { _pFunc = NULL; };
	void SetFunction(pfunc2 pF, CRectD RS, double dx, double dy);
	void SetViewPoint(double r, double fi, double q);
	CMatrix GetViewPoint();
	void SetWinRect(CRect Rect);
	void CreateMatrixFunction(double dx, double dy);
	void CreateMatrixView();
	void CreateMatrixWindow();
	void Draw(CDC& dc);
};