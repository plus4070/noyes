#include "ListTest.h"

#include <iostream>

#include "../include/RTPS/Behavior/ChangeForReader.h"
#include "../include/RTPS/Behavior/DataStructure/ChangeForReaderList.h"
#include "../include/RTPS/Behavior/Type/ChangeForReaderStatusKind.h"
#include "../include/RTPS/Structure/CacheChange.h"
#include "../include/RTPS/Structure/Type/SequenceNumber_t.h"

#include "../include/Utility/SharedPtrList.h"

namespace CNU_DDS
{
	void	ChangeForReaderListTest()
	{
		ChangeForReaderList*	cfr_list	= new ChangeForReaderList(10);

		ChangeForReader*		cfr_00		= new ChangeForReader();
		ChangeForReader*		cfr_01		= new ChangeForReader();
		ChangeForReader*		cfr_02		= new ChangeForReader();
		ChangeForReader*		cfr_03		= new ChangeForReader();
		ChangeForReader*		cfr_04		= new ChangeForReader();
		ChangeForReader*		cfr_05		= new ChangeForReader();
		ChangeForReader*		cfr_06		= new ChangeForReader();
		ChangeForReader*		cfr_07		= new ChangeForReader();
		ChangeForReader*		cfr_08		= new ChangeForReader();
		ChangeForReader*		cfr_09		= new ChangeForReader();
		ChangeForReader*		cfr;

#ifdef SHARED_PTR_CACHE_CHANGE
		pCacheChange			cc_00		= pCacheChange(new CacheChange());
		pCacheChange			cc_01		= pCacheChange(new CacheChange());
		pCacheChange			cc_02		= pCacheChange(new CacheChange());
		pCacheChange			cc_03		= pCacheChange(new CacheChange());
		pCacheChange			cc_04		= pCacheChange(new CacheChange());
		pCacheChange			cc_05		= pCacheChange(new CacheChange());
		pCacheChange			cc_06		= pCacheChange(new CacheChange());
		pCacheChange			cc_07		= pCacheChange(new CacheChange());
		pCacheChange			cc_08		= pCacheChange(new CacheChange());
		pCacheChange			cc_09		= pCacheChange(new CacheChange());
#else
		pCacheChange			cc_00		= new CacheChange();
		pCacheChange			cc_01		= new CacheChange();
		pCacheChange			cc_02		= new CacheChange();
		pCacheChange			cc_03		= new CacheChange();
		pCacheChange			cc_04		= new CacheChange();
		pCacheChange			cc_05		= new CacheChange();
		pCacheChange			cc_06		= new CacheChange();
		pCacheChange			cc_07		= new CacheChange();
		pCacheChange			cc_08		= new CacheChange();
		pCacheChange			cc_09		= new CacheChange();
#endif

		cc_00->sequence_number	= SequenceNumber_t(0, 1);
		cc_01->sequence_number	= SequenceNumber_t(0, 2);
		cc_02->sequence_number	= SequenceNumber_t(0, 3);
		cc_03->sequence_number	= SequenceNumber_t(0, 4);
		cc_04->sequence_number	= SequenceNumber_t(0, 5);
		cc_05->sequence_number	= SequenceNumber_t(0, 6);
		cc_06->sequence_number	= SequenceNumber_t(0, 7);
		cc_07->sequence_number	= SequenceNumber_t(0, 8);
		cc_08->sequence_number	= SequenceNumber_t(0, 9);
		cc_09->sequence_number	= SequenceNumber_t(0, 10);

		cfr_00->is_relevant	= true;
		cfr_01->is_relevant	= true;
		cfr_02->is_relevant	= true;
		cfr_03->is_relevant	= true;
		cfr_04->is_relevant	= true;
		cfr_05->is_relevant	= true;
		cfr_06->is_relevant	= true;
		cfr_07->is_relevant	= true;
		cfr_08->is_relevant	= true;
		cfr_09->is_relevant	= true;

		cfr_00->cache_change	= cc_00;
		cfr_01->cache_change	= cc_01;
		cfr_02->cache_change	= cc_02;
		cfr_03->cache_change	= cc_03;
		cfr_04->cache_change	= cc_04;
		cfr_05->cache_change	= cc_05;
		cfr_06->cache_change	= cc_06;
		cfr_07->cache_change	= cc_07;
		cfr_08->cache_change	= cc_08;
		cfr_09->cache_change	= cc_09;

		cfr_00->status	= CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED;
		cfr_01->status	= CHANGE_FOR_READER_STATUS_KIND_REQUESTED;
		cfr_02->status	= CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED;
		cfr_03->status	= CHANGE_FOR_READER_STATUS_KIND_UNDERWAY;
		cfr_04->status	= CHANGE_FOR_READER_STATUS_KIND_UNSENT;
		cfr_05->status	= CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED;
		cfr_06->status	= CHANGE_FOR_READER_STATUS_KIND_REQUESTED;
		cfr_07->status	= CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED;
		cfr_08->status	= CHANGE_FOR_READER_STATUS_KIND_UNDERWAY;
		cfr_09->status	= CHANGE_FOR_READER_STATUS_KIND_UNSENT;

		std::cout << "************ Status Test 001 ************" << std::endl;

		for(int i=0 ;i<2; i++)
		{
/*
			cfr_list->insertValue(cfr_00);
			cfr_list->insertValue(cfr_01);
			cfr_list->insertValue(cfr_02);
			cfr_list->insertValue(cfr_03);
			cfr_list->insertValue(cfr_04);
			cfr_list->insertValue(cfr_05);
			cfr_list->insertValue(cfr_06);
			cfr_list->insertValue(cfr_07);
			cfr_list->insertValue(cfr_08);
			cfr_list->insertValue(cfr_09);
*/

			cfr_list->insertValue(cfr_00);
			cfr_list->insertValue(cfr_02);
			cfr_list->insertValue(cfr_04);
			cfr_list->insertValue(cfr_06);
			cfr_list->insertValue(cfr_08);

			cfr_list->insertValue(cfr_01);
			cfr_list->insertValue(cfr_03);
			cfr_list->insertValue(cfr_05);
			cfr_list->insertValue(cfr_07);
			cfr_list->insertValue(cfr_09);

			cfr	= cfr_list->popFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED);
			if(cfr)
			{
				std::cout << "popFirstValueByStatus : " << cfr->cache_change->sequence_number.low -1 << std::endl;
			}
			else
			{
				std::cout << "popFirstValueByStatus : Fail" << std::endl;
			}

			cfr	= cfr_list->popFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED);
			if(cfr)
			{
				std::cout << "popFirstValueByStatus : " << cfr->cache_change->sequence_number.low -1 << std::endl;
			}
			else
			{
				std::cout << "popFirstValueByStatus : Fail" << std::endl;
			}

			cfr	= cfr_list->popFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_REQUESTED);
			if(cfr)
			{
				std::cout << "popFirstValueByStatus : " << cfr->cache_change->sequence_number.low -1 << std::endl;
			}
			else
			{
				std::cout << "popFirstValueByStatus : Fail" << std::endl;
			}

			cfr	= cfr_list->popFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_REQUESTED);
			if(cfr)
			{
				std::cout << "popFirstValueByStatus : " << cfr->cache_change->sequence_number.low -1 << std::endl;
			}
			else
			{
				std::cout << "popFirstValueByStatus : Fail" << std::endl;
			}

			cfr	= cfr_list->popFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED);
			if(cfr)
			{
				std::cout << "popFirstValueByStatus : " << cfr->cache_change->sequence_number.low -1 << std::endl;
			}
			else
			{
				std::cout << "popFirstValueByStatus : Fail" << std::endl;
			}

			cfr	= cfr_list->popFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED);
			if(cfr)
			{
				std::cout << "popFirstValueByStatus : " << cfr->cache_change->sequence_number.low -1 << std::endl;
			}
			else
			{
				std::cout << "popFirstValueByStatus : Fail" << std::endl;
			}

			cfr	= cfr_list->popFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNDERWAY);
			if(cfr)
			{
				std::cout << "popFirstValueByStatus : " << cfr->cache_change->sequence_number.low -1 << std::endl;
			}
			else
			{
				std::cout << "popFirstValueByStatus : Fail" << std::endl;
			}

			cfr	= cfr_list->popFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNDERWAY);
			if(cfr)
			{
				std::cout << "popFirstValueByStatus : " << cfr->cache_change->sequence_number.low -1 << std::endl;
			}
			else
			{
				std::cout << "popFirstValueByStatus : Fail" << std::endl;
			}

			cfr	= cfr_list->popFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNSENT);
			if(cfr)
			{
				std::cout << "popFirstValueByStatus : " << cfr->cache_change->sequence_number.low -1 << std::endl;
			}
			else
			{
				std::cout << "popFirstValueByStatus : Fail" << std::endl;
			}

			cfr	= cfr_list->popFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNSENT);
			if(cfr)
			{
				std::cout << "popFirstValueByStatus : " << cfr->cache_change->sequence_number.low -1 << std::endl;
			}
			else
			{
				std::cout << "popFirstValueByStatus : Fail" << std::endl;
			}

			cfr_list->refresh();

			std::cout << std::endl;
		}

		std::cout << "************ Status Test 002 ************" << std::endl;

		cfr_list->insertValue(cfr_00);
		cfr_list->insertValue(cfr_01);
		cfr_list->insertValue(cfr_02);
		cfr_list->insertValue(cfr_03);
		cfr_list->insertValue(cfr_04);
		cfr_list->insertValue(cfr_05);
		cfr_list->insertValue(cfr_06);
		cfr_list->insertValue(cfr_07);
		cfr_list->insertValue(cfr_08);
		cfr_list->insertValue(cfr_09);

		for(int i =0 ; i< 10; i++)
		{
			cfr	= cfr_list->getValueByIndex(i);
			if(cfr)
			{
				std::cout << "popFirstValueByStatus : " << cfr->cache_change->sequence_number.low -1 << std::endl;
			}
			else
			{
				std::cout << "popFirstValueByStatus : Fail" << std::endl;
			}
		}

		cfr_list->refresh();
		std::cout << std::endl;

		std::cout << "************ Status Test 003 ************" << std::endl;

		cfr_list->insertValue(cfr_00);
		cfr_list->insertValue(cfr_02);
		cfr_list->insertValue(cfr_04);
		cfr_list->insertValue(cfr_06);
		cfr_list->insertValue(cfr_08);

		cfr_list->insertValue(cfr_01);
		cfr_list->insertValue(cfr_03);
		cfr_list->insertValue(cfr_05);
		cfr_list->insertValue(cfr_07);
		cfr_list->insertValue(cfr_09);

		cfr_list->removeByIndex(0);
		cfr_list->removeByIndex(1);
		cfr_list->removeByIndex(2);
		cfr_list->removeBySequenceNumber(SequenceNumber_t(0, 4));
		cfr_list->removeBySequenceNumber(SequenceNumber_t(0, 5));

		cfr_list->refresh();

		for(int i =0 ; i< 10; i++)
		{
			cfr	= cfr_list->getValueByIndex(i);
			if(cfr)
			{
				std::cout << "popFirstValueByStatus : " << cfr->cache_change->sequence_number.low -1 << std::endl;
			}
			else
			{
				std::cout << "popFirstValueByStatus : Fail" << std::endl;
			}
		}

		cfr_list->refresh();
		std::cout << std::endl;
	}

	void	ChangeForReaderListTest_OverInsert()
	{
		ChangeForReaderList*	cfr_list	= new ChangeForReaderList(10);
		int						i			= 0;

		ChangeForReader*		cfr			= NULL;
		pCacheChange			change		= NULL;

		printf("ChangeForReaderListTest_OverInsert()\n");
		printf("\n");

		printf("Insertion start\n");
		for(i=1; i<11; i++)
		{
#ifdef SHARED_PTR_CACHE_CHANGE
			change	= SHARED_PTR_TYPE_NATIVE<CacheChange>(new CacheChange());
#else
			change	= new CacheChange();
#endif
			change->sequence_number.high	= 0;
			change->sequence_number.low		= i;

			cfr					= new ChangeForReader();
			cfr->cache_change	= change;
			if(i<11)
			{
				cfr->status			= CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED;
			}
			else
			{
				cfr->status			= CHANGE_FOR_READER_STATUS_KIND_UNSENT;
			}
			cfr->is_relevant	= true;

			if(!cfr_list->insertValue(cfr))
			{
				printf("\tcfr_list is full\n");
			}
		}
		printf("Insertion end\n");
		printf("ValidSize(%d)\n", cfr_list->getValidSize());
		printf("\n");

		printf("Over insertion start\n");
		for(i=11; i<16; i++)
		{
#ifdef SHARED_PTR_CACHE_CHANGE
			change	= SHARED_PTR_TYPE_NATIVE<CacheChange>(new CacheChange());
#else
			change	= new CacheChange();
#endif
			change->sequence_number.high	= 0;
			change->sequence_number.low		= i;

			cfr					= new ChangeForReader();
			cfr->cache_change	= change;
			cfr->status			= CHANGE_FOR_READER_STATUS_KIND_UNSENT;
			cfr->is_relevant	= true;

			if(!cfr_list->insertValue(cfr))
			{
				printf("\tcfr_list is full\n");
				
				delete(cfr);
#ifdef SHARED_PTR_CACHE_CHANGE
				change.reset();
#else
				delete(change);
#endif
			}
			else
			{
				printf("ERROR\n");
			}
		}
		printf("Over insertion end\n");
		printf("ValidSize(%d)\n", cfr_list->getValidSize());
		printf("\n");

		printf("Print cfr_list\n");
		for(i=0; i<10; i++)
		{
			cfr	= cfr_list->getValueByIndex(i);
			if(cfr)
			{
				printf("[%d]cfr->status is (%d, %d)(s:%d)\n", i, cfr->cache_change->sequence_number.high, cfr->cache_change->sequence_number.low, cfr->status);
			}
			else
			{
				printf("[%d]cfr is NULL\n", i);
			}
		}

		if(cfr_list->refreshByStatus())
		{
			printf("cfr_list->refreshByStatus() is SUCCESS\n");
		}
		else
		{
			printf("cfr_list->refreshByStatus() is FAILED");
		}
		printf("ValidSize(%d)\n", cfr_list->getValidSize());
		printf("\n");

		printf("cfr_list->getBaseNumber() (%d, %d)\n", cfr_list->getBaseNumber().high, cfr_list->getBaseNumber().low);
		printf("\n");

		printf("Over insertion start\n");
		for(i=11; i<16; i++)
		{
#ifdef SHARED_PTR_CACHE_CHANGE
			change	= SHARED_PTR_TYPE_NATIVE<CacheChange>(new CacheChange());
#else
			change	= new CacheChange();
#endif
			change->sequence_number.high	= 0;
			change->sequence_number.low		= i;

			cfr					= new ChangeForReader();
			cfr->cache_change	= change;
			cfr->status			= CHANGE_FOR_READER_STATUS_KIND_UNSENT;
			cfr->is_relevant	= true;

			if(!cfr_list->insertValue(cfr))
			{
				printf("\tcfr_list is full\n");

				delete(cfr);
#ifdef SHARED_PTR_CACHE_CHANGE
				change.reset();
#else
				delete(change);
#endif
			}
			else
			{
				
			}
		}
		printf("ValidSize(%d)\n", cfr_list->getValidSize());
		printf("Over insertion end\n");
		printf("\n");

		printf("Print cfr_list\n");
		for(i=0; i<10; i++)
		{
			cfr	= cfr_list->getValueByIndex(i);
			if(cfr)
			{
				printf("[%d]cfr->status is (%d, %d)(s:%d)\n", i, cfr->cache_change->sequence_number.high, cfr->cache_change->sequence_number.low, cfr->status);
			}
			else
			{
				printf("[%d]cfr is NULL\n", i);
			}
		}
		printf("ValidSize(%d)\n", cfr_list->getValidSize());
	}

	void	SharedPtrListTest()
	{
		SharedPtrList<long>	ptr_list;
		long*	long_0	= new long(12);
		long*	long_1	= new long(56);
		long*	long_2	= new long(34);
		long*	long_3	= new long(78);

		SHARED_PTR_TYPE_NATIVE<long>	ptr_long_0(long_0);
		SHARED_PTR_TYPE_NATIVE<long>	ptr_long_1(long_1);
		SHARED_PTR_TYPE_NATIVE<long>	ptr_long_2(long_2);
		SHARED_PTR_TYPE_NATIVE<long>	ptr_long_3(long_3);
		SHARED_PTR_TYPE_NATIVE<long>	ptr_long_4;

		std::cout << "create" << std::endl;
		std::cout << ptr_long_0.use_count() << std::endl;
		std::cout << ptr_long_1.use_count() << std::endl;
		std::cout << ptr_long_2.use_count() << std::endl;
		std::cout << ptr_long_3.use_count() << std::endl;
		std::cout << std::endl;

		//ptr_list.insertInRear(ptr_long_0);
		ptr_list.insertAscendingInRear(ptr_long_0);
		ptr_list.insertAscendingInRear(ptr_long_1);
		ptr_list.insertAscendingInRear(ptr_long_2);
		ptr_list.insertAscendingInRear(ptr_long_3);

		std::cout << "insertInRear" << std::endl;
		std::cout << ptr_long_0.use_count() << std::endl;
		std::cout << ptr_long_1.use_count() << std::endl;
		std::cout << ptr_long_2.use_count() << std::endl;
		std::cout << ptr_long_3.use_count() << std::endl;
		std::cout << std::endl;

		ptr_long_4	= ptr_list.getNodeByIndex(1)->value;
		std::cout << "value == " << *ptr_long_4 << std::endl;
		ptr_long_4	= ptr_list.getNodeByIndex(2)->value;
		std::cout << "value == " << *ptr_long_4 << std::endl;
		ptr_long_4	= ptr_list.getNodeByIndex(3)->value;
		std::cout << "value == " << *ptr_long_4 << std::endl;
		ptr_long_4	= ptr_list.getNodeByIndex(4)->value;
		std::cout << "value == " << *ptr_long_4 << std::endl;
		std::cout << std::endl;

		std::cout << "ptr_list.getNodeByIndex(1)->value" << std::endl;
		std::cout << ptr_long_0.use_count() << std::endl;
		std::cout << ptr_long_1.use_count() << std::endl;
		std::cout << ptr_long_2.use_count() << std::endl;
		std::cout << ptr_long_3.use_count() << std::endl;
		std::cout << std::endl;

		ptr_long_0.reset();
		ptr_long_4	= ptr_list.popFirst();

		std::cout << "ptr_list.popFirst()" << std::endl;
		std::cout << ptr_long_0.use_count() << std::endl;
		std::cout << ptr_long_1.use_count() << std::endl;
		std::cout << ptr_long_2.use_count() << std::endl;
		std::cout << ptr_long_3.use_count() << std::endl;
		std::cout << ptr_long_4.use_count() << std::endl;
		std::cout << std::endl;

		std::cout << long_0 << *long_0 << std::endl;
		ptr_long_4.reset();
		std::cout << long_0 << *long_0 << std::endl;
	}

	void	ChangeForReaderListPerformanceTest()
	{
		_dual_list_test();
		printf("\n");

		//_array_test();
		//printf("\n");

		//_linked_list_test();
		//printf("\n");

		//_status_list_test();
		//printf("\n");
	}

	void	_dual_list_test()
	{
		ChangeForReaderList*	cfr_list	= new ChangeForReaderList(CFR_LIST_TEST_SIZE);
		ChangeForReader**		cfr			= NULL;
		CacheChange**			change		= NULL;
		int						i			= 1;
		ChangeForReader*		cur_cfr		= NULL;

		LARGE_INTEGER	time_begin;
		LARGE_INTEGER	time_end;
		INT64			time_calc;
		double			result;

		int						size_ack	= 0;
		int						size_req	= 0;
		int						size_unack	= 0;
		int						size_under	= 0;
		int						size_unsen	= 0;

		cfr		= (ChangeForReader**)malloc(sizeof(ChangeForReader*)*CFR_LIST_TEST_SIZE);
		change	= (CacheChange**)malloc(sizeof(CacheChange*)*CFR_LIST_TEST_SIZE);

		for(i=0; i<CFR_LIST_TEST_SIZE; i++)
		{
			change[i]	= new CacheChange();
			change[i]->sequence_number	= SequenceNumber_t(0, i+1);

			cfr[i]		= new ChangeForReader();
			cfr[i]->cache_change	= change[i];
			cfr[i]->is_relevant		= true;

			switch(i%5)
			{
			case 0:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED;
				size_ack++;
				break;
			case 1:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_REQUESTED;
				size_req++;
				break;
			case 2:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED;
				size_unack++;
				break;
			case 3:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_UNDERWAY;
				size_under++;
				break;
			case 4:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_UNSENT;
				size_unsen++;
				break;
			}
		}
		printf("[DUAL_LIST_TEST]\n");

/////////////////////////////////////////////////////////////////////////////////////////////
//	TIMER SETUP
/////////////////////////////////////////////////////////////////////////////////////////////
		LARGE_INTEGER	freq;
		QueryPerformanceFrequency(&freq);
		//printf("\tFREQUENCY: %lld\n", freq);

/////////////////////////////////////////////////////////////////////////////////////////////
//	[DUAL_LIST] INSERTION TIME
/////////////////////////////////////////////////////////////////////////////////////////////
		//time_begin	= get_current_time();
		QueryPerformanceCounter(&time_begin);
		for(i=0; i<CFR_LIST_TEST_SIZE; i++)
		{
			cfr_list->insertValue(cfr[i]);
		}
		//time_end	= get_current_time();
		//time_calc	= time_end	- time_begin;
		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tINSERTION TIME: %lf\n", result);
		
/////////////////////////////////////////////////////////////////////////////////////////////
//	[DUAL_LIST] SEQUENCIAL ACCESS TIME 
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);

		for(i=1; i<=CFR_LIST_TEST_SIZE; i++)
		{
			cur_cfr	= cfr_list->getValueByIndex(i);
		}

		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tSEQUENCIAL ACCESS TIME: %lf\n", result);

/////////////////////////////////////////////////////////////////////////////////////////////
//	[DUAL_LIST] STATUS ACCESS TIME 
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);
		/*
		for(i=0; i<10000; i++)
		{
			cur_cfr	= cfr_list->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED);
		}
		*/
		for(i=0; i<size_ack; i++)
		{
			cfr_list->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED);
		}
		for(i=0; i<size_req; i++)
		{
			cfr_list->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_REQUESTED);
		}
		for(i=0; i<size_unack; i++)
		{
			cfr_list->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED);
		}
		for(i=0; i<size_under; i++)
		{
			cfr_list->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNDERWAY);
		}
		for(i=0; i<size_unsen; i++)
		{
			cfr_list->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNSENT);
		}

		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tSTATUS ACCESS TIME: %lf\n", result);

/////////////////////////////////////////////////////////////////////////////////////////////
//	[DUAL_LIST] STATUS CHANGE TIME 
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);

		for(i=0; i<CFR_LIST_TEST_SIZE; i++)
		{
			switch(i)
			{
			case 4:
				cfr_list->changeStatusByIndex(i, CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED);
				break;
			default:
				cfr_list->changeStatusByIndex(i, CHANGE_FOR_READER_STATUS_KIND_UNSENT);
				break;
			}
		}

		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tSTATUS CHANGE TIME: %lf\n", result);

/////////////////////////////////////////////////////////////////////////////////////////////
//	[DUAL_LIST] REMOVAL TIME
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);

		for(i=0; i<CFR_LIST_TEST_SIZE; i++)
		{
			cfr_list->removeByIndex(i);
		}

		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tREMOVAL TIME: %lf\n", result);
	}

	void	_array_test()
	{
		
		ChangeForReader**		cfr			= NULL;
		CacheChange**			change		= NULL;
		int						i			= 1;
		ChangeForReader*		cur_cfr		= NULL;

		LARGE_INTEGER	time_begin;
		LARGE_INTEGER	time_end;
		INT64			time_calc;
		double			result;

		cfr		= (ChangeForReader**)malloc(sizeof(ChangeForReader*)*CFR_LIST_TEST_SIZE);
		change	= (CacheChange**)malloc(sizeof(CacheChange*)*CFR_LIST_TEST_SIZE);

		for(i=0; i<CFR_LIST_TEST_SIZE; i++)
		{
			change[i]	= new CacheChange();
			change[i]->sequence_number	= SequenceNumber_t(0, i+1);

			cfr[i]		= new ChangeForReader();
			cfr[i]->cache_change	= change[i];
			cfr[i]->is_relevant		= true;

			switch(i%5)
			{
			case 0:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED;
				break;
			case 1:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_REQUESTED;
				break;
			case 2:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED;
				break;
			case 3:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_UNDERWAY;
				break;
			case 4:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_UNSENT;
				break;
			}
		}
		printf("[ARRAY_TEST]\n");

/////////////////////////////////////////////////////////////////////////////////////////////
//	TIMER SETUP
/////////////////////////////////////////////////////////////////////////////////////////////
		LARGE_INTEGER	freq;
		QueryPerformanceFrequency(&freq);
		//printf("\tFREQUENCY: %lld\n", freq);

/////////////////////////////////////////////////////////////////////////////////////////////
//	[ARRAY] INSERTION TIME
/////////////////////////////////////////////////////////////////////////////////////////////
		ChangeForReader*	cfr_arr[CFR_LIST_TEST_SIZE];
		MUTEX_TYPE_NATIVE	mutex;
		create_mutex(&mutex);

		QueryPerformanceCounter(&time_begin);
		for(i=0;i<CFR_LIST_TEST_SIZE; i++)
		{
			mutex_lock(&mutex);
			cfr_arr[i]	= cfr[i];
			mutex_unlock(&mutex);
		}
		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tINSERTION TIME: %lf\n", result);
		
/////////////////////////////////////////////////////////////////////////////////////////////
//	[ARRAY] SEQUENCIAL ACCESS TIME 
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);

		for(i=0; i<CFR_LIST_TEST_SIZE; i++)
		{
			mutex_lock(&mutex);
			cur_cfr	= cfr_arr[i];
			mutex_unlock(&mutex);
		}

		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tSEQUENCIAL ACCESS TIME: %lf\n", result);

/////////////////////////////////////////////////////////////////////////////////////////////
//	[ARRAY] STATUS ACCESS TIME 
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);

		for(i=0; i<CFR_LIST_TEST_SIZE; i++)
		{
			mutex_lock(&mutex);

			if(cfr_arr[i]->status != 0)
			{
				cfr_arr[i]->status	= 0;
				i	= 0;
			}

			mutex_unlock(&mutex);
		}

		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tSTATUS ACCESS TIME: %lf\n", result);

/////////////////////////////////////////////////////////////////////////////////////////////
//	[ARRAY] STATUS CHANGE TIME 
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);

		for(i=0; i<CFR_LIST_TEST_SIZE; i++)
		{
			mutex_lock(&mutex);

			if(cfr_arr[i]->status != 0)
			{
				cfr_arr[i]->status	= 0;
			}

			mutex_unlock(&mutex);
		}

		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tSTATUS CHANGE TIME: %lf\n", result);

/////////////////////////////////////////////////////////////////////////////////////////////
//	[ARRAY] REMOVAL TIME
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);

		for(i=0;i<CFR_LIST_TEST_SIZE; i++)
		{
			mutex_lock(&mutex);
			cfr_arr[i]	= NULL;
			mutex_unlock(&mutex);
		}

		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tREMOVAL TIME: %lf\n", result);
	}

	void	_linked_list_test()
	{
		DoublyLinkedList<ChangeForReader>*	cfr_list	= new DoublyLinkedList<ChangeForReader>();
		ChangeForReader**					cfr			= NULL;
		CacheChange**						change		= NULL;
		int									i			= 1;
		ChangeForReader*					cur_cfr		= NULL;

		LARGE_INTEGER	time_begin;
		LARGE_INTEGER	time_end;
		INT64			time_calc;
		double			result;

		cfr		= (ChangeForReader**)malloc(sizeof(ChangeForReader*)*CFR_LIST_TEST_SIZE);
		change	= (CacheChange**)malloc(sizeof(CacheChange*)*CFR_LIST_TEST_SIZE);

		for(i=0; i<CFR_LIST_TEST_SIZE; i++)
		{
			change[i]	= new CacheChange();
			change[i]->sequence_number	= SequenceNumber_t(0, i+1);

			cfr[i]		= new ChangeForReader();
			cfr[i]->cache_change	= change[i];
			cfr[i]->is_relevant		= true;

			switch(i%5)
			{
			case 0:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED;
				break;
			case 1:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_REQUESTED;
				break;
			case 2:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED;
				break;
			case 3:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_UNDERWAY;
				break;
			case 4:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_UNSENT;
				break;
			}
		}
		printf("[LINKED_LIST_TEST]\n");

/////////////////////////////////////////////////////////////////////////////////////////////
//	TIMER SETUP
/////////////////////////////////////////////////////////////////////////////////////////////
		LARGE_INTEGER	freq;
		QueryPerformanceFrequency(&freq);
		//printf("\tFREQUENCY: %lld\n", freq);

/////////////////////////////////////////////////////////////////////////////////////////////
//	[LINKED LIST] INSERTION TIME
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);

		for(i=0;i<CFR_LIST_TEST_SIZE; i++)
		{
			cfr_list->insertInRear(cfr[i]);
		}

		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tINSERTION TIME: %lf\n", result);

/////////////////////////////////////////////////////////////////////////////////////////////
//	[LINKED LIST] SEQUENCIAL ACCESS TIME 
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);

		for(i=0; i<CFR_LIST_TEST_SIZE; i++)
		{
			cfr_list->getNodeByIndex(i+1);
		}

		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tSEQUENCIAL ACCESS TIME: %lf\n", result);

/////////////////////////////////////////////////////////////////////////////////////////////
//	[LINKED LIST] STATUS ACCESS TIME 
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);
		
		for(i=1; i<=CFR_LIST_TEST_SIZE; i++)
		{
			cur_cfr	= cfr_list->getNodeByIndex(i)->value;
			if(cur_cfr->status != 0)
			{
				cur_cfr->status	= 0;
				i	= 1;
			}
		}
		
		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tSTATUS ACCESS TIME : %lf\n", result);
		
/////////////////////////////////////////////////////////////////////////////////////////////
//	[LINKED LIST] STATUS CHANGE TIME 
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//	[LINKED LIST] REMOVAL TIME
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);

		while(cfr_list->popFirst());

		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tREMOVAL TIME: %lf\n", result);
	}
	
	void	_status_list_test()
	{
		DoublyLinkedList<ChangeForReader>*	cfr_list_ack	= new DoublyLinkedList<ChangeForReader>();
		DoublyLinkedList<ChangeForReader>*	cfr_list_req	= new DoublyLinkedList<ChangeForReader>();
		DoublyLinkedList<ChangeForReader>*	cfr_list_unack	= new DoublyLinkedList<ChangeForReader>();
		DoublyLinkedList<ChangeForReader>*	cfr_list_under	= new DoublyLinkedList<ChangeForReader>();
		DoublyLinkedList<ChangeForReader>*	cfr_list_unsen	= new DoublyLinkedList<ChangeForReader>();
		ChangeForReader**					cfr			= NULL;
		CacheChange**						change		= NULL;
		int									i			= 1;
		ChangeForReader*					cur_cfr		= NULL;

		int						size_ack	= 0;
		int						size_req	= 0;
		int						size_unack	= 0;
		int						size_under	= 0;
		int						size_unsen	= 0;

		LARGE_INTEGER	time_begin;
		LARGE_INTEGER	time_end;
		INT64			time_calc;
		double			result;

		cfr		= (ChangeForReader**)malloc(sizeof(ChangeForReader*)*CFR_LIST_TEST_SIZE);
		change	= (CacheChange**)malloc(sizeof(CacheChange*)*CFR_LIST_TEST_SIZE);

		for(i=0; i<CFR_LIST_TEST_SIZE; i++)
		{
			change[i]	= new CacheChange();
			change[i]->sequence_number	= SequenceNumber_t(0, i+1);

			cfr[i]		= new ChangeForReader();
			cfr[i]->cache_change	= change[i];
			cfr[i]->is_relevant		= true;

			switch(i%5)
			{
			case 0:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED;
				size_ack++;
				break;
			case 1:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_REQUESTED;
				size_req++;
				break;
			case 2:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED;
				size_unack++;
				break;
			case 3:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_UNDERWAY;
				size_under++;
				break;
			case 4:
				cfr[i]->status			= CHANGE_FOR_READER_STATUS_KIND_UNSENT;
				size_unsen++;
				break;
			}
		}
		printf("[STATUS_LIST_TEST]\n");

/////////////////////////////////////////////////////////////////////////////////////////////
//	TIMER SETUP
/////////////////////////////////////////////////////////////////////////////////////////////
		LARGE_INTEGER	freq;
		QueryPerformanceFrequency(&freq);
		//printf("\tFREQUENCY: %lld\n", freq);

/////////////////////////////////////////////////////////////////////////////////////////////
//	[STATUS LIST] INSERTION TIME
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);

		for(i=0;i<CFR_LIST_TEST_SIZE; i++)
		{
			switch(i%5)
			{
			case 0:
				cfr_list_ack->insertInRear(cfr[i]);
				break;
			case 1:
				cfr_list_req->insertInRear(cfr[i]);
				break;
			case 2:
				cfr_list_unack->insertInRear(cfr[i]);
				break;
			case 3:
				cfr_list_under->insertInRear(cfr[i]);
				break;
			case 4:
				cfr_list_unsen->insertInRear(cfr[i]);
				break;
			}
		}

		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tINSERTION TIME: %lf\n", result);

/////////////////////////////////////////////////////////////////////////////////////////////
//	[STATUS LIST] SEQUENCIAL ACCESS TIME 
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);

		int		j			= 0;
		bool	isFounded	= false;
		Node<ChangeForReader>*	cur_node;
		for(i=1; i<=CFR_LIST_TEST_SIZE; i++)
		{
			isFounded	= false;

			cur_node	= cfr_list_ack->getNodeByIndex(1);
			cur_cfr		= cur_node->value;
			for(j=1; j<cfr_list_ack->getSize(); j++)
			{
				if(cur_cfr->cache_change->sequence_number.low == i)
				{
					isFounded	= true;
					break;
				}
				else if(cur_cfr->cache_change->sequence_number.low > i)
				{
					break;
				}
				else
				{
					cur_node	= cur_node->rear;
					cur_cfr		= cur_node->value;
				}
			}

			if(isFounded == false)
			{
				cur_node	= cfr_list_unack->getNodeByIndex(1);
				cur_cfr		= cur_node->value;

				for(j=1; j<cfr_list_unack->getSize(); j++)
				{
					if(cur_cfr->cache_change->sequence_number.low == i)
					{
						isFounded	= true;
						break;
					}
					else if(cur_cfr->cache_change->sequence_number.low > i)
					{
						break;
					}
					else
					{
						cur_node	= cur_node->rear;
						cur_cfr		= cur_node->value;
					}
				}
			}

			if(isFounded == false)
			{
				cur_node	= cfr_list_req->getNodeByIndex(1);
				cur_cfr		= cur_node->value;

				for(j=1; j<cfr_list_req->getSize(); j++)
				{
					if(cur_cfr->cache_change->sequence_number.low == i)
					{
						isFounded	= true;
						break;
					}
					else if(cur_cfr->cache_change->sequence_number.low > i)
					{
						break;
					}
					else
					{
						cur_node	= cur_node->rear;
						cur_cfr		= cur_node->value;
					}
				}
			}

			if(isFounded == false)
			{
				cur_node	= cfr_list_under->getNodeByIndex(1);
				cur_cfr		= cur_node->value;

				for(j=1; j<cfr_list_under->getSize(); j++)
				{
					if(cur_cfr->cache_change->sequence_number.low == i)
					{
						isFounded	= true;
						break;
					}
					else if(cur_cfr->cache_change->sequence_number.low > i)
					{
						break;
					}
					else
					{
						cur_node	= cur_node->rear;
						cur_cfr		= cur_node->value;
					}
				}
			}

			if(isFounded == false)
			{
				cur_node	= cfr_list_unsen->getNodeByIndex(1);
				cur_cfr		= cur_node->value;

				for(j=1; j<cfr_list_unsen->getSize(); j++)
				{
					if(cur_cfr->cache_change->sequence_number.low == i)
					{
						isFounded	= true;
						break;
					}
					else if(cur_cfr->cache_change->sequence_number.low > i)
					{
						break;
					}
					else
					{
						cur_node	= cur_node->rear;
						cur_cfr		= cur_node->value;
					}
				}
			}
		}

		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tSEQUENCIAL ACCESS TIME: %lf\n", result);

/////////////////////////////////////////////////////////////////////////////////////////////
//	[STATUS LIST] STATUS ACCESS TIME 
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);

		for(i=0; i<size_ack; i++)
		{
			cfr_list_ack->getFirst();
		}
		for(i=0; i<size_req; i++)
		{
			cfr_list_req->getFirst();
		}
		for(i=0; i<size_unack; i++)
		{
			cfr_list_unack->getFirst();
		}
		for(i=0; i<size_under; i++)
		{
			cfr_list_under->getFirst();
		}
		for(i=0; i<size_unsen; i++)
		{
			cfr_list_unsen->getFirst();
		}

		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tSTATUS ACCESS TIME: %lf\n", result);

/////////////////////////////////////////////////////////////////////////////////////////////
//	[STATUS LIST] STATUS CHANGE TIME 
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);

		for(i=0; i<size_ack; i++)
		{
			cur_cfr	= cfr_list_ack->popFirst();
			cfr_list_unsen->insertInRear(cur_cfr);
		}
		for(i=0; i<size_req; i++)
		{
			cur_cfr	= cfr_list_req->popFirst();
			cfr_list_unsen->insertInRear(cur_cfr);
		}
		for(i=0; i<size_unack; i++)
		{
			cur_cfr	= cfr_list_unack->popFirst();
			cfr_list_unsen->insertInRear(cur_cfr);
		}
		for(i=0; i<size_under; i++)
		{
			cur_cfr	= cfr_list_under->popFirst();
			cfr_list_unsen->insertInRear(cur_cfr);
		}
		for(i=0; i<size_unsen; i++)
		{
			cur_cfr	= cfr_list_unsen->popFirst();
			cfr_list_unsen->insertInRear(cur_cfr);
		}

		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tSTATUS CHANGE TIME: %lf\n", result);

/////////////////////////////////////////////////////////////////////////////////////////////
//	[STATUS LIST] REMOVAL TIME
/////////////////////////////////////////////////////////////////////////////////////////////
		QueryPerformanceCounter(&time_begin);

		while(cfr_list_ack->getSize())
		{
			cfr_list_ack->popFirst();
		}
		while(cfr_list_unack->getSize())
		{
			cfr_list_unack->popFirst();
		}
		while(cfr_list_under->getSize())
		{
			cfr_list_under->popFirst();
		}
		while(cfr_list_unsen->getSize())
		{
			cfr_list_unsen->popFirst();
		}
		while(cfr_list_req->getSize())
		{
			cfr_list_req->popFirst();
		}

		QueryPerformanceCounter(&time_end);
		time_calc	= time_end.QuadPart	- time_begin.QuadPart;
		result		= (double)time_calc/(double)freq.QuadPart;
		printf("\tREMOVAL TIME: %lf\n", result);
	}
}
