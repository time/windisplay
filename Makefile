
windisplay.exe: windisplay.c
	winegcc windisplay.c -o windisplay.exe -lgdi32 -luuid -lole32 -loleaut32
	mv windisplay.exe.so windisplay.exe
