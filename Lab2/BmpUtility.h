#pragma once

HBITMAP LoadBmp(CString);

void ShowBmp(HWND, HBITMAP, int, int);

void SaveBmp(HWND, RECT&, CString);