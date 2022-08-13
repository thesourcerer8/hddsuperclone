// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


#include <stdbool.h>
#include <stdint.h>

#define MAX_USB_STRING_SIZE 256 // max USB string is 128 UTF-16 chars

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// generic chineese red relay
// uses libusb found at http://libusb.sourceforge.net
// To install on Ubuntu: sudo apt-get install libusb-dev
#define CHEAP_RED_RELAY_VENDOR_ID  0x16c0
#define CHEAP_RED_RELAY_PRODUCT_ID 0x05df
#define CHEAP_RED_RELAY_VENDOR_NAME "www.dcttech.com"
#define CHEAP_RED_RELAY_PRODUCT_NAME_PREF "USBRelay"  // + number
#define CHEAP_RED_RELAY_ID_STR_LEN 5 // length of "unique serial number" in the devices
#define CHEAP_RED_RELAY_USB_REPORT_REQUEST_TIMEOUT 5000 // in milliseconds
// request type
#define CHEAP_RED_RELAY_USB_DIR_OUT 0
#define CHEAP_RED_RELAY_USB_ENDPOINT_OUT 0
#define CHEAP_RED_RELAY_USB_DIR_IN 0x80
#define CHEAP_RED_RELAY_USB_ENDPOINT_IN 0x80
#define CHEAP_RED_RELAY_USB_TYPE_CLASS 0x20
#define CHEAP_RED_RELAY_USB_RECIP_DEVICE 0
// request
#define CHEAP_RED_RELAY_USBRQ_HID_GET_REPORT    0x01
#define CHEAP_RED_RELAY_USBRQ_HID_SET_REPORT    0x09
// value
#define CHEAP_RED_RELAY_USB_HID_REPORT_TYPE_FEATURE 3
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// USBmicro U451
#define USBMICRO_U451_RELAY_VENDOR_ID 0x0DE7
#define USBMICRO_U451_RELAY_PRODUCT_ID 0x01C3
// request type
#define USBMICRO_U451_RELAY_REQUESTTYPE 0x21
// request
#define USBMICRO_U451_RELAY_REQUEST 9
// value
#define USBMICRO_U451_RELAY_VALUE 0x200
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// YEPKIT YKUSH
#define YEPKIT_RELAY_VENDOR_ID 0x04d8
#define YEPKIT_YKUSH3_PRODUCT_ID 0xF11B    // YKUSH3 3 ports 64 bytes
#define YEPKIT_YKUSH_PRODUCT_ID 0xF2F7     // ykush 3 ports 64 bytes
#define YEPKIT_YKUSHLEGACY_PRODUCT_ID 0x0042    // ykushlegacy 3 ports 6 bytes
#define YEPKIT_YKUSHXS_PRODUCT_ID 0xF0CD    // ykushxs 1 port 64 bytes
#define YEPKIT_VENDOR_NAME "Yepkit Lda."
#define YEPKIT_PRODUCT_NAME_PREF "YKUSH"
#define YEPKIT_USB_ENDPOINT_OUT 0x01
#define YEPKIT_USB_ENDPOINT_IN 0x81
#define YEPKIT_USB_INTERRUPT_TIMEOUT 5000 // in milliseconds



bool activate_primary_relay1_ccc = true;
bool activate_primary_relay2_ccc = true;
bool activate_primary_relay3_ccc = true;
bool activate_primary_relay4_ccc = true;
bool activate_primary_relay5_ccc = true;
bool activate_primary_relay6_ccc = true;
bool activate_primary_relay7_ccc = true;
bool activate_primary_relay8_ccc = true;
bool deactivate_primary_relay1_ccc = false;
bool deactivate_primary_relay2_ccc = false;
bool deactivate_primary_relay3_ccc = false;
bool deactivate_primary_relay4_ccc = false;
bool deactivate_primary_relay5_ccc = false;
bool deactivate_primary_relay6_ccc = false;
bool deactivate_primary_relay7_ccc = false;
bool deactivate_primary_relay8_ccc = false;



















