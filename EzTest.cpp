#include "stdafx.h"
#include "EzFont.h"

// 17-3 EZTEST
void PaintRoutine(HWND hwnd, HDC hdc, int cxArea, int cyArea)
{
	HFONT hFont;
	int y, iPointSize;
	LOGFONT lf;
	TCHAR szBuffer[100];
	TEXTMETRIC tm;
	// Set Logical Twips mapping mode
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetWindowExtEx(hdc, 1440, 1440, NULL);
	SetViewportExtEx(hdc, GetDeviceCaps(hdc, LOGPIXELSX), GetDeviceCaps(hdc, LOGPIXELSY), NULL);
	// Try some fonts
	y = 0;
	for (iPointSize = 80; iPointSize <= 120; iPointSize++)
	{
		hFont = EzCreateFont(hdc, TEXT("Times New Roman"), iPointSize, 0, 0, TRUE);
		GetObject(hFont, sizeof(LOGFONT), &lf);
		SelectObject(hdc, hFont);
		GetTextMetrics(hdc, &tm);
		TextOut(hdc, 0, y, szBuffer,
			wsprintf(szBuffer,
				TEXT("Times New Roman font of %i.%i points, ")
				TEXT("lf.lfHeight = %i, tm.tmHeight = %i"),
				iPointSize / 10, iPointSize % 10,
				lf.lfHeight, tm.tmHeight));
		DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
		y += tm.tmHeight;
	}
}

// 17-4 FONTROT
void PaintRotation(HWND hwnd, HDC hdc, int cxArea, int cyArea)
{
	static TCHAR szString[] = TEXT(" Rotation");
	HFONT hFont;
	int i;
	LOGFONT lf;
	hFont = EzCreateFont(hdc, TEXT("Times New Roman"), 540, 0, 0, TRUE);
	GetObject(hFont, sizeof(LOGFONT), &lf);
	DeleteObject(hFont);
	SetBkMode(hdc, TRANSPARENT);
	SetTextAlign(hdc, TA_BASELINE);
	SetViewportOrgEx(hdc, cxArea / 2, cyArea / 2, NULL);
	for (i = 0; i < 12; i++)
	{
		lf.lfEscapement = lf.lfOrientation = i * 300;
		SelectObject(hdc, CreateFontIndirect(&lf));
		TextOut(hdc, 0, 0, szString, lstrlen(szString));
		DeleteObject(SelectObject(hdc, GetStockObject
		(SYSTEM_FONT)));
	}
}

// 17-10 FONTOUT1
void PaintOut1(HWND hwnd, HDC hdc, int cxArea, int cyArea)
{
	static TCHAR szString[] = TEXT("Outline");
	HFONT hFont;
	SIZE size;
	hFont = EzCreateFont(hdc, TEXT("Times New Roman"), 1440, 0, 0, TRUE);
	SelectObject(hdc, hFont);
	GetTextExtentPoint32(hdc, szString, lstrlen(szString), &size);
	BeginPath(hdc);
	TextOut(hdc, (cxArea - size.cx) / 2, (cyArea - size.cy) / 2, szString, lstrlen(szString));
	EndPath(hdc);
	StrokePath(hdc);
	SelectObject(hdc, GetStockObject(SYSTEM_FONT));
	DeleteObject(hFont);
}

// 17-11 FONTOUT2
void PaintOut2(HWND hwnd, HDC hdc, int cxArea, int cyArea)
{
	static TCHAR szString[] = TEXT("Outline");
	HFONT hFont;
	LOGBRUSH lb;
	SIZE size;
	hFont = EzCreateFont(hdc, TEXT("Times New Roman"), 1440, 0, 0, TRUE);
	SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);
	GetTextExtentPoint32(hdc, szString, lstrlen(szString), &size);
	BeginPath(hdc);
	TextOut(hdc, (cxArea - size.cx) / 2, (cyArea - size.cy) / 2,
		szString, lstrlen(szString));
	EndPath(hdc);
	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(255, 0, 0);
	lb.lbHatch = 0;
	SelectObject(hdc, ExtCreatePen(PS_GEOMETRIC | PS_DOT,
		GetDeviceCaps(hdc, LOGPIXELSX) / 24, &lb, 0, NULL));
	StrokePath(hdc);
	DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
	SelectObject(hdc, GetStockObject(SYSTEM_FONT));
	DeleteObject(hFont);
}

// 17-12 FONTFILL
void PaintFill(HWND hwnd, HDC hdc, int cxArea, int cyArea)
{
	static TCHAR szString[] = TEXT("Filling");
	HFONT hFont;
	SIZE size;
	hFont = EzCreateFont(hdc, TEXT("Times New Roman"), 1440, 0, 0, TRUE);
	SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);
	GetTextExtentPoint32(hdc, szString, lstrlen(szString), &size);
	BeginPath(hdc);
	TextOut(hdc, (cxArea - size.cx) / 2, (cyArea - size.cy) / 2,
		szString, lstrlen
		(szString));
	EndPath(hdc);
	SelectObject(hdc, CreateHatchBrush(HS_DIAGCROSS, RGB(255, 0, 0)));
	SetBkColor(hdc, RGB(0, 0, 255));
	SetBkMode(hdc, OPAQUE);
	StrokeAndFillPath(hdc);
	DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
	SelectObject(hdc, GetStockObject(SYSTEM_FONT));
	DeleteObject(hFont);
}

// 17-13 FONTCLIP
void PaintClip(HWND hwnd, HDC hdc, int cxArea, int cyArea)
{
	static TCHAR szString[] = TEXT("Clipping");
	HFONT hFont;
	int y, iOffset;
	POINT pt[4];
	SIZE size;
	hFont = EzCreateFont(hdc, TEXT("Times New Roman"), 1200, 0, 0, TRUE);
	SelectObject(hdc, hFont);
	GetTextExtentPoint32(hdc, szString, lstrlen(szString), &size);
	BeginPath(hdc);
	TextOut(hdc, (cxArea - size.cx) / 2, (cyArea - size.cy) / 2, szString, lstrlen(szString));
	EndPath(hdc);
	// Set clipping area
	SelectClipPath(hdc, RGN_COPY);
	// Draw Bezier splines
	iOffset = (cxArea + cyArea) / 4;
	for (y = -iOffset; y < cyArea + iOffset; y++)
	{
		pt[0].x = 0;
		pt[0].y = y;
		pt[1].x = cxArea / 3;
		pt[1].y = y + iOffset;
		pt[2].x = 2 * cxArea / 3;
		pt[2].y = y - iOffset;
		pt[3].x = cxArea;
		pt[3].y = y;
		SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(rand() % 256, rand() % 256, rand() % 256)));
		PolyBezier(hdc, pt, 4);
		DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
	}
	DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
	SelectObject(hdc, GetStockObject(SYSTEM_FONT));
	DeleteObject(hFont);
}
