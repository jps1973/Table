// ListViewWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#include "HtmlFile.h"

#define LIST_VIEW_WINDOW_CLASS_NAME												WC_LISTVIEW

#define LIST_VIEW_WINDOW_EXTENDED_STYLE											LVS_EX_FULLROWSELECT
#define LIST_VIEW_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE| WS_BORDER | LVS_REPORT )
#define LIST_VIEW_WINDOW_TEXT													NULL

#define LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH									100
#define LIST_VIEW_WINDOW_COLUMN_TITLE_FORMAT_STRING								"Column %d"
#define LIST_VIEW_WINDOW_FIRST_COLUMN_NUMBER									1

BOOL IsListViewWindow( HWND hWnd );

int ListViewWindowAddColumn( LPCTSTR lpszTitle, int nWidth );

int ListViewWindowAddItem( LPCTSTR lpszItemText );

int ListViewWindowAutoSizeAllColumns();

BOOL ListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance );

int ListViewWindowGetCurrentSelection();

int ListViewWindowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText );

BOOL ListViewWindowGetRect( LPRECT lpRect );

BOOL ListViewWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText, int nWhichItem ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText, int nWhichItem ) );

BOOL ListViewWindowLoad( LPCTSTR lpszFileName );

BOOL ListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

BOOL ListViewWindowSave( LPCTSTR lpszFileName );

HWND ListViewWindowSetFocus();

void ListViewWindowSetFont( HFONT hFont );

int ListViewWindowSetItemText( int nWhichItem, int nWhichSubItem, LPCTSTR lpszItemText );
