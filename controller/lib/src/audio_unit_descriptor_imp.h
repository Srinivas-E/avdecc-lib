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
 * audio_unit_descriptor_imp.h
 *
 * Audio Unit descriptor implementation class
 */

#pragma once
#ifndef _AVDECC_CONTROLLER_LIB_AUDIO_UNIT_DESCRIPTOR_IMP_H_
#define _AVDECC_CONTROLLER_LIB_AUDIO_UNIT_DESCRIPTOR_IMP_H_

#include "descriptor_base_imp.h"
#include "audio_unit_descriptor.h"

namespace avdecc_lib
{
	class audio_unit_descriptor_imp : public virtual audio_unit_descriptor, public virtual descriptor_base_imp
	{
	private:
		struct jdksavdecc_descriptor_audio audio_unit_desc; // Structure containing the audio_unit_desc fields
		int desc_audio_read_returned; // Status of extracting AUDIO_UNIT descriptor information from a network buffer

		struct current_sample_rates
		{
			uint8_t pull_field;
			uint32_t base_freq;
			uint32_t freq;
		};

		std::vector<struct current_sample_rates> sample_rates_vector; // Store sample rates pull field, base frequency, and frequency information
		struct current_sample_rates sample_rates;
		struct jdksavdecc_aem_command_set_sampling_rate_response aem_cmd_set_sampling_rate_resp;
		struct jdksavdecc_aem_command_get_sampling_rate_response aem_cmd_get_sampling_rate_resp;

	public:
		/**
		 * An empty constructor for audio_unit_descriptor_imp
		 */
		audio_unit_descriptor_imp();

		/**
		 * Constructor for audio_unit_descriptor_imp used for constructing an object with a base End Station, frame, position offest,
		 * and memory buffer length.
		 */
		audio_unit_descriptor_imp(end_station_imp *base_end_station_imp_ref, uint8_t *frame, size_t pos, size_t mem_buf_len);

		/**
		 * Destructor for audio_unit_descriptor_imp used for destroying objects
		 */
		virtual ~audio_unit_descriptor_imp();

		/**
		 * Get the descriptor type of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_descriptor_type();

		/**
		 * Get the descriptor index of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_descriptor_index();

		/**
		 * Get the name of the Audio Unit descriptor object.
		 */
		uint8_t * STDCALL get_object_name();

		/**
		 * Get the localized description of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_localized_description();

		/**
		 * Get the clock domain index of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_clock_domain_index();

		/**
		 * Get the number of stream_input_ports of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_number_of_stream_input_ports();

		/**
		 * Get the base stream input port of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_base_stream_input_port();

		/**
		 * Get the number of stream_output_ports of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_number_of_stream_output_ports();

		/**
		 * Get the base stream output port of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_base_stream_output_port();

		/**
		 * Get the number of external input ports of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_number_of_external_input_ports();

		/**
		 * Get the base external input port of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_base_external_input_port();

		/**
		 * Get the number of external output_ports of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_number_of_external_output_ports();

		/**
		 * Get the base external output port of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_base_external_output_port();

		/**
		 * Get the number of internal input ports of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_number_of_internal_input_ports();

		/**
		 * Get the base internal input port of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_base_internal_input_port();

		/**
		 * Get the number of internal output ports of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_number_of_internal_output_ports();

		/**
		 * Get the base internal output port of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_base_internal_output_port();

		/**
		 * Get the number of controls of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_number_of_controls();

		/**
		 * Get the base control of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_base_control();

		/**
		 * Get the number of signal selectors of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_number_of_signal_selectors();

		/**
		 * Get the base signal selector of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_base_signal_selector();

		/**
		 * Get the number of mixers of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_number_of_mixers();

		/**
		 * Get the base mixer of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_base_mixer();

		/**
		 * Get the number of matrices of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_number_of_matrices();

		/**
		 * Get the base matrix of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_base_matrix();

		/**
		 * Get the number of splitters of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_number_of_splitters();

		/**
		 * Get the base splitter of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_base_splitter();

		/**
		 * Get the number of combiners of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_number_of_combiners();

		/**
		 * Get the base combiner of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_base_combiner();

		/**
		 * Get the number of demultiplexers of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_number_of_demultiplexers();

		/**
		 * Get the base demultiplexer of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_base_demultiplexer();

		/**
		 * Get the number of multiplexers of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_number_of_multiplexers();

		/**
		 * Get the base multiplexer of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_base_multiplexer();

		/**
		 * Get the number of transcoders of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_number_of_transcoders();

		/**
		 * Get the base transcoder of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_base_transcoder();

		/**
		 * Get the number of control blocks of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_number_of_control_blocks();

		/**
		 * Get the base control block of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_base_control_block();

		/**
		 * Get the current sampling rate of the Audio Unit descriptor object.
		 */
		uint32_t STDCALL get_current_sampling_rate();

		/**
		 * Get the sampling rates offset of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_sampling_rates_offset();

		/**
		 * Get the sampling rates count of the Audio Unit descriptor object.
		 */
		uint16_t STDCALL get_sampling_rates_count();

		/**
		 * Get the corresponding pull field multiplier for the pull field value.
		 */
		uint8_t STDCALL get_pull_field_multiplier(uint8_t pull_field_value);

		/**
		 * Get the sampling rate of a port or unit after sending a SET_SAMPLING_RATE command and
		 * receiving a response back for the command.
		 */
		uint32_t STDCALL set_sampling_rate_sampling_rates();

		/**
		 * Get the sampling rate of a port or unit after sending a GET_SAMPLING_RATE command and
		 * receiving a response back for the command.
		 */
		uint32_t STDCALL get_sampling_rate_sampling_rates();

		/**
		 * Send a SET_SAMPLING_RATE command to change the sampling rate of a port or unit.
		 */
		int STDCALL send_set_sampling_rate_cmd(void *notification_id, uint16_t desc_index, uint32_t new_sampling_rate);

		/**
		 * Process a SET_SAMPLING_RATE response for the SET_SAMPLING_RATE command.
		 */
		int proc_set_sampling_rate_resp(void *&notification_id, uint32_t &notification_flag, uint8_t *frame, uint16_t mem_buf_len, int &status);

		/**
		 * Send a GET_SAMPLING_RATE command to get the current sampling rate of a port or unit.
		 */
		int STDCALL send_get_sampling_rate_cmd(void *notification_id, uint16_t desc_index);

		/**
		 * Process a GET_SAMPLING_RATE response for the GET_SAMPLING_RATE command.
		 */
		int proc_get_sampling_rate_resp(void *&notification_id, uint32_t &notification_flag, uint8_t *frame, uint16_t mem_buf_len, int &status);
	};
}

#endif
