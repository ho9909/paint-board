#include "head.h"
#include <stdio.h>
#include <math.h>
#include <tchar.h>


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

LPCTSTR lpszClass = TEXT("그림판");			// LPCSTR = char *
HINSTANCE g_hinstance;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, //WINAPI : 윈도우 프로그램이라는 의미
	LPSTR lpszCmdLine, int nCmdShow)						 //hInstance : 운영체제의 커널이 응용 프로그램에 부여한 ID
{																 //szCmdLine : 커멘트라인 상에서 프로그램 구동 시 전달된 문자열
	HWND	hwnd;												 //iCmdShow : 윈도우가 화면에 출력될 형태
	MSG		msg;
	WNDCLASS WndClass;											//WndClass 라는 구조체 정의									 

	WndClass.lpfnWndProc = WndProc;							//프로시저 함수명	
	WndClass.cbClsExtra = 0;			 					//O/S 사용 여분 메모리 (Class)
	WndClass.cbWndExtra = 0;			 					//O/s 사용 여분 메모리 (Window)
	WndClass.hInstance = hInstance;						//응용 프로그램 ID
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	  //아이콘 유형
	WndClass.hCursor = LoadCursor(NULL, IDC_CROSS);		  //커서 유형
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//배경색   
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);		 //메뉴 이름
	WndClass.lpszClassName = lpszClass;	//클래스 이름
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;			//출력스타일 : 수직/수평의 변화시 다시 그림		//Horizontal, Vertical 
	RegisterClass(&WndClass);									 //앞서 정의한 윈도우 클래스의 주소 // os 에다가 지금 이 클래스를 등록시킴 

	g_hinstance = hInstance; // 전역 인스턴스 핸들
	hwnd = CreateWindow(lpszClass,								 //윈도우가 생성되면 핸들(hwnd)이 반환		//실제 윈도우를 만들기 시작 
		lpszClass,												 //윈도우 클래스, 타이틀 이름
		WS_OVERLAPPEDWINDOW,									 //윈도우 스타일
		CW_USEDEFAULT,											 //윈도우 위치, x좌표
		CW_USEDEFAULT,											 //윈도우 위치, y좌표
		CW_USEDEFAULT,											 //윈도우 폭   
		CW_USEDEFAULT,											 //윈도우 높이   
		NULL,													 //부모 윈도우 핸들	 
		NULL,													 //메뉴 핸들
		hInstance,    											 //응용 프로그램 ID
		NULL     												 //생성된 윈도우 정보
		);
	ShowWindow(hwnd, nCmdShow);									 //윈도우의 화면 출력
	UpdateWindow(hwnd);											 //O/S 에 WM_PAINT 메시지 전송

	while (GetMessage(&msg, NULL, 0, 0))							 //WinProc()에서 PostQuitMessage() 호출 때까지 처리
	{															 // 
		TranslateMessage(&msg);
		DispatchMessage(&msg);									 //WinMain -> WinProc  
	}
	return (int)msg.wParam;
}

HDC hdc, backMemDC, MemDC;
HBITMAP OldhBit, hBit, sizeHBIT;
BOOL draw = FALSE;
RECT rt, mrt;
HPEN hpen, old_hpen;
HBRUSH hbrush, old_hbrush;
int x = 0, y = 0, old_x = 0, old_y = 0, shape = 0, size = 1, p_color = 0x000000, b_color = 0x000000,e_color = 0xffffff, line_shape = 0, brus = 0, new_flag = 0, max_width, max_height;
int pencolor=0, brucolor=0;
OPENFILENAME SFN, OFN;
HANDLE fp;
DWORD readn;
BITMAP map;
BITMAPINFOHEADER infohead;
BITMAPFILEHEADER hf;
BYTE* Data;
float xPos = 0.0, yPos = 0.0;
TCHAR lpstrFile[100], lpstrFile2[100];//파일이름 저장위함
LinkedList control;
LinkedList tmp;
TCHAR vi_1[100000][13]; //TCHAR vi_1[100][10]; // 1:크기, 2: old.x, 3: old.y, 4:now.x, 5: now.y 6: 굵기, 7. 선색상, 8: 채우기 여부, 9: 채우기 색상, 10: 구조체 개수
int vi_free[100000][13];
RAWINFOHEADER rw;
POINT pt_Mouse ={0,0};
int cur_tmp=0;



LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hwnd;
	OFN.lpstrFilter = TEXT("비트맵 파일(.bmp)\0*.bmp\0VI 파일(.vi)\0*.vi\0raw 파일(.raw)\0*.raw\0모든파일(*.*)\0*.*\0");
	OFN.lpstrFile = lpstrFile;
	OFN.Flags = OFN_NOCHANGEDIR;
	OFN.nMaxFile = MAX_PATH;
	HRGN hRgn1;
	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rt);
		hdc = GetDC(hwnd);

		backMemDC = CreateCompatibleDC(hdc);
		MemDC = CreateCompatibleDC(hdc);

		OldhBit = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
		hBit = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);

		(HBITMAP)SelectObject(backMemDC, OldhBit);
		(HBITMAP)SelectObject(MemDC, hBit);

		FillRect(backMemDC, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));
		FillRect(MemDC, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));
		max_width = rt.right;
		max_height = rt.bottom;

		hpen = CreatePen(PS_SOLID, size, 0x000000);
		old_hpen = (HPEN)SelectObject(MemDC, hpen);
		hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush);

		control.count = 0;
		control.head_ptr = NULL;
		control.now_ptr = NULL;
		hRgn1 = CreateRectRgn(100,100,400,250);
		FillRgn(hdc,hRgn1,(HBRUSH)GetStockObject(GRAY_BRUSH));
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case ID_LINE:
			shape = 1;
			break;
		case ID_CIRCLE:
			shape = 2;
			break;
		case ID_TRIANGLE:
			shape = 3;
			break;
		case ID_RECTANGLE:
			shape = 4;
			break;
		case ID_FREELINE:
			shape = 0;
			break;
		case ID_ERASE:
			shape = 0;
			e_color = 0xffffff;
			switch (line_shape) {
			case(0):
				make_penE();
				break;
			case(1):
				make_dashpenE();
				break;
			case(2):
				make_dotpenE();
				break;
			}
			break;
		case ID_PX_1PX:
			size = 1;
			switch (line_shape) {
			case(0):
				make_pen();
				break;
			case(1):
				make_dashpen();
				break;
			case(2):
				make_dotpen();
				break;
			}
			break;
		case ID_PX_5PX:
			size = 5;
			switch (line_shape) {
			case(0):
				make_pen();
				break;
			case(1):
				make_dashpen();
				break;
			case(2):
				make_dotpen();
				break;
			}
			break;
		case ID_PX_10PX:
			size = 10;
			switch (line_shape) {
			case(0):
				make_pen();
				break;
			case(1):
				make_dashpen();
				break;
			case(2):
				make_dotpen();
				break;
			}
			break;
		case ID_BLACK:
			pencolor = 0;
			p_color = 0x000000;
			switch (line_shape) {
			case(0):
				make_pen();
				break;
			case(1):
				make_dashpen();
				break;
			case(2):
				make_dotpen();
				break;
			}
			break;
		case ID_RED:
			pencolor = 1;
			p_color = 0x0000ff;
			switch (line_shape) {
			case(0):
				make_pen();
				break;
			case(1):
				make_dashpen();
				break;
			case(2):
				make_dotpen();
				break;
			}
			break;
		case ID_GREEN:
			pencolor = 2;
			p_color = 0x00ff00;
			switch (line_shape) {
			case(0):
				make_pen();
				break;
			case(1):
				make_dashpen();
				break;
			case(2):
				make_dotpen();
				break;
			}
			break;
		case ID_BLUE:
			pencolor = 3;
			p_color = 0xff0000;
			switch (line_shape) {
			case(0):
				make_pen();
				break;
			case(1):
				make_dashpen();
				break;
			case(2):
				make_dotpen();
				break;
			}
			break;
		case(ID_FILL_NON): // 채우기
			brus=0;
			brucolor = 0;
			hbrush = (HBRUSH)GetStockObject(NULL_BRUSH); 
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); 
			DeleteObject(old_hbrush);
			break;			
		case(ID_FILL_BLACK):
			brus=1;
			brucolor = 1;
			b_color = 0x000000;
			hbrush = CreateSolidBrush(0x000000); 
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush);
			DeleteObject(old_hbrush);
			break;			
		case(ID_FILL_WHITE):
			brus=1;
			brucolor = 2;
			b_color = 0xffffff;
			hbrush = CreateSolidBrush(0xffffff); 
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); 
			DeleteObject(old_hbrush);
			break;			
		case(ID_FILL_RED):
			brus=1;
			brucolor = 3;
			b_color = 0x0000ff;
			hbrush = CreateSolidBrush(0x0000ff); 
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); 
			DeleteObject(old_hbrush);
			break;			
		case(ID_FILL_GREEN):
			brus=1;
			brucolor = 4;
			b_color = 0x00ff00;
			hbrush = CreateSolidBrush(0x00ff00); 
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); 
			DeleteObject(old_hbrush);
			break;			
		case(ID_FILL_BLUE):
			brus=1;
			brucolor = 5;
			b_color = 0xff0000;
			hbrush = CreateSolidBrush(0xff0000); 
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); 
			DeleteObject(old_hbrush);
			break;
		case(ID_SOLIDLINE):
			line_shape=0;
			make_pen();
			break;
		case(ID_DOTTEDLINE):
			line_shape=2;
			make_dotpen();
			break;
		case(ID_DASHLINE):
			line_shape=1;
			make_dashpen();
			break;

		case(ID_FILEOPEN):
			//열기
			int widthstep;
			PatBlt(MemDC, 0, 0, rt.right, rt.bottom, WHITENESS);	
			InvalidateRect(hwnd, NULL, TRUE);

			if(GetOpenFileName(&OFN) != 0) 
			{

				if(control.count != 0){
					vi* temp;
					control.now_ptr = control.head_ptr;
					for(int i = 0; i < control.count; i++) {
						temp = control.now_ptr;
						control.now_ptr = control.now_ptr->ptr;
						delete(temp);
					}
					delete(control.now_ptr);
				}
				control.head_ptr = NULL;
				WORD checkbm;
				fp  = CreateFile(OFN.lpstrFile, GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				ReadFile(fp, &checkbm, 2, &readn, NULL);
				if(OFN.nFilterIndex == 1&& checkbm == 0x4d42){
					CloseHandle(fp);

					fp  = CreateFile(OFN.lpstrFile, GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

					if (fp != INVALID_HANDLE_VALUE) 
					{
						ReadFile(fp,&hf, sizeof(BITMAPFILEHEADER), &readn, NULL); 
						ReadFile(fp,&infohead, sizeof(BITMAPINFOHEADER), &readn, NULL); 

						xPos = 0;
						yPos = infohead.biHeight - 1;


						if (infohead.biBitCount == 1) // 단일 비트
						{
							RGBQUAD       pRGB[2];//팔레트
							ReadFile(fp, pRGB, sizeof(RGBQUAD) * 2, &readn, NULL);
							Data = new BYTE[infohead.biSizeImage];

							ReadFile(fp, Data, infohead.biSizeImage, &readn, NULL);

							widthstep = 32 - (infohead.biWidth % 32); 
							if(widthstep==32) 
								widthstep=0;

							BYTE half; 
							BYTE bit = 128;
							int count=0;

							for(int i=0 ; i < infohead.biSizeImage ; i++) 
							{
								for(int j=0 ; j<8 ; j++) 
								{
									half = Data[i] & bit; 
									half = half >> 7; 
									SetPixel(MemDC, xPos, yPos, RGB(pRGB[half].rgbRed, pRGB[half].rgbGreen, pRGB[half].rgbBlue)); 
									xPos++; // 다음 픽셀
									if(xPos >= infohead.biWidth) // 가로폭만큼 출력 완료시에
									{
										yPos --; // 다음줄로 이동
										xPos = 0; 
									}
									count++; 
									if(count == infohead.biWidth) 
									{
										count = 0; 
										i += (widthstep / 8); 
										break;
									}
									Data[i] = Data[i] << 1;
								}
							}
						}

						else if (infohead.biBitCount == 8) // 8비트
						{
							RGBQUAD       pRGB[256];//팔레트
							ReadFile(fp, pRGB, sizeof(RGBQUAD) * 256, &readn, NULL);
							Data = new BYTE[infohead.biSizeImage];

							ReadFile(fp, Data, infohead.biSizeImage, &readn, NULL);

							widthstep = 4 - (infohead.biWidth % 4); // widthstep의 개수
							if(widthstep==4) // 4일 경우 0
								widthstep=0;

							for(int i=0 ; i < infohead.biSizeImage ; i++) // 픽셀 바이트 만큼 반복
							{
								SetPixel(MemDC, xPos, yPos, RGB(pRGB[Data[i+2]].rgbRed, pRGB[Data[i+1]].rgbGreen, pRGB[Data[i]].rgbBlue)); // 색상 데이터 입력
								xPos++; // 다음 픽셀로 이동
								if(xPos >= infohead.biWidth) // 가로폭만큼 출력 완료시에
								{
									yPos --; // 다음줄로 이동
									xPos = 0; 
									i += widthstep;
								}
							}
						}

						else if (infohead.biBitCount == 24)
						{
							int count=0;

							widthstep = 4 - (infohead.biWidth * 3 % 4);
							if (widthstep == 4) 
								widthstep = 0;

							Data = new BYTE[infohead.biSizeImage];
							ReadFile(fp, Data, infohead.biSizeImage, &readn, NULL); 

							for (int i = 0; i < infohead.biWidth * infohead.biHeight; i++) 
							{
								if (i != 0 && i % infohead.biWidth == 0 ) // 줄 넘김
								{
									xPos = 0;
									yPos--;
									count++; 
								}
								SetPixel(MemDC, xPos, yPos, RGB(Data[(i*3)+(count*widthstep)+2], Data[(i*3)+(count*widthstep)+1], Data[(i*3)+(count*widthstep)]));

								xPos++; //다음
							}
						}
						InvalidateRect(hwnd, NULL, FALSE);
					}
				}
				else if(OFN.nFilterIndex == 2)// vi 파일 열기
				{
					CloseHandle(fp);
					int resize1 = 0; //
					int index1 = 0;
					int b_count = 0;
					TCHAR *str_read;
					fp  = CreateFile(OFN.lpstrFile, GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

					control.head_ptr = new vi; // 동적 할당
					control.count = 0; 
					control.now_ptr = control.head_ptr;

					str_read = (TCHAR*)malloc(20);
					ReadFile(fp, str_read, 20, &readn, NULL);
					str_read[readn/2] = NULL;
					if(str_read[index1] == 0xfeff){
							index1++;
					}
					control.count += (str_read[index1++]-48) * 10; //전체 개수 파악
					control.count += (str_read[index1++] - 48);
					index1++; //,
					resize1 += (str_read[index1++] - 48) * 100000;
					resize1 += (str_read[index1++] - 48) * 10000;
					resize1 += (str_read[index1++] - 48) * 1000;
					resize1 += (str_read[index1++] - 48) * 100;
					resize1 += (str_read[index1++] - 48) * 10;
					resize1 += (str_read[index1++] - 48);
					index1++; //\n
					if(str_read[0] !=65279){
						index1 = 0;
					}
					if(str_read[0] == 65279){
						index1 =1;
					}

					str_read = (TCHAR*)malloc(resize1*2);
					ReadFile(fp, str_read, resize1*2, &readn, NULL);
					str_read[readn/2] = NULL;

					for(int i=0; i<control.count;i++){
						control.now_ptr->fill_color = 0;
						control.now_ptr->flag_fill =0;
						control.now_ptr->vshape = 0;
						control.now_ptr->size = 0;
						control.now_ptr->line_color_count = 0;
						control.now_ptr->fill_color_count = 0;
						control.now_ptr->line_color = 0;
						control.now_ptr->fill_color = 0;
						control.now_ptr->lineshape = 0;
						control.now_ptr->vshape += (str_read[index1++] - 48) * 1000;
						control.now_ptr->vshape += (str_read[index1++] - 48) * 100;
						control.now_ptr->vshape += (str_read[index1++] - 48) * 10;
						control.now_ptr->vshape += (str_read[index1++] - 48);
						index1++;
						control.now_ptr->size += (str_read[index1++] - 48) * 1000;
						control.now_ptr->size += (str_read[index1++] - 48) * 100;
						control.now_ptr->size += (str_read[index1++] - 48) * 10;
						control.now_ptr->size += (str_read[index1++] - 48);
						index1++;
						control.now_ptr->line_color_count += (str_read[index1++] - 48) * 1000;
						control.now_ptr->line_color_count += (str_read[index1++] - 48) * 100;
						control.now_ptr->line_color_count += (str_read[index1++] - 48) * 10;
						control.now_ptr->line_color_count += (str_read[index1++] - 48);
						index1++;
						control.now_ptr->flag_fill += (str_read[index1++] - 48) * 1000;
						control.now_ptr->flag_fill += (str_read[index1++] - 48) * 100;
						control.now_ptr->flag_fill += (str_read[index1++] - 48) * 10;
						control.now_ptr->flag_fill += (str_read[index1++] - 48);
						index1++;
						control.now_ptr->fill_color_count += (str_read[index1++] - 48) * 1000;
						control.now_ptr->fill_color_count += (str_read[index1++] - 48) * 100;
						control.now_ptr->fill_color_count += (str_read[index1++] - 48) * 10;
						control.now_ptr->fill_color_count += (str_read[index1++] - 48);
						index1++;
						control.now_ptr->lineshape += (str_read[index1++] - 48) * 1000;
						control.now_ptr->lineshape += (str_read[index1++] - 48) * 100;
						control.now_ptr->lineshape += (str_read[index1++] - 48) * 10;
						control.now_ptr->lineshape += (str_read[index1++] - 48);
						index1++;
						control.now_ptr->old.x = 0;
						control.now_ptr->old.y = 0;
						control.now_ptr->now.x = 0;
						control.now_ptr->now.y = 0;
						control.now_ptr->old.x += (str_read[index1++] - 48) * 1000;
						control.now_ptr->old.x += (str_read[index1++] - 48) * 100;
						control.now_ptr->old.x += (str_read[index1++] - 48) * 10;
						control.now_ptr->old.x += (str_read[index1++] - 48);
						index1++;
						control.now_ptr->old.y += (str_read[index1++] - 48) * 1000;
						control.now_ptr->old.y += (str_read[index1++] - 48) * 100;
						control.now_ptr->old.y += (str_read[index1++] - 48) * 10;
						control.now_ptr->old.y += (str_read[index1++] - 48);
						index1++;
						control.now_ptr->now.x += (str_read[index1++] - 48) * 1000;
						control.now_ptr->now.x += (str_read[index1++] - 48) * 100;
						control.now_ptr->now.x += (str_read[index1++] - 48) * 10;
						control.now_ptr->now.x += (str_read[index1++] - 48);
						index1++;
						control.now_ptr->now.y += (str_read[index1++] - 48) * 1000;
						control.now_ptr->now.y += (str_read[index1++] - 48) * 100;
						control.now_ptr->now.y += (str_read[index1++] - 48) * 10;
						control.now_ptr->now.y += (str_read[index1++] - 48);
						index1++;
						control.now_ptr->ptr = new vi;
						control.now_ptr = control.now_ptr->ptr;
					}
					control.now_ptr = control.head_ptr;
					b_count = control.count;
					while(b_count>0){
						switch(control.now_ptr->line_color_count){
						case 0:
							p_color = 0x000000;
							control.now_ptr->line_color = p_color;
							break;
						case 1:
							p_color = 0x0000ff;
							control.now_ptr->line_color = p_color;
							break;
						case 2:
							p_color = 0x00ff00;
							control.now_ptr->line_color = p_color;
							break;
						case 3:
							p_color = 0xff0000;
							control.now_ptr->line_color = p_color;
							break;
						}
						switch(control.now_ptr->fill_color_count){
						case 0:
							break;
						case 1:
							b_color = 0x000000;
							control.now_ptr->fill_color = b_color;
							break;
						case 2:
							b_color = 0xffffff;
							control.now_ptr->fill_color = b_color;
							break;
						case 3:
							b_color = 0x0000ff;
							control.now_ptr->fill_color = b_color;
							break;
						case 4:
							b_color = 0x00ff00;
							control.now_ptr->fill_color = b_color;
							break;
						case 5:
							b_color = 0xff0000;
							control.now_ptr->fill_color = b_color;
							break;
						}
						switch(control.now_ptr->lineshape){
						case(0):
							hpen = CreatePen(PS_SOLID, control.now_ptr->size, p_color);
							old_hpen = (HPEN)SelectObject(MemDC, hpen);
							DeleteObject(old_hpen);
							break;
						case(1):
							hpen = CreatePen(PS_DASH, control.now_ptr->size, p_color);
							old_hpen = (HPEN)SelectObject(MemDC, hpen);
							DeleteObject(old_hpen);
							break;
						case(2):
							hpen = CreatePen(PS_DOT, control.now_ptr->size, p_color);
							old_hpen = (HPEN)SelectObject(MemDC, hpen);
							DeleteObject(old_hpen);
							break;
						}
						if(control.now_ptr -> vshape == 1){ // line
							size = control.now_ptr->size;
							MoveToEx(MemDC, control.now_ptr->old.x, control.now_ptr->old.y, NULL);
							LineTo(MemDC, control.now_ptr->now.x, control.now_ptr->now.y);
						}
						else if(control.now_ptr -> vshape == 2){ // 원
							if(control.now_ptr->flag_fill == 1) {
								hbrush = CreateSolidBrush(control.now_ptr->fill_color);
								old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
								DeleteObject(old_hbrush);
							}
							else{ //브러시 0
								hbrush = (HBRUSH)GetStockObject(NULL_BRUSH); // brush를 사용하지 않음
								old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
								DeleteObject(old_hbrush);
							}
							size = control.now_ptr->size;

							MoveToEx(MemDC, control.now_ptr->old.x,control.now_ptr->old.y, NULL);
							Ellipse(MemDC, control.now_ptr->old.x, control.now_ptr->old.y, control.now_ptr->now.x, control.now_ptr->now.y);
						}
						else if(control.now_ptr -> vshape == 3) {// 삼각형
							if(control.now_ptr->flag_fill == 1) {
								hbrush = CreateSolidBrush(control.now_ptr->fill_color);
								old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
								DeleteObject(old_hbrush);
							}
							else{ //브러시 0
								hbrush = (HBRUSH)GetStockObject(NULL_BRUSH); // brush를 사용하지 않음
								old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
								DeleteObject(old_hbrush);
							}
							size = control.now_ptr->size;
							POINT gag[3];
							gag[0].x=(control.now_ptr->old.x + control.now_ptr->now.x)/2;
							gag[0].y=control.now_ptr->old.y;
							gag[1].x=control.now_ptr->now.x;
							gag[1].y=control.now_ptr->now.y;
							gag[2].x=control.now_ptr->old.x;
							gag[2].y=control.now_ptr->now.y;
							Polygon(MemDC, gag, 3);
						}
						else if(control.now_ptr -> vshape == 4) { // 사각형
							if(control.now_ptr->flag_fill == 1) {
								hbrush = CreateSolidBrush(control.now_ptr->fill_color);
								old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
								DeleteObject(old_hbrush);
							}
							else{ //브러시 0
								hbrush = (HBRUSH)GetStockObject(NULL_BRUSH); // brush를 사용하지 않음
								old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
								DeleteObject(old_hbrush);
							}
							size = control.now_ptr->size;
							MoveToEx(MemDC, control.now_ptr->old.x, control.now_ptr->old.y, NULL);
							Rectangle(MemDC, control.now_ptr->old.x, control.now_ptr->old.y, control.now_ptr->now.x, control.now_ptr->now.y);
						}
						else if(control.now_ptr -> vshape == 0){
							size = control.now_ptr->size;
							MoveToEx(MemDC, control.now_ptr->old.x, control.now_ptr->old.y, NULL);
							LineTo(MemDC, control.now_ptr->now.x, control.now_ptr->now.y);
						}
						control.now_ptr = control.now_ptr->ptr;
						b_count--;
					}
					control.now_ptr -> ptr = NULL;
				}
				else if(OFN.nFilterIndex == 3){ //raw파일 열기
					CloseHandle(fp);


					fp  = CreateFile(OFN.lpstrFile, GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

					if (fp != INVALID_HANDLE_VALUE) 
					{
						ReadFile(fp,&rw, sizeof(rw), &readn, NULL); //8바이트

						xPos = 0;
						yPos =0;


						int count=0;

						widthstep = 4 - (rw.biWidth * 3 % 4);
						if (widthstep == 4) 
							widthstep = 0;

						Data = new BYTE[(rw.biHeight*rw.biWidth)*3];
						ReadFile(fp, Data, (rw.biHeight*rw.biWidth)*3, &readn, NULL); 

						for (int i = 0; i < rw.biWidth * rw.biHeight; i++) 
						{
							if (i != 0 && i % rw.biWidth == 0 ) 
							{
								xPos = 0;
								yPos++;
								count++; 
							}
							SetPixel(MemDC, xPos, yPos, RGB(Data[(i*3)+(count*widthstep)+2], Data[(i*3)+(count*widthstep)+1], Data[(i*3)+(count*widthstep)]));

							xPos++; // 다음
						}
					}
					InvalidateRect(hwnd, NULL, FALSE);
				}
			}
			else
			{
				MessageBox(hwnd, TEXT("비트맵 파일을 열지 못했습니다."), 0, MB_OK);
			}
			CloseHandle(fp);
			BitBlt(hdc, 0, 0, rt.right, rt.bottom, MemDC, 0, 0, SRCCOPY);
			BitBlt(backMemDC, 0, 0, rt.right, rt.bottom, hdc, 0, 0, SRCCOPY);
			break;

		case ID_FILESAVE:
			{
				memset(&SFN, 0, sizeof(OPENFILENAME));
				SFN.lStructSize = sizeof(OPENFILENAME);
				SFN.hwndOwner = hwnd;
				SFN.lpstrFilter = TEXT("24비트 비트맵(*.bmp)\0*.bmp\08비트 비트맵(*.bmp)\0*.bmp\0단색 비트맵 파일(*.bmp)\0*.bmp\0VI 파일(*.vi)\0*.vi\0raw파일(*.raw)\0*.raw\0모든 파일(*.*)\0*.*\0");
				SFN.lpstrDefExt = TEXT("txt");
				SFN.lpstrFile = lpstrFile2;
				SFN.nMaxFile = 100;
				int b, g, r; // 색상

				if (GetSaveFileName(&SFN) != 0) // 다이얼로그 열기
				{
					int size, r, g, b;
					DWORD Size;
					TCHAR lpstrFile[100]; // 파일이름 저장위함
					HANDLE fp;
					DWORD readn;
					BITMAPINFO* pih;

					GetObject(hBit, sizeof(BITMAP), &map); // 파일 헤더 복사

					hf.bfType = 0x4d42;// 파일의 형태를 지정하는 매직 넘버이며 반드기 BM(bmp 파일의 전체 크기)이어야 한다(0x42, 0x4d)
					hf.bfReserved1 = 0;// 예비
					hf.bfReserved2 = 0;// 예비

					infohead.biSize = sizeof(BITMAPINFOHEADER);				// 구조체의 크기
					infohead.biWidth = map.bmWidth;							// 비트맵 가로 픽셀수
					infohead.biHeight = map.bmHeight;						// 비트맵 세로 픽셀수
					infohead.biPlanes = 1;									// 플래인 개수를 나타내는데 반드시 1로 고정되어있어야한다
					infohead.biCompression = BI_RGB;						// 압축방식->압축되지 않은 비트맵 (BI_RGB)
					infohead.biXPelsPerMeter = 0;							// 미터당 가로픽셀수
					infohead.biYPelsPerMeter = 0;							// 미터당 세로픽셀수
					infohead.biClrUsed = 0;									// 색상 테이블중 비트맵에서 사용되는 색상 수. 
					infohead.biClrImportant = 0;							// 비트맵을 출력하는데 필수적인 색상 수

					if (SFN.nFilterIndex == 1) { // 24비트 일 경우

						widthstep = 4 - (infohead.biWidth * 3 % 4);
						if (widthstep == 4)
							widthstep = 0;

						hf.bfSize = (widthstep * infohead.biHeight) + (infohead.biWidth * infohead.biHeight * 3) + 54;		// 비트맵 파일 전체의 크기
						hf.bfOffBits = 54;																					// 픽셀 데이터의 시작 주소
						infohead.biBitCount = 24;																			// 한 픽셀당 몇 개의 비트로 이루어지는가
						infohead.biSizeImage = hf.bfSize - 54;
						// 헤더는 이제 완성
						fp = CreateFile(lpstrFile2, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // 파일 생성

						WriteFile(fp, &hf, sizeof(hf), &readn, NULL);// 헤더 작성
						WriteFile(fp, &infohead, sizeof(infohead), &readn, NULL); // 파일헤더 작성
						COLORREF color;
						for (int y = rt.bottom-1; y >= 0; y--)
						{
							for (int k = 0; k <rt.right; k++)
							{
								color = GetPixel(backMemDC, k, y);
								b = color & 255;
								color = color >> 8;
								g = color & 255;
								color = color >> 8;
								r = color & 255;
								WriteFile(fp, (char*)&r, 1, &color, NULL);
								WriteFile(fp, (char*)&g, 1, &color, NULL);
								WriteFile(fp, (char*)&b, 1, &color, NULL);
							}
							for (int temp = widthstep; temp > 0; x--)
							{
								int step = 0;
								WriteFile(fp, (char*)&step, 1, &color, NULL);
							}
						}
						CloseHandle(fp);
					}
					else if (SFN.nFilterIndex == 2) { // 8비트(256색) 일 경우

						widthstep = 4 - (infohead.biWidth % 4);
						if (widthstep == 4)
							widthstep = 0;

						hf.bfSize = (widthstep * infohead.biHeight) + (infohead.biWidth * infohead.biHeight) + 1078;	// 비트맵 파일 전체의 크기
						hf.bfOffBits = 1078;																			// 픽셀 데이터의 시작 주소
						infohead.biBitCount = 8;																		
						infohead.biSizeImage = hf.bfSize - 1078;
						int temp_pal[1024] = { 0, 0, 0, 0, 0, 0, 128, 0, 0, 128, 0, 0, 0, 128, 128, 0, 128, 0, 0, 0, 128, 0, 128, 0, 128, 128, 0, 0, 192, 192, 192, 0, 192, 220, 192, 0, 240, 202, 166, 0, 0, 32, 64, 0, 0, 32, 96, 0, 0, 32, 128, 0, 0, 32, 160, 0, 0, 32, 192, 0, 0, 32, 224, 0, 0, 64, 0, 0, 0, 64, 32, 0, 0, 64, 64, 0, 0, 64, 96, 0, 0, 64, 128, 0, 0, 64, 160, 0, 0, 64, 192, 0, 0, 64, 224, 0, 0, 96, 0, 0, 0, 96, 32, 0, 0, 96, 64, 0, 0, 96, 96, 0, 0, 96, 128, 0, 0, 96, 160, 0, 0, 96, 192, 0, 0, 96, 224, 0, 0, 128, 0, 0, 0, 128, 32, 0, 0, 128, 64, 0, 0, 128, 96, 0, 0, 128, 128, 0, 0, 128, 160, 0, 0, 128, 192, 0, 0, 128, 224, 0, 0, 160, 0, 0, 0, 160, 32, 0, 0, 160, 64, 0, 0, 160, 96, 0, 0, 160, 128, 0, 0, 160, 160, 0, 0, 160, 192, 0, 0, 160, 224, 0, 0, 192, 0, 0, 0, 192, 32, 0, 0, 192, 64, 0, 0, 192, 96, 0, 0, 192, 128, 0, 0, 192, 160, 0, 0, 192, 192, 0, 0, 192, 224, 0, 0, 224, 0, 0, 0, 224, 32, 0, 0, 224, 64, 0, 0, 224, 96, 0, 0, 224, 128, 0, 0, 224, 160, 0, 0, 224, 192, 0, 0, 224, 224, 0, 64, 0, 0, 0, 64, 0, 32, 0, 64, 0, 64, 0, 64, 0, 96, 0, 64, 0, 128, 0, 64, 0, 160, 0, 64, 0, 192, 0, 64, 0, 224, 0, 64, 32, 0, 0, 64, 32, 32, 0, 64, 32, 64, 0, 64, 32,96, 0, 64, 32, 128, 0, 64, 32, 160, 0, 64, 32, 192, 0, 64, 32, 224, 0, 64, 64, 0, 0, 64, 64, 32, 0, 64, 64, 64, 0, 64, 64, 96, 0, 64, 64, 128, 0, 64, 64, 160, 0, 64, 64, 192, 0, 64, 64, 224, 0, 64, 96, 0, 0, 64, 96, 32, 0, 64, 96, 64, 0, 64, 96, 96, 0, 64, 96, 128, 0, 64, 96, 160, 0, 64, 96, 192, 0, 64, 96, 224, 0, 64, 128, 0, 0, 64, 128, 32, 0, 64, 128, 64, 0, 64, 128, 96, 0, 64, 128, 128, 0, 64, 128, 160, 0, 64, 128, 192, 0, 64, 128, 224, 0, 64, 160, 0, 0, 64, 160, 32, 0, 64, 160, 64, 0, 64, 160, 96, 0, 64, 160, 128, 0, 64, 160, 160, 0, 64, 160, 192, 0, 64, 160, 224, 0, 64, 192, 0, 0, 64, 192, 32, 0, 64, 192, 64, 0, 64, 192, 96, 0, 64, 192, 128, 0, 64, 192, 160, 0, 64, 192, 192, 0, 64, 192, 224, 0, 64, 224, 0, 0, 64, 224, 32, 0, 64, 224, 64, 0, 64, 224, 96, 0, 64, 224, 128, 0, 64, 224, 160, 0, 64, 224, 192, 0, 64, 224, 224, 0, 128, 0, 0, 0, 128, 0, 32, 0, 128, 0, 64, 0, 128, 0, 96, 0, 128, 0, 128, 0, 128, 0, 160, 0, 128, 0, 192, 0, 128, 0, 224, 0, 128, 32, 0, 0, 128, 32, 32, 0, 128, 32, 64, 0, 128, 32, 96, 0, 128, 32, 128, 0, 128, 32, 160, 0, 128, 32, 192, 0, 128, 32, 224, 0, 128, 64, 0, 0, 128, 64, 32, 0, 128, 64, 64, 0, 128, 64, 96, 0, 128, 64, 128, 0, 128, 64, 160, 0, 128, 64, 192, 0, 128, 64, 224, 0, 128, 96, 0, 0, 128, 96, 32, 0, 128, 96, 64, 0, 128, 96, 96, 0, 128, 96, 128, 0, 128, 96, 160, 0, 128, 96, 192, 0, 128, 96, 224, 0, 128, 128, 0, 0, 128, 128, 32, 0, 128, 128, 64, 0, 128, 128, 96, 0, 128, 128, 128, 0, 128, 128, 160, 0, 128, 128, 192, 0, 128, 128, 224, 0, 128, 160, 0, 0, 128, 160, 32, 0, 128, 160, 64, 0, 128, 160, 96, 0, 128, 160, 128, 0, 128, 160, 160, 0, 128, 160, 192, 0, 128, 160, 224, 0, 128, 192, 0, 0, 128, 192, 32, 0, 128, 192, 64, 0, 128, 192, 96, 0, 128, 192, 128, 0, 128, 192, 160, 0, 128, 192, 192, 0, 128, 192, 224, 0, 128, 224, 0, 0, 128, 224, 32, 0, 128, 224, 64, 0, 128, 224, 96, 0, 128, 224, 128, 0, 128, 224, 160, 0, 128, 224, 192, 0, 128, 224, 224, 0, 192, 0, 0, 0, 192, 0, 32, 0, 192, 0, 64, 0, 192, 0, 96, 0, 192, 0, 128, 0, 192, 0, 160, 0, 192, 0, 192, 0, 192, 0, 224, 0, 192, 32, 0, 0, 192, 32, 32, 0, 192, 32, 64, 0, 192, 32, 96, 0, 192, 32, 128, 0, 192, 32, 160, 0, 192, 32, 192, 0, 192, 32, 224, 0, 192, 64, 0, 0, 192, 64, 32, 0, 192, 64, 64, 0, 192, 64, 96, 0, 192, 64, 128, 0, 192, 64, 160, 0, 192, 64, 192, 0, 192, 64, 224, 0, 192, 96, 0, 0, 192, 96, 32, 0, 192, 96, 64, 0, 192, 96, 96, 0, 192, 96, 128, 0, 192, 96, 160, 0, 192, 96, 192, 0, 192, 96, 224, 0, 192, 128, 0, 0, 192, 128, 32, 0, 192, 128, 64, 0, 192, 128, 96, 0, 192, 128, 128, 0,  192, 128, 160, 0, 192, 128, 192, 0, 192, 128, 224, 0, 192, 160, 0, 0, 192, 160, 32, 0, 192, 160, 64, 0, 192, 160, 96, 0, 192, 160, 128, 0, 192, 160, 160, 0, 192, 160, 192, 0, 192, 160, 224, 0, 192, 192, 0, 0, 192, 192, 32, 0, 192, 192, 64, 0, 192, 192, 96, 0, 192, 192, 128, 0, 192, 192, 160, 0, 240, 251, 255, 0, 164, 160, 160, 0, 128, 128, 128, 0, 0, 0, 255, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 0, 0, 0, 255, 0, 255, 0, 255, 255, 0, 0, 255, 255, 255, 0 };

						COLORREF color;
						RGBQUAD pal[256];
						int k = 0, tmp = 0, count = 0, cur = 0, x, y;
						int arr[3];
						int min =128;
						byte num;

						fp = CreateFile(lpstrFile2, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

						WriteFile(fp, &hf, sizeof(hf), &readn, NULL);// 헤더
						WriteFile(fp, &infohead, sizeof(infohead), &readn, NULL); // 파일헤더

						for (int i = 0; i < 256; i++)
						{
							pal[i].rgbBlue = temp_pal[k];
							k++;
							pal[i].rgbGreen = temp_pal[k];
							k++;
							pal[i].rgbRed = temp_pal[k];
							k++;
							pal[i].rgbReserved = temp_pal[k];
							k++;
						}
						WriteFile(fp, (char*)&pal, sizeof(RGBQUAD) * 256, &readn, NULL); // 팔레트
						for (y = rt.bottom - 1; y >= 0; y--)
						{
							for (x = 0; x < rt.right; x++)
							{
								count++;
								color = GetPixel(backMemDC, x, y);
								b = color & 255;
								color = color >> 8;
								g = color & 255;
								color = color >> 8;
								r = color & 255;

								for (num = 0; num < 256; num++) { // 팔레트에 있는 색이면 삽입
									tmp = 0;
									if (pal[num].rgbBlue == r && pal[num].rgbGreen == g && pal[num].rgbRed == b) {
										WriteFile(fp, (char*)&num, 1, &readn, NULL);
										tmp = 1;
										break;
									}
									/*else if(pal[num].rgbBlue == b && pal[num].rgbGreen == g || pal[num].rgbBlue ==b && pal[num].rgbRed == r || pal[num].rgbGreen ==g && pal[num].rgbRed == r){
									if((pal[num].rgbBlue == b && pal[num].rgbGreen == g)){
									if(0 <= r <= 64){
									arr[0] = abs(r-0);
									arr[1] = abs(r-32);
									arr[2] = abs(r-64);
									for(int i=0; i<3; i++){
									if(arr[i] < 64)
									min = arr[i];
									}
									}
									else if(64< r <= 192){
									arr[0] = abs(r-128);
									arr[1] = abs(r-160);
									arr[2] = abs(r-192);
									for(int i=0; i<3; i++){
									if(arr[i] < 192)
									min = arr[i];
									}
									}
									else if(192< r <= 256){
									arr[0] = abs(r-192);
									arr[1] = abs(r-224);
									arr[2] = abs(r- 256);
									for(int i=0; i<3; i++){
									if(arr[i] < 256)
									min = arr[i];
									}
									}
									r = min;
									}
									WriteFile(fp, (char*)&num, 1, &readn, NULL);
									}
									break;*/
								}

								if (count == rt.right) {
									for (int temp = 0; temp < widthstep / 2; temp++) {
										WriteFile(fp, (char*)cur, 1, &readn, NULL);
									}
									count = 0;
								}
							}
							x = 0;
						}
						CloseHandle(fp);
					}
					else if(SFN.nFilterIndex == 3) { //단일 비트

						int k=0;
						int count = 0, cur = 0;
						int tmp = 0;
						byte num;
						int PalSize;
						DWORD Size;
						RGBQUAD pal[2];
						COLORREF color;
						int color_num;
						BYTE *real_data;				//오로지 그림 데이터만 저장되는 공간

						color_num = 2;

						widthstep = 32 - (infohead.biWidth%32);			
						if(widthstep == 32)
							widthstep = 0;


						infohead.biBitCount = 1;																		
						infohead.biSizeImage = 	infohead.biHeight*4*((infohead.biWidth-1)/32 +1);
						hf.bfOffBits = 62;
						hf.bfSize =hf.bfOffBits + infohead.biSizeImage; 	//비트맵 정보해더

						
						memset(&pal[1],255,sizeof(BYTE)*3);
						real_data = new BYTE[infohead.biSizeImage];

						memset(real_data , 0,infohead.biSizeImage);

						fp = CreateFile(lpstrFile2, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // write = 쓰기전용, always는 파일이 존재할 경우 덮어쓰기, normal 속성지정x

						WriteFile(fp, &hf, sizeof(hf), &readn, NULL);//헤더
						WriteFile(fp, &infohead, sizeof(infohead), &readn, NULL); // 파일헤더

						pal[0].rgbBlue = 0;
						pal[0].rgbGreen = 0;
						pal[0].rgbRed = 0;
						pal[0].rgbReserved = 0;
						pal[1].rgbBlue = 255;
						pal[1].rgbGreen = 255;
						pal[1].rgbRed = 255;
						pal[1].rgbReserved = 0;
						WriteFile(fp, (char*)&pal, sizeof(RGBQUAD) * 2, &readn, NULL); // 팔레트

						for (y = rt.bottom - 1; y >= 0; y--)
						{
							for (x = 0; x < rt.right; x++)
							{
								count++;
								color = GetPixel(backMemDC, x, y);
								if (color == 16777215) {
									real_data[k] = real_data[k] <<1;
									real_data[k] += 1;
								}
								else
									real_data[k] = real_data[k] << 1;
							}
							if(x%8 == 7){
								k++;
							}
							if(x%8 != 0){
								real_data[k] = real_data[k] << (8 -(infohead.biWidth%8));
								k++;
							}
							k += widthstep/8;
						}
						WriteFile(fp, real_data, k, &readn, NULL);
						CloseHandle(fp);
					}

					//TCHAR vi_1[100][10]; // 1:크기, 2: old.x, 3: old.y, 4:now.x, 5: now.y 6: 굵기, 7. 선색상, 8: 채우기 여부, 9: 채우기 색상, 10: 구조체 개수
					else if (SFN.nFilterIndex == 4) { // vi로 저장
						int index = 0;
						int resize = 0;
						TCHAR* str = NULL;
						control.now_ptr = control.head_ptr;
						fp = CreateFile(lpstrFile2, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // 파일 생성
						for (int i = 0; i < control.count; i++) { 
							vi_1[i][0] = control.now_ptr->vshape;
							vi_1[i][1] = control.now_ptr->old.x;
							vi_1[i][2] = control.now_ptr->old.y;
							vi_1[i][3] = control.now_ptr->now.x;
							vi_1[i][4] = control.now_ptr->now.y;
							vi_1[i][5] = control.now_ptr->size;
							vi_1[i][6] = control.now_ptr->line_color_count;
							vi_1[i][7] = control.now_ptr->flag_fill;
							vi_1[i][8] = control.now_ptr->fill_color_count;
							vi_1[0][9] = control.count;
							vi_1[i][10] = control.now_ptr->lineshape;
							control.now_ptr = control.now_ptr->ptr;
						}
						for (int i = 0; i < control.count; i++) {
							if (vi_1[i][0] != 0)

								resize += 14*6+1;
							else
								resize += (14 * 5) + (vi_1[i][9] * 10) + 2;
						}

						str = (TCHAR*)malloc(resize*2);
						str[index++] = (vi_1[0][9] / 10) + 48;
						str[index++] = (vi_1[0][9] % 10) + 48;
						str[index++] = ',';
						str[index++] = (resize / 100000) % 100000 + 48;
						str[index++] = (resize / 10000) % 10000 + 48;
						str[index++] = (resize / 1000) % 1000 + 48;
						str[index++] = (resize / 100) % 100 + 48;
						str[index++] = (resize / 10) % 10 + 48;
						str[index++] = (resize) % 10 + 48;
						str[index++] = '\n';
						control.now_ptr = control.head_ptr;
						for (int j = 0; j < control.count; j++) {
							str[index++] = (vi_1[j][0] / 1000) + 48;
							str[index++] = (vi_1[j][0] / 100) % 10 + 48;
							str[index++] = (vi_1[j][0] / 10) % 10 + 48;
							str[index++] = (vi_1[j][0] % 10) + 48;
							str[index++] = ',';
							str[index++] = (vi_1[j][5] / 1000) + 48;
							str[index++] = (vi_1[j][5] / 100) % 10 + 48;
							str[index++] = (vi_1[j][5] / 10) % 10 + 48;
							str[index++] = (vi_1[j][5] % 10) + 48;
							str[index++] = ',';
							str[index++] = (vi_1[j][6] / 1000) + 48;
							str[index++] = (vi_1[j][6] / 100) % 10 + 48;
							str[index++] = (vi_1[j][6] / 10) % 10 + 48;
							str[index++] = (vi_1[j][6] % 10) + 48;
							str[index++] = ',';
							str[index++] = (vi_1[j][7] / 1000) + 48;
							str[index++] = (vi_1[j][7] / 100) % 10 + 48;
							str[index++] = (vi_1[j][7] / 10) % 10 + 48;
							str[index++] = (vi_1[j][7] % 10) + 48;
							str[index++] = ',';
							str[index++] = (vi_1[j][8] / 1000) + 48;
							str[index++] = (vi_1[j][8] / 100) % 10 + 48;
							str[index++] = (vi_1[j][8] / 10) % 10 + 48;
							str[index++] = (vi_1[j][8] % 10) + 48;
							str[index++] = ',';
							str[index++] = (vi_1[j][10] / 1000) + 48;
							str[index++] = (vi_1[j][10] / 100) % 10 + 48;
							str[index++] = (vi_1[j][10] / 10) % 10 + 48;
							str[index++] = (vi_1[j][10] % 10) + 48;
							str[index++] = '\n';
							str[index++] = (vi_1[j][1] / 1000) + 48;
							str[index++] = (vi_1[j][1] / 100) % 10 + 48;
							str[index++] = (vi_1[j][1] / 10) % 10 + 48;
							str[index++] = (vi_1[j][1] % 10) + 48;
							str[index++] = ',';
							str[index++] = (vi_1[j][2] / 1000) + 48;
							str[index++] = (vi_1[j][2] / 100) % 10 + 48;
							str[index++] = (vi_1[j][2] / 10) % 10 + 48;
							str[index++] = (vi_1[j][2] % 10) + 48;
							str[index++] = ',';
							str[index++] = (vi_1[j][3] / 1000) + 48;
							str[index++] = (vi_1[j][3] / 100) % 10 + 48;
							str[index++] = (vi_1[j][3] / 10) % 10 + 48;
							str[index++] = (vi_1[j][3] % 10) + 48;
							str[index++] = ',';
							str[index++] = (vi_1[j][4] / 1000) + 48;
							str[index++] = (vi_1[j][4] / 100) % 10 + 48;
							str[index++] = (vi_1[j][4] / 10) % 10 + 48;
							str[index++] = (vi_1[j][4] % 10) + 48;
							str[index++] = '\n';
						}
						control.now_ptr = control.head_ptr;
						WriteFile(fp, str, index * 2, &readn, NULL);
						CloseHandle(fp);
					}
					else if(SFN.nFilterIndex == 5){ // raw파일 저장
						rw.biHeight = rt.bottom;
						rw.biWidth = rt.right;
						fp = CreateFile(lpstrFile2, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

						widthstep = 4 - (rw.biWidth * 3 % 4);
						if (widthstep == 4)
							widthstep = 0;

						WriteFile(fp, &rw, sizeof(rw), &readn, NULL);// 헤더(너비 높이) 작성
						COLORREF color;
						for (int i =1; i < rt.bottom+1; i++)
						{
							for (int k = 1; k < rt.right+1; k++)
							{
								color = GetPixel(backMemDC, k, i); //픽셀 데이터 읽어오기
								b = color & 255;
								color = color >> 8;
								g = color & 255;
								color = color >> 8;
								r = color & 255;
								WriteFile(fp, (char*)&r, 1, &color, NULL);
								WriteFile(fp, (char*)&g, 1, &color, NULL);
								WriteFile(fp, (char*)&b, 1, &color, NULL);
							}
							for (int temp = widthstep; temp > 0; x--)
							{
								int step = 0;
								WriteFile(fp, (char*)&step, 1, &color, NULL);
							}
						}
						CloseHandle(fp);
					}

				}
			}

			break;

		case ID_NEWFILE:
			PatBlt(MemDC, 0, 0, rt.right, rt.bottom, WHITENESS);
			PatBlt(backMemDC, 0, 0, rt.right, rt.bottom, WHITENESS);
			new_flag = 1;
			InvalidateRect(hwnd, NULL, TRUE);
			filenew();
			break;

		case ID_FILEEXIT:
			DestroyWindow(hwnd);
			break;
		}
		break;
	case WM_LBUTTONDOWN:

		draw = TRUE;
		x = (int)LOWORD(lParam);
		y = (int)HIWORD(lParam);
		old_x = x;
		old_y = y;
		if(shape ==0){
			if (control.head_ptr == NULL) {
				control.head_ptr = new vi;
				control.now_ptr = control.head_ptr;

				control.now_ptr->lineshape = line_shape;
				control.now_ptr->vshape = 0;
				control.now_ptr->flag_fill = 0;
				control.now_ptr->fill_color = 0;
				control.now_ptr->line_color = p_color;
				control.now_ptr->line_color_count = pencolor;
				control.now_ptr->fill_color_count = brucolor;
				control.now_ptr->now.x = x;
				control.now_ptr->now.y = y;
				control.now_ptr->old.x = x;
				control.now_ptr->old.y = y;
				control.now_ptr->size = size;
				control.count++;
				control.now_ptr->ptr = new vi;
				control.now_ptr = control.now_ptr->ptr;
				control.now_ptr->ptr = NULL;
			}
		}
		break;

	case WM_LBUTTONUP:
		draw = FALSE;
		if (control.head_ptr == NULL) {
			control.head_ptr = new vi;
			control.now_ptr = control.head_ptr;
		}

		switch (shape) {

		case 1: //직선
			control.now_ptr->vshape = 1;
			control.now_ptr->flag_fill = 0;
			control.now_ptr->fill_color = 0;
			control.now_ptr->line_color = p_color;
			control.now_ptr->line_color_count = pencolor;
			control.now_ptr->fill_color_count = brucolor;
			control.now_ptr->lineshape = line_shape;
			control.now_ptr->now.x = x;
			control.now_ptr->now.y = y;
			control.now_ptr->old.x = old_x;
			control.now_ptr->old.y = old_y;
			control.now_ptr->size = size;
			control.count++;
			break;
		case 2: // 원
			control.now_ptr->vshape = 2;
			control.now_ptr->flag_fill = brus;
			control.now_ptr->fill_color = b_color;
			control.now_ptr->line_color = p_color;
			control.now_ptr->line_color_count = pencolor;
			control.now_ptr->fill_color_count = brucolor;
			control.now_ptr->lineshape = line_shape;
			control.now_ptr->now.x = x;
			control.now_ptr->now.y = y;
			control.now_ptr->old.x = old_x;
			control.now_ptr->old.y = old_y;
			control.now_ptr->size = size;
			control.count++;
			break;
		case 3: // 삼각형
			control.now_ptr->vshape = 3;
			control.now_ptr->flag_fill = brus;
			control.now_ptr->fill_color = b_color;
			control.now_ptr->line_color = p_color;
			control.now_ptr->line_color_count = pencolor;
			control.now_ptr->fill_color_count = brucolor;
			control.now_ptr->lineshape = line_shape;
			control.now_ptr->now.x = x;
			control.now_ptr->now.y = y;
			control.now_ptr->old.x = old_x;
			control.now_ptr->old.y = old_y;
			control.now_ptr->size = size;
			control.count++;
			break;
		case 4: // 사각형
			control.now_ptr->vshape = 4;
			control.now_ptr->flag_fill = brus;
			control.now_ptr->fill_color = b_color;
			control.now_ptr->line_color = p_color;
			control.now_ptr->line_color_count = pencolor;
			control.now_ptr->fill_color_count = brucolor;
			control.now_ptr->lineshape = line_shape;
			control.now_ptr->now.x = x;
			control.now_ptr->now.y = y;
			control.now_ptr->old.x = old_x;
			control.now_ptr->old.y = old_y;
			control.now_ptr->size = size;
			control.count++;
			break;
		}
		if (shape != 0) {
			BitBlt(backMemDC, 0, 0, rt.right, rt.bottom, hdc, 0, 0, SRCCOPY);
			control.now_ptr->ptr = new vi;
			control.now_ptr = control.now_ptr->ptr;
			control.now_ptr->ptr = NULL;
		}
		break;

	case WM_MOUSEMOVE:
		if (shape != 0)
			BitBlt(MemDC, 0, 0, rt.right, rt.bottom, backMemDC, 0, 0, SRCCOPY);

		if (draw && shape == 0) {  // 자유선
			MoveToEx(MemDC, x, y, NULL);
			control.now_ptr->old.x = x;
			control.now_ptr->old.y = y;
			x = (int)LOWORD(lParam);
			y = (int)HIWORD(lParam);
			LineTo(MemDC, x, y);
			control.now_ptr->vshape = 0;
			control.now_ptr->flag_fill = 0;
			control.now_ptr->fill_color = 0;
			control.now_ptr->line_color = p_color;
			control.now_ptr->now.x = x;
			control.now_ptr->now.y = y;
			control.now_ptr->line_color_count = pencolor;
			control.now_ptr->fill_color_count = brucolor;
			control.now_ptr->lineshape = line_shape;
			control.now_ptr->size = size;
			control.count++;
			control.now_ptr->ptr = new vi;
			control.now_ptr = control.now_ptr->ptr;
			control.now_ptr->ptr = NULL;
		} 
		else if (draw && shape == 1) { // 직선
			MoveToEx(MemDC, old_x, old_y, NULL);
			x = (int)LOWORD(lParam);
			y = (int)HIWORD(lParam);
			LineTo(MemDC, x, y);
		}
		else if (draw && shape == 2) { // 원
			x = (int)LOWORD(lParam);
			y = (int)HIWORD(lParam);
			MoveToEx(MemDC, old_x, old_y, NULL);
			Ellipse(MemDC, old_x, old_y, x, y);
		}
		else if (draw && shape == 3) { // 삼각형
			POINT gag[3];
			x = (int)LOWORD(lParam);
			y = (int)HIWORD(lParam);
			DeleteObject(old_hbrush);
			gag[0].x = (old_x + x) / 2;
			gag[0].y = old_y;
			gag[1].x = x;
			gag[1].y = y;
			gag[2].x = old_x;
			gag[2].y = y;
			Polygon(MemDC, gag, 3);
		}
		else if (draw && shape == 4) { // 사각형
			x = (int)LOWORD(lParam);
			y = (int)HIWORD(lParam);
			MoveToEx(MemDC, old_x, old_y, NULL);
			Rectangle(MemDC, old_x, old_y, x, y);
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_RBUTTONDBLCLK:
		COLORREF cur_color;
		int cur_r[13];
		pt_Mouse.x = (int)LOWORD(lParam);
		pt_Mouse.y = (int)HIWORD(lParam);
		int cur_count;

		control.now_ptr = control.head_ptr;
		cur_count = control.count;
		for(int i=0; i<control.count; i++){
			vi_free[i][0] = control.now_ptr->vshape;
			vi_free[i][1] = control.now_ptr->old.x;
			vi_free[i][2] = control.now_ptr->old.y;
			vi_free[i][3] = control.now_ptr->now.x;
			vi_free[i][4] = control.now_ptr->now.y;
			vi_free[i][5] = control.now_ptr->size;
			vi_free[i][6] = control.now_ptr->line_color;
			vi_free[i][7] = control.now_ptr->flag_fill;
			vi_free[i][8] = control.now_ptr->fill_color;
			vi_free[i][9] = control.now_ptr->line_color_count;
			vi_free[i][10] = control.now_ptr->fill_color_count;
			vi_free[i][11] = control.now_ptr->lineshape;
			control.now_ptr = control.now_ptr->ptr;
		}
		cur_color = GetPixel(hdc,pt_Mouse.x,pt_Mouse.y);
		for(int i=0; i<control.count-1; i++){
			if(vi_free[i][1] <= pt_Mouse.x && pt_Mouse.x <= vi_free[i][3] && vi_free[i][2]<= pt_Mouse.y && pt_Mouse.y <= vi_free[i][4] && cur_tmp ==0){
				if(vi_free[i][7] != 0){
					if(vi_free[i][8] == cur_color){
						for(int k=0; k<13; k++){//임시 저장
							cur_r[k] = vi_free[i][k];

						}
						for(int j=i; j<cur_count; j++){ // 위치 변경
							for(int q=0; q<13; q++){
								vi_free[j][q] = vi_free[j+1][q];
							}

						}
						for(int v=0; v<13; v++){ // 저장한 값 넣기
							vi_free[cur_count-1][v] = cur_r[v];
						}
						cur_tmp =1;
					}
				}
				else if(vi_free[i][7] == 0){

						for(int k=0; k<13; k++){//임시 저장
							cur_r[k] = vi_free[i][k];

						}
						for(int j=i; j<cur_count; j++){ // 위치 변경
							for(int q=0; q<13; q++){
								vi_free[j][q] = vi_free[j+1][q];
							}

						}
						for(int v=0; v<13; v++){ // 저장한 값 넣기
							vi_free[cur_count-1][v] = cur_r[v];
						}
						cur_tmp =1;

				}

			}
			else if(vi_free[i][3] <= pt_Mouse.x && pt_Mouse.x <= vi_free[i][1] && vi_free[i][4]<= pt_Mouse.y && pt_Mouse.y <= vi_free[i][2] && cur_tmp ==0){
				if(vi_free[i][7] != 0){
					if(vi_free[i][8] == cur_color){
						for(int k=0; k<13; k++){//임시 저장
							cur_r[k] = vi_free[i][k];

						}
						for(int j=i; j<cur_count; j++){ // 위치 변경
							for(int q=0; q<13; q++){
								vi_free[j][q] = vi_free[j+1][q];
							}

						}
						for(int v=0; v<13; v++){ // 저장한 값 넣기
							vi_free[cur_count-1][v] = cur_r[v];
						}
						cur_tmp =1;
					}
				}
				else if(vi_free[i][7] == 0){

						for(int k=0; k<13; k++){//임시 저장
							cur_r[k] = vi_free[i][k];

						}
						for(int j=i; j<cur_count; j++){ // 위치 변경
							for(int q=0; q<13; q++){
								vi_free[j][q] = vi_free[j+1][q];
							}

						}
						for(int v=0; v<13; v++){ // 저장한 값 넣기
							vi_free[cur_count-1][v] = cur_r[v];
						}
						cur_tmp =1;
				}

			}
			else if(vi_free[i][3] <= pt_Mouse.x && pt_Mouse.x <= vi_free[i][1] && vi_free[i][2]<= pt_Mouse.y && pt_Mouse.y <= vi_free[i][4] && cur_tmp ==0){
				if(vi_free[i][7] != 0){
					if(vi_free[i][8] == cur_color){
						for(int k=0; k<13; k++){//임시 저장
							cur_r[k] = vi_free[i][k];

						}
						for(int j=i; j<cur_count; j++){ // 위치 변경
							for(int q=0; q<13; q++){
								vi_free[j][q] = vi_free[j+1][q];
							}

						}
						for(int v=0; v<13; v++){ // 저장한 값 넣기
							vi_free[cur_count-1][v] = cur_r[v];
						}
						cur_tmp =1;
					}
				}
				else if(vi_free[i][7] == 0){

					for(int k=0; k<13; k++){//임시 저장
						cur_r[k] = vi_free[i][k];

					}
					for(int j=i; j<cur_count; j++){ // 위치 변경
						for(int q=0; q<13; q++){
							vi_free[j][q] = vi_free[j+1][q];
						}

					}
					for(int v=0; v<13; v++){ // 저장한 값 넣기
						vi_free[cur_count-1][v] = cur_r[v];
					}
					cur_tmp =1;
				}

			}
			else if(vi_free[i][1] <= pt_Mouse.x && pt_Mouse.x <= vi_free[i][3] && vi_free[i][4]<= pt_Mouse.y && pt_Mouse.y <= vi_free[i][2] && cur_tmp ==0){
				if(vi_free[i][7] != 0){
					if(vi_free[i][8] == cur_color){
						for(int k=0; k<13; k++){//임시 저장
							cur_r[k] = vi_free[i][k];

						}
						for(int j=i; j<cur_count; j++){ // 위치 변경
							for(int q=0; q<13; q++){
								vi_free[j][q] = vi_free[j+1][q];
							}

						}
						for(int v=0; v<13; v++){ // 저장한 값 넣기
							vi_free[cur_count-1][v] = cur_r[v];
						}
						cur_tmp =1;
					}
				}
				else if(vi_free[i][7] == 0){

					for(int k=0; k<13; k++){//임시 저장
						cur_r[k] = vi_free[i][k];

					}
					for(int j=i; j<cur_count; j++){ // 위치 변경
						for(int q=0; q<13; q++){
							vi_free[j][q] = vi_free[j+1][q];
						}

					}
					for(int v=0; v<13; v++){ // 저장한 값 넣기
						vi_free[cur_count-1][v] = cur_r[v];
					}
					cur_tmp =1;
				}

			}

		}
		control.now_ptr = control.head_ptr;
		for(int i=0; i<control.count; i++){
			control.now_ptr->vshape = vi_free[i][0];
			control.now_ptr->old.x = vi_free[i][1];
			control.now_ptr->old.y = vi_free[i][2];
			control.now_ptr->now.x = vi_free[i][3];
			control.now_ptr->now.y = vi_free[i][4];
			control.now_ptr->size = vi_free[i][5];
			control.now_ptr->line_color = vi_free[i][6];
			control.now_ptr->flag_fill = vi_free[i][7];
			control.now_ptr->fill_color	= vi_free[i][8];
			control.now_ptr->line_color_count = vi_free[i][9];
			control.now_ptr->fill_color_count = vi_free[i][10];
			control.now_ptr->lineshape = vi_free[i][11];
			control.now_ptr = control.now_ptr->ptr;
		}
		control.now_ptr = control.head_ptr;
		PatBlt(MemDC, 0, 0, rt.right, rt.bottom, WHITENESS);
		PatBlt(backMemDC, 0, 0, rt.right, rt.bottom, WHITENESS);
		BitBlt(hdc, 0, 0, rt.right, rt.bottom, MemDC, 0, 0, SRCCOPY);
		for(int j=0; j<control.count; j++){
			switch(control.now_ptr->lineshape){
			case(0):
				hpen = CreatePen(PS_SOLID, control.now_ptr->size, control.now_ptr->line_color);
				old_hpen = (HPEN)SelectObject(MemDC, hpen);
				DeleteObject(old_hpen);
				break;
			case(1):
				hpen = CreatePen(PS_DASH, control.now_ptr->size, control.now_ptr->line_color);
				old_hpen = (HPEN)SelectObject(MemDC, hpen);
				DeleteObject(old_hpen);
				break;
			case(2):
				hpen = CreatePen(PS_DOT, control.now_ptr->size, control.now_ptr->line_color);
				old_hpen = (HPEN)SelectObject(MemDC, hpen);
				DeleteObject(old_hpen);
				break;
			}
			switch(control.now_ptr->vshape){
			case 1: //직선
				p_color = control.now_ptr->line_color;
				size = control.now_ptr->size;
				MoveToEx(MemDC, control.now_ptr->old.x, control.now_ptr->old.y, NULL);
				LineTo(MemDC, control.now_ptr->now.x, control.now_ptr->now.y);
				break;
			case 2: //원
				if(control.now_ptr->flag_fill == 1) {
					hbrush = CreateSolidBrush(control.now_ptr->fill_color);
					old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
					DeleteObject(old_hbrush);
				}
				else{ //브러시 0
					hbrush = (HBRUSH)GetStockObject(NULL_BRUSH); // brush를 사용하지 않음
					old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
					DeleteObject(old_hbrush);
				}
				p_color = control.now_ptr->line_color;
				size = control.now_ptr->size;

				MoveToEx(MemDC, control.now_ptr->old.x,control.now_ptr->old.y, NULL);
				Ellipse(MemDC, control.now_ptr->old.x, control.now_ptr->old.y, control.now_ptr->now.x, control.now_ptr->now.y);
				break;
			case 3:
				if(control.now_ptr->flag_fill == 1) {
					hbrush = CreateSolidBrush(control.now_ptr->fill_color);
					old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
					DeleteObject(old_hbrush);
				}
				else{ //브러시 0
					hbrush = (HBRUSH)GetStockObject(NULL_BRUSH); // brush를 사용하지 않음
					old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
					DeleteObject(old_hbrush);
				}
				p_color = control.now_ptr->line_color;
				size = control.now_ptr->size;
				POINT gag[3];
				gag[0].x=(control.now_ptr->old.x + control.now_ptr->now.x)/2;
				gag[0].y=control.now_ptr->old.y;
				gag[1].x=control.now_ptr->now.x;
				gag[1].y=control.now_ptr->now.y;
				gag[2].x=control.now_ptr->old.x;
				gag[2].y=control.now_ptr->now.y;
				Polygon(MemDC, gag, 3);
				break;
			case 4:
				if(control.now_ptr->flag_fill == 1) {
					hbrush = CreateSolidBrush(control.now_ptr->fill_color);
					old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
					DeleteObject(old_hbrush);
				}
				else{ //브러시 0
					hbrush = (HBRUSH)GetStockObject(NULL_BRUSH); // brush를 사용하지 않음
					old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
					DeleteObject(old_hbrush);
				}
				p_color = control.now_ptr->line_color;
				size = control.now_ptr->size;
				MoveToEx(MemDC, control.now_ptr->old.x, control.now_ptr->old.y, NULL);
				Rectangle(MemDC, control.now_ptr->old.x, control.now_ptr->old.y, control.now_ptr->now.x, control.now_ptr->now.y);
				break;
			case 0:
				size = control.now_ptr->size;
				MoveToEx(MemDC, control.now_ptr->old.x, control.now_ptr->old.y, NULL);
				LineTo(MemDC, control.now_ptr->now.x, control.now_ptr->now.y);
				break;
			}
			control.now_ptr = control.now_ptr->ptr;
		}
		cur_tmp = 0;
		BitBlt(hdc, 0, 0, rt.right, rt.bottom, MemDC, 0, 0, SRCCOPY);
		BitBlt(backMemDC, 0, 0, rt.right, rt.bottom, hdc, 0, 0, SRCCOPY);
		break;



	case WM_PAINT:
		BitBlt(hdc, 0, 0, rt.right, rt.bottom, MemDC, 0, 0, SRCCOPY);
		if (shape == 0 || new_flag == 1) {
			BitBlt(backMemDC, 0, 0, rt.right, rt.bottom, MemDC, 0, 0, SRCCOPY);
			new_flag = 0;
		}
		break;

	case WM_DESTROY:
		vi* temp;
		control.now_ptr = control.head_ptr;
		for (int i = 0; i < control.count; i++) {
			temp = control.now_ptr;
			control.now_ptr = control.now_ptr->ptr;
			delete(temp);
		}
		delete(control.now_ptr);
		DeleteDC(MemDC);
		DeleteDC(backMemDC);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);			 //CASE에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
}