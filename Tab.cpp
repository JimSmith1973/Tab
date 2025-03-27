// Tab.cpp

#include "Tab.h"

// Global variables
HINSTANCE g_hInstance;
HWND g_hWndTabControl;


typedef enum
{
	TAB_CONTROL_IDS_SUNDAY = 0,
	TAB_CONTROL_IDS_MONDAY,
	TAB_CONTROL_IDS_TUESDAY,
	TAB_CONTROL_IDS_WEDNESDAY,
	TAB_CONTROL_IDS_THURSDAY,
	TAB_CONTROL_IDS_FRIDAY,
	TAB_CONTROL_IDS_SATURDAY,

	TAB_CONTROL_NUMBER_OF_DAYS

} DAYS;

BOOL TabControlWindowCreate(HWND hWndParent )
{
	BOOL bResult = FALSE;

	INITCOMMONCONTROLSEX iccex;

	// Initialise common controls structure
	iccex.dwSize	= sizeof( INITCOMMONCONTROLSEX );
	iccex.dwICC		= ICC_TAB_CLASSES;

	// Initialise common controls
	InitCommonControlsEx( &iccex );

	// Create tab control window
	g_hWndTabControl = CreateWindowEx( TAB_CONTROL_WINDOW_EXTENDED_STYLE, TAB_CONTROL_WINDOW_CLASS_NAME, TAB_CONTROL_WINDOW_TITLE, TAB_CONTROL_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWndParent, NULL, g_hInstance, NULL );

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
		for( nWhichDay = 0; nWhichDay < TAB_CONTROL_NUMBER_OF_DAYS; nWhichDay ++ )
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

BOOL TabControlWindowMove( int nLeft, int nTop, int nWidth, int nHeight, BOOL bRepaint = TRUE )
{
	// Move tab control window
	return MoveWindow( g_hWndTabControl, nLeft, nTop, nWidth, nHeight, bRepaint );

} // End of function TabControlWindowMove

void TabControlWindowSetFont( HFONT hFont, BOOL bRedraw = TRUE )
{
	// Set tab control window font
	SendMessage( g_hWndTabControl, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )bRedraw );

} // End of function TabControlWindowSetFont

int ShowAboutMessage( HWND hWndParent )
{
	int nResult = 0;

	MSGBOXPARAMS mbp;

	// Clear message box parameter structure
	ZeroMemory( &mbp, sizeof( mbp ) );

	// Initialise message box parameter structure
	mbp.cbSize		= sizeof( MSGBOXPARAMS );
	mbp.hwndOwner	= hWndParent;
	mbp.hInstance	= NULL;
	mbp.lpszText	= ABOUT_MESSAGE_TEXT;
	mbp.lpszCaption	= ABOUT_MESSAGE_CAPTION;
	mbp.dwStyle		= ( MB_OK | MB_USERICON );
	mbp.lpszIcon	= MAIN_WINDOW_CLASS_ICON_NAME;

	// Show message box
	nResult = MessageBoxIndirect( &mbp );

	return nResult;

} // End of function ShowAboutMessage

LRESULT CALLBACK MainWindowProcedure( HWND hWndMain, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr = 0;

	// Select message
	switch( uMessage )
	{
		case WM_CREATE:
		{
			// A create message
			HINSTANCE hInstance;

			// Get instance
			hInstance = ( ( LPCREATESTRUCT )lParam )->hInstance;

			// Update global instance handle
			g_hInstance = hInstance;

			// Create
			if( TabControlWindowCreate( hWndMain ) )
			{
				// Successfully created tab control window
				HFONT hFont;

				// Get font
				hFont = ( HFONT )GetStockObject( DEFAULT_GUI_FONT );

				// Set tab control window font
				TabControlWindowSetFont( hFont );

				// Create status bar window
				if( StatusBarWindowCreate( hWndMain, hInstance ) )
				{
					// Successfully created status bar window

					// Set status bar window font
					StatusBarWindowSetFont( hFont );

				} // End of successfully created status bar window

			} // End of successfully created tab control window

			// Break out of switch
			break;

		} // End of a create message
		case WM_SIZE:
		{
			// A size message
			int nClientWidth;
			int nClientHeight;
			RECT rcStatusBar;
			int nStatusBarWindowHeight;
			int nTabControlWindowHeight;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			StatusBarWindowSize();

			// Get status bar window size
			StatusBarWindowGetRect( &rcStatusBar );

			// Calculate window sizes
			nStatusBarWindowHeight	= ( rcStatusBar.bottom - rcStatusBar.top );
			nTabControlWindowHeight	= ( nClientHeight - nStatusBarWindowHeight );

			// Move tab control window
			TabControlWindowMove( 0, 0, nClientWidth, nTabControlWindowHeight );

			// Break out of switch
			break;

		} // End of a size message
		case WM_ACTIVATE:
		{
			// An activate message

			// Break out of switch
			break;

		} // End of an activate message
		case WM_GETMINMAXINFO:
		{
			// A get min max info message
			MINMAXINFO FAR *lpMinMaxInfo;

			// Get min max info structure
			lpMinMaxInfo = ( MINMAXINFO FAR * )lParam;

			// Update min max info structure
			lpMinMaxInfo->ptMinTrackSize.x = MAIN_WINDOW_MINIMUM_WIDTH;
			lpMinMaxInfo->ptMinTrackSize.y = MAIN_WINDOW_MINIMUM_HEIGHT;

			// Break out of switch
			break;

		} // End of a get min max info message
		case WM_COMMAND:
		{
			// A command message

			// Select command
			switch( LOWORD( wParam ) )
			{
				case IDM_FILE_EXIT:
				{
					// A file exit command

					// Destroy window
					DestroyWindow( hWndMain );

					// Break out of switch
					break;

				} // End of a file exit command
				case IDM_HELP_ABOUT:
				{
					// A help about command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about command
				default:
				{
					// Default command

					// Source window is ( HWND )lParam

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					// Break out of switch
					break;

				} // End of default command

			}; // End of selection for command

			// Break out of switch
			break;

		} // End of a command message
		case WM_SYSCOMMAND:
		{
			// A system command message

			// Select system command
			switch( LOWORD( wParam ) )
			{
				case IDM_HELP_ABOUT:
				{
					// A help about command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about command
				default:
				{
					// Default system command

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					// Break out of switch
					break;

				} // End of default system command

			}; // End of selection for system command

			// Break out of switch
			break;

		} // End of a system command message
		case WM_NOTIFY:
		{
			// A notify message
			LPNMHDR lpNmHdr;

			// Get notify message handler
			lpNmHdr = ( LPNMHDR )lParam;

			// Source window is lpNmHdr->hwndFrom
			// See if notify message is from tab control window
			if( lpNmHdr->hwndFrom == g_hWndTabControl )
			{
				// Notify message is from tab control window

				// Handle notify message from tab control window
				if( !( TabControlWindowHandleNotifyMessage( wParam, lParam, &StatusBarWindowSetText ) ) )
				{
					// Notify message was not handled from tab control window

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

				} // End of notify message was not handled from tab control window

			} // End of notify message is from tab control window

			// Break out of switch
			break;

		} // End of a notify message
		case WM_CONTEXTMENU:
		{
			// A context menu message
			HMENU hMenuContext;

			// Load context menu
			hMenuContext = LoadMenu( NULL, MAKEINTRESOURCE( IDR_CONTEXT_MENU ) );

			// Show context menu
			TrackPopupMenu( GetSubMenu( hMenuContext, 0 ), ( TPM_LEFTALIGN | TPM_LEFTBUTTON ), LOWORD( lParam ), HIWORD( lParam ), 0, hWndMain, NULL );

			// Break out of switch
			break;

		} // End of a context menu message
		case WM_CLOSE:
		{
			// A close message

			// Destroy main window
			DestroyWindow( hWndMain );

			// Break out of switch
			break;

		} // End of a close message
		case WM_DESTROY:
		{
			// A destroy message

			// Terminate thread
			PostQuitMessage( 0 );

			// Break out of switch
			break;

		} // End of a destroy message
		default:
		{
			// Default message

			// Call default window procedure
			lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function MainWindowProcedure

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow )
{
	MSG msg;

	WNDCLASSEX wcMain;

	// Clear message structure
	ZeroMemory( &msg, sizeof( msg ) );

	// Clear main window class structure
	ZeroMemory( &wcMain, sizeof( wcMain ) );

	// Initialise main window class structure
	wcMain.cbSize			= sizeof( WNDCLASSEX );
	wcMain.lpszClassName	= MAIN_WINDOW_CLASS_NAME;
	wcMain.lpfnWndProc		= MainWindowProcedure;
	wcMain.hInstance		= hInstance;
	wcMain.style			= MAIN_WINDOW_CLASS_STYLE;
	wcMain.hIcon			= MAIN_WINDOW_CLASS_ICON;
	wcMain.hCursor			= MAIN_WINDOW_CLASS_CURSOR;
	wcMain.hbrBackground	= MAIN_WINDOW_CLASS_BACKGROUND;
	wcMain.lpszMenuName		= MAIN_WINDOW_CLASS_MENU_MAME;
	wcMain.hIconSm			= MAIN_WINDOW_CLASS_SMALL_ICON;

	// Register main window class
	if( RegisterClassEx( &wcMain ) )
	{
		// Successfully registered main window class
		HWND hWndMain;

		// Create main window
		hWndMain = CreateWindowEx( MAIN_WINDOW_EXTENDED_STYLE, MAIN_WINDOW_CLASS_NAME, MAIN_WINDOW_TITLE, MAIN_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL );

		// Ensure that main window was created
		if( hWndMain )
		{
			// Successfully created main window
			HMENU hMenuSystem;

			// Get system menu
			hMenuSystem = GetSystemMenu( hWndMain, FALSE );

			// Add separator to system menu
			InsertMenu( hMenuSystem, SYSTEM_MENU_SEPARATOR_ITEM_POSITION, ( MF_BYPOSITION | MF_SEPARATOR ), 0, NULL );

			// Add about item to system menu
			InsertMenu( hMenuSystem, SYSTEM_MENU_ABOUT_ITEM_POSITION, MF_BYPOSITION, IDM_HELP_ABOUT, SYSTEM_MENU_ABOUT_ITEM_TEXT );

			// Show main window
			ShowWindow( hWndMain, nCmdShow );

			// Update main window
			UpdateWindow( hWndMain );

			// Main message loop
			while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
			{
				// Translate message
				TranslateMessage( &msg );

				// Dispatch message
				DispatchMessage( &msg);

			}; // End of main message loop

		} // End of successfully created main window
		else
		{
			// Unable to create main window

			// Display error message
			MessageBox( NULL, UNABLE_TO_CREATE_MAIN_WINDOW_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

		} // End of unable to create main window

	} // End of successfully registered main window class
	else
	{
		// Unable to register main window class

		// Display error message
		MessageBox( NULL, UNABLE_TO_REGISTER_MAIN_WINDOW_CLASS_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

	} // End of unable to register main window class

	return msg.wParam;

} // End of function WinMain
