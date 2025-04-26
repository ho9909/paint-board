#include "head.h"

void make_pen(){
	hpen = CreatePen(PS_SOLID, size, p_color);
	old_hpen = (HPEN)SelectObject(MemDC, hpen);
	DeleteObject(old_hpen);
}

void make_dashpen(){
	hpen = CreatePen(PS_DASH, size, p_color);
	old_hpen = (HPEN)SelectObject(MemDC, hpen);
	DeleteObject(old_hpen);
}
void make_dotpen(){
	hpen = CreatePen(PS_DOT, size, p_color);
	old_hpen = (HPEN)SelectObject(MemDC, hpen);
	DeleteObject(old_hpen);
}
void make_penE(){
	hpen = CreatePen(PS_SOLID, size, e_color);
	old_hpen = (HPEN)SelectObject(MemDC, hpen);
	DeleteObject(old_hpen);
}

void make_dashpenE(){
	hpen = CreatePen(PS_DASH, size, e_color);
	old_hpen = (HPEN)SelectObject(MemDC, hpen);
	DeleteObject(old_hpen);
}
void make_dotpenE(){
	hpen = CreatePen(PS_DOT, size, e_color);
	old_hpen = (HPEN)SelectObject(MemDC, hpen);
	DeleteObject(old_hpen);
}

void filenew(){
	vi* temp;
	control.now_ptr = control.head_ptr;
	for (int i = 0; i < control.count; i++) { 
		temp = control.now_ptr;
		control.now_ptr = control.now_ptr->ptr;
		delete(temp);
	}
	delete(control.now_ptr);
	control.count = 0;
	control.head_ptr = NULL;
	control.now_ptr = NULL;
}
