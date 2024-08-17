// HtmlFile.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH									100

#define HTML_FILE_PREFIX_FORMAT_STRING											"<head>\r\n"					\
																				"<title>%s</title>\r\n"			\
																				"</head>\r\n"					\
																				"<body>\r\n"					\
																				"<h1>%s</h1>\r\n"

#define HTML_FILE_SUFFIX														"</body>"

#define HTML_FILE_TABLE_PREFIX													"<table>\r\n"
#define HTML_FILE_TABLE_ROW_PREFIX												"<tr>"
#define HTML_FILE_TABLE_ITEM_PREFIX												"<td>"

#define HTML_FILE_TABLE_SUFFIX													"</table>\r\n"
#define HTML_FILE_TABLE_ROW_SUFFIX												"</tr>\r\n"
#define HTML_FILE_TABLE_ITEM_SUFFIX												"</td>"
