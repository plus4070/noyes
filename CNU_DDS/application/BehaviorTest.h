#ifndef APPLICATION_BEHAVIORTEST_H_
#define APPLICATION_BEHAVIORTEST_H_

namespace CNU_DDS
{
	void	BehaviorTest();
	void	StatelessWriterTest();
	void	StatelessReaderTest();
	void	StatefulWriterTest();
	void	StatefulReaderTest();

	void*	create_PDP_message();
	void*	create_Pub_EDP_message();
	void*	create_Sub_EDP_message();
}


#endif /* APPLICATION_BEHAVIORTEST_H_ */
