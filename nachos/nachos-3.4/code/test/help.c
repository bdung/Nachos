#include "syscall.h"



int main()
{
	PrintString("\t********* DANH SACH THANH VIEN ************\n");
	PrintString("\t*	1.Nguyen Hoan Hoai Tam\t19127544	*\n");
	PrintString("\t*	2.Nguyen Vu Khoi\t20127676	*\n");
	PrintString("\t*	3.Bui Thi Dung\t\t20127472	*\n");
	PrintString("\t*******************************************\n");
	PrintString("		MO TA VE HAM BUBBLE SORT\n");
	PrintString("Voi 1 mang so nguyen chua duoc sap xep co kich thuoc n.Ta dung 1 vong for voi i = n-1 va cho 1 vong for ben trong voi bien j chay tu 0 den i.Trong vong for con thi ta so sanh 2 phan tu canh nha.\n");
	PrintString("Neu 2 phan tu dung sai vi tri thi ta hoan doi vi tri 2 phan tu do cho nhau.Cu chay vong for va cho i giam cho den khi bien i >= 1 thi dung\n");
	PrintString("\t********************************\n");
	PrintString("		MO TA HAM XUAT BANG ASCII\n");
	PrintString("Ta dung 1 vong lap for cho bien 'i' chay tu 0 den 255, sau do dung ham PrintChar() de in ra man hinh  ki tu va ham PrintInt() de xuat gia tri trong bang ma ASCII tuong ung voi ki tu do\n");
	PrintString("\t********************************\n");
	Halt();
}
