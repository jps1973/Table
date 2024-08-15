cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra Table.cpp ListViewWindow.cpp StatusBarWindow.cpp Resource.o -o Table.exe
