#include "windows.h"
#include "ocidl.h"
#include "olectl.h"

HBITMAP hbmp;
BITMAP bmp;

LRESULT CALLBACK WndProc(
  HWND hwnd,
  UINT uMsg,
  WPARAM wParam,
  LPARAM lParam
) {
  PAINTSTRUCT ps; 
  HDC hdc; 
  switch (uMsg)
  { 
    case WM_CREATE: 
      return 0 ; 
      
    case WM_PAINT: 
      hdc = BeginPaint(hwnd, &ps);
      HDC memdc = CreateCompatibleDC(hdc);
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
  
  printf("Loading %s...\n", filename);
  hbmp = (HBITMAP)LoadImage(
    NULL,
    filename,
    IMAGE_BITMAP,
    0, 0,
    LR_DEFAULTSIZE | LR_LOADFROMFILE
  );
  
  if (hbmp == NULL)
  {
    printf("Error loading bmp: %d\n", GetLastError());
    exit(1);
  }
  GetObject(hbmp,sizeof(bmp),&bmp);

  wndclass.cbClsExtra    = 0 ;
  wndclass.cbWndExtra    = 0 ;
  
  // The default window background
  wndclass.hbrBackground = COLOR_WINDOW;
  
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
  RECT rect;
  rect.left = 0;
  rect.top = 0;
  rect.right = bmp.bmWidth;
  rect.bottom = bmp.bmHeight;
  
  AdjustWindowRect(
      &rect,
      WS_OVERLAPPEDWINDOW,
      NULL
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
    return msg.wParam;
}
