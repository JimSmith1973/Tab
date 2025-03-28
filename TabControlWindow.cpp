// TabControlWindow.cpp

#include "TabControlWindow.h"

// Global variables
static HWND g_hWndTabControl;

BOOL IsTabControlWindow( HWND hWndQuery )
{
	return( hWndQuery == g_hWndTabControl );

} // End of function IsTabControlWindow

int TabControlWindowAddTab( LPCTSTR lpszTab )
{
	int nResult = -1;

	TCITEM tcItem;

	int nTabCount;

	// Clear tab control item structure
	ZeroMemory( &tcItem, sizeof( tcItem ) );

	// Initialise tab control item structure
	tcItem.mask			= TCIF_TEXT;
	tcItem.pszText		= ( LPTSTR )lpszTab;
	tcItem.cchTextMax	= STRING_LENGTH;

	// Count tabs
	nTabCount = SendMessage( g_hWndTabControl, TCM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

	// Add item to tab control window
	nResult = SendMessage( g_hWndTabControl, TCM_INSERTITEM, ( WPARAM )nTabCount, ( LPARAM )&tcItem );

	return nResult;

} // End of function TabControlWindowAddTab

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
		int nWhichDay;
		LPCTSTR lpszTabControlTitles [] = TAB_CONTROL_WINDOW_TITLES;

		// Assume success
		bResult = TRUE;

		// Loop through daya
		for( nWhichDay = 0; nWhichDay < TAB_CONTROL_WINDOW_NUMBER_OF_DAYS; nWhichDay ++ )
		{
			// Add item to tab control window
			if( TabControlWindowAddTab( lpszTabControlTitles[ nWhichDay ] ) < 0 )
			{
				// Unable to add item to tab control window

				// Update return value
				bResult = TRUE;

				// Force exit from loop
				nWhichDay = TAB_CONTROL_WINDOW_NUMBER_OF_DAYS;

			} // End of unable to add item to tab control window

		}; // End of loop through days

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

			// Call selection change function
			bResult = TabControlWindowSelectionChange( lpStatusFunction );

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

BOOL TabControlWindowSelectionChange( BOOL( *lpStatusFunction )( LPCTSTR lpszStatusMessage ) )
{
	BOOL bResult = FALSE;

	int nSelectedTab;

	// Get selected tab
	nSelectedTab = SendMessage( g_hWndTabControl, TCM_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

	// Ensure that selected tab was got
	if( nSelectedTab >= 0 )
	{
		// Successfully got selected tab

		// Call selection change function
		bResult = TabControlWindowSelectionChange( nSelectedTab, lpStatusFunction );

		// Update return value
		bResult = TRUE;

	} // End of successfully got selected tab

	return bResult;

} // End of function TabControlWindowSelectionChange

BOOL TabControlWindowSelectionChange( int nSelectedTab, BOOL( *lpStatusFunction )( LPCTSTR lpszStatusMessage ) )
{
	BOOL bResult = FALSE;

	TCITEM tcItem;

	// Allocate string memory
	LPTSTR lpszItemText = new char[ STRING_LENGTH + sizeof( char ) ];

	// Clear tab control item structure
	ZeroMemory( &tcItem, sizeof( tcItem ) );

	// Initialise tab control item structure
	tcItem.mask			= TCIF_TEXT;
	tcItem.pszText		= lpszItemText;
	tcItem.cchTextMax	= STRING_LENGTH;

	// Get item text
	if( SendMessage( g_hWndTabControl, TCM_GETITEM, ( WPARAM )nSelectedTab, ( LPARAM )&tcItem ) )
	{
		// Successfully got item text

		// Show item text
		( *lpStatusFunction )( lpszItemText );

	} // End of successfully got item text

	// Free string memory
	delete [] lpszItemText;

	return bResult;

} // End of function TabControlWindowSelectionChange

void TabControlWindowSetFont( HFONT hFont, BOOL bRedraw )
{
	// Set tab control window font
	SendMessage( g_hWndTabControl, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )bRedraw );

} // End of function TabControlWindowSetFont
