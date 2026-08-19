// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include "clone_gui_common.h"
#include "clone_gui_common.c"
#include "clone_gui2.h"
#include "common.h"
#include "hddsuperclone2_glade.h"

#include "strncpy_wrapper.h"
#include "util.h"


int start_gtk_ccc(int argc, char **argv, char *title, char *version)
{
  // setup initial language
  setup_enlanguage_ccc();
  copy_enlanguage_ccc();

  // initialize memory so it is ready
  return_value_ccc = initialize_logfile_memory_ccc();
  if (return_value_ccc != 0)
  {
    exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
    return -1;
  }
  return_value_ccc = initialize_domainfile_memory_ccc();
  if (return_value_ccc != 0)
  {
    exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
    return -1;
  }
  return_value_ccc = initialize_memory_ccc();
  if (return_value_ccc != 0)
  {
    exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
    return -1;
  }

  char window_title[256];
  strlcpy (window_title, title, sizeof(window_title));
  strcat (window_title, " ");
  strcat (window_title, version);

  strlcpy (program_title, "_", sizeof(program_title));
  strcat (program_title, title);
  strcat (program_title, "_");
  strcat (program_title, version);
  strcat (program_title, "_");

  gtk_init(&argc, &argv);

  if (translate_ccc)
  {
    translate_all_ccc();
    exit(0);
  }

  set_language_ccc();

  if (enable_data_dump_ccc || enable_dump_identify_on_check_ccc)
  {
    set_data_dump_filename_ccc();
    char temp_string[256];
    dump_info_to_filename_ccc (data_dump_filename_ccc, "*****************************************************************\n");
    snprintf (temp_string, sizeof(temp_string), "********** %s %s data dump file **********\n", title_ccc, version_number_ccc);
    dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
  }

  GtkBuilder *builder = gtk_builder_new();
  GError *err = NULL; // It is mandatory to initialize to NULL
  if(0 == gtk_builder_add_from_string (builder, (const gchar *)hddsuperclone2_glade, hddsuperclone2_glade_len, &err))
  {
    fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
  }

  main_window_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "window_main"));
  if (NULL == main_window_ccc)
  {
    fprintf(stderr, "Object error: window_main \n");
  }

  bottom_status_bar_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "bottom_status_bar"));

  gtk_builder_connect_signals(builder, NULL);

  gtk_window_set_title(GTK_WINDOW(main_window_ccc), window_title);

  topmenubar_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "topmenubar"));
  filemi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "filemi"));
  quitmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "quitmi"));
  openmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "openmi"));
  newmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "newmi"));
  sourcemi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "sourcemi"));
  destinationmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "destinationmi"));
  drivesmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "drivesmi"));
  blockmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "blockmi"));
  drivemi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "drivemi"));
  imagemi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "imagemi"));
  devnullmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "devnullmi"));
  modemi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "modemi"));
  copyimagemi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "copyimagemi"));
  genericmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "genericmi"));
  pass_automi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "pass_automi"));
  pass_scsimi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "pass_scsimi"));
  pass_atrami_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "pass_atami"));
  directmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "directmi"));
  ahcimi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "ahcimi"));
  usbmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "usbmi"));
  usbatami_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "usbatami"));
  settingsmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "settingsmi"));
  advancedmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "advancedmi"));
  clonemi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "clonemi"));
  savemi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "savemi"));
  saveasmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "saveasmi"));
  resetstatusmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "resetstatusmi"));
  toolsmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "toolsmi"));
  resetlogmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "resetlogmi"));
  repairlogmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "repairlogmi"));
  fillzeromi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "fillzeromi"));
  fillmarkmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "fillmarkmi"));
  fillerasemi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "fillerasemi"));
  domainmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "domainmi"));
  importddmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "importddmi"));
  exportddmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "exportddmi"));
  label_project_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_project"));
  data_project_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_project"));
  label_domain_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_domain"));
  data_domain_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_domain"));
  label_source_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_source"));
  data_source_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_source"));
  label_destination_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_destination"));
  data_destination_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_destination"));
  label_totallba_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_totallba"));
  data_totallba_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_totallba"));
  label_lbatoread_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_lbatoread"));
  data_lbatoread_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_lbatoread"));
  label_domainsize_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_domainsize"));
  data_domainsize_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_domainsize"));
  label_runtime_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_runtime"));
  data_runtime_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_runtime"));
  label_remainingtime_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_remainingtime"));
  data_remainingtime_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_remainingtime"));
  label_currentposition_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_currentposition"));
  data_currentposition_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_currentposition"));
  label_currentstatus_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_currentstatus"));
  data_currentstatus_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_currentstatus"));
  label_currentrate_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_currentrate"));
  data_currentrate_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_currentrate"));
  label_recentrate_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_recentrate"));
  data_recentrate_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_recentrate"));
  label_totalrate_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_totalrate"));
  data_totalrate_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_totalrate"));
  label_skipsize_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_skipsize"));
  data_skipsize_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_skipsize"));
  label_skips_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_skips"));
  data_skips_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_skips"));
  label_slowskips_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_slowskips"));
  data_slowskips_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_slowskips"));
  label_skipruns_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_skipruns"));
  data_skipruns_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_skipruns"));
  label_skipresets_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_skipresets"));
  data_skipresets_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_skipresets"));
  label_runsize_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_runsize"));
  data_runsize_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_runsize"));
  label_finished_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_finished"));
  data_finished_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_finished"));
  label_nontried_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_nontried"));
  data_nontried_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_nontried"));
  label_nontrimmed_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_nontrimmed"));
  data_nontrimmed_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_nontrimmed"));
  label_nondivided_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_nondivided"));
  data_nondivided_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_nondivided"));
  label_nonscraped_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_nonscraped"));
  data_nonscraped_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_nonscraped"));
  label_bad_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_bad"));
  data_bad_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_bad"));
  label_datapreview_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_datapreview"));
  data_datapreview_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_datapreview"));
  label_longestreadtime_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_longestreadtime"));
  data_longestreadtime_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_longestreadtime"));
  label_filled_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_filled"));
  data_filled_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_filled"));
  label_retried_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_retried"));
  data_retried_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_retried"));
  label_retrypasses_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_retrypasses"));
  data_retrypasses_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_retrypasses"));
  cleardomainmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "cleardomainmi"));
  helpmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "helpmi"));
  aboutmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "aboutmi"));
  timermi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "timermi"));
  label_drivermode_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_drivermode"));
  data_drivermode_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_drivermode"));
  installdrivermi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "installdrivermi"));
  uninstalldrivermi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "uninstalldrivermi"));
  fixdrivermemorymi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "fixdrivermemorymi"));
  driveronlymi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "driveronlymi"));
  adddomainmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "adddomainmi"));
  newdomainmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "newdomainmi"));
  savedomainmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "savedomainmi"));
  savedomainasmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "savedomainasmi"));
  disableportsmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "disableportsmi"));
  restoreportsmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "restoreportsmi"));
  displayidentifymi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "displayidentifymi"));
  displayanalyzemi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "displayanalyzemi"));
  displaysmartmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "displaysmartmi"));
  helphtmlmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "helphtmlmi"));
  helptextmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "helptextmi"));
  primaryrelaymi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "primaryrelaymi"));
  chooseprimaryrelaymi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "chooseprimaryrelaymi"));
  disableusbmassmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "disableusbmassmi"));
  restoreusbmassmi_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "restoreusbmassmi"));
  activate_primary_relay_button_main_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "activate_primary_relay_button_main"));
  deactivate_primary_relay_button_main_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "deactivate_primary_relay_button_main"));
  // = GTK_WIDGET (gtk_builder_get_object (builder, "")); // template


  gtk_menu_item_set_label(GTK_MENU_ITEM(filemi_ccc), curlang_ccc[LANGFILE]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(openmi_ccc), curlang_ccc[LANGOPENPROJ]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(newmi_ccc), curlang_ccc[LANGNEWPROJ]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(quitmi_ccc), curlang_ccc[LANGQUIT]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(sourcemi_ccc), curlang_ccc[LANGSOURCE]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(destinationmi_ccc), curlang_ccc[LANGDESTINATION]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(blockmi_ccc), curlang_ccc[LANGBLOCKDEV]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(drivemi_ccc), curlang_ccc[LANGDRIVE]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(drivesmi_ccc), curlang_ccc[LANGDRIVES]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(imagemi_ccc), curlang_ccc[LANGIMAGEFILE]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(devnullmi_ccc), curlang_ccc[LANGNODESTINATION]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(modemi_ccc), curlang_ccc[LANGMODE]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(copyimagemi_ccc), curlang_ccc[LANGCOPYIMAGE]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(genericmi_ccc), curlang_ccc[LANGGENERIC]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(pass_automi_ccc), curlang_ccc[LANGMODEAUTO]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(pass_scsimi_ccc), curlang_ccc[LANGMODESCSI]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(pass_atrami_ccc), curlang_ccc[LANGMODEATA]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(directmi_ccc), curlang_ccc[LANGMODEIDE]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(ahcimi_ccc), curlang_ccc[LANGMODEAHCI]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(usbmi_ccc), curlang_ccc[LANGMODEUSB]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(usbatami_ccc), curlang_ccc[LANGMODEUSBATA]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(settingsmi_ccc), curlang_ccc[LANGSETTINGS]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(advancedmi_ccc), curlang_ccc[LANGADVANCED]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(clonemi_ccc), curlang_ccc[LANGCLONE]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(savemi_ccc), curlang_ccc[LANGSAVE]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(saveasmi_ccc), curlang_ccc[LANGSAVEAS]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(resetstatusmi_ccc), curlang_ccc[LANGRESETSTATUS]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(toolsmi_ccc), curlang_ccc[LANGTOOLS]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(resetlogmi_ccc), curlang_ccc[LANGRESETLOG]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(repairlogmi_ccc), curlang_ccc[LANGREPAIRLOG]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(fillzeromi_ccc), curlang_ccc[LANGFILLZERO]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(fillmarkmi_ccc), curlang_ccc[LANGFILLMARK]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(fillerasemi_ccc), curlang_ccc[LANGFILLERASE]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(domainmi_ccc), curlang_ccc[LANGDOMAIN]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(importddmi_ccc), curlang_ccc[LANGIMPORTDD]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(exportddmi_ccc), curlang_ccc[LANGEXPORTDD]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(cleardomainmi_ccc), curlang_ccc[LANGCLEARDOMAIN]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(helpmi_ccc), curlang_ccc[LANGHELP]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(aboutmi_ccc), curlang_ccc[LANGABOUT]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(timermi_ccc), curlang_ccc[LANGTIMERS]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(installdrivermi_ccc), curlang_ccc[LANGINSTALLDRIVER]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(uninstalldrivermi_ccc), curlang_ccc[LANGUNINSTALLDRIVER]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(fixdrivermemorymi_ccc), curlang_ccc[LANGFIXDRIVERMEMORYERROR]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(driveronlymi_ccc), curlang_ccc[LANGDRIVERONLYMI]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(adddomainmi_ccc), curlang_ccc[LANGADDDOMAINFILE]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(newdomainmi_ccc), curlang_ccc[LANGNEWDOMAINFILE]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(savedomainmi_ccc), curlang_ccc[LANGSAVEDOMAINFILE]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(savedomainasmi_ccc), curlang_ccc[LANGSAVEDOMAINFILEAS]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(disableportsmi_ccc), curlang_ccc[LANGDISABLEPORTS]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(restoreportsmi_ccc), curlang_ccc[LANGRESTOREPORTS]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(displayidentifymi_ccc), curlang_ccc[LANGDISPLAYIDENTIFYRESULTS]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(displayanalyzemi_ccc), curlang_ccc[LANGDISPLAYANALYZERESULTS]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(displaysmartmi_ccc), curlang_ccc[LANGDISPLAYSMARTRESULTS]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(helphtmlmi_ccc), curlang_ccc[LANGHELPHTML]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(helptextmi_ccc), curlang_ccc[LANGHELPTEXT]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(primaryrelaymi_ccc), curlang_ccc[LANGPRIMARYRELAY]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(chooseprimaryrelaymi_ccc), curlang_ccc[LANGCHOOSEPRIMARYUSB]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(disableusbmassmi_ccc), curlang_ccc[LANGDISABLEUSBMASS]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(restoreusbmassmi_ccc), curlang_ccc[LANGRESTOREUSBMASS]);
  gtk_button_set_label(GTK_BUTTON(activate_primary_relay_button_main_ccc), curlang_ccc[LANGACTIVATEMAIN]);
  gtk_button_set_label(GTK_BUTTON(deactivate_primary_relay_button_main_ccc), curlang_ccc[LANGDEACTIVATEMAIN]);
  //gtk_menu_item_set_label(GTK_MENU_ITEM(), curlang_ccc[]); // template

  main_label = GTK_WIDGET (gtk_builder_get_object (builder, "main_label"));

  connect_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "connect_button"));
  disconnect_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "disconnect_button"));
  start_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "start_button"));
  stop_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "stop_button"));
  analyze_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "analyze_button"));
  analyze_long_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "analyze_long_button"));
  smart_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "smart_button"));
  clone_mode_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "clone_mode_button"));
  driver_mode1_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "driver_mode1_button"));
  agressive_driver_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "agressive_driver_checkbutton"));
  driver_mode2_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "driver_mode2_button"));
  driver_mode3_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "driver_mode3_button"));
  driver_mode4_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "driver_mode4_button"));
  driver_mode5_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "driver_mode5_button"));
  soft_reset_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "soft_reset_button"));
  hard_reset_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "hard_reset_button"));
  // = GTK_WIDGET (gtk_builder_get_object (builder, "")); // template

  gtk_button_set_label(GTK_BUTTON(connect_button_ccc), curlang_ccc[LANGCONNECT]);
  gtk_button_set_label(GTK_BUTTON(disconnect_button_ccc), curlang_ccc[LANGDISCONNECT]);
  gtk_button_set_label(GTK_BUTTON(start_button_ccc), curlang_ccc[LANGSTART]);
  gtk_button_set_label(GTK_BUTTON(stop_button_ccc), curlang_ccc[LANGSTOP]);
  gtk_button_set_label(GTK_BUTTON(analyze_button_ccc), curlang_ccc[LANGANALYZE]);
  gtk_button_set_label(GTK_BUTTON(analyze_long_button_ccc), curlang_ccc[LANGANALYZELONG]);
  gtk_button_set_label(GTK_BUTTON(smart_button_ccc), curlang_ccc[LANGSMART]);
  gtk_button_set_label(GTK_BUTTON(clone_mode_button_ccc), curlang_ccc[LANGCLONEMODEBUTTON]);
  gtk_button_set_label(GTK_BUTTON(agressive_driver_checkbutton_ccc), curlang_ccc[LANGAGRESSIVEDRIVERBUTTON]);
  gtk_button_set_label(GTK_BUTTON(driver_mode1_button_ccc), curlang_ccc[LANGDRIVERMODE1BUTTON]);
  gtk_button_set_label(GTK_BUTTON(driver_mode2_button_ccc), curlang_ccc[LANGDRIVERMODE2BUTTON]);
  gtk_button_set_label(GTK_BUTTON(driver_mode3_button_ccc), curlang_ccc[LANGDRIVERMODE3BUTTON]);
  gtk_button_set_label(GTK_BUTTON(driver_mode4_button_ccc), curlang_ccc[LANGDRIVERMODE4BUTTON]);
  gtk_button_set_label(GTK_BUTTON(driver_mode5_button_ccc), curlang_ccc[LANGDRIVERMODE5BUTTON]);
  gtk_button_set_label(GTK_BUTTON(soft_reset_button_ccc), curlang_ccc[LANGHARDRESETBUTTON]);
  gtk_button_set_label(GTK_BUTTON(hard_reset_button_ccc), curlang_ccc[LANGSOFTRESETBUTTON]);
  //gtk_button_set_label(GTK_BUTTON(), curlang_ccc[]); // template



  gtk_label_set_text(GTK_LABEL(label_project_ccc), curlang_ccc[LANGPROJECTLABEL]);
  gtk_label_set_text(GTK_LABEL(label_domain_ccc), curlang_ccc[LANGDOMAINLABEL]);
  gtk_label_set_text(GTK_LABEL(label_source_ccc), curlang_ccc[LANGSOURCELABEL]);
  gtk_label_set_text(GTK_LABEL(label_destination_ccc), curlang_ccc[LANGDESTINATIONLABEL]);
  gtk_label_set_text(GTK_LABEL(label_totallba_ccc), curlang_ccc[LANGTOTALLBALABEL]);
  gtk_label_set_text(GTK_LABEL(label_lbatoread_ccc), curlang_ccc[LANGLBATOREADLABEL]);
  gtk_label_set_text(GTK_LABEL(label_domainsize_ccc), curlang_ccc[LANGDOMAINSIZELABEL]);
  gtk_label_set_text(GTK_LABEL(label_runtime_ccc), curlang_ccc[LANGRUNTIMELABEL]);
  gtk_label_set_text(GTK_LABEL(label_remainingtime_ccc), curlang_ccc[LANGREMAININGTIMELABEL]);
  gtk_label_set_text(GTK_LABEL(label_currentposition_ccc), curlang_ccc[LANGCURRENTPOSISIONLABEL]);
  gtk_label_set_text(GTK_LABEL(label_currentstatus_ccc), curlang_ccc[LANGCURRENTSTATUSLABEL]);
  gtk_label_set_text(GTK_LABEL(label_currentrate_ccc), curlang_ccc[LANGCURRENTRATELABEL]);
  gtk_label_set_text(GTK_LABEL(label_recentrate_ccc), curlang_ccc[LANGRECENTRATELABEL]);
  gtk_label_set_text(GTK_LABEL(label_totalrate_ccc), curlang_ccc[LANGTOTALRATELABEL]);
  gtk_label_set_text(GTK_LABEL(label_skipsize_ccc), curlang_ccc[LANGSKIPSIZELABEL]);
  gtk_label_set_text(GTK_LABEL(label_skips_ccc), curlang_ccc[LANGSKIPSLABEL]);
  gtk_label_set_text(GTK_LABEL(label_slowskips_ccc), curlang_ccc[LANGSLOWSKIPLABEL]);
  gtk_label_set_text(GTK_LABEL(label_skipruns_ccc), curlang_ccc[LANGSKIPRUNSLABEL]);
  gtk_label_set_text(GTK_LABEL(label_skipresets_ccc), curlang_ccc[LANGSKIPRESETSLABEL]);
  gtk_label_set_text(GTK_LABEL(label_runsize_ccc), curlang_ccc[LANGRUNSIZELABEL]);
  gtk_label_set_text(GTK_LABEL(label_finished_ccc), curlang_ccc[LANGFINISHEDLABEL]);
  gtk_label_set_text(GTK_LABEL(label_nontried_ccc), curlang_ccc[LANGNONTRIEDLABEL]);
  gtk_label_set_text(GTK_LABEL(label_nontrimmed_ccc), curlang_ccc[LANGNONTRIMMEDLABEL]);
  gtk_label_set_text(GTK_LABEL(label_nondivided_ccc), curlang_ccc[LANGNONDIVIDEDLABEL]);
  gtk_label_set_text(GTK_LABEL(label_nonscraped_ccc), curlang_ccc[LANGNONSCRAPEDLABEL]);
  gtk_label_set_text(GTK_LABEL(label_bad_ccc), curlang_ccc[LANGBADLABEL]);
  gtk_label_set_text(GTK_LABEL(label_datapreview_ccc), curlang_ccc[LANGDATAPREVIEWLABEL]);
  gtk_label_set_text(GTK_LABEL(label_longestreadtime_ccc), curlang_ccc[LANGLONGESTREADTIMELABEL]);
  gtk_label_set_text(GTK_LABEL(label_filled_ccc), curlang_ccc[LANGFILLEDLABEL]);
  gtk_label_set_text(GTK_LABEL(label_retried_ccc), curlang_ccc[LANGRETRIEDLABEL]);
  gtk_label_set_text(GTK_LABEL(label_retrypasses_ccc), curlang_ccc[LANGRETRYPASSESLABEL]);
  gtk_label_set_text(GTK_LABEL(label_drivermode_ccc), curlang_ccc[LANGDRIVERMODE]);
  gtk_label_set_text(GTK_LABEL(data_drivermode_ccc), curlang_ccc[LANGCLONEMODE]);
  //gtk_label_set_text(GTK_LABEL(), curlang_ccc[]); // template



  // set status buttons
  button_bbk_ccc = gtk_button_new_with_label("BBK");
  button_unc_ccc = gtk_button_new_with_label("UNC");
  button_mc_ccc = gtk_button_new_with_label("MC");
  button_idnf_ccc = gtk_button_new_with_label("IDNF");
  button_mcr_ccc = gtk_button_new_with_label("MCR");
  button_abrt_ccc = gtk_button_new_with_label("ABRT");
  button_tk0nf_ccc = gtk_button_new_with_label("TK0NF");
  button_amnf_ccc = gtk_button_new_with_label("AMNF");
  button_bsy_ccc = gtk_button_new_with_label("BSY");
  button_drdy_ccc = gtk_button_new_with_label("DRDY");
  button_df_ccc = gtk_button_new_with_label("DF");
  button_dsc_ccc = gtk_button_new_with_label("DSC");
  button_drq_ccc = gtk_button_new_with_label("DRQ");
  button_corr_ccc = gtk_button_new_with_label("CORR");
  button_idx_ccc = gtk_button_new_with_label("IDX");
  button_err_ccc = gtk_button_new_with_label("ERR");
  button_status_separator_ccc = gtk_button_new_with_label("|");

  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_bsy_ccc, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_drdy_ccc, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_df_ccc, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_dsc_ccc, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_drq_ccc, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_corr_ccc, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_idx_ccc, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_err_ccc, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_status_separator_ccc, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_bbk_ccc, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_unc_ccc, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_mc_ccc, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_idnf_ccc, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_mcr_ccc, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_abrt_ccc, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_tk0nf_ccc, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (bottom_status_bar_ccc), button_amnf_ccc, TRUE, TRUE, 0);


  // set intial button states
  set_disconnected_ccc();

  set_mode_auto_passthrough_ccc();

  if (scsi_passthrough_ccc || generic_mode_ccc)
  {
    deactivate_status_buttons_ccc();
  }
  else
  {
    activate_status_buttons_ccc();
  }





  g_object_unref(builder);

  gtk_widget_show_all(main_window_ccc);
  gtk_main();

  return 0;
}






int copy_enlanguage_ccc(void)
{
  for (int i = 0; i < LANGCOUNT; i++)
  {
    strlcpy (curlang_ccc[i], enlang_ccc[i], sizeof(curlang_ccc[i]));
  }
  return 0;
}





int copy_newlanguage_ccc(void)
{
  int i;
  for (i = 0; i < LANGCOUNT; i++)
  {
    strlcpy (curlang_ccc[i], newlang_ccc[i], sizeof(curlang_ccc[i]));
    //fprintf(stdout, "%d %s", i, newlang_ccc[i]);
  }
  return 0;
}





int set_language_ccc(void)
{
  language_window_ccc = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  //gtk_window_set_keep_above(GTK_WINDOW(language_window_ccc), TRUE);
  gtk_window_set_title(GTK_WINDOW(language_window_ccc), "Language Options");
  gtk_window_set_default_size(GTK_WINDOW(language_window_ccc), 300, 200);
  gtk_window_set_position(GTK_WINDOW(language_window_ccc), GTK_WIN_POS_CENTER);

  gint handler_id1 = g_signal_connect(G_OBJECT(language_window_ccc), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  GtkWidget *box = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (language_window_ccc), box);

  GtkWidget *enbutton = gtk_button_new_with_label("Continue");
  //GtkWidget *exbutton = gtk_button_new_with_label("Export language file");
  GtkWidget *imbutton = gtk_button_new_with_label("Change Language");
  GtkWidget *okim = gtk_image_new_from_stock(GTK_STOCK_OK, GTK_ICON_SIZE_BUTTON);
  //GtkWidget *saveim = gtk_image_new_from_stock(GTK_STOCK_SAVE, GTK_ICON_SIZE_BUTTON);
  GtkWidget *openim = gtk_image_new_from_stock(GTK_STOCK_OPEN, GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image(GTK_BUTTON(enbutton), okim);
  //gtk_button_set_image(GTK_BUTTON(exbutton), saveim);
  gtk_button_set_image(GTK_BUTTON(imbutton), openim);

  gtk_box_pack_start (GTK_BOX (box), enbutton, TRUE, TRUE, 0);
  //gtk_box_pack_start (GTK_BOX (box), exbutton, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (box), imbutton, TRUE, TRUE, 0);

  gint handler_id2 = g_signal_connect (enbutton, "clicked", G_CALLBACK (gtk_main_quit), NULL);
  //gint handler_id3 = g_signal_connect (exbutton, "clicked", G_CALLBACK (export_language_file_ccc), NULL);
  gint handler_id4 = g_signal_connect (imbutton, "clicked", G_CALLBACK (import_language_file_ccc), NULL);

  gtk_widget_show_all(language_window_ccc);
  gtk_main();
  g_signal_handler_disconnect(language_window_ccc, handler_id1);
  g_signal_handler_disconnect(enbutton, handler_id2);
  //g_signal_handler_disconnect(exbutton, handler_id3);
  g_signal_handler_disconnect(imbutton, handler_id4);
  gtk_widget_hide_all(language_window_ccc);
  gtk_widget_destroy(language_window_ccc);

  return 0;
}




int set_data_dump_filename_ccc(void)
{
  language_window_ccc = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  //gtk_window_set_keep_above(GTK_WINDOW(language_window_ccc), TRUE);
  gtk_window_set_title(GTK_WINDOW(language_window_ccc), "Select Data Dump File");
  gtk_window_set_default_size(GTK_WINDOW(language_window_ccc), 400, 300);
  gtk_window_set_position(GTK_WINDOW(language_window_ccc), GTK_WIN_POS_CENTER);

  gint handler_id1 = g_signal_connect(G_OBJECT(language_window_ccc), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  GtkWidget *box = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (language_window_ccc), box);

  GtkWidget *enbutton = gtk_button_new_with_label("Continue");
  GtkWidget *imbutton = gtk_button_new_with_label("Change Data Dump File");
  GtkWidget *okim = gtk_image_new_from_stock(GTK_STOCK_OK, GTK_ICON_SIZE_BUTTON);
  GtkWidget *openim = gtk_image_new_from_stock(GTK_STOCK_OPEN, GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image(GTK_BUTTON(enbutton), okim);
  gtk_button_set_image(GTK_BUTTON(imbutton), openim);

  gtk_box_pack_start (GTK_BOX (box), enbutton, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (box), imbutton, TRUE, TRUE, 0);

  gint handler_id2 = g_signal_connect (enbutton, "clicked", G_CALLBACK (gtk_main_quit), NULL);
  gint handler_id4 = g_signal_connect (imbutton, "clicked", G_CALLBACK (get_data_dump_filename_ccc), NULL);

  gtk_widget_show_all(language_window_ccc);
  gtk_main();
  g_signal_handler_disconnect(language_window_ccc, handler_id1);
  g_signal_handler_disconnect(enbutton, handler_id2);
  g_signal_handler_disconnect(imbutton, handler_id4);
  gtk_widget_hide_all(language_window_ccc);
  gtk_widget_destroy(language_window_ccc);

  return 0;
}





void get_data_dump_filename_ccc(void)
{
  GtkWidget *dialog;
  dialog = gtk_file_chooser_dialog_new ("Select Data Dump File",
                                        GTK_WINDOW(language_window_ccc),
                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                        NULL);
  gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "debug_data_dump.log");
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    fprintf (stdout, "%s\n", filename);
    strlcpy (data_dump_filename_ccc, filename, sizeof(data_dump_filename_ccc));
    g_free (filename);
  }
  gtk_widget_destroy (dialog);
}





#ifdef DEBUG
#ifdef USE_CURL
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}
#endif
#endif



int translate_all_ccc(void)
{
  unsigned int total_language_items = n_languages_ccc;
  int translate_failed = 0;
  if (translate_ccc >= 80000 && translate_ccc < 80010)
  {
    char (*reverselang)[LANGCOUNT][MAXLANGLENGTH];
    reverselang = malloc((total_language_items/3) * sizeof *reverselang);
    char (*alllang)[LANGCOUNT][MAXLANGLENGTH];
    alllang = malloc((total_language_items/3) * sizeof *alllang);
    char *reverse_file = "REVERSE_LANGUAGES.CSV";
    char *all_file = "ALL_LANGUAGES.CSV";
    char *export_file = "English";
    FILE *writefile;
    writefile = fopen(export_file, "w");
    if (writefile == NULL)
    {
      snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s: %s (%s)", curlang_ccc[LANGLANGEXPORTERR], export_file, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
    fprintf(writefile, "%s", program_title);
    unsigned int i;
    unsigned int n;
    for (i = 0; i < LANGCOUNT; i++)
    {
      fprintf(writefile, "\n|__|%04d|__|\n", i);
      fprintf(writefile, "%s", enlang_ccc[i]);
    }
    fclose(writefile);
    chmod(export_file, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if (total_language_items % 3)
    {
      fprintf (stdout, "\n*****************************************************\n");
      fprintf (stdout, "\n*****************************************************\n");
      fprintf (stdout, "\n*****************************************************\n");
      fprintf (stdout, "language count not dividable by 3\n");
      fprintf (stdout, "\n*****************************************************\n");
      fprintf (stdout, "\n*****************************************************\n");
      fprintf (stdout, "\n*****************************************************\n");
      return -1;
    }
    for (i = 0; i < total_language_items; i+=3)
    {
      fprintf (stdout, "translating %d of %d\n", i/3, total_language_items/3);
      char *translang = languages_ccc[i+1];
      char *language = languages_ccc[i];
      const char *native = languages_ccc[i+2];
      copy_enlanguage_ccc();
      if ( translate_language_ccc("en", translang, language, native) )
      {
        fprintf (stdout, "\n*****************************************************\n");
        fprintf (stdout, "Failed to translate to %s\n", language);
        fprintf (stdout, "\n*****************************************************\n");
        translate_failed = 1;
      }
      if (translate_ccc == 80009)
      {
        for (n = 0; n < LANGCOUNT; n++)
        {
          strcpy (alllang[i/3][n], newlang_ccc[n]);
        }
        copy_newlanguage_ccc();
        if (translate_language_ccc(translang, "en", language, "english") )
        {
          fprintf (stdout, "\n*****************************************************\n");
          fprintf (stdout, "Failed to reverse translate %s\n", language);
          fprintf (stdout, "\n*****************************************************\n");
          translate_failed = 1;
        }
        for (n = 0; n < LANGCOUNT; n++)
        {
          strcpy (reverselang[i/3][n], newlang_ccc[n]);
        }
      }
    }

    if (translate_ccc == 80009)
    {
      writefile = fopen(reverse_file, "w");
      if (writefile == NULL)
      {
        snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s: %s (%s)", curlang_ccc[LANGLANGEXPORTERR], reverse_file, strerror(errno));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
      }
      fprintf(writefile, "\"%s\", ", "English");
      for (i = 0; i < total_language_items; i+=3)
      {
        fprintf(writefile, "\"%s\", ", languages_ccc[i]);
      }
      fprintf(writefile, "\n");
      for (n = 0; n < LANGCOUNT; n++)
      {
        fprintf(writefile, "\"%s\", ", enlang_ccc[n]);
        for (i = 0; i < (total_language_items/3); i++)
        {
          fprintf(writefile, "\"%s\", ", reverselang[i][n]);
        }
        fprintf(writefile, "\n");
      }
      fclose(writefile);
      chmod(reverse_file, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);


      writefile = fopen(all_file, "w");
      if (writefile == NULL)
      {
        snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s: %s (%s)", curlang_ccc[LANGLANGEXPORTERR], all_file, strerror(errno));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
      }
      fprintf(writefile, "\"%s\", ", "English");
      for (i = 0; i < total_language_items; i+=3)
      {
        fprintf(writefile, "\"%s\", ", languages_ccc[i]);
      }
      fprintf(writefile, "\n");
      for (n = 0; n < LANGCOUNT; n++)
      {
        fprintf(writefile, "\"%s\", ", enlang_ccc[n]);
        for (i = 0; i < (total_language_items/3); i++)
        {
          fprintf(writefile, "\"%s\", ", alllang[i][n]);
        }
        fprintf(writefile, "\n");
      }
      fclose(writefile);
      chmod(all_file, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    }
  }
  if (translate_failed)
  {
    fprintf (stdout, "\n*****************************************************\n");
    fprintf (stdout, "\n*****************************************************\n");
    fprintf (stdout, "\n*****************************************************\n");
    fprintf (stdout, "Failed to translate\n");
    fprintf (stdout, "\n*****************************************************\n");
    fprintf (stdout, "\n*****************************************************\n");
    fprintf (stdout, "\n*****************************************************\n");
    return -1;
  }
  return 0;
}





int translate_language_ccc(char *fromlang, char *translang, const char *language, const char *native)
{
  int failure = 0;
  char return_data[1000000];
  char new_lang_data[500000];
  char url_data[250000];
  char lang_data[250000];
  char new_data[250000];
  char temp_data[100000];
  strcpy (lang_data, "");
  strcpy (new_data, "");
  strcpy (return_data, "");
  int i;
  for (i = 0; i < LANGCOUNT; i++)
  {
    strcpy (newlang_ccc[i], "");
  }
  for (i = 0; i < LANGCOUNT; i++)
  {
    snprintf(temp_data, sizeof(temp_data), "\n|__|%04d|__|\n", i);
    strcat(lang_data, temp_data);
    snprintf(temp_data, sizeof(temp_data), "%s", curlang_ccc[i]);
    strcat(lang_data, temp_data);
    int linebyline = 0;
    if (strlen(lang_data) > 3000 || i == LANGCOUNT - 1 || linebyline)
    {
      int n;
      int len = strlen(lang_data);
      for (n = 0; n < len; n++)
      {
        char c = lang_data[n];
        if (c == ' ')
        {
          strcat (new_data, "%20");
        }
        else if (c == '\n')
        {
          strcat (new_data, "%0a");
        }
        else
        {
          strncat (new_data, &c, 1);
        }
      }
      //fprintf (stdout, "%s\n", new_data);

      strlcpy(url_data, "https://translate.googleapis.com/translate_a/single?client=gtx&sl=", sizeof(url_data));
      strcat(url_data, fromlang);
      strcat(url_data, "&tl=");
      strcat(url_data, translang);
      strcat(url_data, "&dt=t&q=");
      strcat(url_data, new_data);

      fprintf (stdout, "%d %s to %s  ", i, fromlang, translang);
      do_nanosleep_ccc(TRANSLATETIMERFAST);  // this is a timer to deal with google translator
      char *data = get_translated_data_ccc(url_data);
      //fprintf (stdout, "%s\n", data);
      strcat (return_data, data);
      strcpy (lang_data, "");
      strcpy (new_data, "");
    }
  }
  //fprintf (stdout, "\n*****************************************************\n");
  //fprintf (stdout, "%s\n", return_data);

  strcpy (new_lang_data, "");
  int return_length = strlen(return_data);
  int bracket_depth = 0;
  for (i = 0; i < return_length; i++)
  {
    if (return_data[i] == '[')
    {
      bracket_depth++;
      if (return_data[i+1] == '"' && bracket_depth < 5)
      {
        int n = i+2;
        while (n < return_length)
        {
          if (return_data[n] == '"')
          {
            strcat (new_lang_data, "\n");
            break;
          }
          else if (return_data[n] == '\\' && return_data[n+1] == 'n')
          {
            strcat (new_lang_data, "\n");
            if (return_data[n+2] == '\\' && return_data[n+3] == 'n')
            {
              strcat (new_lang_data, "\n");
            }
            if (return_data[n+4] == '\\' && return_data[n+5] == 'n')
            {
              strcat (new_lang_data, "\n");
            }
            break;
          }
          char c = return_data[n];
          strncat (new_lang_data, &c, 1);
          n++;
          i = n;
        }
      }
    }
    else if (return_data[i] == ']')
    {
      bracket_depth--;
    }
  }
  //fprintf (stdout, "\n*****************************************************\n");
  //fprintf (stdout, "%s\n", new_lang_data);

  // read language data out of the variable
  char * line = new_lang_data;
  int count = 0;
  int found_count = -1;
  while(line)
  {
    char * nextLine = strchr(line, '\n');
    if (nextLine) *nextLine = '\0';  // temporarily terminate the current line
    //printf("%s\n", line);

    int scanline;
    int new_count = -1;
    scanline = sscanf(line, "| _ _ | %d | _ _ |", &new_count);
    if (scanline == 1)
    {
      if (new_count >= 0 && new_count < LANGCOUNT)
      {
        //fprintf (stdout, "num=%d, line=%s\n", new_count, line);
        // remove the last new line from the end
        if (found_count >= 0)
        {
          int l = strlen(newlang_ccc[found_count]);
          if (l > 0 && newlang_ccc[found_count][l-1] == '\n')
          {
            newlang_ccc[found_count][l-1] = '\0';
          }
        }
        found_count = new_count;
        strcpy (newlang_ccc[found_count], "");
        if (count != found_count)
        {
          //fprintf (stdout, "FAILED count=%d found_count=%d\n", count, found_count);
        }
        count++;
      }
      else
      {
        fprintf (stdout, "Warning: language count %d not valid.\n", new_count);
        failure = 1;
      }
    }
    else if (found_count >= 0 && found_count < LANGCOUNT)
    {
      unsigned int l = strlen(newlang_ccc[found_count]);
      strncat (newlang_ccc[found_count], line, MAXLANGLENGTH - l - 1);
      strcat (newlang_ccc[found_count], "\n");
      //fprintf (stdout, "%s\n", newlang_ccc[found_count]);
      if (MAXLANGLENGTH - l < strlen(line))
      {
        fprintf (stdout, "Warning: language count %d exceeded max length and was truncated.\n", found_count);
        failure = 1;
      }
    }

    if (nextLine) *nextLine = '\n';  // then restore newline-char, just to be tidy
    line = nextLine ? (nextLine+1) : NULL;
  }
  count--;
  if (count == found_count && count == LANGCOUNT - 1)
  {
    // do noting
  }
  else
  {
    fprintf (stdout, "FAILED count=%d found_count=%d LANGCOUNT=%d\n", count, found_count, LANGCOUNT-1);
    failure = 1;
  }

  if (failure && 0)
  {
    int retry = translate_language_slow_ccc(fromlang, translang, language, native);
    return retry;
  }

  char langfile[256];
  strlcpy (langfile, language, sizeof(langfile));
  strcat (langfile, "(");
  strcat (langfile, native);
  strcat (langfile, ")");
  if (failure)
  {
    strcat (langfile, "_FAILED_TRANSLATION");
  }
  FILE *writefile;
  writefile = fopen(langfile, "w");
  if (writefile == NULL)
  {
    snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s: %s (%s)", curlang_ccc[LANGLANGEXPORTERR], langfile, strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }

  fprintf(writefile, "%s", program_title);
  for (i = 0; i < LANGCOUNT; i++)
  {
    fprintf(writefile, "\n|__|%04d|__|\n", i);
    fprintf(writefile, "%s", newlang_ccc[i]);
  }

  fclose(writefile);
  chmod(langfile, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

  return failure;
}





char* get_translated_data_ccc(char *url_data)
{
 #ifdef DEBUG
  #ifndef USE_CURL
   return url_data;
  #else

  do_nanosleep_ccc(TRANSLATETIMERALL);  // this is a timer to deal with google translator
  CURL *curl_handle;
  CURLcode res;

  struct MemoryStruct chunk;

  chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
  chunk.size = 0;    /* no data at this point */

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
  curl_handle = curl_easy_init();

  /* specify URL to get */
  curl_easy_setopt(curl_handle, CURLOPT_URL, url_data);

  /* specify minimum TLS version */
  curl_easy_setopt(curl_handle, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);

  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0");

  /* get it! */
  res = curl_easy_perform(curl_handle);

  /* check for errors */
  if(res != CURLE_OK)
  {
    fprintf(stdout, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  }
  else
  {
    fprintf(stdout, "%lu bytes retrieved\n", (long)chunk.size);
    //printf("%s\n", chunk.memory);
  }

  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);

  //free(chunk.memory);

  /* we're done with libcurl, so clean it up */
  curl_global_cleanup();

  return chunk.memory;
  #endif
#else
  return url_data;
#endif
}





int translate_language_slow_ccc(char *fromlang, char *translang, const char *language, const char *native)
{
  int failure = 0;
  char return_data[65536];
  char new_lang_data[32768];
  char url_data[6000];
  char lang_data[8000];
  char new_data[16000];
  char temp_data[4096];
  strcpy (lang_data, "");
  strcpy (new_data, "");
  strcpy (return_data, "");
  for (int count = 0; count < LANGCOUNT; count++)
  {
    strcpy (lang_data, "");
    strcpy (new_data, "");
    snprintf(temp_data, sizeof(temp_data), "%s", curlang_ccc[count]);
    strcat(lang_data, temp_data);
    int len = strlen(lang_data);
    for (int n = 0; n < len; n++)
    {
      char c = lang_data[n];
      if (c == ' ')
      {
        strcat (new_data, "%20");
      }
      else if (c == '\n')
      {
        strcat (new_data, "%0a");
      }
      else
      {
        strncat (new_data, &c, 1);
      }
    }
    //fprintf (stdout, "%s\n", new_data);

    strlcpy(url_data, "https://translate.googleapis.com/translate_a/single?client=gtx&sl=", sizeof(url_data));
    strcat(url_data, fromlang);
    strcat(url_data, "&tl=");
    strcat(url_data, translang);
    strcat(url_data, "&dt=t&q=");
    strcat(url_data, new_data);

    fprintf (stdout, "%d %s to %s  ", count, fromlang, translang);
    do_nanosleep_ccc(TRANSLATETIMERSLOW);  // this is a timer to deal with google translator
    const char *data = get_translated_data_ccc(url_data);
    //fprintf (stdout, "%s\n", data);
    strlcpy (return_data, data, sizeof(return_data));
    //fprintf (stdout, "\n*****************************************************\n");
    //fprintf (stdout, "%s\n", return_data);

    strcpy (new_lang_data, "");
    int return_length = strlen(return_data);
    for (int i = 0; i < return_length; i++)
    {
      if (return_data[i] == '[')
      {
        if (return_data[i+1] == '"')
        {
          int n = i+2;
          while (n < return_length)
          {
            if (return_data[n] == '"')
            {
              break;
            }
            else if (return_data[n] == '\\' && return_data[n+1] == 'n')
            {
              strcat (new_lang_data, "\n");
              if (return_data[n+2] == '\\' && return_data[n+3] == 'n')
              {
                strcat (new_lang_data, "\n");
              }
              if (return_data[n+4] == '\\' && return_data[n+5] == 'n')
              {
                strcat (new_lang_data, "\n");
              }
              break;
            }
            char c = return_data[n];
            strncat (new_lang_data, &c, 1);
            n++;
            i = n;
          }
        }
      }
    }
    //fprintf (stdout, "\n*****************************************************\n");
    //fprintf (stdout, "%s\n", new_lang_data);
    if (strlen(new_lang_data) >= MAXLANGLENGTH-1)
    {
      fprintf (stdout, "Warning: language count %d exceeded max length and was truncated.\n", count);
      failure = 1;
    }
    strcpy (newlang_ccc[count], new_lang_data);
  }


  char langfile[256];
  strlcpy (langfile, language, sizeof(langfile));
  strcat (langfile, "(");
  strcat (langfile, native);
  strcat (langfile, ")");
  if (failure)
  {
    strcat (langfile, "_FAILED_TRANSLATION");
  }
  FILE *writefile;
  writefile = fopen(langfile, "w");
  if (writefile == NULL)
  {
    snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s: %s (%s)", curlang_ccc[LANGLANGEXPORTERR], langfile, strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }

  fprintf(writefile, "%s", program_title);
  for (int i = 0; i < LANGCOUNT; i++)
  {
    fprintf(writefile, "\n|__|%04d|__|\n", i);
    fprintf(writefile, "%s", newlang_ccc[i]);
  }

  fclose(writefile);
  chmod(langfile, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);


  return failure;
}





int print_gui_error_message_ccc(char *message, char *title, int type)
{
  if (!superclone_ccc)
  {
    message_now_ccc(message);
  }
  else if (check_message_ccc && (do_call_command_ccc || activate_primary_relay_on_error_ccc ) )
  {
    g_print("%s\n", message);
  }
  else
  {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *dialog;
#if 0
    GtkDialogFlags message_type;
#else
    GtkMessageType message_type;
#endif
    if (type)
    {
      message_type = GTK_MESSAGE_WARNING;
      //dialog = gtk_message_dialog_new(GTK_WINDOW(main_window_ccc), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "%s", message);
    }
    else
    {
      message_type = GTK_MESSAGE_INFO;
      //dialog = gtk_message_dialog_new(GTK_WINDOW(main_window_ccc), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", message);
    }
    dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, message_type, GTK_BUTTONS_OK, "%s", message);
    gtk_window_set_keep_above(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_widget_destroy(window);
  }

  check_message_ccc = false;
  return 0;
}





void export_language_file_ccc(void)
{
  GtkWidget *dialog;
  dialog = gtk_file_chooser_dialog_new (curlang_ccc[LANGEXPORTLANG],
                                      GTK_WINDOW(language_window_ccc),
                                      GTK_FILE_CHOOSER_ACTION_SAVE,
                                      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                      NULL);
  gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    fprintf (stdout, "%s\n", filename);
    file_export_sel_ccc(filename);
    g_free (filename);
  }
  gtk_widget_destroy (dialog);
}





// get the export filename and load it
static void file_export_sel_ccc( char *export_file )
{
  FILE *writefile;
  writefile = fopen(export_file, "w");
  if (writefile == NULL)
  {
    snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s: %s (%s)", curlang_ccc[LANGLANGEXPORTERR], export_file, strerror(errno));
    message_error_ccc(tempmessage_ccc);
//     print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }

  fprintf(writefile, "%s", program_title);
  int i;
  for (i = 0; i < LANGCOUNT; i++)
  {
    fprintf(writefile, "\n|__|%04d|__|\n", i);
    fprintf(writefile, "%s", enlang_ccc[i]);
  }

  fclose(writefile);
  chmod(export_file, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}





void import_language_file_ccc(void)
{
  GtkWidget *dialog;
  dialog = gtk_file_chooser_dialog_new (curlang_ccc[LANGIMPORTLANG],
                                        GTK_WINDOW(language_window_ccc),
                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                        NULL);
  if ( access( "/usr/local/share/doc/hddsuperclone/Language/English", F_OK ) == 0 )
  {
    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), "/usr/local/share/doc/hddsuperclone/Language");
    g_print ("found installed languages\n");
  }
  else if ( access( "Language/English", F_OK ) == 0 )
  {
    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), "Language");
    g_print ("found languages locally\n");
  }
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    fprintf (stdout, "%s\n", filename);
    file_import_sel_ccc(filename);
    g_free (filename);
  }
  gtk_widget_destroy (dialog);
}





// get the import filename and load it
static void file_import_sel_ccc( char *import_file )
{
  FILE *readfile;
  readfile = fopen(import_file, "r");
  if (readfile == NULL)
  {
    snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s: %s (%s)", curlang_ccc[LANGLANGIMPORTERR], import_file, strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {

    char line[MAXLANGLENGTH];
    int count = 0;
    int found_count = -1;
    char found_title[MAXLANGLENGTH] = "";
    int firstline = 1;

    while (fgets(line, sizeof line, readfile))
    {
      if (firstline)
      {
        strncpy (found_title, line, MAXLANGLENGTH - 1);
        firstline = 0;
      }
      else
      {
        int scanline;
        int new_count = -1;
        scanline = sscanf(line, "| _ _ | %d | _ _ |", &new_count);
        if (scanline == 1)
        {
          //fprintf (stdout, "num=%d, line=%s", new_count, line);
          // remove the last new line from the end
          if (found_count >= 0)
          {
            int l = strlen(newlang_ccc[found_count]);
            if (l > 0 && newlang_ccc[found_count][l-1] == '\n')
            {
              newlang_ccc[found_count][l-1] = '\0';
            }
          }
          found_count = new_count;
          strlcpy (newlang_ccc[found_count], "", sizeof(newlang_ccc[found_count]));
          count++;
        }
        else if (found_count >= 0 && found_count < LANGCOUNT)
        {
          unsigned int l = strlen(newlang_ccc[found_count]);
          strncat (newlang_ccc[found_count], line, MAXLANGLENGTH - l - 1);
          //fprintf (stdout, "%s", newlang_ccc[found_count]);
          if (MAXLANGLENGTH - l < strlen(line))
          {
            g_print ("Warning: language count %d exceeded max length and was truncated.\n", found_count);
          }
        }
      }
    }

    count--;
    if (count == found_count && count == LANGCOUNT - 1)
    {
      copy_newlanguage_ccc();
      snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s", curlang_ccc[LANGLANGCHANGESUCCESS]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGSUCCESS], 0);
      clear_error_message_ccc();
    }
    else
    {
      fprintf (stdout, "count=%d found_count=%d LANGCOUNT=%d\n", count, found_count, LANGCOUNT-1);
      snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s", curlang_ccc[LANGLANGIMPORTERR2]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }

    fclose(readfile);
  }
}





void select_file_ccc(void)
{
  if (connected_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGSTILLCONNECTED], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new (curlang_ccc[LANGLOADPROJFILE],
                                          GTK_WINDOW(main_window_ccc),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      clear_source_ccc();
      clear_destination_ccc();
      project_chosen_ccc = 0;
      char *filename;
      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      fprintf (stdout, "%s\n", filename);
      load_log_file_ccc(filename);
      g_free (filename);
    }
    gtk_widget_destroy (dialog);
  }
}





void new_file_ccc(void)
{
  if (connected_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGSTILLCONNECTED], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new (curlang_ccc[LANGNEWPROJFILE],
                                          GTK_WINDOW(main_window_ccc),
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      clear_source_ccc();
      clear_destination_ccc();
      char *filename;
      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      fprintf (stdout, "%s\n", filename);
      new_log_file_ccc(filename);
      g_free (filename);
    }
    gtk_widget_destroy (dialog);
  }
}





void new_domain_ccc(void)
{
  if (connected_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGSTILLCONNECTED], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new (curlang_ccc[LANGNEWDOMAINFILE],
                                          GTK_WINDOW(main_window_ccc),
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      char *filename;
      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      fprintf (stdout, "%s\n", filename);
      new_domain_file_ccc(filename);
      g_free (filename);
    }
    gtk_widget_destroy (dialog);
  }
}





void select_ddrescue_file_ccc(void)
{
  if (connected_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGSTILLCONNECTED], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else if (!new_project_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGCHOOSENEWPROJECT], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new (curlang_ccc[LANGIMPORTDD],
                                          GTK_WINDOW(main_window_ccc),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      char *filename;
      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      fprintf (stdout, "%s\n", filename);
      load_ddrescue_log_file_ccc(filename);
      g_free (filename);
    }
    gtk_widget_destroy (dialog);
  }
}





void save_file_ccc(void)
{
  if (!project_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNOPROJECT], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    return_value_ccc = write_logfile_ccc(log_file_ccc, 0);
    if (return_value_ccc)
    {
      strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGWRITEERR], sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
  }
}





void save_file_as_ccc(void)
{
  if (!project_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNOPROJECT], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new (curlang_ccc[LANGSAVEPROJFILE],
                                          GTK_WINDOW(main_window_ccc),
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      char *filename;
      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      fprintf (stdout, "%s\n", filename);
      return_value_ccc = write_logfile_ccc(filename, 0);
      if (return_value_ccc)
      {
        strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGWRITEERR], sizeof(tempmessage_ccc));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
      }
      g_free (filename);
    }
    gtk_widget_destroy (dialog);
  }
}





void save_domain_ccc(void)
{
  if (!project_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNOPROJECT], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    return_value_ccc = write_domainfile_ccc(domain_file_ccc, 0);
    if (return_value_ccc)
    {
      strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGWRITEERR], sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
  }
}





void save_domain_as_ccc(void)
{
  if (!project_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNOPROJECT], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new (curlang_ccc[LANGSAVEDOMAINFILEAS],
                                          GTK_WINDOW(main_window_ccc),
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      char *filename;
      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      fprintf (stdout, "%s\n", filename);
      return_value_ccc = write_domainfile_ccc(filename, 0);
      if (return_value_ccc)
      {
        strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGWRITEERR], sizeof(tempmessage_ccc));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
      }
      g_free (filename);
    }
    gtk_widget_destroy (dialog);
  }
}





void export_ddrescue_file_ccc(void)
{
  if (!project_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNOPROJECT], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new (curlang_ccc[LANGEXPORTDD],
                                          GTK_WINDOW(main_window_ccc),
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      char *filename;
      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      fprintf (stdout, "%s\n", filename);
      return_value_ccc = write_ddrescue_logfile_ccc(filename);
      if (return_value_ccc)
      {
        strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGEXPORTERR], sizeof(tempmessage_ccc));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
      }
      g_free (filename);
    }
    gtk_widget_destroy (dialog);
  }
}





void select_domain_ccc(void)
{
  if (connected_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGSTILLCONNECTED], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new (curlang_ccc[LANGDOMAIN],
                                          GTK_WINDOW(main_window_ccc),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      char *filename;
      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      fprintf (stdout, "%s\n", filename);
      load_domain_file_ccc(filename);
      g_free (filename);
    }
    gtk_widget_destroy (dialog);
  }
}





void add_domain_ccc(void)
{
  if (connected_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGSTILLCONNECTED], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else if (1 && !do_domain_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNODOMAIN], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new (curlang_ccc[LANGADDDOMAINFILE],
                                          GTK_WINDOW(main_window_ccc),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      char *filename;
      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      fprintf (stdout, "%s\n", filename);
      //load_domain_file_ccc(filename);
      add_domain_file_ccc(filename);
      g_free (filename);
    }
    gtk_widget_destroy (dialog);
  }
}





// Get the selected filename and load it
static void load_log_file_ccc( char *log_file )
{
  gtk_label_set_text(GTK_LABEL(main_label), "");
  //g_print ("%s\n", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
  log_file_ccc = malloc (1024);
  memset (log_file_ccc, 0, 1024);
  strncpy (log_file_ccc, log_file, 1024);
  printf ("%s\n",  log_file_ccc);

  // initialize memory
  return_value_ccc = initialize_logfile_memory_ccc();
  if (return_value_ccc != 0)
  {
    exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
  }

  else
  {
    int ret = read_log_file_ccc(log_file_ccc);
    int ret2 = 0;
    if (ret != 0)
    {
      strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGLOADERR], sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
    else
    {
      check_log_size_ccc = 0;
      ret = check_log_ccc();
      // if only same status then repair log automatically
      if (ret == 0x10)
      {
        ret2 = check_and_repair_log_ccc();
        clear_error_message_ccc();
        ret = check_log_ccc();
      }
      check_log_size_ccc = 1;
      if (ret != 0 || ret2 != 0)
      {
        strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGCHECKERR], sizeof(tempmessage_ccc));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGWARN], 1);
        clear_error_message_ccc();
      }

      project_chosen_ccc = 1;
      new_project_ccc = 0;
      data_read_from_log_ccc = 1;
      clear_destination_ccc();
      clear_source_ccc();
      set_initial_data_from_log_ccc(new_project_ccc);
      logfile_changed_ccc = true;
      update_display_ccc(0);
      //gtk_label_set_text(GTK_LABEL(main_label), display_message_ccc);
    }
  }
}





// Get the selected ddrescue filename and load it
static void load_ddrescue_log_file_ccc( char *log_file )
{
  gtk_label_set_text(GTK_LABEL(main_label), "");
  //g_print ("%s\n", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
  ddilog_file_ccc = malloc (1024);
  memset (ddilog_file_ccc, 0, 1024);
  strncpy (ddilog_file_ccc, log_file, 1024);
  printf ("%s\n",  ddilog_file_ccc);

  // initialize memory
  return_value_ccc = initialize_logfile_memory_ccc();
  if (return_value_ccc != 0)
  {
    exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
  }

  else
  {
    int ret = read_ddrescue_log_ccc(ddilog_file_ccc);
    int ret2 = 0;
    if (ret != 0)
    {
      strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGLOADERR], sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
    else
    {
      importdd_ccc = true;
      ret = check_log_ccc();
      if (ret)
      {
        snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "There were errors found in the progress log file\n");
        message_now_ccc(tempmessage_ccc);
        ret2 = check_and_repair_log_ccc();
        snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "Checking the progress log file again to make sure it was fixed correctly.\n");
        message_now_ccc(tempmessage_ccc);
        ret = check_log_ccc();
        if (ret != 0 || ret2 != 0)
        {
          strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGCHECKERR], sizeof(tempmessage_ccc));
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGWARN], 1);
          clear_error_message_ccc();
        }
      }
      importdd_ccc = false;

      project_chosen_ccc = 1;
      new_project_ccc = 0;
      clear_destination_ccc();
      clear_source_ccc();
      set_initial_data_from_log_ccc(new_project_ccc);
      logfile_changed_ccc = true;
      update_display_ccc(0);
      //gtk_label_set_text(GTK_LABEL(main_label), display_message_ccc);
    }
  }
}





// Get the selected domain and load it
static void load_domain_file_ccc( char *log_file )
{
  gtk_label_set_text(GTK_LABEL(main_label), "");
  //g_print ("%s\n", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
  clear_domain_ccc();
  strncpy (domain_file_ccc, log_file, 1024);
  printf ("%s\n",  domain_file_ccc);

  // initialize memory
  return_value_ccc = initialize_domainfile_memory_ccc();
  if (return_value_ccc != 0)
  {
    exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
  }

  else
  {
    int ret = read_domain_file_ccc(domain_file_ccc);
    if (ret < 0)
    {
      strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGLOADERR], sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      do_domain_ccc = false;
    }
    else
    {
      do_domain_ccc = true;
      update_display_ccc(0);
      //gtk_label_set_text(GTK_LABEL(main_label), display_message_ccc);
    }
  }
}





// Get the selected domain and load it
static void add_domain_file_ccc( char *log_file )
{
  gtk_label_set_text(GTK_LABEL(main_label), "");
  //g_print ("%s\n", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
  printf ("%s\n",  log_file);

  int ret = read_domain_add_file_ccc(log_file);
  if (ret < 0)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGLOADERR], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  update_display_ccc(0);
}





void clear_domain_ccc(void)
{
  if (connected_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGSTILLCONNECTED], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    initialize_domainfile_memory_ccc();
    if (domain_file_ccc != NULL)
    {
      free(domain_file_ccc);
    }
    domain_file_ccc = malloc (1024);
    memset (domain_file_ccc, 0, 1024);
    do_domain_ccc = false;
  }
  update_display_ccc(0);
}





// Get the selected filename and set it
static void new_log_file_ccc( char *log_file )
{
  log_file_ccc = malloc (1024);
  memset (log_file_ccc, 0, 1024);
  strncpy (log_file_ccc, log_file, 1024);
  printf ("%s\n",  log_file_ccc);

  clear_domain_ccc();
  strncpy (domain_file_ccc, log_file_ccc, 1016);
  strcat (domain_file_ccc, ".domain");
  remove(domain_file_ccc);
  clear_domain_ccc();

  // initialize memory
  return_value_ccc = initialize_logfile_memory_ccc();
  if (return_value_ccc != 0)
  {
    exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
  }
  project_chosen_ccc = 1;
  new_project_ccc = 1;
  data_read_from_log_ccc = 0;
  enable_extended_analyze_ccc = false;
  clear_destination_ccc();
  clear_source_ccc();
  clear_variables_for_new_project_ccc();
  initialize_new_log_ccc();
  logfile_changed_ccc = true;
  set_initial_data_from_log_ccc(new_project_ccc);
  logfile_changed_ccc = true;
  update_display_ccc(0);
  //gtk_label_set_text(GTK_LABEL(main_label), display_message_ccc);
}





static void new_domain_file_ccc( char *log_file )
{
  clear_domain_ccc();
  strncpy (domain_file_ccc, log_file, 1024);
  printf ("%s\n",  domain_file_ccc);

  // initialize memory
  return_value_ccc = initialize_domainfile_memory_ccc();
  if (return_value_ccc != 0)
  {
    exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
  }
  initialize_domain_log_ccc();
  do_domain_ccc = true;
  domainfile_changed_ccc = true;
  update_display_ccc(0);
  //gtk_label_set_text(GTK_LABEL(main_label), display_message_ccc);
}





// open choose source dialog window
void choose_source_ccc(void)
{
  usb_lun_set_ccc = 0;
  bool use_rebuild_assist_bak = use_rebuild_assist_ccc;
  use_rebuild_assist_ccc = false;
  if (!project_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNOPROJECT], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else if (connected_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGSTILLCONNECTED], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else if (memory_failed_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGBUFFERADDRESSRANGE], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else if (copy_image_mode_ccc)
  {
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new (curlang_ccc[LANGCHOOSEIMAGE],
                                          GTK_WINDOW(main_window_ccc),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      char *filename;
      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      fprintf (stdout, "%s\n", filename);
      free (disk_1_ccc);
      disk_1_ccc = malloc (strlen(filename)+2);
      strlcpy (disk_1_ccc, filename, sizeof(disk_1_ccc));
      g_free (filename);
      strcpy (current_source_model_ccc, "");
      strcpy (current_source_serial_ccc, "");
      update_display_ccc(0);

      if (!connect_source_disk_ccc() && !process_source_ccc() && !check_and_adjust_variables_after_choosing_source_ccc(new_project_ccc))
      {
        source_chosen_ccc = 1;
        if (new_project_ccc)
        {
          initialize_new_log_ccc();
          logfile_changed_ccc = true;
          set_initial_data_from_log_ccc(new_project_ccc);
          logfile_changed_ccc = true;
        }
        clear_domain_ccc();
        strncpy (domain_file_ccc, log_file_ccc, 1016);
        strcat (domain_file_ccc, ".domain");
        printf ("%s\n",  domain_file_ccc);
        if( access( domain_file_ccc, F_OK ) != -1 )
        {
          int ret = read_domain_file_ccc(domain_file_ccc);
          if (ret < 0)
          {
            strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGLOADERR], sizeof(tempmessage_ccc));
            message_error_ccc(tempmessage_ccc);
            print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
            clear_error_message_ccc();
            clear_domain_ccc();
            //initialize_domainfile_memory_ccc();
            //initialize_domain_log_ccc();
          }
        }
        else
        {
          initialize_domain_log_ccc();
          //domainfile_changed_ccc = true;
        }
      }
      else
      {
        fprintf (stdout, "error selecting source\n");
        clear_source_ccc();
        strcpy (current_source_model_ccc, "");
        strcpy (current_source_serial_ccc, "");
        snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s", curlang_ccc[LANGSOURCEERROR]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
      }
      release_devices_ccc();
      update_display_ccc(0);
      //gtk_label_set_text(GTK_LABEL(main_label), display_message_ccc);
    }
    gtk_widget_destroy (dialog);
  }
  else
  {
    if (memory_used_ccc)
    {
      initialize_memory_ccc();
    }
    find_all_devices_ccc();

    choose_source_dialog_window_ccc = gtk_dialog_new_with_buttons (curlang_ccc[LANGSOURCE], GTK_WINDOW(main_window_ccc), GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL);
    //gtk_window_set_title (GTK_WINDOW (choose_source_dialog_window_ccc), "GtkScrolledWindow example");
    gtk_container_set_border_width (GTK_CONTAINER (choose_source_dialog_window_ccc), 0);
    gtk_widget_set_size_request (choose_source_dialog_window_ccc, 600, 400);

    choose_source_scrolled_window_ccc = gtk_scrolled_window_new (NULL, NULL);
    gtk_container_set_border_width (GTK_CONTAINER (choose_source_scrolled_window_ccc), 10);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (choose_source_scrolled_window_ccc), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG(choose_source_dialog_window_ccc)->vbox), choose_source_scrolled_window_ccc, TRUE, TRUE, 0);

    choose_source_vbox_ccc = gtk_vbox_new (FALSE, 0);
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (choose_source_scrolled_window_ccc), choose_source_vbox_ccc);

    GtkWidget *button[MAX_DEVICES];

    if (ahci_mode_ccc)
    {
      int i;
      for (i = 0; i < device_count_ccc; i++)
      {
        char button_label[MAX_BUTTON_LABEL_SIZE] = "";
        if (verbose_ccc & DEBUG6)
        {
          snprintf (button_label, sizeof(button_label), "%s %s %s %s %llx %d %llx (%lld) %s %s", device_driver_ccc[i], device_bus_ccc[i], device_reference_ccc[i], device_name_ccc[i], hba_address_ccc[i], port_number_ccc[i], port_address_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
        }
        else if (verbose_ccc & DEBUG5)
        {
          snprintf(button_label, sizeof(button_label), "%s %s %llx %d %llx (%lld) %s %s", device_reference_ccc[i], device_name_ccc[i], hba_address_ccc[i], port_number_ccc[i], port_address_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
        }
        else
        {
          snprintf(button_label, sizeof(button_label), "%s %s (%lld) %s %s", device_reference_ccc[i], device_name_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
        }
        button[i] = gtk_button_new_with_label(button_label);
        gtk_button_set_alignment(GTK_BUTTON(button[i]), 0, .5);
        g_signal_connect(button[i], "clicked", G_CALLBACK(get_source_from_button_ccc), GINT_TO_POINTER(i) );
      }
    }
    else if (direct_mode_ccc)
    {
      int i;
      for (i = 0; i < device_count_ccc; i++)
      {
        char button_label[MAX_BUTTON_LABEL_SIZE] = "";
        if (verbose_ccc & DEBUG6)
        {
          snprintf(button_label, sizeof(button_label), "%s %s %s %s %llx %llx %llx %d (%lld) %s %s", device_driver_ccc[i], device_bus_ccc[i], device_reference_ccc[i], device_name_ccc[i], reg_address_ccc[i], control_address_ccc[i], bus_address_ccc[i], device_select_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
        }
        else if (verbose_ccc & DEBUG5)
        {
          snprintf(button_label, sizeof(button_label), "%s %s %llx %llx %llx %d (%lld) %s %s", device_reference_ccc[i], device_name_ccc[i], reg_address_ccc[i], control_address_ccc[i], bus_address_ccc[i], device_select_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
        }
        else
        {
          snprintf(button_label, sizeof(button_label), "%s %s (%lld) %s %s", device_reference_ccc[i], device_name_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
        }
        button[i] = gtk_button_new_with_label(button_label);
        gtk_button_set_alignment(GTK_BUTTON(button[i]), 0, .5);
        g_signal_connect(button[i], "clicked", G_CALLBACK(get_source_from_button_ccc), GINT_TO_POINTER(i) );
      }
    }
    else if (usb_mode_ccc)
    {
      int i;
      for (i = 0; i < usb_device_count_ccc; i++)
      {
        char button_label[MAX_BUTTON_LABEL_SIZE] = "";
        if (verbose_ccc & DEBUG6)
        {
          snprintf(button_label, sizeof(button_label), "%d:%d %04x:%04x %02x %02x %d %s %s %s\n", usb_bus_real_number_ccc[i], usb_device_number_ccc[i], usb_vendor_id_ccc[i], usb_product_id_ccc[i], usb_bulk_in_endpoint_ccc[i], usb_bulk_out_endpoint_ccc[i], usb_mass_storage_ccc[i], usb_vendor_string_ccc[i], usb_product_string_ccc[i], usb_extra_id_string_ccc[i]);
        }
        else if (verbose_ccc & DEBUG5)
        {
          snprintf(button_label, sizeof(button_label), "%d:%d %04x:%04x %s %s %s\n", usb_bus_real_number_ccc[i], usb_device_number_ccc[i], usb_vendor_id_ccc[i], usb_product_id_ccc[i], usb_vendor_string_ccc[i], usb_product_string_ccc[i], usb_extra_id_string_ccc[i]);
        }
        else
        {
          snprintf(button_label, sizeof(button_label), "%d:%d %04x:%04x %s %s\n", usb_bus_real_number_ccc[i], usb_device_number_ccc[i], usb_vendor_id_ccc[i], usb_product_id_ccc[i], usb_vendor_string_ccc[i], usb_product_string_ccc[i]);
        }
        button[i] = gtk_button_new_with_label(button_label);
        gtk_button_set_alignment(GTK_BUTTON(button[i]), 0, .5);
        g_signal_connect(button[i], "clicked", G_CALLBACK(get_usb_from_button_ccc), GINT_TO_POINTER(i) );
      }
    }
    else
    {
      int i;
      for (i = 0; i < device_count_ccc; i++)
      {
        char button_label[MAX_BUTTON_LABEL_SIZE] = "";
        snprintf(button_label, sizeof(button_label), "%s (%lld) %s %s", drive_list_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
        button[i] = gtk_button_new_with_label(button_label);
        gtk_button_set_alignment(GTK_BUTTON(button[i]), 0, .5);
        g_signal_connect(button[i], "clicked", G_CALLBACK(get_source_from_button_ccc), GINT_TO_POINTER(i) );
      }
    }


    int i;
    if (usb_mode_ccc)
    {
      for (i = 0; i < usb_device_count_ccc; i++)
      {
        if ( (verbose_ccc & DEBUG6) || (verbose_ccc & DEBUG5) || usb_mass_storage_ccc[i])
        {
          gtk_box_pack_start (GTK_BOX (choose_source_vbox_ccc), button[i], FALSE, FALSE, 0);
        }
      }

    }
    else
    {
      for (i = 0; i < device_count_ccc; i++)
      {
        gtk_box_pack_start (GTK_BOX (choose_source_vbox_ccc), button[i], FALSE, FALSE, 0);
      }

    }

    gtk_widget_show_all (choose_source_dialog_window_ccc);
    gint result = gtk_dialog_run (GTK_DIALOG (choose_source_dialog_window_ccc));
    gtk_widget_destroy (choose_source_dialog_window_ccc);

    if (result == GTK_RESPONSE_ACCEPT)
    {
      //fprintf (stdout, "OK\n");
      // TODO it crashes here on scsi devices - fixed

      // disable the window while it is processing the source
      gtk_widget_set_sensitive (GTK_WIDGET(main_window_ccc), FALSE);

      clear_source_ccc();
      int ret = choose_device_ccc();
      if (ahci_mode_ccc)
      {
        hba_reset_address_ccc = hba_base_address_ccc;
        port_reset_address_ccc = port_base_address_ccc;
      }
      if (ret)
      {
        if (ret == -100)
        {
          clear_source_ccc();
          strcpy (current_source_model_ccc, "");
          strcpy (current_source_serial_ccc, "");
          snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s", curlang_ccc[LANGSOURCEERROR]);
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
        }
        else if (ret == -200)
        {
          fprintf (stdout, "error selecting source, ret=%d\n", ret);
          clear_source_ccc();
          strcpy (current_source_model_ccc, "");
          strcpy (current_source_serial_ccc, "");
          snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s", curlang_ccc[LANGSOURCEERROR]);
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
        }
        else
        {
          fprintf (stdout, "error selecting source, ret=%d\n", ret);
          clear_source_ccc();
          strcpy (current_source_model_ccc, "");
          strcpy (current_source_serial_ccc, "");
          snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s", curlang_ccc[LANGSOURCEERROR]);
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
        }
      }
      else if (!connect_source_disk_ccc() && !process_source_ccc() && !check_and_adjust_variables_after_choosing_source_ccc(new_project_ccc))
      {
        source_chosen_ccc = 1;
        if (new_project_ccc)
        {
          initialize_new_log_ccc();
          logfile_changed_ccc = true;
          set_initial_data_from_log_ccc(new_project_ccc);
          logfile_changed_ccc = true;
        }
        clear_domain_ccc();
        strncpy (domain_file_ccc, log_file_ccc, 1016);
        strcat (domain_file_ccc, ".domain");
        printf ("%s\n",  domain_file_ccc);
        if( access( domain_file_ccc, F_OK ) != -1 )
        {
          int ret = read_domain_file_ccc(domain_file_ccc);
          if (ret < 0)
          {
            strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGLOADERR], sizeof(tempmessage_ccc));
            message_error_ccc(tempmessage_ccc);
            print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
            clear_error_message_ccc();
            clear_domain_ccc();
            //initialize_domainfile_memory_ccc();
            //initialize_domain_log_ccc();
          }
        }
        else
        {
          initialize_domain_log_ccc();
          //domainfile_changed_ccc = true;
        }
      }
      else
      {
        fprintf (stdout, "error selecting source\n");
        clear_source_ccc();
        strcpy (current_source_model_ccc, "");
        strcpy (current_source_serial_ccc, "");
        snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s", curlang_ccc[LANGSOURCEERROR]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
      }
      release_devices_ccc();
      update_display_ccc(0);
      //gtk_label_set_text(GTK_LABEL(main_label), display_message_ccc);

      // re-enable the window
      gtk_widget_set_sensitive (GTK_WIDGET(main_window_ccc), TRUE);
    }
  }
  use_rebuild_assist_ccc = use_rebuild_assist_bak;
}





void get_source_from_button_ccc (GtkWidget *w, gpointer data)
{
  (void) w;
  new_source_ccc = GPOINTER_TO_INT( data );
  g_print ("selection=%d  \n", new_source_ccc);
}





void choose_destination_drive_ccc(void)
{
  destination_is_generic_ccc = false;
  choose_destination_ccc();
}

void choose_destination_block_ccc(void)
{
  destination_is_generic_ccc = true;
  choose_destination_ccc();
}



// open choose destination dialog window
void choose_destination_ccc(void)
{
  if (!project_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNOPROJECT], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else if (connected_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGSTILLCONNECTED], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    bool direct_mode_bak = direct_mode_ccc;
    bool ahci_mode_bak = ahci_mode_ccc;
    bool ata_passthrough_bak = ata_passthrough_ccc;
    bool scsi_passthrough_bak = scsi_passthrough_ccc;
    bool generic_mode_ccc_bak = generic_mode_ccc;
    bool usb_mode_bak = usb_mode_ccc;

    direct_mode_ccc = false;
    ahci_mode_ccc = false;
    ata_passthrough_ccc = false;
    scsi_passthrough_ccc = !destination_is_generic_ccc;
    generic_mode_ccc = destination_is_generic_ccc;
    usb_mode_ccc = false;
    if (memory_used_ccc)
    {
      initialize_memory_ccc();
    }
    find_all_devices_ccc();
    direct_mode_ccc = direct_mode_bak;
    ahci_mode_ccc = ahci_mode_bak;
    ata_passthrough_ccc = ata_passthrough_bak;
    scsi_passthrough_ccc = scsi_passthrough_bak;
    generic_mode_ccc = generic_mode_ccc_bak;
    usb_mode_ccc = usb_mode_bak;

    choose_destination_dialog_window_ccc = gtk_dialog_new_with_buttons (curlang_ccc[LANGDESTINATION], GTK_WINDOW(main_window_ccc), GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL);
    //gtk_window_set_title (GTK_WINDOW (choose_source_dialog_window_ccc), "GtkScrolledWindow example");
    gtk_container_set_border_width (GTK_CONTAINER (choose_destination_dialog_window_ccc), 0);
    gtk_widget_set_size_request (choose_destination_dialog_window_ccc, 600, 400);

    choose_destination_scrolled_window_ccc = gtk_scrolled_window_new (NULL, NULL);
    gtk_container_set_border_width (GTK_CONTAINER (choose_destination_scrolled_window_ccc), 10);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (choose_destination_scrolled_window_ccc), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG(choose_destination_dialog_window_ccc)->vbox), choose_destination_scrolled_window_ccc, TRUE, TRUE, 0);

    choose_destination_vbox_ccc = gtk_vbox_new (FALSE, 0);
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (choose_destination_scrolled_window_ccc), choose_destination_vbox_ccc);

    GtkWidget *button[MAX_DEVICES];


    int i;
    for (i = 0; i < device_count_ccc; i++)
    {
      char button_label[MAX_BUTTON_LABEL_SIZE] = "";
      snprintf(button_label, sizeof(button_label), "%s (%lld) %s %s", drive_list_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
      button[i] = gtk_button_new_with_label(button_label);
      gtk_button_set_alignment(GTK_BUTTON(button[i]), 0, .5);
      g_signal_connect(button[i], "clicked", G_CALLBACK(get_destination_from_button_ccc), GINT_TO_POINTER(i) );
    }


    for (i = 0; i < device_count_ccc; i++)
    {
      gtk_box_pack_start (GTK_BOX (choose_destination_vbox_ccc), button[i], FALSE, FALSE, 0);
    }

    gtk_widget_show_all (choose_destination_dialog_window_ccc);
    gint result = gtk_dialog_run (GTK_DIALOG (choose_destination_dialog_window_ccc));
    gtk_widget_destroy (choose_destination_dialog_window_ccc);

    if (result == GTK_RESPONSE_ACCEPT)
    {
      //fprintf (stdout, "OK\n");
      generic_mode_ccc = destination_is_generic_ccc;
      int ret = choose_target_ccc();
      generic_mode_ccc = generic_mode_ccc_bak;
      if (ret)
      {
        fprintf (stdout, "error selecting destination, ret=%d\n", ret);
        clear_destination_ccc();
        snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s", curlang_ccc[LANGDESTINATIONERROR]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
      }
      else
      {
        destination_chosen_ccc = 1;
        destination_size_valid_ccc = 1;
        logfile_changed_ccc = true;

        if (driver_only_ccc)
        {
          clear_domain_ccc();
          strncpy (domain_file_ccc, log_file_ccc, 1016);
          strcat (domain_file_ccc, ".domain");
          printf ("%s\n",  domain_file_ccc);
          if( access( domain_file_ccc, F_OK ) != -1 )
          {
            int ret = read_domain_file_ccc(domain_file_ccc);
            if (ret < 0)
            {
              strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGLOADERR], sizeof(tempmessage_ccc));
              message_error_ccc(tempmessage_ccc);
              print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
              clear_error_message_ccc();
              clear_domain_ccc();
              //initialize_domainfile_memory_ccc();
              //initialize_domain_log_ccc();
            }
          }
          else
          {
            initialize_domain_log_ccc();
            domainfile_changed_ccc = true;
          }
        }

      }
      update_display_ccc(0);
      //gtk_label_set_text(GTK_LABEL(main_label), display_message_ccc);
    }
  }
}





void get_destination_from_button_ccc (GtkWidget *w, gpointer data)
{
  (void) w;
  new_destination_ccc = GPOINTER_TO_INT( data );
  g_print ("selection=%d  \n", new_destination_ccc);
}





void choose_image_ccc (void)
{
  if (!project_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNOPROJECT], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else if (connected_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGSTILLCONNECTED], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new (curlang_ccc[LANGCHOOSEIMAGE],
                                          GTK_WINDOW(main_window_ccc),
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
    //gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      int confirmed = 1;
      char *filename;
      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      if ( access(filename, F_OK ) == 0 )
      {
        snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "   %s   \n%s", filename, curlang_ccc[LANGCONFIRMIMAGEFILE]);
        if (!open_confirmation_dialog_ccc (tempmessage_ccc))
        {
          confirmed = 0;
        }
      }
      if (confirmed)
      {
        fprintf (stdout, "%s\n", filename);
        free(disk_2_ccc);
        disk_2_ccc = malloc (strlen(filename)+2);
        strlcpy (disk_2_ccc, filename, sizeof(disk_2_ccc));
        destination_chosen_ccc = 1;
        destination_size_valid_ccc = 0;
        strcpy (current_destination_model_ccc, "");
        strcpy (current_destination_serial_ccc, "");
        logfile_changed_ccc = true;

        if (driver_only_ccc)
        {
          clear_domain_ccc();
          strncpy (domain_file_ccc, log_file_ccc, 1016);
          strcat (domain_file_ccc, ".domain");
          printf ("%s\n",  domain_file_ccc);
          if( access( domain_file_ccc, F_OK ) != -1 )
          {
            int ret = read_domain_file_ccc(domain_file_ccc);
            if (ret < 0)
            {
              strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGLOADERR], sizeof(tempmessage_ccc));
              message_error_ccc(tempmessage_ccc);
              print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
              clear_error_message_ccc();
              clear_domain_ccc();
              //initialize_domainfile_memory_ccc();
              //initialize_domain_log_ccc();
            }
          }
          else
          {
            initialize_domain_log_ccc();
            domainfile_changed_ccc = true;
          }
        }

        update_display_ccc(0);
        //gtk_label_set_text(GTK_LABEL(main_label), display_message_ccc);
      }
      g_free (filename);
      gtk_widget_destroy (dialog);
    }
  }
}





void choose_null_ccc (void)
{
  if (!project_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNOPROJECT], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else if (connected_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGSTILLCONNECTED], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
      fprintf (stdout, "/dev/null\n");
      free (disk_2_ccc);
      disk_2_ccc = strdup("/dev/null");
      strlcpy (current_destination_model_ccc, curlang_ccc[LANGNODESTINATION], sizeof(current_destination_model_ccc));
      strcpy (current_destination_serial_ccc, "");
      logfile_changed_ccc = true;
      update_display_ccc(0);
      //gtk_label_set_text(GTK_LABEL(main_label), display_message_ccc);
      destination_chosen_ccc = 1;
      destination_size_valid_ccc = 0;
  }
}





void connect_devices_ccc (void)
{
  int relaycheck = 1;
  if ( (activate_primary_relay_on_error_ccc || power_cycle_primary_relay_ccc) && !(fill_mode_ccc || driver_only_ccc) && !usbr1_chosen_ccc)
  {
    relaycheck = 0;
  }
  if (memory_failed_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGBUFFERADDRESSRANGE], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else if (relaycheck && project_chosen_ccc && destination_chosen_ccc && (fill_mode_ccc || driver_only_ccc) )
  {
    check_log_size_ccc = 0;
    // check log and if only same status then repair automatically
    int ret = check_log_ccc();
    int ret2 = 0;
    if (ret == 0x10)
    {
      clear_error_message_ccc();
      ret2 = check_and_repair_log_ccc();
      ret = check_log_ccc();
    }
    if (new_project_ccc && !fill_mode_ccc)
    {
      strlcpy (tempmessage_ccc, curlang_ccc[LANGMISSINGSOURCE], sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      set_disconnected_ccc();
    }
    else if (ret || ret2)
    {
      strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGCHECKERR], sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      set_disconnected_ccc();
    }
    else if (driver_only_ccc && !driver_installed_ccc)
    {
      strlcpy (tempmessage_ccc, curlang_ccc[LANGMISSINGDRIVER], sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      set_disconnected_ccc();
    }
    else
    {
      if (open_target_destination_ccc())
      {
        release_devices_ccc();
        strlcpy (tempmessage_ccc, curlang_ccc[LANGCONNECTIONERR], sizeof(tempmessage_ccc));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        set_disconnected_ccc();
      }
      else
      {
        //strlcpy (tempmessage_ccc, curlang_ccc[LANGCONNECTED], sizeof(tempmessage_ccc));
        //message_error_ccc(tempmessage_ccc);
        //print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGSUCCESS], 0);
        //clear_error_message_ccc();
        check_and_adjust_variables_after_connection_ccc();
        set_connected_ccc();
      }
    }
  }
  else if (relaycheck && project_chosen_ccc && source_chosen_ccc && destination_chosen_ccc)
  {
    check_log_size_ccc = 1;
    // check log and if only same status then repair automatically
    int ret = check_log_ccc();
    int ret2 = 0;
    if (ret == 0x10)
    {
      clear_error_message_ccc();
      ret2 = check_and_repair_log_ccc();
      ret = check_log_ccc();
    }
    if (check_same_device_ccc())
    {
      strlcpy (tempmessage_ccc, curlang_ccc[LANGSAMEDEVICE], sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      set_disconnected_ccc();
    }
    else if (ret || ret2)
    {
      strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGCHECKERR], sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      set_disconnected_ccc();
    }
    else
    {
      if (connect_source_disk_ccc() || open_target_destination_ccc())
      {
        release_devices_ccc();
        strlcpy (tempmessage_ccc, curlang_ccc[LANGCONNECTIONERR], sizeof(tempmessage_ccc));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        set_disconnected_ccc();
      }
      else
      {
        //strlcpy (tempmessage_ccc, curlang_ccc[LANGCONNECTED], sizeof(tempmessage_ccc));
        //message_error_ccc(tempmessage_ccc);
        //print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGSUCCESS], 0);
        //clear_error_message_ccc();
        if (!enable_scsi_write_ccc && destination_size_valid_ccc && source_total_size_ccc > target_total_size_ccc)
        {
          fprintf (stdout, "Warning! Destination is smaller than souce.\n");
          fprintf (stdout, "Source size= %lld\n", source_total_size_ccc);
          fprintf (stdout, "Target size= %lld\n", target_total_size_ccc);
          strlcpy (tempmessage_ccc, curlang_ccc[LANGDESTINATIONTOOSMALL], sizeof(tempmessage_ccc));
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGWARN], 1);
          clear_error_message_ccc();
        }
        check_and_adjust_variables_after_connection_ccc();
        set_connected_ccc();
        new_project_ccc = 0;
      }
    }
  }
  else
  {
    if (!project_chosen_ccc)
    {
      strlcpy (tempmessage_ccc, curlang_ccc[LANGMISSINGPROJECT], sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
    }
    if (!source_chosen_ccc && !(fill_mode_ccc || driver_only_ccc))
    {
      strlcpy (tempmessage_ccc, curlang_ccc[LANGMISSINGSOURCE], sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
    }
    if (!destination_chosen_ccc)
    {
      strlcpy (tempmessage_ccc, curlang_ccc[LANGMISSINGDESTINATION], sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
    }
    strlcpy (tempmessage_ccc, curlang_ccc[LANGDONTCONNECT], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    if (!relaycheck)
    {
      strlcpy (tempmessage_ccc, curlang_ccc[LANGNORELAYCHOSEN], sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
  }
  ata_status_ccc = 0;
  ata_error_ccc = 0;
  update_display_ccc(0);
}





void start_display_status_update_timer_ccc (void)
{
  //g_print ("start=%d timer=%d\n", display_status_timer_ccc, display_status_timer_running_ccc);    //debug
  if (direct_mode_ccc && !display_status_timer_running_ccc)
  {
    display_status_timer_ccc = gtk_timeout_add (500, display_status_update_action_ccc, NULL);
    display_status_timer_running_ccc = 1;
  }
}



void stop_display_status_update_timer_ccc (void)
{
  if (display_status_timer_running_ccc)
  {
    gtk_timeout_remove (display_status_timer_ccc);
    display_status_timer_running_ccc = 0;
  }
}



gint display_status_update_action_ccc (gpointer data)
{
  (void) data;
  if (fill_mode_ccc || !connected_ccc || running_clone_ccc)
  {
    display_status_timer_running_ccc = 0;
    return 0;
  }
  //g_print ("timer\n");    //debug
  refresh_status_ccc(current_disk_ccc);
  update_gui_status_buttons_ccc();
  return 1;
}





void disconnect_devices_ccc (void)
{
  release_devices_ccc();
  connected_ccc = 0;
  //strlcpy (tempmessage_ccc, curlang_ccc[LANGDISCONNECTED], sizeof(tempmessage_ccc));
  //message_error_ccc(tempmessage_ccc);
  //print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGSUCCESS], 0);
  //clear_error_message_ccc();
  set_disconnected_ccc();
  ata_status_ccc = 0;
  ata_error_ccc = 0;
  update_display_ccc(0);
}





void set_connected_ccc (void)
{
  if (driver_mode_ccc && (strcmp(disk_2_ccc, "/dev/null") == 0) )
  {
    clear_error_message_ccc();
    strlcpy (tempmessage_ccc, curlang_ccc[LANGWARNINGNULLDRIVER], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGWARN], 0);
    clear_error_message_ccc();
  }
  connected_ccc = 1;
  gtk_widget_set_sensitive (GTK_WIDGET(connect_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(disconnect_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(start_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(modemi_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(drivesmi_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(primaryrelaymi_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(chooseprimaryrelaymi_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(soft_reset_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(hard_reset_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(clone_mode_button_ccc), driver_only_ccc ? FALSE : TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(analyze_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(smart_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(activate_primary_relay_button_main_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(deactivate_primary_relay_button_main_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(analyze_long_button_ccc), TRUE);
  if (driver_installed_ccc)
  {
    set_driver_mode_button_status_ccc(TRUE);
  }
  start_display_status_update_timer_ccc();
  stop_signal_ccc = false;
  process_dma_mode_ccc();
}



void set_disconnected_ccc (void)
{
  connected_ccc = 0;
  gtk_widget_set_sensitive (GTK_WIDGET(connect_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(disconnect_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(start_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(stop_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(analyze_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(analyze_long_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(smart_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(modemi_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(drivesmi_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(primaryrelaymi_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(chooseprimaryrelaymi_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(activate_primary_relay_button_main_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(deactivate_primary_relay_button_main_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(soft_reset_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(hard_reset_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(clone_mode_button_ccc), FALSE);
  set_driver_mode_button_status_ccc(FALSE);
  stop_display_status_update_timer_ccc();
}



void clear_source_ccc (void)
{
  source_chosen_ccc = 0;
  if (disk_1_ccc != NULL)
  {
    disk_1_ccc = NULL;
  }
  strcpy (current_source_model_ccc, "");
  strcpy (current_source_serial_ccc, "");
  source_total_size_ccc = 0;
}



void clear_destination_ccc (void)
{
  destination_chosen_ccc = 0;
  if (disk_2_ccc != NULL)
  {
    free(disk_2_ccc);
    disk_2_ccc = NULL;
  }
  strcpy (current_destination_model_ccc, "");
  strcpy (current_destination_serial_ccc, "");
}






void update_gui_display_ccc (void)
{
  gtk_label_set_text(GTK_LABEL(main_label), "");
  //gtk_label_set_text(GTK_LABEL(main_label), display_message_ccc);

  gtk_label_set_text(GTK_LABEL (data_project_ccc), display_output_ccc.logfile);
  gtk_label_set_text(GTK_LABEL (data_domain_ccc), display_output_ccc.domainfile);
  gtk_label_set_text(GTK_LABEL (data_source_ccc), display_output_ccc.source);
  gtk_label_set_text(GTK_LABEL (data_destination_ccc), display_output_ccc.destination);
  gtk_label_set_text(GTK_LABEL(data_totallba_ccc), display_output_ccc.totallba);
  gtk_label_set_text(GTK_LABEL(data_lbatoread_ccc), display_output_ccc.lbatoread);
  gtk_label_set_text(GTK_LABEL(data_domainsize_ccc), display_output_ccc.domainsize);
  gtk_label_set_text(GTK_LABEL(data_runtime_ccc), display_output_ccc.runtime);
  gtk_label_set_text(GTK_LABEL(data_remainingtime_ccc), display_output_ccc.remainingtime);
  gtk_label_set_text(GTK_LABEL(data_currentposition_ccc), display_output_ccc.currentposition);
  gtk_label_set_text(GTK_LABEL(data_currentstatus_ccc), display_output_ccc.currentstatus);
  gtk_label_set_text(GTK_LABEL(data_currentrate_ccc), display_output_ccc.currentrate);
  gtk_label_set_text(GTK_LABEL(data_recentrate_ccc), display_output_ccc.recentrate);
  gtk_label_set_text(GTK_LABEL(data_totalrate_ccc), display_output_ccc.totalrate);
  gtk_label_set_text(GTK_LABEL(data_skipsize_ccc), display_output_ccc.skipsize);
  gtk_label_set_text(GTK_LABEL(data_slowskips_ccc), display_output_ccc.slowskips);
  gtk_label_set_text(GTK_LABEL(data_skips_ccc), display_output_ccc.skips);
  gtk_label_set_text(GTK_LABEL(data_skipruns_ccc), display_output_ccc.skipruns);
  gtk_label_set_text(GTK_LABEL(data_skipresets_ccc), display_output_ccc.skipresets);
  gtk_label_set_text(GTK_LABEL(data_runsize_ccc), display_output_ccc.runsize);
  gtk_label_set_text(GTK_LABEL(data_finished_ccc), display_output_ccc.finished);
  gtk_label_set_text(GTK_LABEL(data_nontried_ccc), display_output_ccc.nontried);
  gtk_label_set_text(GTK_LABEL(data_nontrimmed_ccc), display_output_ccc.nontrimmed);
  gtk_label_set_text(GTK_LABEL(data_nondivided_ccc), display_output_ccc.nondivided);
  gtk_label_set_text(GTK_LABEL(data_nonscraped_ccc), display_output_ccc.nonscraped);
  gtk_label_set_text(GTK_LABEL(data_bad_ccc), display_output_ccc.bad);
  gtk_label_set_text(GTK_LABEL(data_datapreview_ccc), display_output_ccc.datapreview);
  gtk_label_set_text(GTK_LABEL(data_longestreadtime_ccc), display_output_ccc.longestreadtime);
  gtk_label_set_text(GTK_LABEL(data_filled_ccc), display_output_ccc.totalfilled);
  gtk_label_set_text(GTK_LABEL(data_retried_ccc), display_output_ccc.retried);
  gtk_label_set_text(GTK_LABEL(data_retrypasses_ccc), display_output_ccc.retrypasses);
  //gtk_label_set_text(GTK_LABEL(), display_output_ccc.); // template


  update_status_buttons_ccc();
  run_gtk_gui_update_ccc();
}





void update_gui_status_buttons_ccc (void)
{
  update_status_buttons_ccc();
  run_gtk_gui_update_ccc();
}





void run_gtk_gui_update_ccc (void)
{
  while (gtk_events_pending ())
  {
    gtk_main_iteration ();
  }
}





void set_stop_signal_ccc (void)
{
  stop_signal_ccc = true;
}




void set_clone_mode_ccc (void)
{
  if (driver_mode_ccc)
  {
    do_domain_ccc = true;
  }
  driver_mode_ccc = 0;
  gtk_label_set_text(GTK_LABEL(data_drivermode_ccc), curlang_ccc[LANGCLONEMODE]);
  update_display_ccc(0);
}






void set_driver_mode1_ccc (void)
{
  do_domain_ccc = false;
  check_driver_null_condition_ccc();
  driver_mode_ccc = 1;
  gtk_label_set_text(GTK_LABEL(data_drivermode_ccc), curlang_ccc[LANGDRIVERMODE1]);
  update_display_ccc(0);
}

void set_driver_mode2_ccc (void)
{
  do_domain_ccc = false;
  check_driver_null_condition_ccc();
  driver_mode_ccc = 2;
  gtk_label_set_text(GTK_LABEL(data_drivermode_ccc), curlang_ccc[LANGDRIVERMODE2]);
  update_display_ccc(0);
}

void set_driver_mode3_ccc (void)
{
  do_domain_ccc = false;
  check_driver_null_condition_ccc();
  driver_mode_ccc = 3;
  gtk_label_set_text(GTK_LABEL(data_drivermode_ccc), curlang_ccc[LANGDRIVERMODE3]);
  update_display_ccc(0);
}

void set_driver_mode4_ccc (void)
{
  do_domain_ccc = false;
  check_driver_null_condition_ccc();
  driver_mode_ccc = 4;
  gtk_label_set_text(GTK_LABEL(data_drivermode_ccc), curlang_ccc[LANGDRIVERMODE4]);
  update_display_ccc(0);
}

void set_driver_mode5_ccc (void)
{
  do_domain_ccc = false;
  check_driver_null_condition_ccc();
  driver_mode_ccc = 5;
  gtk_label_set_text(GTK_LABEL(data_drivermode_ccc), curlang_ccc[LANGDRIVERMODE5]);
  update_display_ccc(0);
}

void check_driver_null_condition_ccc (void)
{
  // only check if coming from clone mode and destination string is not NULL
  if (disk_2_ccc != NULL && driver_mode_ccc == 0 && (strcmp(disk_2_ccc, "/dev/null") == 0) )
  {
    clear_error_message_ccc();
    strlcpy (tempmessage_ccc, curlang_ccc[LANGWARNINGNULLDRIVER], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGWARN], 0);
    clear_error_message_ccc();
  }
}





void set_driver_mode_button_status_ccc (bool active)
{
  if (driver_installed_ccc)
  {
    if (driver_only_ccc)
    {
      set_driver_mode4_ccc();
      gtk_widget_set_sensitive (GTK_WIDGET(driver_mode4_button_ccc), active);
      gtk_widget_set_sensitive (GTK_WIDGET(agressive_driver_checkbutton_ccc), active);
      gtk_widget_set_sensitive (GTK_WIDGET(driver_mode1_button_ccc), FALSE);
      gtk_widget_set_sensitive (GTK_WIDGET(driver_mode2_button_ccc), FALSE);
      gtk_widget_set_sensitive (GTK_WIDGET(driver_mode3_button_ccc), FALSE);
      gtk_widget_set_sensitive (GTK_WIDGET(driver_mode5_button_ccc), FALSE);

    }
    else
    {
      gtk_widget_set_sensitive (GTK_WIDGET(agressive_driver_checkbutton_ccc), active);
      gtk_widget_set_sensitive (GTK_WIDGET(driver_mode1_button_ccc), active);
      gtk_widget_set_sensitive (GTK_WIDGET(driver_mode2_button_ccc), active);
      gtk_widget_set_sensitive (GTK_WIDGET(driver_mode3_button_ccc), active);
      gtk_widget_set_sensitive (GTK_WIDGET(driver_mode4_button_ccc), active);
      gtk_widget_set_sensitive (GTK_WIDGET(driver_mode5_button_ccc), active);
    }
  }
  else
  {
    gtk_widget_set_sensitive (GTK_WIDGET(driver_mode4_button_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(agressive_driver_checkbutton_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(driver_mode1_button_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(driver_mode2_button_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(driver_mode3_button_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(driver_mode5_button_ccc), FALSE);

  }
}





void start_cloning_ccc (void)
{
  int ret = 0;
  stop_display_status_update_timer_ccc();
  stop_signal_ccc = false;
  gtk_widget_set_sensitive (GTK_WIDGET(connect_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(disconnect_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(start_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(analyze_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(analyze_long_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(smart_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(stop_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(topmenubar_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(clone_mode_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(soft_reset_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(hard_reset_button_ccc), FALSE);
  if (driver_mode_ccc)
  {
    set_driver_mode_button_status_ccc(TRUE);
  }
  else
  {
    set_driver_mode_button_status_ccc(FALSE);
  }

  running_clone_ccc = 1;
  memory_used_ccc = 1;
  if (driver_mode_ccc == 0)
  {
    ret = begin_cloning_ccc();
  }
  else
  {
    gtk_label_set_text(GTK_LABEL(label_filled_ccc), curlang_ccc[LANGLASTREADSIZELABEL]);
    ret = begin_driver_ccc();
    gtk_label_set_text(GTK_LABEL(label_filled_ccc), curlang_ccc[LANGFILLEDLABEL]);
  }
  running_clone_ccc = 0;
  update_logfile_ccc(0);
  update_domainfile_ccc(0);

  gtk_widget_set_sensitive (GTK_WIDGET(connect_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(disconnect_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(start_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(stop_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(topmenubar_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(clone_mode_button_ccc), driver_only_ccc ? FALSE : TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(soft_reset_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(hard_reset_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(analyze_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(smart_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(analyze_long_button_ccc), TRUE);
  set_driver_mode_button_status_ccc(TRUE);

  if (ret && ret != STOP_SIGNAL_RETURN_CODE)
  {
    disconnect_devices_ccc();
  }
  start_display_status_update_timer_ccc();
}





void analyze_quick_ccc (void)
{
  enable_extended_analyze_ccc = false;
  start_analyzing_ccc();
}


void analyze_long_ccc (void)
{
  enable_extended_analyze_ccc = true;
  start_analyzing_ccc();
}


void start_analyzing_ccc (void)
{
  int ret = 0;
  stop_display_status_update_timer_ccc();
  stop_signal_ccc = false;
  gtk_widget_set_sensitive (GTK_WIDGET(connect_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(disconnect_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(start_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(analyze_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(analyze_long_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(smart_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(stop_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(topmenubar_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(clone_mode_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(soft_reset_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(hard_reset_button_ccc), FALSE);

  set_driver_mode_button_status_ccc(FALSE);

  memory_used_ccc = 1;

  int extended = 0;
  int sections = MAXANALYZESECTIONS / 4;
  if (enable_extended_analyze_ccc)
  {
    extended = 1;
    sections = MAXANALYZESECTIONS;
  }

  memset (analyze_read_attempts_ccc, 0, sizeof(analyze_read_attempts_ccc));
  memset (analyze_good_reads_ccc, 0, sizeof(analyze_good_reads_ccc));
  memset (analyze_bad_reads_ccc, 0, sizeof(analyze_bad_reads_ccc));
  memset (analyze_slow_reads_ccc, 0, sizeof(analyze_slow_reads_ccc));
  memset (analyze_timeouts_ccc, 0, sizeof(analyze_timeouts_ccc));
  memset (analyze_read_time_ccc, 0, sizeof(analyze_read_time_ccc));
  //memset (analyze_low_time_ccc, 0x7f, sizeof(analyze_low_time_ccc));
  memset (analyze_high_time_ccc, 0, sizeof(analyze_high_time_ccc));
  memset (analyze_text_ccc, 0, sizeof(analyze_text_ccc));
  memset (analyze_slow_position_ccc, 0, sizeof(analyze_slow_position_ccc));
  memset (analyze_slow_size_ccc, 0, sizeof(analyze_slow_size_ccc));
  //memset (analyze_slow_low_ccc, 0x7f, sizeof(analyze_slow_low_ccc));
  memset (analyze_slow_high_ccc, 0, sizeof(analyze_slow_high_ccc));
  analyze_slow_total_reads_ccc = 0;
  int i;
  for (i = 0; i < MAXANALYZESECTIONS; i++)
  {
    analyze_low_time_ccc[i] = 999999999999;
    analyze_slow_low_ccc[i] = 999999999999;
  }
  running_analyze_ccc = 1;
  gtk_label_set_text(GTK_LABEL(label_filled_ccc), curlang_ccc[LANGTOTALGOODBADLABEL]);
  ret = analyze_drive_ccc(sections, extended);
  gtk_label_set_text(GTK_LABEL(label_filled_ccc), curlang_ccc[LANGFILLEDLABEL]);
  running_analyze_ccc = 0;

  long long average_read_time[MAXANALYZESECTIONS];
  memset (average_read_time, 0, sizeof(average_read_time));
  for (i = 0; i < sections; i++)
  {
    // prevent divid by 0 error
    if (analyze_read_attempts_ccc[i] != 0)
    {
      average_read_time[i] = analyze_read_time_ccc[i] / analyze_read_attempts_ccc[i];
    }
  }

  long long total_average_read_time = 0;
  for (i = 0; i < sections; i++)
  {
    total_average_read_time = total_average_read_time + average_read_time[i];
  }
  total_average_read_time = total_average_read_time / sections;

  long long total_low_time = analyze_low_time_ccc[0];
  for (i = 0; i < sections; i++)
  {
    if (analyze_low_time_ccc[i] < total_low_time)
    {
      total_low_time = analyze_low_time_ccc[i];
    }
  }

  long long total_high_time = analyze_high_time_ccc[0];
  for (i = 0; i < sections; i++)
  {
    if (analyze_high_time_ccc[i] > total_high_time)
    {
      total_high_time = analyze_high_time_ccc[i];
    }
  }

  int total_read_attempts = 0;
  for (i = 0; i < sections; i++)
  {
    total_read_attempts = total_read_attempts + analyze_read_attempts_ccc[i];
  }

  int total_good_reads = 0;
  for (i = 0; i < sections; i++)
  {
    total_good_reads = total_good_reads + analyze_good_reads_ccc[i];
  }

  int total_bad_reads = 0;
  for (i = 0; i < sections; i++)
  {
    total_bad_reads = total_bad_reads + analyze_bad_reads_ccc[i];
  }

  int total_slow_reads = 0;
  for (i = 0; i < sections; i++)
  {
    total_slow_reads = total_slow_reads + analyze_slow_reads_ccc[i];
  }

  int total_timeouts = 0;
  for (i = 0; i < sections; i++)
  {
    total_timeouts = total_timeouts + analyze_timeouts_ccc[i];
  }

  float good_percent;
  float bad_percent;
  float slow_percent;
  good_percent = 100.0f * total_good_reads / total_read_attempts;
  bad_percent = 100.0f * total_bad_reads / total_read_attempts;
  slow_percent = 100.0f * total_slow_reads / total_read_attempts;
  snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "# %s = %f%%\n", curlang_ccc[LANGGOOD], good_percent);
  strcat (analyze_text_ccc, tempmessage_ccc);
  snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "# %s = %f%%\n", curlang_ccc[LANGBAD], bad_percent);
  strcat (analyze_text_ccc, tempmessage_ccc);
  snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "# %s = %f%%\n", curlang_ccc[LANGSLOW], slow_percent);
  strcat (analyze_text_ccc, tempmessage_ccc);

  snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "#\n");
  strcat (analyze_text_ccc, tempmessage_ccc);


  float slow_issue_percent;
  if (slow_percent > 50.0f)
  {
    slow_issue_percent = slow_percent;
  }
  else
  {
    slow_issue_percent = slow_percent / 2.0f;
  }
  float slow_variance_percent = 0.0f;
  if (1)
  {
    int slowsections = MAXANALYZESLOW / 4;
    if (extended)
    {
      slowsections = MAXANALYZESLOW;
    }
    int slow_variance_count = 0;
    for (i = 0; i < slowsections; i++)
    {
      if (analyze_slow_high_ccc[i] - analyze_slow_low_ccc[i] > skip_timeout_copy_ccc)
      {
        slow_variance_count++;
      }
    }
    slow_variance_percent = 100.0f * slow_variance_count / slowsections;
  }
  slow_issue_percent = slow_issue_percent + slow_variance_percent;
  snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "# %s = %f%%\n", curlang_ccc[LANGSLOWRESPONDINGISSUE], slow_issue_percent);
  strcat (analyze_text_ccc, tempmessage_ccc);


  int consecutive_no_read_sections = 0;
  int highest_no_read_sections = 0;
  long long highest_read_time_sections = 0;
  for (i = 0; i < sections; i++)
  {
    if (analyze_read_attempts_ccc[i] > 0 && analyze_good_reads_ccc[i] == 0)
    {
      consecutive_no_read_sections++;
      if (highest_no_read_sections < consecutive_no_read_sections)
      {
        highest_no_read_sections = consecutive_no_read_sections;
      }
      if (highest_read_time_sections < analyze_high_time_ccc[i])
      {
        highest_read_time_sections = analyze_high_time_ccc[i];
      }
    }
    else
    {
      consecutive_no_read_sections = 0;
    }
  }
  float partial_access_percent = 100.0f * consecutive_no_read_sections / sections;
  if (consecutive_no_read_sections > 0 && highest_read_time_sections < skip_timeout_copy_ccc)
  {
    partial_access_percent = partial_access_percent * 2.0f;
  }
  snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "# %s = %f%%\n", curlang_ccc[LANGPARTIALACCESSISSUE], partial_access_percent);
  strcat (analyze_text_ccc, tempmessage_ccc);


  int bad_sections = 0;
  for (i = 0; i < sections; i++)
  {
    if (analyze_bad_reads_ccc[i] > 0)
    {
      bad_sections++;
    }
    else if (analyze_slow_reads_ccc[i] > 0 && slow_issue_percent < 50.0f)
    {
      bad_sections++;
    }
  }
  float bad_head_percent = 100.0f * bad_sections / sections;
  snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "# %s = %f%%\n", curlang_ccc[LANGBADORWEAKHEAD], bad_head_percent);
  strcat (analyze_text_ccc, tempmessage_ccc);

  snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "#\n");
  strcat (analyze_text_ccc, tempmessage_ccc);


  if (1)
  {
    snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "# (%d) %s", analyze_slow_total_reads_ccc, curlang_ccc[LANGVARIANCEREADTIMES]);
    strcat (analyze_text_ccc, tempmessage_ccc);
    int slowsections = MAXANALYZESLOW / 4;
    if (extended)
    {
      slowsections = MAXANALYZESLOW;
    }
    for (i = 0; i < slowsections; i++)
    {
      if ((i % 8) == 0)
      {
        snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "\n#     ");
        strcat (analyze_text_ccc, tempmessage_ccc);
      }
      snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%lld/%lld  ", analyze_slow_low_ccc[i]/1000, analyze_slow_high_ccc[i]/1000);
      strcat (analyze_text_ccc, tempmessage_ccc);
    }
    strcat (analyze_text_ccc, "\n");
  }

  snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "#\n");
  strcat (analyze_text_ccc, tempmessage_ccc);


  snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "# %s   %s %d    %s %d    %s %d (%d)    %s %d    %s %lld    %s %lld    %s %lld", curlang_ccc[LANGZONES], curlang_ccc[LANGTOTAL], total_read_attempts, curlang_ccc[LANGGOOD], total_good_reads, curlang_ccc[LANGBAD], total_bad_reads, total_timeouts, curlang_ccc[LANGSLOW], total_slow_reads, curlang_ccc[LANGLOW], total_low_time/1000, curlang_ccc[LANGHIGH], total_high_time/1000, curlang_ccc[LANGAVERAGE], total_average_read_time/1000);
  strcat (analyze_text_ccc, tempmessage_ccc);


  for (i = 0; i < sections; i++)
  {
    snprintf(tempmessage_ccc, sizeof(tempmessage_ccc),"\n# %s %d    %s %d    %s %d    %s %d (%d)    %s %d    %s %lld    %s %lld    %s %lld", curlang_ccc[LANGZONE], i, curlang_ccc[LANGTOTAL], analyze_read_attempts_ccc[i], curlang_ccc[LANGGOOD], analyze_good_reads_ccc[i], curlang_ccc[LANGBAD], analyze_bad_reads_ccc[i], analyze_timeouts_ccc[i], curlang_ccc[LANGSLOW], analyze_slow_reads_ccc[i], curlang_ccc[LANGLOW], analyze_low_time_ccc[i]/1000, curlang_ccc[LANGHIGH], analyze_high_time_ccc[i]/1000, curlang_ccc[LANGAVERAGE], average_read_time[i]/1000);
    strcat (analyze_text_ccc, tempmessage_ccc);
  }
  fprintf (stdout, "%s", analyze_text_ccc);

  display_analyze_results_ccc();

  update_logfile_ccc(0);

  gtk_widget_set_sensitive (GTK_WIDGET(connect_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(disconnect_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(start_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(stop_button_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(topmenubar_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(clone_mode_button_ccc), driver_only_ccc ? FALSE : TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(soft_reset_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(hard_reset_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(smart_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(analyze_button_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(analyze_long_button_ccc), TRUE);
  if (driver_installed_ccc)
  {
    set_driver_mode_button_status_ccc(TRUE);
  }

  if (ret && ret != STOP_SIGNAL_RETURN_CODE)
  {
    disconnect_devices_ccc();
  }
  start_display_status_update_timer_ccc();
  logfile_changed_ccc = true;
}





void display_analyze_results_ccc(void)
{
  GtkBuilder *builder = gtk_builder_new();
  GError *err = NULL; // It is mandatory to initialize to NULL
  if(0 == gtk_builder_add_from_string (builder, (const gchar *)hddsuperclone2_glade, hddsuperclone2_glade_len, &err))
  {
    fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
  }
  gtk_builder_connect_signals(builder, NULL);
  GtkWidget *dialog = GTK_WIDGET (gtk_builder_get_object (builder, "analyze_results_dialog"));
  GtkWidget *analyze_results_data_label = GTK_WIDGET (gtk_builder_get_object (builder, "analyze_results_data_label"));
  //GtkWidget *analyze_results_box_label = GTK_WIDGET (gtk_builder_get_object (builder, "analyze_results_box_label"));
  // = GTK_WIDGET (gtk_builder_get_object (builder, "")); // template
  g_object_unref(builder);

  gtk_label_set_text(GTK_LABEL(analyze_results_data_label), analyze_text_ccc);
  //gtk_button_set_label(GTK_BUTTON(), curlang_ccc[]);
  //gtk_label_set_text(GTK_LABEL(), curlang_ccc[]);

  gtk_window_set_title(GTK_WINDOW(dialog), curlang_ccc[LANGRESULTS]);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}





void get_smart_data_ccc(void)
{
  stop_signal_ccc = false;
  memset (smart_data_text_ccc, 0, sizeof(smart_data_text_ccc));
  int ret = extract_smart_data_ccc();
  if (ret == 0)
  {
    fprintf (stdout, "%s", smart_data_text_ccc);
    display_smart_data_ccc();
  }
  logfile_changed_ccc = true;
}





void display_smart_data_ccc(void)
{
  GtkBuilder *builder = gtk_builder_new();
  GError *err = NULL; // It is mandatory to initialize to NULL
  if(0 == gtk_builder_add_from_string (builder, (const gchar *)hddsuperclone2_glade, hddsuperclone2_glade_len, &err))
  {
    fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
  }
  gtk_builder_connect_signals(builder, NULL);
  GtkWidget *dialog = GTK_WIDGET (gtk_builder_get_object (builder, "smart_results_dialog"));
  GtkWidget *smart_results_data_label = GTK_WIDGET (gtk_builder_get_object (builder, "smart_results_data_label"));
  //GtkWidget *smart_results_box_label = GTK_WIDGET (gtk_builder_get_object (builder, "smart_results_box_label"));
  // = GTK_WIDGET (gtk_builder_get_object (builder, "")); // template
  g_object_unref(builder);

  gtk_label_set_text(GTK_LABEL(smart_results_data_label), smart_data_text_ccc);
  //gtk_button_set_label(GTK_BUTTON(), curlang_ccc[]);
  //gtk_label_set_text(GTK_LABEL(), curlang_ccc[]);

  gtk_window_set_title(GTK_WINDOW(dialog), curlang_ccc[LANGRESULTS]);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}





void display_identify_data_ccc(void)
{
  GtkBuilder *builder = gtk_builder_new();
  GError *err = NULL; // It is mandatory to initialize to NULL
  if(0 == gtk_builder_add_from_string (builder, (const gchar *)hddsuperclone2_glade, hddsuperclone2_glade_len, &err))
  {
    fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
  }
  gtk_builder_connect_signals(builder, NULL);
  GtkWidget *dialog = GTK_WIDGET (gtk_builder_get_object (builder, "identify_results_dialog"));
  GtkWidget *smart_results_data_label = GTK_WIDGET (gtk_builder_get_object (builder, "identify_results_data_label"));
  //GtkWidget *smart_results_box_label = GTK_WIDGET (gtk_builder_get_object (builder, "smart_results_box_label"));
  // = GTK_WIDGET (gtk_builder_get_object (builder, "")); // template
  g_object_unref(builder);

  gtk_label_set_text(GTK_LABEL(smart_results_data_label), identify_device_raw_text_ccc);
  //gtk_button_set_label(GTK_BUTTON(), curlang_ccc[]);
  //gtk_label_set_text(GTK_LABEL(), curlang_ccc[]);

  gtk_window_set_title(GTK_WINDOW(dialog), curlang_ccc[LANGRESULTS]);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}





void quit_and_close_ccc (void)
{
  gtk_main_quit();
  quit_and_exit_ccc();
}





void open_clone_settings_dialog_ccc (void)
{
  GtkBuilder *builder = gtk_builder_new();
  GError *err = NULL; // It is mandatory to initialize to NULL
  if(0 == gtk_builder_add_from_string (builder, (const gchar *)hddsuperclone2_glade, hddsuperclone2_glade_len, &err))
  {
    fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
  }
  gtk_builder_connect_signals(builder, NULL);
  GtkWidget *dialog = GTK_WIDGET (gtk_builder_get_object (builder, "clone_settings_dialog"));
  phase_control_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "phase_control_label"));
  phase1_check_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "phase1_checkbutton"));
  phase2_check_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "phase2_checkbutton"));
  phase3_check_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "phase3_checkbutton"));
  phase4_check_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "phase4_checkbutton"));
  divide_check_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "divide_checkbutton"));
  divide2_check_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "divide2_checkbutton"));
  trim_check_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "trim_checkbutton"));
  scrape_check_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "scrape_checkbutton"));
  markbad_check_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "markbad_checkbutton"));
  readbad_check_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "readbad_checkbutton"));
  rebuild_assist_check_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "rebuild_assist_checkbutton"));
  reverse_check_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "reverse_checkbutton"));
  skipfast_check_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "skipfast_checkbutton"));
  retries_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "retries_button_label"));
  retries_spin_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "retries_spinbutton"));
  clustersize_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "clustersize_button_label"));
  clustersize_spin_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "clustersize_spinbutton"));
  inputoffset_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "inputoffset_button_label"));
  inputoffset_spin_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "inputoffset_spinbutton"));
  size_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "size_button_label"));
  size_spin_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "size_spinbutton"));
  blocksize_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "blocksize_button_label"));
  blocksize_spin_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "blocksize_spinbutton"));
  skipsize_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "skipsize_button_label"));
  skipsize_spin_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "skipsize_spinbutton"));
  maxskipsize_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "maxskipsize_button_label"));
  maxskipsize_spin_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "maxskipsize_spinbutton"));
  skipthreshold_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "skipthreshold_button_label"));
  skipthreshold_spin_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "skipthreshold_spinbutton"));
  rateskip_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "rateskip_button_label"));
  rateskip_spin_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "rateskip_spinbutton"));
  exitonslow_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "exitonslow_button_label"));
  exitonslow_spin_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "exitonslow_spinbutton"));
  exitonslowtime_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "exitonslowtime_button_label"));
  exitonslowtime_spin_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "exitonslowtime_spinbutton"));
  sectorsize_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "sectorsize_button_label"));
  sectorsize_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "sectorsize_spinbutton"));
  alignment_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "alignment_button_label"));
  alignment_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "alignment_spinbutton"));
  maxreadrate_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "maxreadrate_button_label"));
  maxreadrate_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "maxreadrate_spinbutton"));
  // = GTK_WIDGET (gtk_builder_get_object (builder, "")); // template
  g_object_unref(builder);

  gtk_label_set_text(GTK_LABEL(phase_control_label_ccc), curlang_ccc[LANGPHASECONTROLLABEL]);
  gtk_button_set_label(GTK_BUTTON(phase1_check_button_ccc), curlang_ccc[LANGPHASE1BUTTON]);
  gtk_button_set_label(GTK_BUTTON(phase2_check_button_ccc), curlang_ccc[LANGPHASE2BUTTON]);
  gtk_button_set_label(GTK_BUTTON(phase3_check_button_ccc), curlang_ccc[LANGPHASE3BUTTON]);
  gtk_button_set_label(GTK_BUTTON(phase4_check_button_ccc), curlang_ccc[LANGPHASE4BUTTON]);
  gtk_button_set_label(GTK_BUTTON(divide_check_button_ccc), curlang_ccc[LANGDIVIDEBUTTON]);
  gtk_button_set_label(GTK_BUTTON(divide2_check_button_ccc), curlang_ccc[LANGDIVIDE2BUTTON]);
  gtk_button_set_label(GTK_BUTTON(trim_check_button_ccc), curlang_ccc[LANGTRIMBUTTON]);
  gtk_button_set_label(GTK_BUTTON(scrape_check_button_ccc), curlang_ccc[LANGSCRAPTBUTTON]);
  gtk_button_set_label(GTK_BUTTON(markbad_check_button_ccc), curlang_ccc[LANGMARKBADBUTTON]);
  gtk_button_set_label(GTK_BUTTON(readbad_check_button_ccc), curlang_ccc[LANGREADBADBUTTON]);
  gtk_button_set_label(GTK_BUTTON(rebuild_assist_check_button_ccc), curlang_ccc[LANGREBUILDASSIST]);
  gtk_button_set_label(GTK_BUTTON(reverse_check_button_ccc), curlang_ccc[LANGREVERSEBUTTON]);
  gtk_button_set_label(GTK_BUTTON(skipfast_check_button_ccc), curlang_ccc[LANGSKIPFASTBUTTON]);
  gtk_label_set_text(GTK_LABEL(retries_button_label_ccc), curlang_ccc[LANGRETRIESBUTTON]);
  gtk_label_set_text(GTK_LABEL(clustersize_button_label_ccc), curlang_ccc[LANGCLUSTERSIZEBUTTON]);
  gtk_label_set_text(GTK_LABEL(inputoffset_button_label_ccc), curlang_ccc[LANGINPUTOFFSETBUTTON]);
  gtk_label_set_text(GTK_LABEL(size_button_label_ccc), curlang_ccc[LANGSIZEBUTTON]);
  gtk_label_set_text(GTK_LABEL(blocksize_button_label_ccc), curlang_ccc[LANGBLOCKSIZEBUTTON]);
  gtk_label_set_text(GTK_LABEL(skipsize_button_label_ccc), curlang_ccc[LANGSKIPSIZEBUTTON]);
  gtk_label_set_text(GTK_LABEL(maxskipsize_button_label_ccc), curlang_ccc[LANGMAXSKIPSIZEBUTTON]);
  gtk_label_set_text(GTK_LABEL(skipthreshold_button_label_ccc), curlang_ccc[LANGSKIPTHRESHOLDBUTTON]);
  gtk_label_set_text(GTK_LABEL(rateskip_button_label_ccc), curlang_ccc[LANGRATESKIPBUTTON]);
  gtk_label_set_text(GTK_LABEL(exitonslow_button_label_ccc), curlang_ccc[LANGEXITONSLOW]);
  gtk_label_set_text(GTK_LABEL(exitonslowtime_button_label_ccc), curlang_ccc[LANGEXITONSLOWTIME]);
  gtk_label_set_text(GTK_LABEL(sectorsize_button_label_ccc), curlang_ccc[LANGSECTORSIZE]);
  gtk_label_set_text(GTK_LABEL(alignment_button_label_ccc), curlang_ccc[LANGBLOCKALIGNMENT]);
  gtk_label_set_text(GTK_LABEL(maxreadrate_button_label_ccc), curlang_ccc[LANGMAXREADRATE]);
  //gtk_button_set_label(GTK_BUTTON(), curlang_ccc[]);
  //gtk_label_set_text(GTK_LABEL(), curlang_ccc[]);

  g_signal_connect(G_OBJECT(phase1_check_button_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_PHASE1));
  g_signal_connect(G_OBJECT(phase2_check_button_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_PHASE2));
  g_signal_connect(G_OBJECT(phase3_check_button_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_PHASE3));
  g_signal_connect(G_OBJECT(phase4_check_button_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_PHASE4));
  g_signal_connect(G_OBJECT(divide_check_button_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_DIVIDE));
  g_signal_connect(G_OBJECT(divide2_check_button_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_DIVIDE2));
  g_signal_connect(G_OBJECT(trim_check_button_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_TRIM));
  g_signal_connect(G_OBJECT(scrape_check_button_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_SCRAPE));
  g_signal_connect(G_OBJECT(markbad_check_button_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_MARKBAD));
  g_signal_connect(G_OBJECT(readbad_check_button_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_READBAD));
  g_signal_connect(G_OBJECT(rebuild_assist_check_button_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_REBUILDASSIST));
  g_signal_connect(G_OBJECT(reverse_check_button_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_REVERSE));
  g_signal_connect(G_OBJECT(skipfast_check_button_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_SKIPFAST));


  load_clone_settings_ccc();
  update_clone_button_settings_ccc();

  gtk_window_set_title(GTK_WINDOW(dialog), curlang_ccc[LANGCLONE]);
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == 5)
  {
    clone_settings_ccc.retries = gtk_spin_button_get_value(GTK_SPIN_BUTTON(retries_spin_button_ccc));
    clone_settings_ccc.cluster_size = gtk_spin_button_get_value(GTK_SPIN_BUTTON(clustersize_spin_button_ccc));
    clone_settings_ccc.input_offset = gtk_spin_button_get_value(GTK_SPIN_BUTTON(inputoffset_spin_button_ccc));
    clone_settings_ccc.read_size = gtk_spin_button_get_value(GTK_SPIN_BUTTON(size_spin_button_ccc));
    clone_settings_ccc.block_size = gtk_spin_button_get_value(GTK_SPIN_BUTTON(blocksize_spin_button_ccc));
    clone_settings_ccc.min_skip_size = gtk_spin_button_get_value(GTK_SPIN_BUTTON(skipsize_spin_button_ccc));
    clone_settings_ccc.max_skip_size = gtk_spin_button_get_value(GTK_SPIN_BUTTON(maxskipsize_spin_button_ccc));
    clone_settings_ccc.skip_timeout = gtk_spin_button_get_value(GTK_SPIN_BUTTON(skipthreshold_spin_button_ccc)) * 1000;
    clone_settings_ccc.rate_skip = gtk_spin_button_get_value(GTK_SPIN_BUTTON(rateskip_spin_button_ccc)) * 1000;
    clone_settings_ccc.exit_on_slow = gtk_spin_button_get_value(GTK_SPIN_BUTTON(exitonslow_spin_button_ccc)) * 1000;
    clone_settings_ccc.exit_slow_time = gtk_spin_button_get_value(GTK_SPIN_BUTTON(exitonslowtime_spin_button_ccc)) * 1000;
    clone_settings_ccc.sector_size = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sectorsize_spinbutton_ccc));
    clone_settings_ccc.block_offset = gtk_spin_button_get_value(GTK_SPIN_BUTTON(alignment_spinbutton_ccc));
    clone_settings_ccc.max_read_rate = gtk_spin_button_get_value(GTK_SPIN_BUTTON(maxreadrate_spinbutton_ccc)) * 1000;
    //clone_settings_ccc. = gtk_spin_button_get_value(GTK_SPIN_BUTTON());
    update_clone_settings_ccc();
    logfile_changed_ccc = true;
  }
  gtk_widget_destroy(dialog);
  update_display_ccc(0);
}





void open_advanced_settings_dialog_ccc (void)
{
  GtkBuilder *builder = gtk_builder_new();
  GError *err = NULL; // It is mandatory to initialize to NULL
  if(0 == gtk_builder_add_from_string (builder, (const gchar *)hddsuperclone2_glade, hddsuperclone2_glade_len, &err))
  {
    fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
  }
  gtk_builder_connect_signals(builder, NULL);
  GtkWidget *dialog = GTK_WIDGET (gtk_builder_get_object (builder, "advanced_settings_dialog"));
  no_log_backup_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "no_log_backup_checkbutton"));
  force_mounted_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "force_mounted_checkbutton"));
  force_dangerous_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "force_dangerous_checkbutton"));
  enable_output_offset_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "enable_output_offset_checkbutton"));
  output_offset_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "output_offset_spinbutton"));
  output_offset_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "output_offset_button_label"));
  enable_current_position_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "enable_current_position_checkbutton"));
  current_position_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "current_position_spinbutton"));
  current_position_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "current_position_button_label"));
  action_on_error_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "action_on_error_label"));
  stop_on_error_radio_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "stop_on_error_radio_button"));
  call_command_radio_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "call_command_radio_button"));
  command_to_call_text_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "command_to_call_text"));
  test_command_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "test_command_button"));
  primary_relay_on_error_radio_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "primary_relay_on_error_radio_button"));
  write_buffer_disable_radio_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "write_buffer_disable_radio_button"));
  write_buffer_enable_radio_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "write_buffer_enable_radio_button"));
  write_buffer_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "write_buffer_label"));
  disable_identify_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "disable_identify_checkbutton"));
  pio_mode_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "pio_mode_checkbutton"));
  enable_rebuild_assist_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "enable_rebuild_assist_checkbutton"));
  enable_process_chunk_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "enable_process_chunk_checkbutton"));
  enable_read_twice_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "enable_read_twice_checkbutton"));
  enable_retry_connecting_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "enable_retry_connecting_checkbutton"));
  enable_scsi_write_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "enable_scsi_write_checkbutton"));
  enable_phase_log_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "enable_phase_log_checkbutton"));
  enable_output_sector_size_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "enable_output_sector_size_checkbutton"));
  output_sector_size_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "output_sector_size_button_label"));
  driver_io_scsi_only_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "driver_io_scsi_only_checkbutton"));
  use_physical_sector_size_for_virtual_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "use_physical_sector_size_for_virtual_checkbutton"));
  output_sector_size_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "output_sector_size_spinbutton"));
  driver_error_options_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "driver_error_options_label"));
  driver_return_error_radio_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "driver_return_error_radio_button"));
  driver_return_zeros_radio_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "driver_return_zeros_radio_button"));
  driver_return_marked_radio_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "driver_return_marked_radio_button"));
  virtual_disk_device_name_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "virtual_disk_device_name_label"));
  virtual_disk_device_name_text_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "virtual_disk_device_name_text"));
  driver_minimum_cluster_size_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "driver_minimum_cluster_size_button_label"));
  driver_minimum_cluster_size_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "driver_minimum_cluster_size_spinbutton"));
  use_color_statusbar_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "use_color_statusbar_checkbutton"));
  // = GTK_WIDGET (gtk_builder_get_object (builder, "")); // template
  g_object_unref(builder);

  gtk_button_set_label(GTK_BUTTON(no_log_backup_checkbutton_ccc), curlang_ccc[LANGNOLOGBACKUP]);
  gtk_button_set_label(GTK_BUTTON(force_mounted_checkbutton_ccc), curlang_ccc[LANGFORCEMOUNTED]);
  gtk_button_set_label(GTK_BUTTON(force_dangerous_checkbutton_ccc), curlang_ccc[LANGFORCEDANGEROUS]);
  gtk_button_set_label(GTK_BUTTON(enable_output_offset_checkbutton_ccc), curlang_ccc[LANGENABLEOUTPUTOFFSET]);
  gtk_label_set_text(GTK_LABEL(output_offset_button_label_ccc), curlang_ccc[LANGOUTPUTOFFSET]);
  gtk_button_set_label(GTK_BUTTON(enable_current_position_checkbutton_ccc), curlang_ccc[LANGENABLECURRENTPOSITION]);
  gtk_label_set_text(GTK_LABEL(current_position_button_label_ccc), curlang_ccc[LANGCURRENTPOSISIONLABEL]);
  gtk_label_set_text(GTK_LABEL(action_on_error_label_ccc), curlang_ccc[LANGACTIONONERROR]);
  gtk_button_set_label(GTK_BUTTON(stop_on_error_radio_button_ccc), curlang_ccc[LANGSTOPONERROR]);
  gtk_button_set_label(GTK_BUTTON(call_command_radio_button_ccc), curlang_ccc[LANGCALLCOMMAND]);
  gtk_button_set_label(GTK_BUTTON(test_command_button_ccc), curlang_ccc[LANGTESTCOMMAND]);
  gtk_button_set_label(GTK_BUTTON(primary_relay_on_error_radio_button_ccc), curlang_ccc[LANGPRIMARYRELAYONERROR]);
  gtk_button_set_label(GTK_BUTTON(write_buffer_disable_radio_button_ccc), curlang_ccc[LANGWRITEBUFFERDISABLE]);
  gtk_button_set_label(GTK_BUTTON(write_buffer_enable_radio_button_ccc), curlang_ccc[LANGWRITEBUFFERENABLE]);
  gtk_label_set_text(GTK_LABEL(write_buffer_label_ccc), curlang_ccc[LANGWRITEBUFFERLABEL]);
  gtk_button_set_label(GTK_BUTTON(disable_identify_checkbutton_ccc), curlang_ccc[LANGDONTIDENTIFY]);
  gtk_button_set_label(GTK_BUTTON(pio_mode_checkbutton_ccc), curlang_ccc[LANGPIOMODE]);
  gtk_button_set_label(GTK_BUTTON(enable_rebuild_assist_checkbutton_ccc), curlang_ccc[LANGENABLEREBUILDASSIST]);
  gtk_button_set_label(GTK_BUTTON(enable_process_chunk_checkbutton_ccc), curlang_ccc[LANGENABLEPROCESSCHUNK]);
  gtk_button_set_label(GTK_BUTTON(enable_read_twice_checkbutton_ccc), curlang_ccc[LANGENABLEREADTWICE]);
  gtk_button_set_label(GTK_BUTTON(enable_retry_connecting_checkbutton_ccc), curlang_ccc[LANGENABLERETRYCONNECTING]);
  gtk_button_set_label(GTK_BUTTON(enable_scsi_write_checkbutton_ccc), curlang_ccc[LANGENABLESCSIWRITE]);
  gtk_button_set_label(GTK_BUTTON(enable_phase_log_checkbutton_ccc), curlang_ccc[LANGENABLEPHASELOG]);
  gtk_button_set_label(GTK_BUTTON(enable_output_sector_size_checkbutton_ccc), curlang_ccc[LANGENABLEOUTPUTSECTORSIZE]);
  gtk_label_set_text(GTK_LABEL(output_sector_size_button_label_ccc), curlang_ccc[LANGOUTPUTSECTORSIZE]);
  gtk_button_set_label(GTK_BUTTON(driver_io_scsi_only_checkbutton_ccc), curlang_ccc[LANGIOSCSIONLY]);
  gtk_button_set_label(GTK_BUTTON(use_physical_sector_size_for_virtual_checkbutton_ccc), curlang_ccc[LANGUSEPHYSICALSECTORSIZE]);
  gtk_button_set_label(GTK_BUTTON(driver_return_error_radio_button_ccc), curlang_ccc[LANGDRIVERRETURNERROR]);
  gtk_button_set_label(GTK_BUTTON(driver_return_zeros_radio_button_ccc), curlang_ccc[LANGDRIVERRETURNZEROS]);
  gtk_button_set_label(GTK_BUTTON(driver_return_marked_radio_button_ccc), curlang_ccc[LANGDRIVERRETURNMARKED]);
  gtk_label_set_text(GTK_LABEL(driver_error_options_label_ccc), curlang_ccc[LANGDRIVEROUTPUTOPTIONS]);
  gtk_label_set_text(GTK_LABEL(driver_minimum_cluster_size_button_label_ccc), curlang_ccc[LANGVIRTUALMINCLUSTER]);
  gtk_label_set_text(GTK_LABEL(virtual_disk_device_name_label_ccc), curlang_ccc[LANGVIRTUALNAME]);
  gtk_button_set_label(GTK_BUTTON(use_color_statusbar_checkbutton_ccc), curlang_ccc[LANGUSECOLORSTATUSBAR]);
  //gtk_button_set_label(GTK_BUTTON(), curlang_ccc[]); // template
  //gtk_label_set_text(GTK_LABEL(), curlang_ccc[]); // template

  if (!advanced_settings_ccc.enable_output_offset)
  {
    gtk_widget_set_sensitive (GTK_WIDGET(output_offset_spinbutton_ccc), FALSE);
  }
  if (!advanced_settings_ccc.enable_current_position)
  {
    gtk_widget_set_sensitive (GTK_WIDGET(current_position_spinbutton_ccc), FALSE);
  }
  if (!advanced_settings_ccc.enable_output_sector_size)
  {
    gtk_widget_set_sensitive (GTK_WIDGET(output_sector_size_spinbutton_ccc), FALSE);
  }

  gtk_entry_set_max_length (GTK_ENTRY (command_to_call_text_ccc), MAX_CALL_LENGTH);
  gtk_entry_set_max_length (GTK_ENTRY (virtual_disk_device_name_text_ccc), MAX_DEVICE_NAME_LENGTH);

  g_signal_connect(G_OBJECT(no_log_backup_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_NOLOGBACK));
  g_signal_connect(G_OBJECT(force_mounted_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_FORCE));
  g_signal_connect(G_OBJECT(force_dangerous_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_FORCEDANGEROUS));
  g_signal_connect(G_OBJECT(enable_output_offset_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_OFFSET));
  g_signal_connect(G_OBJECT(enable_current_position_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_POSITION));
  g_signal_connect(G_OBJECT(disable_identify_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_DONTIDENTIFY));
  g_signal_connect(G_OBJECT(pio_mode_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_PIOMODE));
  g_signal_connect(G_OBJECT(enable_rebuild_assist_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_ENABLEREBUILDASSIST));
  g_signal_connect(G_OBJECT(enable_process_chunk_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_ENABLEPROCESSCHUNK));
  g_signal_connect(G_OBJECT(enable_read_twice_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_ENABLEREADTWICE));
  g_signal_connect(G_OBJECT(enable_retry_connecting_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_ENABLERETRYCONNECTING));
  g_signal_connect(G_OBJECT(enable_scsi_write_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_ENABLESCSIWRITE));
  g_signal_connect(G_OBJECT(enable_phase_log_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_ENABLEPHASELOGS));
  g_signal_connect(G_OBJECT(enable_output_sector_size_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_ENABLEOUTPUTSECTOR));
  g_signal_connect(G_OBJECT(driver_io_scsi_only_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_IOSCSIONLY));
  g_signal_connect(G_OBJECT(use_physical_sector_size_for_virtual_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_USEPHYSICALSECTORSIZE));
  g_signal_connect(G_OBJECT(use_color_statusbar_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_COLORSTATUSBAR));
  //g_signal_connect(G_OBJECT(), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER());
  //g_signal_connect(test_command_button_ccc, "clicked", G_CALLBACK(test_command_ccc), NULL);

  load_advanced_settings_ccc();
  gtk_entry_set_text (GTK_ENTRY (command_to_call_text_ccc), advanced_settings_ccc.command_to_call);
  gtk_entry_set_text (GTK_ENTRY (virtual_disk_device_name_text_ccc), advanced_settings_ccc.virtual_disk_device_name);
  update_advanced_button_settings_ccc();

  gtk_widget_set_sensitive (GTK_WIDGET(disable_identify_checkbutton_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(pio_mode_checkbutton_ccc), TRUE);

  gtk_window_set_title(GTK_WINDOW(dialog), curlang_ccc[LANGADVANCED]);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == 5)
  {
    advanced_settings_ccc.output_offset = gtk_spin_button_get_value(GTK_SPIN_BUTTON(output_offset_spinbutton_ccc));
    advanced_settings_ccc.current_position = gtk_spin_button_get_value(GTK_SPIN_BUTTON(current_position_spinbutton_ccc));
    advanced_settings_ccc.stop_on_error = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (stop_on_error_radio_button_ccc));
    advanced_settings_ccc.call_command = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (call_command_radio_button_ccc));
    advanced_settings_ccc.activate_primary_relay = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (primary_relay_on_error_radio_button_ccc));
    advanced_settings_ccc.write_buffer_disable = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (write_buffer_disable_radio_button_ccc));
    advanced_settings_ccc.write_buffer_enable = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (write_buffer_enable_radio_button_ccc));
    advanced_settings_ccc.driver_return_error = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (driver_return_error_radio_button_ccc));
    advanced_settings_ccc.driver_return_zeros = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (driver_return_zeros_radio_button_ccc));
    advanced_settings_ccc.driver_return_marked = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (driver_return_marked_radio_button_ccc));
    advanced_settings_ccc.output_sector_adjustment = gtk_spin_button_get_value(GTK_SPIN_BUTTON(output_sector_size_spinbutton_ccc));
    advanced_settings_ccc.driver_minimum_cluster_size = gtk_spin_button_get_value(GTK_SPIN_BUTTON(driver_minimum_cluster_size_spinbutton_ccc));
    const gchar *entry_text;
    entry_text = gtk_entry_get_text (GTK_ENTRY (command_to_call_text_ccc));
    strlcpy (advanced_settings_ccc.command_to_call, entry_text, sizeof(advanced_settings_ccc.command_to_call));
    const gchar *entry_text2;
    entry_text2 = gtk_entry_get_text (GTK_ENTRY (virtual_disk_device_name_text_ccc));
    strlcpy (advanced_settings_ccc.virtual_disk_device_name, entry_text2, sizeof(advanced_settings_ccc.virtual_disk_device_name));
    update_advanced_settings_ccc();
    logfile_changed_ccc = true;
  }
  gtk_widget_destroy(dialog);
  update_display_ccc(0);
}





void open_timer_settings_dialog_ccc (void)
{
  GtkBuilder *builder = gtk_builder_new();
  GError *err = NULL; // It is mandatory to initialize to NULL
  if(0 == gtk_builder_add_from_string (builder, (const gchar *)hddsuperclone2_glade, hddsuperclone2_glade_len, &err))
  {
    fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
  }
  gtk_builder_connect_signals(builder, NULL);
  GtkWidget *dialog = GTK_WIDGET (gtk_builder_get_object (builder, "timer_settings_dialog"));
  timer_settings_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "timer_settings_label"));
  timer_settings_info_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "timer_settings_info_label"));
  initial_busy_wait_time_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "initial_busy_wait_time_button_label"));
  initial_busy_wait_time_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "initial_busy_wait_time_spinbutton"));
  busy_wait_time_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "busy_wait_time_button_label"));
  busy_wait_time_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "busy_wait_time_spinbutton"));
  soft_reset_time_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "soft_reset_time_button_label"));
  soft_reset_time_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "soft_reset_time_spinbutton"));
  hard_reset_time_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "hard_reset_time_button_label"));
  hard_reset_time_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "hard_reset_time_spinbutton"));
  power_cycle_time_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "power_cycle_time_button_label"));
  power_cycle_time_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "power_cycle_time_spinbutton"));
  general_timout_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "general_timout_button_label"));
  general_timeout_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "general_timeout_spinbutton"));
  action_power_cycle_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "action_power_cycle_label"));
  stop_on_power_cycle_radio_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "stop_on_power_cycle_radio_button"));
  call_power_command_radio_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "call_power_command_radio_button"));
  power_command_to_call_text_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "power_command_to_call_text"));
  test_power_command_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "test_power_command_button"));
  primary_relay_power_cycle_radio_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "primary_relay_power_cycle_radio_button"));
  phase_timers_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "phase_timers_label"));
  phase_timers_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "phase_timers_checkbutton"));
  p12_soft_reset_time_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "p12_soft_reset_time_button_label"));
  p12_soft_reset_time_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "p12_soft_reset_time_spinbutton"));
  p3_soft_reset_time_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "p3_soft_reset_time_button_label"));
  p3_soft_reset_time_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "p3_soft_reset_time_spinbutton"));
  p4_soft_reset_time_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "p4_soft_reset_time_button_label"));
  p4_soft_reset_time_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "p4_soft_reset_time_spinbutton"));
  td_soft_reset_time_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "td_soft_reset_time_button_label"));
  td_soft_reset_time_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "td_soft_reset_time_spinbutton"));
  d2_soft_reset_time_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "d2_soft_reset_time_button_label"));
  d2_soft_reset_time_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "d2_soft_reset_time_spinbutton"));
  sc_soft_reset_time_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "sc_soft_reset_time_button_label"));
  sc_soft_reset_time_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "sc_soft_reset_time_spinbutton"));
  rt_soft_reset_time_button_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "rt_soft_reset_time_button_label"));
  rt_soft_reset_time_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "rt_soft_reset_time_spinbutton"));
  always_wait_for_reset_timers_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "always_wait_for_reset_timers_checkbutton"));
  usb_resets_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "usb_resets_label"));
  enable_usb_bulk_reset_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "enable_usb_bulk_reset_checkbutton"));
  enable_usb_soft_reset_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "enable_usb_soft_reset_checkbutton"));
  enable_usb_hard_reset_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "enable_usb_hard_reset_checkbutton"));
  enable_usb_port_reset_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "enable_usb_port_reset_checkbutton"));
  // = GTK_WIDGET (gtk_builder_get_object (builder, "")); // template
  g_object_unref(builder);

  gtk_label_set_text(GTK_LABEL(timer_settings_label_ccc), "");
  gtk_label_set_text(GTK_LABEL(timer_settings_info_label_ccc), curlang_ccc[LANGTIMERINFO]);
  gtk_label_set_text(GTK_LABEL(initial_busy_wait_time_button_label_ccc), curlang_ccc[LANGINITIALBUSYWAIT]);
  gtk_label_set_text(GTK_LABEL(busy_wait_time_button_label_ccc), curlang_ccc[LANGBUSYWAIT]);
  gtk_label_set_text(GTK_LABEL(soft_reset_time_button_label_ccc), curlang_ccc[LANGSOFTTIME]);
  gtk_label_set_text(GTK_LABEL(hard_reset_time_button_label_ccc), curlang_ccc[LANGHARDTIME]);
  gtk_label_set_text(GTK_LABEL(power_cycle_time_button_label_ccc), curlang_ccc[LANGPOWERTIME]);
  gtk_label_set_text(GTK_LABEL(general_timout_button_label_ccc), curlang_ccc[LANGGENERALTIME]);
  gtk_label_set_text(GTK_LABEL(action_power_cycle_label_ccc), curlang_ccc[LANGPOWERACTION]);
  gtk_button_set_label(GTK_BUTTON(stop_on_power_cycle_radio_button_ccc), curlang_ccc[LANGSTOPONPOWER]);
  gtk_button_set_label(GTK_BUTTON(call_power_command_radio_button_ccc), curlang_ccc[LANGCALLCOMMAND]);
  gtk_button_set_label(GTK_BUTTON(test_power_command_button_ccc), curlang_ccc[LANGTESTCOMMAND]);
  gtk_button_set_label(GTK_BUTTON(primary_relay_power_cycle_radio_button_ccc), curlang_ccc[LANGCYCLEPRIMARYRELAY]);
  gtk_label_set_text(GTK_LABEL(phase_timers_label_ccc), curlang_ccc[LANGPHASETIMERS]);
  gtk_button_set_label(GTK_BUTTON(phase_timers_checkbutton_ccc), curlang_ccc[LANGPHASETIMERSENABLE]);
  gtk_label_set_text(GTK_LABEL(p12_soft_reset_time_button_label_ccc), curlang_ccc[LANGSOFTTIMEP12]);
  gtk_label_set_text(GTK_LABEL(p3_soft_reset_time_button_label_ccc), curlang_ccc[LANGSOFTTIMEP3]);
  gtk_label_set_text(GTK_LABEL(p4_soft_reset_time_button_label_ccc), curlang_ccc[LANGSOFTTIMEP4]);
  gtk_label_set_text(GTK_LABEL(td_soft_reset_time_button_label_ccc), curlang_ccc[LANGSOFTTIMETD]);
  gtk_label_set_text(GTK_LABEL(d2_soft_reset_time_button_label_ccc), curlang_ccc[LANGSOFTTIMED2]);
  gtk_label_set_text(GTK_LABEL(sc_soft_reset_time_button_label_ccc), curlang_ccc[LANGSOFTTIMESC]);
  gtk_label_set_text(GTK_LABEL(rt_soft_reset_time_button_label_ccc), curlang_ccc[LANGSOFTTIMERT]);
  gtk_button_set_label(GTK_BUTTON(always_wait_for_reset_timers_checkbutton_ccc), curlang_ccc[LANGALWAYSWAITRESETTIME]);
  gtk_label_set_text(GTK_LABEL(usb_resets_label_ccc), curlang_ccc[LANGUSBRESETS]);
  gtk_button_set_label(GTK_BUTTON(enable_usb_bulk_reset_checkbutton_ccc), curlang_ccc[LANGENABLEUSBBULKRESET]);
  gtk_button_set_label(GTK_BUTTON(enable_usb_soft_reset_checkbutton_ccc), curlang_ccc[LANGENABLEUSBSOFTRESET]);
  gtk_button_set_label(GTK_BUTTON(enable_usb_hard_reset_checkbutton_ccc), curlang_ccc[LANGENABLEUSBHARDRESET]);
  gtk_button_set_label(GTK_BUTTON(enable_usb_port_reset_checkbutton_ccc), curlang_ccc[LANGENABLEUSBPORTRESET]);
  //gtk_button_set_label(GTK_BUTTON(), curlang_ccc[]);
  //gtk_label_set_text(GTK_LABEL(), curlang_ccc[]);

  gtk_entry_set_max_length (GTK_ENTRY (power_command_to_call_text_ccc), MAX_CALL_LENGTH);

  g_signal_connect(G_OBJECT(phase_timers_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_PHASETIMERS));


  load_timer_settings_ccc();
  gtk_entry_set_text (GTK_ENTRY (power_command_to_call_text_ccc), timer_settings_ccc.power_command_to_call);
  update_timer_button_settings_ccc();

  if (!direct_mode_ccc && !usb_mode_ccc)
  {
    gtk_widget_set_sensitive (GTK_WIDGET(initial_busy_wait_time_spinbutton_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(busy_wait_time_spinbutton_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(soft_reset_time_spinbutton_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(power_cycle_time_spinbutton_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(stop_on_power_cycle_radio_button_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(call_power_command_radio_button_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(test_power_command_button_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(power_command_to_call_text_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(primary_relay_power_cycle_radio_button_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(phase_timers_checkbutton_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(p12_soft_reset_time_spinbutton_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(p3_soft_reset_time_spinbutton_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(p4_soft_reset_time_spinbutton_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(td_soft_reset_time_spinbutton_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(d2_soft_reset_time_spinbutton_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(sc_soft_reset_time_spinbutton_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(rt_soft_reset_time_spinbutton_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(always_wait_for_reset_timers_checkbutton_ccc), FALSE);
  }
  if (!usb_mode_ccc)
  {
    gtk_widget_set_sensitive (GTK_WIDGET(enable_usb_bulk_reset_checkbutton_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(enable_usb_soft_reset_checkbutton_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(enable_usb_hard_reset_checkbutton_ccc), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET(enable_usb_port_reset_checkbutton_ccc), FALSE);
  }
  if (!ahci_mode_ccc && !usb_mode_ccc)
  {
    gtk_widget_set_sensitive (GTK_WIDGET(hard_reset_time_spinbutton_ccc), FALSE);
  }

  gtk_window_set_title(GTK_WINDOW(dialog), curlang_ccc[LANGTIMERS]);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == 5)
  {
    timer_settings_ccc.initial_busy_wait_time = gtk_spin_button_get_value(GTK_SPIN_BUTTON(initial_busy_wait_time_spinbutton_ccc));
    timer_settings_ccc.busy_wait_time = gtk_spin_button_get_value(GTK_SPIN_BUTTON(busy_wait_time_spinbutton_ccc));
    timer_settings_ccc.soft_reset_time = gtk_spin_button_get_value(GTK_SPIN_BUTTON(soft_reset_time_spinbutton_ccc));
    timer_settings_ccc.hard_reset_time = gtk_spin_button_get_value(GTK_SPIN_BUTTON(hard_reset_time_spinbutton_ccc));
    timer_settings_ccc.power_cycle_time = gtk_spin_button_get_value(GTK_SPIN_BUTTON(power_cycle_time_spinbutton_ccc));
    timer_settings_ccc.general_timeout = gtk_spin_button_get_value(GTK_SPIN_BUTTON(general_timeout_spinbutton_ccc));
    timer_settings_ccc.stop_on_power_cycle = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (stop_on_power_cycle_radio_button_ccc));
    timer_settings_ccc.call_power_command = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (call_power_command_radio_button_ccc));
    timer_settings_ccc.power_cycle_primary_relay = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (primary_relay_power_cycle_radio_button_ccc));

    timer_settings_ccc.p12_soft_reset_time = gtk_spin_button_get_value(GTK_SPIN_BUTTON(p12_soft_reset_time_spinbutton_ccc));
    timer_settings_ccc.p3_soft_reset_time = gtk_spin_button_get_value(GTK_SPIN_BUTTON(p3_soft_reset_time_spinbutton_ccc));
    timer_settings_ccc.p4_soft_reset_time = gtk_spin_button_get_value(GTK_SPIN_BUTTON(p4_soft_reset_time_spinbutton_ccc));
    timer_settings_ccc.td_soft_reset_time = gtk_spin_button_get_value(GTK_SPIN_BUTTON(td_soft_reset_time_spinbutton_ccc));
    timer_settings_ccc.d2_soft_reset_time = gtk_spin_button_get_value(GTK_SPIN_BUTTON(d2_soft_reset_time_spinbutton_ccc));
    timer_settings_ccc.sc_soft_reset_time = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sc_soft_reset_time_spinbutton_ccc));
    timer_settings_ccc.rt_soft_reset_time = gtk_spin_button_get_value(GTK_SPIN_BUTTON(rt_soft_reset_time_spinbutton_ccc));

    timer_settings_ccc.always_wait_for_reset_timers = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (always_wait_for_reset_timers_checkbutton_ccc));

    timer_settings_ccc.usb_bulk_reset_enabled = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (enable_usb_bulk_reset_checkbutton_ccc));
    timer_settings_ccc.usb_soft_reset_enabled = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (enable_usb_soft_reset_checkbutton_ccc));
    timer_settings_ccc.usb_hard_reset_enabled = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (enable_usb_hard_reset_checkbutton_ccc));
    timer_settings_ccc.usb_port_reset_enabled = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (enable_usb_port_reset_checkbutton_ccc));

    const gchar *entry_text;
    entry_text = gtk_entry_get_text (GTK_ENTRY (power_command_to_call_text_ccc));
    strlcpy (timer_settings_ccc.power_command_to_call, entry_text, sizeof(timer_settings_ccc.power_command_to_call));
    update_timer_settings_ccc();
    logfile_changed_ccc = true;
  }
  gtk_widget_destroy(dialog);
  update_display_ccc(0);
}





int open_ports_dialog_ccc (char *current_ports)
{
  disabled_ports_changed_ccc = 0;
  int confirm = 0;
  GtkBuilder *builder = gtk_builder_new();
  GError *err = NULL; // It is mandatory to initialize to NULL
  if(0 == gtk_builder_add_from_string (builder, (const gchar *)hddsuperclone2_glade, hddsuperclone2_glade_len, &err))
  {
    fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
  }
  gtk_builder_connect_signals(builder, NULL);
  GtkWidget *dialog = GTK_WIDGET (gtk_builder_get_object (builder, "disable_ports_dialog"));
  disable_ports_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "disable_ports_label"));
  disable_ports_info_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "disable_ports_info_label"));
  current_ports_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "current_ports_label"));
  current_ports_info_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "current_ports_info_label"));
  set_disabled_ports_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "set_disabled_ports_label"));
  set_disabled_ports_text_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "set_disabled_ports_text"));
  updated_ports_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "updated_ports_label"));
  updated_ports_info_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "updated_ports_info_label"));
  update_ports_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "update_ports_button"));
  // = GTK_WIDGET (gtk_builder_get_object (builder, "")); // template
  g_object_unref(builder);

  strcpy (new_ports_ccc, "");
  char portsinfo[4096] = "";
  strlcpy (portsinfo, curlang_ccc[LANGDISABLEPORTSINFO1], sizeof(portsinfo));
  strcat (portsinfo, curlang_ccc[LANGDISABLEPORTSINFO2]);
  gtk_label_set_text(GTK_LABEL(disable_ports_info_label_ccc), portsinfo);
  gtk_label_set_text(GTK_LABEL(disable_ports_label_ccc), curlang_ccc[LANGDISABLEPORTS]);
  gtk_label_set_text(GTK_LABEL(current_ports_label_ccc), curlang_ccc[LANGCURRENTDISPORTS]);
  gtk_label_set_text(GTK_LABEL(current_ports_info_label_ccc), current_ports);
  gtk_label_set_text(GTK_LABEL(set_disabled_ports_label_ccc), curlang_ccc[LANGSETDISPORTS]);
  gtk_label_set_text(GTK_LABEL(updated_ports_label_ccc), curlang_ccc[LANGUPDATEDDISPORTS]);
  gtk_label_set_text(GTK_LABEL(updated_ports_info_label_ccc), current_ports);
  gtk_button_set_label(GTK_BUTTON(update_ports_button_ccc), curlang_ccc[LANGUPDATEPORTS]);
  //gtk_button_set_label(GTK_BUTTON(), curlang_ccc[]);
  //gtk_label_set_text(GTK_LABEL(), curlang_ccc[]);

  gtk_window_set_title(GTK_WINDOW(dialog), curlang_ccc[LANGDISABLEPORTS]);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == 5)
  {
    if (disabled_ports_changed_ccc)
    {
      confirm = 1;
    }
  }
  gtk_widget_destroy(dialog);
  return confirm;
}





int set_lun_dialog_ccc (int max_lun)
{
  GtkBuilder *builder = gtk_builder_new();
  GError *err = NULL; // It is mandatory to initialize to NULL
  if(0 == gtk_builder_add_from_string (builder, (const gchar *)hddsuperclone2_glade, hddsuperclone2_glade_len, &err))
  {
    fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
  }
  gtk_builder_connect_signals(builder, NULL);
  GtkWidget *dialog = GTK_WIDGET (gtk_builder_get_object (builder, "select_lun_dialog"));
  select_lun_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "select_lun_label"));
  select_lun_info_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "select_lun_info_label"));
  lun_values_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "lun_values_label"));
  lun_values_info_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "lun_values_info_label"));
  set_current_lun_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "set_current_lun_label"));
  current_lun_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "current_lun_spinbutton"));
  // = GTK_WIDGET (gtk_builder_get_object (builder, "")); // template
  g_object_unref(builder);

  char luninfo[256] = "";
  snprintf (luninfo, sizeof(luninfo), "0 - %d", max_lun);
  gtk_label_set_text(GTK_LABEL(select_lun_label_ccc), curlang_ccc[LANGSELECTLUN]);
  gtk_label_set_text(GTK_LABEL(select_lun_info_label_ccc), curlang_ccc[LANGSELECTLUNINFO]);
  gtk_label_set_text(GTK_LABEL(lun_values_label_ccc), curlang_ccc[LANGLUNVALUES]);
  gtk_label_set_text(GTK_LABEL(lun_values_info_label_ccc), luninfo);
  gtk_label_set_text(GTK_LABEL(set_current_lun_label_ccc), curlang_ccc[LANGSETLUN]);
  //gtk_button_set_label(GTK_BUTTON(), curlang_ccc[]);
  //gtk_label_set_text(GTK_LABEL(), curlang_ccc[]);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 0, max_lun, 1, 1, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(current_lun_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(current_lun_spinbutton_ccc), usb_lun_ccc);

  gtk_window_set_title(GTK_WINDOW(dialog), curlang_ccc[LANGSELECTLUN]);

  int ret = 1;
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == 5)
  {
    usb_lun_ccc = gtk_spin_button_get_value(GTK_SPIN_BUTTON(current_lun_spinbutton_ccc));
    usb_lun_set_ccc = 1;
    ret = 0;
    fprintf (stdout, "lun=%d\n", usb_lun_ccc);
  }
  gtk_widget_destroy(dialog);
  return ret;
}





void open_primary_relay_dialog_ccc (void)
{
  GtkBuilder *builder = gtk_builder_new();
  GError *err = NULL; // It is mandatory to initialize to NULL
  if(0 == gtk_builder_add_from_string (builder, (const gchar *)hddsuperclone2_glade, hddsuperclone2_glade_len, &err))
  {
    fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
  }
  gtk_builder_connect_signals(builder, NULL);
  GtkWidget *dialog = GTK_WIDGET (gtk_builder_get_object (builder, "primary_relay_settings_dialog"));
  primary_relay_settings_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "primary_relay_settings_label"));
  primary_relay_settings_info_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "primary_relay_settings_info_label"));
  label_current_primary_relay_board_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "label_current_primary_relay_board"));
  data_current_relay_board_a_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "data_current_relay_board_a"));
  primary_relay_board_settings_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "primary_relay_board_settings_label"));
  primary_relay_activation_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "primary_relay_activation_label"));
  activate_primary_relay1_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "activate_primary_relay1_checkbutton"));
  activate_primary_relay2_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "activate_primary_relay2_checkbutton"));
  activate_primary_relay3_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "activate_primary_relay3_checkbutton"));
  activate_primary_relay4_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "activate_primary_relay4_checkbutton"));
  activate_primary_relay5_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "activate_primary_relay5_checkbutton"));
  activate_primary_relay6_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "activate_primary_relay6_checkbutton"));
  activate_primary_relay7_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "activate_primary_relay7_checkbutton"));
  activate_primary_relay8_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "activate_primary_relay8_checkbutton"));
  primary_relay_deactivation_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "primary_relay_deactivation_label"));
  deactivate_primary_relay1_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "deactivate_primary_relay1_checkbutton"));
  deactivate_primary_relay2_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "deactivate_primary_relay2_checkbutton"));
  deactivate_primary_relay3_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "deactivate_primary_relay3_checkbutton"));
  deactivate_primary_relay4_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "deactivate_primary_relay4_checkbutton"));
  deactivate_primary_relay5_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "deactivate_primary_relay5_checkbutton"));
  deactivate_primary_relay6_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "deactivate_primary_relay6_checkbutton"));
  deactivate_primary_relay7_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "deactivate_primary_relay7_checkbutton"));
  deactivate_primary_relay8_checkbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "deactivate_primary_relay8_checkbutton"));
  primary_relay_activate_time_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "primary_relay_activate_time_label"));
  primary_relay_energize_time_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "primary_relay_energize_time_spinbutton"));
  primary_relay_delay_time_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "primary_relay_delay_time_label"));
  primary_relay_delay_time_spinbutton_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "primary_relay_delay_time_spinbutton"));
  test_primary_relay_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "test_primary_relay_button"));
  activate_primary_relay_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "activate_primary_relay_button"));
  deactivate_primary_relay_button_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "deactivate_primary_relay_button"));
  // = GTK_WIDGET (gtk_builder_get_object (builder, "")); // template
  g_object_unref(builder);

  gtk_label_set_text(GTK_LABEL(primary_relay_settings_label_ccc), "");
  gtk_label_set_text(GTK_LABEL(primary_relay_settings_info_label_ccc), curlang_ccc[LANGPRIMARYRELAYINFO]);
  gtk_label_set_text(GTK_LABEL(label_current_primary_relay_board_ccc), curlang_ccc[LANGCURRENTRELAY]);
  gtk_label_set_text(GTK_LABEL(primary_relay_board_settings_label_ccc), curlang_ccc[LANGRELAYBOARDSETTINGS]);
  gtk_label_set_text(GTK_LABEL(primary_relay_activation_label_ccc), curlang_ccc[LANGRELAYACTIVATION]);
  gtk_button_set_label(GTK_BUTTON(activate_primary_relay1_checkbutton_ccc), curlang_ccc[LANGACTIVATERELAY1]);
  gtk_button_set_label(GTK_BUTTON(activate_primary_relay2_checkbutton_ccc), curlang_ccc[LANGACTIVATERELAY2]);
  gtk_button_set_label(GTK_BUTTON(activate_primary_relay3_checkbutton_ccc), curlang_ccc[LANGACTIVATERELAY3]);
  gtk_button_set_label(GTK_BUTTON(activate_primary_relay4_checkbutton_ccc), curlang_ccc[LANGACTIVATERELAY4]);
  gtk_button_set_label(GTK_BUTTON(activate_primary_relay5_checkbutton_ccc), curlang_ccc[LANGACTIVATERELAY5]);
  gtk_button_set_label(GTK_BUTTON(activate_primary_relay6_checkbutton_ccc), curlang_ccc[LANGACTIVATERELAY6]);
  gtk_button_set_label(GTK_BUTTON(activate_primary_relay7_checkbutton_ccc), curlang_ccc[LANGACTIVATERELAY7]);
  gtk_button_set_label(GTK_BUTTON(activate_primary_relay8_checkbutton_ccc), curlang_ccc[LANGACTIVATERELAY8]);
  gtk_label_set_text(GTK_LABEL(primary_relay_deactivation_label_ccc), curlang_ccc[LANGRELAYDEACTIVATION]);
  gtk_button_set_label(GTK_BUTTON(deactivate_primary_relay1_checkbutton_ccc), curlang_ccc[LANGACTIVATERELAY1]);
  gtk_button_set_label(GTK_BUTTON(deactivate_primary_relay2_checkbutton_ccc), curlang_ccc[LANGACTIVATERELAY2]);
  gtk_button_set_label(GTK_BUTTON(deactivate_primary_relay3_checkbutton_ccc), curlang_ccc[LANGACTIVATERELAY3]);
  gtk_button_set_label(GTK_BUTTON(deactivate_primary_relay4_checkbutton_ccc), curlang_ccc[LANGACTIVATERELAY4]);
  gtk_button_set_label(GTK_BUTTON(deactivate_primary_relay5_checkbutton_ccc), curlang_ccc[LANGACTIVATERELAY5]);
  gtk_button_set_label(GTK_BUTTON(deactivate_primary_relay6_checkbutton_ccc), curlang_ccc[LANGACTIVATERELAY6]);
  gtk_button_set_label(GTK_BUTTON(deactivate_primary_relay7_checkbutton_ccc), curlang_ccc[LANGACTIVATERELAY7]);
  gtk_button_set_label(GTK_BUTTON(deactivate_primary_relay8_checkbutton_ccc), curlang_ccc[LANGACTIVATERELAY8]);
  gtk_label_set_text(GTK_LABEL(primary_relay_activate_time_label_ccc), curlang_ccc[LANGRELAYACTIVATETIME]);
  gtk_label_set_text(GTK_LABEL(primary_relay_delay_time_label_ccc), curlang_ccc[LANGRELAYDELAYTIME]);
  gtk_button_set_label(GTK_BUTTON(test_primary_relay_button_ccc), curlang_ccc[LANGTESTRELAYBUTTON]);
  gtk_button_set_label(GTK_BUTTON(activate_primary_relay_button_ccc), curlang_ccc[LANGACTIVATE]);
  gtk_button_set_label(GTK_BUTTON(deactivate_primary_relay_button_ccc), curlang_ccc[LANGDEACTIVATE]);
  //gtk_button_set_label(GTK_BUTTON(), curlang_ccc[]);
  //gtk_label_set_text(GTK_LABEL(), curlang_ccc[]);

  //gtk_entry_set_max_length (GTK_ENTRY (power_command_to_call_text_ccc), MAX_CALL_LENGTH);

  //g_signal_connect(G_OBJECT(phase_timers_checkbutton_ccc), "toggled", G_CALLBACK(set_state_from_button_ccc), GINT_TO_POINTER(BUTTONID_PHASETIMERS));

  load_primary_relay_settings_ccc();
  gtk_label_set_text(GTK_LABEL(data_current_relay_board_a_ccc), primary_relay_settings_ccc.primary_relay_name);
  update_primary_relay_button_settings_ccc();


  gtk_window_set_title(GTK_WINDOW(dialog), curlang_ccc[LANGPRIMARYRELAY]);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == 5)
  {
    update_primary_relay_settings_from_buttons_ccc();

    update_primary_relay_settings_ccc();
    logfile_changed_ccc = true;
  }
  gtk_widget_destroy(dialog);
  update_display_ccc(0);
}

void update_primary_relay_settings_from_buttons_ccc (void)
{
  primary_relay_settings_ccc.activate_primary_relay1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(activate_primary_relay1_checkbutton_ccc));
  primary_relay_settings_ccc.activate_primary_relay2 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(activate_primary_relay2_checkbutton_ccc));
  primary_relay_settings_ccc.activate_primary_relay3 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(activate_primary_relay3_checkbutton_ccc));
  primary_relay_settings_ccc.activate_primary_relay4 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(activate_primary_relay4_checkbutton_ccc));
  primary_relay_settings_ccc.activate_primary_relay5 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(activate_primary_relay5_checkbutton_ccc));
  primary_relay_settings_ccc.activate_primary_relay6 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(activate_primary_relay6_checkbutton_ccc));
  primary_relay_settings_ccc.activate_primary_relay7 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(activate_primary_relay7_checkbutton_ccc));
  primary_relay_settings_ccc.activate_primary_relay8 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(activate_primary_relay8_checkbutton_ccc));
  primary_relay_settings_ccc.deactivate_primary_relay1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(deactivate_primary_relay1_checkbutton_ccc));
  primary_relay_settings_ccc.deactivate_primary_relay2 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(deactivate_primary_relay2_checkbutton_ccc));
  primary_relay_settings_ccc.deactivate_primary_relay3 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(deactivate_primary_relay3_checkbutton_ccc));
  primary_relay_settings_ccc.deactivate_primary_relay4 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(deactivate_primary_relay4_checkbutton_ccc));
  primary_relay_settings_ccc.deactivate_primary_relay5 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(deactivate_primary_relay5_checkbutton_ccc));
  primary_relay_settings_ccc.deactivate_primary_relay6 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(deactivate_primary_relay6_checkbutton_ccc));
  primary_relay_settings_ccc.deactivate_primary_relay7 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(deactivate_primary_relay7_checkbutton_ccc));
  primary_relay_settings_ccc.deactivate_primary_relay8 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(deactivate_primary_relay8_checkbutton_ccc));
  primary_relay_settings_ccc.primary_relay_activation_time = gtk_spin_button_get_value(GTK_SPIN_BUTTON(primary_relay_energize_time_spinbutton_ccc));
  primary_relay_settings_ccc.primary_relay_delay_time = gtk_spin_button_get_value(GTK_SPIN_BUTTON(primary_relay_delay_time_spinbutton_ccc));

  // = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON());
  // = gtk_spin_button_get_value(GTK_SPIN_BUTTON());
}



void do_test_primary_relay_ccc (void)
{
  if (!usbr1_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNORELAYCHOSEN], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return;
  }
  int error = 0;
  bool activate_primary_relay1_bak = primary_relay_settings_ccc.activate_primary_relay1;
  bool activate_primary_relay2_bak = primary_relay_settings_ccc.activate_primary_relay2;
  bool activate_primary_relay3_bak = primary_relay_settings_ccc.activate_primary_relay3;
  bool activate_primary_relay4_bak = primary_relay_settings_ccc.activate_primary_relay4;
  bool activate_primary_relay5_bak = primary_relay_settings_ccc.activate_primary_relay5;
  bool activate_primary_relay6_bak = primary_relay_settings_ccc.activate_primary_relay6;
  bool activate_primary_relay7_bak = primary_relay_settings_ccc.activate_primary_relay7;
  bool activate_primary_relay8_bak = primary_relay_settings_ccc.activate_primary_relay8;
  bool deactivate_primary_relay1_bak = primary_relay_settings_ccc.deactivate_primary_relay1;
  bool deactivate_primary_relay2_bak = primary_relay_settings_ccc.deactivate_primary_relay2;
  bool deactivate_primary_relay3_bak = primary_relay_settings_ccc.deactivate_primary_relay3;
  bool deactivate_primary_relay4_bak = primary_relay_settings_ccc.deactivate_primary_relay4;
  bool deactivate_primary_relay5_bak = primary_relay_settings_ccc.deactivate_primary_relay5;
  bool deactivate_primary_relay6_bak = primary_relay_settings_ccc.deactivate_primary_relay6;
  bool deactivate_primary_relay7_bak = primary_relay_settings_ccc.deactivate_primary_relay7;
  bool deactivate_primary_relay8_bak = primary_relay_settings_ccc.deactivate_primary_relay8;
  unsigned long long primary_relay_activation_time_bak = primary_relay_settings_ccc.primary_relay_activation_time;
  unsigned long long primary_relay_delay_time_bak = primary_relay_settings_ccc.primary_relay_delay_time;

  update_primary_relay_settings_from_buttons_ccc();
  update_primary_relay_settings_ccc();

  g_print ("activate primary relay\n");
  if (activate_primary_relay_ccc())
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGUSBRELAYERROR], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    error = 1;
  }

  // wait to deactivate
  if (!error)
  {
    unsigned int i;
    for (i = primary_relay_settings_ccc.primary_relay_activation_time; i > 0 ; i--)
    {
      g_print ("%d  ", i);
      do_nanosleep_ccc(1000000000);
      update_display_ccc(0);
    }
    g_print ("\n");
    g_print ("deactivate primary relay\n");
    if (deactivate_primary_relay_ccc())
    {
      strlcpy (tempmessage_ccc, curlang_ccc[LANGUSBRELAYERROR], sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      error = 1;
    }
  }

  // wait for delay time
  if (!error)
  {
    g_print ("wait for delay time\n");
    unsigned int i;
    for (i = primary_relay_settings_ccc.primary_relay_delay_time; i > 0 ; i--)
    {
      g_print ("%d  ", i);
      do_nanosleep_ccc(1000000000);
      update_display_ccc(0);
    }
    g_print ("\n");
    g_print ("relay test complete\n");
    strlcpy (tempmessage_ccc, curlang_ccc[LANGOPERATIONSUCCEEDED], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    clear_error_message_ccc();
  }

  primary_relay_settings_ccc.activate_primary_relay1 = activate_primary_relay1_bak;
  primary_relay_settings_ccc.activate_primary_relay2 = activate_primary_relay2_bak;
  primary_relay_settings_ccc.activate_primary_relay3 = activate_primary_relay3_bak;
  primary_relay_settings_ccc.activate_primary_relay4 = activate_primary_relay4_bak;
  primary_relay_settings_ccc.activate_primary_relay5 = activate_primary_relay5_bak;
  primary_relay_settings_ccc.activate_primary_relay6 = activate_primary_relay6_bak;
  primary_relay_settings_ccc.activate_primary_relay7 = activate_primary_relay7_bak;
  primary_relay_settings_ccc.activate_primary_relay8 = activate_primary_relay8_bak;
  primary_relay_settings_ccc.deactivate_primary_relay1 = deactivate_primary_relay1_bak;
  primary_relay_settings_ccc.deactivate_primary_relay2 = deactivate_primary_relay2_bak;
  primary_relay_settings_ccc.deactivate_primary_relay3 = deactivate_primary_relay3_bak;
  primary_relay_settings_ccc.deactivate_primary_relay4 = deactivate_primary_relay4_bak;
  primary_relay_settings_ccc.deactivate_primary_relay5 = deactivate_primary_relay5_bak;
  primary_relay_settings_ccc.deactivate_primary_relay6 = deactivate_primary_relay6_bak;
  primary_relay_settings_ccc.deactivate_primary_relay7 = deactivate_primary_relay7_bak;
  primary_relay_settings_ccc.deactivate_primary_relay8 = deactivate_primary_relay8_bak;
  primary_relay_settings_ccc.primary_relay_activation_time = primary_relay_activation_time_bak;
  primary_relay_settings_ccc.primary_relay_delay_time = primary_relay_delay_time_bak;
  update_primary_relay_settings_ccc();
}



int cycle_primary_relay_ccc(void)
{
  if (!usbr1_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNORELAYCHOSEN], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return -1;
  }

  g_print ("activate primary relay ");
  if (activate_primary_relay_ccc())
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGUSBRELAYERROR], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return -1;
  }

  // wait to deactivate
  unsigned int i;
  for (i = primary_relay_activation_time_ccc; i > 0 ; i--)
  {
    g_print ("%d ", i);
    do_nanosleep_ccc(1000000000);
  }
  g_print ("\n");
  g_print ("deactivate and wait ");
  if (deactivate_primary_relay_ccc())
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGUSBRELAYERROR], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return -1;
  }

  // wait for delay time
  for (i = primary_relay_delay_time_ccc; i > 0 ; i--)
  {
    g_print ("%d ", i);
    do_nanosleep_ccc(1000000000);
  }
  g_print ("done\n");

  return 0;
}



void update_ports_ccc (void)
{
  const gchar *entry_text;
  entry_text = gtk_entry_get_text (GTK_ENTRY (set_disabled_ports_text_ccc));
  //fprintf (stdout, "%s\n", entry_text);    //debug
  char line[256];
  char rawport[16][256];
  strncpy (line, entry_text, 255);
  strlcpy (new_ports_ccc, "libata.force=", sizeof(new_ports_ccc));
  int first = 1;
  int count = 0;
  int scanline = sscanf(line, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", rawport[0], rawport[1], rawport[2], rawport[3], rawport[4], rawport[5], rawport[6], rawport[7], rawport[8], rawport[9], rawport[10], rawport[11], rawport[12], rawport[13], rawport[14], rawport[15]);
  while (scanline > 0)
  {
    unsigned int port = strtoull(rawport[count], NULL, 0);
    char temp[256];
    if (first)
    {
      snprintf (temp, sizeof(temp), "%d:disable", port);
    }
    else
    {
      snprintf (temp, sizeof(temp), ",%d:disable", port);
    }
    strcat (new_ports_ccc, temp);
    first = 0;
    count++;
    scanline--;
  }
  gtk_label_set_text(GTK_LABEL(updated_ports_info_label_ccc), new_ports_ccc);
  disabled_ports_changed_ccc = 1;
}





int open_confirmation_dialog_ccc (char *message)
{
  int confirm = 0;
  GtkBuilder *builder = gtk_builder_new();
  GError *err = NULL; // It is mandatory to initialize to NULL
  if(0 == gtk_builder_add_from_string (builder, (const gchar *)hddsuperclone2_glade, hddsuperclone2_glade_len, &err))
  {
    fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
  }
  gtk_builder_connect_signals(builder, NULL);
  GtkWidget *dialog = GTK_WIDGET (gtk_builder_get_object (builder, "confirmation_dialog"));
  confirmation_box_label_ccc = GTK_WIDGET (gtk_builder_get_object (builder, "confirmation_box_label"));
  // = GTK_WIDGET (gtk_builder_get_object (builder, "")); // template
  g_object_unref(builder);

  gtk_label_set_text(GTK_LABEL(confirmation_box_label_ccc), message);
  //gtk_button_set_label(GTK_BUTTON(), curlang_ccc[]);
  //gtk_label_set_text(GTK_LABEL(), curlang_ccc[]);

  gtk_window_set_title(GTK_WINDOW(dialog), curlang_ccc[LANGCONFIRMOPERATION]);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == 5)
  {
    confirm = 1;
  }
  gtk_widget_destroy(dialog);
  return confirm;
}





void test_command_ccc (void)
{
  const gchar *entry_text;
  entry_text = gtk_entry_get_text (GTK_ENTRY (command_to_call_text_ccc));
  int ret = system (entry_text);
  if (ret)
  {
    int status = WEXITSTATUS(ret);
    int signal = WTERMSIG(ret);
    strlcpy (tempmessage_ccc, curlang_ccc[LANGCOMMANDFAILED], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), " (%d / %d)", signal, status);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGCOMMANDCOMPLETED], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    clear_error_message_ccc();
  }
}





void test_power_command_ccc (void)
{
  const gchar *entry_text;
  entry_text = gtk_entry_get_text (GTK_ENTRY (power_command_to_call_text_ccc));
  int ret = system (entry_text);
  if (ret)
  {
    int status = WEXITSTATUS(ret);
    int signal = WTERMSIG(ret);
    strlcpy (tempmessage_ccc, curlang_ccc[LANGCOMMANDFAILED], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), " (%d / %d)", signal, status);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGCOMMANDCOMPLETED], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    clear_error_message_ccc();
  }
}






void activate_status_buttons_ccc (void)
{
  gtk_widget_set_sensitive (GTK_WIDGET(button_bsy_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_drdy_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_df_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_dsc_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_drq_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_corr_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_idx_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_err_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_status_separator_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_bbk_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_unc_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_mc_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_idnf_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_mcr_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_abrt_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_tk0nf_ccc), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_amnf_ccc), TRUE);

  gdk_color_parse (GTKCOLOR_GRAY, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_status_separator_ccc), GTK_STATE_NORMAL, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_status_separator_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_status_separator_ccc), GTK_STATE_ACTIVE, &button_color_ccc);

  gdk_color_parse (GTKCOLOR_WHITE, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_bsy_ccc), GTK_STATE_NORMAL, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_drdy_ccc), GTK_STATE_NORMAL, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_df_ccc), GTK_STATE_NORMAL, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_dsc_ccc), GTK_STATE_NORMAL, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_drq_ccc), GTK_STATE_NORMAL, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_corr_ccc), GTK_STATE_NORMAL, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_idx_ccc), GTK_STATE_NORMAL, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_err_ccc), GTK_STATE_NORMAL, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_bbk_ccc), GTK_STATE_NORMAL, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_unc_ccc), GTK_STATE_NORMAL, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_mc_ccc), GTK_STATE_NORMAL, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_idnf_ccc), GTK_STATE_NORMAL, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_mcr_ccc), GTK_STATE_NORMAL, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_abrt_ccc), GTK_STATE_NORMAL, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_tk0nf_ccc), GTK_STATE_NORMAL, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_amnf_ccc), GTK_STATE_NORMAL, &button_color_ccc);

  gtk_widget_modify_bg ( GTK_WIDGET(button_bsy_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_drdy_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_df_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_dsc_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_drq_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_corr_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_idx_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_err_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_bbk_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_unc_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_mc_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_idnf_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_mcr_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_abrt_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_tk0nf_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_amnf_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);

  gtk_widget_modify_bg ( GTK_WIDGET(button_bsy_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_drdy_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_df_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_dsc_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_drq_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_corr_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_idx_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_err_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_bbk_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_unc_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_mc_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_idnf_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_mcr_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_abrt_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_tk0nf_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
  gtk_widget_modify_bg ( GTK_WIDGET(button_amnf_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
}





void deactivate_status_buttons_ccc (void)
{
  gtk_widget_set_sensitive (GTK_WIDGET(button_bsy_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_drdy_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_df_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_dsc_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_drq_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_corr_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_idx_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_err_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_status_separator_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_bbk_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_unc_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_mc_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_idnf_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_mcr_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_abrt_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_tk0nf_ccc), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET(button_amnf_ccc), FALSE);
}





void update_status_buttons_ccc (void)
{
  if (!scsi_passthrough_ccc && !fill_mode_ccc && !generic_mode_ccc)
  {
    if (color_statusbar_ccc)
    {
      activate_status_buttons_ccc();
      if (connected_ccc)
      {
        gdk_color_parse (GTKCOLOR_LIME, &button_color_ccc);
        if (ata_status_ccc & 0x80)
        {
          gtk_widget_modify_bg ( GTK_WIDGET(button_bsy_ccc), GTK_STATE_NORMAL, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_bsy_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_bsy_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
        }
        if (ata_status_ccc & 0x40)
        {
          gtk_widget_modify_bg ( GTK_WIDGET(button_drdy_ccc), GTK_STATE_NORMAL, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_drdy_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_drdy_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
        }
        if (ata_status_ccc & 0x20)
        {
          gtk_widget_modify_bg ( GTK_WIDGET(button_df_ccc), GTK_STATE_NORMAL, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_df_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_df_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
        }
        if (ata_status_ccc & 0x10)
        {
          gtk_widget_modify_bg ( GTK_WIDGET(button_dsc_ccc), GTK_STATE_NORMAL, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_dsc_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_dsc_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
        }
        if (ata_status_ccc & 0x08)
        {
          gtk_widget_modify_bg ( GTK_WIDGET(button_drq_ccc), GTK_STATE_NORMAL, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_drq_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_drq_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
        }
        if (ata_status_ccc & 0x04)
        {
          gtk_widget_modify_bg ( GTK_WIDGET(button_corr_ccc), GTK_STATE_NORMAL, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_corr_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_corr_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
        }
        if (ata_status_ccc & 0x02)
        {
          gtk_widget_modify_bg ( GTK_WIDGET(button_idx_ccc), GTK_STATE_NORMAL, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_idx_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_idx_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
        }
        if (ata_status_ccc & 0x01)
        {
          gtk_widget_modify_bg ( GTK_WIDGET(button_err_ccc), GTK_STATE_NORMAL, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_err_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_err_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
        }

        gdk_color_parse (GTKCOLOR_RED, &button_color_ccc);
        if (ata_error_ccc & 0x80)
        {
          gtk_widget_modify_bg ( GTK_WIDGET(button_bbk_ccc), GTK_STATE_NORMAL, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_bbk_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_bbk_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
        }
        if (ata_error_ccc & 0x40)
        {
          gtk_widget_modify_bg ( GTK_WIDGET(button_unc_ccc), GTK_STATE_NORMAL, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_unc_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_unc_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
        }
        if (ata_error_ccc & 0x20)
        {
          gtk_widget_modify_bg ( GTK_WIDGET(button_mc_ccc), GTK_STATE_NORMAL, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_mc_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_mc_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
        }
        if (ata_error_ccc & 0x10)
        {
          gtk_widget_modify_bg ( GTK_WIDGET(button_idnf_ccc), GTK_STATE_NORMAL, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_idnf_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_idnf_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
        }
        if (ata_error_ccc & 0x08)
        {
          gtk_widget_modify_bg ( GTK_WIDGET(button_mcr_ccc), GTK_STATE_NORMAL, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_mcr_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_mcr_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
        }
        if (ata_error_ccc & 0x04)
        {
          gtk_widget_modify_bg ( GTK_WIDGET(button_abrt_ccc), GTK_STATE_NORMAL, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_abrt_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_abrt_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
        }
        if (ata_error_ccc & 0x02)
        {
          gtk_widget_modify_bg ( GTK_WIDGET(button_tk0nf_ccc), GTK_STATE_NORMAL, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_tk0nf_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_tk0nf_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
        }
        if (ata_error_ccc & 0x01)
        {
          gtk_widget_modify_bg ( GTK_WIDGET(button_amnf_ccc), GTK_STATE_NORMAL, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_amnf_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
          gtk_widget_modify_bg ( GTK_WIDGET(button_amnf_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
        }
        if (color_switch_flag_ccc == 0)
        {
          color_switch_flag_ccc = 1;
          gdk_color_parse ("#000", &button_color_ccc);
        }
        else if (color_switch_flag_ccc == 1)
        {
          color_switch_flag_ccc = 2;
          gdk_color_parse ("#777", &button_color_ccc);
        }
        else
        {
          color_switch_flag_ccc = 0;
          gdk_color_parse ("#fff", &button_color_ccc);
        }
        gtk_widget_modify_bg ( GTK_WIDGET(button_status_separator_ccc), GTK_STATE_NORMAL, &button_color_ccc);
        gtk_widget_modify_bg ( GTK_WIDGET(button_status_separator_ccc), GTK_STATE_PRELIGHT, &button_color_ccc);
        gtk_widget_modify_bg ( GTK_WIDGET(button_status_separator_ccc), GTK_STATE_ACTIVE, &button_color_ccc);
      }
    }
    else
    {
      deactivate_status_buttons_ccc();
      if (connected_ccc)
      {
        if (ata_status_ccc & 0x80)
        {
          gtk_widget_set_sensitive (GTK_WIDGET(button_bsy_ccc), TRUE);
        }
        if (ata_status_ccc & 0x40)
        {
          gtk_widget_set_sensitive (GTK_WIDGET(button_drdy_ccc), TRUE);
        }
        if (ata_status_ccc & 0x20)
        {
          gtk_widget_set_sensitive (GTK_WIDGET(button_df_ccc), TRUE);
        }
        if (ata_status_ccc & 0x10)
        {
          gtk_widget_set_sensitive (GTK_WIDGET(button_dsc_ccc), TRUE);
        }
        if (ata_status_ccc & 0x08)
        {
          gtk_widget_set_sensitive (GTK_WIDGET(button_drq_ccc), TRUE);
        }
        if (ata_status_ccc & 0x04)
        {
          gtk_widget_set_sensitive (GTK_WIDGET(button_corr_ccc), TRUE);
        }
        if (ata_status_ccc & 0x02)
        {
          gtk_widget_set_sensitive (GTK_WIDGET(button_idx_ccc), TRUE);
        }
        if (ata_status_ccc & 0x01)
        {
          gtk_widget_set_sensitive (GTK_WIDGET(button_err_ccc), TRUE);
        }

        if (ata_error_ccc & 0x80)
        {
          gtk_widget_set_sensitive (GTK_WIDGET(button_bbk_ccc), TRUE);
        }
        if (ata_error_ccc & 0x40)
        {
          gtk_widget_set_sensitive (GTK_WIDGET(button_unc_ccc), TRUE);
        }
        if (ata_error_ccc & 0x20)
        {
          gtk_widget_set_sensitive (GTK_WIDGET(button_mc_ccc), TRUE);
        }
        if (ata_error_ccc & 0x10)
        {
          gtk_widget_set_sensitive (GTK_WIDGET(button_idnf_ccc), TRUE);
        }
        if (ata_error_ccc & 0x08)
        {
          gtk_widget_set_sensitive (GTK_WIDGET(button_mcr_ccc), TRUE);
        }
        if (ata_error_ccc & 0x04)
        {
          gtk_widget_set_sensitive (GTK_WIDGET(button_abrt_ccc), TRUE);
        }
        if (ata_error_ccc & 0x02)
        {
          gtk_widget_set_sensitive (GTK_WIDGET(button_tk0nf_ccc), TRUE);
        }
        if (ata_error_ccc & 0x01)
        {
          gtk_widget_set_sensitive (GTK_WIDGET(button_amnf_ccc), TRUE);
        }
        if (color_switch_flag_ccc == 0)
        {
          color_switch_flag_ccc = 1;
          gtk_widget_set_sensitive (GTK_WIDGET(button_status_separator_ccc), TRUE);
          gtk_button_set_label(GTK_BUTTON(button_status_separator_ccc), "\\");
        }
        else if (color_switch_flag_ccc == 1)
        {
          color_switch_flag_ccc = 2;
          gtk_widget_set_sensitive (GTK_WIDGET(button_status_separator_ccc), FALSE);
          gtk_button_set_label(GTK_BUTTON(button_status_separator_ccc), "|");
        }
        else
        {
          color_switch_flag_ccc = 0;
          gtk_widget_set_sensitive (GTK_WIDGET(button_status_separator_ccc), TRUE);
          gtk_button_set_label(GTK_BUTTON(button_status_separator_ccc), "/");
        }
      }
    }
  }
}





void update_mode_ccc (void)
{
  if (generic_mode_ccc || scsi_passthrough_ccc || nvme_passthrough_ccc || fill_mode_ccc)
  {
    deactivate_status_buttons_ccc();
  }
  else
  {
    activate_status_buttons_ccc();
  }
  if (fill_mode_ccc)
  {
    gtk_widget_set_sensitive (GTK_WIDGET(sourcemi_ccc), FALSE);
  }
  else
  {
    gtk_widget_set_sensitive (GTK_WIDGET(sourcemi_ccc), TRUE);
  }
  update_display_ccc(0);
}





void update_clone_button_settings_ccc (void)
{
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (phase1_check_button_ccc), !clone_settings_ccc.no_phase1 );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (phase2_check_button_ccc), !clone_settings_ccc.no_phase2 );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (phase3_check_button_ccc), !clone_settings_ccc.no_phase3 );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (phase4_check_button_ccc), !clone_settings_ccc.no_phase4 );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (divide_check_button_ccc), !clone_settings_ccc.no_divide1);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (divide2_check_button_ccc), clone_settings_ccc.do_divide2);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (trim_check_button_ccc), !clone_settings_ccc.no_trim);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (scrape_check_button_ccc), !clone_settings_ccc.no_scrape);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (markbad_check_button_ccc), clone_settings_ccc.mark_bad_head);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (readbad_check_button_ccc), clone_settings_ccc.read_bad_head);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (rebuild_assist_check_button_ccc), clone_settings_ccc.rebuild_assist);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (reverse_check_button_ccc), clone_settings_ccc.reverse);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (skipfast_check_button_ccc), clone_settings_ccc.skip_fast);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 0, MAX_RETRIES, 1, 1, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(retries_spin_button_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(retries_spin_button_ccc), clone_settings_ccc.retries);

  int cluster_limit = MAX_CLUSTERSIZE;
  if (source_chosen_ccc)
  {
    cluster_limit = check_buffer_limit_ccc();
    if (cluster_limit < 0)
    {
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    }
    if (clone_settings_ccc.cluster_size > cluster_limit)
    {
      clone_settings_ccc.cluster_size = cluster_limit;
    }
  }
  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 1, cluster_limit, 8, 64, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(clustersize_spin_button_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(clustersize_spin_button_ccc), clone_settings_ccc.cluster_size);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 0, 9999999999999999.0, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(inputoffset_spin_button_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(inputoffset_spin_button_ccc), clone_settings_ccc.input_offset);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, -1, 9999999999999999.0, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(size_spin_button_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(size_spin_button_ccc), clone_settings_ccc.read_size);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 1, MAX_BLOCKSIZE, 1, 8, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(blocksize_spin_button_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(blocksize_spin_button_ccc), clone_settings_ccc.block_size);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 0, 9999999999999999.0, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(skipsize_spin_button_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(skipsize_spin_button_ccc), clone_settings_ccc.min_skip_size);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 0, 9999999999999999.0, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(maxskipsize_spin_button_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(maxskipsize_spin_button_ccc), clone_settings_ccc.max_skip_size);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, -1, 300000, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(skipthreshold_spin_button_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(skipthreshold_spin_button_ccc), clone_settings_ccc.skip_timeout / 1000);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, -1, 1000000, 10, 1000, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(rateskip_spin_button_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(rateskip_spin_button_ccc), clone_settings_ccc.rate_skip / 1000);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, -1, 1000000, 10, 1000, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(exitonslow_spin_button_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(exitonslow_spin_button_ccc), clone_settings_ccc.exit_on_slow / 1000);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, -1, 300, 1, 10, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(exitonslowtime_spin_button_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(exitonslowtime_spin_button_ccc), clone_settings_ccc.exit_slow_time / 1000);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, MIN_SECTORSIZE, MAX_SECTORSIZE, 8, 64, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(sectorsize_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(sectorsize_spinbutton_ccc), clone_settings_ccc.sector_size);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 0, MAX_BLOCKSIZE, 1, 8, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(alignment_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(alignment_spinbutton_ccc), clone_settings_ccc.block_offset);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 0, 10000000, 100, 1000, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(maxreadrate_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(maxreadrate_spinbutton_ccc), clone_settings_ccc.max_read_rate / 1000);

  //gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 1, 1, 1, 1, 0); // template
  //gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(), gtk_adjustment_ccc); // template
  //gtk_spin_button_set_value(GTK_SPIN_BUTTON(), clone_settings_ccc.); // template

}





void update_advanced_button_settings_ccc (void)
{
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (force_mounted_checkbutton_ccc), advanced_settings_ccc.force_mounted );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (force_dangerous_checkbutton_ccc), advanced_settings_ccc.force_dangerous );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (no_log_backup_checkbutton_ccc), advanced_settings_ccc.no_log_backup );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (enable_output_offset_checkbutton_ccc), advanced_settings_ccc.enable_output_offset );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (enable_current_position_checkbutton_ccc), advanced_settings_ccc.enable_current_position );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (stop_on_error_radio_button_ccc), advanced_settings_ccc.stop_on_error );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (call_command_radio_button_ccc), advanced_settings_ccc.call_command );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (primary_relay_on_error_radio_button_ccc), advanced_settings_ccc.activate_primary_relay );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (write_buffer_disable_radio_button_ccc), advanced_settings_ccc.write_buffer_disable );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (write_buffer_enable_radio_button_ccc), advanced_settings_ccc.write_buffer_enable );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (driver_return_error_radio_button_ccc), advanced_settings_ccc.driver_return_error );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (driver_return_zeros_radio_button_ccc), advanced_settings_ccc.driver_return_zeros );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (driver_return_marked_radio_button_ccc), advanced_settings_ccc.driver_return_marked );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (disable_identify_checkbutton_ccc), advanced_settings_ccc.disable_identify );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (pio_mode_checkbutton_ccc), advanced_settings_ccc.pio_mode );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (enable_rebuild_assist_checkbutton_ccc), advanced_settings_ccc.enable_rebuild_assist );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (enable_process_chunk_checkbutton_ccc), advanced_settings_ccc.enable_process_chunk );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (enable_read_twice_checkbutton_ccc), advanced_settings_ccc.enable_read_twice );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (enable_retry_connecting_checkbutton_ccc), advanced_settings_ccc.enable_retry_connecting );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (enable_scsi_write_checkbutton_ccc), advanced_settings_ccc.enable_scsi_write );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (enable_phase_log_checkbutton_ccc), advanced_settings_ccc.enable_phase_log );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (enable_output_sector_size_checkbutton_ccc), advanced_settings_ccc.enable_output_sector_size );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (driver_io_scsi_only_checkbutton_ccc), advanced_settings_ccc.driver_io_scsi_only );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (use_physical_sector_size_for_virtual_checkbutton_ccc), advanced_settings_ccc.use_physical_sector_size_for_virtual );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (use_color_statusbar_checkbutton_ccc), advanced_settings_ccc.color_statusbar );

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, -1, 9999999999999999.0, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(output_offset_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(output_offset_spinbutton_ccc), advanced_settings_ccc.output_offset);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, -1, 9999999999999999.0, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(current_position_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(current_position_spinbutton_ccc), advanced_settings_ccc.current_position);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, -128, 128, 8, 64, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(output_sector_size_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(output_sector_size_spinbutton_ccc), advanced_settings_ccc.output_sector_adjustment);

  int cluster_limit = MAX_CLUSTERSIZE;
  if (source_chosen_ccc)
  {
    cluster_limit = check_buffer_limit_ccc();
    if (cluster_limit < 0)
    {
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    }
  }
  if (clone_settings_ccc.cluster_size > 0 && cluster_limit > clone_settings_ccc.cluster_size)
  {
    cluster_limit = clone_settings_ccc.cluster_size;
  }
  if (advanced_settings_ccc.driver_minimum_cluster_size > cluster_limit)
  {
    advanced_settings_ccc.driver_minimum_cluster_size = cluster_limit;
  }
  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 1, cluster_limit, 8, 64, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(driver_minimum_cluster_size_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(driver_minimum_cluster_size_spinbutton_ccc), advanced_settings_ccc.driver_minimum_cluster_size);




  //gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (), advanced_settings_ccc. );

  //gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 1, 1, 1, 1, 0);
  //gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(), gtk_adjustment_ccc);
  //gtk_spin_button_set_value(GTK_SPIN_BUTTON(), advanced_settings_ccc.);

}





void update_timer_button_settings_ccc (void)
{
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (stop_on_power_cycle_radio_button_ccc), timer_settings_ccc.stop_on_power_cycle );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (call_power_command_radio_button_ccc), timer_settings_ccc.call_power_command );
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (primary_relay_power_cycle_radio_button_ccc), timer_settings_ccc.power_cycle_primary_relay );

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 500, 900000, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(initial_busy_wait_time_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(initial_busy_wait_time_spinbutton_ccc), (timer_settings_ccc.initial_busy_wait_time) );
  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 5000, 900000, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(busy_wait_time_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(busy_wait_time_spinbutton_ccc), timer_settings_ccc.busy_wait_time);
  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 500, 900000, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(soft_reset_time_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(soft_reset_time_spinbutton_ccc), timer_settings_ccc.soft_reset_time);
  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 0, 900000, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(hard_reset_time_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(hard_reset_time_spinbutton_ccc), timer_settings_ccc.hard_reset_time);
  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 0, 900000, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(power_cycle_time_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(power_cycle_time_spinbutton_ccc), timer_settings_ccc.power_cycle_time);
  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 10000, 900000, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(general_timeout_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(general_timeout_spinbutton_ccc), timer_settings_ccc.general_timeout);

  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (phase_timers_checkbutton_ccc), timer_settings_ccc.phase_timers );

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 50, 900000, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(p12_soft_reset_time_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(p12_soft_reset_time_spinbutton_ccc), timer_settings_ccc.p12_soft_reset_time);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 50, 900000, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(p3_soft_reset_time_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(p3_soft_reset_time_spinbutton_ccc), timer_settings_ccc.p3_soft_reset_time);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 50, 900000, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(p4_soft_reset_time_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(p4_soft_reset_time_spinbutton_ccc), timer_settings_ccc.p4_soft_reset_time);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 50, 900000, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(td_soft_reset_time_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(td_soft_reset_time_spinbutton_ccc), timer_settings_ccc.td_soft_reset_time);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 50, 900000, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(d2_soft_reset_time_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(d2_soft_reset_time_spinbutton_ccc), timer_settings_ccc.d2_soft_reset_time);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 50, 900000, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(sc_soft_reset_time_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(sc_soft_reset_time_spinbutton_ccc), timer_settings_ccc.sc_soft_reset_time);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 50, 900000, 1, 100, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(rt_soft_reset_time_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(rt_soft_reset_time_spinbutton_ccc), timer_settings_ccc.rt_soft_reset_time);

  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (always_wait_for_reset_timers_checkbutton_ccc), timer_settings_ccc.always_wait_for_reset_timers);

  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (enable_usb_bulk_reset_checkbutton_ccc), timer_settings_ccc.usb_bulk_reset_enabled);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (enable_usb_soft_reset_checkbutton_ccc), timer_settings_ccc.usb_soft_reset_enabled);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (enable_usb_hard_reset_checkbutton_ccc), timer_settings_ccc.usb_hard_reset_enabled);
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (enable_usb_port_reset_checkbutton_ccc), timer_settings_ccc.usb_port_reset_enabled);


  //gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (), timer_settings_ccc. ); // template

  //gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 1, 1, 1, 1, 0);
  //gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(), gtk_adjustment_ccc);
  //gtk_spin_button_set_value(GTK_SPIN_BUTTON(), timer_settings_ccc.);

}





void update_primary_relay_button_settings_ccc (void)
{
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(activate_primary_relay1_checkbutton_ccc), primary_relay_settings_ccc.activate_primary_relay1);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(activate_primary_relay2_checkbutton_ccc), primary_relay_settings_ccc.activate_primary_relay2);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(activate_primary_relay3_checkbutton_ccc), primary_relay_settings_ccc.activate_primary_relay3);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(activate_primary_relay4_checkbutton_ccc), primary_relay_settings_ccc.activate_primary_relay4);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(activate_primary_relay5_checkbutton_ccc), primary_relay_settings_ccc.activate_primary_relay5);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(activate_primary_relay6_checkbutton_ccc), primary_relay_settings_ccc.activate_primary_relay6);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(activate_primary_relay7_checkbutton_ccc), primary_relay_settings_ccc.activate_primary_relay7);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(activate_primary_relay8_checkbutton_ccc), primary_relay_settings_ccc.activate_primary_relay8);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (deactivate_primary_relay1_checkbutton_ccc), primary_relay_settings_ccc.deactivate_primary_relay1);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (deactivate_primary_relay2_checkbutton_ccc), primary_relay_settings_ccc.deactivate_primary_relay2);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (deactivate_primary_relay3_checkbutton_ccc), primary_relay_settings_ccc.deactivate_primary_relay3);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (deactivate_primary_relay4_checkbutton_ccc), primary_relay_settings_ccc.deactivate_primary_relay4);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (deactivate_primary_relay5_checkbutton_ccc), primary_relay_settings_ccc.deactivate_primary_relay5);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (deactivate_primary_relay6_checkbutton_ccc), primary_relay_settings_ccc.deactivate_primary_relay6);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (deactivate_primary_relay7_checkbutton_ccc), primary_relay_settings_ccc.deactivate_primary_relay7);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (deactivate_primary_relay8_checkbutton_ccc), primary_relay_settings_ccc.deactivate_primary_relay8);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 1, 900, 1, 1, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(primary_relay_energize_time_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(primary_relay_energize_time_spinbutton_ccc), primary_relay_settings_ccc.primary_relay_activation_time);

  gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 1, 900, 1, 1, 0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(primary_relay_delay_time_spinbutton_ccc), gtk_adjustment_ccc);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(primary_relay_delay_time_spinbutton_ccc), primary_relay_settings_ccc.primary_relay_delay_time);



  //gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON (), primary_relay_settings_ccc. );

  //gtk_adjustment_ccc = (GtkAdjustment *) gtk_adjustment_new (0, 1, 1, 1, 1, 0);
  //gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(), gtk_adjustment_ccc);
  //gtk_spin_button_set_value(GTK_SPIN_BUTTON(), primary_relay_settings_ccc.);

}





void set_agressive_driver_status_from_button_ccc (void)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(agressive_driver_checkbutton_ccc)))
  {
    aggressive_driver_ccc = TRUE;
    //fprintf (stdout, "agressive mode on\n");    //debug
  }
  else
  {
    aggressive_driver_ccc = FALSE;
    //fprintf (stdout, "agressive mode off\n");    //debug
  }
}





void set_state_from_button_ccc (GtkWidget *widget, gpointer data)
{
  int data_pointer = GPOINTER_TO_INT( data );
  bool button_status;
  bool previous_status;
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)))
  {
    button_status = TRUE;
  }
  else
  {
    button_status = FALSE;
  }
  //g_print ("status=%d, pointer=%d\n", button_status, data_pointer );


  switch (data_pointer)
  {
    case BUTTONID_PHASE1:
      //g_print ("phase1=%d\n", button_status);
      clone_settings_ccc.no_phase1 = !button_status;
      break;

    case BUTTONID_PHASE2:
      //g_print ("phase2=%d\n", button_status);
      clone_settings_ccc.no_phase2 = !button_status;
      break;

    case BUTTONID_PHASE3:
      //g_print ("phase3=%d\n", button_status);
      clone_settings_ccc.no_phase3 = !button_status;
      break;

    case BUTTONID_PHASE4 :
      //g_print ("phase4=%d\n", button_status);
      clone_settings_ccc.no_phase4 = !button_status;
      break;

    case BUTTONID_DIVIDE:
      //g_print ("divide=%d\n", button_status);
      clone_settings_ccc.no_divide1 = !button_status;
      break;

    case BUTTONID_DIVIDE2:
      //g_print ("divide2=%d\n", button_status);
      clone_settings_ccc.do_divide2 = button_status;
      break;

    case BUTTONID_TRIM:
      //g_print ("trim=%d\n", button_status);
      clone_settings_ccc.no_trim = !button_status;
      break;

    case BUTTONID_SCRAPE:
      //g_print ("scrape=%d\n", button_status);
      clone_settings_ccc.no_scrape = !button_status;
      break;

    case BUTTONID_MARKBAD:
      //g_print ("badhead=%d\n", button_status);
      clone_settings_ccc.mark_bad_head = button_status;
      break;

    case BUTTONID_READBAD:
      //g_print ("goodhead=%d\n", button_status);
      clone_settings_ccc.read_bad_head = button_status;
      break;

    case BUTTONID_REBUILDASSIST:
      clone_settings_ccc.rebuild_assist = button_status;
      break;

    case BUTTONID_REVERSE:
      //g_print ("reverse=%d\n", button_status);
      clone_settings_ccc.reverse = button_status;
      break;

    case BUTTONID_SKIPFAST:
      //g_print ("skipfast=%d\n", button_status);
      clone_settings_ccc.skip_fast = button_status;
      break;

    case BUTTONID_NOLOGBACK:
      //g_print ("nologback=%d\n", button_status);
      advanced_settings_ccc.no_log_backup = button_status;
      break;

    case BUTTONID_FORCE:
      //g_print ("force=%d\n", button_status);
      advanced_settings_ccc.force_mounted = button_status;
      break;

    case BUTTONID_FORCEDANGEROUS:
      advanced_settings_ccc.force_dangerous = button_status;
      break;

    case BUTTONID_DONTIDENTIFY:
      advanced_settings_ccc.disable_identify = button_status;
      break;

    case BUTTONID_PIOMODE:
      advanced_settings_ccc.pio_mode = button_status;
      break;

    case BUTTONID_ENABLEREBUILDASSIST:
      advanced_settings_ccc.enable_rebuild_assist = button_status;
      break;

    case BUTTONID_ENABLEPROCESSCHUNK:
      advanced_settings_ccc.enable_process_chunk = button_status;
      break;

    case BUTTONID_ENABLEREADTWICE:
      advanced_settings_ccc.enable_read_twice = button_status;
      break;

    case BUTTONID_ENABLERETRYCONNECTING:
      advanced_settings_ccc.enable_retry_connecting = button_status;
      break;

    case BUTTONID_ENABLESCSIWRITE:
      advanced_settings_ccc.enable_scsi_write = button_status;
      break;

    case BUTTONID_ENABLEPHASELOGS:
      advanced_settings_ccc.enable_phase_log = button_status;
      break;

    case BUTTONID_PHASETIMERS:
      //g_print ("phasetimers=%d\n", button_status);
      timer_settings_ccc.phase_timers = button_status;
      break;

    case BUTTONID_OFFSET:
      //g_print ("offset=%d\n", button_status);
      previous_status = advanced_settings_ccc.enable_output_offset;
      advanced_settings_ccc.enable_output_offset = button_status;
      if (previous_status != advanced_settings_ccc.enable_output_offset)
      {
        snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s", curlang_ccc[LANGREOPENADVANCED]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
        clear_error_message_ccc();
      }
      break;

    case BUTTONID_POSITION:
      //g_print ("offset=%d\n", button_status);
      previous_status = advanced_settings_ccc.enable_current_position;
      advanced_settings_ccc.enable_current_position = button_status;
      if (previous_status != advanced_settings_ccc.enable_current_position)
      {
        snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s", curlang_ccc[LANGREOPENADVANCED]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
        clear_error_message_ccc();
      }
      break;

    case BUTTONID_ENABLEOUTPUTSECTOR:
      //g_print ("offset=%d\n", button_status);
      previous_status = advanced_settings_ccc.enable_output_sector_size;
      advanced_settings_ccc.enable_output_sector_size = button_status;
      if (previous_status != advanced_settings_ccc.enable_output_sector_size)
      {
        snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s", curlang_ccc[LANGREOPENADVANCED]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
        clear_error_message_ccc();
      }
      break;

    case BUTTONID_IOSCSIONLY:
      advanced_settings_ccc.driver_io_scsi_only = button_status;
      break;

    case BUTTONID_USEPHYSICALSECTORSIZE:
      advanced_settings_ccc.use_physical_sector_size_for_virtual = button_status;
      break;

    case BUTTONID_COLORSTATUSBAR:
      advanced_settings_ccc.color_statusbar = button_status;
      break;



    default:
      g_print ("Unknown button %d", data_pointer);
      break;
  }
}





void do_reset_status_ccc(void)
{
  if (!project_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNOPROJECT], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return;
  }
  GtkWidget *new_dialog_window;
  new_dialog_window = gtk_dialog_new_with_buttons (curlang_ccc[LANGRESETSTATUS], GTK_WINDOW(main_window_ccc), GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL);
  //gtk_widget_set_size_request (new_dialog_window, 400, 200);

  GtkWidget *new_label = gtk_label_new( curlang_ccc[LANGRESETSTATUSMESSAGE] );
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG(new_dialog_window)->vbox), new_label, TRUE, TRUE, 0);

  gtk_widget_show_all (new_dialog_window);
  gint result = gtk_dialog_run (GTK_DIALOG (new_dialog_window));
  gtk_widget_destroy (new_dialog_window);

  if (result == GTK_RESPONSE_ACCEPT)
  {
    g_print ("resetting status\n");
    reset_current_status_ccc();
    logfile_changed_ccc = true;
    update_display_ccc(0);
  }

}





void do_reset_log_status_ccc(void)
{
  if (!project_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNOPROJECT], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return;
  }
  GtkWidget *new_dialog_window;
  new_dialog_window = gtk_dialog_new_with_buttons (curlang_ccc[LANGRESETLOG], GTK_WINDOW(main_window_ccc), GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL);
  //gtk_widget_set_size_request (new_dialog_window, 400, 200);

  GtkWidget *new_label = gtk_label_new( curlang_ccc[LANGRESETLOGMESSAGE] );
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG(new_dialog_window)->vbox), new_label, TRUE, TRUE, 0);

  gtk_widget_show_all (new_dialog_window);
  gint result = gtk_dialog_run (GTK_DIALOG (new_dialog_window));
  gtk_widget_destroy (new_dialog_window);

  if (result == GTK_RESPONSE_ACCEPT)
  {
    g_print ("resetting log\n");
    reset_log_status_ccc();
    update_display_ccc(0);
    // check log and if same status found then repair it
    if (source_chosen_ccc)
    {
      check_log_size_ccc = 1;
    }
    else
    {
      check_log_size_ccc = 0;
    }
    repair_log_ccc = true;
    int ret = check_log_ccc();
    clear_error_message_ccc();
    if (ret == 0x10)
    {
      check_and_repair_log_ccc();
    }
    repair_log_ccc = false;
    logfile_changed_ccc = true;
    update_display_ccc(0);
  }

}





void do_repair_log_ccc(void)
{
  if (!project_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNOPROJECT], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return;
  }
  GtkWidget *new_dialog_window;
  new_dialog_window = gtk_dialog_new_with_buttons (curlang_ccc[LANGREPAIRLOG], GTK_WINDOW(main_window_ccc), GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL);
  //gtk_widget_set_size_request (new_dialog_window, 400, 200);

  GtkWidget *new_label = gtk_label_new( curlang_ccc[LANGREPAIRLOGMESSAGE] );
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG(new_dialog_window)->vbox), new_label, TRUE, TRUE, 0);

  gtk_widget_show_all (new_dialog_window);
  gint result = gtk_dialog_run (GTK_DIALOG (new_dialog_window));
  gtk_widget_destroy (new_dialog_window);

  if (result == GTK_RESPONSE_ACCEPT)
  {
    g_print ("repairing log\n");
    if (source_chosen_ccc)
    {
      check_log_size_ccc = 1;
    }
    else
    {
      check_log_size_ccc = 0;
    }
    repair_log_ccc = true;
    int ret = check_and_repair_log_ccc();
    if (check_log_ccc() || ret)
    {
      strlcpy (tempmessage_ccc, curlang_ccc[LANGLOGCHECKERR], sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
    repair_log_ccc = false;
    check_log_size_ccc = 1;
    logfile_changed_ccc = true;
    update_display_ccc(0);
  }

}





void about_ccc (void)
{
  GtkWidget *dialog = gtk_about_dialog_new();
  gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), title_ccc);
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), version_number_ccc);
  char temp [1024];
  snprintf (temp, sizeof(temp), "Copyright (C) %s Scott Dwyer", copyright_year_ccc);
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),temp);
  strlcpy (temp, "License type: PROPRIETARY (GOD MODE) ", sizeof(temp));
  snprintf (temp, sizeof(temp), "(FULL VERSION)\n");
  strcat (temp, "There is NO WARRANTY, to the extent permitted by law.");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), temp);
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "www.sdcomputingservice.com");
  char eula[hddsuperclone_EULA_txt_len + 1];
  memcpy(eula, hddsuperclone_EULA_txt, hddsuperclone_EULA_txt_len);
  eula[hddsuperclone_EULA_txt_len] = '\0';
  gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(dialog), eula);
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}





void help_html_ccc(void)
{
  // fork here to prevent locking up due to opening as root
  pid_t pid;
  pid = fork();
  if (pid == 0)
  {
    // make sure this exits and does not return
    int res0=system("cp -f /usr/local/share/doc/hddsuperclone/hddsuperclone.html /tmp/");
    int res1=system("sudo -E -H -P -u $(who | head -1 | awk '{print $1}') xdg-open /tmp/hddsuperclone.html > /dev/null 2>&1");
    if(res0 || res1) exit(1);
    exit(0);
  }
}



void help_text_ccc(void)
{
  // fork here to prevent locking up due to opening as root
  pid_t pid;
  pid = fork();
  if (pid == 0)
  {
    // make sure this exits and does not return
    int res0=system("cp -f /usr/local/share/doc/hddsuperclone/hddsuperclone.txt /tmp/");
    int res1=system("sudo -E -H -P -u $(who | head -1 | awk '{print $1}') xdg-open /tmp/hddsuperclone.txt > /dev/null 2>&1");
    if(res0 || res1) exit(1);
    exit(0);
  }
}



// open choose usb dialog window
void choose_primary_usb_ccc(void)
{
  find_all_usb_devices_ccc();

  choose_usb_dialog_window_ccc = gtk_dialog_new_with_buttons (curlang_ccc[LANGCHOOSEUSB], GTK_WINDOW(main_window_ccc), GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL);
  gtk_container_set_border_width (GTK_CONTAINER (choose_usb_dialog_window_ccc), 0);
  gtk_widget_set_size_request (choose_usb_dialog_window_ccc, 600, 400);

  choose_usb_scrolled_window_ccc = gtk_scrolled_window_new (NULL, NULL);
  gtk_container_set_border_width (GTK_CONTAINER (choose_usb_scrolled_window_ccc), 10);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (choose_usb_scrolled_window_ccc), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG(choose_usb_dialog_window_ccc)->vbox), choose_usb_scrolled_window_ccc, TRUE, TRUE, 0);

  choose_usb_vbox_ccc = gtk_vbox_new (FALSE, 0);
  gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (choose_usb_scrolled_window_ccc), choose_usb_vbox_ccc);

  GtkWidget *button[MAX_USB_DEVICES];


  int i;
  for (i = 0; i < usb_device_count_ccc; i++)
  {
    if (usb_known_relay_ccc[i])
    {
      char button_label[MAX_BUTTON_LABEL_SIZE] = "";
      snprintf (button_label, sizeof(button_label), "%04x:%04x %s %s %s\n", usb_vendor_id_ccc[i], usb_product_id_ccc[i], usb_vendor_string_ccc[i], usb_product_string_ccc[i], usb_extra_id_string_ccc[i]);
      button[i] = gtk_button_new_with_label(button_label);
      gtk_button_set_alignment(GTK_BUTTON(button[i]), 0, .5);
      g_signal_connect(button[i], "clicked", G_CALLBACK(get_usb_from_button_ccc), GINT_TO_POINTER(i) );
      gtk_box_pack_start (GTK_BOX (choose_usb_vbox_ccc), button[i], FALSE, FALSE, 0);
    }
  }


  gtk_widget_show_all (choose_usb_dialog_window_ccc);
  gint result = gtk_dialog_run (GTK_DIALOG (choose_usb_dialog_window_ccc));
  gtk_widget_destroy (choose_usb_dialog_window_ccc);

  if (result == GTK_RESPONSE_ACCEPT)
  {
    //fprintf (stdout, "OK\n");
    int ret = choose_usb_relay_ccc();
    if (ret)
    {
      fprintf (stdout, "error selecting usb, ret=%d\n", ret);
      clear_usbr1_ccc();
      snprintf(tempmessage_ccc, sizeof(tempmessage_ccc), "%s", curlang_ccc[LANGUSBERROR]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
    else
    {
      usbr1_chosen_ccc = 1;
      logfile_changed_ccc = true;
      char name[MAX_RELAY_NAME_LENGTH];
      snprintf(name, sizeof(name), "%04x:%04x %s %s %s\n", usbr1_vendor_id_ccc, usbr1_product_id_ccc, usbr1_vendor_string_ccc, usbr1_product_string_ccc, usbr1_extra_id_string_ccc);
      strlcpy (primary_relay_settings_ccc.primary_relay_name, name, sizeof(primary_relay_settings_ccc.primary_relay_name));
      strlcpy (primary_relay_name_ccc, name, sizeof(primary_relay_name_ccc));
    }
    update_display_ccc(0);
  }
}





void get_usb_from_button_ccc (GtkWidget *w, gpointer data)
{
  (void) w;
  new_usb_ccc = GPOINTER_TO_INT( data );
  g_print ("selection=%d  \n", new_usb_ccc);
}





void clear_usbr1_ccc (void)
{
  usbr1_chosen_ccc = 0;
  strcpy (primary_relay_settings_ccc.primary_relay_name, "");
  strcpy (primary_relay_name_ccc, "");
  gtk_label_set_text(GTK_LABEL(data_current_relay_board_a_ccc), "");
}





void do_activate_primary_relay_ccc(void)
{
  if (!usbr1_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNORELAYCHOSEN], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return;
  }
  bool activate_primary_relay1_bak = primary_relay_settings_ccc.activate_primary_relay1;
  bool activate_primary_relay2_bak = primary_relay_settings_ccc.activate_primary_relay2;
  bool activate_primary_relay3_bak = primary_relay_settings_ccc.activate_primary_relay3;
  bool activate_primary_relay4_bak = primary_relay_settings_ccc.activate_primary_relay4;
  bool activate_primary_relay5_bak = primary_relay_settings_ccc.activate_primary_relay5;
  bool activate_primary_relay6_bak = primary_relay_settings_ccc.activate_primary_relay6;
  bool activate_primary_relay7_bak = primary_relay_settings_ccc.activate_primary_relay7;
  bool activate_primary_relay8_bak = primary_relay_settings_ccc.activate_primary_relay8;
  bool deactivate_primary_relay1_bak = primary_relay_settings_ccc.deactivate_primary_relay1;
  bool deactivate_primary_relay2_bak = primary_relay_settings_ccc.deactivate_primary_relay2;
  bool deactivate_primary_relay3_bak = primary_relay_settings_ccc.deactivate_primary_relay3;
  bool deactivate_primary_relay4_bak = primary_relay_settings_ccc.deactivate_primary_relay4;
  bool deactivate_primary_relay5_bak = primary_relay_settings_ccc.deactivate_primary_relay5;
  bool deactivate_primary_relay6_bak = primary_relay_settings_ccc.deactivate_primary_relay6;
  bool deactivate_primary_relay7_bak = primary_relay_settings_ccc.deactivate_primary_relay7;
  bool deactivate_primary_relay8_bak = primary_relay_settings_ccc.deactivate_primary_relay8;
  unsigned long long primary_relay_activation_time_bak = primary_relay_settings_ccc.primary_relay_activation_time;
  unsigned long long primary_relay_delay_time_bak = primary_relay_settings_ccc.primary_relay_delay_time;

  update_primary_relay_settings_from_buttons_ccc();
  update_primary_relay_settings_ccc();

  g_print ("activate primary relay\n");
  if (activate_primary_relay_ccc())
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGUSBRELAYERROR], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }

  primary_relay_settings_ccc.activate_primary_relay1 = activate_primary_relay1_bak;
  primary_relay_settings_ccc.activate_primary_relay2 = activate_primary_relay2_bak;
  primary_relay_settings_ccc.activate_primary_relay3 = activate_primary_relay3_bak;
  primary_relay_settings_ccc.activate_primary_relay4 = activate_primary_relay4_bak;
  primary_relay_settings_ccc.activate_primary_relay5 = activate_primary_relay5_bak;
  primary_relay_settings_ccc.activate_primary_relay6 = activate_primary_relay6_bak;
  primary_relay_settings_ccc.activate_primary_relay7 = activate_primary_relay7_bak;
  primary_relay_settings_ccc.activate_primary_relay8 = activate_primary_relay8_bak;
  primary_relay_settings_ccc.deactivate_primary_relay1 = deactivate_primary_relay1_bak;
  primary_relay_settings_ccc.deactivate_primary_relay2 = deactivate_primary_relay2_bak;
  primary_relay_settings_ccc.deactivate_primary_relay3 = deactivate_primary_relay3_bak;
  primary_relay_settings_ccc.deactivate_primary_relay4 = deactivate_primary_relay4_bak;
  primary_relay_settings_ccc.deactivate_primary_relay5 = deactivate_primary_relay5_bak;
  primary_relay_settings_ccc.deactivate_primary_relay6 = deactivate_primary_relay6_bak;
  primary_relay_settings_ccc.deactivate_primary_relay7 = deactivate_primary_relay7_bak;
  primary_relay_settings_ccc.deactivate_primary_relay8 = deactivate_primary_relay8_bak;
  primary_relay_settings_ccc.primary_relay_activation_time = primary_relay_activation_time_bak;
  primary_relay_settings_ccc.primary_relay_delay_time = primary_relay_delay_time_bak;
  update_primary_relay_settings_ccc();
}





void do_deactivate_primary_relay_ccc(void)
{
  if (!usbr1_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNORELAYCHOSEN], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return;
  }
  bool activate_primary_relay1_bak = primary_relay_settings_ccc.activate_primary_relay1;
  bool activate_primary_relay2_bak = primary_relay_settings_ccc.activate_primary_relay2;
  bool activate_primary_relay3_bak = primary_relay_settings_ccc.activate_primary_relay3;
  bool activate_primary_relay4_bak = primary_relay_settings_ccc.activate_primary_relay4;
  bool activate_primary_relay5_bak = primary_relay_settings_ccc.activate_primary_relay5;
  bool activate_primary_relay6_bak = primary_relay_settings_ccc.activate_primary_relay6;
  bool activate_primary_relay7_bak = primary_relay_settings_ccc.activate_primary_relay7;
  bool activate_primary_relay8_bak = primary_relay_settings_ccc.activate_primary_relay8;
  bool deactivate_primary_relay1_bak = primary_relay_settings_ccc.deactivate_primary_relay1;
  bool deactivate_primary_relay2_bak = primary_relay_settings_ccc.deactivate_primary_relay2;
  bool deactivate_primary_relay3_bak = primary_relay_settings_ccc.deactivate_primary_relay3;
  bool deactivate_primary_relay4_bak = primary_relay_settings_ccc.deactivate_primary_relay4;
  bool deactivate_primary_relay5_bak = primary_relay_settings_ccc.deactivate_primary_relay5;
  bool deactivate_primary_relay6_bak = primary_relay_settings_ccc.deactivate_primary_relay6;
  bool deactivate_primary_relay7_bak = primary_relay_settings_ccc.deactivate_primary_relay7;
  bool deactivate_primary_relay8_bak = primary_relay_settings_ccc.deactivate_primary_relay8;
  unsigned long long primary_relay_activation_time_bak = primary_relay_settings_ccc.primary_relay_activation_time;
  unsigned long long primary_relay_delay_time_bak = primary_relay_settings_ccc.primary_relay_delay_time;

  update_primary_relay_settings_from_buttons_ccc();
  update_primary_relay_settings_ccc();

  g_print ("deactivate primary relay\n");
  if (deactivate_primary_relay_ccc())
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGUSBRELAYERROR], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }

  primary_relay_settings_ccc.activate_primary_relay1 = activate_primary_relay1_bak;
  primary_relay_settings_ccc.activate_primary_relay2 = activate_primary_relay2_bak;
  primary_relay_settings_ccc.activate_primary_relay3 = activate_primary_relay3_bak;
  primary_relay_settings_ccc.activate_primary_relay4 = activate_primary_relay4_bak;
  primary_relay_settings_ccc.activate_primary_relay5 = activate_primary_relay5_bak;
  primary_relay_settings_ccc.activate_primary_relay6 = activate_primary_relay6_bak;
  primary_relay_settings_ccc.activate_primary_relay7 = activate_primary_relay7_bak;
  primary_relay_settings_ccc.activate_primary_relay8 = activate_primary_relay8_bak;
  primary_relay_settings_ccc.deactivate_primary_relay1 = deactivate_primary_relay1_bak;
  primary_relay_settings_ccc.deactivate_primary_relay2 = deactivate_primary_relay2_bak;
  primary_relay_settings_ccc.deactivate_primary_relay3 = deactivate_primary_relay3_bak;
  primary_relay_settings_ccc.deactivate_primary_relay4 = deactivate_primary_relay4_bak;
  primary_relay_settings_ccc.deactivate_primary_relay5 = deactivate_primary_relay5_bak;
  primary_relay_settings_ccc.deactivate_primary_relay6 = deactivate_primary_relay6_bak;
  primary_relay_settings_ccc.deactivate_primary_relay7 = deactivate_primary_relay7_bak;
  primary_relay_settings_ccc.deactivate_primary_relay8 = deactivate_primary_relay8_bak;
  primary_relay_settings_ccc.primary_relay_activation_time = primary_relay_activation_time_bak;
  primary_relay_settings_ccc.primary_relay_delay_time = primary_relay_delay_time_bak;
  update_primary_relay_settings_ccc();
}





void do_activate_primary_relay_main_ccc(void)
{
  if (!usbr1_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNORELAYCHOSEN], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return;
  }
  g_print ("activate primary relay\n");
  if (activate_primary_relay_ccc())
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGUSBRELAYERROR], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
}





void do_deactivate_primary_relay_main_ccc(void)
{
  if (!usbr1_chosen_ccc)
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGNORELAYCHOSEN], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return;
  }
  g_print ("deactivate primary relay\n");
  if (deactivate_primary_relay_ccc())
  {
    strlcpy (tempmessage_ccc, curlang_ccc[LANGUSBRELAYERROR], sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
}





void map_heads_ccc (void)
{
#ifdef GODMODE
  if (!connected_ccc)
  {
    strlcpy (tempmessage_ccc, "you must be connected to map heads", sizeof(tempmessage_ccc));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    if (rebuild_assist_map_heads_ccc())
    {
      strlcpy (tempmessage_ccc, "error mapping heads", sizeof(tempmessage_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
  }
#endif
}


