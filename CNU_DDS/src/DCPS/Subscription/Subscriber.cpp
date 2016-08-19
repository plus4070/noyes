#include "../../../include/DCPS/Subscription/Subscriber.h"
#include "../../../include/DCPS/Subscription/DataReader.h"
#include "../../../include/DCPS/Domain/DomainParticipant.h"
#include "../../../include/DCPS/Topic/TopicDescription.h"  // Add Header
#include "../../../include/DCPS/Subscription/DataReaderListener.h" // // Add Header
#include "../../../include/Status/EntityStatus/SubscriberStatus.h"
#include "../../../include/Status/Condition/StatusCondition.h"

#include "../../../include/RTPS/Structure/Participant.h"
#include "../../../include/RTPS/Behavior/StatefulReader.h"
#include <stdio.h>

namespace CNU_DDS
{
	Subscriber::Subscriber()
	{
		status				= new SubscriberStatus();
		status_condition	= new StatusCondition();
		datareader_list		= new DataReaderSeq();

		status_condition->enabled_statuses	= 0x00;
		status_condition->related_entity	= this;

		is_enabled			= false;
		listener			= NULL;
		qos					= NULL;

		_set_default_datareader_qos();

		entity_key			= 0;
	}

	Subscriber::~Subscriber()
	{
		delete(status_condition);
		delete(status);
		delete(datareader_list);
	}

	ReturnCode_t		Subscriber::get_qos(SubscriberQos* qos_list)
	{
		ReturnCode_t	result;

		if(qos_list == NULL)
		{
			result	= RETCODE_BAD_PARAMETER;
		}
		else if(qos)
		{
			memcpy(qos_list, qos, sizeof(SubscriberQos));
			result		= RETCODE_OK;
		}
		else
		{
			result		= RETCODE_PRECONDITION_NOT_MET;	// undefined
		}

		return	result;
	}

	// 구현 미완료
	// set_qos에서 가능한 Error return type은 2가지 -- INCONSISTENT or IMMUTABLE
	// 처음 생성 또는 enabled 이전에 inconsistent or immutable 설정하는 경우에는 에러가 아님
	ReturnCode_t		Subscriber::set_qos(SubscriberQos* qos_list)
	{
		ReturnCode_t	result; // Not defined

		// enabled 하기 이전에 설정하는 경우와 처음 생성해서 설정하는 경우 immutable, inconsistent 에러 적용 안함
		// default_datareader_qos??

		if(!is_enabled && qos == NULL)
		{
			qos	= new SubscriberQos();
			((SubscriberQos*)qos)->entity_factory.auto_enable_created_entities	= qos_list->entity_factory.auto_enable_created_entities;
			memcpy(((SubscriberQos*)qos)->group_data.value, qos_list->group_data.value, MAX_SIZE_OF_GROUP_DATA_QOS_POLICY);
			((SubscriberQos*)qos)->presentation.access_scope	= qos_list->presentation.access_scope;
			((SubscriberQos*)qos)->presentation.coherent_access	= qos_list->presentation.coherent_access;
			((SubscriberQos*)qos)->presentation.ordered_access	= qos_list->presentation.ordered_access;
			result	= RETCODE_OK;
			return result;
		}

		/*
		if(!is_enabled || default_datareader_qos == NULL)
		{
			((SubscriberQos*)qos)->entity_factory = qos_list->entity_factory;
			((SubscriberQos*)qos)->group_data	 = qos_list->group_data;
			((SubscriberQos*)qos)->partition		 = qos_list->partition;
			((SubscriberQos*)qos)->presentation	 = qos_list->presentation;
			result		= RETCODE_OK;
			return	result;
		}
		else
		{
			((SubscriberQos*)qos)->entity_factory = qos_list->entity_factory;
			((SubscriberQos*)qos)->group_data	 = qos_list->group_data;

			// Immutable Qos in SubscriberQos --> PRESENTATION
			// inconsistent Qos in SubscriberQos --> PRESENTATION

			// 그냥 한번에 확인할 방법이 없는건가? , 그리고 Inconsistent로 해야되나 Immutable로 해야 하나 애매하다.... 수정 예상
			if(((SubscriberQos*)qos)->presentation.access_scope != qos_list->presentation.access_scope ||
					((SubscriberQos*)qos)->presentation.coherent_access != qos_list->presentation.coherent_access ||
					((SubscriberQos*)qos)->presentation.ordered_access != qos_list->presentation.ordered_access)// ||
				//		((SubscriberQos*)qos)->presentation.name != qos_list->presentation.name)
			{

				result			= RETCODE_IMMUTABLE_POLICY;
				return		result;
			}

			result		= RETCODE_OK;
			return	result;
		}
*/
		result		= RETCODE_OK;
		return	result;
	}

	Listener*			Subscriber::get_listener(void)
	{
		Listener*	result;

		if(!is_enabled)
		{
			result		= NULL;
			return	result;
		}

		result		= listener;
		return	result;
	}

	ReturnCode_t		Subscriber::set_listener(Listener* a_listener, StatusMask mask)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}

		if(listener!=NULL && a_listener == NULL)
		{
			delete(listener);
			listener	= NULL;
			status_condition->set_enabled_statuses(mask);
			result		= RETCODE_OK;
		}

		else
		{
			listener	= a_listener;
			status_condition->set_enabled_statuses(mask);
			result		= RETCODE_OK;
		}

		result		= RETCODE_ERROR;
		return	result;
	}

	ReturnCode_t		Subscriber::enable(void)
	{
		ReturnCode_t	result;

		is_enabled 		= true;
		result				= RETCODE_OK;

		return	result;
	}

	pDataReader			Subscriber::create_datareader(pTopicDescription a_topic, DataReaderQos* qos, pDataReaderListener a_listener, StatusMask mask)
	{
		DataReader*		result;
		Participant*	rtps_part	= related_participant->related_rtps_participant;
		StatefulReader*	rtps_reader;
		GUID_t			guid		= {0, };

		if(!is_enabled)
		{
			result			= NULL;
			printf("Subscriber::create_dr() Subscriber is not enabled\n");
			return		result;
		}

		if(a_topic != NULL)
		{
			/*
			if(a_topic->get_participant() != this->get_participant()) // a_topic의 participant와 해당 participant가 같은지 확인
			{
				result 	= NULL;
				printf("Subscriber::create_dr() Topic has some problems\n");
				return	result;
			}
			*/
		}
		else
		{
			result		= NULL;
			printf("Subscriber::create_dr() Topic is NULL\n");
			return	result;
		}

		if(qos == NULL)
		{
			qos	= new DataReaderQos();
			if(get_default_datareader_qos(qos) != RETCODE_OK)
			{
				printf("Subscriber::create_dr() get_default_datareader_qos() has some problems\n");
				return NULL;
			}
		}

		// DataReader 생성,값 설정
		result		= new DataReader();

		result->set_qos(qos);
		result->set_listener(a_listener, mask);
		result->related_subscriber	= this;
		result->related_participant	= related_participant;
		result->related_topic_description = a_topic;

		if( (qos != NULL) && ((SubscriberQos*)qos)->entity_factory.auto_enable_created_entities )
		{
			result->enable();
		}

		// DataReaderList에 DataReader 추가
		datareader_list->insertInRear(result);

		guid.guid_prefix					= related_participant->related_rtps_participant->guid.guid_prefix;
		guid.entity_id.entity_key[0]		= ++entity_key;
		guid.entity_id.entity_kind			= ENTITY_KIND_USER_DEFINED_READER_WITH_KEY;
		rtps_reader							= new StatefulReader(guid, RELIABILITY_KIND_RELIABLE);
		rtps_reader->related_dcps_reader	= result;
		result->related_rtps_reader			= (Reader*)rtps_reader;

		related_participant->_datareader_is_created(result);
		rtps_reader->start();

		return	result;
	}

	void				Subscriber::add_datareader(pDataReader reader)
	{
		datareader_list->insertInRear(reader);
	}

	ReturnCode_t		Subscriber::delete_datareader(pDataReader a_datareader)
	{
		ReturnCode_t	result;

		// 검색하는 a_datareader가 NULL이면 아무거나 반환인지 아니면 에러 처리인지 확인 해봐야 함 (수정 예상) --> 스펙에 언급 안되어 있음 우선 주석 처리
		/*
		if(a_datareader == NULL)
		{
			result		= RETCODE_PRECONDITION_NOT_MET;
			return	result;
		}
		*/

		// 해당 DataReader가 Subscriber에 포함되는지 확인
		if(a_datareader->get_subscriber() != this)
		{
			result		= RETCODE_PRECONDITION_NOT_MET;
			return	result;
		}

		// 해당 DataReader가 ReadCondition & QueryCondition을 포함하고 있는지 확인
		if(a_datareader->read_condition_list.getSize() != 0)
		{
			result		= RETCODE_PRECONDITION_NOT_MET;
			return	result;
		}

		// 해당 DataReader가 read 또는 take를 진행하고 있는지 확인 (구현 필요) --> DataReader 구현하면서 진행해야 할 부분으로 고려
		if(a_datareader->read_flag == true)
		{
			result		= RETCODE_PRECONDITION_NOT_MET;
			return	result;
		}

		int								 i	= 0;
		Node<DataReader>*	node	= datareader_list->getNodeByIndex(0);

		while(datareader_list->getSize() > i++)
		{
			node	= node->rear;
			if(node->value == a_datareader) // 삭제할 노드를 찾은 경우 노드 삭제 후 while문 break
			{
				datareader_list->cutNode(node);
				delete(node->value);
				delete(node);
				result		= RETCODE_OK;
				break;
			}
		}

		return	result;
	}

	pDataReader			Subscriber::lookup_datareader(std::string topic_name) // 다수의 DataReader가 존재하는 경우 아무거나 리턴
	{
		DataReader*	result;

		int								 i 	= 0;
		Node<DataReader>*	node	= datareader_list->getNodeByIndex(0);

		while(datareader_list->getSize() > i++)
		{
			node	= node->rear;
			if(!node->value->get_topic_description()->get_topic_name().compare(topic_name)) // topic이 일치하는 경우 topic을 가지는 DataReader를 반환
			{
				result 	= node->value;
				return	result;
			}
		}

		// 값을 못 찾았으므로 NULL로 반환
		result		= NULL;
		return	result;
	}

	// 구현 미완료
	ReturnCode_t		Subscriber::begin_access(void)
	{
		ReturnCode_t	result;

		// 이전에 호출한 begin_access 이후에 end_access가 호출되었는지 확인할 방법이 필요 (구현 필요)
		// ReTiCom에서는 Flag를 이용했음

		// Subscriber의 Presentation Qos의 access_scope == 'GROUP'인지 확인
		if(((SubscriberQos*)qos)->presentation.access_scope != GROUP_PRESENTATION_QOS)
		{
			result		= RETCODE_PRECONDITION_NOT_MET;
			return	result;
		}

		result		= RETCODE_OK;
		return	result;
	}

	// 구현 미완료
	ReturnCode_t		Subscriber::end_access(void)
	{
		ReturnCode_t	result;

		// Subscriber의 Presentation Qos의 access_scope == 'GROUP'인지 확인
		if(((SubscriberQos*)qos)->presentation.access_scope != GROUP_PRESENTATION_QOS)
		{
			result		= RETCODE_PRECONDITION_NOT_MET;
			return	result;
		}

		result		= RETCODE_OK;
		return	result;
	}

	pDomainParticipant	Subscriber::get_participant(void)
	{
		DomainParticipant*	result;

		result		= related_participant;

		return	result;
	}

	// 구현 미완료
	// QoS에 따른 동작, 'set'과 'list'의 차이점 파악, ordered_access가 true인 경우 어떻게 정렬하여 list에 담을지 생각
	// 'set'은 중복이 없음, 'list'는 중복 가능
	// 지정된 순서가 무엇을 의미?

	ReturnCode_t		Subscriber::get_datareaders(DomainEntityList* readers, SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states)
	{
		ReturnCode_t	result;

		// DataReader를 가지는 Subscriber의 Presentation QoS Policy가 Group인 경우 begin/end_access를 호출 해야 함

		if (((SubscriberQos*)qos)==NULL)
		{
			result		= RETCODE_PRECONDITION_NOT_MET;
			return	result;
		}

		if( ((SubscriberQos*)qos)->presentation.access_scope == GROUP_PRESENTATION_QOS)
		{
			if(begin_access() != RETCODE_OK)
			{
				result		= RETCODE_PRECONDITION_NOT_MET;
				return 	result;
			}

			if( ((SubscriberQos*)qos)->presentation.ordered_access != TRUE) // Not Specified Ordered (Maybe 'set')
			{
				int		i	= 0;
				int		j	= 0;

				Node<DataReader>*	node	= datareader_list->getNodeByIndex(0);

				while(datareader_list->getSize() > i++) // State를 만족하는 Reader를 찾아서 list에 삽입
				{
					node	= node->rear; // DataReader Point

					Node<DataSample>* state_node = node->value->data_sample_list.getNodeByIndex(0);

					while(node->value->data_sample_list.getSize() > j++)
					{
						state_node = state_node->rear; // DataReader의 DataSample에 접근

						// Parameter로 넘어온 3가지 조건을 모두 만족하는 DataReader의 경우 readers List에 해당 DataReader를 추가
						if((state_node->value->info->sample_state & sample_states) &&
								(state_node->value->info->view_state & view_states) &&
								(state_node->value->info->instance_state & instance_states))
						{
							// 조건 모두 만족시 readers list에 추가
							readers->insertInRear(node->value);
							break;
						}
					}
				}
			}
			else // Specified Ordered - 미구현 (Maybe 'list')
			{
				int		i	= 0;
				int		j	= 0;

				Node<DataReader>*node	= datareader_list->getNodeByIndex(0);

				while(datareader_list->getSize() > i++)
				{
					node	=	node->rear;

					Node<DataSample>* state_node		= node->value->data_sample_list.getNodeByIndex(0);

					while(node->value->data_sample_list.getSize() > j++)
					{
						state_node = state_node->rear;

					}
				}
			}

			if(end_access() != RETCODE_OK)
			{
				result		= RETCODE_PRECONDITION_NOT_MET;
				return	result;
			}

			result		= RETCODE_OK;
		}
		else // Presentation Qos가 GROUP이 아닌 경우 (TOPIC, INSTANCE)  -- 'set'
		{
			int		i	= 0;
			int		j	= 0;

			Node<DataReader>*	node	= datareader_list->getNodeByIndex(0);

			while(datareader_list->getSize() > i++) // State를 만족하는 Reader를 찾아서 list에 삽입
			{
				node	= node->rear; // DataReader Point

				Node<DataSample>* state_node = node->value->data_sample_list.getNodeByIndex(0);

				while(node->value->data_sample_list.getSize() > j++)
				{
					state_node = state_node->rear; // DataReader의 DataSample에 접근

					// Parameter로 넘어온 3가지 조건을 모두 만족하는 DataReader의 경우 readers List에 해당 DataReader를 추가
					// 추가 하고 해당 DataReader를 중복해서 추가하면 안되므로 break
					if((state_node->value->info->sample_state & sample_states) &&
							(state_node->value->info->view_state & view_states) &&
							(state_node->value->info->instance_state & instance_states))
					{
						// 조건 모두 만족시 readers list에 추가
						readers->insertInRear(node->value);
						break;
					}
				}
			}
		}

		result		= RETCODE_OK;
		return	result;
	}

	ReturnCode_t		Subscriber::notify_datareaders(void)
	{
		ReturnCode_t	result;
		// 데이터가 Available한 상태의 DataReader들 중 Listener가 등록되어 있는 Reader들만 DataReaderListener 함수인 on_data_available을 모두 호출한다.

		int							i	= 0;
		Node<DataReader>* node 	= datareader_list->getNodeByIndex(0);
		DataReaderListener* pListener;

		while(datareader_list->getSize() > i++)
		{
			node	= node->rear;

			// DataReaderListener가 등록되어 있으며 상태가 DATA_AVAILABLE인 DataReader의 on_data_available을 호출
			if( (node->value->listener != NULL) && (node->value->get_status_changes() & DATA_AVAILABLE_STATUS))
			{
				pListener = (DataReaderListener*)node->value->get_listener();
				pListener->on_data_available(node->value);
			}
		}

		// 에러처리에 관련된 부분 언급 없음
		result		= RETCODE_OK;
		return	result;
	}

	// 생성된 모든 Entity를 삭제
	ReturnCode_t		Subscriber::delete_contained_entities(void)
	{
		ReturnCode_t	result;

		int									i	= 0;
		Node<DataReader>	*node	= datareader_list->getNodeByIndex(0);

		while(datareader_list->getSize() > i++)
		{
			node = node->rear;

			if(delete_datareader(node->value) != RETCODE_OK) // 해당 Reader를 Delete, 실패하는 경우 에러코드 반환 (DataReader에 Read,QueryCondition이 존재하는 것)
			{
				result = RETCODE_PRECONDITION_NOT_MET;
				return	result;
			}
		}

		result		= RETCODE_OK;
		return	result;
	}

	// 구현 미완료
	ReturnCode_t		Subscriber::set_default_datareader_qos(DataReaderQos* qos_list)
	{
		ReturnCode_t	result;

		// Check self consisent??

		default_datareader_qos = qos_list;

		result		= RETCODE_OK;

		return	result;
	}

	ReturnCode_t		Subscriber::get_default_datareader_qos(DataReaderQos* qos_list)
	{
		ReturnCode_t	result;

		memcpy(qos_list, default_datareader_qos, sizeof(DataReaderQos));

		result		= RETCODE_OK;

		return	result;
	}

	// DataReader Qos에 Topic Qos와 일치하는 부분을 copy
	ReturnCode_t		Subscriber::copy_from_topic_qos(DataReaderQos* a_datareader_qos, TopicQos* a_topic_qos)
	{
		ReturnCode_t	result;

		// deadline, destination_order, durability, history, latency_budget, liveliness, ownership, reliability, resource_limits

		if(a_datareader_qos != NULL && a_topic_qos != NULL) // QoS가 NULL인 경우에 어떻게 설정하는지 SPEC언급 없음 (수정 예상)
		{
			a_datareader_qos->deadline = a_topic_qos->deadline;
			a_datareader_qos->destination_order = a_topic_qos->destination_order;
			a_datareader_qos->durability = a_topic_qos->durability;
			a_datareader_qos->history = a_topic_qos->history;
			a_datareader_qos->latency_budget = a_topic_qos->latency_budget;
			a_datareader_qos->liveliness = a_topic_qos->liveliness;
			a_datareader_qos->ownership = a_topic_qos->ownership;
			a_datareader_qos->reliability = a_topic_qos->reliability;
			a_datareader_qos->resource_limits = a_topic_qos->resource_limits;
			result		= RETCODE_OK;
		}
		else
		{
			result		= RETCODE_PRECONDITION_NOT_MET;
		}

		return	result;
	}

	void				Subscriber::_set_default_datareader_qos(void)
	{
		this->default_datareader_qos	= new DataReaderQos();

		this->default_datareader_qos->deadline.period.sec		= 0;
		this->default_datareader_qos->deadline.period.nanosec	= 0;

		this->default_datareader_qos->destination_order.kind	= BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS;

		this->default_datareader_qos->durability.kind	= VOLATILE_DURABILITY_QOS;

		this->default_datareader_qos->history.kind	= KEEP_ALL_HISTORY_QOS;
		this->default_datareader_qos->history.depth	= -1;

		this->default_datareader_qos->latency_budget.duration.sec		= 0;
		this->default_datareader_qos->latency_budget.duration.nanosec	= 0;

		this->default_datareader_qos->liveliness.kind					= AUTOMATIC_LIVELINESS_QOS;
		this->default_datareader_qos->liveliness.lease_duration.sec		= 0;
		this->default_datareader_qos->liveliness.lease_duration.nanosec	= 0;

		this->default_datareader_qos->ownership.kind	= SHARED_OWNERSHIP_QOS;

		this->default_datareader_qos->reader_data_lifecycle.auto_purge_disposed_samples_delay.sec		= 30;
		this->default_datareader_qos->reader_data_lifecycle.auto_purge_disposed_samples_delay.nanosec	= 0;
		this->default_datareader_qos->reader_data_lifecycle.auto_purge_no_writer_samples_delay.sec		= 30;
		this->default_datareader_qos->reader_data_lifecycle.auto_purge_no_writer_samples_delay.nanosec	= 0;

		this->default_datareader_qos->reliability.kind	= RELIABLE_RELIABILITY_QOS;
		this->default_datareader_qos->reliability.max_blocking_time.sec		= 0;
		this->default_datareader_qos->reliability.max_blocking_time.nanosec	= 0;

		this->default_datareader_qos->resource_limits.max_instances				= -1;
		this->default_datareader_qos->resource_limits.max_samples				= -1;
		this->default_datareader_qos->resource_limits.max_samples_per_instance	= -1;

		this->default_datareader_qos->time_based_filter.minimum_separation.sec		= 0;
		this->default_datareader_qos->time_based_filter.minimum_separation.nanosec	= 0;

		memset(this->default_datareader_qos->user_data.value, 0, MAX_SIZE_OF_USER_DATA_QOS_POLICY);
	}
}
