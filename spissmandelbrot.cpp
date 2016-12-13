/*
 * Copyright (c) 2010-2016 Stephane Poirier
 *
 * stephane.poirier@oifii.org
 *
 * Stephane Poirier
 * 3532 rue Ste-Famille, #3
 * Montreal, QC, H2X 2L1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// spissmandelbrot.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "windows.h"
#include "scrnsave.h"
#include "resource.h"
#include "math.h"

#define MAX_BALLS	7
#define pi			3.141592625

#define szAppName	"spissmandelbrot 1.0"
#define szAuthor	"Written by Stephane Poirier, © 2014"
#define szPreview	"Mandelbrot 1.0"

typedef struct _BALLS
{
	UINT	uBallID;
	HICON	hIcon;
	int		x;
	int		y;
	int		angle;
}BALLS;

BALLS gBalls[] = {IDI_REDBALL,		NULL, 0, 0, 0,
				  IDI_GREENBALL,	NULL, 0, 0, 25,
				  IDI_BLUEBALL,		NULL, 0, 0, 50,
				  IDI_PURPLEBALL,	NULL, 0, 0, 75,
				  IDI_LIGHTREDBALL, NULL, 0, 0, 100,
				  IDI_YELLOWBALL,	NULL, 0, 0, 125,
				  IDI_BLACKBALL,	NULL, 0, 0, 150};


LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int i = 0;
	static PAINTSTRUCT ps = {NULL};
	static HDC hDC = NULL;
	static HBRUSH hBrush = NULL;
	static UINT uTimer = 0;
	static int xpos, ypos;
	static RECT rc;

	switch(message)
	{
	case WM_CREATE:
		for(i = 0; i < MAX_BALLS; i++)
			gBalls[i].hIcon = (HICON)LoadImage(hMainInstance, MAKEINTRESOURCE(gBalls[i].uBallID), IMAGE_ICON, 48, 48, LR_DEFAULTSIZE);

		xpos = GetSystemMetrics(SM_CXSCREEN) / 2;
		ypos = GetSystemMetrics(SM_CYSCREEN) / 2;

		for(i = 0; i < MAX_BALLS; i++)
		{
			double alpha = gBalls[i].angle * pi / 180;
			gBalls[i].x = xpos + int((xpos - 30) * sin(alpha) * cos(alpha) * cos(2 * alpha));
			gBalls[i].y = ypos - 30 + int(265 * cos(alpha));
		}

		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		uTimer = SetTimer(hWnd, 1, 1, NULL);
		break;

    case WM_DESTROY:
		if(uTimer)
			KillTimer(hWnd, uTimer);

		if(hBrush)
			DeleteObject(hBrush);

		for(i = 0; i < MAX_BALLS; i++)
			if(gBalls[i].hIcon)
				DestroyIcon(gBalls[i].hIcon);

		PostQuitMessage(0);
		break;

	case WM_TIMER:
		xpos = GetSystemMetrics(SM_CXSCREEN) / 2;
		ypos = GetSystemMetrics(SM_CYSCREEN) / 2;

		for(i = 0; i < MAX_BALLS; i++)
		{
			double alpha = gBalls[i].angle * pi / 180;
			gBalls[i].x = xpos + int((xpos - 30) * sin(alpha) * cos(alpha) * cos(2 * alpha));
			gBalls[i].y = ypos - 30 + int(265 * cos(alpha));
			gBalls[i].angle = (gBalls[i].angle >= 360) ? 0 : gBalls[i].angle + 1;

			rc.left = gBalls[i].x;
			rc.right = gBalls[i].x + 48;
			rc.top = gBalls[i].y;
			rc.bottom = gBalls[i].y + 48;

			InvalidateRect(hWnd, &rc, FALSE);
		}
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		if(fChildPreview)
		{
			SetBkColor(hDC, RGB(0, 0, 0));
			SetTextColor(hDC, RGB(255, 255, 0));
			TextOut(hDC, 25, 45, szPreview, strlen(szPreview));
		}
		else
		{
			SetBkColor(hDC, RGB(0, 0, 0));
			SetTextColor(hDC, RGB(120, 120, 120));
			TextOut(hDC, 0, ypos * 2 - 40, szAppName, strlen(szAppName));
			TextOut(hDC, 0, ypos * 2 - 25, szAuthor, strlen(szAuthor));

			for(i = 0; i < MAX_BALLS; i++)
				DrawIconEx(hDC, gBalls[i].x, gBalls[i].y, gBalls[i].hIcon, 48, 48, 0, (HBRUSH)hBrush, DI_IMAGE);
		}
		
		EndPaint(hWnd, &ps);
		break;

    default: 
		return DefScreenSaverProc(hWnd, message, wParam, lParam);
	}
	
	return 0;
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
	return TRUE;
}