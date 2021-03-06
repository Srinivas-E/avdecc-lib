/*
 * Licensed under the MIT License (MIT)
 *
 * Copyright (c) 2013 AudioScience Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * clock_domain_descriptor_imp.cpp
 *
 * Clock Domain descriptor implementation
 */

#include <vector>
#include "enumeration.h"
#include "log.h"
#include "adp.h"
#include "aecp.h"
#include "end_station_imp.h"
#include "system_tx_queue.h"
#include "aem_controller_state_machine.h"
#include "clock_domain_descriptor_imp.h"

namespace avdecc_lib
{
	clock_domain_descriptor_imp::clock_domain_descriptor_imp() {}

	clock_domain_descriptor_imp::clock_domain_descriptor_imp(end_station_imp *base_end_station_imp_ref, uint8_t *frame, size_t pos, size_t mem_buf_len) : descriptor_base_imp(base_end_station_imp_ref)
	{
		desc_clock_domain_read_returned = jdksavdecc_descriptor_clock_domain_read(&clock_domain_desc, frame, pos, mem_buf_len);

		if(desc_clock_domain_read_returned < 0)
		{
			avdecc_lib::log_ref->logging(avdecc_lib::LOGGING_LEVEL_ERROR, "desc_clock_domain_read error");
			assert(desc_clock_domain_read_returned >= 0);
		}

		store_clock_sources(frame, pos);
	}

	clock_domain_descriptor_imp::~clock_domain_descriptor_imp() {}

	uint16_t STDCALL clock_domain_descriptor_imp::get_descriptor_type()
	{
		assert(clock_domain_desc.descriptor_type == JDKSAVDECC_DESCRIPTOR_CLOCK_DOMAIN);
		return clock_domain_desc.descriptor_type;
	}

	uint16_t STDCALL clock_domain_descriptor_imp::get_descriptor_index()
	{
		return clock_domain_desc.descriptor_index;
	}

	uint8_t * STDCALL clock_domain_descriptor_imp::get_object_name()
	{
		return clock_domain_desc.object_name.value;
	}

	uint16_t STDCALL clock_domain_descriptor_imp::get_localized_description()
	{
		return clock_domain_desc.localized_description;
	}

	uint16_t STDCALL clock_domain_descriptor_imp::get_clock_source_index()
	{
		return clock_domain_desc.clock_source_index;
	}

	uint16_t STDCALL clock_domain_descriptor_imp::get_clock_sources_offset()
	{
		assert(clock_domain_desc.clock_sources_offset == 76);
		return clock_domain_desc.clock_sources_offset;
	}

	uint16_t STDCALL clock_domain_descriptor_imp::get_clock_sources_count()
	{
		assert(clock_domain_desc.clock_sources_count <= 249);
		return clock_domain_desc.clock_sources_count;
	}

	void clock_domain_descriptor_imp::store_clock_sources(uint8_t *frame, size_t pos)
	{
		uint16_t offset = 0x0;

		for(uint32_t index_i = 0; index_i < get_clock_sources_count(); index_i++)
		{
			clk_src_vec.push_back(jdksavdecc_uint16_get(frame, get_clock_sources_offset() + pos + offset));
			offset += 0x2;
		}
	}

	uint16_t STDCALL clock_domain_descriptor_imp::get_clock_source_by_index(uint32_t clk_src_index)
	{
		return clk_src_vec.at(clk_src_index);
	}

	uint16_t STDCALL clock_domain_descriptor_imp::set_clock_source_clock_source_index()
	{
		return aem_cmd_set_clk_src_resp.clock_source_index;
	}

	uint16_t STDCALL clock_domain_descriptor_imp::get_clock_source_clock_source_index()
	{
		return aem_cmd_get_clk_src_resp.clock_source_index;
	}

	int STDCALL clock_domain_descriptor_imp::send_set_clock_source_cmd(void *notification_id, uint16_t desc_index, uint16_t new_clk_src_index)
	{
		struct jdksavdecc_frame *ether_frame;
		struct jdksavdecc_aem_command_set_clock_source aem_cmd_set_clk_src;
		int aem_cmd_set_clk_src_returned;
		ether_frame = (struct jdksavdecc_frame *)malloc(sizeof(struct jdksavdecc_frame));

		/***************************************** AECP Common Data ********************************************/
		aem_cmd_set_clk_src.controller_entity_id = base_end_station_imp_ref->get_adp()->get_controller_guid();
		// Fill aem_cmd_set_clk_src.sequence_id in AEM Controller State Machine
		aem_cmd_set_clk_src.command_type = JDKSAVDECC_AEM_COMMAND_SET_CLOCK_SOURCE;

		/************************** AECP Message Specific Data ************************/
		aem_cmd_set_clk_src.descriptor_type = JDKSAVDECC_DESCRIPTOR_CLOCK_DOMAIN;
		aem_cmd_set_clk_src.descriptor_index = desc_index;
		aem_cmd_set_clk_src.clock_source_index = new_clk_src_index;

		/******************************** Fill frame payload with AECP data and send the frame ***************************/
		aecp::ether_frame_init(base_end_station_imp_ref, ether_frame);
		aem_cmd_set_clk_src_returned = jdksavdecc_aem_command_set_clock_source_write(&aem_cmd_set_clk_src,
		                                                                             ether_frame->payload,
		                                                                             aecp::CMD_POS,
		                                                                             sizeof(ether_frame->payload));

		if(aem_cmd_set_clk_src_returned < 0)
		{
			avdecc_lib::log_ref->logging(avdecc_lib::LOGGING_LEVEL_ERROR, "aem_cmd_get_clk_src_write error\n");
			assert(aem_cmd_set_clk_src_returned >= 0);
			return -1;
		}

		aecp::common_hdr_init(ether_frame, base_end_station_imp_ref->get_end_station_guid());
		system_queue_tx(notification_id, avdecc_lib::CMD_WITH_NOTIFICATION, ether_frame->payload, ether_frame->length);

		free(ether_frame);
		return 0;

	}

	int clock_domain_descriptor_imp::proc_set_clock_source_resp(void *&notification_id, uint32_t &notification_flag, uint8_t *frame, uint16_t mem_buf_len, int &status)
	{
		struct jdksavdecc_frame *ether_frame;
		int aem_cmd_set_clk_src_resp_returned;
		uint32_t msg_type;
		bool u_field;

		ether_frame = (struct jdksavdecc_frame *)malloc(sizeof(struct jdksavdecc_frame));
		memcpy(ether_frame->payload, frame, mem_buf_len);

		aem_cmd_set_clk_src_resp_returned = jdksavdecc_aem_command_get_clock_source_response_read(&aem_cmd_set_clk_src_resp,
		                                                                                          frame,
		                                                                                          aecp::CMD_POS,
		                                                                                          mem_buf_len);

		if(aem_cmd_set_clk_src_resp_returned < 0)
		{
			avdecc_lib::log_ref->logging(avdecc_lib::LOGGING_LEVEL_ERROR, "aem_cmd_get_clk_src_resp_read error\n");
			assert(aem_cmd_set_clk_src_resp_returned >= 0);
			return -1;
		}

		msg_type = aem_cmd_set_clk_src_resp.aem_header.aecpdu_header.header.message_type;
		status = aem_cmd_set_clk_src_resp.aem_header.aecpdu_header.header.status;
		u_field = aem_cmd_set_clk_src_resp.command_type >> 15 & 0x01; // u_field = the msb of the uint16_t command_type

		aem_controller_state_machine_ref->update_inflight_for_rcvd_resp(notification_id, notification_flag, msg_type, u_field, ether_frame);

		free(ether_frame);
		return 0;
	}

	int STDCALL clock_domain_descriptor_imp::send_get_clock_source_cmd(void *notification_id, uint16_t desc_index)
	{
		struct jdksavdecc_frame *ether_frame;
		struct jdksavdecc_aem_command_get_clock_source aem_cmd_get_clk_src;
		int aem_cmd_get_clk_src_returned;
		ether_frame = (struct jdksavdecc_frame *)malloc(sizeof(struct jdksavdecc_frame));

		/***************************************** AECP Common Data ********************************************/
		aem_cmd_get_clk_src.controller_entity_id = base_end_station_imp_ref->get_adp()->get_controller_guid();
		// Fill aem_cmd_get_clk_src.sequence_id in AEM Controller State Machine
		aem_cmd_get_clk_src.command_type = JDKSAVDECC_AEM_COMMAND_GET_CLOCK_SOURCE;

		/************************** AECP Message Specific Data ************************/
		aem_cmd_get_clk_src.descriptor_type = JDKSAVDECC_DESCRIPTOR_CLOCK_DOMAIN;
		aem_cmd_get_clk_src.descriptor_index = desc_index;

		/******************************** Fill frame payload with AECP data and send the frame ***************************/
		aecp::ether_frame_init(base_end_station_imp_ref, ether_frame);
		aem_cmd_get_clk_src_returned = jdksavdecc_aem_command_get_clock_source_write(&aem_cmd_get_clk_src,
		                                                                             ether_frame->payload,
		                                                                             aecp::CMD_POS,
		                                                                             sizeof(ether_frame->payload));

		if(aem_cmd_get_clk_src_returned < 0)
		{
			avdecc_lib::log_ref->logging(avdecc_lib::LOGGING_LEVEL_ERROR, "aem_cmd_get_clk_src_write error\n");
			assert(aem_cmd_get_clk_src_returned >= 0);
			return -1;
		}

		aecp::common_hdr_init(ether_frame, base_end_station_imp_ref->get_end_station_guid());
		system_queue_tx(notification_id, avdecc_lib::CMD_WITH_NOTIFICATION, ether_frame->payload, ether_frame->length);

		free(ether_frame);
		return 0;
	}

	int clock_domain_descriptor_imp::proc_get_clock_source_resp(void *&notification_id, uint32_t &notification_flag, uint8_t *frame, uint16_t mem_buf_len, int &status)
	{
		struct jdksavdecc_frame *ether_frame;
		int aem_cmd_get_clk_src_resp_returned;
		uint32_t msg_type;
		bool u_field;

		ether_frame = (struct jdksavdecc_frame *)malloc(sizeof(struct jdksavdecc_frame));
		memcpy(ether_frame->payload, frame, mem_buf_len);

		aem_cmd_get_clk_src_resp_returned = jdksavdecc_aem_command_get_clock_source_response_read(&aem_cmd_get_clk_src_resp,
		                                                                                          frame,
		                                                                                          aecp::CMD_POS,
		                                                                                          mem_buf_len);

		if(aem_cmd_get_clk_src_resp_returned < 0)
		{
			avdecc_lib::log_ref->logging(avdecc_lib::LOGGING_LEVEL_ERROR, "aem_cmd_get_clk_src_resp_read error\n");
			assert(aem_cmd_get_clk_src_resp_returned >= 0);
			return -1;
		}

		msg_type = aem_cmd_get_clk_src_resp.aem_header.aecpdu_header.header.message_type;
		status = aem_cmd_get_clk_src_resp.aem_header.aecpdu_header.header.status;
		u_field = aem_cmd_get_clk_src_resp.command_type >> 15 & 0x01; // u_field = the msb of the uint16_t command_type

		aem_controller_state_machine_ref->update_inflight_for_rcvd_resp(notification_id, notification_flag, msg_type, u_field, ether_frame);

		free(ether_frame);
		return 0;
	}
}
