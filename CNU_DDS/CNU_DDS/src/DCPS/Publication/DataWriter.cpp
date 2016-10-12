#include "../../../include/DCPS/Publication/DataWriter.h"
#include "../../../include/DCPS/Publication/Publisher.h"
#include "../../../include/DCPS/Topic/Topic.h"
#include "../../../include/Status/EntityStatus/DataWriterStatus.h"
#include "../../../include/Status/Condition/StatusCondition.h"

#include "../../../include/RTPS/Behavior/StatefulWriter.h"

namespace CNU_DDS
{
	DataWriter::DataWriter()
	{
		status				= new DataWriterStatus();
		status_condition	= new StatusCondition();

		status_condition->enabled_statuses	= 0x00;
		status_condition->related_entity	= this;

		is_enabled			= false;
		listener			= NULL;
		qos					= NULL;
	}

	DataWriter::~DataWriter()
	{
		delete(status_condition);
		delete(status);
	}

	ReturnCode_t		DataWriter::get_qos(DataWriterQos* qos_list)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(qos)
		{
			memcpy(qos_list, qos, sizeof(DataWriterQos));
			result		= RETCODE_OK;
		}
		else
		{
			result		= RETCODE_PRECONDITION_NOT_MET;	// undefined
		}

		return	result;
	}

	ReturnCode_t		DataWriter::set_qos(DataWriterQos* qos_list)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(!is_enabled && qos == NULL)
		{
			qos	= new DataWriterQos();
			//printf("sizeof(DataWriterQos) == %d\n", sizeof(DataWriterQos));
			//printf("sizeof(qos_list) == %d\n", sizeof(*qos_list));
			memcpy(qos, qos_list, sizeof(DataWriterQos));
			result	= RETCODE_OK;
			return result;
		}

		if(qos)	// possible error : RETCODE_IMMUTABLE_POLICY
		{

		}
		else	// possible error : RETCODE_INCONSISTENT_POLICY
		{
			qos		= qos_list;
			result	= RETCODE_OK;
		}

		return result;
	}

	Listener*			DataWriter::get_listener(void)
	{
		Listener*	result	= NULL;

		result	= listener;

		return result;
	}

	ReturnCode_t		DataWriter::set_listener(Listener* a_listener, StatusMask mask)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(listener != NULL && a_listener == NULL)
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

		return result;
	}

	ReturnCode_t		DataWriter::enable(void)
	{
		ReturnCode_t	result	= RETCODE_OK;

		is_enabled	= true;

		return	result;
	}

	InstanceHandle_t	DataWriter::register_instance(DDS_Data* instance)
	{
		InstanceHandle_t	result	= 0;

		related_data	= instance;

		result			= (InstanceHandle_t)related_data;

		return	result;
	}

	InstanceHandle_t	DataWriter::register_instance_w_timestamp(DDS_Data* instance, Time_t timestamp)
	{
		InstanceHandle_t	result	= 0;

		related_data	= instance;

		result			= (InstanceHandle_t)related_data;

		return	result;
	}

	ReturnCode_t		DataWriter::unregister_instance(DDS_Data* instance, InstanceHandle_t handle)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(is_enabled == false)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{

		}

		return	result;
	}

	ReturnCode_t		DataWriter::unregister_instance_w_timestamp(DDS_Data* instance, InstanceHandle_t handle, Time_t timestamp)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(is_enabled == false)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{

		}

		return	result;
	}

	ReturnCode_t		DataWriter::get_key_value(DDS_Data* key_holder, InstanceHandle_t handle)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(is_enabled == false)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			if(false /* handle does not correspond to an existing data_object */)
			{
				result	= RETCODE_BAD_PARAMETER;
			}
			else
			{
				key_holder	= related_data;

				result		= RETCODE_OK;
			}
		}

		return	result;
	}

	InstanceHandle_t	DataWriter::lookup_instance(DDS_Data* instance)
	{
		InstanceHandle_t	result	= 0;

		result	= (InstanceHandle_t)related_data;

		return	result;
	}

	ReturnCode_t		DataWriter::write(DDS_Data* data, InstanceHandle_t handle)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(is_enabled == false)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* automatically supply the value of the "source_timestamp" */

			/* asserts liveliness on the DataWriter itself */

			if(false /* handle != NULL && does not correspond to the same instance referred by data */)
			{
				result	= RETCODE_PRECONDITION_NOT_MET;
			}
			else if(false /* handle != NULL && handle does not correspond to an existing instance */)
			{
				result	= RETCODE_BAD_PARAMETER;
			}
			else if(false /* writer's BLOCK state relates with RESOURCE_LIMIT */)
			{
				result	= RETCODE_TIMEOUT;
			}
			else if(false /* writer's BLOCK state relates with RESOURCE_LIMIT */)
			{
				result	= RETCODE_OUT_OF_RESOURCES;
			}
			else
			{
				((StatefulWriter*)this->related_rtps_writer)->new_change(CHANGE_KIND_ALIVE, data->Serialize() ,handle);
				result	= RETCODE_OK;
			}
		}

		return	result;
	}

	ReturnCode_t		DataWriter::write_w_timestamp(DDS_Data* data, InstanceHandle_t handle, Time_t timestamp)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(is_enabled == false)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* automatically supply the value of the "source_timestamp" */

			/* asserts liveliness on the DataWriter itself */

			if(false /* handle != NULL && does not correspond to the same instance referred by data */)
			{
				result	= RETCODE_PRECONDITION_NOT_MET;
			}
			else if(false /* handle != NULL && handle does not correspond to an existing instance */)
			{
				result	= RETCODE_BAD_PARAMETER;
			}
			else if(false /* writer's BLOCK state relates with RESOURCE_LIMIT */)
			{
				result	= RETCODE_TIMEOUT;
			}
			else if(false /* writer's BLOCK state relates with RESOURCE_LIMIT */)
			{
				result	= RETCODE_OUT_OF_RESOURCES;
			}
			else
			{
				result	= RETCODE_OK;
			}
		}

		return	result;
	}

	ReturnCode_t		DataWriter::dispose(DDS_Data* data, InstanceHandle_t handle)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(is_enabled == false)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* automatically supply the value of the "source_timestamp" */

			if(false /* same with "write" operation */)
			{
				result	= RETCODE_TIMEOUT;
			}
			else if(false /* same with "write" operation */)
			{
				result	= RETCODE_OUT_OF_RESOURCES;
			}
			else
			{
				result	= RETCODE_OK;
			}
		}

		return	result;
	}

	ReturnCode_t		DataWriter::disopose_w_timestamp(DDS_Data* data, InstanceHandle_t handle, Time_t timestamp)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(is_enabled == false)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* automatically supply the value of the "source_timestamp" */

			if(false /* same with "write" operation */)
			{
				result	= RETCODE_PRECONDITION_NOT_MET;
			}
			else if(false /* same with "write" operation */)
			{
				result	= RETCODE_BAD_PARAMETER;
			}
			else if(false /* same with "write" operation */)
			{
				result	= RETCODE_TIMEOUT;
			}
			else if(false /* same with "write" operation */)
			{
				result	= RETCODE_OUT_OF_RESOURCES;
			}
			else
			{
				result	= RETCODE_OK;
			}
		}

		return	result;
	}

	ReturnCode_t		DataWriter::wait_for_acknowledgements(Duration_t max_wait)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(is_enabled == false)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* blocks the calling thread until either all data written by the DataWriter is acknowledged by all matched DataReader entities
			 * that have RELIABILITY QoS kind RELIABLE
			 */

			if(((DataWriterQos*)qos)->reliability.kind != RELIABLE_RELIABILITY_QOS)
			{
				result	= RETCODE_PRECONDITION_NOT_MET;
			}
			else if(false /* over the max_wait time */)
			{
				result = RETCODE_TIMEOUT;
			}
			else
			{
				result	= RETCODE_OK;
			}
		}

		return	result;
	}

	ReturnCode_t		DataWriter::assert_liveliness(void)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(is_enabled == false)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			if(((DataWriterQos*)qos)->liveliness.kind == MANUAL_BY_PARTICIPANT_LIVELINESS_QOS || ((DataWriterQos*)qos)->liveliness.kind == MANUAL_BY_TOPIC_LIVELINESS_QOS)
			{
				result	= RETCODE_OK;
			}
			else
			{
				result	= RETCODE_PRECONDITION_NOT_MET;
			}
		}

		return	result;
	}

	ReturnCode_t		DataWriter::get_liveliness_lost_status(LivelinessLostStatus status)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(is_enabled == false)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* access to the LIVELINESS_LOST communication status */

			result	= RETCODE_OK;
		}

		return	result;
	}

	ReturnCode_t		DataWriter::get_offered_deadline_missed_status(OfferedDeadlineMissedStatus status)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(is_enabled == false)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* access to the OFFERED_DEADLINE_MISSED communication status */

			result	= RETCODE_OK;
		}

		return	result;
	}

	ReturnCode_t		DataWriter::get_offered_incompatible_qos_status(OfferedIncompatibleQosStatus status)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(is_enabled == false)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* access to the OFFERED_INCOMPATIBLE_QOS communication status */

			result	= RETCODE_OK;
		}

		return	result;
	}

	ReturnCode_t		DataWriter::get_publication_matched_stataus(PublicationMatchedStatus status)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(is_enabled == false)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* access to the PUBLICATION_MATCHED communication status */

			result	= RETCODE_OK;
		}

		return	result;
	}

	pTopic				DataWriter::get_topic(void)
	{
		Topic*	result	= NULL;

		result	= related_topic;

		return	result;
	}

	pPublisher			DataWriter::get_publisher(void)
	{
		Publisher*	publisher	= NULL;

		publisher	= related_publisher;

		return	publisher;
	}

	ReturnCode_t		DataWriter::get_matched_subscription_data(SubscriptionBuiltinTopicData* subscription_data, InstanceHandle_t subscription_handle)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(is_enabled == false)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			if(false /* no existing subscription_handle */)
			{
				result	= RETCODE_BAD_PARAMETER;
			}
			else if(false /* unsupported */)
			{
				result	= RETCODE_UNSUPPORTED;
			}
			else
			{
				result	= RETCODE_OK;
			}
		}

		return	result;
	}

	ReturnCode_t		DataWriter::get_matched_subscriptions(InstanceHandleSeq* subscription_handles)
	{
		ReturnCode_t	result	= RETCODE_OK;

		if(is_enabled == false)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			result	= RETCODE_OK;
		}

		return	result;
	}
}
