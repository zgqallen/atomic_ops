#include <windows.h>
#include <tchar.h>
#include "atomic_op.h"

#define NUMS 50

int g_Value = 0;
long long g_lValue = 0;

int g_Flag = 0;
#define SET1 0x01
#define SET2 0x02

int exp_val[NUMS] = {0};
int real_val[NUMS] = {0};

DWORD WINAPI MyThreadFunction( LPVOID lpParam )
{
	volatile int ppid, i;
	ppid = *(int*)lpParam;
	printf("Thread %d start to loading...\n", ppid);

	/* increase */
	ATOMIC_ADD(&g_Value, NUMS);
	for(i = 0; i < NUMS; i++)
	{
		ATOMIC_INC(&g_Value);
	}

	exp_val[ppid] = g_Value + 1;
	real_val[ppid] = ATOMIC_INC_RETURN(&g_Value);

	if(real_val[ppid] != exp_val[ppid])
	{
		printf("Unit test ATOMIC_INC_RETURN -- FAILED, expected %d got %d.\n", exp_val[ppid], real_val[ppid]);
	}
	
	Sleep(2000);
	/* bigint log add */
	for(i = 0; i < NUMS; i++)
	{
		ATOMIC_INC_BIGINT(&g_lValue);
	}

	/* and */
	ATOMIC_OR(&g_Flag, SET1);
	ATOMIC_OR(&g_Flag, SET2);


	Sleep(2000);

	/* decrease cleanup */
	ATOMIC_DEC_RETURN(&g_Value);
	for(i = 0; i < NUMS; i++)
	{
		ATOMIC_DEC(&g_Value);
	}

	ATOMIC_SUB(&g_Value, NUMS);

	Sleep(2000);
	for(i = 0; i < NUMS; i++)
	{
		ATOMIC_DEC_BIGINT(&g_lValue);
	}
	
	printf("Thread %d exit...\n", ppid);
	return 0;

}

int _tmain(int argc, _TCHAR* argv[])
{
	int i;
	HANDLE  hThreadArray[NUMS]; 
	DWORD   dwThreadIdArray[NUMS];

	for(i = 0; i < NUMS; i++)
	{
		int ppid = i;
        hThreadArray[i] = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            MyThreadFunction,       // thread function name
            (void*)&ppid,          // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadIdArray[i]);   // returns the thread identifier 
		Sleep(1000);
	}

	WaitForMultipleObjects(NUMS, hThreadArray, TRUE, INFINITE);


	/* Check the g_value should return to 0.*/
	if(g_Value == 0)
	{
		printf("Unit test ATOMIC_ADD/DEC -- PASSED.\n");
	}
	else
	{
		printf("Unit test ATOMIC_ADD/DEC -- FAILED.\n");
	}

	if(g_lValue == 0)
	{
		printf("Unit test ATOMIC_ADD/DEC_BIGINT -- PASSED.\n");
	}
	else
	{
		printf("Unit test ATOMIC_ADD/DEC_BIGINT -- FAILED.\n");
	}

	if((CAS32(&g_Value, 0, NUMS) == 0) && (g_Value ==  NUMS))
	{
		printf("Unit test CAS32 -- PASSED.\n");
	}
	else
	{
		printf("Unit test CAS32 -- FAILED.\n");
	}

	if((g_Flag & SET1) && (g_Flag &SET2))
	{
		ATOMIC_AND(&g_Flag, 0x0);
		if(g_Flag == 0)
		{
			printf("Unit test ATOMIC_AND/OR -- PASSED.\n");
		}
		else
		{
			printf("Unit test ATOMIC_AND/OR -- FAILED.\n");
		}
	}
	else
	{
		printf("Unit test ATOMIC_AND/OR -- FAILED.\n");
	}	

	printf("sizeof(long) %d and sizeof(long long) %d and sizeof(int) %d sizeof(int32_t) %d.\n", sizeof(long), sizeof(long long), sizeof(int), sizeof(int32_t));

	return 0;
}
