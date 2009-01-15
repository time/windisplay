/*
  Copyright 2009 Time
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "windows.h"
#include "ocidl.h"
#include "olectl.h"
#include "stdio.h"

HBITMAP hbmp;
BITMAP bmp;

LRESULT CALLBACK WndProc(
  HWND hwnd,
  UINT uMsg,
  WPARAM wParam,
  LPARAM lParam
) {
  PAINTSTRUCT ps; 
  HDC hdc, memdc;

  switch (uMsg)
  { 
    case WM_CREATE: 
      return 0 ; 
      
    case WM_PAINT: 
      hdc = BeginPaint(hwnd, &ps);
      memdc = CreateCompatibleDC(hdc);
      SelectObject(memdc, hbmp);
      BitBlt(
        hdc,            // handle to destination DC
        0,              // x-coord of destination upper-left corner
        0,              // y-coord of destination upper-left corner
        bmp.bmWidth,    // width of destination rectangle
        bmp.bmHeight,   // height of destination rectangle
        memdc,          // handle to source DC
        0,              // x-coordinate of source upper-left corner
        0,              // y-coordinate of source upper-left corner
        SRCCOPY         // raster operation code
      );
      DeleteDC(memdc);
      EndPaint(hwnd, &ps); 
      return 0 ; 
      
    case WM_DESTROY: 
        PostQuitMessage(0);
        return 0;
  }
  
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
) {
  static TCHAR lpszClassName[] = TEXT ("display") ;
  HWND         hwnd;
  MSG          msg ;
  WNDCLASS     wndclass ;
  char *filename = lpCmdLine;
  size_t filename_length = strlen(filename);
  RECT rect;
  
  if (filename_length >= 2)
  {
    if (filename[0] == '"')
    {
      filename += 1;
      filename_length -= 1;
    }
    if (filename[filename_length-1] == '"')
    {
      filename[filename_length-1] = '\0';
      filename_length -= 1;
    }
  }
  
  hbmp = (HBITMAP)LoadImage(
    NULL,
    filename,
    IMAGE_BITMAP,
    0, 0,
    LR_DEFAULTSIZE | LR_LOADFROMFILE
  );
  
  if (hbmp == NULL)
  {
    printf("Unable to load file \"%s\": %d", filename, GetLastError());
    return 1;
  }
  GetObject(hbmp,sizeof(bmp),&bmp);

  wndclass.cbClsExtra    = 0 ;
  wndclass.cbWndExtra    = 0 ;
  
  // The default window background
  wndclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
  
  // The system, IDC_ARROW cursor
  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
  
  //The system IDI_APPLICATION icon
  wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
  wndclass.hInstance     = hInstance ;
  wndclass.lpfnWndProc   = WndProc ;
  
  //The name of the class, needed for CreateWindow
  wndclass.lpszClassName = lpszClassName;
  wndclass.lpszMenuName  = NULL ;
  wndclass.style         = CS_HREDRAW | CS_VREDRAW ;

  //Window rect
  rect.left = 0;
  rect.top = 0;
  rect.right = bmp.bmWidth;
  rect.bottom = bmp.bmHeight;
  
  AdjustWindowRect(
      &rect,
      WS_OVERLAPPEDWINDOW,
      FALSE
  );

  RegisterClass (&wndclass);
  hwnd = 
      CreateWindow (lpszClassName, // pointer to registered class name
          filename,                                             // pointer to window name
          WS_OVERLAPPEDWINDOW,                                  // window style
          CW_USEDEFAULT, CW_USEDEFAULT,                         // position of window
          rect.right - rect.left, rect.bottom - rect.top,       // size of window
          NULL,         // handle to parent or owner window
          NULL,         // handle to menu 
          hInstance,    // handle to application instance
          NULL);        // pointer to window-creation data

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
   
    while (GetMessage(&msg, NULL, 0, 0))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
