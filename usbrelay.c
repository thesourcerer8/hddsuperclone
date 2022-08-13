// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include "common.h"
#include "usbrelay.h"
#include "clone_gui_common.h"






int find_all_usb_devices_ccc(void)
{
  if (superbyte_ccc[62] == 0x15)
  {
    sprintf (tempmessage_ccc, "Finding USB devices\n");
    message_now_ccc(tempmessage_ccc);
    // clear the list
    int i;
    for (i = 0; i < MAX_USB_DEVICES; i++)
    {
      usb_vendor_id_ccc[i] = 0;
      usb_product_id_ccc[i] = 0;
      usb_bus_number_ccc[i] = 0;
      usb_device_number_ccc[i] = 0;
      usb_known_relay_ccc[i] = 0;
      usb_mass_storage_ccc[i] = 0;
      usb_bulk_in_endpoint_ccc[i] = 0;
      usb_bulk_out_endpoint_ccc[i] = 0;
      strcpy (usb_vendor_string_ccc[i], "");
      strcpy (usb_product_string_ccc[i], "");
      strcpy (usb_serial_string_ccc[i], "");
      strcpy (usb_extra_id_string_ccc[i], "");
    }

    // initialize the usb system
    usb_init();
    usb_find_busses(); // update info on busses
    usb_find_devices(); // update info on devices
    struct usb_bus *bus = usb_get_busses(); // get actual bus objects

    struct usb_device *dev;

    usb_device_count_ccc = 0;
    int busnum = 0;
    // look through all busses
    for ( ; bus; bus = bus->next )
    {
      busnum++;
      // look at every device
      for ( dev = bus->devices; dev; dev = dev->next )
      {
        usb_bus_number_ccc[usb_device_count_ccc] = busnum;
        usb_device_number_ccc[usb_device_count_ccc] = dev->devnum;

        char buffer[256] = ""; // max USB string is 128 UTF-16 chars
        struct usb_dev_handle *dev_handle;
        if ( ( dev_handle = usb_open(dev) ) == NULL )
        {
          fprintf (stdout, "Failed to open device bus %d device %d (%s)\n", usb_bus_number_ccc[usb_device_count_ccc], usb_device_number_ccc[usb_device_count_ccc], strerror(errno));
          continue;
        }

        usb_vendor_id_ccc[usb_device_count_ccc] = dev->descriptor.idVendor;
        usb_product_id_ccc[usb_device_count_ccc] = dev->descriptor.idProduct;

        int rval;
        rval = usb_get_string_simple(dev_handle, dev->descriptor.iManufacturer, buffer, sizeof(buffer));
        if (rval >= 0)
        {
          strcpy (usb_vendor_string_ccc[usb_device_count_ccc], buffer);
        }
        else
        {
          fprintf (stdout, "Error: Unnable to get vendor string %04x:%04x (%s)\n", usb_vendor_id_ccc[i], usb_product_id_ccc[i], strerror(errno));
        }

        rval = usb_get_string_simple(dev_handle, dev->descriptor.iProduct, buffer, sizeof(buffer));
        if (rval >= 0)
        {
          strcpy ( usb_product_string_ccc[usb_device_count_ccc], buffer);
        }
        else
        {
          fprintf (stdout, "Error: Unable to get product string %04x:%04x (%s)\n", usb_vendor_id_ccc[i], usb_product_id_ccc[i], strerror(errno));
        }

        rval = usb_get_string_simple(dev_handle, dev->descriptor.iSerialNumber, buffer, sizeof(buffer));
        if (rval >= 0)
        {
          strcpy ( usb_serial_string_ccc[usb_device_count_ccc], buffer);
        }
        else
        {
          fprintf (stdout, "Error: Unable to get serial string %04x:%04x (%s)\n", usb_vendor_id_ccc[i], usb_product_id_ccc[i], strerror(errno));
        }

        // if the device is known to have an extra id or serial then get it
        // generic chineese red relay www.dcttech.com USBRelay
        if (dev->descriptor.idVendor == CHEAP_RED_RELAY_VENDOR_ID && dev->descriptor.idProduct == CHEAP_RED_RELAY_PRODUCT_ID)
        {
          int relay_id_length = CHEAP_RED_RELAY_ID_STR_LEN;
          int requesttype = CHEAP_RED_RELAY_USB_TYPE_CLASS | CHEAP_RED_RELAY_USB_RECIP_DEVICE | CHEAP_RED_RELAY_USB_ENDPOINT_IN;
          int request = CHEAP_RED_RELAY_USBRQ_HID_GET_REPORT;
          int value = CHEAP_RED_RELAY_USB_HID_REPORT_TYPE_FEATURE << 8;
          int index = 0;
          char buffer[8];
          int length = 8;
          int timer = CHEAP_RED_RELAY_USB_REPORT_REQUEST_TIMEOUT;
          int bytesreceived = usb_control_msg(dev_handle, requesttype, request, value, index, buffer, length, timer);
          if (bytesreceived == length)
          {
            strncpy(usb_extra_id_string_ccc[usb_device_count_ccc], buffer, relay_id_length);
          }
          else
          {
            fprintf (stdout, "Failed to get special id %04x:%04x, %d (%s)\n", usb_vendor_id_ccc[i], usb_product_id_ccc[i], bytesreceived, strerror(errno));
          }
          // if it is a known usb relay then mark it
          usb_known_relay_ccc[usb_device_count_ccc] = 1;
        }
        // YEPKIT YKUSH
        if (dev->descriptor.idVendor == YEPKIT_RELAY_VENDOR_ID && (dev->descriptor.idProduct == YEPKIT_YKUSH3_PRODUCT_ID || dev->descriptor.idProduct == YEPKIT_YKUSH_PRODUCT_ID || dev->descriptor.idProduct == YEPKIT_YKUSHLEGACY_PRODUCT_ID || dev->descriptor.idProduct == YEPKIT_YKUSHXS_PRODUCT_ID))
        {
          strcpy(usb_extra_id_string_ccc[usb_device_count_ccc], usb_serial_string_ccc[usb_device_count_ccc]);
          // if it is a known usb relay then mark it
          usb_known_relay_ccc[usb_device_count_ccc] = 1;
        }

        // if it is a known usb relay then mark it
#ifdef GODMODE
        if (dev->descriptor.idVendor == USBMICRO_U451_RELAY_VENDOR_ID && dev->descriptor.idProduct == USBMICRO_U451_RELAY_PRODUCT_ID)
        {
          usb_known_relay_ccc[usb_device_count_ccc] = 1;
        }
#endif

        if (1)
        {
          struct usb_config_descriptor *config = &dev->config[0];
          int icount;
          for (icount = 0; icount < config->bNumInterfaces; icount++)
          {
            struct usb_interface *ainterface = &config->interface[icount];
            int aicount;
            for (aicount = 0; aicount < ainterface->num_altsetting; aicount++)
            {
              struct usb_interface_descriptor *interface = &ainterface->altsetting[aicount];
              // mass storage is class 8 and scsi is subclass 6
              if ((interface->bInterfaceClass == 8 && interface->bInterfaceSubClass == 6) || (dev->descriptor.bDeviceClass == 8 &&  dev->descriptor.bDeviceSubClass == 6))
              {
                usb_mass_storage_ccc[usb_device_count_ccc] = 1;
                int ecount;
                for (ecount = 0; ecount < interface->bNumEndpoints; ecount++)
                {
                  struct usb_endpoint_descriptor *endpoint = &interface->endpoint[ecount];
                  if (usb_bulk_in_endpoint_ccc[usb_device_count_ccc] == 0)
                  {
                    if (endpoint->bEndpointAddress >= 0x80)
                    {
                      usb_bulk_in_endpoint_ccc[usb_device_count_ccc] = endpoint->bEndpointAddress;
                    }
                  }
                  if (usb_bulk_out_endpoint_ccc[usb_device_count_ccc] == 0)
                  {
                    if (endpoint->bEndpointAddress < 0x80)
                    {
                      usb_bulk_out_endpoint_ccc[usb_device_count_ccc] = endpoint->bEndpointAddress;
                    }
                  }
                }
              }
            }
          }
        }

        if ( usb_close(dev_handle) )
        {
          fprintf (stdout, "Failed to close device bus %d device %d (%s)\n", usb_bus_number_ccc[usb_device_count_ccc], usb_device_number_ccc[usb_device_count_ccc], strerror(errno));
        }

        usb_device_count_ccc++;
      }
    }

    // flip bus numbers so they match up with what the OS reports
    for (i = 0; i < usb_device_count_ccc; i++)
    {
      usb_bus_real_number_ccc[i] = (busnum + 1) - usb_bus_number_ccc[i];
    }

    // show all device info
    for (i = 0; i < usb_device_count_ccc; i++)
    {
      //fprintf (stdout, "%d:%d  %04x:%04x  %s  %s\n", usb_bus_real_number_ccc[i], usb_device_number_ccc[i], usb_vendor_id_ccc[i], usb_product_id_ccc[i], usb_vendor_string_ccc[i], usb_product_string_ccc[i]);
    }
  }
  return 0;
}





int choose_usb_relay_ccc(void)
{
  if (superbyte_ccc[62] == 0x15)
  {
    char input_text[32];
    char raw_value[32];
    int choice = -1;
    int keeptrying = 1;

    if (!superclone_ccc)
    {
      while (keeptrying)
      {
        strcpy(raw_value, "");
        int i;
        fprintf (stdout, "\nQ) Quit\n");
        fprintf (stdout, "R) Refresh device list\n");
        for (i = 0; i < usb_device_count_ccc; i++)
        {
          fprintf (stdout, "%d) %d:%d %04x:%04x %s %s %s\n", i+1, usb_bus_real_number_ccc[i], usb_device_number_ccc[i], usb_vendor_id_ccc[i], usb_product_id_ccc[i], usb_vendor_string_ccc[i], usb_product_string_ccc[i], usb_extra_id_string_ccc[i]);
        }

        fprintf (stdout, "Choose which device > ");

        fflush(stdout);

        if ( fgets(input_text, sizeof input_text, stdin) != NULL )
        {
          sscanf(input_text, "%s", raw_value);
          if (strcasecmp(raw_value, "Q") == 0)
          {
            return (-1);
          }
          else if (strcasecmp(raw_value, "R") == 0)
          {
            find_all_usb_devices_ccc();
          }
          else
          {
            char* endptr;
            choice = strtol(raw_value, &endptr, 0);
            if (*endptr)
            {
              fprintf (stderr, "Error! Choice \'%s\' was unable to processed.\n", raw_value);
            }
            else if (choice < 1 || choice > usb_device_count_ccc)
            {
              fprintf (stderr, "Error! Choice %d is out of range.\n", choice);
            }
            else
            {
              keeptrying = 0;
            }
          }
        }
      }
    }
    else
    {
      choice = new_usb_ccc + 1;
    }

    int i = choice - 1;
    if (verbose_ccc & DEBUG6)
    {
      fprintf (stdout, "choice=%d %d:%d %04x:%04x %s %s %s\n", i+1, usb_bus_real_number_ccc[i], usb_device_number_ccc[i], usb_vendor_id_ccc[i], usb_product_id_ccc[i], usb_vendor_string_ccc[i], usb_product_string_ccc[i], usb_extra_id_string_ccc[i]);
    }

    usbr1_bus_number_ccc = usb_bus_number_ccc[i];
    usbr1_bus_real_number_ccc = usb_bus_real_number_ccc[i];
    usbr1_device_number_ccc = usb_device_number_ccc[i];
    usbr1_vendor_id_ccc = usb_vendor_id_ccc[i];
    usbr1_product_id_ccc = usb_product_id_ccc[i];
    strcpy (usbr1_vendor_string_ccc, usb_vendor_string_ccc[i]);
    strcpy (usbr1_product_string_ccc, usb_product_string_ccc[i]);
    strcpy (usbr1_extra_id_string_ccc, usb_extra_id_string_ccc[i]);


    // initialize the usb system
    usb_init();
    usb_find_busses(); // update info on busses
    usb_find_devices(); // update info on devices
    struct usb_bus *bus = usb_get_busses(); // get actual bus objects

    struct usb_device *dev;

    int busnum = 0;
    // look through all busses
    for ( ; bus; bus = bus->next )
    {
      busnum++;
      // look at every device
      for ( dev = bus->devices; dev; dev = dev->next )
      {
        if (usbr1_bus_number_ccc == busnum && usbr1_device_number_ccc == dev->devnum)
        {
          if ( ( usbr1_dev_handle_ccc = usb_open(dev) ) == NULL )
          {
            fprintf (stdout, "Failed to open device bus %d device %d (%s)\n", usbr1_bus_number_ccc, usbr1_device_number_ccc, strerror(errno));
            usbr1_dev_handle_ccc = NULL;
            return -1;
          }
          usb_reset(usbr1_dev_handle_ccc);

          if (usbr1_vendor_id_ccc != dev->descriptor.idVendor || usbr1_product_id_ccc != dev->descriptor.idProduct)
          {
            fprintf (stdout, "ID does not match what was chosen (wanted %04x:%04x, found %04x:%04x\n", usbr1_vendor_id_ccc, usbr1_product_id_ccc, dev->descriptor.idVendor, dev->descriptor.idProduct);
            usb_close(usbr1_dev_handle_ccc);
            usbr1_dev_handle_ccc = NULL;
            return -1;
          }

          int status = usb_detach_kernel_driver_np(usbr1_dev_handle_ccc, 0);
          if (status && (errno != ENODATA))
          {
            fprintf (stdout, "Failed to detach device: (%s)\n", strerror(errno));
            usb_close(usbr1_dev_handle_ccc);
            usbr1_dev_handle_ccc = NULL;
            return -1;
          }

          status = usb_set_configuration(usbr1_dev_handle_ccc, 1);
          if (status)
          {
            fprintf (stdout, "Failed to set configuration: (%s)\n", strerror(errno));
            usb_close(usbr1_dev_handle_ccc);
            usbr1_dev_handle_ccc = NULL;
            return -1;
          }

          status = usb_claim_interface(usbr1_dev_handle_ccc, 0);
          if (status)
          {
            fprintf (stdout, "Failed to claim interface: (%s)\n", strerror(errno));
            usb_close(usbr1_dev_handle_ccc);
            usbr1_dev_handle_ccc = NULL;
            return -1;
          }

        }
      }
    }
    fprintf (stdout, "Interface connected\n");
  }
  return 0;
}





int choose_usb_device_ccc(void)
{
  if (superbyte_ccc[62] == 0x15)
  {
    char input_text[32];
    char raw_value[32];
    int choice = -1;
    int keeptrying = 1;

    if (!superclone_ccc)
    {
      while (keeptrying)
      {
        strcpy(raw_value, "");
        int i;
        fprintf (stdout, "\nQ) Quit\n");
        fprintf (stdout, "R) Refresh device list\n");
        for (i = 0; i < usb_device_count_ccc; i++)
        {
          fprintf (stdout, "%d) %d:%d %04x:%04x %x %x %s %s %s\n", i+1, usb_bus_real_number_ccc[i], usb_device_number_ccc[i], usb_vendor_id_ccc[i], usb_product_id_ccc[i], usb_bulk_in_endpoint_ccc[i], usb_bulk_out_endpoint_ccc[i], usb_vendor_string_ccc[i], usb_product_string_ccc[i], usb_extra_id_string_ccc[i]);
        }

        fprintf (stdout, "Choose which device > ");

        fflush(stdout);

        if ( fgets(input_text, sizeof input_text, stdin) != NULL )
        {
          sscanf(input_text, "%s", raw_value);
          if (strcasecmp(raw_value, "Q") == 0)
          {
            return (-1);
          }
          else if (strcasecmp(raw_value, "R") == 0)
          {
            find_all_usb_devices_ccc();
          }
          else
          {
            char* endptr;
            choice = strtol(raw_value, &endptr, 0);
            if (*endptr)
            {
              fprintf (stderr, "Error! Choice \'%s\' was unable to processed.\n", raw_value);
            }
            else if (choice < 1 || choice > usb_device_count_ccc)
            {
              fprintf (stderr, "Error! Choice %d is out of range.\n", choice);
            }
            else
            {
              keeptrying = 0;
            }
          }
        }
      }
    }
    else
    {
      choice = new_usb_ccc + 1;
    }

    int i = choice - 1;
    if (verbose_ccc & DEBUG6)
    {
      fprintf (stdout, "choice=%d %d:%d %04x:%04x %d %s %s %s\n", i+1, usb_bus_real_number_ccc[i], usb_device_number_ccc[i], usb_vendor_id_ccc[i], usb_product_id_ccc[i], usb_mass_storage_ccc[i], usb_vendor_string_ccc[i], usb_product_string_ccc[i], usb_extra_id_string_ccc[i]);
    }

    if (superclone_ccc && !usb_mass_storage_ccc[i])
    {
      sprintf(tempmessage_ccc, "Error: USB device is not mass storage.\n");
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }

    usbd1_bus_number_ccc = usb_bus_number_ccc[i];
    usbd1_bus_real_number_ccc = usb_bus_real_number_ccc[i];
    usbd1_device_number_ccc = usb_device_number_ccc[i];
    usbd1_vendor_id_ccc = usb_vendor_id_ccc[i];
    usbd1_product_id_ccc = usb_product_id_ccc[i];
    usbd1_bulk_in_endpoint_ccc = usb_bulk_in_endpoint_ccc[i];
    usbd1_bulk_out_endpoint_ccc = usb_bulk_out_endpoint_ccc[i];
    strcpy (usbd1_vendor_string_ccc, usb_vendor_string_ccc[i]);
    strcpy (usbd1_product_string_ccc, usb_product_string_ccc[i]);
    strcpy (usbd1_extra_id_string_ccc, usb_extra_id_string_ccc[i]);

    if (!superclone_ccc)
    {
      int ret = connect_usbd1_ccc();
      if (ret != 0)
      {
        return ret;
      }
    }
  }
  return 0;
}





int connect_usbd1_ccc(void)
{
  int found_device = 0;
  int dev_cnt_mod;
  for (dev_cnt_mod = 0; dev_cnt_mod < 5; dev_cnt_mod++)
  {
    if (found_device)
    {
      break;
    }
    // initialize the usb system
    usb_init();
    usb_find_busses(); // update info on busses
    usb_find_devices(); // update info on devices
    struct usb_bus *bus = usb_get_busses(); // get actual bus objects

    struct usb_device *dev;

    int busnum = 0;
    // look through all busses
    for ( ; bus; bus = bus->next )
    {
      busnum++;
      // look at every device
      for ( dev = bus->devices; dev; dev = dev->next )
      {
        if (usbd1_bus_number_ccc == busnum && (usbd1_device_number_ccc + dev_cnt_mod) == dev->devnum)
        {
          if ( ( usbd1_dev_handle_ccc = usb_open(dev) ) == NULL )
          {
            fprintf (stdout, "Failed to open device bus %d device %d (%s)\n", usbd1_bus_number_ccc, usbd1_device_number_ccc, strerror(errno));
            usbd1_dev_handle_ccc = NULL;
            return -1;
          }

          char buffer[256] = ""; // max USB string is 128 UTF-16 chars
          char vendorstring[256] = "";
          char productstring[256] = "";
          int rval;
          rval = usb_get_string_simple(usbd1_dev_handle_ccc, dev->descriptor.iManufacturer, buffer, sizeof(buffer));
          if (rval >= 0)
          {
            strcpy (vendorstring, buffer);
          }
          else
          {
            fprintf (stdout, "Error: Unnable to get vendor string (%s)\n", strerror(errno));
          }

          rval = usb_get_string_simple(usbd1_dev_handle_ccc, dev->descriptor.iProduct, buffer, sizeof(buffer));
          if (rval >= 0)
          {
            strcpy (productstring, buffer);
          }
          else
          {
            fprintf (stdout, "Error: Unable to get product string (%s)\n", strerror(errno));
          }

          if (usbd1_vendor_id_ccc != dev->descriptor.idVendor || usbd1_product_id_ccc != dev->descriptor.idProduct || strcmp(usbd1_vendor_string_ccc, vendorstring) || strcmp(usbd1_product_string_ccc, productstring))
          {
            fprintf (stdout, "ID does not match what was chosen:\nwanted %04x:%04x %s %s\nfound %04x:%04x %s %s\n", usbd1_vendor_id_ccc, usbd1_product_id_ccc, usbd1_vendor_string_ccc, usbd1_product_string_ccc, dev->descriptor.idVendor, dev->descriptor.idProduct, vendorstring, productstring);
            usb_close(usbd1_dev_handle_ccc);
            usbd1_dev_handle_ccc = NULL;
            return -1;
          }

          usb_reset(usbd1_dev_handle_ccc);

          int status = usb_detach_kernel_driver_np(usbd1_dev_handle_ccc, 0);
          if (status && (errno != ENODATA))
          {
            fprintf (stdout, "Failed to detach device: (%s)\n", strerror(errno));
            usb_close(usbd1_dev_handle_ccc);
            usbd1_dev_handle_ccc = NULL;
            return -1;
          }

          status = usb_set_configuration(usbd1_dev_handle_ccc, 1);
          if (status)
          {
            fprintf (stdout, "Failed to set configuration: (%s)\n", strerror(errno));
            usb_close(usbd1_dev_handle_ccc);
            usbd1_dev_handle_ccc = NULL;
            return -1;
          }

          status = usb_claim_interface(usbd1_dev_handle_ccc, 0);
          if (status)
          {
            fprintf (stdout, "Failed to claim interface: (%s)\n", strerror(errno));
            usb_close(usbd1_dev_handle_ccc);
            usbd1_dev_handle_ccc = NULL;
            return -1;
          }
          found_device = 1;
          break;
        }
      }
      if (found_device)
      {
        usbd1_device_number_ccc = usbd1_device_number_ccc + dev_cnt_mod;
        break;
      }
    }
  }
  if (found_device)
  {
    fprintf (stdout, "Interface connected\n");
  }
  else
  {
    fprintf (stdout, "Device not found\n");
    return -1;
  }
  int max_lun = usb_get_max_lun_ccc(initial_busy_wait_time_ccc / 1000);
  if (max_lun > 0 && !usb_lun_set_ccc)
  {
    if (set_lun_dialog_ccc(max_lun))
    {
      usb_close(usbd1_dev_handle_ccc);
      usbd1_dev_handle_ccc = NULL;
      fprintf (stdout, "Choosing LUN canceled\n");
      return -1;
    }
  }
  return 0;
}





int dissconnect_usbr1_ccc(void)
{
  if (usbr1_dev_handle_ccc != NULL)
  {
    usb_close(usbr1_dev_handle_ccc);
    usbr1_dev_handle_ccc = NULL;
  }
  return 0;
}





int dissconnect_usbd1_ccc(void)
{
  if (usbd1_dev_handle_ccc != NULL)
  {
    usb_close(usbd1_dev_handle_ccc);
    usbd1_dev_handle_ccc = NULL;
  }
  return 0;
}





int do_send_usbr_control_msg_ccc(int requesttype, int request, int value, int index)
{
  int status = 0;
  if (superbyte_ccc[62] == 0x15)
  {
    // handle, bytes, size, and timeout are globally set variables
    // int usb_control_msg(usb_dev_handle *dev, int requesttype, int request, int value, int index, char *bytes, int size, int timeout);
    // timeout is in milliseconds

    int ret = 0;
    ret = usb_control_msg(usbr1_dev_handle_ccc, requesttype, request, value, index, ccc_usbbuffer_ccc, ccc_main_usbbuffer_size_ccc, usb_timeout_ccc);

    set_number_variable_value_ccc("$usb_return_status", ret);
    set_number_variable_value_ccc("$usb_return_error", errno);

    if (ret < 0)
    {
      fprintf (stdout, "USB control message error %d (%s)\n", ret, strerror(abs(ret)));
      status = 1;
    }
    else if (ret != (int)ccc_main_usbbuffer_size_ccc)
    {
      fprintf (stdout, "USB control message short return %d\n", ret);
      status = 1;
    }
  }

  return status;
}





int do_send_usbr_interrupt_write_ccc(int endpointout)
{
  int status = 0;
  if (superbyte_ccc[62] == 0x15)
  {
    int ret = usb_interrupt_write(usbr1_dev_handle_ccc, endpointout, ccc_usbbuffer_ccc, ccc_main_usbbuffer_size_ccc, usb_timeout_ccc);

    set_number_variable_value_ccc("$usb_return_status", ret);
    set_number_variable_value_ccc("$usb_return_error", errno);

    if (ret < 0)
    {
      fprintf (stdout, "USB interrupt write error %d (%s)\n", ret, strerror(abs(ret)));
      status = 1;
    }
    else if (ret != (int)ccc_main_usbbuffer_size_ccc)
    {
      fprintf (stdout, "USB interrupt write short return %d\n", ret);
      status = 1;
    }
  }

  return status;
}





int do_send_usbr_interrupt_read_ccc(int endpointin)
{
  int status = 0;
  if (superbyte_ccc[62] == 0x15)
  {
    int ret = usb_interrupt_read(usbr1_dev_handle_ccc, endpointin, ccc_usbbuffer_ccc, ccc_main_usbbuffer_size_ccc, usb_timeout_ccc);

    set_number_variable_value_ccc("$usb_return_status", ret);
    set_number_variable_value_ccc("$usb_return_error", errno);

    if (ret < 0)
    {
      fprintf (stdout, "USB interrupt read error %d (%s)\n", ret, strerror(abs(ret)));
      status = 1;
    }
    else if (ret != (int)ccc_main_usbbuffer_size_ccc)
    {
      fprintf (stdout, "USB interrupt read short return %d\n", ret);
      status = 1;
    }
  }

  return status;
}





int do_send_usbd_control_msg_ccc(int requesttype, int request, int value, int index, int timeout)
{
  int status = 0;
  if (superbyte_ccc[62] == 0x15)
  {
    // handle, bytes, and size are globally set variables
    // int usb_control_msg(usb_dev_handle *dev, int requesttype, int request, int value, int index, char *bytes, int size, int timeout);
    // timeout is in milliseconds

    int ret = 0;
    ret = usb_control_msg(usbd1_dev_handle_ccc, requesttype, request, value, index, ccc_usbbuffer_ccc, ccc_main_usbbuffer_size_ccc, timeout);

    set_number_variable_value_ccc("$usb_return_status", ret);
    set_number_variable_value_ccc("$usb_return_error", errno);

    if (ret < 0)
    {
      fprintf (stdout, "USB control message error %d (%s)\n", ret, strerror(abs(ret)));
      status = 1;
    }
    else if (ret != (int)ccc_main_usbbuffer_size_ccc)
    {
      fprintf (stdout, "USB control message short return %d\n", ret);
      status = 1;
    }
    if (superclone_ccc)
    {
      return ret;
    }
  }

  return status;
}





int do_usb_read_sense_ccc(int timeout)
{
  if (superbyte_ccc[62] == 0x15)
  {
    int ret = 0;
    ret = usb_bulk_read(usbd1_dev_handle_ccc, usbd1_bulk_in_endpoint_ccc, sensebuffer_ccc, sensebuffer_size_ccc, timeout);

    if (debug_ccc & DEBUG19)
    {
      fprintf (stdout, "endpointin=%x size=%lld timeout=%d\n", usbd1_bulk_in_endpoint_ccc, sensebuffer_size_ccc, timeout);
      int i;
      int size = sensebuffer_size_ccc;
      for (i = 0; i < size; i+=16)
      {
        fprintf (stdout, "%x: ", i);
        unsigned char *c;
        int n;
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)sensebuffer_ccc+i+n;
          fprintf (stdout, "%02x ", *c);
        }
        fprintf (stdout, "   ");
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)sensebuffer_ccc+i+n;
          fprintf (stdout, "%c", isprint(*c) ? *c : '.');
        }
        fprintf (stdout, "\n");
      }
    }

    if (ret < 0)
    {
      fprintf (stdout, "USB sense read error %d (%s)\n", ret, strerror(abs(ret)));
      reset_usb_endpoint_in_ccc();
    }
    else if (ret != (int)sensebuffer_size_ccc)
    {
      fprintf (stdout, "USB sense read short return %d\n", ret);
      reset_usb_endpoint_in_ccc();
    }
    if (superclone_ccc)
    {
      return ret;
    }
  }

  return 0;
}





int do_usb_raw_read_ccc(int timeout)
{
  if (superbyte_ccc[62] == 0x15)
  {
    int ret = 0;
    ret = usb_bulk_read(usbd1_dev_handle_ccc, usbd1_bulk_in_endpoint_ccc, ccc_usbbuffer_ccc, ccc_main_usbbuffer_size_ccc, timeout);

    if (debug_ccc & DEBUG19)
    {
      fprintf (stdout, "endpointin=%x size=%lld timeout=%d\n", usbd1_bulk_in_endpoint_ccc, ccc_main_usbbuffer_size_ccc, timeout);
      int i;
      int size = ccc_main_usbbuffer_size_ccc;
      for (i = 0; i < size; i+=16)
      {
        fprintf (stdout, "%x: ", i);
        unsigned char *c;
        int n;
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)ccc_usbbuffer_ccc+i+n;
          fprintf (stdout, "%02x ", *c);
        }
        fprintf (stdout, "   ");
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)ccc_usbbuffer_ccc+i+n;
          fprintf (stdout, "%c", isprint(*c) ? *c : '.');
        }
        fprintf (stdout, "\n");
      }
    }

    set_number_variable_value_ccc("$usb_return_status", ret);
    set_number_variable_value_ccc("$usb_return_error", errno);

    if (ret < 0)
    {
      fprintf (stdout, "USB raw read error %d (%s)\n", ret, strerror(abs(ret)));
      reset_usb_endpoint_in_ccc();
    }
    else if (ret != (int)ccc_main_usbbuffer_size_ccc)
    {
      fprintf (stdout, "USB raw read short return %d\n", ret);
      reset_usb_endpoint_in_ccc();
    }
    if (superclone_ccc)
    {
      return ret;
    }
  }

  return 0;
}





int do_usb_raw_write_ccc(int timeout)
{
  if (superbyte_ccc[62] == 0x15)
  {

    if (debug_ccc & DEBUG19)
    {
      fprintf (stdout, "endpointout=%x size=%lld timeout=%d\n", usbd1_bulk_out_endpoint_ccc, ccc_main_usbbuffer_size_ccc, timeout);
      int i;
      int size = ccc_main_usbbuffer_size_ccc;
      for (i = 0; i < size; i+=16)
      {
        fprintf (stdout, "%x: ", i);
        unsigned char *c;
        int n;
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)ccc_usbbuffer_ccc+i+n;
          fprintf (stdout, "%02x ", *c);
        }
        fprintf (stdout, "   ");
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)ccc_usbbuffer_ccc+i+n;
          fprintf (stdout, "%c", isprint(*c) ? *c : '.');
        }
        fprintf (stdout, "\n");
      }
    }

    int ret = 0;
    ret = usb_bulk_write(usbd1_dev_handle_ccc, usbd1_bulk_out_endpoint_ccc, ccc_usbbuffer_ccc, ccc_main_usbbuffer_size_ccc, timeout);

    set_number_variable_value_ccc("$usb_return_status", ret);
    set_number_variable_value_ccc("$usb_return_error", errno);

    if (ret < 0)
    {
      fprintf (stdout, "USB raw write error %d (%s)\n", ret, strerror(abs(ret)));
      reset_usb_endpoint_out_ccc();
    }
    else if (ret != (int)ccc_main_usbbuffer_size_ccc)
    {
      fprintf (stdout, "USB raw write short return %d\n", ret);
      reset_usb_endpoint_out_ccc();
    }
    if (superclone_ccc)
    {
      return ret;
    }
  }

  return 0;
}





int usb_read_data_ccc(int timeout)
{
  if (superbyte_ccc[62] == 0x15)
  {
    int ret = 0;
    ret = usb_bulk_read(usbd1_dev_handle_ccc, usbd1_bulk_in_endpoint_ccc, ccc_buffer_ccc, ccc_main_buffer_size_ccc, timeout);

    if (debug_ccc & DEBUG19)
    {
      fprintf (stdout, "endpointin=%x size=%lld timeout=%d\n", usbd1_bulk_in_endpoint_ccc, ccc_main_buffer_size_ccc, timeout);
      int i;
      int size = ccc_main_buffer_size_ccc;
      for (i = 0; i < size; i+=16)
      {
        fprintf (stdout, "%x: ", i);
        unsigned char *c;
        int n;
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)ccc_buffer_ccc+i+n;
          fprintf (stdout, "%02x ", *c);
        }
        fprintf (stdout, "   ");
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)ccc_buffer_ccc+i+n;
          fprintf (stdout, "%c", isprint(*c) ? *c : '.');
        }
        fprintf (stdout, "\n");
      }
    }

    if (ret < 0)
    {
      fprintf (stdout, "USB raw read error %d (%s)\n", ret, strerror(abs(ret)));
      reset_usb_endpoint_in_ccc();
    }
    else if (ret != (int)ccc_main_buffer_size_ccc)
    {
      fprintf (stdout, "USB raw read short return %d\n", ret);
      reset_usb_endpoint_in_ccc();
    }
    return ret;
  }

  return 0;
}





int usb_write_data_ccc(int timeout)
{
  if (superbyte_ccc[62] == 0x15)
  {

    if (debug_ccc & DEBUG19)
    {
      fprintf (stdout, "endpointout=%x size=%lld timeout=%d\n", usbd1_bulk_out_endpoint_ccc, ccc_main_buffer_size_ccc, timeout);
      int i;
      int size = ccc_main_buffer_size_ccc;
      for (i = 0; i < size; i+=16)
      {
        fprintf (stdout, "%x: ", i);
        unsigned char *c;
        int n;
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)ccc_buffer_ccc+i+n;
          fprintf (stdout, "%02x ", *c);
        }
        fprintf (stdout, "   ");
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)ccc_buffer_ccc+i+n;
          fprintf (stdout, "%c", isprint(*c) ? *c : '.');
        }
        fprintf (stdout, "\n");
      }
    }

    int ret = 0;
    ret = usb_bulk_write(usbd1_dev_handle_ccc, usbd1_bulk_out_endpoint_ccc, ccc_buffer_ccc, ccc_main_buffer_size_ccc, timeout);

    if (ret < 0)
    {
      fprintf (stdout, "USB raw write error %d (%s)\n", ret, strerror(abs(ret)));
      reset_usb_endpoint_out_ccc();
    }
    else if (ret != (int)ccc_main_buffer_size_ccc)
    {
      fprintf (stdout, "USB raw write short return %d\n", ret);
      reset_usb_endpoint_out_ccc();
    }
    if (superclone_ccc)
    {
      return ret;
    }
  }

  return 0;
}





int do_usb_send_cbw_ccc(int timeout)
{
  if (superbyte_ccc[62] == 0x15)
  {

    if (debug_ccc & DEBUG19)
    {
      fprintf (stdout, "endpointout=%x size=%d timeout=%d\n", usbd1_bulk_out_endpoint_ccc, USBCBW_BUFFER_SIZE, timeout);
      int i;
      int size = USBCBW_BUFFER_SIZE;
      for (i = 0; i < size; i+=16)
      {
        fprintf (stdout, "%x: ", i);
        unsigned char *c;
        int n;
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)usbcbwbuffer_ccc+i+n;
          fprintf (stdout, "%02x ", *c);
        }
        fprintf (stdout, "   ");
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)usbcbwbuffer_ccc+i+n;
          fprintf (stdout, "%c", isprint(*c) ? *c : '.');
        }
        fprintf (stdout, "\n");
      }
    }

    int ret = 0;
    ret = usb_bulk_write(usbd1_dev_handle_ccc, usbd1_bulk_out_endpoint_ccc, usbcbwbuffer_ccc, USBCBW_BUFFER_SIZE, timeout);

    if (ret < 0)
    {
      fprintf (stdout, "USB send cbw error %d (%s)\n", ret, strerror(abs(ret)));
      reset_usb_endpoint_out_ccc();
    }
    else if (ret != USBCBW_BUFFER_SIZE)
    {
      fprintf (stdout, "USB send cbw short return %d\n", ret);
      reset_usb_endpoint_out_ccc();
    }
    return ret;
  }
  return 0;
}





int do_usb_receive_csw_ccc(int timeout)
{
  if (superbyte_ccc[62] == 0x15)
  {
    memset (usbcswbuffer_ccc, 0, USBCSW_BUFFER_SIZE);
    int ret = 0;
    ret = usb_bulk_read(usbd1_dev_handle_ccc, usbd1_bulk_in_endpoint_ccc, usbcswbuffer_ccc, USBCSW_BUFFER_SIZE, timeout);

    if (debug_ccc & DEBUG19)
    {
      fprintf (stdout, "endpointin=%x size=%d timeout=%d\n", usbd1_bulk_in_endpoint_ccc, USBCSW_BUFFER_SIZE, timeout);
      int i;
      int size = USBCSW_BUFFER_SIZE;
      for (i = 0; i < size; i+=16)
      {
        fprintf (stdout, "%x: ", i);
        unsigned char *c;
        int n;
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)usbcswbuffer_ccc+i+n;
          fprintf (stdout, "%02x ", *c);
        }
        fprintf (stdout, "   ");
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)usbcswbuffer_ccc+i+n;
          fprintf (stdout, "%c", isprint(*c) ? *c : '.');
        }
        fprintf (stdout, "\n");
      }
    }

    if (ret < 0)
    {
      fprintf (stdout, "USB raw read error %d (%s)\n", ret, strerror(abs(ret)));
      reset_usb_endpoint_in_ccc();
    }
    else if (ret != USBCSW_BUFFER_SIZE)
    {
      fprintf (stdout, "USB raw read short return %d\n", ret);
      reset_usb_endpoint_in_ccc();
    }
    return ret;
  }
  return 0;
}





int usb_get_max_lun_ccc(int timeout)
{
  if (superclone_ccc)
    {
      sprintf (tempmessage_ccc, "usb get max lun\n");
      message_console_log_ccc(tempmessage_ccc, 0);
    }
  int requesttype = 0xa1;
  int request = 0xfe;
  int value = 0;
  int index = 0;
  char buffer;
  int buffsize = 1;
  int ret = usb_control_msg(usbd1_dev_handle_ccc, requesttype, request, value, index, &buffer, buffsize, timeout);
  if (ret < 0)
  {
    fprintf (stdout, "USB control message error %d (%s)\n", ret, strerror(abs(ret)));
    return -1;
  }
  fprintf (stdout, "Max LUN = %d\n", buffer);

  return buffer;
}





int check_usb_endpoint_in_ccc(int timeout)
{
  int requesttype = 0x82;
  int request = 0;
  int value = 0;
  int index = usbd1_bulk_in_endpoint_ccc;
  char buffer[2];
  int buffsize = 2;
  int ret = usb_control_msg(usbd1_dev_handle_ccc, requesttype, request, value, index, buffer, buffsize, timeout);
  if (ret < 0)
  {
    fprintf (stdout, "USB control message error %d (%s)\n", ret, strerror(abs(ret)));
    return -1;
  }
  else if (ret != buffsize)
  {
    fprintf (stdout, "USB control message short return %d\n", ret);
    return -1;
  }
  uint16_t status;
  memcpy (&status, buffer, 2);

  return status;
}





int check_usb_endpoint_out_ccc(int timeout)
{
  int requesttype = 0x82;
  int request = 0;
  int value = 0;
  int index = usbd1_bulk_out_endpoint_ccc;
  char buffer[2];
  int buffsize = 2;
  int ret = usb_control_msg(usbd1_dev_handle_ccc, requesttype, request, value, index, buffer, buffsize, timeout);
  if (ret < 0)
  {
    fprintf (stdout, "USB control message error %d (%s)\n", ret, strerror(abs(ret)));
    return -1;
  }
  else if (ret != buffsize)
  {
    fprintf (stdout, "USB control message short return %d\n", ret);
    return -1;
  }
  uint16_t status;
  memcpy (&status, buffer, 2);

  return status;
}





int reset_usb_endpoint_in_ccc(void)
{
  fprintf (stdout, "USB endpoint in reset\n");
  int ret = usb_clear_halt(usbd1_dev_handle_ccc, usbd1_bulk_in_endpoint_ccc);
  if (ret < 0)
  {
    fprintf (stdout, "USB clear halt error %d (%s)\n", ret, strerror(abs(ret)));
  }

  return ret;
}





int reset_usb_endpoint_out_ccc(void)
{
  fprintf (stdout, "USB endpoint out reset\n");
  int ret = usb_clear_halt(usbd1_dev_handle_ccc, usbd1_bulk_out_endpoint_ccc);
  if (ret < 0)
  {
    fprintf (stdout, "USB clear halt error %d (%s)\n", ret, strerror(abs(ret)));
  }

  return ret;
}





int usb_auto_endpoint_reset(int timeout)
{
  int ret = check_usb_endpoint_in_ccc(timeout);
  if (ret < 0)
  {
    return ret;
  }
  else if (ret > 0)
  {
    ret = reset_usb_endpoint_in_ccc();
    if (ret < 0)
    {
      return ret;
    }
  }
  ret = check_usb_endpoint_out_ccc(timeout);
  if (ret < 0)
  {
    return ret;
  }
  else if (ret > 0)
  {
    ret = reset_usb_endpoint_out_ccc();
    if (ret < 0)
    {
      return ret;
    }
  }
  return 0;
}





int usb_reset_recovery_ccc(int timeout)
{
  fprintf (stdout, "USB Reset Recovery\n");
  usb_bulk_only_reset_ccc(timeout);
  reset_usb_endpoint_in_ccc();
  reset_usb_endpoint_out_ccc();
  return 0;
}





int usb_bulk_only_reset_ccc(int timeout)
{
  if (superclone_ccc)
    {
      sprintf (tempmessage_ccc, "usb bulk reset\n");
      message_console_log_ccc(tempmessage_ccc, 0);
    }
  int requesttype = 0x21;
  int request = 0xff;
  int value = 0;
  int index = 0;
  char buffer[2];
  int buffsize = 0;
  int ret = usb_control_msg(usbd1_dev_handle_ccc, requesttype, request, value, index, buffer, buffsize, timeout);
  if (ret < 0)
  {
    fprintf (stdout, "USB control message error %d (%s)\n", ret, strerror(abs(ret)));
    return -1;
  }

  return ret;
}





int do_usb_reset_ccc(void)
{
  if (superbyte_ccc[62] == 0x15)
  {
    if (superclone_ccc)
    {
      sprintf (tempmessage_ccc, "usb port reset\n");
      message_console_log_ccc(tempmessage_ccc, 0);
    }
    int ret = 0;
    ret = usb_reset(usbd1_dev_handle_ccc);
    set_number_variable_value_ccc("$usb_return_status", ret);
    set_number_variable_value_ccc("$usb_return_error", errno);
    if (ret)
    {
      fprintf (stdout, "USB reset error status/errno %d/%d (%s/%s)\n", ret, errno, strerror(abs(ret)), strerror(errno));
    }

    ret = usb_detach_kernel_driver_np(usbd1_dev_handle_ccc, 0);
    if (ret && (errno != ENODATA))
    {
      fprintf (stdout, "Failed to detach device: status/errno %d/%d (%s/%s)\n", ret, errno, strerror(abs(ret)), strerror(errno));
    }

    ret = usb_set_configuration(usbd1_dev_handle_ccc, 1);
    if (ret)
    {
      fprintf (stdout, "Failed to set configuration: status/errno %d/%d (%s/%s)\n", ret, errno, strerror(abs(ret)), strerror(errno));
    }

    ret = usb_claim_interface(usbd1_dev_handle_ccc, 0);
    if (ret)
    {
      fprintf (stdout, "Failed to claim interface: status/errno %d/%d (%s/%s)\n", ret, errno, strerror(abs(ret)), strerror(errno));
    }
  }

  return 0;
}





void load_primary_relay_settings_ccc (void)
{
  fprintf (stdout, "loading primary relay settings\n");

  primary_relay_settings_ccc.activate_primary_relay1 = activate_primary_relay1_ccc;
  primary_relay_settings_ccc.activate_primary_relay2 = activate_primary_relay2_ccc;
  primary_relay_settings_ccc.activate_primary_relay3 = activate_primary_relay3_ccc;
  primary_relay_settings_ccc.activate_primary_relay4 = activate_primary_relay4_ccc;
  primary_relay_settings_ccc.activate_primary_relay5 = activate_primary_relay5_ccc;
  primary_relay_settings_ccc.activate_primary_relay6 = activate_primary_relay6_ccc;
  primary_relay_settings_ccc.activate_primary_relay7 = activate_primary_relay7_ccc;
  primary_relay_settings_ccc.activate_primary_relay8 = activate_primary_relay8_ccc;
  primary_relay_settings_ccc.deactivate_primary_relay1 = deactivate_primary_relay1_ccc;
  primary_relay_settings_ccc.deactivate_primary_relay2 = deactivate_primary_relay2_ccc;
  primary_relay_settings_ccc.deactivate_primary_relay3 = deactivate_primary_relay3_ccc;
  primary_relay_settings_ccc.deactivate_primary_relay4 = deactivate_primary_relay4_ccc;
  primary_relay_settings_ccc.deactivate_primary_relay5 = deactivate_primary_relay5_ccc;
  primary_relay_settings_ccc.deactivate_primary_relay6 = deactivate_primary_relay6_ccc;
  primary_relay_settings_ccc.deactivate_primary_relay7 = deactivate_primary_relay7_ccc;
  primary_relay_settings_ccc.deactivate_primary_relay8 = deactivate_primary_relay8_ccc;
  primary_relay_settings_ccc.primary_relay_activation_time = primary_relay_activation_time_ccc;
  primary_relay_settings_ccc.primary_relay_delay_time = primary_relay_delay_time_ccc;
  strcpy (primary_relay_settings_ccc.primary_relay_name, primary_relay_name_ccc);

}





void update_primary_relay_settings_ccc (void)
{
  fprintf (stdout, "updating primary relay settings\n");

  activate_primary_relay1_ccc = primary_relay_settings_ccc.activate_primary_relay1;
  activate_primary_relay2_ccc = primary_relay_settings_ccc.activate_primary_relay2;
  activate_primary_relay3_ccc = primary_relay_settings_ccc.activate_primary_relay3;
  activate_primary_relay4_ccc = primary_relay_settings_ccc.activate_primary_relay4;
  activate_primary_relay5_ccc = primary_relay_settings_ccc.activate_primary_relay5;
  activate_primary_relay6_ccc = primary_relay_settings_ccc.activate_primary_relay6;
  activate_primary_relay7_ccc = primary_relay_settings_ccc.activate_primary_relay7;
  activate_primary_relay8_ccc = primary_relay_settings_ccc.activate_primary_relay8;

  deactivate_primary_relay1_ccc = primary_relay_settings_ccc.deactivate_primary_relay1;
  deactivate_primary_relay2_ccc = primary_relay_settings_ccc.deactivate_primary_relay2;
  deactivate_primary_relay3_ccc = primary_relay_settings_ccc.deactivate_primary_relay3;
  deactivate_primary_relay4_ccc = primary_relay_settings_ccc.deactivate_primary_relay4;
  deactivate_primary_relay5_ccc = primary_relay_settings_ccc.deactivate_primary_relay5;
  deactivate_primary_relay6_ccc = primary_relay_settings_ccc.deactivate_primary_relay6;
  deactivate_primary_relay7_ccc = primary_relay_settings_ccc.deactivate_primary_relay7;
  deactivate_primary_relay8_ccc = primary_relay_settings_ccc.deactivate_primary_relay8;

  primary_relay_activation_time_ccc = primary_relay_settings_ccc.primary_relay_activation_time;
  primary_relay_delay_time_ccc = primary_relay_settings_ccc.primary_relay_delay_time;
  strcpy (primary_relay_name_ccc, primary_relay_settings_ccc.primary_relay_name);

}





int get_primary_relay_settings_ccc(void)
{
  int a = activate_primary_relay8_ccc;
  a = a << 1;
  a = a + activate_primary_relay7_ccc;
  a = a << 1;
  a = a + activate_primary_relay6_ccc;
  a = a << 1;
  a = a + activate_primary_relay5_ccc;
  a = a << 1;
  a = a + activate_primary_relay4_ccc;
  a = a << 1;
  a = a + activate_primary_relay3_ccc;
  a = a << 1;
  a = a + activate_primary_relay2_ccc;
  a = a << 1;
  a = a + activate_primary_relay1_ccc;
  int d = deactivate_primary_relay8_ccc;
  d = d << 1;
  d = d + deactivate_primary_relay7_ccc;
  d = d << 1;
  d = d + deactivate_primary_relay6_ccc;
  d = d << 1;
  d = d + deactivate_primary_relay5_ccc;
  d = d << 1;
  d = d + deactivate_primary_relay4_ccc;
  d = d << 1;
  d = d + deactivate_primary_relay3_ccc;
  d = d << 1;
  d = d + deactivate_primary_relay2_ccc;
  d = d << 1;
  d = d + deactivate_primary_relay1_ccc;
  return ( (a << 8) + d);
}





void set_primary_relay_settings_ccc(int value)
{
  int a = value >> 8;
  activate_primary_relay1_ccc = a & 1;
  a = a >> 1;
  activate_primary_relay2_ccc = a & 1;
  a = a >> 1;
  activate_primary_relay3_ccc = a & 1;
  a = a >> 1;
  activate_primary_relay4_ccc = a & 1;
  a = a >> 1;
  activate_primary_relay5_ccc = a & 1;
  a = a >> 1;
  activate_primary_relay6_ccc = a & 1;
  a = a >> 1;
  activate_primary_relay7_ccc = a & 1;
  a = a >> 1;
  activate_primary_relay8_ccc = a & 1;
  int d = value & 0xff;
  deactivate_primary_relay1_ccc = d & 1;
  d = d >> 1;
  deactivate_primary_relay2_ccc = d & 1;
  d = d >> 1;
  deactivate_primary_relay3_ccc = d & 1;
  d = d >> 1;
  deactivate_primary_relay4_ccc = d & 1;
  d = d >> 1;
  deactivate_primary_relay5_ccc = d & 1;
  d = d >> 1;
  deactivate_primary_relay6_ccc = d & 1;
  d = d >> 1;
  deactivate_primary_relay7_ccc = d & 1;
  d = d >> 1;
  deactivate_primary_relay8_ccc = d & 1;
}





int activate_primary_relay_ccc(void)
{
  if (usbr1_vendor_id_ccc == CHEAP_RED_RELAY_VENDOR_ID && usbr1_product_id_ccc == CHEAP_RED_RELAY_PRODUCT_ID)
  {
    int requesttype = CHEAP_RED_RELAY_USB_DIR_OUT | CHEAP_RED_RELAY_USB_TYPE_CLASS | CHEAP_RED_RELAY_USB_RECIP_DEVICE;
    int request = CHEAP_RED_RELAY_USBRQ_HID_SET_REPORT;
    int value = CHEAP_RED_RELAY_USB_HID_REPORT_TYPE_FEATURE << 8;
    int index = 0;
    ccc_main_usbbuffer_size_ccc = 8;
    if ( set_main_usb_buffer_ccc() )
    {
      fprintf (stdout, "Error setting usb buffer\n");
      return -1;
    }

    unsigned char tempbuffer[8];
    tempbuffer[2] = 0;
    tempbuffer[3] = 0;
    tempbuffer[4] = 0;
    tempbuffer[5] = 0;
    tempbuffer[6] = 0;
    tempbuffer[7] = 0;

    // relay 1
    if (activate_primary_relay1_ccc)
    {
      tempbuffer[0] = 0xff;
    }
    else
    {
      tempbuffer[0] = 0xfd;
    }
    tempbuffer[1] = 1;
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 8);
    if ( do_send_usbr_control_msg_ccc(requesttype, request, value, index) )
    {
      fprintf (stdout, "Error sending usb control message\n");
      return -1;
    }
    // relay 2
    if (activate_primary_relay2_ccc)
    {
      tempbuffer[0] = 0xff;
    }
    else
    {
      tempbuffer[0] = 0xfd;
    }
    tempbuffer[1] = 2;
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 8);
    if ( do_send_usbr_control_msg_ccc(requesttype, request, value, index) )
    {
      fprintf (stdout, "Error sending usb control message\n");
      return -1;
    }
    // relay 3
    if (activate_primary_relay3_ccc)
    {
      tempbuffer[0] = 0xff;
    }
    else
    {
      tempbuffer[0] = 0xfd;
    }
    tempbuffer[1] = 3;
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 8);
    if ( do_send_usbr_control_msg_ccc(requesttype, request, value, index) )
    {
      fprintf (stdout, "Error sending usb control message\n");
      return -1;
    }
    // relay 4
    if (activate_primary_relay4_ccc)
    {
      tempbuffer[0] = 0xff;
    }
    else
    {
      tempbuffer[0] = 0xfd;
    }
    tempbuffer[1] = 4;
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 8);
    if ( do_send_usbr_control_msg_ccc(requesttype, request, value, index) )
    {
      fprintf (stdout, "Error sending usb control message\n");
      return -1;
    }
    // relay 5
    if (activate_primary_relay5_ccc)
    {
      tempbuffer[0] = 0xff;
    }
    else
    {
      tempbuffer[0] = 0xfd;
    }
    tempbuffer[1] = 5;
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 8);
    if ( do_send_usbr_control_msg_ccc(requesttype, request, value, index) )
    {
      fprintf (stdout, "Error sending usb control message\n");
      return -1;
    }
    // relay 6
    if (activate_primary_relay6_ccc)
    {
      tempbuffer[0] = 0xff;
    }
    else
    {
      tempbuffer[0] = 0xfd;
    }
    tempbuffer[1] = 6;
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 8);
    if ( do_send_usbr_control_msg_ccc(requesttype, request, value, index) )
    {
      fprintf (stdout, "Error sending usb control message\n");
      return -1;
    }
    // relay 7
    if (activate_primary_relay7_ccc)
    {
      tempbuffer[0] = 0xff;
    }
    else
    {
      tempbuffer[0] = 0xfd;
    }
    tempbuffer[1] = 7;
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 8);
    if ( do_send_usbr_control_msg_ccc(requesttype, request, value, index) )
    {
      fprintf (stdout, "Error sending usb control message\n");
      return -1;
    }
    // relay 8
    if (activate_primary_relay8_ccc)
    {
      tempbuffer[0] = 0xff;
    }
    else
    {
      tempbuffer[0] = 0xfd;
    }
    tempbuffer[1] = 8;
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 8);
    if ( do_send_usbr_control_msg_ccc(requesttype, request, value, index) )
    {
      fprintf (stdout, "Error sending usb control message\n");
      return -1;
    }
  }
  else if (usbr1_vendor_id_ccc == YEPKIT_RELAY_VENDOR_ID && (usbr1_product_id_ccc == YEPKIT_YKUSH3_PRODUCT_ID || usbr1_product_id_ccc == YEPKIT_YKUSH_PRODUCT_ID || usbr1_product_id_ccc == YEPKIT_YKUSHLEGACY_PRODUCT_ID || usbr1_product_id_ccc == YEPKIT_YKUSHXS_PRODUCT_ID))
  {
    ccc_main_usbbuffer_size_ccc = 64;
    if (usbr1_product_id_ccc == YEPKIT_YKUSHLEGACY_PRODUCT_ID)
    {
      ccc_main_usbbuffer_size_ccc = 6;
    }
    if ( set_main_usb_buffer_ccc() )
    {
      fprintf (stdout, "Error setting usb buffer\n");
      return -1;
    }
    memset (ccc_usbbuffer_ccc, 0, ccc_main_usbbuffer_size_ccc);

    int ports = 3;
    if (usbr1_product_id_ccc == YEPKIT_YKUSHXS_PRODUCT_ID)
    {
      ports = 1;
    }

    unsigned char tempbuffer;

    // relay 1
    if (activate_primary_relay1_ccc)
    {
      tempbuffer = 0x01;
    }
    else
    {
      tempbuffer = 0x11;
    }
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 1);
    if ( do_send_usbr_interrupt_write_ccc(0x01) )
    {
      fprintf (stdout, "Error sending usb interrupt write\n");
      return -1;
    }
    if ( do_send_usbr_interrupt_read_ccc(0x81) )
    {
      fprintf (stdout, "Error sending usb interrupt write\n");
      return -1;
    }

    if (ports > 1)
    {
      // relay 2
      if (activate_primary_relay2_ccc)
      {
        tempbuffer = 0x02;
      }
      else
      {
        tempbuffer = 0x12;
      }
      memcpy (ccc_usbbuffer_ccc, &tempbuffer, 1);
      if ( do_send_usbr_interrupt_write_ccc(0x01) )
      {
        fprintf (stdout, "Error sending usb interrupt write\n");
        return -1;
      }
      if ( do_send_usbr_interrupt_read_ccc(0x81) )
      {
        fprintf (stdout, "Error sending usb interrupt write\n");
        return -1;
      }
    }

    if (ports > 2)
    {
      // relay 3
      if (activate_primary_relay3_ccc)
      {
        tempbuffer = 0x03;
      }
      else
      {
        tempbuffer = 0x13;
      }
      memcpy (ccc_usbbuffer_ccc, &tempbuffer, 1);
      if ( do_send_usbr_interrupt_write_ccc(0x01) )
      {
        fprintf (stdout, "Error sending usb interrupt write\n");
        return -1;
      }
      if ( do_send_usbr_interrupt_read_ccc(0x81) )
      {
        fprintf (stdout, "Error sending usb interrupt write\n");
        return -1;
      }
    }
  }
  else
  {
    fprintf (stdout, "Error: USB relay ID not recognized\n");
    return -1;
  }
  return 0;
}





int deactivate_primary_relay_ccc(void)
{
  if (usbr1_vendor_id_ccc == CHEAP_RED_RELAY_VENDOR_ID && usbr1_product_id_ccc == CHEAP_RED_RELAY_PRODUCT_ID)
  {
    int requesttype = CHEAP_RED_RELAY_USB_DIR_OUT | CHEAP_RED_RELAY_USB_TYPE_CLASS | CHEAP_RED_RELAY_USB_RECIP_DEVICE;
    int request = CHEAP_RED_RELAY_USBRQ_HID_SET_REPORT;
    int value = CHEAP_RED_RELAY_USB_HID_REPORT_TYPE_FEATURE << 8;
    int index = 0;
    ccc_main_usbbuffer_size_ccc = 8;
    if ( set_main_usb_buffer_ccc() )
    {
      fprintf (stdout, "Error setting usb buffer\n");
      return -1;
    }

    unsigned char tempbuffer[8];
    tempbuffer[2] = 0;
    tempbuffer[3] = 0;
    tempbuffer[4] = 0;
    tempbuffer[5] = 0;
    tempbuffer[6] = 0;
    tempbuffer[7] = 0;

    // relay 1
    if (deactivate_primary_relay1_ccc)
    {
      tempbuffer[0] = 0xff;
    }
    else
    {
      tempbuffer[0] = 0xfd;
    }
    tempbuffer[1] = 1;
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 8);
    if ( do_send_usbr_control_msg_ccc(requesttype, request, value, index) )
    {
      fprintf (stdout, "Error sending usb control message\n");
      return -1;
    }
    // relay 2
    if (deactivate_primary_relay2_ccc)
    {
      tempbuffer[0] = 0xff;
    }
    else
    {
      tempbuffer[0] = 0xfd;
    }
    tempbuffer[1] = 2;
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 8);
    if ( do_send_usbr_control_msg_ccc(requesttype, request, value, index) )
    {
      fprintf (stdout, "Error sending usb control message\n");
      return -1;
    }
    // relay 3
    if (deactivate_primary_relay3_ccc)
    {
      tempbuffer[0] = 0xff;
    }
    else
    {
      tempbuffer[0] = 0xfd;
    }
    tempbuffer[1] = 3;
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 8);
    if ( do_send_usbr_control_msg_ccc(requesttype, request, value, index) )
    {
      fprintf (stdout, "Error sending usb control message\n");
      return -1;
    }
    // relay 4
    if (deactivate_primary_relay4_ccc)
    {
      tempbuffer[0] = 0xff;
    }
    else
    {
      tempbuffer[0] = 0xfd;
    }
    tempbuffer[1] = 4;
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 8);
    if ( do_send_usbr_control_msg_ccc(requesttype, request, value, index) )
    {
      fprintf (stdout, "Error sending usb control message\n");
      return -1;
    }
    // relay 5
    if (deactivate_primary_relay5_ccc)
    {
      tempbuffer[0] = 0xff;
    }
    else
    {
      tempbuffer[0] = 0xfd;
    }
    tempbuffer[1] = 5;
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 8);
    if ( do_send_usbr_control_msg_ccc(requesttype, request, value, index) )
    {
      fprintf (stdout, "Error sending usb control message\n");
      return -1;
    }
    // relay 6
    if (deactivate_primary_relay6_ccc)
    {
      tempbuffer[0] = 0xff;
    }
    else
    {
      tempbuffer[0] = 0xfd;
    }
    tempbuffer[1] = 6;
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 8);
    if ( do_send_usbr_control_msg_ccc(requesttype, request, value, index) )
    {
      fprintf (stdout, "Error sending usb control message\n");
      return -1;
    }
    // relay 7
    if (deactivate_primary_relay7_ccc)
    {
      tempbuffer[0] = 0xff;
    }
    else
    {
      tempbuffer[0] = 0xfd;
    }
    tempbuffer[1] = 7;
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 8);
    if ( do_send_usbr_control_msg_ccc(requesttype, request, value, index) )
    {
      fprintf (stdout, "Error sending usb control message\n");
      return -1;
    }
    // relay 8
    if (deactivate_primary_relay8_ccc)
    {
      tempbuffer[0] = 0xff;
    }
    else
    {
      tempbuffer[0] = 0xfd;
    }
    tempbuffer[1] = 8;
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 8);
    if ( do_send_usbr_control_msg_ccc(requesttype, request, value, index) )
    {
      fprintf (stdout, "Error sending usb control message\n");
      return -1;
    }
  }
  else if (usbr1_vendor_id_ccc == YEPKIT_RELAY_VENDOR_ID && (usbr1_product_id_ccc == YEPKIT_YKUSH3_PRODUCT_ID || usbr1_product_id_ccc == YEPKIT_YKUSH_PRODUCT_ID || usbr1_product_id_ccc == YEPKIT_YKUSHLEGACY_PRODUCT_ID || usbr1_product_id_ccc == YEPKIT_YKUSHXS_PRODUCT_ID))
  {
    ccc_main_usbbuffer_size_ccc = 64;
    if (usbr1_product_id_ccc == YEPKIT_YKUSHLEGACY_PRODUCT_ID)
    {
      ccc_main_usbbuffer_size_ccc = 6;
    }
    if ( set_main_usb_buffer_ccc() )
    {
      fprintf (stdout, "Error setting usb buffer\n");
      return -1;
    }
    memset (ccc_usbbuffer_ccc, 0, ccc_main_usbbuffer_size_ccc);

    int ports = 3;
    if (usbr1_product_id_ccc == YEPKIT_YKUSHXS_PRODUCT_ID)
    {
      ports = 1;
    }

    unsigned char tempbuffer;

    // relay 1
    if (deactivate_primary_relay1_ccc)
    {
      tempbuffer = 0x01;
    }
    else
    {
      tempbuffer = 0x11;
    }
    memcpy (ccc_usbbuffer_ccc, &tempbuffer, 1);
    if ( do_send_usbr_interrupt_write_ccc(0x01) )
    {
      fprintf (stdout, "Error sending usb interrupt write\n");
      return -1;
    }
    if ( do_send_usbr_interrupt_read_ccc(0x81) )
    {
      fprintf (stdout, "Error sending usb interrupt write\n");
      return -1;
    }

    if (ports > 1)
    {
      // relay 2
      if (deactivate_primary_relay2_ccc)
      {
        tempbuffer = 0x02;
      }
      else
      {
        tempbuffer = 0x12;
      }
      memcpy (ccc_usbbuffer_ccc, &tempbuffer, 1);
      if ( do_send_usbr_interrupt_write_ccc(0x01) )
      {
        fprintf (stdout, "Error sending usb interrupt write\n");
        return -1;
      }
      if ( do_send_usbr_interrupt_read_ccc(0x81) )
      {
        fprintf (stdout, "Error sending usb interrupt write\n");
        return -1;
      }
    }

    if (ports > 2)
    {
      // relay 3
      if (deactivate_primary_relay3_ccc)
      {
        tempbuffer = 0x03;
      }
      else
      {
        tempbuffer = 0x13;
      }
      memcpy (ccc_usbbuffer_ccc, &tempbuffer, 1);
      if ( do_send_usbr_interrupt_write_ccc(0x01) )
      {
        fprintf (stdout, "Error sending usb interrupt write\n");
        return -1;
      }
      if ( do_send_usbr_interrupt_read_ccc(0x81) )
      {
        fprintf (stdout, "Error sending usb interrupt write\n");
        return -1;
      }
    }
  }
  else
  {
    fprintf (stdout, "Error: USB relay ID not recognized\n");
    return -1;
  }
  return 0;
}















