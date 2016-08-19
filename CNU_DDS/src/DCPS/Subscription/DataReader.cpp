#include "../../../include/DCPS/Subscription/DataReader.h"
#include "../../../include/DCPS/Subscription/Subscriber.h"
#include "../../../include/Status/EntityStatus/DataReaderStatus.h"
#include "../../../include/Status/Condition/StatusCondition.h"

namespace CNU_DDS
{
	DataReader::DataReader()
	{
		status				= new DataReaderStatus();
		status_condition	= new StatusCondition();

		status_condition->enabled_statuses	= 0x00;
		status_condition->related_entity	= this;

		is_enabled			= false;
		listener			= NULL;
		qos					= NULL;
	}

	DataReader::~DataReader()
	{
		delete(status_condition);
		delete(status);
	}

	ReturnCode_t		DataReader::get_qos(DataReaderQos* qos_list)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(qos)
		{
			memcpy(qos_list, qos, sizeof(DataReaderQos));
			result		= RETCODE_OK;
		}
		else
		{
			result		= RETCODE_PRECONDITION_NOT_MET;	// undefined
		}

		return	result;
	}

	// 구현 미완료
	// Immutable, Inconsistent Qos 모두 고려해야 하므로 우선 공란
	ReturnCode_t		DataReader::set_qos(DataReaderQos* qos_list)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(!is_enabled && qos == NULL)
		{
			qos	= new DataReaderQos();
			memcpy(qos, qos_list, sizeof(DataReaderQos));
			result	= RETCODE_OK;
			return result;
		}

		return	result;
	}

	Listener*			DataReader::get_listener(void)
	{
		Listener*	result	= NULL;


		result		= listener;
		return	result;
	}

	ReturnCode_t		DataReader::set_listener(Listener* a_listener, StatusMask mask)
	{
		ReturnCode_t	result	= RETCODE_OK;

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

	ReturnCode_t		DataReader::enable(void)
	{
		ReturnCode_t	result	= RETCODE_OK;

		is_enabled 		= true;
		result				= RETCODE_OK;

		return	result;
	}

	ReturnCode_t		DataReader::read(DataList* data_values, SampleInfoList* sample_infos, long max_sample, SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(!is_enabled)
		{
			result		=	RETCODE_NOT_ENABLED;
			return	result;
		}

		if(max_sample == 0)		// Zero-copy인 경우 return_loan을 호출해야 함 , 좀 더 생각 필요
		{
			result		= return_loan(data_values, sample_infos);
			return 	result;
		}

		read_flag	 = true; // READING - Subscriber에서 데이터를 읽는 중인지 판별하기 위해서 추가.

		int							i		= 0;
		Node<DataSample>* node = data_sample_list.getNodeByIndex(0);


		if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == INSTANCE_PRESENTATION_QOS)
		{
			while(data_sample_list.getSize() > i++)
			{
				node = node->rear;

				// 넘어오는 파라메터 3가지 조건을 모두 만족하는 Data, SampleInfo인 경우 데이터를 읽음
				if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
						(node->value->info->instance_state & instance_states))
				{
					if(max_sample > data_values->getSize()) // 최대 사이즈만큼 데이터를 읽음
					{
						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);
					}
					else // 읽을 만큼 모두 읽었으면 while루프 탈출... 루프 탈출 이후 RTPS를 이용해서 설정할 부분과 return 처리를 함
					{
						break;
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == TOPIC_PRESENTATION_QOS)
		{
			if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			{
				if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				{
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								(node->value->info->instance_state & instance_states))
						{
							if(max_sample > data_values->getSize())
							{
								node->value->info->sample_state	= READ_SAMPLE_STATE;
								node->value->info->view_state		= NOT_NEW_VIEW_STATE;

								data_values->insertInRear(node->value->value);
								sample_infos->insertInRear(node->value->info);
							}
							else
							{
								break;
							}
						}
					}
				}
				else // Source_timestamp를 고려하여 List에 저장
				{
					Time_t	cTime;
					bool		cFlag;
					int			index;
					int			max_len;
					int			j;

					cTime.sec			= 0;
					cTime.nanosec		= 0;

					// 최소값을 찾기
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						// State를 만족하는 데이터 중 TimeStamp를 고려해야 함
						if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								(node->value->info->instance_state & instance_states))
						{
							if((cTime.sec == 0) && (cTime.nanosec == 0))
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;

								index	= i;
								max_len++;
							}
							else if(cTime.sec > node->value->info->source_timestamp.sec)
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;

								index	= i;
								max_len++;
							}
							else if( (cTime.sec == node->value->info->source_timestamp.sec) && (cTime.nanosec > node->value->info->source_timestamp.nanosec))
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;

								index	= i;
								max_len++;
							}
						}
					}

					// 가장 작은 데이터가 들어있는 Index로 노드를 이동시켜서 해당 노드의 데이터를 저장
					node	=	data_sample_list.getNodeByIndex(index);

					node->value->info->sample_state	= READ_SAMPLE_STATE;
					node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					data_values->insertInRear(node->value->value);
					sample_infos->insertInRear(node->value->info);

					// 실제로 읽을 수 있는 데이터의 크기보다 제한된 크기보다 크다면
					// 최대 크기는 5인데 읽을 수 있는 데이터가 이보다 작다면 최대 크기만큼 데이터를 담을 수 없으므로 의미없는 루프 돌리지 않아도 됨

					if(max_sample <= max_len)
					{
						max_len = max_sample;
					}

					//int		dup_data[max_len];
					int*	dup_data	= new int[max_len];
					int		k;
					dup_data[0] = index;

					cFlag 	= true;
					i			= 0;
					j			= 0;

					cTime.sec			= 0;
					cTime.nanosec		= 0;

					// 가장 작은 값 보다 크지만 나머지 중에서 가장 작은 값들을 차례로 List에 저장
					while(max_len-1 > j++)
					{
						node		=  data_sample_list.getNodeByIndex(0);
						cFlag		=  true;

						while(data_sample_list.getSize() > i++)
						{
							node = node->rear;
							if((node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
									(node->value->info->instance_state & instance_states))
							{
								for(k = 0; k < j; k++) // 이전에 저장했던 Index와 겹친다면 중복되는 것이므로 체크, Flag값이 True라면 겹치지 않는 데이터라는 의미
								{
									if(dup_data[k] == i)
									{
										cFlag = false;
										break;
									}
								}

								if((cTime.sec == 0) && (cTime.nanosec == 0) && cFlag)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;

									dup_data[j] = i;
								}
								else if(cTime.sec > node->value->info->source_timestamp.sec && cFlag)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;

									dup_data[j] = i;
								}
								else if( (cTime.sec == node->value->info->source_timestamp.sec) && (cTime.nanosec > node->value->info->source_timestamp.nanosec) && cFlag)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;

									dup_data[j] = i;
								}
							}
						}

						node	=	data_sample_list.getNodeByIndex(dup_data[j]);

						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);
					}
				}
			}
			else // Ordered_access = false
			 {
				while(data_sample_list.getSize() > i++)
				{
					node = node->rear;

					if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
							(node->value->info->instance_state & instance_states))
					{
						if(max_sample > data_values->getSize())
						{
							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_values->insertInRear(node->value->value);
							sample_infos->insertInRear(node->value->info);
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == GROUP_PRESENTATION_QOS)
		{
			// presentation qos가 group이며 ordered_access가 true인 경우에는 최대 1개의 샘플을 반환한다.
			 if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			 {
				 if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				 {
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

						 if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								 (node->value->info->instance_state & instance_states))
						 {
							 node->value->info->sample_state	= READ_SAMPLE_STATE;
							 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							 data_values->insertInRear(node->value->value);
							 sample_infos->insertInRear(node->value->info);

							 break;
						 }
					 }
				 }
				 else // destination_order.kind == SOURCE_TIMESTAMP
				 {
					 Time_t 	cTime;
					 int		count; // Data, SampleInfo 하나만 얻으면 되므로 배열을 이용하지 않음

					 // 상식적으로 시스템 시작하고 0초에 데이터를 쏠 순 없을테니 0,0으로 초기화 하였음
					 cTime.sec			= 0;
					 cTime.nanosec	= 0;

					 // 가장 작은 값을 찾음
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

						 if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								 (node->value->info->instance_state & instance_states))
						 {
							 if((cTime.sec == 0) && (cTime.nanosec == 0))
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec > node->value->info->source_timestamp.sec)
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec == node->value->info->source_timestamp.sec)
							 {
								 if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
								 {
									 cTime.sec			= node->value->info->source_timestamp.sec;
									 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
									 count				= i;
								 }
							 }
						 }
					 }

					 node = data_sample_list.getNodeByIndex(count);

					 node->value->info->sample_state	= READ_SAMPLE_STATE;
					 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					 data_values->insertInRear(node->value->value);
					 sample_infos->insertInRear(node->value->info);
				 }
			 }
			 else // Ordered_access = false
			 {
				 while(data_sample_list.getSize() > i++)
				 {
					 node = node->rear;

					 if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
							 (node->value->info->instance_state & instance_states))
					 {
						 if(max_sample > data_values->getSize())
						 {
							 node->value->info->sample_state	= READ_SAMPLE_STATE;
							 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							 data_values->insertInRear(node->value->value);
							 sample_infos->insertInRear(node->value->info);
						 }
						 else
						 {
							 break;
						 }
					 }
				 }
			 }
		}

		read_flag	= false;

		// 데이터를 저장한 리스트의 크기가 0보다 크다면 데이터를 읽은 것임.. 하지만 애초에 넘어오는 리스트의 크기가 0이라는 것은 보장할 수 없으므로 대안이 필요함
		// 무식하게 하면 애초에 리스트 크기를 저장하였다가 비교하거나 데이터를 읽는 곳에 result = RETCODE_OK를 넣는 방법이 있겠음..
		if(data_values->getSize() > 0)
		{
			result		= RETCODE_OK;
		}
		else // 데이터를 찾지 못했으면(아예 데이터를 하나도 읽지 못한 경우)
		{
			result		 = RETCODE_NO_DATA;
		}

		// rtps 구현 필요
		// 여기서 sample_info의 다양한?? rank 값들 설정해줘야 함
	}

	// read 에서 받아 오는 파라메터값들을 ReadCondition으로 대체하는 것 같음
	// a_condition에서 받아오는 state 값들을 get 함수로 해야 하는데 구현이 안되있어서 에러 나므로 우선은 그냥 직접 접근으로 구현했음
	ReturnCode_t		DataReader::read_w_condition(DataList* data_values, SampleInfoList* sample_infos, long max_sample, ReadCondition* a_condition)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(!is_enabled)
		{
			result		=	RETCODE_NOT_ENABLED;
			return	result;
		}

		// get_datareader가 미구현 상태이므로 에러가 나서 주석처리 함
		/*
		if(a_condition->get_datareader() != this)
		{
			result		= RETCODE_PRECONDITION_NOT_MET;
			return	result;
		}
		*/

		if(max_sample == 0)		// Zero-copy인 경우 return_loan을 호출해야 함 , 좀 더 생각 필요
		{
			result		= return_loan(data_values, sample_infos);
			return 	result;
		}

		read_flag	 = true; // READING - Subscriber에서 데이터를 읽는 중인지 판별하기 위해서 추가.

		int							i		= 0;
		Node<DataSample>* node = data_sample_list.getNodeByIndex(0);


		if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == INSTANCE_PRESENTATION_QOS)
		{
			while(data_sample_list.getSize() > i++)
			{
				node = node->rear;

				if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
						(node->value->info->instance_state & a_condition->instance_state_mask))
				{
					if(max_sample > data_values->getSize())
					{
						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);
					}
					else
					{
						break;
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == TOPIC_PRESENTATION_QOS)
		{
			if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			{
				if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				{
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
								(node->value->info->instance_state & a_condition->instance_state_mask))
						{
							if(max_sample > data_values->getSize())
							{
								node->value->info->sample_state	= READ_SAMPLE_STATE;
								node->value->info->view_state		= NOT_NEW_VIEW_STATE;

								data_values->insertInRear(node->value->value);
								sample_infos->insertInRear(node->value->info);
							}
							else
							{
								break;
							}
						}
					}
				}
				else // Source_timestamp를 고려하여 List에 저장
				{
					Time_t	cTime;
					bool		cFlag;
					int			index;
					int			max_len;
					int			j;

					cTime.sec			= 0;
					cTime.nanosec		= 0;

					// 최소값을 찾기
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						// State를 만족하는 데이터 중 TimeStamp를 고려해야 함
						if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
								(node->value->info->instance_state & a_condition->instance_state_mask))
						{
							if((cTime.sec == 0) && (cTime.nanosec == 0))
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;

								index	= i;
								max_len++;
							}
							else if(cTime.sec > node->value->info->source_timestamp.sec)
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;

								index	= i;
								max_len++;
							}
							else if( (cTime.sec == node->value->info->source_timestamp.sec) && (cTime.nanosec > node->value->info->source_timestamp.nanosec))
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;

								index	= i;
								max_len++;
							}
						}
					}

					// 가장 작은 데이터가 들어있는 Index로 노드를 이동시켜서 해당 노드의 데이터를 저장
					node	=	data_sample_list.getNodeByIndex(index);

					node->value->info->sample_state	= READ_SAMPLE_STATE;
					node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					data_values->insertInRear(node->value->value);
					sample_infos->insertInRear(node->value->info);

					// 실제로 읽을 수 있는 데이터의 크기보다 제한된 크기보다 크다면
					// 최대 크기는 5인데 읽을 수 있는 데이터가 이보다 작다면 최대 크기만큼 데이터를 담을 수 없으므로 의미없는 루프 돌리지 않아도 됨

					if(max_sample <= max_len)
					{
						max_len = max_sample;
					}

					//int		dup_data[max_len];
					int*	dup_data	= new int[max_len];
					int		k;
					dup_data[0] = index;

					cFlag 	= true;
					i			= 0;
					j			= 0;

					cTime.sec			= 0;
					cTime.nanosec		= 0;

					// 가장 작은 값 보다 크지만 나머지 중에서 가장 작은 값들을 차례로 List에 저장
					while(max_len-1 > j++)
					{
						node		=  data_sample_list.getNodeByIndex(0);
						cFlag		=  true;

						while(data_sample_list.getSize() > i++)
						{
							node = node->rear;
							if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
									(node->value->info->instance_state & a_condition->instance_state_mask))
							{
								for(k = 0; k < j; k++) // 이전에 저장했던 Index와 겹친다면 중복되는 것이므로 체크, Flag값이 True라면 겹치지 않는 데이터라는 의미
								{
									if(dup_data[k] == i)
									{
										cFlag = false;
										break;
									}
								}

								if((cTime.sec == 0) && (cTime.nanosec == 0) && cFlag)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;

									dup_data[j] = i;
								}
								else if(cTime.sec > node->value->info->source_timestamp.sec && cFlag)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;

									dup_data[j] = i;
								}
								else if( (cTime.sec == node->value->info->source_timestamp.sec) && (cTime.nanosec > node->value->info->source_timestamp.nanosec) && cFlag)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;

									dup_data[j] = i;
								}
							}
						}

						node	=	data_sample_list.getNodeByIndex(dup_data[j]);

						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);
					}
				}
			}
			else // Ordered_access = false
			{
				while(data_sample_list.getSize() > i++)
				{
					node = node->rear;

					if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
							(node->value->info->instance_state & a_condition->instance_state_mask))
					{
						if(max_sample > data_values->getSize())
						{
							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_values->insertInRear(node->value->value);
							sample_infos->insertInRear(node->value->info);
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == GROUP_PRESENTATION_QOS)
		{
			// presentation qos가 group이며 ordered_access가 true인 경우에는 최대 1개의 샘플을 반환한다.
			if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			{
				if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				{
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
								(node->value->info->instance_state & a_condition->instance_state_mask))
						{
							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_values->insertInRear(node->value->value);
							sample_infos->insertInRear(node->value->info);

							break;
						}
					}
				}
				else // destination_order.kind == SOURCE_TIMESTAMP
				{
					Time_t 	cTime;
					int			count;

					// 상식적으로 시스템 시작하고 0초에 데이터를 쏠 순 없을테니 0,0으로 초기화 하였음
					cTime.sec			= 0;
					cTime.nanosec	= 0;

					// 가장 작은 값을 찾음
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
								(node->value->info->instance_state & a_condition->instance_state_mask))
						{
							if((cTime.sec == 0) && (cTime.nanosec == 0))
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								count				= i;
							}
							else if(cTime.sec > node->value->info->source_timestamp.sec)
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								count				= i;
							}
							else if(cTime.sec == node->value->info->source_timestamp.sec)
							{
								if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec	= node->value->info->source_timestamp.nanosec;
									count				= i;
								}
							}
						}
					}

					node = data_sample_list.getNodeByIndex(count);

					node->value->info->sample_state	= READ_SAMPLE_STATE;
					node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					data_values->insertInRear(node->value->value);
					sample_infos->insertInRear(node->value->info);
				}
			}
			else // Ordered_access = false
			{
				while(data_sample_list.getSize() > i++)
				{
					node = node->rear;

					if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
							(node->value->info->instance_state & a_condition->instance_state_mask))
					{
						if(max_sample > data_values->getSize())
						{
							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_values->insertInRear(node->value->value);
							sample_infos->insertInRear(node->value->info);
						}
						else
						{
							break;
						}
					}
				}
			}
		}

		read_flag	= false;

		if(data_values->getSize() > 0)
		{
			result		= RETCODE_OK;
		}
		else
		{
			result		 = RETCODE_NO_DATA;
		}

		// rtps 구현 필요
		// 여기서 sample_info의 다양한?? rank 값들 설정해줘야 함
	}

	// 아직 접근하지 않은 DataReader의 Data를 접근하여 복사함, SampleInfo 또한 복사해야 함
	// read Operation과 흡사하지만 max_samples, Sample_state와 view_state, instance_state가 고정된 것이 다른 점.
	// max_sample = 1, sample_state = NOT_READ, view_state = ANY_VIEW, instance_state = ANY_INSTANACE
	ReturnCode_t		DataReader::read_next_sample(DDS_Data* data_value, SampleInfo* sample_info)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(!is_enabled)
		{
			result		=	RETCODE_NOT_ENABLED;
			return	result;
		}

		read_flag	 = true; // READING - Subscriber에서 데이터를 읽는 중인지 판별하기 위해서 추가.

		int							i		= 0;
		Node<DataSample>* node = data_sample_list.getNodeByIndex(0);


		if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == INSTANCE_PRESENTATION_QOS)
		{
			while(data_sample_list.getSize() > i++)
			{
				node = node->rear;

				if( (node->value->info->sample_state & NOT_READ_SAMPLE_STATE) && (node->value->info->view_state & ANY_VIEW_STATE) &&
						(node->value->info->instance_state & ANY_INSTANCE_STATE))
				{
					node->value->info->sample_state	= READ_SAMPLE_STATE;
					node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					data_value = node->value->value;
					sample_info = node->value->info;

					break;
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == TOPIC_PRESENTATION_QOS)
		{
			if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			{
				if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				{
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & NOT_READ_SAMPLE_STATE) && (node->value->info->view_state & ANY_VIEW_STATE) &&
								(node->value->info->instance_state & ANY_INSTANCE_STATE))
						{
								node->value->info->sample_state	= READ_SAMPLE_STATE;
								node->value->info->view_state		= NOT_NEW_VIEW_STATE;

								data_value = node->value->value;
								sample_info = node->value->info;

								break;
						}
					}
				}
				else // Source_timestamp를 고려하여 List에 저장
				{
					Time_t 	cTime;
					bool 		cFlag;
					int			count;

					// 상식적으로 시스템 시작하고 0초에 데이터를 쏠순 없으니 0,0으로 초기화 하였음
					cTime.sec			= 0;
					cTime.nanosec		= 0;

					// 가장 작은 값을 찾음
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & NOT_READ_SAMPLE_STATE) && (node->value->info->view_state & ANY_VIEW_STATE) &&
								(node->value->info->instance_state & ANY_INSTANCE_STATE))
						{
							if((cTime.sec == 0) && (cTime.nanosec == 0))
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;
								count					= i;
							}
							else if(cTime.sec > node->value->info->source_timestamp.sec)
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;
								count					= i;
							}
							else if(cTime.sec == node->value->info->source_timestamp.sec)
							{
								if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count					= i;
								}
							}
						}
					}

					node = data_sample_list.getNodeByIndex(count);

					node->value->info->sample_state	= READ_SAMPLE_STATE;
					node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					data_value = node->value->value;
					sample_info = node->value->info;
				}
			}
			else // false
			 {
				while(data_sample_list.getSize() > i++)
				{
					node = node->rear;

					if( (node->value->info->sample_state & NOT_READ_SAMPLE_STATE) && (node->value->info->view_state & ANY_VIEW_STATE) &&
							(node->value->info->instance_state & ANY_INSTANCE_STATE))
					{
						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_value = node->value->value;
						sample_info = node->value->info;

						break;
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == GROUP_PRESENTATION_QOS)
		{
			// presentation qos가 group이며 ordered_access가 true인 경우에는 최대 1개의 샘플을 반환한다.
			 if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			 {
				 if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				 {
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

							if( (node->value->info->sample_state & NOT_READ_SAMPLE_STATE) && (node->value->info->view_state & ANY_VIEW_STATE) &&
									(node->value->info->instance_state & ANY_INSTANCE_STATE))
						 {
							 node->value->info->sample_state	= READ_SAMPLE_STATE;
							 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							 data_value = node->value->value;
							 sample_info = node->value->info;

							 break;
						 }
					 }
				 }
				 else // destination_order.kind == SOURCE_TIMESTAMP
				 {
					 Time_t 	cTime;
					 int		count;

					 // 상식적으로 시스템 시작하고 0초에 데이터를 쏠 순 없을테니 0,0으로 초기화 하였음
					 cTime.sec			= 0;
					 cTime.nanosec	= 0;

					 // 가장 작은 값을 찾음
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

							if( (node->value->info->sample_state & NOT_READ_SAMPLE_STATE) && (node->value->info->view_state & ANY_VIEW_STATE) &&
									(node->value->info->instance_state & ANY_INSTANCE_STATE))
						 {
							 if((cTime.sec == 0) && (cTime.nanosec == 0))
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec > node->value->info->source_timestamp.sec)
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec == node->value->info->source_timestamp.sec)
							 {
								 if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
								 {
									 cTime.sec			= node->value->info->source_timestamp.sec;
									 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
									 count				= i;
								 }
							 }
						 }
					 }

					 node = data_sample_list.getNodeByIndex(count);

					 node->value->info->sample_state	= READ_SAMPLE_STATE;
					 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					 data_value = node->value->value;
					 sample_info = node->value->info;

				 }
			 }
			 else // Ordered_access = false
			 {
				 while(data_sample_list.getSize() > i++)
				 {
					 node = node->rear;

					 if( (node->value->info->sample_state & NOT_READ_SAMPLE_STATE) && (node->value->info->view_state & ANY_VIEW_STATE) &&
							 (node->value->info->instance_state & ANY_INSTANCE_STATE))
					 {
						 node->value->info->sample_state	= READ_SAMPLE_STATE;
						 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						 data_value = node->value->value;
						 sample_info = node->value->info;

						 break;
					 }
				 }
			 }
		}

		read_flag	= false;

		// OK인지 NO_DATA인지 판별하는 방법에 대해 고려..
		if(data_value != NULL)
		{
			result		= RETCODE_OK;
		}
		else
		{
			result		 = RETCODE_NO_DATA;
		}

		// rtps 구현 필요
		// 여기서 sample_info의 다양한?? rank 값들 설정해줘야 함
	}

	ReturnCode_t		DataReader::take(DataList* data_values, SampleInfoList* sample_infos, long max_sample, SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(!is_enabled)
		{
			result		=	RETCODE_NOT_ENABLED;
			return	result;
		}

		if(max_sample == 0)		// Zero-copy인 경우 return_loan을 호출해야 함 , 좀 더 생각 필요
		{
			result		= return_loan(data_values, sample_infos);
			return 	result;
		}

		read_flag	 = true; // READING - Subscriber에서 데이터를 읽는 중인지 판별하기 위해서 추가.

		int							i		= 0;
		Node<DataSample>* node = data_sample_list.getNodeByIndex(0);


		if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == INSTANCE_PRESENTATION_QOS)
		{
			while(data_sample_list.getSize() > i++)
			{
				node = node->rear;

				if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
						(node->value->info->instance_state & instance_states))
				{
					if(max_sample > data_values->getSize())
					{
						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);

						data_sample_list.cutNode(node); // 읽은 노드 삭제
						delete(node->value);
						delete(node);
					}
					else
					{
						break;
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == TOPIC_PRESENTATION_QOS)
		{
			if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			{
				if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				{
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								(node->value->info->instance_state & instance_states))
						{
							if(max_sample > data_values->getSize())
							{
								node->value->info->sample_state	= READ_SAMPLE_STATE;
								node->value->info->view_state		= NOT_NEW_VIEW_STATE;

								data_values->insertInRear(node->value->value);
								sample_infos->insertInRear(node->value->info);

								data_sample_list.cutNode(node); // 읽은 노드 삭제
								delete(node->value);
								delete(node);
							}
							else
							{
								break;
							}
						}
					}
				}
				else // Source_timestamp를 고려하여 List에 저장
				{
					Time_t 	cTime;
					int			count;
					int			j;

					// 상식적으로 시스템 시작하고 0초에 데이터를 쏠순 없으니 0,0으로 초기화 하였음
					cTime.sec			= 0;
					cTime.nanosec		= 0;
					j						= 0;

					while(max_sample > j++)
					{
						while(data_sample_list.getSize() > i++)
						{
							node = node->rear;

							if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
									(node->value->info->instance_state & instance_states))
							{
								if((cTime.sec == 0) && (cTime.nanosec == 0))
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count					= i;
								}
								else if(cTime.sec > node->value->info->source_timestamp.sec)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count					= i;
								}
								else if(cTime.sec == node->value->info->source_timestamp.sec)
								{
									if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
									{
										cTime.sec			= node->value->info->source_timestamp.sec;
										cTime.nanosec		= node->value->info->source_timestamp.nanosec;
										count					= i;
									}
								}
							}
						}

						node = data_sample_list.getNodeByIndex(count);

						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);

						data_sample_list.cutNode(node);
						delete(node->value);
						delete(node);

						node = data_sample_list.getNodeByIndex(0);

						i						= 0;
						cTime.sec			= 0;
						cTime.nanosec		= 0;
					}
				}
			}
			else // false
			 {
				while(data_sample_list.getSize() > i++)
				{
					node = node->rear;

					if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
							(node->value->info->instance_state & instance_states))
					{
						if(max_sample > data_values->getSize())
						{
							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_values->insertInRear(node->value->value);
							sample_infos->insertInRear(node->value->info);

							data_sample_list.cutNode(node);
							delete(node->value);
							delete(node);
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == GROUP_PRESENTATION_QOS)
		{
			// presentation qos가 group이며 ordered_access가 true인 경우에는 최대 1개의 샘플을 반환한다.
			 if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			 {
				 if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				 {
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

							if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
									(node->value->info->instance_state & instance_states))
						 {
							 node->value->info->sample_state	= READ_SAMPLE_STATE;
							 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							 data_values->insertInRear(node->value->value);
							 sample_infos->insertInRear(node->value->info);

							 data_sample_list.cutNode(node);
							 delete(node->value);
							 delete(node);

							 break;
						 }
					 }
				 }
				 else // destination_order.kind == SOURCE_TIMESTAMP
				 {
					 Time_t 	cTime;
					 int			count;

					 // 상식적으로 시스템 시작하고 0초에 데이터를 쏠 순 없을테니 0,0으로 초기화 하였음
					 cTime.sec			= 0;
					 cTime.nanosec	= 0;

					 // 가장 작은 값을 찾음
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

							if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
									(node->value->info->instance_state & instance_states))
						 {
							 if((cTime.sec == 0) && (cTime.nanosec == 0))
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec > node->value->info->source_timestamp.sec)
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec == node->value->info->source_timestamp.sec)
							 {
								 if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
								 {
									 cTime.sec			= node->value->info->source_timestamp.sec;
									 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
									 count				= i;
								 }
							 }
						 }
					 }

					 node = data_sample_list.getNodeByIndex(count);

					 node->value->info->sample_state	= READ_SAMPLE_STATE;
					 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					 data_values->insertInRear(node->value->value);
					 sample_infos->insertInRear(node->value->info);

					 data_sample_list.cutNode(node);
					 delete(node->value);
					 delete(node);
				 }
			 }
			 else // Ordered_access = false
			 {
				 while(data_sample_list.getSize() > i++)
				 {
					 node = node->rear;

						if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								(node->value->info->instance_state & instance_states))
					 {
						 if(max_sample > data_values->getSize())
						 {
							 node->value->info->sample_state	= READ_SAMPLE_STATE;
							 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							 data_values->insertInRear(node->value->value);
							 sample_infos->insertInRear(node->value->info);

							 data_sample_list.cutNode(node);
							 delete(node->value);
							 delete(node);
						 }
						 else
						 {
							 break;
						 }
					 }
				 }
			 }
		}

		read_flag	= false;

		if(data_values->getSize() > 0)
		{
			result		= RETCODE_OK;
		}
		else
		{
			result		 = RETCODE_NO_DATA;
		}

		// rtps 구현 필요
		// 여기서 sample_info의 다양한?? rank 값들 설정해줘야 함

	}

	ReturnCode_t		DataReader::take_w_condition(DataList* data_values, SampleInfoList* sample_infos, long max_sample, ReadCondition* a_condition)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(!is_enabled)
		{
			result		=	RETCODE_NOT_ENABLED;
			return	result;
		}
		// get_datareader가 미구현 상태이므로 에러가 나서 주석처리 함
		/*
		if(a_condition->get_datareader() != this)
		{
			result		= RETCODE_PRECONDITION_NOT_MET;
			return	result;
		}
		*/

		if(max_sample == 0)		// Zero-copy인 경우 return_loan을 호출해야 함 , 좀 더 생각 필요
		{
			result		= return_loan(data_values, sample_infos);
			return 	result;
		}

		read_flag	 = true; // READING - Subscriber에서 데이터를 읽는 중인지 판별하기 위해서 추가.

		int							i		= 0;
		Node<DataSample>* node = data_sample_list.getNodeByIndex(0);


		if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == INSTANCE_PRESENTATION_QOS)
		{
			while(data_sample_list.getSize() > i++)
			{
				node = node->rear;

				if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
						(node->value->info->instance_state & a_condition->instance_state_mask))
				{
					if(max_sample > data_values->getSize())
					{
						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);

						data_sample_list.cutNode(node); // 읽은 노드 삭제
						delete(node->value);
						delete(node);
					}
					else
					{
						break;
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == TOPIC_PRESENTATION_QOS)
		{
			if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			{
				if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				{
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
								(node->value->info->instance_state & a_condition->instance_state_mask))
						{
							if(max_sample > data_values->getSize())
							{
								node->value->info->sample_state	= READ_SAMPLE_STATE;
								node->value->info->view_state		= NOT_NEW_VIEW_STATE;

								data_values->insertInRear(node->value->value);
								sample_infos->insertInRear(node->value->info);

								data_sample_list.cutNode(node); // 읽은 노드 삭제
								delete(node->value);
								delete(node);
							}
							else
							{
								break;
							}
						}
					}
				}
				else // Source_timestamp를 고려하여 List에 저장
				{
					Time_t 	cTime;
					int			count;
					int			j;

					// 상식적으로 시스템 시작하고 0초에 데이터를 쏠순 없으니 0,0으로 초기화 하였음
					cTime.sec			= 0;
					cTime.nanosec		= 0;
					j						= 0;

					while(max_sample > j++)
					{
						while(data_sample_list.getSize() > i++)
						{
							node = node->rear;

							if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
									(node->value->info->instance_state & a_condition->instance_state_mask))
							{
								if((cTime.sec == 0) && (cTime.nanosec == 0))
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count					= i;
								}
								else if(cTime.sec > node->value->info->source_timestamp.sec)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count					= i;
								}
								else if(cTime.sec == node->value->info->source_timestamp.sec)
								{
									if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
									{
										cTime.sec			= node->value->info->source_timestamp.sec;
										cTime.nanosec		= node->value->info->source_timestamp.nanosec;
										count					= i;
									}
								}
							}
						}

						node = data_sample_list.getNodeByIndex(count);

						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);

						data_sample_list.cutNode(node);
						delete(node->value);
						delete(node);

						node = data_sample_list.getNodeByIndex(0);

						i						= 0;
						cTime.sec 			= 0;
						cTime.nanosec		= 0;

					}
				}
			}
			else // false
			 {
				while(data_sample_list.getSize() > i++)
				{
					node = node->rear;

					if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
							(node->value->info->instance_state & a_condition->instance_state_mask))
					{
						if(max_sample > data_values->getSize())
						{
							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_values->insertInRear(node->value->value);
							sample_infos->insertInRear(node->value->info);

							data_sample_list.cutNode(node);
							delete(node->value);
							delete(node);
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == GROUP_PRESENTATION_QOS)
		{
			// presentation qos가 group이며 ordered_access가 true인 경우에는 최대 1개의 샘플을 반환한다.
			 if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			 {
				 if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				 {
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

						 if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
								 (node->value->info->instance_state & a_condition->instance_state_mask))
						 {
							 node->value->info->sample_state	= READ_SAMPLE_STATE;
							 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							 data_values->insertInRear(node->value->value);
							 sample_infos->insertInRear(node->value->info);

							 data_sample_list.cutNode(node);
							 delete(node->value);
							 delete(node);
						 }
					 }
				 }
				 else // destination_order.kind == SOURCE_TIMESTAMP
				 {
					 Time_t 	cTime;
					 int			count;

					 // 상식적으로 시스템 시작하고 0초에 데이터를 쏠 순 없을테니 0,0으로 초기화 하였음
					 cTime.sec			= 0;
					 cTime.nanosec	= 0;

					 // 가장 작은 값을 찾음
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

						 if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
								 (node->value->info->instance_state & a_condition->instance_state_mask))
						 {
							 if((cTime.sec == 0) && (cTime.nanosec == 0))
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec > node->value->info->source_timestamp.sec)
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec == node->value->info->source_timestamp.sec)
							 {
								 if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
								 {
									 cTime.sec			= node->value->info->source_timestamp.sec;
									 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
									 count				= i;
								 }
							 }
						 }
					 }

					 node = data_sample_list.getNodeByIndex(count);

					 node->value->info->sample_state	= READ_SAMPLE_STATE;
					 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					 data_values->insertInRear(node->value->value);
					 sample_infos->insertInRear(node->value->info);

					 data_sample_list.cutNode(node);
					 delete(node->value);
					 delete(node);
				 }
			 }
			 else // Ordered_access = false
			 {
				 while(data_sample_list.getSize() > i++)
				 {
					 node = node->rear;

					 if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
							 (node->value->info->instance_state & a_condition->instance_state_mask))
					 {
						 if(max_sample > data_values->getSize())
						 {
							 node->value->info->sample_state	= READ_SAMPLE_STATE;
							 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							 data_values->insertInRear(node->value->value);
							 sample_infos->insertInRear(node->value->info);

							 data_sample_list.cutNode(node);
							 delete(node->value);
							 delete(node);
						 }
						 else
						 {
							 break;
						 }
					 }
				 }
			 }
		}

		read_flag	= false;

		if(data_values->getSize() > 0)
		{
			result		= RETCODE_OK;
		}
		else
		{
			result		 = RETCODE_NO_DATA;
		}

		// rtps 구현 필요
		// 여기서 sample_info의 다양한?? rank 값들 설정해줘야 함
	}

	// 아직 접근하지 않은 DataReader의 Data를 접근하여 복사함, SampleInfo 또한 복사해야 함
	// read Operation과 흡사하지만 max_samples, Sample_state와 view_state, instance_state가 고정된 것이 다른 점.
	// max_sample = 1, sample_state = NOT_READ, view_state = ANY_VIEW, instance_state = ANY_INSTANACE
	// read_next_sample과 동일하며 다른 점은 읽은 이후 노드를 삭제한다는 것
	ReturnCode_t		DataReader::take_next_sample(DDS_Data* data_value, SampleInfo* sample_info)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(!is_enabled)
		{
			result		=	RETCODE_NOT_ENABLED;
			return	result;
		}

		read_flag	 = true; // READING - Subscriber에서 데이터를 읽는 중인지 판별하기 위해서 추가.

		int							i		= 0;
		Node<DataSample>* node = data_sample_list.getNodeByIndex(0);


		if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == INSTANCE_PRESENTATION_QOS)
		{
			while(data_sample_list.getSize() > i++)
			{
				node = node->rear;

				if( (node->value->info->sample_state & NOT_READ_SAMPLE_STATE) && (node->value->info->view_state & ANY_VIEW_STATE) &&
						(node->value->info->instance_state & ANY_INSTANCE_STATE))
				{
						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_value= node->value->value;
						sample_info = node->value->info;

						data_sample_list.cutNode(node); // 읽은 노드 삭제
						delete(node->value);
						delete(node);
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == TOPIC_PRESENTATION_QOS)
		{
			if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			{
				if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				{
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & NOT_READ_SAMPLE_STATE) && (node->value->info->view_state & ANY_VIEW_STATE) &&
								(node->value->info->instance_state & ANY_INSTANCE_STATE))
						{
							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_value= node->value->value;
							sample_info = node->value->info;

							data_sample_list.cutNode(node); // 읽은 노드 삭제
							delete(node->value);
							delete(node);
						}
					}
				}
				else // Source_timestamp를 고려하여 List에 저장
				{
					Time_t 	cTime;
					int			count;

					// 상식적으로 시스템 시작하고 0초에 데이터를 쏠순 없으니 0,0으로 초기화 하였음
					cTime.sec			= 0;
					cTime.nanosec		= 0;

					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & NOT_READ_SAMPLE_STATE) && (node->value->info->view_state & ANY_VIEW_STATE) &&
								(node->value->info->instance_state & ANY_INSTANCE_STATE))
						{
							if((cTime.sec == 0) && (cTime.nanosec == 0))
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;
								count					= i;
							}
							else if(cTime.sec > node->value->info->source_timestamp.sec)
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;
								count					= i;
							}
							else if(cTime.sec == node->value->info->source_timestamp.sec)
							{
								if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count					= i;
								}
							}
						}
					}

					node = data_sample_list.getNodeByIndex(count);

					node->value->info->sample_state	= READ_SAMPLE_STATE;
					node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					data_value= node->value->value;
					sample_info = node->value->info;

					data_sample_list.cutNode(node); // 읽은 노드 삭제
					delete(node->value);
					delete(node);
				}
			}
			else // false
			 {
				while(data_sample_list.getSize() > i++)
				{
					node = node->rear;

					if( (node->value->info->sample_state & NOT_READ_SAMPLE_STATE) && (node->value->info->view_state & ANY_VIEW_STATE) &&
							(node->value->info->instance_state & ANY_INSTANCE_STATE))
					{
						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_value= node->value->value;
						sample_info = node->value->info;

						data_sample_list.cutNode(node); // 읽은 노드 삭제
						delete(node->value);
						delete(node);
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == GROUP_PRESENTATION_QOS)
		{
			// presentation qos가 group이며 ordered_access가 true인 경우에는 최대 1개의 샘플을 반환한다.
			 if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			 {
				 if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				 {
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

						 if( (node->value->info->sample_state & NOT_READ_SAMPLE_STATE) && (node->value->info->view_state & ANY_VIEW_STATE) &&
								 (node->value->info->instance_state & ANY_INSTANCE_STATE))
						 {
							 node->value->info->sample_state	= READ_SAMPLE_STATE;
							 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							 data_value= node->value->value;
							 sample_info = node->value->info;

							 data_sample_list.cutNode(node); // 읽은 노드 삭제
							 delete(node->value);
							 delete(node);
						 }
					 }
				 }
				 else // destination_order.kind == SOURCE_TIMESTAMP
				 {
					 Time_t 	cTime;
					 int			count;

					 // 상식적으로 시스템 시작하고 0초에 데이터를 쏠 순 없을테니 0,0으로 초기화 하였음
					 cTime.sec			= 0;
					 cTime.nanosec	= 0;

					 // 가장 작은 값을 찾음
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

						 if( (node->value->info->sample_state & NOT_READ_SAMPLE_STATE) && (node->value->info->view_state & ANY_VIEW_STATE) &&
								 (node->value->info->instance_state & ANY_INSTANCE_STATE))
						 {
							 if((cTime.sec == 0) && (cTime.nanosec == 0))
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec > node->value->info->source_timestamp.sec)
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec == node->value->info->source_timestamp.sec)
							 {
								 if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
								 {
									 cTime.sec			= node->value->info->source_timestamp.sec;
									 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
									 count				= i;
								 }
							 }
						 }
					 }

					 node = data_sample_list.getNodeByIndex(count);

					 node->value->info->sample_state	= READ_SAMPLE_STATE;
					 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					 data_value= node->value->value;
					 sample_info = node->value->info;

					 data_sample_list.cutNode(node);
					 delete(node->value);
					 delete(node);
				 }
			 }
			 else // Ordered_access = false
			 {
				 while(data_sample_list.getSize() > i++)
				 {
					 node = node->rear;

					 if( (node->value->info->sample_state & NOT_READ_SAMPLE_STATE) && (node->value->info->view_state & ANY_VIEW_STATE) &&
							 (node->value->info->instance_state & ANY_INSTANCE_STATE))
					 {
						 node->value->info->sample_state	= READ_SAMPLE_STATE;
						 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						 data_value= node->value->value;
						 sample_info = node->value->info;

						 data_sample_list.cutNode(node); // 읽은 노드 삭제
						 delete(node->value);
						 delete(node);
					 }
				 }
			 }
		}

		read_flag	= false;

		if(data_value != NULL)
		{
			result		= RETCODE_OK;
		}
		else
		{
			result		 = RETCODE_NO_DATA;
		}

		// rtps 구현 필요
		// 여기서 sample_info의 다양한?? rank 값들 설정해줘야 함
	}

	// read 함수와 동일하며 instance를 확인하는 것이 다르다.
	ReturnCode_t		DataReader::read_instance(DataList* data_values, SampleInfoList* sample_infos, long max_sample, InstanceHandle_t a_handle, SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states)
	{
		ReturnCode_t	result	= RETCODE_OK;

		bool	Invalid_Flag = false;

		if(!is_enabled)
		{
			result		=	RETCODE_NOT_ENABLED;
			return	result;
		}

		if(max_sample == 0)		// Zero-copy인 경우 return_loan을 호출해야 함 , 좀 더 생각 필요
		{
			result		= return_loan(data_values, sample_infos);
			return 	result;
		}

		read_flag	 = true; // READING - Subscriber에서 데이터를 읽는 중인지 판별하기 위해서 추가.

		int							i		= 0;
		Node<DataSample>* node = data_sample_list.getNodeByIndex(0);


		if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == INSTANCE_PRESENTATION_QOS)
		{
			while(data_sample_list.getSize() > i++)
			{
				node = node->rear;

				// 넘어오는 파라메터 3가지 조건을 모두 만족하는 Data, SampleInfo인 경우 데이터를 읽음
				if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
						(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == a_handle))
				{
					Invalid_Flag = true;

					if(max_sample > data_values->getSize()) // 최대 사이즈만큼 데이터를 읽음
					{
						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);
					}
					else // 읽을 만큼 모두 읽었으면 while루프 탈출... 루프 탈출 이후 RTPS를 이용해서 설정할 부분과 return 처리를 함
					{
						break;
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == TOPIC_PRESENTATION_QOS)
		{
			if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			{
				if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				{
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == a_handle))
						{
							Invalid_Flag = true;

							if(max_sample > data_values->getSize())
							{
								node->value->info->sample_state	= READ_SAMPLE_STATE;
								node->value->info->view_state		= NOT_NEW_VIEW_STATE;

								data_values->insertInRear(node->value->value);
								sample_infos->insertInRear(node->value->info);
							}
							else
							{
								break;
							}
						}
					}
				}
				else // Source_timestamp를 고려하여 List에 저장
				{
					Time_t 	cTime; // Source_timestamp값을 비교하기 위한 변수
					bool 		cFlag; // 읽었던 데이터인지 확인
					//int			count[max_sample]; // 읽어야 할 데이터들의 노드 Index를 저장
					int*		count	= new int[max_sample];
					int	 		j ;

					// 상식적으로 시스템 시작하고 0초에 데이터를 쏠순 없으니 0,0으로 초기화 하였음
					cTime.sec			= 0;
					cTime.nanosec		= 0;

					// 가장 작은 Source_Timestamp 값을 찾음
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == a_handle))
						{
							Invalid_Flag = true;

							if((cTime.sec == 0) && (cTime.nanosec == 0)) // 초기값 설정
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;
								count[0]				= i;
							}
							else if(cTime.sec > node->value->info->source_timestamp.sec)
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;
								count[0]				= i;
							}
							else if(cTime.sec == node->value->info->source_timestamp.sec)
							{
								if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count[0]				= i;
								}
							}
						}
					}

					i		= 0;
					j		= 0;
					cFlag	= true;
					cTime.sec			= 0;
					cTime.nanosec		= 0;

					while(max_sample-1 > j++)
					{
						node = data_sample_list.getNodeByIndex(0); // 모든 노드를 검색하기 위해서 노드의 처음부터 시작

						while(data_sample_list.getSize() > i++)
						{
							node = node->rear;

							if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
									(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == a_handle))
							{
								int		k;
								for(k = 0; k < j; k++) // 이전에 등록되어 있는 값이면 비교할 필요가 없으므로 체크
								{
									if(count[k] == i) // 이전에 등록 되었으면 Flag값을 변경
									{
										cFlag	= false;
										break;
									}
								}

								if((cTime.sec == 0) && (cTime.nanosec == 0) && cFlag)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count[j]				= i;
								}

								if(cTime.sec > node->value->info->source_timestamp.sec && cFlag)
								{
									cTime.sec 			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count[j]				= i;
								}
								else if(cTime.sec == node->value->info->source_timestamp.sec && cFlag)
								{
									if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
									{
										cTime.sec 			= node->value->info->source_timestamp.sec;
										cTime.nanosec		= node->value->info->source_timestamp.nanosec;
										count[j]				= i;
									}
								}
							}

							cFlag = true;
						}
					}

					for(i = 0; i < max_sample; i++) // 위에서 저장한 노드의 Index를 이용해서 데이터를 읽음
					{
						node = data_sample_list.getNodeByIndex(count[i]);

						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);
					}
				}
			}
			else // Ordered_access = false
			 {
				while(data_sample_list.getSize() > i++)
				{
					node = node->rear;

					if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
							(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == a_handle))
					{
						Invalid_Flag = true;

						if(max_sample > data_values->getSize())
						{
							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_values->insertInRear(node->value->value);
							sample_infos->insertInRear(node->value->info);
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == GROUP_PRESENTATION_QOS)
		{
			// presentation qos가 group이며 ordered_access가 true인 경우에는 최대 1개의 샘플을 반환한다.
			if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			{
				if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				{
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == a_handle))
						{
							Invalid_Flag = true;

							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_values->insertInRear(node->value->value);
							sample_infos->insertInRear(node->value->info);

							break;
						}
					 }
				 }
				 else // destination_order.kind == SOURCE_TIMESTAMP
				 {
					 Time_t 	cTime;
					 int		count; // Data, SampleInfo 하나만 얻으면 되므로 배열을 이용하지 않음

					 // 상식적으로 시스템 시작하고 0초에 데이터를 쏠 순 없을테니 0,0으로 초기화 하였음
					 cTime.sec			= 0;
					 cTime.nanosec	= 0;

					 // 가장 작은 값을 찾음
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

						 if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								 (node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == a_handle))
						 {
							 Invalid_Flag = true;

							 if((cTime.sec == 0) && (cTime.nanosec == 0))
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec > node->value->info->source_timestamp.sec)
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec == node->value->info->source_timestamp.sec)
							 {
								 if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
								 {
									 cTime.sec			= node->value->info->source_timestamp.sec;
									 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
									 count				= i;
								 }
							 }
						 }
					 }

					 node = data_sample_list.getNodeByIndex(count);

					 node->value->info->sample_state	= READ_SAMPLE_STATE;
					 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					 data_values->insertInRear(node->value->value);
					 sample_infos->insertInRear(node->value->info);
				 }
			}
			else // Ordered_access = false
			{
				while(data_sample_list.getSize() > i++)
				{
					node = node->rear;

					if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
							(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == a_handle))
					{
						Invalid_Flag = true;

						if(max_sample > data_values->getSize())
						{
							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_values->insertInRear(node->value->value);
							sample_infos->insertInRear(node->value->info);
						 }
						 else
						 {
							 break;
						 }
					 }
				 }
			 }
		}

		read_flag	= false;

		// 데이터를 저장한 리스트의 크기가 0보다 크다면 데이터를 읽은 것임.. 하지만 애초에 넘어오는 리스트의 크기가 0이라는 것은 보장할 수 없으므로 대안이 필요함
		// 무식하게 하면 애초에 리스트 크기를 저장하였다가 비교하거나 데이터를 읽는 곳에 result = RETCODE_OK를 넣는 방법이 있겠음..
		if(Invalid_Flag == false)
		{
			result		= RETCODE_BAD_PARAMETER;
		}

		if(data_values->getSize() > 0)
		{
			result		= RETCODE_OK;
		}
		else // 데이터를 찾지 못했으면(아예 데이터를 하나도 읽지 못한 경우)
		{
			result		 = RETCODE_NO_DATA;
		}

		return	result;
		// rtps 구현 필요
		// 여기서 sample_info의 다양한?? rank 값들 설정해줘야 함
	}

	// Ordered_access를 통해서 Handle 조건 만족한 대상에 대해서 Timestamp 계산하는 것으로 구현함
	ReturnCode_t		DataReader::read_next_instance(DataList* data_values, SampleInfoList* sample_infos, long max_sample, InstanceHandle_t previous_handle, SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(!is_enabled)
		{
			result		=	RETCODE_NOT_ENABLED;
			return	result;
		}

		if(max_sample == 0)		// Zero-copy인 경우 return_loan을 호출해야 함 , 좀 더 생각 필요
		{
			result		= return_loan(data_values, sample_infos);
			return 	result;
		}

		read_flag = true;

		int							i		= 0;
		Node<DataSample>* node	= data_sample_list.getNodeByIndex(0);

		InstanceHandle_t	smallest_handle = 0;


		// previous_handle보다는 크고 가장 작은 Handle을 찾음
		while(data_sample_list.getSize() > i++)
		{
			node	= node->rear;

			if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
					(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle > previous_handle))
			{
				if(smallest_handle == 0)
				{
					smallest_handle = node->value->info->instance_handle;
				}
				else if(smallest_handle > node->value->info->instance_handle)
				{
					smallest_handle = node->value->info->instance_handle;
				}
			}
		}

		node	= data_sample_list.getNodeByIndex(0);
		i		= 0;

		if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == INSTANCE_PRESENTATION_QOS)
		{
			while(data_sample_list.getSize() > i++)
			{
				node = node->rear;

				// 넘어오는 파라메터 3가지 조건을 모두 만족하는 Data, SampleInfo인 경우 데이터를 읽음
				if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
						(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == smallest_handle))
				{
					if(max_sample > data_values->getSize()) // 최대 사이즈만큼 데이터를 읽음
					{
						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);
					}
					else // 읽을 만큼 모두 읽었으면 while루프 탈출... 루프 탈출 이후 RTPS를 이용해서 설정할 부분과 return 처리를 함
					{
						break;
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == TOPIC_PRESENTATION_QOS)
		{
			if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			{
				if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				{
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == smallest_handle))
						{
							if(max_sample > data_values->getSize())
							{
								node->value->info->sample_state	= READ_SAMPLE_STATE;
								node->value->info->view_state		= NOT_NEW_VIEW_STATE;

								data_values->insertInRear(node->value->value);
								sample_infos->insertInRear(node->value->info);
							}
							else
							{
								break;
							}
						}
					}
				}
				else // Source_timestamp를 고려하여 List에 저장
				{
					Time_t	cTime;
					bool		cFlag;
					int			index;
					int			max_len;
					int			j;

					cTime.sec			= 0;
					cTime.nanosec		= 0;

					// 최소값을 찾기
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						// State를 만족하는 데이터 중 TimeStamp를 고려해야 함
						if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == smallest_handle))
						{
							if((cTime.sec == 0) && (cTime.nanosec == 0))
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;

								index	= i;
								max_len++;
							}
							else if(cTime.sec > node->value->info->source_timestamp.sec)
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;

								index	= i;
								max_len++;
							}
							else if( (cTime.sec == node->value->info->source_timestamp.sec) && (cTime.nanosec > node->value->info->source_timestamp.nanosec))
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;

								index	= i;
								max_len++;
							}
						}
					}

					// 가장 작은 데이터가 들어있는 Index로 노드를 이동시켜서 해당 노드의 데이터를 저장
					node	=	data_sample_list.getNodeByIndex(index);

					node->value->info->sample_state	= READ_SAMPLE_STATE;
					node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					data_values->insertInRear(node->value->value);
					sample_infos->insertInRear(node->value->info);

					// 실제로 읽을 수 있는 데이터의 크기보다 제한된 크기보다 크다면
					// 최대 크기는 5인데 읽을 수 있는 데이터가 이보다 작다면 최대 크기만큼 데이터를 담을 수 없으므로 의미없는 루프 돌리지 않아도 됨

					if(max_sample <= max_len)
					{
						max_len = max_sample;
					}

					//int		dup_data[max_len];
					int*	dup_data	= new int[max_len];
					int		k;
					dup_data[0] = index;

					cFlag 	= true;
					i			= 0;
					j			= 0;

					cTime.sec			= 0;
					cTime.nanosec		= 0;

					// 가장 작은 값 보다 크지만 나머지 중에서 가장 작은 값들을 차례로 List에 저장
					while(max_len-1 > j++)
					{
						node		=  data_sample_list.getNodeByIndex(0);
						cFlag		=  true;

						while(data_sample_list.getSize() > i++)
						{
							node = node->rear;
							if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
									(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == smallest_handle))
							{
								for(k = 0; k < j; k++) // 이전에 저장했던 Index와 겹친다면 중복되는 것이므로 체크, Flag값이 True라면 겹치지 않는 데이터라는 의미
								{
									if(dup_data[k] == i)
									{
										cFlag = false;
										break;
									}
								}

								if((cTime.sec == 0) && (cTime.nanosec == 0) && cFlag)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;

									dup_data[j] = i;
								}
								else if(cTime.sec > node->value->info->source_timestamp.sec && cFlag)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;

									dup_data[j] = i;
								}
								else if( (cTime.sec == node->value->info->source_timestamp.sec) && (cTime.nanosec > node->value->info->source_timestamp.nanosec) && cFlag)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;

									dup_data[j] = i;
								}
							}
						}

						node	=	data_sample_list.getNodeByIndex(dup_data[j]);

						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);
					}
				}
			}
			else // Ordered_access = false
			 {
				while(data_sample_list.getSize() > i++)
				{
					node = node->rear;

					if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
							(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == smallest_handle))
					{
						if(max_sample > data_values->getSize())
						{
							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_values->insertInRear(node->value->value);
							sample_infos->insertInRear(node->value->info);
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == GROUP_PRESENTATION_QOS)
		{
			// presentation qos가 group이며 ordered_access가 true인 경우에는 최대 1개의 샘플을 반환한다.
			 if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			 {
				 if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				 {
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

							if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
									(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == smallest_handle))
						 {
							 node->value->info->sample_state	= READ_SAMPLE_STATE;
							 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							 data_values->insertInRear(node->value->value);
							 sample_infos->insertInRear(node->value->info);

							 break;
						 }
					 }
				 }
				 else // destination_order.kind == SOURCE_TIMESTAMP
				 {
					 Time_t 	cTime;
					 int		count; // Data, SampleInfo 하나만 얻으면 되므로 배열을 이용하지 않음

					 // 상식적으로 시스템 시작하고 0초에 데이터를 쏠 순 없을테니 0,0으로 초기화 하였음
					 cTime.sec			= 0;
					 cTime.nanosec	= 0;

					 // 가장 작은 값을 찾음
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

						 if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								 (node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == smallest_handle))
						 {
							 if((cTime.sec == 0) && (cTime.nanosec == 0))
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec > node->value->info->source_timestamp.sec)
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec == node->value->info->source_timestamp.sec)
							 {
								 if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
								 {
									 cTime.sec			= node->value->info->source_timestamp.sec;
									 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
									 count				= i;
								 }
							 }
						 }
					 }

					 node = data_sample_list.getNodeByIndex(count);

					 node->value->info->sample_state	= READ_SAMPLE_STATE;
					 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					 data_values->insertInRear(node->value->value);
					 sample_infos->insertInRear(node->value->info);
				 }
			 }
			 else // Ordered_access = false
			 {
				 while(data_sample_list.getSize() > i++)
				 {
					 node = node->rear;

					 if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
							 (node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == smallest_handle))
					 {
						 if(max_sample > data_values->getSize())
						 {
							 node->value->info->sample_state	= READ_SAMPLE_STATE;
							 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							 data_values->insertInRear(node->value->value);
							 sample_infos->insertInRear(node->value->info);
						 }
						 else
						 {
							 break;
						 }
					 }
				 }
			 }
		}

		read_flag	= false;

		// 데이터를 저장한 리스트의 크기가 0보다 크다면 데이터를 읽은 것임.. 하지만 애초에 넘어오는 리스트의 크기가 0이라는 것은 보장할 수 없으므로 대안이 필요함
		// 무식하게 하면 애초에 리스트 크기를 저장하였다가 비교하거나 데이터를 읽는 곳에 result = RETCODE_OK를 넣는 방법이 있겠음..
		if(data_values->getSize() > 0)
		{
			result		= RETCODE_OK;
		}
		else // 데이터를 찾지 못했으면(아예 데이터를 하나도 읽지 못한 경우)
		{
			result		 = RETCODE_NO_DATA;
		}

		// rtps 구현 필요
		// 여기서 sample_info의 다양한?? rank 값들 설정해줘야 함
	}

	ReturnCode_t		DataReader::read_next_instance_w_condition(DataList* data_values, SampleInfoList* sample_infos, long max_sample, InstanceHandle_t previous_handle, ReadCondition* a_condition)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(!is_enabled)
		{
			result		=	RETCODE_NOT_ENABLED;
			return	result;
		}

		if(max_sample == 0)		// Zero-copy인 경우 return_loan을 호출해야 함 , 좀 더 생각 필요
		{
			result		= return_loan(data_values, sample_infos);
			return 	result;
		}

		read_flag = true;

		int							i		= 0;
		Node<DataSample>* node	= data_sample_list.getNodeByIndex(0);

		InstanceHandle_t	smallest_handle = 0;


		// previous_handle보다는 크고 가장 작은 Handle을 찾음
		while(data_sample_list.getSize() > i++)
		{
			node	= node->rear;

			if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
					(node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
			{
				if(smallest_handle == 0)
				{
					smallest_handle = node->value->info->instance_handle;
				}
				else if(smallest_handle > node->value->info->instance_handle)
				{
					smallest_handle = node->value->info->instance_handle;
				}
			}
		}

		node	= data_sample_list.getNodeByIndex(0);
		i		= 0;

		if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == INSTANCE_PRESENTATION_QOS)
		{
			while(data_sample_list.getSize() > i++)
			{
				node = node->rear;

				// 넘어오는 파라메터 3가지 조건을 모두 만족하는 Data, SampleInfo인 경우 데이터를 읽음
				if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
						(node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
				{
					if(max_sample > data_values->getSize()) // 최대 사이즈만큼 데이터를 읽음
					{
						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);
					}
					else // 읽을 만큼 모두 읽었으면 while루프 탈출... 루프 탈출 이후 RTPS를 이용해서 설정할 부분과 return 처리를 함
					{
						break;
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == TOPIC_PRESENTATION_QOS)
		{
			if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			{
				if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				{
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
								(node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
						{
							if(max_sample > data_values->getSize())
							{
								node->value->info->sample_state	= READ_SAMPLE_STATE;
								node->value->info->view_state		= NOT_NEW_VIEW_STATE;

								data_values->insertInRear(node->value->value);
								sample_infos->insertInRear(node->value->info);
							}
							else
							{
								break;
							}
						}
					}
				}
				else // Source_timestamp를 고려하여 List에 저장
				{
					Time_t 	cTime; // Source_timestamp값을 비교하기 위한 변수
					bool 		cFlag; // 읽었던 데이터인지 확인
					//int			count[max_sample]; // 읽어야 할 데이터들의 노드 Index를 저장
					int*		count	= new int[max_sample];
					int	 		j ;

					// 상식적으로 시스템 시작하고 0초에 데이터를 쏠순 없으니 0,0으로 초기화 하였음
					cTime.sec			= 0;
					cTime.nanosec		= 0;

					// 가장 작은 Source_Timestamp 값을 찾음
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
								(node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
						{
							if((cTime.sec == 0) && (cTime.nanosec == 0)) // 초기값 설정
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;
								count[0]				= i;
							}
							else if(cTime.sec > node->value->info->source_timestamp.sec)
							{
								cTime.sec			= node->value->info->source_timestamp.sec;
								cTime.nanosec		= node->value->info->source_timestamp.nanosec;
								count[0]				= i;
							}
							else if(cTime.sec == node->value->info->source_timestamp.sec)
							{
								if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count[0]				= i;
								}
							}
						}
					}

					i		= 0;
					j		= 0;
					cFlag	= true;
					cTime.sec			= 0;
					cTime.nanosec		= 0;

					while(max_sample-1 > j++)
					{
						node = data_sample_list.getNodeByIndex(0); // 모든 노드를 검색하기 위해서 노드의 처음부터 시작

						while(data_sample_list.getSize() > i++)
						{
							node = node->rear;

							if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
									(node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
							{
								int		k;
								for(k = 0; k < j; k++) // 이전에 등록되어 있는 값이면 비교할 필요가 없으므로 체크
								{
									if(count[k] == i) // 이전에 등록 되었으면 Flag값을 변경
									{
										cFlag	= false;
										break;
									}
								}

								if((cTime.sec == 0) && (cTime.nanosec == 0) && cFlag)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count[j]				= i;
								}

								if(cTime.sec > node->value->info->source_timestamp.sec && cFlag)
								{
									cTime.sec 			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count[j]				= i;
								}
								else if(cTime.sec == node->value->info->source_timestamp.sec && cFlag)
								{
									if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
									{
										cTime.sec 			= node->value->info->source_timestamp.sec;
										cTime.nanosec		= node->value->info->source_timestamp.nanosec;
										count[j]				= i;
									}
								}
							}

							cFlag = true;
						}
					}

					for(i = 0; i < max_sample; i++) // 위에서 저장한 노드의 Index를 이용해서 데이터를 읽음
					{
						node = data_sample_list.getNodeByIndex(count[i]);

						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);
					}
				}
			}
			else // Ordered_access = false
			 {
				while(data_sample_list.getSize() > i++)
				{
					node = node->rear;

					if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
							(node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
					{
						if(max_sample > data_values->getSize())
						{
							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_values->insertInRear(node->value->value);
							sample_infos->insertInRear(node->value->info);
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == GROUP_PRESENTATION_QOS)
		{
			// presentation qos가 group이며 ordered_access가 true인 경우에는 최대 1개의 샘플을 반환한다.
			 if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			 {
				 if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				 {
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

							if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
									(node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
						 {
							 node->value->info->sample_state	= READ_SAMPLE_STATE;
							 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							 data_values->insertInRear(node->value->value);
							 sample_infos->insertInRear(node->value->info);

							 break;
						 }
					 }
				 }
				 else // destination_order.kind == SOURCE_TIMESTAMP
				 {
					 Time_t 	cTime;
					 int		count; // Data, SampleInfo 하나만 얻으면 되므로 배열을 이용하지 않음

					 // 상식적으로 시스템 시작하고 0초에 데이터를 쏠 순 없을테니 0,0으로 초기화 하였음
					 cTime.sec			= 0;
					 cTime.nanosec	= 0;

					 // 가장 작은 값을 찾음
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

							if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
									(node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
						 {
							 if((cTime.sec == 0) && (cTime.nanosec == 0))
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec > node->value->info->source_timestamp.sec)
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec == node->value->info->source_timestamp.sec)
							 {
								 if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
								 {
									 cTime.sec			= node->value->info->source_timestamp.sec;
									 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
									 count				= i;
								 }
							 }
						 }
					 }

					 node = data_sample_list.getNodeByIndex(count);

					 node->value->info->sample_state	= READ_SAMPLE_STATE;
					 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					 data_values->insertInRear(node->value->value);
					 sample_infos->insertInRear(node->value->info);
				 }
			 }
			 else // Ordered_access = false
			 {
				 while(data_sample_list.getSize() > i++)
				 {
					 node = node->rear;

						if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
								(node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
					 {
						 if(max_sample > data_values->getSize())
						 {
							 node->value->info->sample_state	= READ_SAMPLE_STATE;
							 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							 data_values->insertInRear(node->value->value);
							 sample_infos->insertInRear(node->value->info);
						 }
						 else
						 {
							 break;
						 }
					 }
				 }
			 }
		}

		read_flag	= false;

		// 데이터를 저장한 리스트의 크기가 0보다 크다면 데이터를 읽은 것임.. 하지만 애초에 넘어오는 리스트의 크기가 0이라는 것은 보장할 수 없으므로 대안이 필요함
		// 무식하게 하면 애초에 리스트 크기를 저장하였다가 비교하거나 데이터를 읽는 곳에 result = RETCODE_OK를 넣는 방법이 있겠음..
		if(data_values->getSize() > 0)
		{
			result		= RETCODE_OK;
		}
		else // 데이터를 찾지 못했으면(아예 데이터를 하나도 읽지 못한 경우)
		{
			result		 = RETCODE_NO_DATA;
		}

		// rtps 구현 필요
		// 여기서 sample_info의 다양한?? rank 값들 설정해줘야 함
	}

	ReturnCode_t		DataReader::take_instance(DataList* data_values, SampleInfoList* sample_infos, long max_sample, InstanceHandle_t a_handle, SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states)
	{
		ReturnCode_t	result	= RETCODE_OK;

		bool	Invalid_Flag = false;

		if(!is_enabled)
		{
			result		=	RETCODE_NOT_ENABLED;
			return	result;
		}

		if(max_sample == 0)		// Zero-copy인 경우 return_loan을 호출해야 함 , 좀 더 생각 필요
		{
			result		= return_loan(data_values, sample_infos);
			return 	result;
		}

		read_flag	 = true; // READING - Subscriber에서 데이터를 읽는 중인지 판별하기 위해서 추가.

		int							i		= 0;
		Node<DataSample>* node = data_sample_list.getNodeByIndex(0);


		if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == INSTANCE_PRESENTATION_QOS)
		{
			while(data_sample_list.getSize() > i++)
			{
				node = node->rear;

				if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
						(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == a_handle))
				{
					Invalid_Flag = true;

					if(max_sample > data_values->getSize())
					{
						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);

						data_sample_list.cutNode(node); // 읽은 노드 삭제
						delete(node->value);
						delete(node);
					}
					else
					{
						break;
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == TOPIC_PRESENTATION_QOS)
		{
			if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			{
				if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				{
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == a_handle))
						{
							Invalid_Flag = true;

							if(max_sample > data_values->getSize())
							{
								node->value->info->sample_state	= READ_SAMPLE_STATE;
								node->value->info->view_state		= NOT_NEW_VIEW_STATE;

								data_values->insertInRear(node->value->value);
								sample_infos->insertInRear(node->value->info);

								data_sample_list.cutNode(node); // 읽은 노드 삭제
								delete(node->value);
								delete(node);
							}
							else
							{
								break;
							}
						}
					}
				}
				else // Source_timestamp를 고려하여 List에 저장
				{
					Time_t 	cTime;
					int			count;
					int			j;

					// 상식적으로 시스템 시작하고 0초에 데이터를 쏠순 없으니 0,0으로 초기화 하였음
					cTime.sec			= 0;
					cTime.nanosec		= 0;
					j						= 0;

					while(max_sample > j++)
					{
						while(data_sample_list.getSize() > i++)
						{
							node = node->rear;

							if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
									(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == a_handle))
							{
								Invalid_Flag = true;

								if((cTime.sec == 0) && (cTime.nanosec == 0))
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count					= i;
								}
								else if(cTime.sec > node->value->info->source_timestamp.sec)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count					= i;
								}
								else if(cTime.sec == node->value->info->source_timestamp.sec)
								{
									if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
									{
										cTime.sec			= node->value->info->source_timestamp.sec;
										cTime.nanosec		= node->value->info->source_timestamp.nanosec;
										count					= i;
									}
								}
							}
						}

						node = data_sample_list.getNodeByIndex(count);

						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);

						data_sample_list.cutNode(node);
						delete(node->value);
						delete(node);

						node = data_sample_list.getNodeByIndex(0);

						i						= 0;
						cTime.sec			= 0;
						cTime.nanosec		= 0;
					}
				}
			}
			else // false
			 {
				while(data_sample_list.getSize() > i++)
				{
					node = node->rear;

					if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
							(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == a_handle))
					{
						Invalid_Flag = true;

						if(max_sample > data_values->getSize())
						{
							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_values->insertInRear(node->value->value);
							sample_infos->insertInRear(node->value->info);

							data_sample_list.cutNode(node);
							delete(node->value);
							delete(node);
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == GROUP_PRESENTATION_QOS)
		{
			// presentation qos가 group이며 ordered_access가 true인 경우에는 최대 1개의 샘플을 반환한다.
			 if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			 {
				 if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				 {
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

						 if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								 (node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == a_handle))
						 {
							 Invalid_Flag = true;

							 node->value->info->sample_state	= READ_SAMPLE_STATE;
							 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							 data_values->insertInRear(node->value->value);
							 sample_infos->insertInRear(node->value->info);

							 data_sample_list.cutNode(node);
							 delete(node->value);
							 delete(node);

							 break;
						 }
					 }
				 }
				 else // destination_order.kind == SOURCE_TIMESTAMP
				 {
					 Time_t 	cTime;
					 int			count;

					 // 상식적으로 시스템 시작하고 0초에 데이터를 쏠 순 없을테니 0,0으로 초기화 하였음
					 cTime.sec			= 0;
					 cTime.nanosec	= 0;

					 // 가장 작은 값을 찾음
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

						 if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								 (node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == a_handle))
						 {
							 Invalid_Flag = true;

							 if((cTime.sec == 0) && (cTime.nanosec == 0))
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec > node->value->info->source_timestamp.sec)
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec == node->value->info->source_timestamp.sec)
							 {
								 if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
								 {
									 cTime.sec			= node->value->info->source_timestamp.sec;
									 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
									 count				= i;
								 }
							 }
						 }
					 }

					 node = data_sample_list.getNodeByIndex(count);

					 node->value->info->sample_state	= READ_SAMPLE_STATE;
					 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					 data_values->insertInRear(node->value->value);
					 sample_infos->insertInRear(node->value->info);

					 data_sample_list.cutNode(node);
					 delete(node->value);
					 delete(node);
				 }
			 }
			 else // Ordered_access = false
			 {
				 while(data_sample_list.getSize() > i++)
				 {
					 node = node->rear;

					 if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
							 (node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == a_handle))
					 {
						 Invalid_Flag = true;

						 if(max_sample > data_values->getSize())
						 {
							 node->value->info->sample_state	= READ_SAMPLE_STATE;
							 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							 data_values->insertInRear(node->value->value);
							 sample_infos->insertInRear(node->value->info);

							 data_sample_list.cutNode(node);
							 delete(node->value);
							 delete(node);
						 }
						 else
						 {
							 break;
						 }
					 }
				 }
			 }
		}

		read_flag	= false;

		if(Invalid_Flag == false)
		{
			result		= RETCODE_BAD_PARAMETER;
		}

		if(data_values->getSize() > 0)
		{
			result		= RETCODE_OK;
		}
		else
		{
			result		 = RETCODE_NO_DATA;
		}

		// rtps 구현 필요
		// 여기서 sample_info의 다양한?? rank 값들 설정해줘야 함

	}

	ReturnCode_t		DataReader::take_next_instance(DataList* data_values, SampleInfoList* sample_infos, long max_sample, InstanceHandle_t previous_handle, SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states)
	{
		ReturnCode_t	result	= RETCODE_OK;

		bool	Invalid_Flag = false;

		if(!is_enabled)
		{
			result		=	RETCODE_NOT_ENABLED;
			return	result;
		}

		if(max_sample == 0)		// Zero-copy인 경우 return_loan을 호출해야 함 , 좀 더 생각 필요
		{
			result		= return_loan(data_values, sample_infos);
			return 	result;
		}

		read_flag = true;

		int							i		= 0;
		Node<DataSample>* node	= data_sample_list.getNodeByIndex(0);

		InstanceHandle_t	smallest_handle = 0;


		// previous_handle보다는 크고 가장 작은 Handle을 찾음
		while(data_sample_list.getSize() > i++)
		{
			node	= node->rear;

			if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
					(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle > previous_handle))
			{
				if(smallest_handle == 0)
				{
					smallest_handle = node->value->info->instance_handle;
				}
				else if(smallest_handle > node->value->info->instance_handle)
				{
					smallest_handle = node->value->info->instance_handle;
				}
			}
		}

		node	= data_sample_list.getNodeByIndex(0);
		i		= 0;


		if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == INSTANCE_PRESENTATION_QOS)
		{
			while(data_sample_list.getSize() > i++)
			{
				node = node->rear;

				if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
						(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == smallest_handle))
				{
					Invalid_Flag = true;

					if(max_sample > data_values->getSize())
					{
						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);

						data_sample_list.cutNode(node); // 읽은 노드 삭제
						delete(node->value);
						delete(node);
					}
					else
					{
						break;
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == TOPIC_PRESENTATION_QOS)
		{
			if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			{
				if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				{
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == smallest_handle))
						{
							Invalid_Flag = true;

							if(max_sample > data_values->getSize())
							{
								node->value->info->sample_state	= READ_SAMPLE_STATE;
								node->value->info->view_state		= NOT_NEW_VIEW_STATE;

								data_values->insertInRear(node->value->value);
								sample_infos->insertInRear(node->value->info);

								data_sample_list.cutNode(node); // 읽은 노드 삭제
								delete(node->value);
								delete(node);
							}
							else
							{
								break;
							}
						}
					}
				}
				else // Source_timestamp를 고려하여 List에 저장
				{
					Time_t 	cTime;
					int			count;
					int			j;

					// 상식적으로 시스템 시작하고 0초에 데이터를 쏠순 없으니 0,0으로 초기화 하였음
					cTime.sec			= 0;
					cTime.nanosec		= 0;
					j						= 0;

					while(max_sample > j++)
					{
						while(data_sample_list.getSize() > i++)
						{
							node = node->rear;

							if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
									(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == smallest_handle))
							{
								Invalid_Flag = true;

								if((cTime.sec == 0) && (cTime.nanosec == 0))
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count					= i;
								}
								else if(cTime.sec > node->value->info->source_timestamp.sec)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count					= i;
								}
								else if(cTime.sec == node->value->info->source_timestamp.sec)
								{
									if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
									{
										cTime.sec			= node->value->info->source_timestamp.sec;
										cTime.nanosec		= node->value->info->source_timestamp.nanosec;
										count					= i;
									}
								}
							}
						}

						node = data_sample_list.getNodeByIndex(count);

						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);

						data_sample_list.cutNode(node);
						delete(node->value);
						delete(node);

						node = data_sample_list.getNodeByIndex(0);

						i						= 0;
						cTime.sec			= 0;
						cTime.nanosec		= 0;
					}
				}
			}
			else // false
			 {
				while(data_sample_list.getSize() > i++)
				{
					node = node->rear;

					if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
							(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == smallest_handle))
					{
						Invalid_Flag = true;

						if(max_sample > data_values->getSize())
						{
							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_values->insertInRear(node->value->value);
							sample_infos->insertInRear(node->value->info);

							data_sample_list.cutNode(node);
							delete(node->value);
							delete(node);
						}
						else
						{
							break;
						}
					}
				}
			 }
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == GROUP_PRESENTATION_QOS)
		{
			// presentation qos가 group이며 ordered_access가 true인 경우에는 최대 1개의 샘플을 반환한다.
			if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			{
				if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				{
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								(node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == smallest_handle))
						{
							Invalid_Flag = true;

							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_values->insertInRear(node->value->value);
							sample_infos->insertInRear(node->value->info);

							data_sample_list.cutNode(node);
							delete(node->value);
							delete(node);

							break;
						 }
					 }
				 }
				 else // destination_order.kind == SOURCE_TIMESTAMP
				 {
					 Time_t 	cTime;
					 int			count;

					 // 상식적으로 시스템 시작하고 0초에 데이터를 쏠 순 없을테니 0,0으로 초기화 하였음
					 cTime.sec			= 0;
					 cTime.nanosec	= 0;

					 // 가장 작은 값을 찾음
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

						 if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
								 (node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == smallest_handle))
						 {
							 Invalid_Flag = true;

							 if((cTime.sec == 0) && (cTime.nanosec == 0))
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec > node->value->info->source_timestamp.sec)
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec == node->value->info->source_timestamp.sec)
							 {
								 if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
								 {
									 cTime.sec			= node->value->info->source_timestamp.sec;
									 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
									 count				= i;
								 }
							 }
						 }
					 }

					 node = data_sample_list.getNodeByIndex(count);

					 node->value->info->sample_state	= READ_SAMPLE_STATE;
					 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					 data_values->insertInRear(node->value->value);
					 sample_infos->insertInRear(node->value->info);

					 data_sample_list.cutNode(node);
					 delete(node->value);
					 delete(node);
				 }
			 }
			 else // Ordered_access = false
			 {
				 while(data_sample_list.getSize() > i++)
				 {
					 node = node->rear;

					 if( (node->value->info->sample_state & sample_states) && (node->value->info->view_state & view_states) &&
							 (node->value->info->instance_state & instance_states) && (node->value->info->instance_handle == smallest_handle))
					 {
						 Invalid_Flag = true;

						 if(max_sample > data_values->getSize())
						 {
							 node->value->info->sample_state	= READ_SAMPLE_STATE;
							 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							 data_values->insertInRear(node->value->value);
							 sample_infos->insertInRear(node->value->info);

							 data_sample_list.cutNode(node);
							 delete(node->value);
							 delete(node);
						 }
						 else
						 {
							 break;
						 }
					 }
				 }
			 }
		}

		read_flag	= false;

		if(Invalid_Flag == false)
		{
			result		= RETCODE_BAD_PARAMETER;
		}

		if(data_values->getSize() > 0)
		{
			result		= RETCODE_OK;
		}
		else
		{
			result		 = RETCODE_NO_DATA;
		}

		// rtps 구현 필요
		// 여기서 sample_info의 다양한?? rank 값들 설정해줘야 함

		return	result;
	}

	ReturnCode_t		DataReader::take_next_instance_w_condition(DataList* data_values, SampleInfoList* sample_infos, long max_sample, InstanceHandle_t previous_handle, ReadCondition* a_condition)
	{
		ReturnCode_t	result	= RETCODE_OK;

		bool	Invalid_Flag = false;

		if(!is_enabled)
		{
			result		=	RETCODE_NOT_ENABLED;
			return	result;
		}

		if(max_sample == 0)		// Zero-copy인 경우 return_loan을 호출해야 함 , 좀 더 생각 필요
		{
			result		= return_loan(data_values, sample_infos);
			return 	result;
		}

		read_flag = true;

		int							i		= 0;
		Node<DataSample>* node	= data_sample_list.getNodeByIndex(0);

		InstanceHandle_t	smallest_handle = 0;


		// previous_handle보다는 크고 가장 작은 Handle을 찾음
		while(data_sample_list.getSize() > i++)
		{
			node	= node->rear;

			if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
					(node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
			{
				if(smallest_handle == 0)
				{
					smallest_handle = node->value->info->instance_handle;
				}
				else if(smallest_handle > node->value->info->instance_handle)
				{
					smallest_handle = node->value->info->instance_handle;
				}
			}
		}

		node	= data_sample_list.getNodeByIndex(0);
		i		= 0;


		if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == INSTANCE_PRESENTATION_QOS)
		{
			while(data_sample_list.getSize() > i++)
			{
				node = node->rear;

				if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
						(node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
				{
					Invalid_Flag = true;

					if(max_sample > data_values->getSize())
					{
						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);

						data_sample_list.cutNode(node); // 읽은 노드 삭제
						delete(node->value);
						delete(node);
					}
					else
					{
						break;
					}
				}
			}
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == TOPIC_PRESENTATION_QOS)
		{
			if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			{
				if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				{
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
								(node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
						{
							Invalid_Flag = true;

							if(max_sample > data_values->getSize())
							{
								node->value->info->sample_state	= READ_SAMPLE_STATE;
								node->value->info->view_state		= NOT_NEW_VIEW_STATE;

								data_values->insertInRear(node->value->value);
								sample_infos->insertInRear(node->value->info);

								data_sample_list.cutNode(node); // 읽은 노드 삭제
								delete(node->value);
								delete(node);
							}
							else
							{
								break;
							}
						}
					}
				}
				else // Source_timestamp를 고려하여 List에 저장
				{
					Time_t 	cTime;
					int			count;
					int			j;

					// 상식적으로 시스템 시작하고 0초에 데이터를 쏠순 없으니 0,0으로 초기화 하였음
					cTime.sec			= 0;
					cTime.nanosec		= 0;
					j						= 0;

					while(max_sample > j++)
					{
						while(data_sample_list.getSize() > i++)
						{
							node = node->rear;

							if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
									(node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
							{
								Invalid_Flag = true;

								if((cTime.sec == 0) && (cTime.nanosec == 0))
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count					= i;
								}
								else if(cTime.sec > node->value->info->source_timestamp.sec)
								{
									cTime.sec			= node->value->info->source_timestamp.sec;
									cTime.nanosec		= node->value->info->source_timestamp.nanosec;
									count					= i;
								}
								else if(cTime.sec == node->value->info->source_timestamp.sec)
								{
									if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
									{
										cTime.sec			= node->value->info->source_timestamp.sec;
										cTime.nanosec		= node->value->info->source_timestamp.nanosec;
										count					= i;
									}
								}
							}
						}

						node = data_sample_list.getNodeByIndex(count);

						node->value->info->sample_state	= READ_SAMPLE_STATE;
						node->value->info->view_state		= NOT_NEW_VIEW_STATE;

						data_values->insertInRear(node->value->value);
						sample_infos->insertInRear(node->value->info);

						data_sample_list.cutNode(node);
						delete(node->value);
						delete(node);

						node = data_sample_list.getNodeByIndex(0);

						i						= 0;
						cTime.sec			= 0;
						cTime.nanosec		= 0;
					}
				}
			}
			else // false
			 {
				while(data_sample_list.getSize() > i++)
				{
					node = node->rear;

					if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
							(node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
					{
						Invalid_Flag = true;

						if(max_sample > data_values->getSize())
						{
							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_values->insertInRear(node->value->value);
							sample_infos->insertInRear(node->value->info);

							data_sample_list.cutNode(node);
							delete(node->value);
							delete(node);
						}
						else
						{
							break;
						}
					}
				}
			 }
		}
		else if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.access_scope == GROUP_PRESENTATION_QOS)
		{
			// presentation qos가 group이며 ordered_access가 true인 경우에는 최대 1개의 샘플을 반환한다.
			if(((SubscriberQos*)this->get_subscriber()->qos)->presentation.ordered_access == true)
			{
				if(((DataReaderQos*)qos)->destination_order.kind == BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS)
				{
					while(data_sample_list.getSize() > i++)
					{
						node = node->rear;

						if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
								(node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
						{
							Invalid_Flag = true;

							node->value->info->sample_state	= READ_SAMPLE_STATE;
							node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							data_values->insertInRear(node->value->value);
							sample_infos->insertInRear(node->value->info);

							data_sample_list.cutNode(node);
							delete(node->value);
							delete(node);

							break;
						 }
					 }
				 }
				 else // destination_order.kind == SOURCE_TIMESTAMP
				 {
					 Time_t 	cTime;
					 int			count;

					 // 상식적으로 시스템 시작하고 0초에 데이터를 쏠 순 없을테니 0,0으로 초기화 하였음
					 cTime.sec			= 0;
					 cTime.nanosec	= 0;

					 // 가장 작은 값을 찾음
					 while(data_sample_list.getSize() > i++)
					 {
						 node = node->rear;

						 if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
								 (node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
						 {
							 Invalid_Flag = true;

							 if((cTime.sec == 0) && (cTime.nanosec == 0))
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec > node->value->info->source_timestamp.sec)
							 {
								 cTime.sec			= node->value->info->source_timestamp.sec;
								 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
								 count				= i;
							 }
							 else if(cTime.sec == node->value->info->source_timestamp.sec)
							 {
								 if(cTime.nanosec > node->value->info->source_timestamp.nanosec)
								 {
									 cTime.sec			= node->value->info->source_timestamp.sec;
									 cTime.nanosec	= node->value->info->source_timestamp.nanosec;
									 count				= i;
								 }
							 }
						 }
					 }

					 node = data_sample_list.getNodeByIndex(count);

					 node->value->info->sample_state	= READ_SAMPLE_STATE;
					 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

					 data_values->insertInRear(node->value->value);
					 sample_infos->insertInRear(node->value->info);

					 data_sample_list.cutNode(node);
					 delete(node->value);
					 delete(node);
				 }
			 }
			 else // Ordered_access = false
			 {
				 while(data_sample_list.getSize() > i++)
				 {
					 node = node->rear;

					 if( (node->value->info->sample_state & a_condition->sample_state_mask) && (node->value->info->view_state & a_condition->view_state_mask) &&
							 (node->value->info->instance_state & a_condition->instance_state_mask) && (node->value->info->instance_handle > previous_handle))
					 {
						 Invalid_Flag = true;

						 if(max_sample > data_values->getSize())
						 {
							 node->value->info->sample_state	= READ_SAMPLE_STATE;
							 node->value->info->view_state		= NOT_NEW_VIEW_STATE;

							 data_values->insertInRear(node->value->value);
							 sample_infos->insertInRear(node->value->info);

							 data_sample_list.cutNode(node);
							 delete(node->value);
							 delete(node);
						 }
						 else
						 {
							 break;
						 }
					 }
				 }
			 }
		}

		read_flag	= false;

		if(Invalid_Flag == false)
		{
			result		= RETCODE_BAD_PARAMETER;
		}

		if(data_values->getSize() > 0)
		{
			result		= RETCODE_OK;
		}
		else
		{
			result		= RETCODE_NO_DATA;
		}

		// rtps 구현 필요
		// 여기서 sample_info의 다양한?? rank 값들 설정해줘야 함

		return	result;
	}

	ReturnCode_t		DataReader::return_loan(DataList* data_values, SampleInfoList* sample_infos)
	{
		ReturnCode_t	result	= RETCODE_OK;

		// read or take에 의해 반환되는 pair가 일치해야 한다. ( pair -> data_values, sample_infos)
		// 같은 DataReader로 부터 반환 되어야 한다.
		// 위 2가지 조건이 아닌 경우 PRECONDITION_NOT_MET

		// return_loan 함수의 역할
		// CNU_DDS의 경우 배열을 이용하지 않으므로 크기에 대한 문제는 없음.. Zero-copy가 없다는 소리임?

		// Buffer loan from the DataReader to Application
		// provide Zero-Copy access to the data
		// Loan 중에는 Data 및 Sample 정보는 수정되지 않음

		return	result;
	}

	// InstanceHandle_t와 관련된 Instance Key를 검색
	// 구현 미완료 - key_holder에 Key를 넣어줘야 하는거 같은데 key가 없음
	ReturnCode_t		DataReader::get_key_value(DDS_Data* key_holder, InstanceHandle_t handle)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}

		int							i		= 0;
		Node<DataSample>*	node	= data_sample_list.getNodeByIndex(0);

		while(data_sample_list.getSize() > i++)
		{
			node	= node->rear;
			if( node->value->info->instance_handle == handle)
			{

				// key_holder에 값을 넣어줘야 함
				result			= RETCODE_OK;
				return		result;
			}
		}

		result		= RETCODE_ERROR; // SPEC상 Error 코드에 대한 언급은 따로 있지 않음
		return	result;
	}


	// 넘어오는 DDS_Data와 일치하는 DataSample의 InstanceHandle_t를 반환
	InstanceHandle_t	DataReader::lookup_instance(DDS_Data* instance)
	{
		InstanceHandle_t	result	= 0;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}

		int							i 		= 0;
		Node<DataSample>*	node	= data_sample_list.getNodeByIndex(0);

		while(data_sample_list.getSize() > i++)
		{
			node	= node->rear;
			if(node->value->value == instance) // 삭제할 노드를 찾은 경우 노드 삭제 후 while문 break
			{
				result		= node->value->info->instance_handle;
				return 	result;
			}
		}

		result		= NULL;
		return	result;
	}

	ReadCondition*		DataReader::create_readcondition(SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states)
	{
		ReadCondition*	result	= NULL;

		if(!is_enabled)
		{
			result		= NULL;
			return	result;
		}

		result		= new ReadCondition();

		result->sample_state_mask	= sample_states;
		result->view_state_mask		= view_states;
		result->instance_state_mask	= instance_states;
		result->related_waitset_list	= NULL;

		return	result;
	}

	// 구현 미완료
	QueryCondition*		DataReader::create_querycondition(SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states, std::string query_expression, StringSeq* query_parameters)
	{
		QueryCondition*	result	= NULL;

		if(!is_enabled)
		{
			result		= NULL;
			return	result;
		}

		result		= new QueryCondition();

		result->sample_state_mask 	= sample_states;
		result->view_state_mask 		= view_states;
		result->instance_state_mask = instance_states;
		result->related_waitset_list	= NULL;

		// Parameter와 Expression이 2개 넘어오는데 QueryCondition Attribute중 Expression 관련이 없음
		// result->set_query_parameters(query_parameters);
		// What Expression ?
		return	result;
	}

	ReturnCode_t		DataReader::delete_readcondition(ReadCondition* a_condition)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}

		// 삭제할 ReadCondition이 해당 DataReader에 속하는 것인지 확인
		// ReadCondition의 get_datareader가 미 구현 상태라 에러가 나므로 우선 주석 처리
		/*
		if(a_condition->get_datareader() != this)
		{
			result		= RETCODE_PRECONDITION_NOT_MET;
			return	result;
		}
		*/

		int								 i	= 0;
		Node<ReadCondition>*	node	= read_condition_list.getNodeByIndex(0);

		while(read_condition_list.getSize() > i++)
		{
			node	= node->rear;
			if(node->value == a_condition) // 삭제할 노드를 찾은 경우 노드 삭제 후 while문 break
			{
				read_condition_list.cutNode(node);
				delete(node->value);
				delete(node);
				result		= RETCODE_OK;
				break;
			}
		}

		return	result;
	}

	ReturnCode_t		DataReader::get_liveliness_changed_status(LivelinessChangedStatus status)
	{
		ReturnCode_t	result	= RETCODE_OK;


		return	result;
	}

	ReturnCode_t		DataReader::get_requested_deadline_missed_status(RequestedDeadlineMissedStatus status)
	{
		ReturnCode_t	result	= RETCODE_OK;


		return	result;
	}

	ReturnCode_t		DataReader::get_requested_incompatible_qos_status(RequestedIncompatibleQosStatus status)
	{
		ReturnCode_t	result	= RETCODE_OK;

		return	result;
	}

	ReturnCode_t		DataReader::get_sample_lost_status(SampleLostStatus status)
	{
		ReturnCode_t	result	= RETCODE_OK;

		return	result;
	}

	ReturnCode_t		DataReader::get_sample_rejected_status(SampleRejectedStatus status)
	{
		ReturnCode_t	result	= RETCODE_OK;


		return	result;
	}

	ReturnCode_t		DataReader::get_subscription_matched_status(SubscriptionMatchedStatus status)
	{
		ReturnCode_t	result	= RETCODE_OK;

		return	result;
	}

	TopicDescription*	DataReader::get_topic_description(void)
	{
		TopicDescription*	result	= NULL;

		result		=	this->related_topic_description;
		return	result;
	}

	Subscriber*			DataReader::get_subscriber(void)
	{
		Subscriber*	result	= NULL;

		result		= this->related_subscriber;
		return	result;
	}

	ReturnCode_t		DataReader::delete_contained_entities(void)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}

		int										i	= 0;
		Node<ReadCondition>	*node	= read_condition_list.getNodeByIndex(0);

		while(read_condition_list.getSize() > i++)
		{
			node = node->rear;

			if(delete_readcondition(node->value) != RETCODE_OK)
			{
				result 	= RETCODE_PRECONDITION_NOT_MET;
				return	result;
			}
		}

		result		= RETCODE_OK;
		return	result;
	}

	ReturnCode_t		DataReader::wait_for_historical_data(Duration_t max_wait)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(!is_enabled)
		{
			result		= RETCODE_NOT_ENABLED;
			return	result;
		}

		// non-VOLATILE PERSISTENCE Qos Kind

		while(true)
		{
			if(((DataReaderQos*)qos)->durability.kind == VOLATILE_DURABILITY_QOS)
			{
				result		= RETCODE_PRECONDITION_NOT_MET; // 수정 예상 -- VOLATILE일 경우 에러 처리 언급 없음
				return	result;
			}

			// 2가지 루틴이 존재 - 호출되면 우선 block됨.
			// 'historical' 데이터를 모두 받을때 까지 대기
			// Historical 데이터?가 뭘 의미하는건지..


			// 또는 시간이 지나면 타임 아웃으로 함수 종료
			if(max_wait.sec <= 0 && max_wait.nanosec <=0)
			{
				result		= RETCODE_TIMEOUT;
			}

		}

		return	result;
	}

	// 구현 미완료
	// handle로 일치하는 값을 찾은 이후 publication_data에 어떤 값을 넣어야 하는건지..
	ReturnCode_t		DataReader::get_matched_publication_data(PublicationBuiltinToipcData* publication_data, InstanceHandle_t publication_handle)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(!is_enabled)
		{
			result		= RETCODE_NOT_ENABLED;
			return	result;
		}

		int							i		= 0;
		Node<DataSample>* node = data_sample_list.getNodeByIndex(0);

		while(data_sample_list.getSize() > i++)
		{
			node = node->rear;

			if(node->value->info->publication_handle == publication_handle)
			{
				// 어떤 과정을 추가해야 하는건지 모르겠음..
				result		= RETCODE_OK;
				return	 result;
			}
		}

		result		= RETCODE_BAD_PARAMETER;	// Handle을 찾지 못하면 (매칭되는 DataReader가 없는 경우) BAD_PARAMETER
		return	result;
	}

	ReturnCode_t		DataReader::get_matched_publications(InstanceHandleSeq* publication_handles)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(!is_enabled)
		{
			result		= RETCODE_NOT_ENABLED;
			return	result;
		}

		int							i		= 0;
		Node<DataSample>* node = data_sample_list.getNodeByIndex(0);

		while(data_sample_list.getSize() > i++)
		{
			node = node->rear;
			publication_handles->insertInRear(&(node->value->info->publication_handle));
		}

		result		= RETCODE_OK;
		return	result;
	}
}
