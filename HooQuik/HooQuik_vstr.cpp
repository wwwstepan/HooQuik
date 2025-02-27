// HooQuik.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "HooQuik.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HHOOK hMouseHook=NULL;

HWND wndHooQuik;

int HookisActive;
int nLots;
wchar_t lpStrBuffer[100];

int my_str_cmp(wchar_t *str1,wchar_t *str2){
	wchar_t *s1=str1, *s2=str2; int max=0;
	do{
		if(*s1!=*s2) return 0;
	}while(*s1++ && *s2++ && max++<100);
	return 1;
}

//BOOL CALLBACK MyEnumWindowProc(HWND hwindow, LPARAM param)
//{
//	GetWindowText(hwindow,lpStrBuffer,100);
//	if (my_str_cmp(lpStrBuffer,_T("Фьючерсы FORTS Ввод заявки"))){
//		*(int*)param=1;
//		return false;
//	}
//	return true;
//}

LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	HWND h_wnd_clk,h_wnd_stk,h_wnd_z;

	if((HookisActive) && (!nCode) && (wParam==WM_LBUTTONDOWN || wParam==WM_RBUTTONDOWN || wParam==WM_MOUSEWHEEL)){
		//tagMOUSEHOOKSTRUCT *_inf=(tagMOUSEHOOKSTRUCT*)lParam;
		MSLLHOOKSTRUCT *_inf=(MSLLHOOKSTRUCT*)lParam;
		h_wnd_clk=WindowFromPoint(_inf->pt);

		if (h_wnd_clk){

			h_wnd_stk=GetParent(h_wnd_clk);

			if (h_wnd_stk){
				GetWindowText(h_wnd_stk,lpStrBuffer,100);

				//if (my_str_cmp(lpStrBuffer,_T("Запуск программы"))){
				if (my_str_cmp(lpStrBuffer,_T("Stk~"))){
					
					if(wParam==WM_MOUSEWHEEL){
						SendMessage(wndHooQuik,WM_MOUSEWHEEL,_inf->mouseData,0);
						//return CallNextHookEx(hMouseHook,nCode,wParam,lParam);
						return 1;
					}

					int ctrl_state=GetAsyncKeyState(VK_CONTROL);
					int shift_state=GetAsyncKeyState(VK_SHIFT);

					HookisActive=0;
					
					mouse_event(MOUSEEVENTF_LEFTDOWN,_inf->pt.x,_inf->pt.y,0,0);
					
					//if(wParam==WM_RBUTTONDOWN)
						mouse_event(MOUSEEVENTF_LEFTDOWN,_inf->pt.x,_inf->pt.y,0,0);
					
					//if(h3){
					//	param=0;
						h_wnd_z=0;
						for(int i=0; i<20; i++){
							Sleep(25);
							h_wnd_z=GetForegroundWindow();
							if (h_wnd_z){
								GetWindowText(h_wnd_z,lpStrBuffer,100);
								//if (my_str_cmp(lpStrBuffer,_T("Фьючерсы FORTS Ввод заявки")))
								if (my_str_cmp(lpStrBuffer,_T("Акции 1-го уровня (эмулятор) Ввод заявки")))
									break;
								else h_wnd_z=0;
							}
							//h_wnd_Vvz=FindWindowEx(h3,0,0,_T("Фьючерсы FORTS Ввод заявки"));
							//EnumChildWindows(h3,WNDENUMPROC(MyEnumWindowProc),&param);
							//if(param) break;
						}
						if(!h_wnd_z){ HookisActive=1; return CallNextHookEx(hMouseHook,nCode,wParam,lParam); }
					//}else 
					//	Sleep(333);

					if(wParam==WM_RBUTTONDOWN)
						keybd_event( VK_SPACE, 0, 0, 0 );

					switch(nLots){
						case 2:
							keybd_event(VkKeyScan('2'),0,0,0);
							break;
						case 3:
							keybd_event(VkKeyScan('3'),0,0,0);
							break;
						case 4:
							keybd_event(VkKeyScan('4'),0,0,0);
							break;
						case 5:
							keybd_event(VkKeyScan('5'),0,0,0);
							break;
						case 6:
							keybd_event(VkKeyScan('6'),0,0,0);
							break;
						case 7:
							keybd_event(VkKeyScan('7'),0,0,0);
							break;
						case 8:
							keybd_event(VkKeyScan('8'),0,0,0);
							break;
						case 9:
							keybd_event(VkKeyScan('9'),0,0,0);
							break;
						case 10:
							keybd_event(VkKeyScan('1'),0,0,0);
							keybd_event(VkKeyScan('0'),0,0,0);
							break;
						case 11:
							keybd_event(VkKeyScan('1'),0,0,0);
							keybd_event(VkKeyScan('1'),0,0,0);
							break;
						case 12:
							keybd_event(VkKeyScan('1'),0,0,0);
							keybd_event(VkKeyScan('2'),0,0,0);
							break;
						case 13:
							keybd_event(VkKeyScan('1'),0,0,0);
							keybd_event(VkKeyScan('3'),0,0,0);
							break;
						case 14:
							keybd_event(VkKeyScan('1'),0,0,0);
							keybd_event(VkKeyScan('4'),0,0,0);
							break;
						case 15:
							keybd_event(VkKeyScan('1'),0,0,0);
							keybd_event(VkKeyScan('5'),0,0,0);
							break;
						case 16:
							keybd_event(VkKeyScan('1'),0,0,0);
							keybd_event(VkKeyScan('6'),0,0,0);
							break;
						case 17:
							keybd_event(VkKeyScan('1'),0,0,0);
							keybd_event(VkKeyScan('7'),0,0,0);
							break;
						case 18:
							keybd_event(VkKeyScan('1'),0,0,0);
							keybd_event(VkKeyScan('8'),0,0,0);
							break;
						case 19:
							keybd_event(VkKeyScan('1'),0,0,0);
							keybd_event(VkKeyScan('9'),0,0,0);
							break;
						case 20:
							keybd_event(VkKeyScan('2'),0,0,0);
							keybd_event(VkKeyScan('0'),0,0,0);
							break;
					}

					if(ctrl_state || shift_state){
						keybd_event( VK_SHIFT, 0, 0, 0 );
						keybd_event( VK_TAB, 0, 0, 0 );
						keybd_event( VK_TAB, 0, 2, 0 );
						keybd_event( VK_SHIFT, 0, 2, 0 );
						if(ctrl_state){
							keybd_event( VK_CONTROL, 0, 2, 0 );
							keybd_event( VK_DOWN, 0, 0, 0 );
							keybd_event( VK_DOWN, 0, 0, 0 );
						}else if(shift_state){
							keybd_event( VK_SHIFT, 0, 2, 0 );
							keybd_event( VK_UP, 0, 0, 0 );
							keybd_event( VK_UP, 0, 0, 0 );
						}
					}

					//keybd_event( VK_RETURN, 0, 0, 0 );

					HookisActive=1;

					return 1;
				}
			}
		}
	}

	return CallNextHookEx(hMouseHook,nCode,wParam,lParam);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HOOQUIK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HOOQUIK));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HOOQUIK));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= _T("");//;MAKEINTRESOURCE(IDC_HOOQUIK);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   wndHooQuik = CreateWindowW(szWindowClass, szTitle, WS_SYSMENU|WS_MINIMIZEBOX,
      600, 660, 175, 70, NULL, NULL, hInstance, NULL);

   if (!wndHooQuik)
   {
      return FALSE;
   }

   HookisActive=0;
   nLots=1;

   SetWindowsHookEx(WH_MOUSE_LL,&MouseProc,hInstance,0);

   ShowWindow(wndHooQuik, nCmdShow);
   UpdateWindow(wndHooQuik);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HFONT my_font;
	wchar_t lpBuf[100];

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
		case ID_ACCELERATOR_QUIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		my_font = CreateFont(13, 9, 0, 0,
			FW_NORMAL, FALSE, FALSE, FALSE,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_ROMAN,
			_T("Arial"));

		SelectObject(hdc, my_font);

		if(HookisActive){
			swprintf_s(lpBuf,100,_T("on %i     "),nLots);
		}else{
			swprintf_s(lpBuf,100,_T("off %i    "),nLots);
		}
		TextOut(hdc, 4, 4, lpBuf, 8);

		//TextOut(hdc,20,4,lpBuf,100);

        DeleteObject(my_font);

		EndPaint(hWnd, &ps);
		
		if(HookisActive){
			SetWindowText(wndHooQuik,szTitle);
		}else{
			swprintf_s(lpBuf,100,_T("off %s"),szTitle);
			SetWindowText(wndHooQuik,lpBuf);
		}

		break;

	case WM_MOUSEWHEEL:
		{
			short int ww = HIWORD(wParam);

			if (ww>199) nLots+=3;
			else if (ww>0) nLots++;
			else if (ww<-199) nLots-=3;
			else if (ww<0) nLots--;

			if (nLots>20) nLots=20;
			else if(nLots<1) nLots=1;

			InvalidateRect(hWnd,NULL,0);

			//MessageBox(0,_T("Wheel"),_T("i"),0);
		}
		break;
	case WM_LBUTTONDOWN:
		
		HookisActive=!HookisActive;

		InvalidateRect(hWnd,NULL,0);

		break;
	case WM_DESTROY:
		UnhookWindowsHookEx(hMouseHook);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
    case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return (INT_PTR)FALSE;
}
