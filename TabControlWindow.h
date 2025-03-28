// TabControlWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define TAB_CONTROL_WINDOW_CLASS_NAME											WC_TABCONTROL
#define TAB_CONTROL_WINDOW_EXTENDED_STYLE										0
#define TAB_CONTROL_WINDOW_TITLE												NULL
#define TAB_CONTROL_WINDOW_STYLE												( WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE )

#define TAB_CONTROL_WINDOW_TITLES												{ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" }

typedef enum
{
	TAB_CONTROL_WINDOW_IDS_SUNDAY = 0,
	TAB_CONTROL_WINDOW_IDS_MONDAY,
	TAB_CONTROL_WINDOW_IDS_TUESDAY,
	TAB_CONTROL_WINDOW_IDS_WEDNESDAY,
	TAB_CONTROL_WINDOW_IDS_THURSDAY,
	TAB_CONTROL_WINDOW_IDS_FRIDAY,
	TAB_CONTROL_WINDOW_IDS_SATURDAY,

	TAB_CONTROL_WINDOW_NUMBER_OF_DAYS

} DAYS;

BOOL IsTabControlWindow( HWND hWndQuery );

BOOL TabControlWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL TabControlWindowHandleNotifyMessage( WPARAM, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszStatusMessage ) );

BOOL TabControlWindowMove( int nLeft, int nTop, int nWidth, int nHeight, BOOL bRepaint = TRUE );

void TabControlWindowSetFont( HFONT hFont, BOOL bRedraw = TRUE );
