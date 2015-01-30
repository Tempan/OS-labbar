#include <stdio.h>
#include <Windows.h>
#include "wrapper.h"
int j = 0;
CRITICAL_SECTION CriticalSection; 

LPTSTR Slot = TEXT("\\\\.\\mailslot\\sample_mailslot");

//LAB 2 
DWORD WINAPI PrintMoon4Ever( LPVOID lpParam ) 
{
	int i;
	//forloop goes infinite
	for (i = 0; i < INFINITE; i++)
	{
		printf("Hello Moon! \n");
		Sleep(200);		//Sleep 200ms = 0.2s
	}
}

//LAB 3
DWORD WINAPI Thread_Moon( LPVOID lpParam ) 
{
	int i;
	EnterCriticalSection(&CriticalSection);		//A mutex lock!
	for (i = 0; i < 10; i++)
	{
		printf("Hello Moon! \n");
		Sleep(200);
	}
	LeaveCriticalSection(&CriticalSection);		//Opens again
}
DWORD WINAPI Thread_Hello( LPVOID lpParam ) 
{
	int i;
	EnterCriticalSection(&CriticalSection);		//A mutex lock!
	for (i = 0; i < 10; i++)
	{
		printf("Hello World! \n");
		Sleep(200);
	}
	LeaveCriticalSection(&CriticalSection);		//Opens again
}

//LAB 4
DWORD WINAPI ThreadWrite( LPVOID lpParam ) 
{
	HANDLE WriteHandle;
	char arr[20];
	struct pt *planet;									//Create pointer of strukt
	planet = (struct pt*)malloc(sizeof(struct pt));		//Give pointer memory the sixe of the struct
	WriteHandle = mailslotConnect(Slot);
	while(1)
	{
		fgets(arr, sizeof(arr), stdin);
		//planet->name = arr;
		strcpy_s(planet->name, 20, arr);				//copy array to planet
		planet->name[19] = '\0';						//Array is done
		mailslotWrite(WriteHandle, arr, strlen(arr));
	}	
}


int main(int argc, const char* argv[])
{
	HANDLE ReadSlot;

	//This part is for the Mailslots (Create)
	struct pt *planet = (struct pt*)malloc(sizeof(struct pt));

	//Declatations for loop
	int i;

	ReadSlot = mailslotCreate(Slot);			//Connect to the created mailslot

	threadCreate(ThreadWrite, 0);				//Create a thread to the write part

	while (1)
	{
		mailslotRead(ReadSlot, planet, 424);			//Read from console
	}

	mailslotClose(ReadSlot);					//Close mailslot

#pragma region LAB 3
	/*
	//if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400)) // 0x00000400 hexadecimal string for 1024
	//return;

	while (1) //infinity loop
	{
	if(j == 0)
	{
	//Create a thread and send it too the "Thread_Moon" function
	threadCreate(Thread_Moon, 0);  
	//Set j to 1 to say it's "Thread_Hello"s turn
	j = 1;
	}
	else if (j == 1)
	{
	//Create a thread and send it too the "Thread_Hello" function
	threadCreate(Thread_Hello, 0); 
	//Set j to 0 to say it's "Thread_Moon"s turn
	j = 0;
	}
	}
	*/
#pragma endregion 

#pragma region LAB 2
	/*
	//Create thread to run the "PrintMoon4Ever" function
	threadCreate(PrintMoon4Ever, 0); 
	for (i = 0; i < 10; i++)	
	{
	printf("Hello World! \n");
	//Wait 1000ms = 1sec
	Sleep(1000);
	}
	*/
#pragma endregion

#pragma region LAB 1
	/*
	for (i = 0; i < 10; i++)	
	{
	printf("Hello World! \n");
	//Wait 1000ms = 1sec
	Sleep(1000);
	}
	*/
#pragma endregion 

	//Keeps console alive
	system("pause");
	return 0;
}