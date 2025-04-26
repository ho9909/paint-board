#include <windows.h>
#include "resource.h"

extern HDC hdc, backMemDC, MemDC;
extern HBITMAP OldhBit, hBit;
extern BOOL draw;
extern int x, y, old_x, old_y, shape, size, p_color, e_color;
extern RECT rt;
extern HPEN hpen, old_hpen;
extern HBRUSH hbrush, old_hbrush;
extern OPENFILENAME OFN; //입출력 구조체
extern HANDLE fp; 
extern DWORD readn;

typedef struct save_vi { // vi 구조제
		int		vshape; // 도형 모양4
		int		lineshape;
        POINT	old; // 처음 기준 좌표8
        POINT	now; // 나중 좌표8
		int	flag_fill; // 채우기 여부4
        COLORREF	line_color; // 라인 색상4
        COLORREF	fill_color; // 채우기 색상4
		int line_color_count;
		int fill_color_count;
		int		size; // 선의 두께4
		save_vi*	ptr; // 다음 포인터4
} vi;

typedef struct LinkedList { 
		save_vi*	now_ptr; // 현재 포인터
		save_vi*	head_ptr; // 첫번째 포인터
		DWORD count; // vi구조체의 개수
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