// exception.cc 


//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.



#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include <time.h>



#define MaxFileLength 32 // Do dai quy uoc cho file name

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	Cac quy dinh su dung thanh ghi:
//
// 	   1. Thanh ghi r2: Chua ma lenh syscall va ket qua thuc hien o syscall se tra nguoc ve
// 	   2. Thanh ghi r4: Tham so thu nhat
//	   3. Thanh ghi r5: Tham so thu hai
//	   4. Thanh ghi r6: Tham so thu ba	   
//	   5. Thanh ghi r7: Tham so thu tu
//
// 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

// Doi thanh ghi Program counter cua he thong ve sau 4 byte de tiep tuc nap lenh
void IncreasePC()
{
	int counter = machine->ReadRegister(PCReg);
   	machine->WriteRegister(PrevPCReg, counter);
    	counter = machine->ReadRegister(NextPCReg);
    	machine->WriteRegister(PCReg, counter);
   	machine->WriteRegister(NextPCReg, counter + 4);
}

// Input: Khong gian dia chi User(int) - gioi han cua buffer(int)
// Output: Bo nho dem Buffer(char*)
// Chuc nang: Sao chep vung nho User sang vung nho System
char* User2System(int s, int limit)
{
	int i; //chi so index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; //can cho chuoi terminal
	if (kernelBuf == NULL)
		return kernelBuf;
		
	memset(kernelBuf, 0, limit + 1);
	
	for (i = 0; i < limit; i++)
	{
		machine->ReadMem(s + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}


// Input: Khong gian vung nho User(int) - gioi han cua buffer(int) - bo nho dem buffer(char*)
// Output: So byte da sao chep(int)
// Chuc nang: Sao chep vung nho System sang vung nho User
int System2User(int s, int len, char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0;
	do{
		oneChar = (int)buffer[i];
		machine->WriteMem(s + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}


// xu ly ngoai le runtime Exception va system call
void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

	switch (which) { // 'which' la chi EnceptionType
		case NoException:
			return;

		case PageFaultException:
			//DEBUG('a', "\n No valid translation found");
			printf("\n\n No valid translation found");
			interrupt->Halt();
			break;

		case ReadOnlyException:
			//DEBUG('a', "\n Write attempted to page marked read-only");
			printf("\n\n Write attempted to page marked read-only");
			interrupt->Halt();
			break;

		case BusErrorException:
			//DEBUG('a', "\n Translation resulted invalid physical address");
			printf("\n\n Translation resulted invalid physical address");
			interrupt->Halt();
			break;

		case AddressErrorException:
			//DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space");
			printf("\n\n Unaligned reference or one that was beyond the end of the address space");
			interrupt->Halt();
			break;

		case OverflowException:
			//DEBUG('a', "\nInteger overflow in add or sub.");
			printf("\n\n Integer overflow in add or sub.");
			interrupt->Halt();
			break;

		case IllegalInstrException:
			//DEBUG('a', "\n Unimplemented or reserved instr.");
			printf("\n\n Unimplemented or reserved instr.");
			interrupt->Halt();
			break;

		case NumExceptionTypes:
			//DEBUG('a', "\n Number exception types");
			printf("\n\n Number exception types");
			interrupt->Halt();
			break;

		case SyscallException:
			switch (type){

				case SC_Halt:{
					// Input: Khong co
					// Output: Thong bao tat may
					// Chuc nang: Tat HDH
					//DEBUG('a', "\nShutdown, initiated by user program. ");
					printf("\nShutdown, initiated by user program. ");
					interrupt->Halt();
					return;
					}
				case SC_ReadNum:
					{
					    
					    int MAX_BUFFER = 255;
					    char* buffer = new char[MAX_BUFFER + 1];
					    int numbytes = gSynchConsole->Read(buffer, MAX_BUFFER); //  doc data tu man hinh
					    int number = 0; 						                  			                                         
					    int firstNumIndex = 0;                    
					    if(buffer[0] == '-')                    
						firstNumIndex = 1;                          			   		                                                           
					    for(int i = firstNumIndex; i < numbytes; i++)					                    
						if(buffer[i] == '.' or (buffer[i] < '0' && buffer[i] > '9')) // Tra ve neu ki tu ko hop le                      			    
						    goto RETURN;                                                                                                                                           
					    for(int i = firstNumIndex; i < numbytes; i++)                    
						number = number * 10 + (int)(buffer[i] - 48); // ep kieu ve kieu int                                                    
					    if(buffer[0] == '-') //Kiem tra dau
						number = number * -1;
					    goto RETURN;
					    RETURN:
					    	machine->WriteRegister(2, number); // Ghi number vao thanh ghi so 2
					    	IncreasePC(); // Tang program counter
					    	delete buffer;
					    	return;					
					}

				
				case SC_PrintNum:
					{
    					char* buffer;
						    
					    int number = machine->ReadRegister(4); // Lay gia tri chuoi tu thanh ghi so 4
					    if(number == 0)
					    {
						gSynchConsole->Write("0", 1); // in so 0
						IncreasePC(); // Tang program counter
						return;    
					    }                                                            
					    int numberOfNum = 0;                   			
					    int t_number = number; 
						// Tinh so luong chu so
					    while(t_number)
					    {
						numberOfNum++;
						t_number /= 10;
					    } 
					    bool pos = (number < 0) ? 1 : 0; // De tang vi tri mang len 1 neu number < 0 nguoc lai thi khong tang vi tri
					    if(number < 0){                                         
						buffer = new char[numberOfNum + 1]; //Tao mang moi, phan tu dau chua dau am
                                            	buffer[0] = '-'; // Gan vi tri dau la dau am
						number = -number; // Chuyen so number thanh so duong                 		    
					    }
					    else if (number > 0) 
					    {
						buffer = new char[numberOfNum];
					    }
					    for(int i = numberOfNum - 1; i >= 0; i--)
					    {	
						buffer[i + pos] = (char)((number % 10) + 48);  // Ep kieu ve char va them so vao mang
						number /= 10;
					    }                
					    gSynchConsole->Write(buffer, numberOfNum + pos);// in chuoi
					    delete buffer;
					    IncreasePC(); // Tang program counter
					    return;        								
					}

				case SC_ReadChar:
				{

					//Input: Khong co
					//Output: Duy nhat 1 ky tu (char)
					//Cong dung: Doc mot ky tu tu nguoi dung nhap
					int maxBytes = 255;
					char* buffer = new char[255];
					int numBytes = gSynchConsole->Read(buffer, maxBytes);//  doc data tu man hinh

					if(numBytes > 1) //Neu nhap nhieu hon 1 ky tu thi khong hop le
					{
						printf("Chi duoc nhap duy nhat 1 ky tu!");
						//DEBUG('a', "\nERROR: Chi duoc nhap duy nhat 1 ky tu!");
						machine->WriteRegister(2, 0); // Ghi vao thanh ghi so 2
					}
					else if(numBytes == 0) //Ky tu rong
					{
						printf("Ky tu rong!");
						//DEBUG('a', "\nERROR: Ky tu rong!");
						machine->WriteRegister(2, 0); // Ghi vao thanh ghi so 2
					}
					else
					{
						//Chuoi vua lay co dung 1 ky tu, lay ky tu o index = 0, return vao thanh ghi R2
						char c = buffer[0];
						machine->WriteRegister(2, c); // Ghi vao thanh ghi so 2
					}

					delete buffer;
					IncreasePC(); // Tang program counter
					break;
				}

				case SC_PrintChar:
				{
					
					char c = (char)machine->ReadRegister(4); // Doc ki tu tu thanh ghi r4
					gSynchConsole->Write(&c, 1); // In ky tu tu bien c, 1 byte
					IncreasePC(); //Tang program counter
					break;

				}

				case SC_ReadString:
				{
					int s, length;
					char* buffer;
					s = machine->ReadRegister(4); // Lay gia tri chuoi tu thanh ghi so 4
					length = machine->ReadRegister(5); // Lay kich thuoc cua chuoi tu thanh ghi so 5
					buffer = User2System(s, length); // Copy chuoi tu vung nho User Space sang System Space
					gSynchConsole->Read(buffer, length); // Doc chuoi tu man hinh 
					System2User(s, length, buffer); // Sao chep chuoi tu vung nho System sang User Space
					IncreasePC(); // Tang Program Counter 
					break;
				}

				case SC_PrintString:
				{
			
					int s;
					char* buffer;
					s = machine->ReadRegister(4); // Lay gia tri cua chuoi tu thanh ghi so 4
					buffer = User2System(s, 255); // Sao chep chuoi tu vung nho System sang User Space voi bo dem 255 ki tu
					int length = 0;
					while (buffer[length] != 0) length++; // Dem do dai that cua chuoi
					gSynchConsole->Write(buffer, length); // Goi ham Write cua SynchConsole de in chuoi
					delete buffer; 
					IncreasePC(); // Tang Program Counter 
					break;
				}
				case SC_RandomNum:
				{
					RandomInit(time(NULL));
					machine->WriteRegister(2, Random());
					IncreasePC(); // Tang Program Counter 
					break;
				}

		
		}
	}

}


