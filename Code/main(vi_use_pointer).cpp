#include "head.h"


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

LPCTSTR lpszClass = TEXT("4. 그림판 만들기 2017E7032 장현우");			// LPCSTR = char *
HINSTANCE g_hinstance;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, //WINAPI : 윈도우 프로그램이라는 의미
	LPSTR lpszCmdLine, int nCmdShow)						 //hInstance : 운영체제의 커널이 응용 프로그램에 부여한 ID
{																 //szCmdLine : 커멘트라인 상에서 프로그램 구동 시 전달된 문자열
	HWND	hwnd;												 //iCmdShow : 윈도우가 화면에 출력될 형태
	MSG		msg;
	WNDCLASS WndClass;											//WndClass 라는 구조체 정의									 
	WndClass.style			= CS_HREDRAW | CS_VREDRAW;			//출력스타일 : 수직/수평의 변화시 다시 그림		//Horizontal, Vertical 
	WndClass.lpfnWndProc	= WndProc;							//프로시저 함수명	
	WndClass.cbClsExtra		= 0;			 					//O/S 사용 여분 메모리 (Class)
	WndClass.cbWndExtra		= 0;			 					//O/s 사용 여분 메모리 (Window)
	WndClass.hInstance		= hInstance;						//응용 프로그램 ID
	WndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);	  //아이콘 유형
	WndClass.hCursor		= LoadCursor(NULL, IDC_CROSS);		  //커서 유형
	WndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);//배경색   
	WndClass.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1);		 //메뉴 이름
	WndClass.lpszClassName	= lpszClass;						 //클래스 이름
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

	while(GetMessage(&msg, NULL, 0, 0))							 //WinProc()에서 PostQuitMessage() 호출 때까지 처리
	{															 // 
		TranslateMessage(&msg);
		DispatchMessage(&msg);									 //WinMain -> WinProc  
	}
	return (int)msg.wParam;
}

HDC hdc, backMemDC, MemDC, sizeDC;
HBITMAP OldhBit, hBit, sizeHBIT;
BOOL draw=FALSE;
RECT rt, mrt;
HPEN hpen, old_hpen;
HBRUSH hbrush, old_hbrush;
int x=0, y=0, old_x=0, old_y=0, shape=0, size=1, p_color=0x000000, b_color=0x000000, line_shape=0, brus=0, new_flag=0, max_width, max_height;
int old_wi = 0, old_he = 0;
OPENFILENAME SFN, OFN; 
HANDLE fp; 
DWORD readn;
BITMAP map;
BITMAPINFOHEADER infohead;
BITMAPFILEHEADER hf;
BYTE* Data;
float xPos=0.0, yPos=0.0;
TCHAR lpstrFile[100], lpstrFile2[100];//파일이름 저장위함
LinkedList control;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)     
{
	
	int teee=0;
	PAINTSTRUCT ps;
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hwnd;
	OFN.lpstrFilter = TEXT("비트맵 파일(.bmp)\0*.bmp\0VI 파일(.vi)\0*.vi\0모든파일(*.*)\0*.*\0");
	OFN.lpstrFile = lpstrFile;
	OFN.Flags = OFN_NOCHANGEDIR;
	OFN.nMaxFile = MAX_PATH;

	switch (iMsg) 
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rt);
		hdc=GetDC(hwnd);

		backMemDC=CreateCompatibleDC(hdc); 
		MemDC=CreateCompatibleDC(hdc);

		OldhBit=CreateCompatibleBitmap(hdc, rt.right, rt.bottom); //도화지 준비!
		hBit=CreateCompatibleBitmap(hdc, rt.right, rt.bottom);

		(HBITMAP)SelectObject(backMemDC, OldhBit); //도화지 세팅
		(HBITMAP)SelectObject(MemDC, hBit);

		FillRect(backMemDC, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH)); //도화지 색 변경 
		FillRect(MemDC, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH)); 
		max_width=rt.right;
		max_height=rt.bottom;

		hpen = CreatePen(PS_SOLID, size, 0x000000); // pen
		old_hpen = (HPEN)SelectObject(MemDC, hpen);
		hbrush = (HBRUSH)GetStockObject(NULL_BRUSH); // brush를 사용하지 않음
		old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정

		control.count=0;
		//control.head_ptr = new vi;
		//control.now_ptr = control.head_ptr;
		//control.now_ptr->ptr = NULL;
		control.head_ptr = NULL;
		control.now_ptr = NULL;
		break;

	//case WM_SIZE:
	//	GetClientRect(hwnd, &rt); // right 가로, bottom 세로
	//	if(max_width < rt.right || max_height < rt.bottom){
	//					
	//		if(max_width < rt.right){
	//			old_wi = max_width;
	//			max_width = rt.right;
	//			mrt.right = rt.right;
	//		}
	//		if(max_height < rt.bottom){
	//			old_he = max_height;
	//			max_height = rt.bottom;
	//			mrt.bottom = rt.bottom;
	//		}
	//		sizeDC=CreateCompatibleDC(hdc);
	//		sizeHBIT=CreateCompatibleBitmap(hdc, max_width, max_height); // 도화지 생성
	//		(HBITMAP)SelectObject(sizeDC, sizeHBIT); // 도화지 세팅
	//		FillRect(sizeDC, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));
	//		// 사이즈 생성
	//		BitBlt(sizeDC, 0, 0, old_wi, old_he, backMemDC, 0, 0, SRCCOPY);
	//		
	//		// 사이즈로 백업
	//		DeleteDC(backMemDC);
	//		backMemDC=CreateCompatibleDC(hdc);
	//			OldhBit=CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
	//			(HBITMAP)SelectObject(backMemDC, OldhBit);
	//		FillRect(backMemDC, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));
	//		BitBlt(backMemDC, 0, 0, rt.right, rt.bottom, sizeDC, 0, 0, SRCCOPY);

	//		DeleteDC(MemDC);
	//		MemDC=CreateCompatibleDC(hdc);
	//		hBit=CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
	//		(HBITMAP)SelectObject(MemDC, hBit);
	//		FillRect(MemDC, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));
	//		BitBlt(MemDC, 0, 0, rt.right, rt.bottom, backMemDC, 0, 0, SRCCOPY);
	//		
	//		BitBlt(hdc, 0, 0, rt.right, rt.bottom, backMemDC, 0, 0, SRCCOPY);
	//	}
	case WM_COMMAND:
		switch (LOWORD(wParam)){   

		case(ID_SHAPE_LINE):
			shape=1;
			break;
		case(ID_SHAPE_CIRCLE):
			shape=2;
			break;
		case(ID_SHAPE_TRIANGLE):
			shape=3;
			break;
		case(ID_SHAPE_RECTANGLE):
			shape=4;
			break;
		case(ID_SHAPE_FREELINE):
			shape=0;
			break;
		case(ID_PX_1PX):
			size=1;
			switch (line_shape){              
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
		case(ID_PX_3PX):
			size=3;
			switch (line_shape){              
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
		case(ID_PX_5PX):
			size=5;
			switch (line_shape){              
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
		case(ID_COLOR_BLACK ):
			p_color = 0x000000;
			switch (line_shape){              
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
		case(ID_COLOR_RED):
			p_color = 0x0000ff;
			switch (line_shape){              
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
		case(ID_COLOR_GREEN):
			p_color = 0x00ff00;
			switch (line_shape){              
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
		case(ID_COLOR_BLUE):
			p_color = 0xff0000;
			switch (line_shape){              
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
			hbrush = (HBRUSH)GetStockObject(NULL_BRUSH); // brush를 사용하지 않음
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
			DeleteObject(old_hbrush);
			break;			
		case(ID_FILL_BLACK):
			brus=1;
			b_color=0x000000;
			hbrush = CreateSolidBrush(0x000000); // 단색의 brush를 생성
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
			DeleteObject(old_hbrush);
			break;			
		case(ID_FILL_WHITE):
			brus=1;
			b_color=0xffffff;
			hbrush = CreateSolidBrush(0xffffff); // 단색의 brush를 생성
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
			DeleteObject(old_hbrush);
			break;			
		case(ID_FILL_RED):
			brus=1;
			b_color=0x0000ff;
			hbrush = CreateSolidBrush(0x0000ff); // 단색의 brush를 생성
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
			DeleteObject(old_hbrush);
			break;			
		case(ID_FILL_GREEN):
			brus=1;
			b_color=0x00ff00;
			hbrush = CreateSolidBrush(0x00ff00); // 단색의 brush를 생성
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
			DeleteObject(old_hbrush);
			break;			
		case(ID_FILL_BLUE):
			brus=1;
			b_color=0x00ff0000;
			hbrush = CreateSolidBrush(0xff0000); // 단색의 brush를 생성
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush를 dc에 지정
			DeleteObject(old_hbrush);
			break;
		case(ID_LINESHAPE_SOLIDLINE):
			line_shape=0;
			make_pen();
			break;
		case(ID_LINESHAPE_DOTTEDLINE):
			line_shape=2;
			make_dotpen();
			break;
		case(ID_LINESHAPE_DASHLINE):
			line_shape=1;
			make_dashpen();
			break;

		case(ID_MENU_LOAD):
			//열기
			int widthstep;
			PatBlt(MemDC, 0, 0, rt.right, rt.bottom, WHITENESS);	
			InvalidateRect(hwnd, NULL, TRUE);

			if(GetOpenFileName(&OFN) != 0) //다이얼로그 열기
			{
				//@@@@@
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
				WORD checkbm;
				fp  = CreateFile(OFN.lpstrFile, GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				ReadFile(fp, &checkbm, 2, &readn, NULL); //2바이트
				if(checkbm == 0x4d42){
					CloseHandle(fp);

					fp  = CreateFile(OFN.lpstrFile, GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

					if (fp != INVALID_HANDLE_VALUE) // 파일의 존재 여부 확인 실패하면 INVALID_HANDLE_VALUE를 반화한다.
					{
						ReadFile(fp,&hf, sizeof(BITMAPFILEHEADER), &readn, NULL); //14바이트
						ReadFile(fp,&infohead, sizeof(BITMAPINFOHEADER), &readn, NULL); //40바이트

						xPos = 0;
						yPos = infohead.biHeight - 1;// 아래부터 그릴거고 배열이니까 1빼줌


						if (infohead.biBitCount == 1) // 단일 비트
						{
							RGBQUAD       pRGB[2];//팔레트
							ReadFile(fp, pRGB, sizeof(RGBQUAD) * 2, &readn, NULL);//팔레트의 정보를 가져옴
							Data = new BYTE[infohead.biSizeImage];

							ReadFile(fp, Data, infohead.biSizeImage, &readn, NULL);//우선 다저장

							widthstep = 32 - (infohead.biWidth % 32); // widthstep의 개수
							if(widthstep==32) // 32일 경우 0
								widthstep=0;

							BYTE half; // 1비트이므로 8개로 쪼개야함
							BYTE bit = 128;//0b 1000 0000
							int count=0;

							for(int i=0 ; i < infohead.biSizeImage ; i++) // 픽셀비트의 개수 만큼 반복
							{
								for(int j=0 ; j<8 ; j++) // 바이트를 읽어왔으니 8번 반복
								{
									half = Data[i] & bit; // 첫번째 바이트에서 맨 앞 비트 읽어오기
									half = half >> 7; // 첫번째 자리수로 이동, 즉 팔레트 인덱스임
									SetPixel(MemDC, xPos, yPos, RGB(pRGB[half].rgbRed, pRGB[half].rgbGreen, pRGB[half].rgbBlue)); // 색상 데이터 입력
									xPos++; // 다음 픽셀로 이동
									if(xPos >= infohead.biWidth) // 가로폭만큼 출력 완료시에
									{
										yPos --; // 다음줄로 이동
										xPos = 0; 
									}
									count++; // 몇개의 픽셀을 출력햇는가?
									if(count == infohead.biWidth) // 가로폭만큼 출력 완료시에
									{
										count = 0; // 초기화
										i += (widthstep / 8); // widthstep 바이트만큼 건너 뛰기
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
								SetPixel(MemDC, xPos, yPos, RGB(pRGB[Data[i]].rgbRed, pRGB[Data[i]].rgbGreen, pRGB[Data[i]].rgbBlue)); // 색상 데이터 입력
								xPos++; // 다음 픽셀로 이동
								if(xPos >= infohead.biWidth) // 가로폭만큼 출력 완료시에
								{
									yPos --; // 다음줄로 이동
									xPos = 0; 
									i += widthstep;
								}
							}
						}

						else if (infohead.biBitCount == 24) // 24비트 일때 한개의 픽셀 = 3바이트
						{
							int count=0;

							widthstep = 4 - (infohead.biWidth * 3 % 4);
							if (widthstep == 4) 
								widthstep = 0;

							Data = new BYTE[infohead.biSizeImage];
							ReadFile(fp, Data, infohead.biSizeImage, &readn, NULL); // 픽셀 데이터를 바이트 단위로 배열에 저장

							for (int i = 0; i < infohead.biWidth * infohead.biHeight; i++) // 픽셀 개수 만큼 반복
							{
								if (i != 0 && i % infohead.biWidth == 0 ) // 줄 넘김
								{
									xPos = 0;
									yPos--;
									count++; // 몇개의 줄을 출력했는가? 나중에 widthstep개수를 계산하기 위함
								}
								SetPixel(MemDC, xPos, yPos, RGB(Data[(i*3)+(count*widthstep)+2], Data[(i*3)+(count*widthstep)+1], Data[(i*3)+(count*widthstep)]));//그리기
								// i*3 은 1픽셀이 3바이트라서, count*widthstep 허수의 개수를 계산하기 위함, +2 +1은 bgr 순서라서
								xPos++; // 다음 픽셀
							}
						}
						InvalidateRect(hwnd, NULL, FALSE);
					}
					CloseHandle(fp);
				}
				else // vi 파일 열기
				{
					CloseHandle(fp);
					fp  = CreateFile(OFN.lpstrFile, GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					DWORD tcount;
					ReadFile(fp, &tcount, 4, &readn, NULL); 
					control.count = tcount; // 구조체 개수 읽어오기
					control.head_ptr = new vi; // 동적 할당
					control.count = 0; 
					control.now_ptr = control.head_ptr;
					while(tcount>0){
						control.count++;
						ReadFile(fp, &control.now_ptr->vshape, 4, &readn, NULL); 
						ReadFile(fp, &control.now_ptr->old.x, 4, &readn, NULL); 
						ReadFile(fp, &control.now_ptr->old.y, 4, &readn, NULL); 
						ReadFile(fp, &control.now_ptr->now.x, 4, &readn, NULL); 
						ReadFile(fp, &control.now_ptr->now.y, 4, &readn, NULL);
						ReadFile(fp, &control.now_ptr->flag_fill, 4, &readn, NULL); 
						ReadFile(fp, &control.now_ptr->line_color, 4, &readn, NULL); 
						ReadFile(fp, &control.now_ptr->fill_color, 4, &readn, NULL); 
						ReadFile(fp,&control.now_ptr->size, 4, &readn, NULL); 
						tcount--;
						if(control.now_ptr -> vshape == 1){ // line
							p_color = control.now_ptr->line_color;
							size = control.now_ptr->size;
							make_pen();
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
							p_color = control.now_ptr->line_color;
							size = control.now_ptr->size;
							make_pen();
							
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
							p_color = control.now_ptr->line_color;
							size = control.now_ptr->size;
							make_pen();
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
							p_color = control.now_ptr->line_color;
							size = control.now_ptr->size;
							make_pen();
							MoveToEx(MemDC, control.now_ptr->old.x, control.now_ptr->old.y, NULL);
							Rectangle(MemDC, control.now_ptr->old.x, control.now_ptr->old.y, control.now_ptr->now.x, control.now_ptr->now.y);
						}
						else if(control.now_ptr -> vshape == 0){
							p_color = control.now_ptr->line_color;
							size = control.now_ptr->size;
							make_pen();
							MoveToEx(MemDC, control.now_ptr->old.x, control.now_ptr->old.y, NULL);
							LineTo(MemDC, control.now_ptr->now.x, control.now_ptr->now.y);
						}
						control.now_ptr->ptr = new vi; // 동적할당 한번 더
						control.now_ptr = control.now_ptr->ptr;
					}
					control.now_ptr -> ptr = NULL;
				}
				CloseHandle(fp);
			}
			else
			{
				MessageBox(hwnd, TEXT("비트맵 파일을 열지 못했습니다."), 0, MB_OK);
			}
			break;
			
		case(ID_MENU_SAVE):
			{				
				memset(&SFN,0,sizeof(OPENFILENAME));
				SFN.lStructSize = sizeof(OPENFILENAME);
				SFN.hwndOwner = hwnd;
				SFN.lpstrFilter = TEXT("24비트 비트맵(*.bmp)\0*.bmp\08비트 비트맵(*.bmp)\0*.bmp\0단색 비트맵 파일(*.bmp)\0*.bmp\0VI 파일(*.vi)\0*.vi\0모든 파일(*.*)\0*.*\0");
				SFN.lpstrDefExt = TEXT("txt");
				SFN.lpstrFile = lpstrFile2;
				SFN.nMaxFile = 100; 
				int b, g, r;
				
				if(GetSaveFileName(&SFN) != 0) // 다이얼로그 열기
				{
					int size, r, g, b;
					DWORD Size;
					TCHAR lpstrFile[100]; // 파일이름 저장위함
					HANDLE fp; 
					DWORD readn;  
					BITMAPINFO *pih;

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
					infohead.biClrUsed = 0;									// 색상 테이블중 비트맵에서 사용되는 색상 수 -> 0일 경우 비트맵은 사용 가능한 모든 색상을 다 사용한다. 
					infohead.biClrImportant = 0;							// 비트맵을 출력하는데 필수적인 색상 수 -> 0일 경우 비트맵은 사용 가능한 모든 색상을 다 사용되어야한다.

					if(SFN.nFilterIndex==1){ // 24비트 일 경우

						widthstep = 4 - (infohead.biWidth * 3 % 4);
						if (widthstep == 4) 
							widthstep = 0;

						hf.bfSize = (widthstep * infohead.biHeight) + (infohead.biWidth * infohead.biHeight * 3) + 54;		// 비트맵 파일 전체의 크기
						hf.bfOffBits = 54;																					// 픽셀 데이터의 시작 주소
						infohead.biBitCount = 24;																			// 한 픽셀당 몇 개의 비트로 이루어지는가
						infohead.biSizeImage = hf.bfSize - 54;
						// 헤더는 이제 완성
						fp = CreateFile(lpstrFile2, GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // 파일 생성

						WriteFile(fp, &hf, sizeof(hf), &readn, NULL);// 헤더 작성
						WriteFile(fp, &infohead, sizeof(infohead), &readn, NULL); // 파일헤더 작성
						COLORREF color;
						for (int y = rt.bottom-1; y >= 0; y--)
						{
							for (int k = 0; k < rt.right; k++)
							{
								color = GetPixel(backMemDC, k, y);
								b = color &  255;
								color = color >> 8;
								g = color & 255;
								color = color >> 8;
								r = color & 255;
								WriteFile(fp,(char*)&r,1,&color,NULL);
								WriteFile(fp,(char*)&g,1,&color,NULL);
								WriteFile(fp,(char*)&b,1,&color,NULL);
							}
							for (int temp = widthstep; temp > 0; x--)
							{
								int step = 0;
								WriteFile(fp,(char*)&step, 1, &color, NULL);
							}
						}							
						CloseHandle(fp);
					}
					else if (SFN.nFilterIndex==2){ // 8비트(256색) 일 경우

						widthstep = 4 - (infohead.biWidth % 4);
						if (widthstep == 4) 
							widthstep = 0;
						// widthstep 계산 완료
						hf.bfSize = (widthstep * infohead.biHeight) + (infohead.biWidth * infohead.biHeight) + 1078;	// 비트맵 파일 전체의 크기
						hf.bfOffBits = 1078;																			// 픽셀 데이터의 시작 주소
						infohead.biBitCount = 8;																		// 한 픽셀당 몇 개의 비트로 이루어지는가
						infohead.biSizeImage = hf.bfSize - 1078;
						// 헤더는 완성
						int temp_pal[1024]={0, 0, 0, 0, 0, 0, 128, 0, 0, 128, 0, 0, 0, 128, 128, 0, 128, 0, 0, 0, 128, 0, 128, 0, 128, 128, 0, 0, 192, 192, 192, 0, 192, 220, 192, 0, 240, 202, 166, 0, 0, 32, 64, 0, 0, 32, 96, 0, 0, 32, 128, 0, 0, 32, 160, 0, 0, 32, 192, 0, 0, 32, 224, 0, 0, 64, 0, 0, 0, 64, 32, 0, 0, 64, 64, 0, 0, 64, 96, 0, 0, 64, 128, 0, 0, 64, 160, 0, 0, 64, 192, 0, 0, 64, 224, 0, 0, 96, 0, 0, 0, 96, 32, 0, 0, 96, 64, 0, 0, 96, 96, 0, 0, 96, 128, 0, 0, 96, 160, 0, 0, 96, 192, 0, 0, 96, 224, 0, 0, 128, 0, 0, 0, 128, 32, 0, 0, 128, 64, 0, 0, 128, 96, 0, 0, 128, 128, 0, 0, 128, 160, 0, 0, 128, 192, 0, 0, 128, 224, 0, 0, 160, 0, 0, 0, 160, 32, 0, 0, 160, 64, 0, 0, 160, 96, 0, 0, 160, 128, 0, 0, 160, 160, 0, 0, 160, 192, 0, 0, 160, 224, 0, 0, 192, 0, 0, 0, 192, 32, 0, 0, 192, 64, 0, 0, 192, 96, 0, 0, 192, 128, 0, 0, 192, 160, 0, 0, 192, 192, 0, 0, 192, 224, 0, 0, 224, 0, 0, 0, 224, 32, 0, 0, 224, 64, 0, 0, 224, 96, 0, 0, 224, 128, 0, 0, 224, 160, 0, 0, 224, 192, 0, 0, 224, 224, 0, 64, 0, 0, 0, 64, 0, 32, 0, 64, 0, 64, 0, 64, 0, 96, 0, 64, 0, 128, 0, 64, 0, 160, 0, 64, 0, 192, 0, 64, 0, 224, 0, 64, 32, 0, 0, 64, 32, 32, 0, 64, 32, 64, 0, 64, 32,96, 0, 64, 32, 128, 0, 64, 32, 160, 0, 64, 32, 192, 0, 64, 32, 224, 0, 64, 64, 0, 0, 64, 64, 32, 0, 64, 64, 64, 0, 64, 64, 96, 0, 64, 64, 128, 0, 64, 64, 160, 0, 64, 64, 192, 0, 64, 64, 224, 0, 64, 96, 0, 0, 64, 96, 32, 0, 64, 96, 64, 0, 64, 96, 96, 0, 64, 96, 128, 0, 64, 96, 160, 0, 64, 96, 192, 0, 64, 96, 224, 0, 64, 128, 0, 0, 64, 128, 32, 0, 64, 128, 64, 0, 64, 128, 96, 0, 64, 128, 128, 0, 64, 128, 160, 0, 64, 128, 192, 0, 64, 128, 224, 0, 64, 160, 0, 0, 64, 160, 32, 0, 64, 160, 64, 0, 64, 160, 96, 0, 64, 160, 128, 0, 64, 160, 160, 0, 64, 160, 192, 0, 64, 160, 224, 0, 64, 192, 0, 0, 64, 192, 32, 0, 64, 192, 64, 0, 64, 192, 96, 0, 64, 192, 128, 0, 64, 192, 160, 0, 64, 192, 192, 0, 64, 192, 224, 0, 64, 224, 0, 0, 64, 224, 32, 0, 64, 224, 64, 0, 64, 224, 96, 0, 64, 224, 128, 0, 64, 224, 160, 0, 64, 224, 192, 0, 64, 224, 224, 0, 128, 0, 0, 0, 128, 0, 32, 0, 128, 0, 64, 0, 128, 0, 96, 0, 128, 0, 128, 0, 128, 0, 160, 0, 128, 0, 192, 0, 128, 0, 224, 0, 128, 32, 0, 0, 128, 32, 32, 0, 128, 32, 64, 0, 128, 32, 96, 0, 128, 32, 128, 0, 128, 32, 160, 0, 128, 32, 192, 0, 128, 32, 224, 0, 128, 64, 0, 0, 128, 64, 32, 0, 128, 64, 64, 0, 128, 64, 96, 0, 128, 64, 128, 0, 128, 64, 160, 0, 128, 64, 192, 0, 128, 64, 224, 0, 128, 96, 0, 0, 128, 96, 32, 0, 128, 96, 64, 0, 128, 96, 96, 0, 128, 96, 128, 0, 128, 96, 160, 0, 128, 96, 192, 0, 128, 96, 224, 0, 128, 128, 0, 0, 128, 128, 32, 0, 128, 128, 64, 0, 128, 128, 96, 0, 128, 128, 128, 0, 128, 128, 160, 0, 128, 128, 192, 0, 128, 128, 224, 0, 128, 160, 0, 0, 128, 160, 32, 0, 128, 160, 64, 0, 128, 160, 96, 0, 128, 160, 128, 0, 128, 160, 160, 0, 128, 160, 192, 0, 128, 160, 224, 0, 128, 192, 0, 0, 128, 192, 32, 0, 128, 192, 64, 0, 128, 192, 96, 0, 128, 192, 128, 0, 128, 192, 160, 0, 128, 192, 192, 0, 128, 192, 224, 0, 128, 224, 0, 0, 128, 224, 32, 0, 128, 224, 64, 0, 128, 224, 96, 0, 128, 224, 128, 0, 128, 224, 160, 0, 128, 224, 192, 0, 128, 224, 224, 0, 192, 0, 0, 0, 192, 0, 32, 0, 192, 0, 64, 0, 192, 0, 96, 0, 192, 0, 128, 0, 192, 0, 160, 0, 192, 0, 192, 0, 192, 0, 224, 0, 192, 32, 0, 0, 192, 32, 32, 0, 192, 32, 64, 0, 192, 32, 96, 0, 192, 32, 128, 0, 192, 32, 160, 0, 192, 32, 192, 0, 192, 32, 224, 0, 192, 64, 0, 0, 192, 64, 32, 0, 192, 64, 64, 0, 192, 64, 96, 0, 192, 64, 128, 0, 192, 64, 160, 0, 192, 64, 192, 0, 192, 64, 224, 0, 192, 96, 0, 0, 192, 96, 32, 0, 192, 96, 64, 0, 192, 96, 96, 0, 192, 96, 128, 0, 192, 96, 160, 0, 192, 96, 192, 0, 192, 96, 224, 0, 192, 128, 0, 0, 192, 128, 32, 0, 192, 128, 64, 0, 192, 128, 96, 0, 192, 128, 128, 0,  192, 128, 160, 0, 192, 128, 192, 0, 192, 128, 224, 0, 192, 160, 0, 0, 192, 160, 32, 0, 192, 160, 64, 0, 192, 160, 96, 0, 192, 160, 128, 0, 192, 160, 160, 0, 192, 160, 192, 0, 192, 160, 224, 0, 192, 192, 0, 0, 192, 192, 32, 0, 192, 192, 64, 0, 192, 192, 96, 0, 192, 192, 128, 0, 192, 192, 160, 0, 240, 251, 255, 0, 164, 160, 160, 0, 128, 128, 128, 0, 0, 0, 255, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 0, 0, 0, 255, 0, 255, 0, 255, 255, 0, 0, 255, 255, 255, 0};
						int inb[7]={0, 64, 128, 164, 192, 240, 255}, ing[12]={0, 32, 64, 96, 128, 160, 192, 202, 220, 224, 251, 255}, inr[9] = {0, 32, 64, 96, 128, 160, 192, 224, 255};
						COLORREF color;
						RGBQUAD pal[256];
						int k=0, is=0, tr, tg, tb, diffMin, count=0, zero=0, x, y;
						int intnear, temp2[1]={0};
						byte num;

						fp = CreateFile(lpstrFile2, GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // 파일 생성

						WriteFile(fp, &hf, sizeof(hf), &readn, NULL);// 헤더 작성
						WriteFile(fp, &infohead, sizeof(infohead), &readn, NULL); // 파일헤더 작성

						for(int i = 0; i < 256 ; i++)
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
						WriteFile(fp,(char*)&pal,sizeof(RGBQUAD)*256,&readn,NULL); // 팔레트 작성
						for (y = rt.bottom-1; y >= 0; y--)
						{
							for (x = 0; x < rt.right; x++)
							{
								count++;
								color = GetPixel(backMemDC, x, y);
								b = color &  255;
								color = color >> 8;
								g = color & 255;
								color = color >> 8;
								r = color & 255;
								// 각 rgb값 찾았음
								for(num = 0; num < 256; num++){ // 팔레트에 있는 색이면 삽입
									is=0;
									if(pal[num].rgbBlue == r && pal[num].rgbGreen == g && pal[num].rgbRed == b){
										WriteFile(fp,(char*)&num,1,&readn,NULL);
										is=1;
										break;
									}
								}
								//if(is==0){ // 못찾았을 경우 근사값 찾기
								//	diffMin = inb[0] - b;
								//	for(int i = 0; i < 7; i++) //b
								//	{
								//		diff = inb[i] - b;
								//		if(abs(diffMin) > abs(diff))
								//		{
								//			diffMin = diff;
								//			intnear = inb[i];
								//		}
								//	}
								//	b = intnear;
								//	diffMin = ing[0] - g;
								//	for(int i = 0 ;i < 12; i++) //g
								//	{
								//		diff = ing[i] - g;
								//		if(abs(diffMin) > abs(diff))
								//		{
								//			diffMin = diff;
								//			intnear = ing[i];
								//		}
								//	}
								//	g = intnear;
								//	diffMin = inr[0] - r;
								//	for(int i = 0 ;i < 9; i++) //r
								//	{
								//		diff = inr[i] - r;
								//		if(abs(diffMin) > abs(diff))
								//		{
								//			diffMin = diff;
								//			intnear = inr[i];
								//		}
								//	}
								//	r = intnear;
								//	for(int temp = 0; temp < 256; temp++){ // 근사값으로 다시
								//		if(pal[temp].rgbBlue == b && pal[temp].rgbGreen == g && pal[temp].rgbRed == r){
								//			WriteFile(fp, (char*)temp2, 1, &readn, NULL);
								//			is=0;
								//		}
								//	}
								//}	
								if(count==rt.right){
									for(int temp = 0; temp < widthstep/2; temp++){
										WriteFile(fp, (char*)zero, 1, &readn, NULL);
									}
									count=0;
								}
							}// 한줄 다 작성
							x = 0;
						}	
					CloseHandle(fp);	
					}
					else if (SFN.nFilterIndex == 4){ // vi로 저장
						control.now_ptr = control.head_ptr;
						fp = CreateFile(lpstrFile2, GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // 파일 생성
						WriteFile(fp, &control.count, 4, &readn, NULL);
						for(int i = 0; i < control.count; i++) { // head부터 다음 포인터 정보를 제외하고 다 삽입
							WriteFile(fp, control.now_ptr, sizeof(vi)-4, &readn, NULL);
							control.now_ptr = control.now_ptr->ptr;
						}
						CloseHandle(fp);
					}
					break;
				}
			}
		case(ID_MENU_NEW):
			vi* temp;
			PatBlt(MemDC, 0, 0, rt.right, rt.bottom, WHITENESS); 
			new_flag=1;
			InvalidateRect(hwnd,NULL,TRUE);
			control.now_ptr = control.head_ptr;
			for(int i = 0; i < control.count; i++) { // 기존 그려져서 생성된 vi구조체 할당 해제
				temp = control.now_ptr;
				control.now_ptr = control.now_ptr->ptr;
				delete(temp);
			}
			delete(control.now_ptr);
			control.count=0;
			control.head_ptr = NULL;
			control.now_ptr = NULL;
			break;
		case(ID_MENU_EXIT):
			DestroyWindow(hwnd);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		
		draw=TRUE;
		x=(int)LOWORD(lParam);
		y=(int)HIWORD(lParam);
		old_x=x;
		old_y=y;
		if(shape == 0){
			if(control.head_ptr==NULL) {
				control.head_ptr = new vi;
				control.now_ptr = control.head_ptr;
			}
			control.now_ptr->vshape = 0;
			control.now_ptr->flag_fill = 0;
			control.now_ptr->fill_color = 0;
			control.now_ptr->line_color = p_color;
			control.now_ptr->now.x = x;
			control.now_ptr->now.y = y;
			control.now_ptr->old.x = x;
			control.now_ptr->old.y = y;
			control.now_ptr->size = size;
			control.count++;
			control.now_ptr->ptr = new vi;
			control.now_ptr = control.now_ptr->ptr; // 자유선 만들면 바꾸기
			control.now_ptr->ptr = NULL;
		}
		break; 

	case WM_LBUTTONUP:
		draw=FALSE;
		if(control.head_ptr==NULL) {
			control.head_ptr = new vi;
			control.now_ptr = control.head_ptr;
		}// vi구조체가 하나도 없으면 생성
		switch(shape){
		case 1: //직선
			control.now_ptr->vshape = 1;
			control.now_ptr->flag_fill = 0;
			control.now_ptr->fill_color = 0;
			control.now_ptr->line_color = p_color;
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
			control.now_ptr->now.x = x;
			control.now_ptr->now.y = y;
			control.now_ptr->old.x = old_x;
			control.now_ptr->old.y = old_y;
			control.now_ptr->size = size;
			control.count++;
			break;
		}
		if(shape !=0) {// 자유선이 아닐 경우
			BitBlt(backMemDC, 0, 0, rt.right, rt.bottom, hdc, 0, 0, SRCCOPY);
			control.now_ptr->ptr = new vi;
			control.now_ptr = control.now_ptr->ptr; // 자유선 만들면 바꾸기
			control.now_ptr->ptr = NULL;
		}
		break;

	case WM_MOUSEMOVE:
		if(shape !=0)
			BitBlt(MemDC, 0, 0, rt.right, rt.bottom, backMemDC, 0, 0, SRCCOPY); // 백업을 현재로 초기화

		if(draw && shape == 0){  // 자유선
			MoveToEx(MemDC,x,y,NULL);
			control.now_ptr->old.x = x;
			control.now_ptr->old.y = y;
			x=(int)LOWORD(lParam);
			y=(int)HIWORD(lParam);
			LineTo(MemDC,x,y);
			control.now_ptr->vshape = 0;
			control.now_ptr->flag_fill = 0;
			control.now_ptr->fill_color = 0;
			control.now_ptr->line_color = p_color;
			control.now_ptr->now.x = x;
			control.now_ptr->now.y = y;
			
			control.now_ptr->size = size;
			control.count++;
			control.now_ptr->ptr = new vi;
			control.now_ptr = control.now_ptr->ptr;
			control.now_ptr->ptr = NULL;
		} // 자유선 정보를를 계속해서 저장
		else if(draw && shape == 1){ // 직선
			MoveToEx(MemDC, old_x, old_y, NULL);
			x=(int)LOWORD(lParam);
			y=(int)HIWORD(lParam);
			LineTo(MemDC, x, y);
		}
		else if(draw && shape == 2){ // 원
			x=(int)LOWORD(lParam);
			y=(int)HIWORD(lParam);
			MoveToEx(MemDC, old_x, old_y, NULL);
			Ellipse(MemDC, old_x, old_y, x, y);
		}
		else if(draw && shape == 3){ // 삼각형
			POINT gag[3];
			x=(int)LOWORD(lParam);
			y=(int)HIWORD(lParam);
			DeleteObject(old_hbrush);
			gag[0].x=(old_x+x)/2;
			gag[0].y=old_y;
			gag[1].x=x;
			gag[1].y=y;
			gag[2].x=old_x;
			gag[2].y=y;
			Polygon(MemDC, gag, 3);
		}
		else if(draw && shape == 4){ // 사각형
			x=(int)LOWORD(lParam);
			y=(int)HIWORD(lParam);
			MoveToEx(MemDC, old_x, old_y, NULL);
			Rectangle(MemDC, old_x, old_y, x, y);
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_PAINT:
		BitBlt(hdc, 0, 0, rt.right, rt.bottom, MemDC, 0, 0, SRCCOPY); // 현재의 그림을 출력
		if(shape == 0 || new_flag==1){
			BitBlt(backMemDC, 0, 0, rt.right, rt.bottom, MemDC, 0, 0, SRCCOPY); // 자유선일 경우 바로 백업
			new_flag=0;
		}
		break;

	case WM_DESTROY:			//프로그램 종료 처리 
		vi* temp;
		control.now_ptr = control.head_ptr;
		for(int i = 0; i < control.count; i++) {
			temp = control.now_ptr;
			control.now_ptr = control.now_ptr->ptr;
			delete(temp);
		}
		delete(control.now_ptr);
		DeleteDC(MemDC);
		DeleteDC(backMemDC);		
		DeleteDC(sizeDC);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);			 //CASE에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
}
