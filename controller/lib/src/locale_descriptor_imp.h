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
 * locale_descriptor_imp.h
 *
 * Locale descriptor implementation class
 */

#pragma once
#ifndef _AVDECC_CONTROLLER_LIB_LOCALE_DESCRIPTOR_IMP_H_
#define _AVDECC_CONTROLLER_LIB_LOCALE_DESCRIPTOR_IMP_H_

#include "descriptor_base_imp.h"
#include "locale_descriptor.h"

namespace avdecc_lib
{
	class locale_descriptor_imp : public virtual locale_descriptor, public virtual descriptor_base_imp
	{
	private:
		struct jdksavdecc_descriptor_locale locale_desc; // Structure containing the locale_desc fields
		int desc_locale_read_returned; // Status of extracting Locale descriptor information from a network buffer

	public:
		/**
		 * An empty constructor for locale_descriptor_imp
		 */
		locale_descriptor_imp();

		/**
		 * Constructor for locale_descriptor_imp used for constructing an object with a base End Station, frame, position offest,
		 * and memory buffer length.
		 */
		locale_descriptor_imp(end_station_imp *base_end_station_imp_ref, uint8_t *frame, size_t pos, size_t mem_buf_len);

		/**
		 * Destructor for locale_descriptor_imp used for destroying objects
		 */
		virtual ~locale_descriptor_imp();

		/**
		 * Get the descriptor type of the Locale descriptor object.
		 */
		uint16_t STDCALL get_descriptor_type();

		/**
		 * Get the descriptor index of the Locale descriptor object.
		 */
		uint16_t STDCALL get_descriptor_index();

		/**
		 * Get the locale identifier of the Locale descriptor object.
		 */
		uint8_t * STDCALL get_locale_identifier();

		/**
		 * Get the number of strings of the Locale descriptor object.
		 */
		uint16_t STDCALL get_number_of_strings();

		/**
		 * Get the base strings of the Locale descriptor object.
		 */
		uint16_t STDCALL get_base_strings();
	};
}

#endif
