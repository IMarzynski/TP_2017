// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw.h"
#include "Elevator.h"
#include "BuildingFloor.h"
#include "Passenger.h"
#include <queue>
#include <string> 

#define MAX_LOADSTRING 100
#define TMR_1 1

#define ID_BUTTONp1d2      200
#define ID_BUTTONp1d3      201
#define ID_BUTTONp1d4      202
#define ID_BUTTONp2d3      203
#define ID_BUTTONp2d4      204
#define ID_BUTTONp2d1      205
#define ID_BUTTONp3d2      206
#define ID_BUTTONp3d1      207
#define ID_BUTTONp3d4      208
#define ID_BUTTONp4d2      209
#define ID_BUTTONp4d3      210
#define ID_BUTTONp4d1      211


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

using namespace std;

INT value;
HWND hwndButton;
HWND hwndStatic;

const int winWidth = 1024;
const int winHeight = 768;
const int nrOfFlrs = 4;

RECT drawArea = { 150, 0, 600, winHeight};//450, 0, 600, winHeight
RECT drawAreaWeight = { winWidth - 100, 10, winWidth, 50};//450, 0, 600, winHeight
queue <int> floorQue;

Elevator elevator = Elevator(winHeight - 240, 0);
BuildingFloor bldngFloor[nrOfFlrs];
queue <Passenger> queForElevator[nrOfFlrs + 1];
vector <Passenger> queInElevator;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void drawPassengerInElevator(HDC hdc) {
	Graphics graphics(hdc);
	Pen penPassenger(Color(255, 255, 0, 255));
	int distance = 15;
	if (!queInElevator.empty()) {
		for (int i = 0; i < queInElevator.size(); i++) {
			graphics.DrawRectangle(&penPassenger, queInElevator.front().getPositionX() - distance, elevator.getPosition(), 10, 50);
			distance += 10;
		}
	}else {
		distance = 15;
	}
}

void drawPassenger(HDC hdc) {
	Graphics graphics(hdc);
	Pen penPassenger(Color(255, 255, 0, 255));
	int distance = 10;
	for (int i = 1; i <= nrOfFlrs; i++) {
		if (!queForElevator[i].empty()) {
			for (int j = 0; j < queForElevator[i].size(); j++) {
				graphics.DrawRectangle(&penPassenger, queForElevator[i].front().getPositionX() - distance, queForElevator[i].front().getPositionY(), 10, 50);
				distance += 15;
			}
		}else {
			distance = 15;
		}
	}
}

void showWeight(HDC hdc) {
	int weight = queInElevator.size() * 70;
	string s = to_string(weight);
	char const *pchar = s.c_str();

	TextOut(hdc, winWidth - 100, 10, pchar, s.length());
}

void MyOnPaint(HDC hdc){
	elevator.setPosition(winHeight - 240 - value);
	Graphics graphics(hdc);
	Pen penElevator(Color(255,0,0,255));
	Pen penPlatform(Color(255, 0, 255, 255));
	drawPassenger(hdc);
	drawPassengerInElevator(hdc);
	graphics.DrawRectangle(&penPlatform, 0, winHeight - 100, 450, 10);
	graphics.DrawRectangle(&penPlatform, 0, winHeight - 250, 450, 10);
	graphics.DrawRectangle(&penPlatform, 0, winHeight - 400, 450, 10);
	graphics.DrawRectangle(&penPlatform, 0, winHeight - 550, 450, 10);

	graphics.DrawRectangle(&penElevator, winWidth/2 - 50, elevator.getPosition(),100, 150);
}

void createPsngr(int floorNr, int destFloor) {
	bldngFloor[floorNr].incrPsngrAmount();
	Passenger psngr = Passenger(bldngFloor[floorNr].getFloorStop() + 100, floorNr, destFloor);
	psngr.setPositionX(420 + queForElevator[floorNr].size() * 10);
	queForElevator[floorNr].push(psngr);
}

void initFloors() {

	int distance;
	for (int i = 1; i <= nrOfFlrs; i++) {
		switch (i) {
		case 1:
			distance = winHeight - 240;
			break;
		case 2:
			distance = 374;
			break;
		case 3:
			distance = 222;
			break;
		case 4:
			distance = 66;
			break;
		default:
			distance = 0;
			break;
		}
		bldngFloor[i] = BuildingFloor(distance);
	}
	
}

void goToFloor(int floorNr) {

	if (elevator.getPosition() > bldngFloor[floorNr].getFloorStop()) {
		elevator.setDirection(1);
	}
	else if (elevator.getPosition() < bldngFloor[floorNr].getFloorStop()) {
		elevator.setDirection(2);
	}
	else {
		elevator.setDirection(0);
	}

}

void checkAndTake(int floorNr) {
	int passengerNrOnFloor = queForElevator[floorNr].size();
	if (!queForElevator[floorNr].empty()) {
		for (int i = 0; i < passengerNrOnFloor; i++) {
			if (i == 8) {
				break;
			}
			queInElevator.push_back(queForElevator[floorNr].front());
			queInElevator.front().setPositionX(winWidth / 2 + 50);
			queInElevator.front().setPositionY(elevator.getPosition() + 50);
			queForElevator[floorNr].pop();
		}
	}
	if (!queInElevator.empty()) {
		if (queInElevator.size() > 1) {
			for (int i = 0; i < queInElevator.size(); i++) {
				if (queInElevator[i].getDestFloor() == floorNr) {
					queInElevator.erase(queInElevator.begin() + i);
					i = 0;
				}
			}
		}
		if (queInElevator.size() == 1) {
			if (queInElevator[0].getDestFloor() == floorNr) {
				queInElevator.erase(queInElevator.begin());
			}
		}
	}
}

bool checkFloor(int floorNr) {

	if (elevator.getPosition() == bldngFloor[floorNr].getFloorStop()) {
		elevator.setDirection(0);
		checkAndTake(floorNr);
		return true;
	}
	else {
		return false;
	}

}

void changeDirection() {

	switch (elevator.getDirection()) {
	case 0:
		value += 0;
		break;
	case 1:
		value += 2;
		break;
	case 2:
		value -= 2;
		break;
	default:
		value += 0;
		break;
	}
}

void rideProcess() {

	goToFloor(floorQue.front());
	if (checkFloor(floorQue.front())) {
		Sleep(500);//1000
		elevator.setDirection(0);
		floorQue.pop();
	}
}

int OnCreate(HWND window)
{
   initFloors();
   SetTimer(window, TMR_1, 25, 0);
   return 0;
}



int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	value= 10;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_DRAW);
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable
   const int btnSize = 20;

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, winWidth, winHeight, NULL, NULL, hInstance, NULL);
   /*1*/
   hwndButton = CreateWindow(TEXT("button"), TEXT("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	   0, winHeight - 270, btnSize, btnSize, hWnd, (HMENU)ID_BUTTONp2d1, hInstance, NULL);

   hwndButton = CreateWindow(TEXT("button"), TEXT("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	   0, winHeight - 420, btnSize, btnSize, hWnd, (HMENU)ID_BUTTONp3d1, hInstance, NULL);

   hwndButton = CreateWindow(TEXT("button"), TEXT("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	   0, winHeight - 570, btnSize, btnSize, hWnd, (HMENU)ID_BUTTONp4d1, hInstance, NULL);
   /*2*/
   hwndButton = CreateWindow(TEXT("button"), TEXT("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	   btnSize, winHeight - 120, btnSize, btnSize, hWnd, (HMENU)ID_BUTTONp1d2, hInstance, NULL);

   hwndButton = CreateWindow(TEXT("button"), TEXT("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	   btnSize, winHeight - 420, btnSize, btnSize, hWnd, (HMENU)ID_BUTTONp3d2, hInstance, NULL);

   hwndButton = CreateWindow(TEXT("button"), TEXT("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	   btnSize, winHeight - 570, btnSize, btnSize, hWnd, (HMENU)ID_BUTTONp4d2, hInstance, NULL);
   /*3*/
   hwndButton = CreateWindow(TEXT("button"), TEXT("3"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	   2 * btnSize, winHeight - 120, btnSize, btnSize, hWnd, (HMENU)ID_BUTTONp1d3, hInstance, NULL);

   hwndButton = CreateWindow(TEXT("button"), TEXT("3"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	   2 * btnSize, winHeight - 270, btnSize, btnSize, hWnd, (HMENU)ID_BUTTONp2d3, hInstance, NULL);

   hwndButton = CreateWindow(TEXT("button"), TEXT("3"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	   2 * btnSize, winHeight - 570, btnSize, btnSize, hWnd, (HMENU)ID_BUTTONp4d3, hInstance, NULL);
   /*4*/
   hwndButton = CreateWindow(TEXT("button"), TEXT("4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	   3 * btnSize, winHeight - 120, btnSize, btnSize, hWnd, (HMENU)ID_BUTTONp1d4, hInstance, NULL);

   hwndButton = CreateWindow(TEXT("button"), TEXT("4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	   3 * btnSize, winHeight - 270, btnSize, btnSize, hWnd, (HMENU)ID_BUTTONp2d4, hInstance, NULL);

   hwndButton = CreateWindow(TEXT("button"), TEXT("4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	   3 * btnSize, winHeight - 420, btnSize, btnSize, hWnd, (HMENU)ID_BUTTONp3d4, hInstance, NULL);

   /*statyczny tekst*//*
   hwndStatic = CreateWindowEx(0, TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE |
	   SS_LEFT, winWidth - 100, 10, 100, 100, hWnd, NULL, hInstance, NULL);
   SetWindowText(hwndStatic, TEXT("Masa pasa¿erów"));
   */
   OnCreate(hWnd);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
	//OnCreate(hWnd,wParam,lParam);
	//OnTimer(hWnd,wParam,lParam);

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
			DestroyWindow(hWnd);
			break;
		case ID_BUTTONp1d2:
			createPsngr(1, 2);
			floorQue.push(1);
			floorQue.push(2);
			break;
		case ID_BUTTONp1d3:
			createPsngr(1, 3);
			floorQue.push(1);
			floorQue.push(3);
			break;
		case ID_BUTTONp1d4:
			createPsngr(1, 4);
			floorQue.push(1);
			floorQue.push(4);
			break;
		case ID_BUTTONp2d1:
			createPsngr(2, 1);
			floorQue.push(2);
			floorQue.push(1);
			break;
		case ID_BUTTONp2d3:
			createPsngr(2, 3);
			floorQue.push(2);
			floorQue.push(3);
			break;
		case ID_BUTTONp2d4:
			createPsngr(2, 4);
			floorQue.push(2);
			floorQue.push(4);
			break;
		case ID_BUTTONp3d1:
			createPsngr(3, 1);
			floorQue.push(3);
			floorQue.push(1);
			break;
		case ID_BUTTONp3d2:
			createPsngr(3, 2);
			floorQue.push(3);
			floorQue.push(2);
			break;
		case ID_BUTTONp3d4:
			createPsngr(3, 4);
			floorQue.push(3);
			floorQue.push(4);
			break;
		case ID_BUTTONp4d1:
			createPsngr(4, 1);
			floorQue.push(4);
			floorQue.push(1);
			break;
		case ID_BUTTONp4d2:
			createPsngr(4, 2);
			floorQue.push(4);
			floorQue.push(2);
			break;
		case ID_BUTTONp4d3:
			createPsngr(4, 3);
			floorQue.push(4);
			floorQue.push(3);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		MyOnPaint(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
			case TMR_1:
				//force window to repaint
				if (!floorQue.empty()) {
					changeDirection();
					rideProcess();
				}
				InvalidateRect(hWnd, &drawArea, TRUE);
				InvalidateRect(hWnd, &drawAreaWeight, TRUE);
				hdc = BeginPaint(hWnd, &ps);
				MyOnPaint(hdc);
				showWeight(hdc);
				EndPaint(hWnd, &ps);
			break;
		}

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
