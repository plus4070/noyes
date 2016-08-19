#ifndef APPLICATION_LISTTEST_H_
#define APPLICATION_LISTTEST_H_

#include "../include/RTPS/Behavior/DataStructure/ChangeForReaderList.h"

namespace CNU_DDS
{
	#define	CFR_LIST_TEST_SIZE	4000

	void	ChangeForReaderListTest();
	void	ChangeForReaderListTest_OverInsert();
	void	SharedPtrListTest();

	void	ChangeForReaderListPerformanceTest();
	void	_dual_list_test();
	void	_array_test();
	void	_linked_list_test();
	void	_status_list_test();
}

#endif /* APPLICATION_LISTTEST_H_ */
