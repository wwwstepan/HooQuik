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
int isTest;
int nLots;
wchar_t lpStrBuffer[100];
wchar_t szMessage[100];

int globX,globY;

int my_str_cmp(wchar_t *str1,wchar_t *str2){
	wchar_t *s1=str1, *s2=str2; int max=0;
	do{
		if(*s1!=*s2) return 0;
	}while(*s1++ && *s2++ && max++<100);
	return 1;
}

HWND WaitForOrderWindow()
{
	HWND h_wnd_z;
	for(int i=0; i<20; i++){
		Sleep(25);
		h_wnd_z=GetForegroundWindow();
		if (h_wnd_z){
			GetWindowText(h_wnd_z,lpStrBuffer,100);
			if(
				(my_str_cmp(lpStrBuffer,_T("Фьючерсы FORTS Ввод заявки")))
			||	(my_str_cmp(lpStrBuffer,_T("Акции 1-го уровня (эмулятор) Ввод заявки")))
			)
				return h_wnd_z;
		}
	}
	return 0;
}

void EnterSizePosition(){
	if(nLots>0 && nLots<1000){
		int l1=nLots;
		int l100=l1/100;
		if(l100){ keybd_event('0'+l100,0,0,0); l1%=100; }
		int l10=l1/10;
		if(l10 || l100){ keybd_event('0'+l10,0,0,0); l1%=10; }
		keybd_event('0'+l1,0,0,0);
	}
}

LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	HWND h_wnd_clk,h_wnd_stk,h_wnd_z;
	RECT stkRect;
	int is_buy,is_sell,ctrl_state;

	if((HookisActive) && (!nCode) && (wParam==WM_LBUTTONDOWN || wParam==WM_MOUSEWHEEL)){
		
		MSLLHOOKSTRUCT *_inf=(MSLLHOOKSTRUCT*)lParam;
		h_wnd_clk=WindowFromPoint(_inf->pt);

		if (h_wnd_clk){

			h_wnd_stk=GetParent(h_wnd_clk);

			if (h_wnd_stk){
				GetWindowText(h_wnd_stk,lpStrBuffer,100);

				if ((wParam==WM_MOUSEWHEEL) && (my_str_cmp(lpStrBuffer,_T("Stk~")))){

					if(wParam==WM_MOUSEWHEEL){
						HookisActive=0;
						for (int i=0; i<30; i++)
							SendMessage(h_wnd_stk,WM_MOUSEWHEEL,_inf->mouseData,0);
						HookisActive=1;
					}


					// no clicks!
					return 1;


					GetWindowRect(h_wnd_stk,&stkRect);

					is_buy=0; is_sell=0;

					int stk_x_2=(stkRect.right-stkRect.left)>>1;
					int stk_x_4=stk_x_2>>2;
					int stk_x_c=stkRect.left + stk_x_2;

					int stk_x_buy = stk_x_c - stk_x_4;
					int stk_x_sell= stk_x_c + stk_x_4;

					if(_inf->pt.x>stk_x_sell)
						is_sell=1;
					else if(_inf->pt.x<stk_x_buy)
						is_buy=1;
					
					//if(!is_buy && !is_sell){
					//	//wcscpy_s(szMessage,100,_T("no order"));
					//	//SendMessage(wndHooQuik,WM_PAINT,0,0);
					//	return CallNextHookEx(hMouseHook,nCode,wParam,lParam);
					//}

					ctrl_state=GetAsyncKeyState(VK_CONTROL);

					HookisActive=0;
					
					mouse_event(MOUSEEVENTF_LEFTDOWN,_inf->pt.x,_inf->pt.y,0,0);
					mouse_event(MOUSEEVENTF_LEFTUP,_inf->pt.x,_inf->pt.y,0,0);
					
					h_wnd_z=WaitForOrderWindow();
					if(!h_wnd_z){ HookisActive=1; return CallNextHookEx(hMouseHook,nCode,wParam,lParam); }

					if(ctrl_state) keybd_event( VK_CONTROL, 0, 2, 0 );

					if (is_buy || is_sell){
						keybd_event( VK_SPACE, 0, 0, 0 );
						keybd_event( VK_SPACE, 0, 2, 0 );

						if(is_sell){
 							keybd_event( VK_SPACE, 0, 0, 0 );
 							keybd_event( VK_SPACE, 0, 2, 0 );
						}
					}
					
					EnterSizePosition();
	
					if(is_buy || is_sell){
						if (ctrl_state){

							keybd_event( VK_SHIFT, 0, 0, 0 );
							keybd_event( VK_TAB, 0, 0, 0 );
							keybd_event( VK_TAB, 0, 2, 0 );
							keybd_event( VK_SHIFT, 0, 2, 0 );

							int pips=2;

							if(ctrl_state) keybd_event( VK_CONTROL, 0, 2, 0 );

							for(int i=0; i<pips; i++)
								if(is_sell){
									keybd_event( VK_DOWN, 0, 0, 0 );
									keybd_event( VK_DOWN, 0, 2, 0 );
								}else if(is_buy){
									keybd_event( VK_UP, 0, 0, 0 );
									keybd_event( VK_UP, 0, 2, 0 );
								}
						}
						
						if(!isTest){
							keybd_event( VK_RETURN, 0, 0, 0 );
							keybd_event( VK_RETURN, 0, 2, 0 );
						}
					}

					HookisActive=1;

					return 1;

				}else if (my_str_cmp(lpStrBuffer,_T("Ord~"))){
					if(wParam==WM_MOUSEWHEEL){
						short int ww = HIWORD(_inf->mouseData);
						if(ww){
							HookisActive=0;
							
							mouse_event(MOUSEEVENTF_RIGHTDOWN,_inf->pt.x,_inf->pt.y,0,0);
							mouse_event(MOUSEEVENTF_RIGHTUP,_inf->pt.x,_inf->pt.y,0,0);
							mouse_event(MOUSEEVENTF_RIGHTDOWN,_inf->pt.x,_inf->pt.y,0,0);
							mouse_event(MOUSEEVENTF_RIGHTUP,_inf->pt.x,_inf->pt.y,0,0);

							mouse_event(MOUSEEVENTF_LEFTDOWN,_inf->pt.x,_inf->pt.y,0,0);
							mouse_event(MOUSEEVENTF_LEFTUP,_inf->pt.x,_inf->pt.y,0,0);
							mouse_event(MOUSEEVENTF_LEFTDOWN,_inf->pt.x,_inf->pt.y,0,0);
							mouse_event(MOUSEEVENTF_LEFTUP,_inf->pt.x,_inf->pt.y,0,0);
							
							ctrl_state=GetAsyncKeyState(VK_CONTROL);

							h_wnd_z=WaitForOrderWindow();
							if(!h_wnd_z){ HookisActive=1; return CallNextHookEx(hMouseHook,nCode,wParam,lParam); }

							if(ctrl_state) keybd_event( VK_CONTROL, 0, 2, 0 );

							EnterSizePosition();

							keybd_event( VK_SHIFT, 0, 0, 0 );
							keybd_event( VK_TAB, 0, 0, 0 );
							keybd_event( VK_TAB, 0, 2, 0 );
							keybd_event( VK_SHIFT, 0, 2, 0 );
							
							if(!ctrl_state){
								keybd_event( VK_CONTROL, 0, 0, 0 );

								if(ww>0){
									keybd_event( VK_UP, 0, 0, 0 );
									keybd_event( VK_UP, 0, 2, 0 );
								}else{
									keybd_event( VK_DOWN, 0, 0, 0 );
									keybd_event( VK_DOWN, 0, 2, 0 );
								}
								keybd_event( VK_CONTROL, 0, 2, 0 );
							}
							
							if(!ctrl_state && !isTest){
								keybd_event( VK_RETURN, 0, 0, 0 );
								keybd_event( VK_RETURN, 0, 2, 0 );
							}

							HookisActive=1;
							return 1;
						}
					}
				}
			}
		}
	}

	return CallNextHookEx(hMouseHook,nCode,wParam,lParam);
}

BOOL CALLBACK MyEnumWindowProc(HWND hwindow, LPARAM param)
{
	GetWindowText(hwindow,lpStrBuffer,100);
	if(*lpStrBuffer){
		size_t ln=wcslen(lpStrBuffer);
		size_t sr=wcscspn(lpStrBuffer,_T("Информационная система QUIK"));
		if(sr && sr<ln){
			GetClassName(hwindow,lpStrBuffer,100);
			if(my_str_cmp(lpStrBuffer,_T("InfoClass"))){
				RECT stkRect;
				GetWindowRect(hwindow,&stkRect);
				globX=stkRect.right;
				globY=stkRect.bottom-70;
			}
		}
	}
	return true;
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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	globX=600;
	globY=660;
	EnumWindows(WNDENUMPROC(MyEnumWindowProc),0);

	HDC hDCScreen =  GetDC(NULL);
	int maxX = GetDeviceCaps(hDCScreen,  HORZRES);
	int maxY = GetDeviceCaps(hDCScreen,  VERTRES);
	ReleaseDC(NULL, hDCScreen);	

	if(globX>maxX-175) globX=maxX-175;
	if(globY>maxY-70) globY=maxY-70;

	wndHooQuik = CreateWindowW(szWindowClass, szTitle, WS_SYSMENU|WS_MINIMIZEBOX,
		globX, globY, 175, 70, NULL, NULL, hInstance, NULL);

	if (!wndHooQuik)
	{
		return FALSE;
	}

	HookisActive=0;
	isTest=0;
	nLots=1;

	SetWindowsHookEx(WH_MOUSE_LL,&MouseProc,hInstance,0);

	ShowWindow(wndHooQuik, nCmdShow);
	UpdateWindow(wndHooQuik);

	SetTimer(wndHooQuik, 0, 4000, 0);

	return TRUE;
}

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
	case WM_TIMER:
		if(*szMessage){
			*szMessage=0;
			InvalidateRect(hWnd,NULL,0);
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

		if(isTest)
			TextOut(hdc, 140, 4, _T("test"), 8);
		else
			TextOut(hdc, 140, 4, _T("real"), 8);

		//if(*szMessage)
		//	TextOut(hdc, 4, 24, szMessage, wcsnlen(szMessage,100));

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
		{
			int x=LOWORD(lParam);

			if(x>140){
				isTest=!isTest;
			}else{
				HookisActive=!HookisActive;
			}

			InvalidateRect(hWnd,NULL,0);
		}
		break;
	//case SW_MINIMIZE: // событие минимизации окна?
	//	HookisActive=0;
	//	InvalidateRect(hWnd,NULL,0);
	//	break;
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
	}
	return (INT_PTR)FALSE;
}
