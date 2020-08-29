/*!
 * @file M62429_volume.h
 * 
 * @section author Author
 * Written by Tauno Erik
 * 
 */

#ifndef M62429_VOLUME_h
#define M62429_VOLUME_h

#include <Arduino.h>
#include "constants.h"

/**
 * Write volume to M62459 chip
 * 
 * @param volume The volume value 0- 100
 */
void set_volume (uint8_t volume)
{
	uint8_t bits;
	uint16_t data = 0; // control word is built by OR-ing in the bits

	// convert attenuation to volume
	volume = (volume > 100) ? 0 : (((volume * 83) / -100) + 83); // remember 0 is full volume!
       // generate 10 bits of data
	data |= (0 << 0); // D0 (channel select: 0=ch1, 1=ch2)
	data |= (0 << 1); // D1 (individual/both select: 0=both, 1=individual)
	data |= ((21 - (volume / 4)) << 2); // D2...D6 (ATT1: coarse attenuator: 0,-4dB,-8dB, etc.. steps of 4dB)
	data |= ((3 - (volume % 4)) << 7); // D7...D8 (ATT2: fine attenuator: 0...-1dB... steps of 1dB)
	data |= (0b11 << 9); // D9...D10 // D9 & D10 must both be 1

	for (bits = 0; bits < 11; bits++) { // send out 11 control bits
		delayMicroseconds (2); // pg.4 - M62429P/FP datasheet
		digitalWrite (VOLUME_DT_PIN, 0);
		delayMicroseconds (2);
		digitalWrite (VOLUME_CLK_PIN, 0);
		delayMicroseconds (2);
		digitalWrite (VOLUME_DT_PIN, (data >> bits) & 0x01);
		delayMicroseconds (2);
		digitalWrite (VOLUME_CLK_PIN, 1);
	}
	delayMicroseconds (2);
	digitalWrite (VOLUME_DT_PIN, 1); // final clock latches data in
	delayMicroseconds (2);
	digitalWrite (VOLUME_CLK_PIN, 0);
	//return data; // return bit pattern in case you want it :)
}

#endif // I2C_SCANNER_h