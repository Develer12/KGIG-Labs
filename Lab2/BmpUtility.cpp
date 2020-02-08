#include "stdafx.h"
#include "BmpUtility.h"

HBITMAP LoadBmp(CString fileName)
{
	HBITMAP hBmp = (HBITMAP)LoadImage(NULL,
		fileName,
		IMAGE_BITMAP, 0, 0, //SIZE
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	return hBmp;
}

void ShowBmp(HWND hWindow, HBITMAP hBmp, int x, int y)
{
	BITMAP bmp; //тут будет храниться файл в виде битов
	GetObject(hBmp,// сам файл bmp
		sizeof(BITMAP), //размер битполя
		(LPSTR)&bmp); //переводим в длинный стринг и помещаем туда файл

	HDC dcDisplay = GetDC(hWindow);
	HDC dcMemory = CreateCompatibleDC(dcDisplay);//создает  контекст памяти совместимый с контекстом отображения

	HBITMAP oldBmp = (HBITMAP)SelectObject(dcMemory, hBmp);  // Выбираем изображение bitmap в контекст памяти

	BitBlt(//рисуем изображение
		dcDisplay, x, y, //в окне и в каких координатах в целевом рисунке
		bmp.bmWidth, bmp.bmHeight, //размеры изображения
		dcMemory,
		0, 0, //в окне и в каких координатах в исходном рисунке
		SRCCOPY); //копируем исходник(код растровой операции)

	SelectObject(dcMemory, oldBmp);    // Восстанавливаем контекст памяти

	ReleaseDC(hWindow, dcDisplay); //освобождаем управление окном
	DeleteDC(dcMemory);// удаляем контекст памяти
}

void SaveBmp(HWND hWindow, RECT& area, CString fileName)
{
	HDC dcDisplay = GetDC(hWindow);
	HDC dcMemory = CreateCompatibleDC(dcDisplay);//создает  контекст памяти совместимый с контекстом отображения

	HBITMAP hBmp = CreateCompatibleBitmap(dcDisplay, //создание точечного рисунка
		abs(area.right - area.left),//модуль ширины рисунка
		abs(area.bottom - area.top));
	HBITMAP hOldBmp = (HBITMAP)SelectObject(dcMemory, hBmp); // Выбираем изображение bitmap в контекст памяти
	BitBlt(//рисуем изображение
		dcMemory, 0, 0, //в окне и в каких координатах в целевом рисунке
		abs(area.right - area.left), abs(area.bottom - area.top), //размеры изображения
		dcDisplay,
		area.left, area.top, //в окне и в каких координатах в исходном рисунке
		SRCCOPY);//копируем исходник(код растровой операции)


	hBmp = (HBITMAP)SelectObject(dcMemory, hOldBmp);// Восстанавливаем контекст памяти

	BITMAP bmp;
	BITMAPINFOHEADER bmpInfoHeader;//передаст структуру с инфой о изображении
	BITMAPFILEHEADER bmpFileHeader; //сюда запишем сам файл
	int colorDepth = 32; //глубина цвета

	GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp);// получаем объект

	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);//колличество байт необходимое структуре
	bmpInfoHeader.biWidth = bmp.bmWidth;//ширина
	bmpInfoHeader.biHeight = bmp.bmHeight;//высота
	bmpInfoHeader.biPlanes = 1;//колличество слоев для целевого устройства
	bmpInfoHeader.biBitCount = colorDepth; //колво битов на пиксел - глубина цвета
	bmpInfoHeader.biCompression = BI_RGB; //тип сжатия(смена формата, например на png или jpeg), тут без сжатия
	bmpInfoHeader.biSizeImage = 0; //размер изображения, для BI_RGB 0, тк без сжатия, если другой формат, то оно сожмет до заданного значения
	bmpInfoHeader.biXPelsPerMeter = 0; //разрешение в пикселях на метр целевого устройства для растрового изображения
	bmpInfoHeader.biYPelsPerMeter = 0;
	bmpInfoHeader.biClrUsed = 0; //Количество цветовых индексов в таблице цветов используемое. Если равно нулю, то юзает макс колво
	bmpInfoHeader.biClrImportant = 0; //Количество цветовых индексов, необходимых для отображения растрового изображения. Если  нуль, все цвета.

	DWORD dwBitSize = ((bmp.bmWidth * colorDepth + 31) / 32) *
		4 * bmp.bmHeight; //битовый размер
		DWORD dwDibSize = sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER) +
		dwBitSize;

	bmpFileHeader.bfType = ('M' << 8) | 'B'; //тип файла
	bmpFileHeader.bfSize = dwDibSize; //размер в байтах файла растрового изображения.
	bmpFileHeader.bfReserved1 = 0; //Зарезервированный
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
		(DWORD)sizeof(BITMAPINFOHEADER); //Смещение в байтах от начала структуры к битам битовой карты.

	HANDLE hDib = GlobalAlloc(//выделяет глобальный блок памяти
		GHND, //Во все байты блока записываются нулевые значения и Заказывается перемещаемый блок памяти. Логический адрес перемещаемого блока памяти может изменяться
		dwBitSize + sizeof(BITMAPINFOHEADER));//выделяемый размер
	LPBITMAPINFOHEADER lpBmpInfoHeader = (LPBITMAPINFOHEADER)GlobalLock(hDib); //выделяем память под Структуру BITMAPINFOHEADER содержит информацию о размерах и цветовом формате DIB(BMP).
	*lpBmpInfoHeader = bmpInfoHeader;

	GetDIBits(//извлекает биты заданного совместимого точечного рисунка и копирует их в буфер как DIB
		dcMemory,
		hBmp,  // дескриптор рисунка
		0, // первая устанавливаемая строка развертки
		(UINT)bmp.bmHeight,  // число копируемых строк развертки
		(LPSTR)lpBmpInfoHeader + sizeof(BITMAPINFOHEADER), // массив для битов рисунка
		(BITMAPINFO*)lpBmpInfoHeader, // буфер данных рисунка
		DIB_RGB_COLORS);// индексы RGB или палитры

	HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL |
		FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"Не удалось создать файл", L"Ошибка", MB_ICONSTOP);
		return;
	}

	DWORD dwWritten;

	WriteFile(hFile, // дескриптор файла
		(LPSTR)&bmpFileHeader, // буфер данных
		sizeof(BITMAPFILEHEADER),// число байтов для записи
		&dwWritten, // число записанных байтов
		NULL);// асинхронный буфер
	WriteFile(hFile, (LPSTR)lpBmpInfoHeader, dwDibSize,
		&dwWritten, NULL);

	GlobalUnlock(hDib);//Далее освобождение памяти
	GlobalFree(hDib);
	CloseHandle(hFile);

	ReleaseDC(hWindow, dcDisplay);
	DeleteDC(dcMemory);

	if (dwWritten == 0)
	{
		MessageBox(NULL, L"Не удалось записать файл", L"Ошибка", MB_ICONSTOP);
		return;
	}
}