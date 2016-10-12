#ifndef INCLUDE_UTILITY_TIMER_LISTHEADER_H_
#define INCLUDE_UTILITY_TIMER_LISTHEADER_H_

namespace CNU_DDS
{
class ListHeader
{
	private:
		ListHeader*	prev;
		ListHeader*	next;

	public:
		static void	listReplaceInit(ListHeader* oldList, ListHeader* newList);

	public:
		ListHeader();
		ListHeader(ListHeader* a_Prev, ListHeader* a_Next);
		~ListHeader();

		void		setPrev(ListHeader* a_Prev);
		void		setNext(ListHeader* a_Next);

		ListHeader*	getPrev();
		ListHeader*	getNext();

		void		disconnectThis();
		ListHeader*	disconnectMeaningfulList();

		bool		isEmpty();
		void		list_add_tail(ListHeader* newList);
	};
}


#endif /* INCLUDE_UTILITY_TIMER_LISTHEADER_H_ */
