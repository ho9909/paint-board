#include "head.h"


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

LPCTSTR lpszClass = TEXT("4. �׸��� ����� 2017E7032 ������");			// LPCSTR = char *
HINSTANCE g_hinstance;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, //WINAPI : ������ ���α׷��̶�� �ǹ�
	LPSTR lpszCmdLine, int nCmdShow)						 //hInstance : �ü���� Ŀ���� ���� ���α׷��� �ο��� ID
{																 //szCmdLine : Ŀ��Ʈ���� �󿡼� ���α׷� ���� �� ���޵� ���ڿ�
	HWND	hwnd;												 //iCmdShow : �����찡 ȭ�鿡 ��µ� ����
	MSG		msg;
	WNDCLASS WndClass;											//WndClass ��� ����ü ����									 
	WndClass.style			= CS_HREDRAW | CS_VREDRAW;			//��½�Ÿ�� : ����/������ ��ȭ�� �ٽ� �׸�		//Horizontal, Vertical 
	WndClass.lpfnWndProc	= WndProc;							//���ν��� �Լ���	
	WndClass.cbClsExtra		= 0;			 					//O/S ��� ���� �޸� (Class)
	WndClass.cbWndExtra		= 0;			 					//O/s ��� ���� �޸� (Window)
	WndClass.hInstance		= hInstance;						//���� ���α׷� ID
	WndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);	  //������ ����
	WndClass.hCursor		= LoadCursor(NULL, IDC_CROSS);		  //Ŀ�� ����
	WndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);//����   
	WndClass.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1);		 //�޴� �̸�
	WndClass.lpszClassName	= lpszClass;						 //Ŭ���� �̸�
	RegisterClass(&WndClass);									 //�ռ� ������ ������ Ŭ������ �ּ� // os ���ٰ� ���� �� Ŭ������ ��Ͻ�Ŵ 

	g_hinstance = hInstance; // ���� �ν��Ͻ� �ڵ�
	hwnd = CreateWindow(lpszClass,								 //�����찡 �����Ǹ� �ڵ�(hwnd)�� ��ȯ		//���� �����츦 ����� ���� 
		lpszClass,												 //������ Ŭ����, Ÿ��Ʋ �̸�
		WS_OVERLAPPEDWINDOW,									 //������ ��Ÿ��
		CW_USEDEFAULT,											 //������ ��ġ, x��ǥ
		CW_USEDEFAULT,											 //������ ��ġ, y��ǥ
		CW_USEDEFAULT,											 //������ ��   
		CW_USEDEFAULT,											 //������ ����   
		NULL,													 //�θ� ������ �ڵ�	 
		NULL,													 //�޴� �ڵ�
		hInstance,    											 //���� ���α׷� ID
		NULL     												 //������ ������ ����
		);
	ShowWindow(hwnd, nCmdShow);									 //�������� ȭ�� ���
	UpdateWindow(hwnd);											 //O/S �� WM_PAINT �޽��� ����

	while(GetMessage(&msg, NULL, 0, 0))							 //WinProc()���� PostQuitMessage() ȣ�� ������ ó��
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
TCHAR lpstrFile[100], lpstrFile2[100];//�����̸� ��������
LinkedList control;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)     
{
	
	int teee=0;
	PAINTSTRUCT ps;
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hwnd;
	OFN.lpstrFilter = TEXT("��Ʈ�� ����(.bmp)\0*.bmp\0VI ����(.vi)\0*.vi\0�������(*.*)\0*.*\0");
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

		OldhBit=CreateCompatibleBitmap(hdc, rt.right, rt.bottom); //��ȭ�� �غ�!
		hBit=CreateCompatibleBitmap(hdc, rt.right, rt.bottom);

		(HBITMAP)SelectObject(backMemDC, OldhBit); //��ȭ�� ����
		(HBITMAP)SelectObject(MemDC, hBit);

		FillRect(backMemDC, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH)); //��ȭ�� �� ���� 
		FillRect(MemDC, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH)); 
		max_width=rt.right;
		max_height=rt.bottom;

		hpen = CreatePen(PS_SOLID, size, 0x000000); // pen
		old_hpen = (HPEN)SelectObject(MemDC, hpen);
		hbrush = (HBRUSH)GetStockObject(NULL_BRUSH); // brush�� ������� ����
		old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush�� dc�� ����

		control.count=0;
		//control.head_ptr = new vi;
		//control.now_ptr = control.head_ptr;
		//control.now_ptr->ptr = NULL;
		control.head_ptr = NULL;
		control.now_ptr = NULL;
		break;

	//case WM_SIZE:
	//	GetClientRect(hwnd, &rt); // right ����, bottom ����
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
	//		sizeHBIT=CreateCompatibleBitmap(hdc, max_width, max_height); // ��ȭ�� ����
	//		(HBITMAP)SelectObject(sizeDC, sizeHBIT); // ��ȭ�� ����
	//		FillRect(sizeDC, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));
	//		// ������ ����
	//		BitBlt(sizeDC, 0, 0, old_wi, old_he, backMemDC, 0, 0, SRCCOPY);
	//		
	//		// ������� ���
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
		case(ID_FILL_NON): // ä���
			brus=0;
			hbrush = (HBRUSH)GetStockObject(NULL_BRUSH); // brush�� ������� ����
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush�� dc�� ����
			DeleteObject(old_hbrush);
			break;			
		case(ID_FILL_BLACK):
			brus=1;
			b_color=0x000000;
			hbrush = CreateSolidBrush(0x000000); // �ܻ��� brush�� ����
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush�� dc�� ����
			DeleteObject(old_hbrush);
			break;			
		case(ID_FILL_WHITE):
			brus=1;
			b_color=0xffffff;
			hbrush = CreateSolidBrush(0xffffff); // �ܻ��� brush�� ����
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush�� dc�� ����
			DeleteObject(old_hbrush);
			break;			
		case(ID_FILL_RED):
			brus=1;
			b_color=0x0000ff;
			hbrush = CreateSolidBrush(0x0000ff); // �ܻ��� brush�� ����
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush�� dc�� ����
			DeleteObject(old_hbrush);
			break;			
		case(ID_FILL_GREEN):
			brus=1;
			b_color=0x00ff00;
			hbrush = CreateSolidBrush(0x00ff00); // �ܻ��� brush�� ����
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush�� dc�� ����
			DeleteObject(old_hbrush);
			break;			
		case(ID_FILL_BLUE):
			brus=1;
			b_color=0x00ff0000;
			hbrush = CreateSolidBrush(0xff0000); // �ܻ��� brush�� ����
			old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush�� dc�� ����
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
			//����
			int widthstep;
			PatBlt(MemDC, 0, 0, rt.right, rt.bottom, WHITENESS);	
			InvalidateRect(hwnd, NULL, TRUE);

			if(GetOpenFileName(&OFN) != 0) //���̾�α� ����
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
				ReadFile(fp, &checkbm, 2, &readn, NULL); //2����Ʈ
				if(checkbm == 0x4d42){
					CloseHandle(fp);

					fp  = CreateFile(OFN.lpstrFile, GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

					if (fp != INVALID_HANDLE_VALUE) // ������ ���� ���� Ȯ�� �����ϸ� INVALID_HANDLE_VALUE�� ��ȭ�Ѵ�.
					{
						ReadFile(fp,&hf, sizeof(BITMAPFILEHEADER), &readn, NULL); //14����Ʈ
						ReadFile(fp,&infohead, sizeof(BITMAPINFOHEADER), &readn, NULL); //40����Ʈ

						xPos = 0;
						yPos = infohead.biHeight - 1;// �Ʒ����� �׸��Ű� �迭�̴ϱ� 1����


						if (infohead.biBitCount == 1) // ���� ��Ʈ
						{
							RGBQUAD       pRGB[2];//�ȷ�Ʈ
							ReadFile(fp, pRGB, sizeof(RGBQUAD) * 2, &readn, NULL);//�ȷ�Ʈ�� ������ ������
							Data = new BYTE[infohead.biSizeImage];

							ReadFile(fp, Data, infohead.biSizeImage, &readn, NULL);//�켱 ������

							widthstep = 32 - (infohead.biWidth % 32); // widthstep�� ����
							if(widthstep==32) // 32�� ��� 0
								widthstep=0;

							BYTE half; // 1��Ʈ�̹Ƿ� 8���� �ɰ�����
							BYTE bit = 128;//0b 1000 0000
							int count=0;

							for(int i=0 ; i < infohead.biSizeImage ; i++) // �ȼ���Ʈ�� ���� ��ŭ �ݺ�
							{
								for(int j=0 ; j<8 ; j++) // ����Ʈ�� �о������ 8�� �ݺ�
								{
									half = Data[i] & bit; // ù��° ����Ʈ���� �� �� ��Ʈ �о����
									half = half >> 7; // ù��° �ڸ����� �̵�, �� �ȷ�Ʈ �ε�����
									SetPixel(MemDC, xPos, yPos, RGB(pRGB[half].rgbRed, pRGB[half].rgbGreen, pRGB[half].rgbBlue)); // ���� ������ �Է�
									xPos++; // ���� �ȼ��� �̵�
									if(xPos >= infohead.biWidth) // ��������ŭ ��� �Ϸ�ÿ�
									{
										yPos --; // �����ٷ� �̵�
										xPos = 0; 
									}
									count++; // ��� �ȼ��� ����޴°�?
									if(count == infohead.biWidth) // ��������ŭ ��� �Ϸ�ÿ�
									{
										count = 0; // �ʱ�ȭ
										i += (widthstep / 8); // widthstep ����Ʈ��ŭ �ǳ� �ٱ�
										break;
									}
									Data[i] = Data[i] << 1;
								}
							}
						}

						else if (infohead.biBitCount == 8) // 8��Ʈ
						{
							RGBQUAD       pRGB[256];//�ȷ�Ʈ
							ReadFile(fp, pRGB, sizeof(RGBQUAD) * 256, &readn, NULL);
							Data = new BYTE[infohead.biSizeImage];

							ReadFile(fp, Data, infohead.biSizeImage, &readn, NULL);

							widthstep = 4 - (infohead.biWidth % 4); // widthstep�� ����
							if(widthstep==4) // 4�� ��� 0
								widthstep=0;

							for(int i=0 ; i < infohead.biSizeImage ; i++) // �ȼ� ����Ʈ ��ŭ �ݺ�
							{
								SetPixel(MemDC, xPos, yPos, RGB(pRGB[Data[i]].rgbRed, pRGB[Data[i]].rgbGreen, pRGB[Data[i]].rgbBlue)); // ���� ������ �Է�
								xPos++; // ���� �ȼ��� �̵�
								if(xPos >= infohead.biWidth) // ��������ŭ ��� �Ϸ�ÿ�
								{
									yPos --; // �����ٷ� �̵�
									xPos = 0; 
									i += widthstep;
								}
							}
						}

						else if (infohead.biBitCount == 24) // 24��Ʈ �϶� �Ѱ��� �ȼ� = 3����Ʈ
						{
							int count=0;

							widthstep = 4 - (infohead.biWidth * 3 % 4);
							if (widthstep == 4) 
								widthstep = 0;

							Data = new BYTE[infohead.biSizeImage];
							ReadFile(fp, Data, infohead.biSizeImage, &readn, NULL); // �ȼ� �����͸� ����Ʈ ������ �迭�� ����

							for (int i = 0; i < infohead.biWidth * infohead.biHeight; i++) // �ȼ� ���� ��ŭ �ݺ�
							{
								if (i != 0 && i % infohead.biWidth == 0 ) // �� �ѱ�
								{
									xPos = 0;
									yPos--;
									count++; // ��� ���� ����ߴ°�? ���߿� widthstep������ ����ϱ� ����
								}
								SetPixel(MemDC, xPos, yPos, RGB(Data[(i*3)+(count*widthstep)+2], Data[(i*3)+(count*widthstep)+1], Data[(i*3)+(count*widthstep)]));//�׸���
								// i*3 �� 1�ȼ��� 3����Ʈ��, count*widthstep ����� ������ ����ϱ� ����, +2 +1�� bgr ������
								xPos++; // ���� �ȼ�
							}
						}
						InvalidateRect(hwnd, NULL, FALSE);
					}
					CloseHandle(fp);
				}
				else // vi ���� ����
				{
					CloseHandle(fp);
					fp  = CreateFile(OFN.lpstrFile, GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					DWORD tcount;
					ReadFile(fp, &tcount, 4, &readn, NULL); 
					control.count = tcount; // ����ü ���� �о����
					control.head_ptr = new vi; // ���� �Ҵ�
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
						else if(control.now_ptr -> vshape == 2){ // ��
							if(control.now_ptr->flag_fill == 1) {
								hbrush = CreateSolidBrush(control.now_ptr->fill_color);
								old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush�� dc�� ����
								DeleteObject(old_hbrush);
							}
							else{ //�귯�� 0
								hbrush = (HBRUSH)GetStockObject(NULL_BRUSH); // brush�� ������� ����
								old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush�� dc�� ����
								DeleteObject(old_hbrush);
							}
							p_color = control.now_ptr->line_color;
							size = control.now_ptr->size;
							make_pen();
							
							MoveToEx(MemDC, control.now_ptr->old.x,control.now_ptr->old.y, NULL);
							Ellipse(MemDC, control.now_ptr->old.x, control.now_ptr->old.y, control.now_ptr->now.x, control.now_ptr->now.y);
						}
						else if(control.now_ptr -> vshape == 3) {// �ﰢ��
							if(control.now_ptr->flag_fill == 1) {
								hbrush = CreateSolidBrush(control.now_ptr->fill_color);
								old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush�� dc�� ����
								DeleteObject(old_hbrush);
							}
							else{ //�귯�� 0
								hbrush = (HBRUSH)GetStockObject(NULL_BRUSH); // brush�� ������� ����
								old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush�� dc�� ����
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
						else if(control.now_ptr -> vshape == 4) { // �簢��
							if(control.now_ptr->flag_fill == 1) {
								hbrush = CreateSolidBrush(control.now_ptr->fill_color);
								old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush�� dc�� ����
								DeleteObject(old_hbrush);
							}
							else{ //�귯�� 0
								hbrush = (HBRUSH)GetStockObject(NULL_BRUSH); // brush�� ������� ����
								old_hbrush = (HBRUSH)SelectObject(MemDC, hbrush); // brush�� dc�� ����
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
						control.now_ptr->ptr = new vi; // �����Ҵ� �ѹ� ��
						control.now_ptr = control.now_ptr->ptr;
					}
					control.now_ptr -> ptr = NULL;
				}
				CloseHandle(fp);
			}
			else
			{
				MessageBox(hwnd, TEXT("��Ʈ�� ������ ���� ���߽��ϴ�."), 0, MB_OK);
			}
			break;
			
		case(ID_MENU_SAVE):
			{				
				memset(&SFN,0,sizeof(OPENFILENAME));
				SFN.lStructSize = sizeof(OPENFILENAME);
				SFN.hwndOwner = hwnd;
				SFN.lpstrFilter = TEXT("24��Ʈ ��Ʈ��(*.bmp)\0*.bmp\08��Ʈ ��Ʈ��(*.bmp)\0*.bmp\0�ܻ� ��Ʈ�� ����(*.bmp)\0*.bmp\0VI ����(*.vi)\0*.vi\0��� ����(*.*)\0*.*\0");
				SFN.lpstrDefExt = TEXT("txt");
				SFN.lpstrFile = lpstrFile2;
				SFN.nMaxFile = 100; 
				int b, g, r;
				
				if(GetSaveFileName(&SFN) != 0) // ���̾�α� ����
				{
					int size, r, g, b;
					DWORD Size;
					TCHAR lpstrFile[100]; // �����̸� ��������
					HANDLE fp; 
					DWORD readn;  
					BITMAPINFO *pih;

					GetObject(hBit, sizeof(BITMAP), &map); // ���� ��� ����

					hf.bfType = 0x4d42;// ������ ���¸� �����ϴ� ���� �ѹ��̸� �ݵ�� BM(bmp ������ ��ü ũ��)�̾�� �Ѵ�(0x42, 0x4d)
					hf.bfReserved1 = 0;// ����
					hf.bfReserved2 = 0;// ����

					infohead.biSize = sizeof(BITMAPINFOHEADER);				// ����ü�� ũ��
					infohead.biWidth = map.bmWidth;							// ��Ʈ�� ���� �ȼ���
					infohead.biHeight = map.bmHeight;						// ��Ʈ�� ���� �ȼ���
					infohead.biPlanes = 1;									// �÷��� ������ ��Ÿ���µ� �ݵ�� 1�� �����Ǿ��־���Ѵ�
					infohead.biCompression = BI_RGB;						// ������->������� ���� ��Ʈ�� (BI_RGB)
					infohead.biXPelsPerMeter = 0;							// ���ʹ� �����ȼ���
					infohead.biYPelsPerMeter = 0;							// ���ʹ� �����ȼ���
					infohead.biClrUsed = 0;									// ���� ���̺��� ��Ʈ�ʿ��� ���Ǵ� ���� �� -> 0�� ��� ��Ʈ���� ��� ������ ��� ������ �� ����Ѵ�. 
					infohead.biClrImportant = 0;							// ��Ʈ���� ����ϴµ� �ʼ����� ���� �� -> 0�� ��� ��Ʈ���� ��� ������ ��� ������ �� ���Ǿ���Ѵ�.

					if(SFN.nFilterIndex==1){ // 24��Ʈ �� ���

						widthstep = 4 - (infohead.biWidth * 3 % 4);
						if (widthstep == 4) 
							widthstep = 0;

						hf.bfSize = (widthstep * infohead.biHeight) + (infohead.biWidth * infohead.biHeight * 3) + 54;		// ��Ʈ�� ���� ��ü�� ũ��
						hf.bfOffBits = 54;																					// �ȼ� �������� ���� �ּ�
						infohead.biBitCount = 24;																			// �� �ȼ��� �� ���� ��Ʈ�� �̷�����°�
						infohead.biSizeImage = hf.bfSize - 54;
						// ����� ���� �ϼ�
						fp = CreateFile(lpstrFile2, GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // ���� ����

						WriteFile(fp, &hf, sizeof(hf), &readn, NULL);// ��� �ۼ�
						WriteFile(fp, &infohead, sizeof(infohead), &readn, NULL); // ������� �ۼ�
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
					else if (SFN.nFilterIndex==2){ // 8��Ʈ(256��) �� ���

						widthstep = 4 - (infohead.biWidth % 4);
						if (widthstep == 4) 
							widthstep = 0;
						// widthstep ��� �Ϸ�
						hf.bfSize = (widthstep * infohead.biHeight) + (infohead.biWidth * infohead.biHeight) + 1078;	// ��Ʈ�� ���� ��ü�� ũ��
						hf.bfOffBits = 1078;																			// �ȼ� �������� ���� �ּ�
						infohead.biBitCount = 8;																		// �� �ȼ��� �� ���� ��Ʈ�� �̷�����°�
						infohead.biSizeImage = hf.bfSize - 1078;
						// ����� �ϼ�
						int temp_pal[1024]={0, 0, 0, 0, 0, 0, 128, 0, 0, 128, 0, 0, 0, 128, 128, 0, 128, 0, 0, 0, 128, 0, 128, 0, 128, 128, 0, 0, 192, 192, 192, 0, 192, 220, 192, 0, 240, 202, 166, 0, 0, 32, 64, 0, 0, 32, 96, 0, 0, 32, 128, 0, 0, 32, 160, 0, 0, 32, 192, 0, 0, 32, 224, 0, 0, 64, 0, 0, 0, 64, 32, 0, 0, 64, 64, 0, 0, 64, 96, 0, 0, 64, 128, 0, 0, 64, 160, 0, 0, 64, 192, 0, 0, 64, 224, 0, 0, 96, 0, 0, 0, 96, 32, 0, 0, 96, 64, 0, 0, 96, 96, 0, 0, 96, 128, 0, 0, 96, 160, 0, 0, 96, 192, 0, 0, 96, 224, 0, 0, 128, 0, 0, 0, 128, 32, 0, 0, 128, 64, 0, 0, 128, 96, 0, 0, 128, 128, 0, 0, 128, 160, 0, 0, 128, 192, 0, 0, 128, 224, 0, 0, 160, 0, 0, 0, 160, 32, 0, 0, 160, 64, 0, 0, 160, 96, 0, 0, 160, 128, 0, 0, 160, 160, 0, 0, 160, 192, 0, 0, 160, 224, 0, 0, 192, 0, 0, 0, 192, 32, 0, 0, 192, 64, 0, 0, 192, 96, 0, 0, 192, 128, 0, 0, 192, 160, 0, 0, 192, 192, 0, 0, 192, 224, 0, 0, 224, 0, 0, 0, 224, 32, 0, 0, 224, 64, 0, 0, 224, 96, 0, 0, 224, 128, 0, 0, 224, 160, 0, 0, 224, 192, 0, 0, 224, 224, 0, 64, 0, 0, 0, 64, 0, 32, 0, 64, 0, 64, 0, 64, 0, 96, 0, 64, 0, 128, 0, 64, 0, 160, 0, 64, 0, 192, 0, 64, 0, 224, 0, 64, 32, 0, 0, 64, 32, 32, 0, 64, 32, 64, 0, 64, 32,96, 0, 64, 32, 128, 0, 64, 32, 160, 0, 64, 32, 192, 0, 64, 32, 224, 0, 64, 64, 0, 0, 64, 64, 32, 0, 64, 64, 64, 0, 64, 64, 96, 0, 64, 64, 128, 0, 64, 64, 160, 0, 64, 64, 192, 0, 64, 64, 224, 0, 64, 96, 0, 0, 64, 96, 32, 0, 64, 96, 64, 0, 64, 96, 96, 0, 64, 96, 128, 0, 64, 96, 160, 0, 64, 96, 192, 0, 64, 96, 224, 0, 64, 128, 0, 0, 64, 128, 32, 0, 64, 128, 64, 0, 64, 128, 96, 0, 64, 128, 128, 0, 64, 128, 160, 0, 64, 128, 192, 0, 64, 128, 224, 0, 64, 160, 0, 0, 64, 160, 32, 0, 64, 160, 64, 0, 64, 160, 96, 0, 64, 160, 128, 0, 64, 160, 160, 0, 64, 160, 192, 0, 64, 160, 224, 0, 64, 192, 0, 0, 64, 192, 32, 0, 64, 192, 64, 0, 64, 192, 96, 0, 64, 192, 128, 0, 64, 192, 160, 0, 64, 192, 192, 0, 64, 192, 224, 0, 64, 224, 0, 0, 64, 224, 32, 0, 64, 224, 64, 0, 64, 224, 96, 0, 64, 224, 128, 0, 64, 224, 160, 0, 64, 224, 192, 0, 64, 224, 224, 0, 128, 0, 0, 0, 128, 0, 32, 0, 128, 0, 64, 0, 128, 0, 96, 0, 128, 0, 128, 0, 128, 0, 160, 0, 128, 0, 192, 0, 128, 0, 224, 0, 128, 32, 0, 0, 128, 32, 32, 0, 128, 32, 64, 0, 128, 32, 96, 0, 128, 32, 128, 0, 128, 32, 160, 0, 128, 32, 192, 0, 128, 32, 224, 0, 128, 64, 0, 0, 128, 64, 32, 0, 128, 64, 64, 0, 128, 64, 96, 0, 128, 64, 128, 0, 128, 64, 160, 0, 128, 64, 192, 0, 128, 64, 224, 0, 128, 96, 0, 0, 128, 96, 32, 0, 128, 96, 64, 0, 128, 96, 96, 0, 128, 96, 128, 0, 128, 96, 160, 0, 128, 96, 192, 0, 128, 96, 224, 0, 128, 128, 0, 0, 128, 128, 32, 0, 128, 128, 64, 0, 128, 128, 96, 0, 128, 128, 128, 0, 128, 128, 160, 0, 128, 128, 192, 0, 128, 128, 224, 0, 128, 160, 0, 0, 128, 160, 32, 0, 128, 160, 64, 0, 128, 160, 96, 0, 128, 160, 128, 0, 128, 160, 160, 0, 128, 160, 192, 0, 128, 160, 224, 0, 128, 192, 0, 0, 128, 192, 32, 0, 128, 192, 64, 0, 128, 192, 96, 0, 128, 192, 128, 0, 128, 192, 160, 0, 128, 192, 192, 0, 128, 192, 224, 0, 128, 224, 0, 0, 128, 224, 32, 0, 128, 224, 64, 0, 128, 224, 96, 0, 128, 224, 128, 0, 128, 224, 160, 0, 128, 224, 192, 0, 128, 224, 224, 0, 192, 0, 0, 0, 192, 0, 32, 0, 192, 0, 64, 0, 192, 0, 96, 0, 192, 0, 128, 0, 192, 0, 160, 0, 192, 0, 192, 0, 192, 0, 224, 0, 192, 32, 0, 0, 192, 32, 32, 0, 192, 32, 64, 0, 192, 32, 96, 0, 192, 32, 128, 0, 192, 32, 160, 0, 192, 32, 192, 0, 192, 32, 224, 0, 192, 64, 0, 0, 192, 64, 32, 0, 192, 64, 64, 0, 192, 64, 96, 0, 192, 64, 128, 0, 192, 64, 160, 0, 192, 64, 192, 0, 192, 64, 224, 0, 192, 96, 0, 0, 192, 96, 32, 0, 192, 96, 64, 0, 192, 96, 96, 0, 192, 96, 128, 0, 192, 96, 160, 0, 192, 96, 192, 0, 192, 96, 224, 0, 192, 128, 0, 0, 192, 128, 32, 0, 192, 128, 64, 0, 192, 128, 96, 0, 192, 128, 128, 0,  192, 128, 160, 0, 192, 128, 192, 0, 192, 128, 224, 0, 192, 160, 0, 0, 192, 160, 32, 0, 192, 160, 64, 0, 192, 160, 96, 0, 192, 160, 128, 0, 192, 160, 160, 0, 192, 160, 192, 0, 192, 160, 224, 0, 192, 192, 0, 0, 192, 192, 32, 0, 192, 192, 64, 0, 192, 192, 96, 0, 192, 192, 128, 0, 192, 192, 160, 0, 240, 251, 255, 0, 164, 160, 160, 0, 128, 128, 128, 0, 0, 0, 255, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 0, 0, 0, 255, 0, 255, 0, 255, 255, 0, 0, 255, 255, 255, 0};
						int inb[7]={0, 64, 128, 164, 192, 240, 255}, ing[12]={0, 32, 64, 96, 128, 160, 192, 202, 220, 224, 251, 255}, inr[9] = {0, 32, 64, 96, 128, 160, 192, 224, 255};
						COLORREF color;
						RGBQUAD pal[256];
						int k=0, is=0, tr, tg, tb, diffMin, count=0, zero=0, x, y;
						int intnear, temp2[1]={0};
						byte num;

						fp = CreateFile(lpstrFile2, GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // ���� ����

						WriteFile(fp, &hf, sizeof(hf), &readn, NULL);// ��� �ۼ�
						WriteFile(fp, &infohead, sizeof(infohead), &readn, NULL); // ������� �ۼ�

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
						WriteFile(fp,(char*)&pal,sizeof(RGBQUAD)*256,&readn,NULL); // �ȷ�Ʈ �ۼ�
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
								// �� rgb�� ã����
								for(num = 0; num < 256; num++){ // �ȷ�Ʈ�� �ִ� ���̸� ����
									is=0;
									if(pal[num].rgbBlue == r && pal[num].rgbGreen == g && pal[num].rgbRed == b){
										WriteFile(fp,(char*)&num,1,&readn,NULL);
										is=1;
										break;
									}
								}
								//if(is==0){ // ��ã���� ��� �ٻ簪 ã��
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
								//	for(int temp = 0; temp < 256; temp++){ // �ٻ簪���� �ٽ�
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
							}// ���� �� �ۼ�
							x = 0;
						}	
					CloseHandle(fp);	
					}
					else if (SFN.nFilterIndex == 4){ // vi�� ����
						control.now_ptr = control.head_ptr;
						fp = CreateFile(lpstrFile2, GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // ���� ����
						WriteFile(fp, &control.count, 4, &readn, NULL);
						for(int i = 0; i < control.count; i++) { // head���� ���� ������ ������ �����ϰ� �� ����
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
			for(int i = 0; i < control.count; i++) { // ���� �׷����� ������ vi����ü �Ҵ� ����
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
			control.now_ptr = control.now_ptr->ptr; // ������ ����� �ٲٱ�
			control.now_ptr->ptr = NULL;
		}
		break; 

	case WM_LBUTTONUP:
		draw=FALSE;
		if(control.head_ptr==NULL) {
			control.head_ptr = new vi;
			control.now_ptr = control.head_ptr;
		}// vi����ü�� �ϳ��� ������ ����
		switch(shape){
		case 1: //����
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
		case 2: // ��
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
		case 3: // �ﰢ��
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
		case 4: // �簢��
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
		if(shape !=0) {// �������� �ƴ� ���
			BitBlt(backMemDC, 0, 0, rt.right, rt.bottom, hdc, 0, 0, SRCCOPY);
			control.now_ptr->ptr = new vi;
			control.now_ptr = control.now_ptr->ptr; // ������ ����� �ٲٱ�
			control.now_ptr->ptr = NULL;
		}
		break;

	case WM_MOUSEMOVE:
		if(shape !=0)
			BitBlt(MemDC, 0, 0, rt.right, rt.bottom, backMemDC, 0, 0, SRCCOPY); // ����� ����� �ʱ�ȭ

		if(draw && shape == 0){  // ������
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
		} // ������ �������� ����ؼ� ����
		else if(draw && shape == 1){ // ����
			MoveToEx(MemDC, old_x, old_y, NULL);
			x=(int)LOWORD(lParam);
			y=(int)HIWORD(lParam);
			LineTo(MemDC, x, y);
		}
		else if(draw && shape == 2){ // ��
			x=(int)LOWORD(lParam);
			y=(int)HIWORD(lParam);
			MoveToEx(MemDC, old_x, old_y, NULL);
			Ellipse(MemDC, old_x, old_y, x, y);
		}
		else if(draw && shape == 3){ // �ﰢ��
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
		else if(draw && shape == 4){ // �簢��
			x=(int)LOWORD(lParam);
			y=(int)HIWORD(lParam);
			MoveToEx(MemDC, old_x, old_y, NULL);
			Rectangle(MemDC, old_x, old_y, x, y);
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_PAINT:
		BitBlt(hdc, 0, 0, rt.right, rt.bottom, MemDC, 0, 0, SRCCOPY); // ������ �׸��� ���
		if(shape == 0 || new_flag==1){
			BitBlt(backMemDC, 0, 0, rt.right, rt.bottom, MemDC, 0, 0, SRCCOPY); // �������� ��� �ٷ� ���
			new_flag=0;
		}
		break;

	case WM_DESTROY:			//���α׷� ���� ó�� 
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
	return DefWindowProc(hwnd, iMsg, wParam, lParam);			 //CASE���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
}