#include <stdio.h>
#include <pthread.h>
#include "atomic_ops.h"

#define NUMS 50

int g_Value = 0;
long long g_lValue = 0;

int g_Flag = 0;
#define SET1 0x01
#define SET2 0x02

int exp_val[NUMS] = {0};
int real_val[NUMS] = {0};

void *thread_fn(void *arg)
{
	volatile int ppid, i;
	ppid = *(int*)arg;
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
	
	sleep(2);
	/* bigint log add */
	for(i = 0; i < NUMS; i++)
	{
		ATOMIC_INC_BIGINT(&g_lValue);
	}

	/* and */
	ATOMIC_OR(&g_Flag, SET1);
	ATOMIC_OR(&g_Flag, SET2);


	sleep(2);

	/* decrease cleanup */
	ATOMIC_DEC_RETURN(&g_Value);
	for(i = 0; i < NUMS; i++)
	{
		ATOMIC_DEC(&g_Value);
	}

	ATOMIC_SUB(&g_Value, NUMS);

	sleep(2);
	for(i = 0; i < NUMS; i++)
	{
		ATOMIC_DEC_BIGINT(&g_lValue);
	}
	
	printf("Thread %d exit...\n", ppid);
	return NULL;
}

int main(int argc, char *argv[])
{
	int i;
	pthread_t pids[NUMS];

	for(i = 0; i < NUMS; i++)
	{
		int ppid = i;
		pthread_create(&pids[i], NULL, thread_fn, &ppid);
		sleep(1);
	}

	for(i = 0; i < NUMS; i++)
	{
		pthread_join(pids[i], NULL);
	}


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

	return 0;
}
