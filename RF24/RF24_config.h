
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __RF24_CONFIG_H__
#define __RF24_CONFIG_H__

#include "../Core/pgmspace.h"
#include "../Core/bit_constants.h"
#include "../Core/util.h"
#include "../timer-driver.h"
#include "../gpio-driver.h"
#include "../led-driver.h"
   
#include "../SPI/SPI.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern SPIClass SPI;

#define _BV(x) (1<<(x))

#undef SERIAL_DEBUG
#ifdef SERIAL_DEBUG
#define IF_SERIAL_DEBUG(x) ({x;})
#else
#define IF_SERIAL_DEBUG(x)
#endif

// Avoid spurious warnings
#if 1
#if ! defined( NATIVE ) && defined( ARDUINO )
#undef PROGMEM
#define PROGMEM __attribute__(( section(".progmem.data") ))
#undef PSTR
#define PSTR(s) (__extension__({static const char __c[] PROGMEM = (s); &__c[0];}))
#endif
#endif

typedef uint16_t prog_uint16_t;
#define printf_P printf
#define PROGMEM
#define PRIPSTR "%s"
#endif

// vim:ai:cin:sts=2 sw=2 ft=cpp
