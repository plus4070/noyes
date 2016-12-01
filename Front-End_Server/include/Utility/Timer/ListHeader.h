#ifndef INCLUDE_UTILITY_TIMER_LISTHEADER_H_
#define INCLUDE_UTILITY_TIMER_LISTHEADER_H_

#ifdef CNU_DDS_DLL
#define CNU_DDS_DLL_API	__declspec(dllexport)	
#else
#define CNU_DDS_DLL_API	__declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif
namespace CNU_DDS
{
class CNU_DDS_DLL_API ListHeader
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
#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_UTILITY_TIMER_LISTHEADER_H_ */
