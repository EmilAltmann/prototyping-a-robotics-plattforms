#ifndef BLUELIB_H
#define BLUELIB_H

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hardware/pio.h"
#include "btstack.h"
#include "pico/cyw43_arch.h"

#ifndef sm3
#define sm3 3
#endif

#define RFCOMM_SERVER_CHANNEL 1

static void packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);

static uint16_t rfcomm_channel_id;
static uint8_t  spp_service_buffer[150];
static btstack_packet_callback_registration_t hci_event_callback_registration;

static char * lineBuffer;
static uint16_t sizeof_lineBuffer;

void send_string(char c[], uint16_t len);

int setupBluetoothTransmission();

int btstack_main(int argc, const char * argv[]);

#endif
