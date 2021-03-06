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
 * controller.h
 *
 * Public Controller interface class
 */

#pragma once
#ifndef _AVDECC_CONTROLLER_LIB_CONTROLLER_H_
#define _AVDECC_CONTROLLER_LIB_CONTROLLER_H_

#include <stdint.h>
#include "build.h"

namespace avdecc_lib
{
	class end_station;
	class configuration_descriptor;

	class controller
	{
	public:
		/**
		 * Deallocate memory
		 */
		AVDECC_CONTROLLER_LIB32_API virtual void STDCALL destroy() = 0;

		/**
		 * \return The current build release version.
		 */
		AVDECC_CONTROLLER_LIB32_API virtual const char * STDCALL get_version() = 0;

		/**
		 * \return The AVDECC Controller GUID of the AVDECC Entity sending the command.
		 */
		AVDECC_CONTROLLER_LIB32_API virtual uint64_t STDCALL get_controller_guid() = 0;

		/**
		 * \return The number of End Stations connected.
		 */
		AVDECC_CONTROLLER_LIB32_API virtual uint32_t STDCALL get_end_station_count() = 0;

		/**
		 * \return The corresponding End Station by index.
		 */
		AVDECC_CONTROLLER_LIB32_API virtual end_station * STDCALL get_end_station_by_index(uint32_t end_station_index) = 0;

		/**
		 * \return The corresponding Configuration descriptor by index.
		 */
		AVDECC_CONTROLLER_LIB32_API virtual configuration_descriptor * STDCALL get_config_by_index(uint32_t end_station_index, uint16_t entity_index, uint16_t config_index) = 0;

		/**
		 * \return The corresponding Configuration descriptor by GUID.
		 */
		AVDECC_CONTROLLER_LIB32_API virtual configuration_descriptor * STDCALL get_config_by_guid(uint64_t end_station_guid, uint16_t entity_index, uint16_t config_index) = 0;

		/**
		 * Check if the command with the corresponding notification id is in the inflight list.
		 */
		AVDECC_CONTROLLER_LIB32_API virtual bool STDCALL is_inflight_cmd_with_notification_id(void *notification_id) = 0;

		/**
		 * Update the log level.
		 */
		AVDECC_CONTROLLER_LIB32_API virtual void STDCALL update_log_level(int32_t new_log_level) = 0;

		/**
		 * \return The missed notification events count.
		 */
		AVDECC_CONTROLLER_LIB32_API virtual uint32_t STDCALL missed_notification_count() = 0;

		/**
		 * \return The missed log events count.
		 */
		AVDECC_CONTROLLER_LIB32_API virtual uint32_t STDCALL missed_log_count() = 0;

		/**
		 * Check for End Station connection, command packet, and response packet timeouts.
		 */
		AVDECC_CONTROLLER_LIB32_API virtual void STDCALL time_tick_event() = 0;

		/**
		 * Lookup and process packet received.
		 */
		AVDECC_CONTROLLER_LIB32_API virtual void STDCALL rx_packet_event(void *&notification_id, bool &notification_id_flag, uint32_t &notification_flag, uint8_t *frame, uint16_t mem_buf_len, int &status) = 0;

		/**
		 * Send queued packet to the AEM Controller State Machine.
		 */
		AVDECC_CONTROLLER_LIB32_API virtual void STDCALL tx_packet_event(void *notification_id, uint32_t notification_flag, uint8_t *frame, uint16_t mem_buf_len) = 0;
	};

	/**
	 * Create a public AVDECC Controller object with a network interface object, notification and logging callback functions used for accessing from outside the library.
	 *
	 * \param netif A network interface object created in the application level using the public network interface API provided.
	 * \param notification_user_obj A void pointer used to store any helpful C++ class object.
	 * \param notification_type The type of notification that the callback function is called with. (Refer to notifications enumeration included in the library for a list of notification types supported.)
	 * \param guid An unique identifier of the End Station the command is targeted to.
	 * \param cmd_type The type of the AEM command used in the command data to be sent.
	 * \param desc_type The type of the AEM descriptor used in the command data to be sent.
	 * \param desc_index The index associated with the corresponding descriptor type used in the command data to be sent.
	 * \param notification_id A void pointer to the unique identifier for each notification message.
	 *
	 * \param log_user_obj A void pointer used to store any helpful class object.
	 * \param log_level The log level that the callback function is called with. (Refer to logging levels enumeration included in the library for a list of log levels supported.)
	 * \param log_msg A message containing additional information to be logged.
	 * \param time_stamp_ms The time in milliseconds indicating when the message is logged.
	 */
	extern "C" AVDECC_CONTROLLER_LIB32_API controller * STDCALL create_controller(net_interface *netif,
	                                                                              void (*notification_callback) (void *notification_user_obj, int32_t notification_type, uint64_t guid, uint16_t cmd_type,
	                                                                                                             uint16_t desc_type, uint16_t desc_index, void *notification_id),
	                                                                              void (*log_callback) (void *log_user_obj, int32_t log_level, const char *log_msg, int32_t time_stamp_ms));
}

#endif
