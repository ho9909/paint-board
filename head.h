#include <windows.h>
#include "resource.h"

extern HDC hdc, backMemDC, MemDC;
extern HBITMAP OldhBit, hBit;
extern BOOL draw;
extern int x, y, old_x, old_y, shape, size, p_color, e_color;
extern RECT rt;
extern HPEN hpen, old_hpen;
extern HBRUSH hbrush, old_hbrush;
extern OPENFILENAME OFN; //����� ����ü
extern HANDLE fp; 
extern DWORD readn;

typedef struct save_vi { // vi ������
		int		vshape; // ���� ���4
		int		lineshape;
        POINT	old; // ó�� ���� ��ǥ8
        POINT	now; // ���� ��ǥ8
		int	flag_fill; // ä��� ����4
        COLORREF	line_color; // ���� ����4
        COLORREF	fill_color; // ä��� ����4
		int line_color_count;
		int fill_color_count;
		int		size; // ���� �β�4
		save_vi*	ptr; // ���� ������4
} vi;

typedef struct LinkedList { 
		save_vi*	now_ptr; // ���� ������
		save_vi*	head_ptr; // ù��° ������
		DWORD count; // vi����ü�� ����
} Linkedlist;

typedef struct tagRAWINFOHEARER{
        LONG       biWidth;
        LONG       biHeight;

} RAWINFOHEADER;

extern LinkedList control;
extern vi* sub_r;
void save(HWND hwnd);
void load(HWND hwnd);
void make_pen();
void make_dashpen();
void make_dotpen();
void make_penE();
void make_dashpenE();
void make_dotpenE();
void filenew();
void vi_set();