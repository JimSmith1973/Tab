// TabControlWindow.cpp

#include "TabControlWindow.h"

// Global variables
static HWND g_hWndTabControl;

BOOL IsTabControlWindow( HWND hWndQuery )
{
	return( hWndQuery == g_hWndTabControl );

} // End of function IsTabControlWindow

BOOL TabControlWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	INITCOMMONCONTROLSEX iccex;

	// Initialise common controls structure
	iccex.dwSize	= sizeof( INITCOMMONCONTROLSEX );
	iccex.dwICC		= ICC_TAB_CLASSES;

	// Initialise common controls
	InitCommonControlsEx( &iccex );

	// Create tab control window
	g_hWndTabControl = CreateWindowEx( TAB_CONTROL_WINDOW_EXTENDED_STYLE, TAB_CONTROL_WINDOW_CLASS_NAME, TAB_CONTROL_WINDOW_TITLE, TAB_CONTROL_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWndParent, NULL, hInstance, NULL );

	// Ensure that tab control window was created
	if( g_hWndTabControl )
	{
		// Successfully created tab control window
		TCITEM tcItem;
		int nWhichDay;
		LPCTSTR lpszTabControlTitles [] = TAB_CONTROL_WINDOW_TITLES;

		// Clear tab control item structure
		ZeroMemory( &tcItem, sizeof( tcItem ) );

		// Initialise tab control item structure
		tcItem.mask = TCIF_TEXT;

		// Loop through daya
		for( nWhichDay = 0; nWhichDay < TAB_CONTROL_WINDOW_NUMBER_OF_DAYS; nWhichDay ++ )
		{
			// Update tab control item structure for current day
			tcItem.pszText = ( LPTSTR )lpszTabControlTitles[ nWhichDay ];

			// Add item to tab control window
			if( SendMessage( g_hWndTabControl, TCM_INSERTITEM, ( WPARAM )nWhichDay, ( LPARAM )&tcItem ) >= 0 )
			{
				// Successfully added item to tab control window
			} // End of successfully added item to tab control window

		}; // End of loop through days

		// Update return value
		bResult = TRUE;

	} // End of successfully created tab control window

	return bResult;

} // End of function TabControlWindowCreate

BOOL TabControlWindowHandleNotifyMessage( WPARAM, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszStatusMessage ))
{
	BOOL bResult = FALSE;

	LPNMHDR lpNmHdr;

	// Get notify message handler
	lpNmHdr = ( LPNMHDR )lParam;

	// Select notify message
	switch( lpNmHdr->code )
	{
		case TCN_SELCHANGE:
		{
			// A selection change message
			int nSelectedTab;
			LPCTSTR lpszTabControlTitles [] = TAB_CONTROL_WINDOW_TITLES;

			// Get selected tab
			nSelectedTab = SendMessage( g_hWndTabControl, TCM_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

			// Show selected tab
			( *lpStatusFunction )( lpszTabControlTitles[ nSelectedTab ] );

			// Update return value
			bResult = TRUE;

			// Break out of switch
			break;

		} // End of a selection change message

	}; // End of selection for notify message

	return bResult;

} // End of function TabControlWindowHandleNotifyMessage

BOOL TabControlWindowMove( int nLeft, int nTop, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move tab control window
	return MoveWindow( g_hWndTabControl, nLeft, nTop, nWidth, nHeight, bRepaint );

} // End of function TabControlWindowMove

void TabControlWindowSetFont( HFONT hFont, BOOL bRedraw )
{
	// Set tab control window font
	SendMessage( g_hWndTabControl, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )bRedraw );

} // End of function TabControlWindowSetFont
