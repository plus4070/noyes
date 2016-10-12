#include "SharedPointerTest.h"
#include "../include/RTPS/Structure/CacheChange.h"
#include "../include/Utility/PlatformOrientedInterface.h"
#include <stdio.h>

namespace CNU_DDS
{
	void	SharedPointerTest()
	{
		SHARED_PTR_TYPE_NATIVE<CacheChange>	shared_ptr_list[SHARED_PTR_TEST_SIZE];
		CacheChange*						native_ptr_list[SHARED_PTR_TEST_SIZE];
		unsigned long						shared_ptr_time_start	= 0;
		unsigned long						shared_ptr_time_end		= 0;
		unsigned long						native_ptr_time_start	= 0;
		unsigned long						native_ptr_time_end		= 0;
		int									i						= 0;

		printf("===== POINTER CREATING TIME =====\n");

		shared_ptr_time_start	= get_current_time();
		for(i=0; i<SHARED_PTR_TEST_SIZE; i++)
		{
			shared_ptr_list[i]	= SHARED_PTR_TYPE_NATIVE<CacheChange>(new CacheChange());
		}
		shared_ptr_time_end		= get_current_time();

		native_ptr_time_start	= get_current_time();
		for(i=0; i<SHARED_PTR_TEST_SIZE; i++)
		{
			native_ptr_list[i]	= new CacheChange();
		}
		native_ptr_time_end		= get_current_time();

		printf("Shared pointer : %8d(ms)\n", shared_ptr_time_end - shared_ptr_time_start);
		printf("Native pointer : %8d(ms)\n", native_ptr_time_end - native_ptr_time_start);
		printf("\n");



		printf("===== POINTER REFERENCING TIME =====\n");

		shared_ptr_time_start	= get_current_time();
		for(i=0; i<SHARED_PTR_TEST_SIZE; i++)
		{
			SHARED_PTR_TYPE_NATIVE<CacheChange>	tmp_ptr;
			tmp_ptr	= shared_ptr_list[i];
		}
		shared_ptr_time_end		= get_current_time();

		native_ptr_time_start	= get_current_time();
		for(i=0; i<SHARED_PTR_TEST_SIZE; i++)
		{
			CacheChange*	tmp_ptr;
			tmp_ptr	= native_ptr_list[i];
		}
		native_ptr_time_end		= get_current_time();

		printf("Shared pointer : %8d(ms)\n", shared_ptr_time_end - shared_ptr_time_start);
		printf("Native pointer : %8d(ms)\n", native_ptr_time_end - native_ptr_time_start);
		printf("\n");



		printf("===== POINTER RELEASE TIME =====\n");

		shared_ptr_time_start	= get_current_time();
		for(i=0; i<SHARED_PTR_TEST_SIZE; i++)
		{
			shared_ptr_list[i].reset();
		}
		shared_ptr_time_end		= get_current_time();

		native_ptr_time_start	= get_current_time();
		for(i=0; i<SHARED_PTR_TEST_SIZE; i++)
		{
			delete(native_ptr_list[i]);
		}
		native_ptr_time_end		= get_current_time();

		printf("Shared pointer : %8d(ms)\n", shared_ptr_time_end - shared_ptr_time_start);
		printf("Native pointer : %8d(ms)\n", native_ptr_time_end - native_ptr_time_start);
		printf("\n");
	}
}
