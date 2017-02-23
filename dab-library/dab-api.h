#
/*
 *    Copyright (C) 2013, 2014, 2015, 2016, 2017
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Programming
 *
 *    This file is the API description of the DAB-library.
 *    Many of the ideas as implemented in the DAB-library are derived from
 *    other work, made available through the GNU general Public License. 
 *    All copyrights of the original authors are acknowledged.
 *
 *    DAB-library is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    DAB-library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with DAB-library, if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef	__DAB_API
#define	 __DAB_API
#include	<stdio.h>
#include	<stdint.h>
#include	<string>
#include	<list>

//	Experimental API for controlling the dab software library
//
//	Version 0.5
//	An example of the use of the library - using this API -
//	is enclosed in the directory "example" in this distribution

extern "C" {
	enum dabBand {
	   BAND_III	= 0,
	   L_BAND	= 1
	};
//
//
//////////////////////// C A L L B A C K F U N C T I O N S ///////////////
//	The ensemble  - when discovered in the selected channel -
//	is presented as a list of strings,
//	the boolean tells whether or not an ensemble was found
//	If no ensemble was found, it is (almost) certain that there is
//	no decent signal.
//	The type of the callback function should be conformant to
	typedef void (*cb_ensemble_t)(std::list<std::string>, bool);

//	Note that this function is *required* to be provided for,

//	The resulting audio samples - if any - are returned as pairs of
//	16 bit integers, with the length (in items), and the baudrate
//	as other parameters.
//	The type of the callback function should be conformant to
	typedef void (*cb_audio_t)(int16_t *, int, int);

//	if a NULL is provided, no data will be transferred.
//
//	The dynamic labelvalue - if any - is passed through a
//	callback function, whose type is to be conformant to
	typedef void (*cb_data_t)(std::string);

//	if a NULL is provided, no data will be transferred.
//
//	The technical data of the selected program is
//	passed through a function, whose type is to be conformant to
	typedef void (*cb_programdata_t)(int16_t,	// start address
	                                 int16_t,	// length
	                                 int16_t,	// subchId
	                                 int16_t,	// protection
	                                 int16_t	// bitRate
	                                );
//
//
//	if a NULL is provided, no data will be transferred.
//
////////////////////// A P I - F U N C T I O N S ///////////////////////
//	The initialization function takes as parameters the 
//	immutable system parameters,
//	    the dabMode is just 1, 2 or 4
//	    the dabBand, see the type above
//	    the callback for the sound handling,
//	    the callback for the dynamic label
//
//	Note that by creating a dab-library, you already selected a device
//
//	The function returns a non-NULL handle when the device
//	could be opened for delivery input.
//	Otherwise it return NULL.
	void	*dab_initialize	(uint8_t,	// dab Mode
	                         dabBand,	// Band
	                         cb_audio_t,	// callback for sound output
	                         cb_data_t	// callback for dynamic labels
	                         );
//
//	The gain of the device can be set and changed to a value
//	in the range 0 .. 100. The value is mapped upon an appropriate
//	value for the device
	void	dab_Gain	(void *handle, uint16_t);	
//
//	The function setupChannel maps the name of the channel
//	onto a frequency for the device and prepares the device
//	for action
//	If the software was already running for another channel,
//	then the thread running the software will be halter first
	bool	dab_Channel	(void *handle, std::string);

//	the function runDAB will start a separate thread, running the
//	dab decoding software at the selected channel.
//	If DAB data, i.e. an ensemble, is found, then the function
//	passed as callback is called with as parameter the std::list
//	of strings, representing the names of the programs in that
//	ensemble.  If no data was found, the list is empty
//	Note that the thread executing the dab decoding will continue
//	to run.
	void	dab_run		(void *handle, cb_ensemble_t);
//
//	with dab_Service, the user may - finally - select a program
//	to be decoded. This - obviously only makes sense when
//	there are programs and "runDAB" is still active.
//	The name of the program may be a prefix of the real name,
//	however, letter case is important.
	bool	dab_Service	(void *handle, std::string, cb_programdata_t);
//
//	experimenting with:
	bool	dab_Service_Id	(void *handle, int32_t, cb_programdata_t);
//
//	the function stop will stop the running of the thread 
//	executing the dab decoding software
	void	dab_stop	(void *handle);
//
//	the exit function will close down the library software
	void	dab_exit	(void **handle);
//
//
////////////////// G E N E R A T I N G  T H E  L I B R A R Y //////////////
//
//	The dab-library directory contains a CMakeLists.txt script
//	for use with CMake
//
//	You will need a few libraries to be installed, and
//	you have to make a choice for the device you want
}
#endif

