// ListViewWindow.cpp

#include "ListViewWindow.h"

// Global variables
HWND g_hWndListView;
int g_nColumnCount;

BOOL IsListViewWindow( HWND hWnd )
{
	// See if supplied window is list view window
	return( hWnd == g_hWndListView );

} // End of function IsListViewWindow

int ListViewWindowAddColumn( LPCTSTR lpszTitle, int nWidth )
{
	int nResult = 0;

	LVCOLUMN lvColumn;

	// Clear list view column structure
	ZeroMemory( &lvColumn, sizeof( lvColumn ) );

	// Initialise list view column structure
	lvColumn.mask		= ( LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM );
	lvColumn.pszText	= ( LPTSTR )lpszTitle;
	lvColumn.cx			= nWidth;

	// Insert column
	nResult = SendMessage( g_hWndListView, LVM_INSERTCOLUMN, ( WPARAM )g_nColumnCount , ( LPARAM )&lvColumn );

	// Ensure that column was inserted
	if( nResult >= 0 )
	{
		// Successfully inserted column

		// Update column count
		g_nColumnCount ++;

	} // End of successfully inserted column

	return nResult;

} // End of function ListViewWindowAddColumn

int ListViewWindowAddItem( LPCTSTR lpszItemText )
{
	int nResult;

	LVITEM lvItem;
	int nItemCount;

	// Count items on list view window
	nItemCount = SendMessage( g_hWndListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= nItemCount;
	lvItem.iSubItem		= 0;
	lvItem.pszText		= ( LPTSTR )lpszItemText;

	// Add item to list view window
	nResult = SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )nItemCount, ( LPARAM )&lvItem );

	return nResult;

} // End of function ListViewWindowAddItem

int ListViewWindowAutoSizeAllColumns()
{
	int nResult = 0;

	int nWhichColumn;

	// Loop through all columns
	for( nWhichColumn = 0; nWhichColumn < g_nColumnCount; nWhichColumn ++ )
	{
		// Auto-size current column
		if( SendMessage( g_hWndListView, LVM_SETCOLUMNWIDTH, ( WPARAM )nWhichColumn, ( LPARAM )LVSCW_AUTOSIZE_USEHEADER ) )
		{
			// Successfully auto-sized current column

			// Update return value
			nResult ++;

		} // End of successfully auto-sized current column
		else
		{
			// Unable to auto-size current column

			// Force exit from loop
			nWhichColumn = g_nColumnCount;

		} // End of unable to auto-size current column

	}; // End of loop through all columns

	return nResult;

} // End of function ListViewWindowAutoSizeAllColumns

BOOL ListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create list view window
	g_hWndListView = ::CreateWindowEx( LIST_VIEW_WINDOW_EXTENDED_STYLE, LIST_VIEW_WINDOW_CLASS_NAME, LIST_VIEW_WINDOW_TEXT, LIST_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list view window was created
	if( g_hWndListView )
	{
		// Successfully created list view window

		// Set extended list view window style
		SendMessage( g_hWndListView, LVM_SETEXTENDEDLISTVIEWSTYLE, ( WPARAM )0, ( LPARAM )LIST_VIEW_WINDOW_EXTENDED_STYLE );

		// Initialise global variables
		g_nColumnCount = 0;

		// Update return value
		bResult = TRUE;

	} // End of successfully created list view window
	return bResult;

} // End of function ListViewWindowCreate

int ListViewWindowGetCurrentSelection()
{
	// Get current selection item
	return ::SendMessage( g_hWndListView, LVM_GETNEXTITEM, ( WPARAM )-1, ( LPARAM )LVNI_SELECTED );

} // End of function ListViewWindowGetCurrentSelection

int ListViewWindowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText )
{
	int nResult;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= nWhichSubItem;
	lvItem.pszText		= lpszItemText;

	// Get list view window item text
	nResult = SendMessage( g_hWndListView, LVM_GETITEMTEXT, ( WPARAM )nWhichItem, ( LPARAM )&lvItem );

	return nResult;

} // End of function ListViewWindowGetItemText

BOOL ListViewWindowGetRect( LPRECT lpRect )
{
	// Get list view window rect
	return ::GetWindowRect( g_hWndListView, lpRect );

} // End of function ListViewWindowGetRect

BOOL ListViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText, int nWhichItem ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText, int nWhichItem ) )
{
	BOOL bResult = FALSE;

	LPNMHDR lpNmHdr;

	// Get notify message information structure
	lpNmHdr = ( LPNMHDR )lParam;

	// Select notify message
	switch( lpNmHdr->code )
	{
		case LVN_ITEMCHANGED:
		{
			// A list view window item changed notification
			LPNMLISTVIEW lpNmListView;

			// Get list view notify message information structure
			lpNmListView = ( LPNMLISTVIEW )lParam;

			// See if item state has changed
			if( lpNmListView->uChanged & LVIF_STATE )
			{
				// Item state has changed

				// See if item has changed to selected
				if( ( lpNmListView->uNewState ^ lpNmListView->uOldState ) & LVIS_SELECTED )
				{
					// Item has changed to selected

					// Allocate string memory
					LPTSTR lpszItemText = new char[ STRING_LENGTH ];

					// Get item text
					if( ListViewWindowGetItemText( lpNmListView->iItem, lpNmListView->iSubItem, lpszItemText ) )
					{
						// Successfully got item text

						// Call selection changed function
						( *lpSelectionChangedFunction )( lpszItemText, lpNmListView->iItem );

						// Update return value
						bResult = TRUE;

					} // End of successfully got item text

					// Free string memory
					delete [] lpszItemText;

				} // End of item has changed to selected

			} // End of item state has changed

			// Break out of switch
			break;

		} // End of a list view window item changed notification
		case NM_DBLCLK:
		{
			// A double click notification
			LPNMLISTVIEW lpNmListView;

			// Get list view notify message information structure
			lpNmListView = ( LPNMLISTVIEW )lParam;

			// Allocate string memory
			LPTSTR lpszItemText = new char[ STRING_LENGTH ];

			// Get item text
			if( ListViewWindowGetItemText( lpNmListView->iItem, lpNmListView->iSubItem, lpszItemText ) )
			{
				// Successfully got item text

				// Call double click function
				( *lpDoubleClickFunction )( lpszItemText, lpNmListView->iItem );

				// Update return value
				bResult = TRUE;

			} // End of successfully got item text

			// Free string memory
			delete [] lpszItemText;

			// Break out of switch
			break;

		} // End of a double click notification

	}; // End of selection for notify message

	return bResult;

} // End of function ListViewWindowHandleNotifyMessage

BOOL ListViewWindowLoad( LPCTSTR lpszFileName )
{
	BOOL bResult = FALSE;

	HANDLE hFile;

	// Open file
	hFile = CreateFile( lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );

	// Ensure that file was opened
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully opened file
		DWORD dwFileSize;

		// Get file size
		dwFileSize = GetFileSize( hFile, NULL );

		// Ensure that file size was got
		if( dwFileSize != INVALID_FILE_SIZE )
		{
			// Successfully got file size

			// Allocate string memory
			LPTSTR lpszFileText = new char[ STRING_LENGTH + sizeof( char ) ];

			// Read file text
			if( ReadFile (hFile, lpszFileText, dwFileSize, NULL, NULL ) )
			{
				// Successfully read file text
				LPTSTR lpszTable;

				// Terminate file text
				lpszFileText[ dwFileSize ] = ( char )NULL;

				// Find start of table
				lpszTable = strstr( lpszFileText, HTML_FILE_TABLE_PREFIX );

				// Ensure that start of table was found
				if( lpszTable )
				{
					// Successfully found start of table
					LPTSTR lpszTableSuffix;

					// Find table suffix
					lpszTableSuffix = strstr( lpszTable, HTML_FILE_TABLE_SUFFIX );

					// Ensure that table suffix was found
					if( lpszTableSuffix )
					{
						// Successfully found table suffix
						LPTSTR lpszRowPrefix;
						LPTSTR lpszRowSuffix;
						DWORD dwRowLength;
						LPTSTR lpszItemPrefix;
						LPTSTR lpszItemSuffix;
						LPTSTR lpszItemStart;
						DWORD dwItemLength;

						// Allocate string memory
						LPTSTR lpszRow	= new char[ STRING_LENGTH ];
						LPTSTR lpszItem	= new char[ STRING_LENGTH ];

						// Terminate table after suffix
						lpszTableSuffix[ lstrlen( HTML_FILE_TABLE_SUFFIX ) ] = ( char )NULL;

						// Find first row prefix
						lpszRowPrefix = strstr( lpszTable, HTML_FILE_TABLE_ROW_PREFIX );

						// Loop through all table rows
						while( lpszRowPrefix )
						{
							// Find row suffix
							lpszRowSuffix = strstr( lpszRowPrefix, HTML_FILE_TABLE_ROW_SUFFIX );

							// Ensure that row suffix was found
							if( lpszRowSuffix )
							{
								// Successfully found row suffix

								// Calculate row length
								dwRowLength = ( ( lpszRowSuffix + lstrlen( HTML_FILE_TABLE_ROW_SUFFIX )) - lpszRowPrefix );

								// Store row
								lstrcpyn( lpszRow, lpszRowPrefix, dwRowLength );

								// Find first item prefix in row
								lpszItemPrefix = strstr( lpszRow, HTML_FILE_TABLE_ITEM_PREFIX );

								// Loop through all items in row
								while( lpszItemPrefix )
								{
									// Find item suffix
									lpszItemSuffix = strstr( lpszItemPrefix, HTML_FILE_TABLE_ITEM_SUFFIX );

									// Ensure that item suffix was found
									if( lpszItemSuffix )
									{
										// Successfully found item suffix

										// Get start of item
										lpszItemStart = ( lpszItemPrefix + lstrlen( HTML_FILE_TABLE_ITEM_PREFIX ) );

										// Calculate item length
										dwItemLength = ( lpszItemSuffix - lpszItemStart );

										// Store item
										lstrcpyn( lpszItem, lpszItemStart, ( dwItemLength + sizeof( char ) ) );

										// Display file text
										MessageBox( NULL, lpszItem, "Item", ( MB_OK | MB_ICONINFORMATION ) );

										// Find next item prefix
										lpszItemPrefix = strstr( lpszItemSuffix, HTML_FILE_TABLE_ITEM_PREFIX );

									} // End of successfully found item suffix
									else
									{
										// Unable to find item suffix

										// Force exit from loops
										lpszRowPrefix	= NULL;
										lpszItemPrefix	= NULL;

									} // End of unable to find item suffix

								}; // End of loop through all items in row

								// Find next row prefix
								lpszRowPrefix = strstr( lpszRowSuffix, HTML_FILE_TABLE_ROW_PREFIX );

							} // End of successfully found row suffix
							else
							{
								// Unable to find row suffix

								// Force exit from loop
								lpszRowPrefix = NULL;

							} // End of unable to find row suffix

						}; // End of loop through all table rows

						// Free string memory
						delete [] lpszRow;
						delete [] lpszItem;

					} // End of successfully found table suffix

				} // End of successfully found start of table

				// Update return value
				bResult = TRUE;

			} // End of successfully read file text

			// Free string memory
			delete [] lpszFileText;

		} // End of successfully got file size

		// Close file
		CloseHandle( hFile );

	} // End of successfully opened file

	return bResult;

} // End of function ListViewWindowLoad

BOOL ListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move list view window
	return ::MoveWindow( g_hWndListView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function ListViewWindowMove

BOOL ListViewWindowSave( LPCTSTR lpszFileName )
{
	BOOL bResult = FALSE;

	HANDLE hFile;

	// Open file
	hFile = CreateFile( lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	// Ensure that file was opened
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully opened file

		// Allocate string memory
		LPTSTR lpszHtmlFilePrefix = new char[ STRING_LENGTH ];

		// Format html file prefix
		wsprintf( lpszHtmlFilePrefix, HTML_FILE_PREFIX_FORMAT_STRING, lpszFileName, lpszFileName );

		// Write html file prefix to file
		if( WriteFile( hFile, lpszHtmlFilePrefix, lstrlen( lpszHtmlFilePrefix ), NULL, NULL ) )
		{
			// Successfully wrote html file prefix to file
			LVITEM lvItem;
			int nItemCount;

			// Allocate string memory
			LPTSTR lpszItemText = new char[ STRING_LENGTH ];

			// Clear list view item structure
			ZeroMemory( &lvItem, sizeof( lvItem ) );

			// Initialise list view item structure
			lvItem.mask			= LVIF_TEXT;
			lvItem.cchTextMax	= STRING_LENGTH;
			lvItem.pszText		= lpszItemText;

			// Count items on list view window
			nItemCount = SendMessage( g_hWndListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

			// Write table prefix to file
			WriteFile( hFile, HTML_FILE_TABLE_PREFIX, lstrlen( HTML_FILE_TABLE_PREFIX ), NULL, NULL );

			// Loop through items on list view window
			for( lvItem.iItem = 0; lvItem.iItem < nItemCount; lvItem.iItem ++ )
			{
				// Write table row prefix to file
				WriteFile( hFile, HTML_FILE_TABLE_ROW_PREFIX, lstrlen( HTML_FILE_TABLE_ROW_PREFIX ), NULL, NULL );

				// Loop through sub-items on list view window
				for( lvItem.iSubItem = 0; lvItem.iSubItem < g_nColumnCount; lvItem.iSubItem ++ )
				{
					// Write table item prefix to file
					WriteFile( hFile, HTML_FILE_TABLE_ITEM_PREFIX, lstrlen( HTML_FILE_TABLE_ITEM_PREFIX ), NULL, NULL );

					// Get list view window item text
					if( SendMessage( g_hWndListView, LVM_GETITEMTEXT, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem ) )
					{
						// Successfully got list view window item text

						// Write table item text to file
						WriteFile( hFile, lpszItemText, lstrlen( lpszItemText ), NULL, NULL );

					} // End of successfully got list view window item text

					// Write table item suffix to file
					WriteFile( hFile, HTML_FILE_TABLE_ITEM_SUFFIX, lstrlen( HTML_FILE_TABLE_ITEM_SUFFIX ), NULL, NULL );

				}; // End of loop through sub-items on list view window

				// Write table row suffix to file
				WriteFile( hFile, HTML_FILE_TABLE_ROW_SUFFIX, lstrlen( HTML_FILE_TABLE_ROW_SUFFIX ), NULL, NULL );

			}; // End of loop through items on list view window

			// Write table suffix to file
			WriteFile( hFile, HTML_FILE_TABLE_SUFFIX, lstrlen( HTML_FILE_TABLE_SUFFIX ), NULL, NULL );

			// Write html file suffix to file
			WriteFile( hFile, HTML_FILE_SUFFIX, lstrlen( HTML_FILE_SUFFIX ), NULL, NULL );

			// Free string memory
			delete [] lpszItemText;

		} // End of successfully wrote html file prefix to file

		// Close file
		CloseHandle( hFile );

	} // End of successfully opened file

	return bResult;

} // End of function ListViewWindowSave

HWND ListViewWindowSetFocus()
{
	// Focus on list view window
	return ::SetFocus( g_hWndListView );

} // End of function ListViewWindowSetFocus

void ListViewWindowSetFont( HFONT hFont )
{
	// Set list view window font
	::SendMessage( g_hWndListView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ListViewWindowSetFont

int ListViewWindowSetItemText( int nWhichItem, int nWhichSubItem, LPCTSTR lpszItemText )
{
	int nResult;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= nWhichSubItem;
	lvItem.pszText		= ( LPTSTR )lpszItemText;

	// Add item to list view window
	nResult = SendMessage( g_hWndListView, LVM_SETITEMTEXT, ( WPARAM )nWhichItem, ( LPARAM )&lvItem );

	return nResult;

} // End of function ListViewWindowSetItemText
