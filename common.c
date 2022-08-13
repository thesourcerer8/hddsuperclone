// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


#include "common.h"



int message_exit_ccc(char *message)
{
  if (debug_ccc > 0)
  {
    fprintf (debug_file_ccc, "%s", message);
  }
  strncat(exit_message_ccc, message, EXIT_MESSAGE_SIZE - strlen(exit_message_ccc) - 1);
  return 0;
}




int message_error_ccc(char *message)
{
  if (debug_ccc > 0)
  {
    fprintf (debug_file_ccc, "%s", message);
  }
  strncat(error_message_ccc, message, ERROR_MESSAGE_SIZE - strlen(error_message_ccc) - 1);
  return 0;
}




int clear_error_message_ccc(void)
{
  strcpy(error_message_ccc, "\0");
  return 0;
}




int message_display_ccc(char *message)
{
  if (debug_ccc & DEBUG18)
  {
    fprintf (debug_file_ccc, "%s", message);
  }
  strncat(display_message_ccc, message, DISPLAY_MESSAGE_SIZE - strlen(display_message_ccc) - 1);
  if (strlen(message) + strlen(display_message_ccc) < DISPLAY_MESSAGE_SIZE -1)
  {
    //strcat(display_message_ccc, message);
  }
  return 0;
}




int message_now_ccc(char *message)
{
  fprintf (stdout, "%s", message);
  if (debug_ccc)
  {
    get_the_time_ccc();
    fprintf (debug_file_ccc, "%s %s", current_time_ccc, message);
  }
  return 0;
}



int message_console_log_ccc(char *message, unsigned long long level)
{
  get_the_time_ccc();
  if (level == 0 || (level & verbose_ccc))
  {
    fprintf (stdout, "%s %s", current_time_ccc, message);
    if (debug_ccc)
    {
      fprintf (debug_file_ccc, "%s %s", current_time_ccc, message);
    }
    if (enable_data_dump_ccc || enable_dump_identify_on_check_ccc)
    {
      char temp_string[1024];
      sprintf (temp_string, "%s %s", current_time_ccc, message);
      dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
    }
  }
  return 0;
}




int message_debug_ccc(char *message, unsigned long long level)
{
  get_the_time_ccc();
  if (debug_ccc & level)
  {
    fprintf (debug_file_ccc, "%s %s", current_time_ccc, message);
  }
  if (verbose_ccc & level)
  {
    fprintf (stdout, "%s %s", current_time_ccc, message);
  }
  return 0;
}




// function to set main buffer
int set_main_buffer_ccc(void)
{
  if (usb_mode_ccc)
  {
    if (ccc_main_buffer_size_ccc > MAX_USB_BUFFER_SIZE || ccc_main_buffer_size_ccc > real_buffer_size_ccc)
    {
      sprintf (tempmessage_ccc, "ERROR: Maximum buffer size exceeded.\n");
      message_now_ccc(tempmessage_ccc);
      return (-1);
    }
  }
  else if (ccc_main_buffer_size_ccc > MAX_BUFFER_SIZE || ccc_main_buffer_size_ccc > real_buffer_size_ccc)
  {
    sprintf (tempmessage_ccc, "ERROR: Maximum buffer size exceeded.\n");
    message_now_ccc(tempmessage_ccc);
    return (-1);
  }
  if (superbyte_ccc[29] == 0x60)
  {
    if (ahci_mode_ccc && ccc_main_buffer_size_ccc > max_dma_size_ccc)
    {
      sprintf (tempmessage_ccc, "ERROR: Maximum AHCI buffer size (%lld) exceeded.\n", max_dma_size_ccc);
      message_now_ccc(tempmessage_ccc);
      return (-1);
    }
    if (direct_mode_ccc)
    {
      create_dma_table_ccc();
    }
  }
  return (0);
}





int create_dma_table_ccc(void)
{
  // ahci mode
  if (ahci_mode_ccc)
  {
    int max_entries = max_dma_size_ccc / pagesize_ccc;
    memset (table_buffer_ccc, 0, table_size_ccc);
    unsigned int write_size;
    int i;
    int page_count = (ccc_main_buffer_size_ccc / pagesize_ccc);
    if (ccc_main_buffer_size_ccc % pagesize_ccc > 0)
    {
      page_count++;
    }
    //fprintf (stdout, "page_count2=%d\n", page_count);
    table_entry_count_ccc = page_count;
    for (i = 0; i < page_count; i++)
    {
      if (i >= max_entries)
      {
        table_entry_count_ccc = max_entries;
        break;
      }
      int n = (i*16) + superbyte_ccc[15];    //potential superbyte
      uint32_t dword = buffer_physical_address_ccc[i];
      memcpy(table_buffer_ccc+n, &dword, 4);
      memset(table_buffer_ccc+n+4, 0, 8);

      // if last page get remainder of buffer size
      if (i == page_count - 1)
      {
        if (ccc_main_buffer_size_ccc % pagesize_ccc == 0)
        {
          write_size = pagesize_ccc;
        }
        else
        {
          write_size = ccc_main_buffer_size_ccc % pagesize_ccc;
        }
      }
      else
      {
        write_size = pagesize_ccc;
      }
      //fprintf (stdout, "write_size=%d\n", write_size);
      dword = (write_size - 1);
      memcpy(table_buffer_ccc+12+n, &dword, 4);
      #ifdef DEBUG
      if (debug_ccc & DEBUG33)
      {
        int i;
        for (i = 0; i < 16; i++)
        {
          unsigned char *c;
          c = (unsigned char *)table_buffer_ccc+i+n;
          fprintf (stdout, "tb%02x ", *c);
        }
        fprintf (stdout, "\n");
      }
      #endif
    }
  }


  //direct mode
  else
  {
    int max_entries = max_dma_size_ccc / pagesize_ccc;
    memset (table_buffer_ccc, 0, table_size_ccc);
    unsigned int write_size;
    unsigned char c;
    int i;
    int page_count = (ccc_main_buffer_size_ccc / pagesize_ccc);
    if (ccc_main_buffer_size_ccc % pagesize_ccc > 0)
    {
      page_count++;
    }
    //fprintf (stdout, "page_count2=%d\n", page_count);
    for (i = 0; i < page_count; i++)
    {
      if (i >= max_entries)
      {
        break;
      }
      c = buffer_physical_address_ccc[i];
      memcpy(table_buffer_ccc+(i*8), &c, 1);
      c = buffer_physical_address_ccc[i] >> 8;
      memcpy(table_buffer_ccc+1+(i*8), &c, 1);
      c = buffer_physical_address_ccc[i] >> 16;
      memcpy(table_buffer_ccc+2+(i*8), &c, 1);
      c = buffer_physical_address_ccc[i] >> 24;
      memcpy(table_buffer_ccc+3+(i*8), &c, 1);

      // if last page get remainder of buffer size
      if (i == page_count - 1)
      {
        if (ccc_main_buffer_size_ccc % pagesize_ccc == 0)
        {
          write_size = pagesize_ccc;
        }
        else
        {
          write_size = ccc_main_buffer_size_ccc % pagesize_ccc;
        }
      }
      else
      {
        write_size = pagesize_ccc;
      }
      //fprintf (stdout, "write_size=%d\n", write_size);
      c = write_size;
      memcpy(table_buffer_ccc+4+(i*8), &c, 1);
      c = write_size >> 8;
      memcpy(table_buffer_ccc+5+(i*8), &c, 1);

      c = 0;
      memcpy(table_buffer_ccc+6+(i*8), &c, 1);
      // if last page then mark it
      if (i == page_count - 1)
      {
        c = superbyte_ccc[15];    //potential superbyte
      }
      else
      {
        c = superbyte_ccc[11];    //potential superbyte
      }
      memcpy(table_buffer_ccc+7+(i*8), &c, 1);
      #ifdef DEBUG
      if (debug_ccc & DEBUG33)
      {
        int i;
        for (i = 0; i < 8; i++)
        {
          unsigned char *c;
          c = (unsigned char *)table_buffer_ccc+i;
          fprintf (stdout, "tb%02x ", *c);
        }
        fprintf (stdout, "\n");
      }
      #endif
    }
  }
  return (0);
}





// function to initialize table buffer
int set_table_buffer_ccc(void)
{
  int multiplier = 1;
  if (driver_installed_ccc && driver_memory_mapped_ccc)
  {
    multiplier = 4;
  }
  table_size_ccc = pagesize_ccc * multiplier;
  long long tries = 0;
  long long maxtries = 0x10000000 / table_size_ccc;
  //fprintf (stdout, "maxtries=%lld\n", maxtries);   //debug
  while (tries < maxtries)
  {
    if (!driver_memory_mapped_ccc)
    {
      free (table_buffer_ccc);
    }
    unsigned int align = pagesize_ccc;
    if (tries != 0)
    {
      padding_buffer_ccc = realloc(padding_buffer_ccc, table_size_ccc*tries);
      //memset (padding_buffer_ccc, 0, table_size_ccc*tries);   //debug
    }
    if (driver_memory_mapped_ccc)
    {
      table_buffer_ccc = driver_table_buffer_ccc;
    }
    else if (posix_memalign(&table_buffer_ccc, align, table_size_ccc))
    {
      sprintf (tempmessage_ccc, "posix_memalign failed (%s)", strerror(errno));
      message_now_ccc(tempmessage_ccc);
      return (-1);
    }
    memset (table_buffer_ccc, 0, table_size_ccc);
    #if UINTPTR_MAX == 0xffffffff
    table_virtual_address_ccc = (unsigned int)table_buffer_ccc;
    #elif UINTPTR_MAX == 0xffffffffffffffff
    table_virtual_address_ccc = (unsigned long long)table_buffer_ccc;
    #else
    table_virtual_address_ccc = (unsigned int)table_buffer_ccc;
    #endif
    //fprintf (stdout, "table address=0x%llx\n", table_virtual_address_ccc);   //debug

    return_value_ccc = get_table_physical_memory_location_ccc();
    if (return_value_ccc == 0)
    {
      break;
    }
    if (driver_memory_mapped_ccc)
    {
      return -1;
    }

    tries++;
  }
  //fprintf (stdout, "tries=%lld\n", tries);   //debug
  return (return_value_ccc);
}





int get_table_physical_memory_location_ccc(void)
{
  int out_of_range = 0;
  // get table buffer location
  int multiplier = 1;
  if (driver_installed_ccc && driver_memory_mapped_ccc)
  {
    multiplier = 4;
  }
  table_size_ccc = pagesize_ccc * multiplier;
  unsigned long long file_offset = table_virtual_address_ccc / pagesize_ccc * 8;

  int proc_id = getpid();
  //char command[255];
  //sprintf (command, "cat /proc/%d/maps", proc_id);
  //fprintf (stdout, "%s\n", command);
  //system (command);

  char path[255];
  sprintf (path, "/proc/%d/pagemap", proc_id);
  pagemap_ccc = fopen(path, "rb");
  if(!pagemap_ccc)
  {
    sprintf(tempmessage_ccc, "Error! Cannot open %s\n", path);
    message_now_ccc(tempmessage_ccc);
    return -1;
  }

  //printf("Vaddr: 0x%llx, pagesize_ccc: %d\n", table_virtual_address_ccc, pagesize_ccc);
  //printf("Reading %s at 0x%llx\n", path, file_offset);
  int status = fseek(pagemap_ccc, file_offset, SEEK_SET);
  if(status){
    sprintf (tempmessage_ccc, "Failed to do fseek! (%s)", strerror(errno));
    message_now_ccc(tempmessage_ccc);
    return -1;
  }

  int count = (table_size_ccc / pagesize_ccc);
  if (table_size_ccc % pagesize_ccc > 0)
  {
    count++;
  }
  unsigned int read_size = 8 * count;
  //fprintf (stdout, "count=%d\n", count);
  //exit (0);
  unsigned char file_data[read_size];
  if (fread(file_data, 1, read_size, pagemap_ccc) == read_size )
  {
    while (read_size > 0)
    {
      int i;
      for (i = read_size-1; i >= (int)read_size - 8; i--)
      {
        //fprintf (stdout, "%02x", file_data[i]);
        table_physical_address_ccc = (table_physical_address_ccc << 8) + file_data[i];
      }
      //fprintf (stdout, "\n");
      table_physical_address_ccc <<= 12;
#ifdef DEBUG
      if (debug_ccc & DEBUG34)
      {
        fprintf (stdout, "table_physical_address=0x%llx\n", table_physical_address_ccc);
      }
#endif
      if (table_physical_address_ccc > 0xffffffff)
      {
#ifdef DEBUG
        if (debug_ccc & DEBUG34)
        {
          fprintf (stdout, "table_physical_address=0x%llx is out of range\n", table_physical_address_ccc);
        }
#endif
        out_of_range++;
      }
      read_size -= 8;
    }
  }
  fclose(pagemap_ccc);
  return (out_of_range);
}






// function to initialize command list buffer
int set_command_list_buffer_ccc(void)
{
  command_list_size_ccc = pagesize_ccc;
  long long tries = 0;
  long long maxtries = 0x10000000 / command_list_size_ccc;
  //fprintf (stdout, "maxtries=%lld\n", maxtries);
  while (tries < maxtries)
  {
    if (!driver_memory_mapped_ccc)
    {
      free (command_list_buffer_ccc);
    }
    unsigned int align = pagesize_ccc;
    if (tries != 0)
    {
      padding_buffer_ccc = realloc(padding_buffer_ccc, command_list_size_ccc*tries);
      //memset (padding_buffer_ccc, 0, command_list_size_ccc*tries);
    }
    if (driver_memory_mapped_ccc)
    {
      command_list_buffer_ccc = driver_command_list_buffer_ccc;
    }
    else if (posix_memalign(&command_list_buffer_ccc, align, command_list_size_ccc))
    {
      sprintf (tempmessage_ccc, "posix_memalign failed (%s)", strerror(errno));
      message_now_ccc(tempmessage_ccc);
      return (-1);
    }
    memset (command_list_buffer_ccc, 0, command_list_size_ccc);
    #if UINTPTR_MAX == 0xffffffff
    command_list_virtual_address_ccc = (unsigned int)command_list_buffer_ccc;
    #elif UINTPTR_MAX == 0xffffffffffffffff
    command_list_virtual_address_ccc = (unsigned long long)command_list_buffer_ccc;
    #else
    command_list_virtual_address_ccc = (unsigned int)command_list_buffer_ccc;
    #endif
    //fprintf (stdout, "command list address=0x%llx\n", command_list_virtual_address_ccc);

    return_value_ccc = get_command_list_physical_memory_location_ccc();
    if (return_value_ccc == 0)
    {
      break;
    }
    if (driver_memory_mapped_ccc)
    {
      return (-1);
    }

    tries++;
  }
  //fprintf (stdout, "tries=%lld\n", tries);
  return (return_value_ccc);
}






int get_command_list_physical_memory_location_ccc(void)
{
  int out_of_range = 0;
  // get command list buffer location
  command_list_size_ccc = pagesize_ccc;
  unsigned long long file_offset = command_list_virtual_address_ccc / pagesize_ccc * 8;

  int proc_id = getpid();
  //char command[255];
  //sprintf (command, "cat /proc/%d/maps", proc_id);
  //fprintf (stdout, "%s\n", command);
  //system (command);

  char path[255];
  sprintf (path, "/proc/%d/pagemap", proc_id);
  pagemap_ccc = fopen(path, "rb");
  if(!pagemap_ccc)
  {
    sprintf(tempmessage_ccc, "Error! Cannot open %s\n", path);
    message_now_ccc(tempmessage_ccc);
    return -1;
  }

  //printf("Vaddr: 0x%llx, pagesize_ccc: %d\n", command_list_virtual_address_ccc, pagesize_ccc);
  //printf("Reading %s at 0x%llx\n", path, file_offset);
  int status = fseek(pagemap_ccc, file_offset, SEEK_SET);
  if(status){
    sprintf (tempmessage_ccc, "Failed to do fseek! (%s)", strerror(errno));
    message_now_ccc(tempmessage_ccc);
    return -1;
  }

  int count = (command_list_size_ccc / pagesize_ccc);
  if (command_list_size_ccc % pagesize_ccc > 0)
  {
    count++;
  }
  unsigned int read_size = 8 * count;
  //fprintf (stdout, "count=%d\n", count);
  //exit (0);
  unsigned char file_data[read_size];
  if (fread(file_data, 1, read_size, pagemap_ccc) == read_size )
  {
    while (read_size > 0)
    {
      int i;
      for (i = read_size-1; i >= (int)read_size - 8; i--)
      {
        //fprintf (stdout, "%02x", file_data[i]);
        command_list_physical_address_ccc = (command_list_physical_address_ccc << 8) + file_data[i];
      }
      //fprintf (stdout, "\n");
      command_list_physical_address_ccc <<= 12;
#ifdef DEBUG
      if (debug_ccc & DEBUG34)
      {
        fprintf (stdout, "command_list_physical_address=0x%llx\n", command_list_physical_address_ccc);
      }
#endif
      if (command_list_physical_address_ccc > 0xffffffff)
      {
#ifdef DEBUG
        if (debug_ccc & DEBUG34)
        {
          fprintf (stdout, "command_list_physical_address=0x%llx is out of range\n", command_list_physical_address_ccc);
        }
#endif
        out_of_range++;
      }
      read_size -= 8;
    }
  }
  fclose(pagemap_ccc);
  return (out_of_range);
}





// function to initialize fis buffer
int set_fis_buffer_ccc(void)
{
  int fis_size_ccc = pagesize_ccc;
  long long tries = 0;
  long long maxtries = 0x10000000 / fis_size_ccc;
  //fprintf (stdout, "maxtries=%lld\n", maxtries);
  while (tries < maxtries)
  {
    if (!driver_memory_mapped_ccc)
    {
      free (fis_buffer_ccc);
    }
    unsigned int align = pagesize_ccc;
    if (tries != 0)
    {
      padding_buffer_ccc = realloc(padding_buffer_ccc, fis_size_ccc*tries);
      //memset (padding_buffer_ccc, 0, command_list_size_ccc*tries);
    }
    if (driver_memory_mapped_ccc)
    {
      fis_buffer_ccc = driver_fis_buffer_ccc;
    }
    else if (posix_memalign(&fis_buffer_ccc, align, fis_size_ccc))
    {
      sprintf (tempmessage_ccc, "posix_memalign failed (%s)", strerror(errno));
      message_now_ccc(tempmessage_ccc);
      return (-1);
    }
    memset (fis_buffer_ccc, 0, fis_size_ccc);
    #if UINTPTR_MAX == 0xffffffff
    fis_virtual_address_ccc = (unsigned int)fis_buffer_ccc;
    #elif UINTPTR_MAX == 0xffffffffffffffff
    fis_virtual_address_ccc = (unsigned long long)fis_buffer_ccc;
    #else
    fis_virtual_address_ccc = (unsigned int)fis_buffer_ccc;
    #endif
    //fprintf (stdout, "fis address=0x%llx\n", fis_virtual_address_ccc);

    return_value_ccc = get_fis_physical_memory_location_ccc();
    if (return_value_ccc == 0)
    {
      break;
    }
    if (driver_memory_mapped_ccc)
    {
      return -1;
    }

    tries++;
  }
  //fprintf (stdout, "tries=%lld\n", tries);
  return (return_value_ccc);
}





int get_fis_physical_memory_location_ccc(void)
{
  int out_of_range = 0;
  // get FIS buffer location
  fis_size_ccc = pagesize_ccc;
  unsigned long long file_offset = fis_virtual_address_ccc / pagesize_ccc * 8;

  int proc_id = getpid();
  //char command[255];
  //sprintf (command, "cat /proc/%d/maps", proc_id);
  //fprintf (stdout, "%s\n", command);
  //system (command);

  char path[255];
  sprintf (path, "/proc/%d/pagemap", proc_id);
  pagemap_ccc = fopen(path, "rb");
  if(!pagemap_ccc)
  {
    sprintf(tempmessage_ccc, "Error! Cannot open %s\n", path);
    message_now_ccc(tempmessage_ccc);
    return -1;
  }

  //printf("Vaddr: 0x%llx, pagesize_ccc: %d\n", fis_virtual_address_ccc, pagesize_ccc);
  //printf("Reading %s at 0x%llx\n", path, file_offset);
  int status = fseek(pagemap_ccc, file_offset, SEEK_SET);
  if(status){
    sprintf (tempmessage_ccc, "Failed to do fseek! (%s)", strerror(errno));
    message_now_ccc(tempmessage_ccc);
    return -1;
  }

  int count = (fis_size_ccc / pagesize_ccc);
  if (fis_size_ccc % pagesize_ccc > 0)
  {
    count++;
  }
  unsigned int read_size = 8 * count;
  //fprintf (stdout, "count=%d\n", count);
  //exit (0);
  unsigned char file_data[read_size];
  if (fread(file_data, 1, read_size, pagemap_ccc) == read_size )
  {
    while (read_size > 0)
    {
      int i;
      for (i = read_size-1; i >= (int)read_size - 8; i--)
      {
        //fprintf (stdout, "%02x", file_data[i]);
        fis_physical_address_ccc = (fis_physical_address_ccc << 8) + file_data[i];
      }
      //fprintf (stdout, "\n");
      fis_physical_address_ccc <<= 12;
#ifdef DEBUG
      if (debug_ccc & DEBUG34)
      {
        fprintf (stdout, "fis_physical_address=0x%llx\n", fis_physical_address_ccc);
      }
#endif
      if (fis_physical_address_ccc > 0xffffffff)
      {
#ifdef DEBUG
        if (debug_ccc & DEBUG34)
        {
          fprintf (stdout, "fis_physical_address=0x%llx is out of range\n", fis_physical_address_ccc);
        }
#endif
        out_of_range++;
      }
      read_size -= 8;
    }
  }
  fclose(pagemap_ccc);
  return (out_of_range);
}





int get_buffer_physical_memory_locations_ccc(void)
{
  int out_of_range = 0;
  long long tries = 0;
  long long maxtries = 0x100000000 / max_dma_size_ccc;
  //fprintf (stdout, "maxtries=%lld\n", maxtries);    //debug
  while (tries < maxtries)
  {
    unsigned int align = pagesize_ccc;
    if (!driver_memory_mapped_ccc)
    {
      free (ccc_buffer_ccc);
    }
    if (tries != 0)
    {
    padding_buffer_ccc = realloc(padding_buffer_ccc, real_buffer_size_ccc * tries);
    }

    if (driver_memory_mapped_ccc)
    {
      ccc_buffer_ccc = driver_main_data_buffer_address_ccc;
    }
    else if (posix_memalign(&ccc_buffer_ccc, align, real_buffer_size_ccc))
    {
      sprintf (tempmessage_ccc, "posix_memalign failed (%s)", strerror(errno));
      message_now_ccc(tempmessage_ccc);
      return (-1);
    }
    memset (ccc_buffer_ccc, 0, real_buffer_size_ccc);

    out_of_range = 0;
    #if UINTPTR_MAX == 0xffffffff
    buffer_virtual_address_ccc = (unsigned int)ccc_buffer_ccc;
    #elif UINTPTR_MAX == 0xffffffffffffffff
    buffer_virtual_address_ccc = (unsigned long long)ccc_buffer_ccc;
    #else
    buffer_virtual_address_ccc = (unsigned int)ccc_buffer_ccc;
    #endif
    //fprintf (stdout, "buffer address=0x%llx\n", buffer_virtual_address_ccc);    //debug

    int proc_id = getpid();
    //char command[255];
    //sprintf (command, "cat /proc/%d/maps", proc_id);
    //fprintf (stdout, "%s\n", command);
    //system (command);

    char path[255];
    sprintf (path, "/proc/%d/pagemap", proc_id);
    pagemap_ccc = fopen(path, "rb");
    if(!pagemap_ccc)
    {
      sprintf(tempmessage_ccc, "Error! Cannot open %s\n", path);
      message_now_ccc(tempmessage_ccc);
      return -1;
    }

    // get main buffer location
    unsigned long long file_offset = buffer_virtual_address_ccc / pagesize_ccc * 8;
    //printf("Vaddr: 0x%llx, pagesize_ccc: %d\n", buffer_virtual_address_ccc, pagesize_ccc);    //debug
    //printf("Reading %s at 0x%llx\n", path, file_offset);    //debug
    int status = fseek(pagemap_ccc, file_offset, SEEK_SET);
    if(status){
      sprintf (tempmessage_ccc, "Failed to do fseek! (%s)", strerror(errno));
      message_now_ccc(tempmessage_ccc);
      return -1;
    }

    int count = (real_buffer_size_ccc / pagesize_ccc);
    //int count = (max_dma_size_ccc / pagesize_ccc);
    if (real_buffer_size_ccc % pagesize_ccc > 0)
    {
      count++;
    }
    unsigned int read_size = 8 * count;
    //fprintf (stdout, "total_buffer_page_count=%d\n", count);    //debug
    //exit (0);
    count--;
    unsigned char file_data[read_size];
    if (fread(file_data, 1, read_size, pagemap_ccc) == read_size )
    {
      while (read_size > 0)
      {
        buffer_physical_address_ccc[count] = 0;
        int i;
        for (i = read_size-1; i >= (int)read_size - 8; i--)
        {
          //fprintf (stdout, "%02x", file_data[i]);    //debug
          buffer_physical_address_ccc[count] = (buffer_physical_address_ccc[count] << 8) + file_data[i];
        }
        //fprintf (stdout, "\n");
        buffer_physical_address_ccc[count] <<= 12;
#ifdef DEBUG
        if ((debug_ccc & DEBUG34) && count < (int)(max_dma_size_ccc / pagesize_ccc) )
        {
          fprintf (stdout, "buffer_physical_address[%d]=0x%llx\n", count, buffer_physical_address_ccc[count]);
        }
#endif
        if (buffer_physical_address_ccc[count] > 0xffffffff && count < (int)(max_dma_size_ccc / pagesize_ccc) )
        {
#ifdef DEBUG
          if (debug_ccc & DEBUG34)
          {
            fprintf (stdout, "buffer_physical_address[%d]=0x%llx is out of range\n", count, buffer_physical_address_ccc[count]);
          }
#endif
          out_of_range++;
        }
        read_size -= 8;
        count--;
      }
    }
    fclose(pagemap_ccc);
    if (out_of_range == 0)
    {
      break;
    }
    if (driver_memory_mapped_ccc)
    {
      return -1;
    }
    tries++;
  }
  //fprintf (stdout, "tries=%lld\n", tries);    //debug
  free (padding_buffer_ccc);
  return (out_of_range);
}





void dump_data_to_filename_binary_ccc(char *filename, unsigned char *data, int size, char *description)
{
  FILE *file = fopen(filename, "a");
  time_t mytime;
  mytime = time(NULL);
  fprintf (file, "******dump data ****** %d %s %s", size, description, ctime(&mytime));
  if (size % 4)
  {
    fprintf (file, "warning, size of data not evenly dividable by 4\n");
  }
  int i;
  uint32_t doubleword;
  for (i = 0; i < size; i+=4)
  {
    memcpy(&doubleword, data + i, 4);
    fprintf (file, "%x %08x ", i, doubleword);
    int n;
    for (n = 0; n < 32; n++)
    {
      if ( !(n % 8) )
      {
        fprintf (file, " ");
      }
      if ((doubleword<<n) & 0x80000000)
        fprintf (file, "1");
      else
        fprintf (file, "0");
    }
    fprintf (file, "\n");
  }
  int fp = fileno(file);
  fsync(fp);
  fclose(file);
}





void dump_data_to_filename_ccc(char *filename, unsigned char *data, int size, char *description)
{
  FILE *file = fopen(filename, "a");
  time_t mytime;
  mytime = time(NULL);
  fprintf (file, "****** dump data ****** %d %s %s", size, description, ctime(&mytime));
  if (size % 16)
  {
    fprintf (file, "warning, size of data not evenly dividable by 16\n");
  }
  int i;
  for (i = 0; i < size; i+=16)
  {
    fprintf (file, "%x: ", i);
    unsigned char *c;
    int n;
    for (n=0; n < 16 && i+n < size; n++)
    {
      c = (unsigned char *)data+i+n;
      fprintf (file, "%02x ", *c);
    }
    fprintf (file, "   ");
    for (n=0; n < 16 && i+n < size; n++)
    {
      c = (unsigned char *)data+i+n;
      fprintf (file, "%c", isprint(*c) ? *c : '.');
    }
    fprintf (file, "\n");
  }
  int fp = fileno(file);
  fsync(fp);
  fclose(file);
}





void dump_info_to_filename_ccc(char *filename, char *info)
{
  FILE *file = fopen(filename, "a");
  time_t mytime;
  mytime = time(NULL);
  fprintf (file, "****** dump info ****** %s", ctime(&mytime));
  fprintf (file, "%s", info);
  int fp = fileno(file);
  fsync(fp);
  fclose(file);
}











