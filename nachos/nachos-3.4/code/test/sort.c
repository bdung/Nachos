/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"
#include "copyright.h"




void BubbleSort(int arr[], int n)
{
    int i, j, temp;
    int haveSwap = 0;
    for (i = n-1; i >=1; i--)
    {
        haveSwap = 0;
        for (j = 0; j < i-1; j++)
	{
            if (arr[j] > arr[j+1]){ //Swap if arr[j] > arr[j+1]
                 temp = arr[j];
		arr[j] = arr[j+1];
		arr[j+1] = temp;
                haveSwap = 1; 
            }
        }
        if(haveSwap == 0){
            break;
        }
    }
}

void readArray(int arr[], int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		PrintString("Nhap phan tu thu ");
		PrintNum(i + 1);
		PrintChar(':');
		arr[i] = ReadNum();
	}		
}


void printArray(int arr[], int n)
{
    int i;
    for (i=0; i < n; i++)
    {
        PrintNum(arr[i]);
   	PrintChar(' ');
    }
}

int main()
{
	int n, arr[100];
	
	PrintString("Enter number of array(<= 100): ");
	n = ReadNum();
	readArray(arr, n);
	PrintString("Array before sort: ");
	printArray(arr, n);
	BubbleSort(arr, n);
	PrintString("\nArray after sort: ");
	printArray(arr, n);

	Halt();
}
