// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "hddscviewer2.h"
#include "hddscviewer2_glade.h"



char *title = "HDDSCViewer";
char *version_number = "1.0_20220212";
int copyright_year = 2022;

#include "strncpy_wrapper.h"
#include "util.h"

int main (int argc, char **argv)
{
  char window_title[256];
  strcpy (window_title, title);
  strcat (window_title, " ");
  strcat (window_title, version_number);

  strcpy (program_title, "_");
  strcat (program_title, title);
  strcat (program_title, "_");
  strcat (program_title, version_number);
  strcat (program_title, "_");

  // setup initial language
  setup_enlanguage();
  copy_enlanguage();

    // begin processing command line arguments
  int command_line_argument;
  int arguments_required;       // required number of non-option arguments
  bool command_line_error = false;      // initialize error to false

  while (1)
  {
    static struct option long_options[] =
    {
      {"debug",         no_argument,    0, 'd'},
      {"help",          no_argument,    0, 'h'},
      {"version",       no_argument,    0, 'v'},
      {"translate",     required_argument, 0, 't'},
      {0, 0, 0, 0}
    };
    // getopt_long stores the option index here.
    int option_index = 0;

    command_line_argument = getopt_long (argc, argv, "dhvt:", long_options, &option_index);

    // Detect the end of the options.
    if (command_line_argument == -1)
      break;

    switch (command_line_argument)
    {
      case 'd':
        debug++;
        break;

      case 'h':
        help();
        exit(0);

      case 'v':
        version();
        exit(0);

      case 't':
        translate = strtoull(optarg, NULL, 0);
        break;

      case '?':
        // getopt_long already printed an error message.
        command_line_error = true;
        break;

      default:
        fprintf (stdout, "Unknown error processing command line options\n");
        command_line_error = true;
    }
  }


  arguments_required = 0;
  if ((argc - optind) < arguments_required)
  {
    command_line_error = true;
    if ((argc - optind) < arguments_required)
    {
      fprintf (stdout, "Error: too few arguments\n");
    }
    if ((argc - optind) > arguments_required)
    {
      fprintf (stdout, "Error: too many arguments\n");
    }
  }


  // exit on command line error
  if (command_line_error)
  {
    fprintf (stdout, "%s: Command line error.\n", title);
    fprintf (stdout, "Try '%s --help' for more information.\n", title);
    exit(1);
  }


  // end of command line processing


  // initialize the memory for the progress log file
  if (initialize_memory() )
  {
    exit (1);
  }



  gtk_init(&argc, &argv);

  if (translate)
  {
    translate_all();
    exit(0);
  }

  // change language dialog at startup
  set_language();


  GtkBuilder *builder = gtk_builder_new();
  GError *err = NULL; // It is mandatory to initialize to NULL
  if(0 == gtk_builder_add_from_string (builder, (const gchar *)hddscviewer2_glade, hddscviewer2_glade_len, &err))
  {
    fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
  }

  main_window = GTK_WIDGET (gtk_builder_get_object (builder, "main_window"));
  if (NULL == main_window)
  {
    fprintf(stderr, "Object error: main_window \n");
  }
  gtk_builder_connect_signals(builder, NULL);
  //main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(main_window), window_title);
  g_signal_connect(main_window, "size-allocate", G_CALLBACK(getsize_main_window), NULL);
  gtk_window_set_default_size(GTK_WINDOW(main_window), MAINWINDOWWIDTH, MAINWINDOWHEIGTH);
  gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);


  // set it to exit if closed
  g_signal_connect(G_OBJECT(main_window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  //gtk_container_set_border_width (GTK_CONTAINER (main_window), 2);

  //main_vbox = gtk_vbox_new (FALSE, 0);
  //gtk_container_add (GTK_CONTAINER (main_window), main_vbox);
  main_vbox = GTK_WIDGET (gtk_builder_get_object (builder, "main_vbox"));


  // start of menu
  //menubar = gtk_menu_bar_new();
  //menuseparater = gtk_separator_menu_item_new();

  // file menu
  //filemenu = gtk_menu_new();
  //filemi = gtk_menu_item_new_with_label(curlang[LANGFILE]);
  //quitmi = gtk_menu_item_new_with_label(curlang[LANGQUIT]);
  //openmi = gtk_menu_item_new_with_label(curlang[LANGOPEN]);
  //gtk_menu_item_set_submenu(GTK_MENU_ITEM(filemi), filemenu);
  //gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), openmi);
  //gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), menuseparater);
  //gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quitmi);
  //gtk_menu_shell_append(GTK_MENU_SHELL(menubar), filemi);
  filemi = GTK_WIDGET (gtk_builder_get_object (builder, "filemi"));
  quitmi = GTK_WIDGET (gtk_builder_get_object (builder, "quitmi"));
  openmi = GTK_WIDGET (gtk_builder_get_object (builder, "openmi"));
  domainmi = GTK_WIDGET (gtk_builder_get_object (builder, "domainmi"));
  dmdedomainmi = GTK_WIDGET (gtk_builder_get_object (builder, "dmdedomainmi"));
  gtk_menu_item_set_label(GTK_MENU_ITEM(filemi), curlang[LANGFILE]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(quitmi), curlang[LANGQUIT]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(openmi), curlang[LANGOPEN]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(domainmi), curlang[LANGDOMAIN]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(dmdedomainmi), curlang[LANGDMDEDOMAIN]);

  progress_log_label = GTK_WIDGET (gtk_builder_get_object (builder, "progress_log_label"));
  domain_log_label = GTK_WIDGET (gtk_builder_get_object (builder, "domain_log_label"));

  block_information_label = GTK_WIDGET (gtk_builder_get_object (builder, "block_information_label"));

  // options menu
  //optionsmenu = gtk_menu_new();
  //optionsmi = gtk_menu_item_new_with_label(curlang[LANGOPTIONS]);
  optionsmi = GTK_WIDGET (gtk_builder_get_object (builder, "optionsmi"));
  gtk_menu_item_set_label(GTK_MENU_ITEM(optionsmi), curlang[LANGOPTIONS]);


  // left res menu
  //leftresmenu = gtk_menu_new();
  //leftresolutionmi = gtk_menu_item_new_with_label(curlang[LANGLEFTRES]);
  //leftresbutton1 = gtk_radio_menu_item_new_with_label (leftresgroup, curlang[LANG1PT]);
  //leftresgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (leftresbutton1));
  //leftresbutton2 = gtk_radio_menu_item_new_with_label (leftresgroup, curlang[LANG2PT]);
  //leftresgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (leftresbutton2));
  //leftresbutton3 = gtk_radio_menu_item_new_with_label (leftresgroup, curlang[LANG3PT]);
  //leftresgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (leftresbutton3));
  //leftresbutton4 = gtk_radio_menu_item_new_with_label (leftresgroup, curlang[LANG4PT]);
  //leftresgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (leftresbutton4));
  leftresolutionmi  = GTK_WIDGET (gtk_builder_get_object (builder, "leftresolutionmi"));
  leftresbutton1 = GTK_WIDGET (gtk_builder_get_object (builder, "leftresbutton1"));
  leftresbutton2 = GTK_WIDGET (gtk_builder_get_object (builder, "leftresbutton2"));
  leftresbutton3 = GTK_WIDGET (gtk_builder_get_object (builder, "leftresbutton3"));
  leftresbutton4 = GTK_WIDGET (gtk_builder_get_object (builder, "leftresbutton4"));
  gtk_menu_item_set_label(GTK_MENU_ITEM(leftresolutionmi), curlang[LANGLEFTRES]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(leftresbutton1), curlang[LANG1PT]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(leftresbutton2), curlang[LANG2PT]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(leftresbutton3), curlang[LANG3PT]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(leftresbutton4), curlang[LANG4PT]);

  //gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (leftresbutton2), TRUE);

  //gtk_menu_item_set_submenu(GTK_MENU_ITEM(leftresolutionmi), leftresmenu);
  //gtk_menu_shell_append(GTK_MENU_SHELL(leftresmenu), leftresbutton1);
  //gtk_menu_shell_append(GTK_MENU_SHELL(leftresmenu), leftresbutton2);
  //gtk_menu_shell_append(GTK_MENU_SHELL(leftresmenu), leftresbutton3);
  //gtk_menu_shell_append(GTK_MENU_SHELL(leftresmenu), leftresbutton4);


  // main res menu
  //mainresmenu = gtk_menu_new();
  //mainresolutionmi = gtk_menu_item_new_with_label(curlang[LANGMAINRES]);
  //mainresbutton4 = gtk_radio_menu_item_new_with_label (mainresgroup, curlang[LANG4PT]);
  //mainresgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainresbutton4));
  //mainresbutton6 = gtk_radio_menu_item_new_with_label (mainresgroup, curlang[LANG6PT]);
  //mainresgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainresbutton6));
  //mainresbutton8 = gtk_radio_menu_item_new_with_label (mainresgroup, curlang[LANG8PT]);
  //mainresgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainresbutton8));
  //mainresbutton10 = gtk_radio_menu_item_new_with_label (mainresgroup, curlang[LANG10PT]);
  //mainresgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainresbutton10));
  //mainresbutton12 = gtk_radio_menu_item_new_with_label (mainresgroup, curlang[LANG12PT]);
  //mainresgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainresbutton12));
  //mainresbutton14 = gtk_radio_menu_item_new_with_label (mainresgroup, curlang[LANG14PT]);
  //mainresgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainresbutton14));
  //mainresbutton16 = gtk_radio_menu_item_new_with_label (mainresgroup, curlang[LANG16PT]);
  //mainresgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainresbutton16));
  mainresolutionmi = GTK_WIDGET (gtk_builder_get_object (builder, "mainresolutionmi"));
  mainresbutton4 = GTK_WIDGET (gtk_builder_get_object (builder, "mainresbutton4"));
  mainresbutton6 = GTK_WIDGET (gtk_builder_get_object (builder, "mainresbutton6"));
  mainresbutton8 = GTK_WIDGET (gtk_builder_get_object (builder, "mainresbutton8"));
  mainresbutton10 = GTK_WIDGET (gtk_builder_get_object (builder, "mainresbutton10"));
  mainresbutton12 = GTK_WIDGET (gtk_builder_get_object (builder, "mainresbutton12"));
  mainresbutton14 = GTK_WIDGET (gtk_builder_get_object (builder, "mainresbutton14"));
  mainresbutton16 = GTK_WIDGET (gtk_builder_get_object (builder, "mainresbutton16"));
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainresolutionmi), curlang[LANGMAINRES]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainresbutton4), curlang[LANG4PT]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainresbutton6), curlang[LANG6PT]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainresbutton8), curlang[LANG8PT]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainresbutton10), curlang[LANG10PT]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainresbutton12), curlang[LANG12PT]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainresbutton14), curlang[LANG14PT]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainresbutton16), curlang[LANG16PT]);

  //gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (mainresbutton8), TRUE);

  //gtk_menu_item_set_submenu(GTK_MENU_ITEM(mainresolutionmi), mainresmenu);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainresmenu), mainresbutton4);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainresmenu), mainresbutton6);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainresmenu), mainresbutton8);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainresmenu), mainresbutton10);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainresmenu), mainresbutton12);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainresmenu), mainresbutton14);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainresmenu), mainresbutton16);


  // main size menu
  //mainsizemenu = gtk_menu_new();
  //maingridsizemi = gtk_menu_item_new_with_label(curlang[LANGMAINSIZE]);
  //mainsizebutton4k = gtk_radio_menu_item_new_with_label (mainsizegroup, curlang[LANG4K]);
  //mainsizegroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainsizebutton4k));
  //mainsizebutton8k = gtk_radio_menu_item_new_with_label (mainsizegroup, curlang[LANG8K]);
  //mainsizegroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainsizebutton8k));
  //mainsizebutton16k = gtk_radio_menu_item_new_with_label (mainsizegroup, curlang[LANG16K]);
  //mainsizegroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainsizebutton16k));
  //mainsizebutton32k = gtk_radio_menu_item_new_with_label (mainsizegroup, curlang[LANG32K]);
  //mainsizegroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainsizebutton32k));
  //mainsizebutton64k = gtk_radio_menu_item_new_with_label (mainsizegroup, curlang[LANG64K]);
  //mainsizegroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainsizebutton64k));
  //mainsizebutton128k = gtk_radio_menu_item_new_with_label (mainsizegroup, curlang[LANG128K]);
  //mainsizegroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainsizebutton128k));
  //mainsizebutton256k = gtk_radio_menu_item_new_with_label (mainsizegroup, curlang[LANG256K]);
  //mainsizegroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainsizebutton256k));
  //mainsizebutton512k = gtk_radio_menu_item_new_with_label (mainsizegroup, curlang[LANG512K]);
  //mainsizegroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainsizebutton512k));
  //mainsizebutton1m = gtk_radio_menu_item_new_with_label (mainsizegroup, curlang[LANG1M]);
  //mainsizegroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainsizebutton1m));
  //mainsizebutton2m = gtk_radio_menu_item_new_with_label (mainsizegroup, curlang[LANG2M]);
  //mainsizegroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainsizebutton2m));
  //mainsizebutton4m = gtk_radio_menu_item_new_with_label (mainsizegroup, curlang[LANG4M]);
  //mainsizegroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainsizebutton4m));
  //mainsizebutton8m = gtk_radio_menu_item_new_with_label (mainsizegroup, curlang[LANG8M]);
  //mainsizegroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainsizebutton8m));
  //mainsizebutton16m = gtk_radio_menu_item_new_with_label (mainsizegroup, curlang[LANG16M]);
  //mainsizegroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mainsizebutton16m));
  maingridsizemi = GTK_WIDGET (gtk_builder_get_object (builder, "maingridsizemi"));
  mainsizebutton4k = GTK_WIDGET (gtk_builder_get_object (builder, "mainsizebutton4k"));
  mainsizebutton8k = GTK_WIDGET (gtk_builder_get_object (builder, "mainsizebutton8k"));
  mainsizebutton16k = GTK_WIDGET (gtk_builder_get_object (builder, "mainsizebutton16k"));
  mainsizebutton32k = GTK_WIDGET (gtk_builder_get_object (builder, "mainsizebutton32k"));
  mainsizebutton64k = GTK_WIDGET (gtk_builder_get_object (builder, "mainsizebutton64k"));
  mainsizebutton128k = GTK_WIDGET (gtk_builder_get_object (builder, "mainsizebutton128k"));
  mainsizebutton256k = GTK_WIDGET (gtk_builder_get_object (builder, "mainsizebutton256k"));
  mainsizebutton512k = GTK_WIDGET (gtk_builder_get_object (builder, "mainsizebutton512k"));
  mainsizebutton1m = GTK_WIDGET (gtk_builder_get_object (builder, "mainsizebutton1m"));
  mainsizebutton2m = GTK_WIDGET (gtk_builder_get_object (builder, "mainsizebutton2m"));
  mainsizebutton4m = GTK_WIDGET (gtk_builder_get_object (builder, "mainsizebutton4m"));
  mainsizebutton8m = GTK_WIDGET (gtk_builder_get_object (builder, "mainsizebutton8m"));
  mainsizebutton16m = GTK_WIDGET (gtk_builder_get_object (builder, "mainsizebutton16m"));
  gtk_menu_item_set_label(GTK_MENU_ITEM(maingridsizemi), curlang[LANGMAINSIZE]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainsizebutton4k), curlang[LANG4K]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainsizebutton8k), curlang[LANG8K]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainsizebutton16k), curlang[LANG16K]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainsizebutton32k), curlang[LANG32K]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainsizebutton64k), curlang[LANG64K]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainsizebutton128k), curlang[LANG128K]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainsizebutton256k), curlang[LANG256K]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainsizebutton512k), curlang[LANG512K]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainsizebutton1m), curlang[LANG1M]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainsizebutton2m), curlang[LANG2M]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainsizebutton4m), curlang[LANG4M]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainsizebutton8m), curlang[LANG8M]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(mainsizebutton16m), curlang[LANG16M]);

  //gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (mainsizebutton64k), TRUE);

  //gtk_menu_item_set_submenu(GTK_MENU_ITEM(maingridsizemi), mainsizemenu);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainsizemenu), mainsizebutton4k);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainsizemenu), mainsizebutton8k);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainsizemenu), mainsizebutton16k);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainsizemenu), mainsizebutton32k);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainsizemenu), mainsizebutton64k);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainsizemenu), mainsizebutton128k);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainsizemenu), mainsizebutton256k);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainsizemenu), mainsizebutton512k);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainsizemenu), mainsizebutton1m);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainsizemenu), mainsizebutton2m);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainsizemenu), mainsizebutton4m);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainsizemenu), mainsizebutton8m);
  //gtk_menu_shell_append(GTK_MENU_SHELL(mainsizemenu), mainsizebutton16m);


  // auto update menu
  //autoupdatemenu = gtk_menu_new();
  //autoupdatemi = gtk_menu_item_new_with_label(curlang[LANGAUTOUPDATE]);
  //autoupdatebuttonoff = gtk_radio_menu_item_new_with_label (autoupgroup, curlang[LANGOFF]);
  //autoupgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (autoupdatebuttonoff));
  //autoupdatebutton5s = gtk_radio_menu_item_new_with_label (autoupgroup, curlang[LANG5SEC]);
  //autoupgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (autoupdatebutton5s));
  //autoupdatebutton10s = gtk_radio_menu_item_new_with_label (autoupgroup, curlang[LANG10SEC]);
  //autoupgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (autoupdatebutton10s));
  //autoupdatebutton30s = gtk_radio_menu_item_new_with_label (autoupgroup, curlang[LANG30SEC]);
  //autoupgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (autoupdatebutton30s));
  //autoupdatebutton1m = gtk_radio_menu_item_new_with_label (autoupgroup, curlang[LANG1MIN]);
  //autoupgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (autoupdatebutton1m));
  //autoupdatebutton2m = gtk_radio_menu_item_new_with_label (autoupgroup, curlang[LANG2MIN]);
  //autoupgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (autoupdatebutton2m));
  //autoupdatebutton5m = gtk_radio_menu_item_new_with_label (autoupgroup, curlang[LANG5MIN]);
  //autoupgroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (autoupdatebutton5m));
  autoupdatemi = GTK_WIDGET (gtk_builder_get_object (builder, "autoupdatemi"));
  autoupdatebuttonoff = GTK_WIDGET (gtk_builder_get_object (builder, "autoupdatebuttonoff"));
  autoupdatebutton5s = GTK_WIDGET (gtk_builder_get_object (builder, "autoupdatebutton5s"));
  autoupdatebutton10s = GTK_WIDGET (gtk_builder_get_object (builder, "autoupdatebutton10s"));
  autoupdatebutton30s = GTK_WIDGET (gtk_builder_get_object (builder, "autoupdatebutton30s"));
  autoupdatebutton1m = GTK_WIDGET (gtk_builder_get_object (builder, "autoupdatebutton1m"));
  autoupdatebutton2m = GTK_WIDGET (gtk_builder_get_object (builder, "autoupdatebutton2m"));
  autoupdatebutton5m = GTK_WIDGET (gtk_builder_get_object (builder, "autoupdatebutton5m"));
  gtk_menu_item_set_label(GTK_MENU_ITEM(autoupdatemi), curlang[LANGAUTOUPDATE]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(autoupdatebuttonoff), curlang[LANGOFF]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(autoupdatebutton5s), curlang[LANG5SEC]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(autoupdatebutton10s), curlang[LANG10SEC]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(autoupdatebutton30s), curlang[LANG30SEC]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(autoupdatebutton1m), curlang[LANG1MIN]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(autoupdatebutton2m), curlang[LANG2MIN]);
  gtk_menu_item_set_label(GTK_MENU_ITEM(autoupdatebutton5m), curlang[LANG5MIN]);

  //gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (autoupdatebuttonoff), TRUE);

  //gtk_menu_item_set_submenu(GTK_MENU_ITEM(autoupdatemi), autoupdatemenu);
  //gtk_menu_shell_append(GTK_MENU_SHELL(autoupdatemenu), autoupdatebuttonoff);
  //gtk_menu_shell_append(GTK_MENU_SHELL(autoupdatemenu), autoupdatebutton5s);
  //gtk_menu_shell_append(GTK_MENU_SHELL(autoupdatemenu), autoupdatebutton10s);
  //gtk_menu_shell_append(GTK_MENU_SHELL(autoupdatemenu), autoupdatebutton30s);
  //gtk_menu_shell_append(GTK_MENU_SHELL(autoupdatemenu), autoupdatebutton1m);
  //gtk_menu_shell_append(GTK_MENU_SHELL(autoupdatemenu), autoupdatebutton2m);
  //gtk_menu_shell_append(GTK_MENU_SHELL(autoupdatemenu), autoupdatebutton5m);

  // show good data item
  //showgoodcheck = gtk_check_menu_item_new_with_label(curlang[LANGSHOWGOODDATA]);
  //gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (showgoodcheck), FALSE);
  showgoodcheck = GTK_WIDGET (gtk_builder_get_object (builder, "showgoodcheck"));
  gtk_menu_item_set_label(GTK_MENU_ITEM(showgoodcheck), curlang[LANGSHOWGOODDATA]);

  // show bad head item
  //showbadcheck = gtk_check_menu_item_new_with_label(curlang[LANGSHOWBADHEAD]);
  //gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (showbadcheck), FALSE);
  showbadcheck = GTK_WIDGET (gtk_builder_get_object (builder, "showbadcheck"));
  gtk_menu_item_set_label(GTK_MENU_ITEM(showbadcheck), curlang[LANGSHOWBADHEAD]);

  showtimemi = GTK_WIDGET (gtk_builder_get_object (builder, "showtimemi"));
  showtimingbuttonoff = GTK_WIDGET (gtk_builder_get_object (builder, "showtimingbuttonoff"));
  showtimingbutton1 = GTK_WIDGET (gtk_builder_get_object (builder, "showtimingbutton1"));
  showtimingbutton2 = GTK_WIDGET (gtk_builder_get_object (builder, "showtimingbutton2"));
  showtimingbutton3 = GTK_WIDGET (gtk_builder_get_object (builder, "showtimingbutton3"));
  showtimingbutton4 = GTK_WIDGET (gtk_builder_get_object (builder, "showtimingbutton4"));
  showtimingbutton5 = GTK_WIDGET (gtk_builder_get_object (builder, "showtimingbutton5"));
  showtimingbutton7 = GTK_WIDGET (gtk_builder_get_object (builder, "showtimingbutton7"));
  showtimingbutton10 = GTK_WIDGET (gtk_builder_get_object (builder, "showtimingbutton10"));
  showtimingbutton15 = GTK_WIDGET (gtk_builder_get_object (builder, "showtimingbutton15"));
  showtimingbutton20 = GTK_WIDGET (gtk_builder_get_object (builder, "showtimingbutton20"));
  showtimingbutton25 = GTK_WIDGET (gtk_builder_get_object (builder, "showtimingbutton25"));
  showtimingbutton30 = GTK_WIDGET (gtk_builder_get_object (builder, "showtimingbutton30"));
  showtimingbutton40 = GTK_WIDGET (gtk_builder_get_object (builder, "showtimingbutton40"));
  showtimingbutton50 = GTK_WIDGET (gtk_builder_get_object (builder, "showtimingbutton50"));
  showtimingbutton60 = GTK_WIDGET (gtk_builder_get_object (builder, "showtimingbutton60"));
  gtk_menu_item_set_label(GTK_MENU_ITEM(showtimemi), curlang[LANGSHOWTIMING]);

  showdomaincheck = GTK_WIDGET (gtk_builder_get_object (builder, "showdomaincheck"));
  gtk_menu_item_set_label(GTK_MENU_ITEM(showdomaincheck), curlang[LANGSHOWDOMAIN]);


  //gtk_menu_item_set_submenu(GTK_MENU_ITEM(optionsmi), optionsmenu);
  //gtk_menu_shell_append(GTK_MENU_SHELL(optionsmenu), leftresolutionmi);
  //gtk_menu_shell_append(GTK_MENU_SHELL(optionsmenu), mainresolutionmi);
  //gtk_menu_shell_append(GTK_MENU_SHELL(optionsmenu), maingridsizemi);
  //gtk_menu_shell_append(GTK_MENU_SHELL(optionsmenu), autoupdatemi);
  //gtk_menu_shell_append(GTK_MENU_SHELL(optionsmenu), showgoodcheck);
  //gtk_menu_shell_append(GTK_MENU_SHELL(optionsmenu), showbadcheck);


  //gtk_menu_shell_append(GTK_MENU_SHELL(menubar), optionsmi);







  // pack the menu in the box
  //gtk_box_pack_start(GTK_BOX(main_vbox), menubar, FALSE, FALSE, 0);

  // set it to exit if the quit item is selected
  g_signal_connect(G_OBJECT(quitmi), "activate", G_CALLBACK(gtk_main_quit), NULL);

  // set it to open a file if the open item is selected
  g_signal_connect(G_OBJECT(openmi), "activate", G_CALLBACK(select_file), NULL);

  // set it to open a file if the domain item is selected
  g_signal_connect(G_OBJECT(domainmi), "activate", G_CALLBACK(select_domain), NULL);

  // set it to open a file if the dmde domain item is selected
  g_signal_connect(G_OBJECT(dmdedomainmi), "activate", G_CALLBACK(select_dmde_domain), NULL);


  g_signal_connect(G_OBJECT(leftresbutton1), "activate", G_CALLBACK(change_left_resolution), GINT_TO_POINTER(1) );
  g_signal_connect(G_OBJECT(leftresbutton2), "activate", G_CALLBACK(change_left_resolution), GINT_TO_POINTER(2) );
  g_signal_connect(G_OBJECT(leftresbutton3), "activate", G_CALLBACK(change_left_resolution), GINT_TO_POINTER(3) );
  g_signal_connect(G_OBJECT(leftresbutton4), "activate", G_CALLBACK(change_left_resolution), GINT_TO_POINTER(4) );

  g_signal_connect(G_OBJECT(mainresbutton4), "activate", G_CALLBACK(change_main_resolution), GINT_TO_POINTER(4) );
  g_signal_connect(G_OBJECT(mainresbutton6), "activate", G_CALLBACK(change_main_resolution), GINT_TO_POINTER(6) );
  g_signal_connect(G_OBJECT(mainresbutton8), "activate", G_CALLBACK(change_main_resolution), GINT_TO_POINTER(8) );
  g_signal_connect(G_OBJECT(mainresbutton10), "activate", G_CALLBACK(change_main_resolution), GINT_TO_POINTER(10) );
  g_signal_connect(G_OBJECT(mainresbutton12), "activate", G_CALLBACK(change_main_resolution), GINT_TO_POINTER(12) );
  g_signal_connect(G_OBJECT(mainresbutton14), "activate", G_CALLBACK(change_main_resolution), GINT_TO_POINTER(14) );
  g_signal_connect(G_OBJECT(mainresbutton16), "activate", G_CALLBACK(change_main_resolution), GINT_TO_POINTER(16) );

  g_signal_connect(G_OBJECT(mainsizebutton4k), "activate", G_CALLBACK(change_main_grid_size), GINT_TO_POINTER(4096) );
  g_signal_connect(G_OBJECT(mainsizebutton8k), "activate", G_CALLBACK(change_main_grid_size), GINT_TO_POINTER(8192) );
  g_signal_connect(G_OBJECT(mainsizebutton16k), "activate", G_CALLBACK(change_main_grid_size), GINT_TO_POINTER(16384) );
  g_signal_connect(G_OBJECT(mainsizebutton32k), "activate", G_CALLBACK(change_main_grid_size), GINT_TO_POINTER(32768) );
  g_signal_connect(G_OBJECT(mainsizebutton64k), "activate", G_CALLBACK(change_main_grid_size), GINT_TO_POINTER(65536) );
  g_signal_connect(G_OBJECT(mainsizebutton128k), "activate", G_CALLBACK(change_main_grid_size), GINT_TO_POINTER(131072) );
  g_signal_connect(G_OBJECT(mainsizebutton256k), "activate", G_CALLBACK(change_main_grid_size), GINT_TO_POINTER(262144) );
  g_signal_connect(G_OBJECT(mainsizebutton512k), "activate", G_CALLBACK(change_main_grid_size), GINT_TO_POINTER(524288) );
  g_signal_connect(G_OBJECT(mainsizebutton1m), "activate", G_CALLBACK(change_main_grid_size), GINT_TO_POINTER(1048576) );
  g_signal_connect(G_OBJECT(mainsizebutton2m), "activate", G_CALLBACK(change_main_grid_size), GINT_TO_POINTER(2097152) );
  g_signal_connect(G_OBJECT(mainsizebutton4m), "activate", G_CALLBACK(change_main_grid_size), GINT_TO_POINTER(4194304) );
  g_signal_connect(G_OBJECT(mainsizebutton8m), "activate", G_CALLBACK(change_main_grid_size), GINT_TO_POINTER(8388608) );
  g_signal_connect(G_OBJECT(mainsizebutton16m), "activate", G_CALLBACK(change_main_grid_size), GINT_TO_POINTER(16777216) );

  g_signal_connect(G_OBJECT(autoupdatebuttonoff), "activate", G_CALLBACK(set_autoupdate_timer), GINT_TO_POINTER(0) );
  g_signal_connect(G_OBJECT(autoupdatebutton5s), "activate", G_CALLBACK(set_autoupdate_timer), GINT_TO_POINTER(5000) );
  g_signal_connect(G_OBJECT(autoupdatebutton10s), "activate", G_CALLBACK(set_autoupdate_timer), GINT_TO_POINTER(10000) );
  g_signal_connect(G_OBJECT(autoupdatebutton30s), "activate", G_CALLBACK(set_autoupdate_timer), GINT_TO_POINTER(30000) );
  g_signal_connect(G_OBJECT(autoupdatebutton1m), "activate", G_CALLBACK(set_autoupdate_timer), GINT_TO_POINTER(60000) );
  g_signal_connect(G_OBJECT(autoupdatebutton2m), "activate", G_CALLBACK(set_autoupdate_timer), GINT_TO_POINTER(120000) );
  g_signal_connect(G_OBJECT(autoupdatebutton5m), "activate", G_CALLBACK(set_autoupdate_timer), GINT_TO_POINTER(300000) );

  g_signal_connect(G_OBJECT(showgoodcheck), "activate", G_CALLBACK(toggle_showgood), NULL);

  g_signal_connect(G_OBJECT(showbadcheck), "activate", G_CALLBACK(toggle_showbad), NULL);

  g_signal_connect(G_OBJECT(showdomaincheck), "activate", G_CALLBACK(toggle_showdomain), NULL);

  g_signal_connect(G_OBJECT(showtimingbuttonoff), "activate", G_CALLBACK(set_show_timing), GINT_TO_POINTER(0) );
  g_signal_connect(G_OBJECT(showtimingbutton1), "activate", G_CALLBACK(set_show_timing), GINT_TO_POINTER(1) );
  g_signal_connect(G_OBJECT(showtimingbutton2), "activate", G_CALLBACK(set_show_timing), GINT_TO_POINTER(2) );
  g_signal_connect(G_OBJECT(showtimingbutton3), "activate", G_CALLBACK(set_show_timing), GINT_TO_POINTER(3) );
  g_signal_connect(G_OBJECT(showtimingbutton4), "activate", G_CALLBACK(set_show_timing), GINT_TO_POINTER(4) );
  g_signal_connect(G_OBJECT(showtimingbutton5), "activate", G_CALLBACK(set_show_timing), GINT_TO_POINTER(5) );
  g_signal_connect(G_OBJECT(showtimingbutton7), "activate", G_CALLBACK(set_show_timing), GINT_TO_POINTER(7) );
  g_signal_connect(G_OBJECT(showtimingbutton10), "activate", G_CALLBACK(set_show_timing), GINT_TO_POINTER(10) );
  g_signal_connect(G_OBJECT(showtimingbutton15), "activate", G_CALLBACK(set_show_timing), GINT_TO_POINTER(15) );
  g_signal_connect(G_OBJECT(showtimingbutton20), "activate", G_CALLBACK(set_show_timing), GINT_TO_POINTER(20) );
  g_signal_connect(G_OBJECT(showtimingbutton25), "activate", G_CALLBACK(set_show_timing), GINT_TO_POINTER(25) );
  g_signal_connect(G_OBJECT(showtimingbutton30), "activate", G_CALLBACK(set_show_timing), GINT_TO_POINTER(30) );
  g_signal_connect(G_OBJECT(showtimingbutton40), "activate", G_CALLBACK(set_show_timing), GINT_TO_POINTER(40) );
  g_signal_connect(G_OBJECT(showtimingbutton50), "activate", G_CALLBACK(set_show_timing), GINT_TO_POINTER(50) );
  g_signal_connect(G_OBJECT(showtimingbutton60), "activate", G_CALLBACK(set_show_timing), GINT_TO_POINTER(60) );




  // main hbox
  //main_hbox = gtk_hbox_new (FALSE, 0);
  main_hbox  = GTK_WIDGET (gtk_builder_get_object (builder, "main_hbox"));
  g_signal_connect(main_hbox, "size-allocate", G_CALLBACK(getsize_main_hbox), NULL);
  //gtk_box_pack_start (GTK_BOX (main_vbox), main_hbox, TRUE, TRUE, 0);


  // left vbox
  //left_vbox = gtk_vbox_new (FALSE, 0);
  left_vbox = GTK_WIDGET (gtk_builder_get_object (builder, "left_vbox"));
  g_signal_connect(left_vbox, "size-allocate", G_CALLBACK(getsize_left_vbox), NULL);
  //gtk_widget_set_size_request (GTK_WIDGET (left_vbox), 100, -1);
  //gtk_container_set_border_width (GTK_CONTAINER (left_vbox), 2);
  //gtk_box_pack_start (GTK_BOX (main_hbox), left_vbox, FALSE, FALSE, 0);

  //left_drawing_area = gtk_drawing_area_new();
  left_drawing_area = GTK_WIDGET (gtk_builder_get_object (builder, "left_drawing_area"));
  g_signal_connect(left_drawing_area, "size-allocate", G_CALLBACK(getsize_left_drawing_area), NULL);
  //gtk_widget_set_size_request (GTK_WIDGET (left_drawing_area), 100, -1);
  //gtk_box_pack_start (GTK_BOX (left_vbox), left_drawing_area, TRUE, TRUE, 0);

  g_signal_connect(left_drawing_area, "expose-event", G_CALLBACK(left_vbox_expose_event), NULL);


  // right vbox
  //right_vbox = gtk_vbox_new (FALSE, 0);
  right_vbox = GTK_WIDGET (gtk_builder_get_object (builder, "right_vbox"));
  g_signal_connect(right_vbox, "size-allocate", G_CALLBACK(getsize_right_vbox), NULL);
  //gtk_container_set_border_width (GTK_CONTAINER (right_vbox), 2);
  //gtk_box_pack_start (GTK_BOX (main_hbox), right_vbox, TRUE, TRUE, 0);

  // top hbox
  //top_hbox = gtk_hbox_new (FALSE, 0);
  top_hbox = GTK_WIDGET (gtk_builder_get_object (builder, "top_hbox"));
  g_signal_connect(top_hbox, "size-allocate", G_CALLBACK(getsize_top_hbox), NULL);
  //gtk_widget_set_size_request (GTK_WIDGET (top_hbox), -1, 100);
  //gtk_container_set_border_width (GTK_CONTAINER (top_hbox), 2);
  //gtk_box_pack_start (GTK_BOX (right_vbox), top_hbox, FALSE, FALSE, 0);

  //top_info_box = gtk_vbox_new (FALSE, 0);
  top_info_box = GTK_WIDGET (gtk_builder_get_object (builder, "top_info_box"));
  g_signal_connect(top_info_box, "size-allocate", G_CALLBACK(getsize_top_info_box), NULL);
  //gtk_widget_set_size_request (GTK_WIDGET (top_info_box), -1, 100);
  //gtk_container_set_border_width (GTK_CONTAINER (top_info_box), 2);
  //gtk_box_pack_start (GTK_BOX (top_hbox), top_info_box, TRUE, TRUE, 0);
  //frame = gtk_frame_new (NULL);
  //gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_OUT);
  //gtk_container_add (GTK_CONTAINER (top_info_box), frame);

  //top_drawing_area = gtk_drawing_area_new();
  top_drawing_area = GTK_WIDGET (gtk_builder_get_object (builder, "top_drawing_area"));
  g_signal_connect(top_drawing_area, "size-allocate", G_CALLBACK(getsize_top_drawing_area), NULL);
  //gtk_widget_set_size_request (GTK_WIDGET (top_drawing_area), 400, -1);
  //gtk_box_pack_start (GTK_BOX (top_hbox), top_drawing_area, FALSE, FALSE, 0);

  g_signal_connect(top_drawing_area, "expose-event", G_CALLBACK(top_drawing_expose_event), NULL);



  // main drawing vbox
  //main_drawing_vbox = gtk_vbox_new (FALSE, 0);
  main_drawing_vbox = GTK_WIDGET (gtk_builder_get_object (builder, "main_drawing_vbox"));
  g_signal_connect(main_drawing_vbox, "size-allocate", G_CALLBACK(getsize_main_drawing_vbox), NULL);
  //gtk_container_set_border_width (GTK_CONTAINER (main_drawing_vbox), 2);
  //gtk_box_pack_start (GTK_BOX (right_vbox), main_drawing_vbox, TRUE, TRUE, 0);

  // main drawing scrolled window
  //main_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  main_scrolled_window = GTK_WIDGET (gtk_builder_get_object (builder, "main_scrolled_window"));
  g_signal_connect(main_scrolled_window, "size-allocate", G_CALLBACK(getsize_main_scrolled_window), NULL);
  //gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (main_scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  //gtk_widget_set_size_request(main_scrolled_window, 400, 300);
  //gtk_box_pack_start (GTK_BOX (main_drawing_vbox), main_scrolled_window, TRUE, TRUE, 0);
  //gtk_container_add (GTK_CONTAINER (main_drawing_vbox), main_scrolled_window);


  //main_drawing_area = gtk_drawing_area_new();
  main_drawing_area = GTK_WIDGET (gtk_builder_get_object (builder, "main_drawing_area"));
  //gtk_container_add(GTK_CONTAINER(main_scrolled_window), main_drawing_area);
  g_signal_connect(main_drawing_area, "size-allocate", G_CALLBACK(getsize_main_drawing_area), NULL);
  //gtk_widget_set_size_request (GTK_WIDGET (main_drawing_area), 200, 200);
  gtk_widget_add_events(main_drawing_area, GDK_BUTTON_PRESS_MASK);

  g_signal_connect(main_drawing_area, "expose-event", G_CALLBACK(main_drawing_expose_event), NULL);
  //g_signal_connect (main_drawing_area, "configure_event", G_CALLBACK (configure_event), NULL);
  g_signal_connect(G_OBJECT(main_drawing_area), "button-press-event", G_CALLBACK(on_button_press), NULL);

  g_object_unref(builder);

  gtk_widget_show_all(main_window);

  gtk_main();

  return 0;
}





static gboolean on_button_press(GtkWidget* widget, GdkEventButton *event, GdkWindowEdge edge)
{
  (void) widget;
  (void) edge;
  if (event->type == GDK_BUTTON_PRESS)
  {
    if (event->button == 1)
    {
      mouse_x = event->x;
      mouse_y = event->y;
      snprintf(tempmessage, sizeof(tempmessage), "x=%d y=%d\n", mouse_x, mouse_y);
      message_debug(tempmessage, 0);
      //redraw_main_drawing_area();
      gtk_widget_queue_draw(main_window);
    }
  }
  return TRUE;
}





static gboolean top_drawing_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  redraw_top_drawing_area();

  // make the compiler happy and not complain about unused paramaters
  (void) widget;
  (void) event;
  (void) data;

  return 0;
}


static gboolean main_drawing_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  redraw_main_drawing_area();

  // make the compiler happy and not complain about unused paramaters
  (void) widget;
  (void) event;
  (void) data;

  return 0;
}


static gboolean left_vbox_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  redraw_left_vbox();

  // make the compiler happy and not complain about unused paramaters
  (void) widget;
  (void) event;
  (void) data;

  return 0;
}


void getsize_main_window(GtkWidget *widget, GtkAllocation *allocation, void *data)
{
  snprintf(tempmessage, sizeof(tempmessage), "main window width = %d, height = %d\n", allocation->width, allocation->height);
  message_debug(tempmessage, 0);
  main_window_width = allocation->width;
  main_window_height = allocation->height;
  // make the compiler happy and not complain about unused paramaters
  (void) widget;
  (void) data;
}

void getsize_main_vbox(GtkWidget *widget, GtkAllocation *allocation, void *data)
{
  snprintf(tempmessage, sizeof(tempmessage), "main_vbox width = %d, height = %d\n", allocation->width, allocation->height);
  message_debug(tempmessage, 0);
  main_vbox_width = allocation->width;
  main_vbox_height = allocation->height;
  // make the compiler happy and not complain about unused paramaters
  (void) widget;
  (void) data;
}

void getsize_main_hbox(GtkWidget *widget, GtkAllocation *allocation, void *data)
{
  snprintf(tempmessage, sizeof(tempmessage), "main_hbox width = %d, height = %d\n", allocation->width, allocation->height);
  message_debug(tempmessage, 0);
  main_hbox_width = allocation->width;
  main_hbox_height = allocation->height;
  // make the compiler happy and not complain about unused paramaters
  (void) widget;
  (void) data;
}

void getsize_left_vbox(GtkWidget *widget, GtkAllocation *allocation, void *data)
{
  snprintf(tempmessage, sizeof(tempmessage), "left_vbox width = %d, height = %d\n", allocation->width, allocation->height);
  message_debug(tempmessage, 0);
  left_vbox_width = allocation->width;
 left_vbox_height = allocation->height;
  // make the compiler happy and not complain about unused paramaters
  (void) widget;
  (void) data;
}

void getsize_right_vbox(GtkWidget *widget, GtkAllocation *allocation, void *data)
{
  snprintf(tempmessage, sizeof(tempmessage), "right_vbox width = %d, height = %d\n", allocation->width, allocation->height);
  message_debug(tempmessage, 0);
  right_vbox_width = allocation->width;
  right_vbox_height = allocation->height;
  // make the compiler happy and not complain about unused paramaters
  (void) widget;
  (void) data;
}

void getsize_top_hbox(GtkWidget *widget, GtkAllocation *allocation, void *data)
{
  snprintf(tempmessage, sizeof(tempmessage), "top_hbox width = %d, height = %d\n", allocation->width, allocation->height);
  message_debug(tempmessage, 0);
  top_hbox_width = allocation->width;
  top_hbox_height = allocation->height;
  // make the compiler happy and not complain about unused paramaters
  (void) widget;
  (void) data;
}

void getsize_top_info_box(GtkWidget *widget, GtkAllocation *allocation, void *data)
{
  snprintf(tempmessage, sizeof(tempmessage), "top_hbox width = %d, height = %d\n", allocation->width, allocation->height);
  message_debug(tempmessage, 0);
  top_info_width = allocation->width;
  top_info_height = allocation->height;
  // make the compiler happy and not complain about unused paramaters
  (void) widget;
  (void) data;
}

void getsize_top_drawing_area(GtkWidget *widget, GtkAllocation *allocation, void *data)
{
  snprintf(tempmessage, sizeof(tempmessage), "top_drawing_area width = %d, height = %d\n", allocation->width, allocation->height);
  message_debug(tempmessage, 0);
  top_drawing_area_width = allocation->width;
  top_drawing_area_height = allocation->height;
  // make the compiler happy and not complain about unused paramaters
  (void) widget;
  (void) data;
}

void getsize_main_drawing_area(GtkWidget *widget, GtkAllocation *allocation, void *data)
{
  snprintf(tempmessage, sizeof(tempmessage), "main drawing area width = %d, height = %d\n", allocation->width, allocation->height);
  message_debug(tempmessage, 0);
  main_drawing_area_width = allocation->width;
  main_drawing_area_height = allocation->height;
  // make the compiler happy and not complain about unused paramaters
  (void) widget;
  (void) data;
}

void getsize_main_drawing_vbox(GtkWidget *widget, GtkAllocation *allocation, void *data)
{
  snprintf(tempmessage, sizeof(tempmessage), "main drawing vbox width = %d, height = %d\n", allocation->width, allocation->height);
  message_debug(tempmessage, 0);
  main_drawing_vbox_width = allocation->width;
  main_drawing_vbox_height = allocation->height;
  // make the compiler happy and not complain about unused paramaters
  (void) widget;
  (void) data;
}

void getsize_main_scrolled_window(GtkWidget *widget, GtkAllocation *allocation, void *data)
{
  snprintf(tempmessage, sizeof(tempmessage), "main_scrolled_window width = %d, height = %d\n", allocation->width, allocation->height);
  message_debug(tempmessage, 0);
  main_scrolled_window_width = allocation->width;
  main_scrolled_window_height = allocation->height;
  // make the compiler happy and not complain about unused paramaters
  (void) widget;
  (void) data;
}

void getsize_left_drawing_area(GtkWidget *widget, GtkAllocation *allocation, void *data)
{
  snprintf(tempmessage, sizeof(tempmessage), "left drawing area width = %d, height = %d\n", allocation->width, allocation->height);
  message_debug(tempmessage, 0);
  left_drawing_area_width = allocation->width;
  left_drawing_area_height = allocation->height;
  // make the compiler happy and not complain about unused paramaters
  (void) widget;
  (void) data;
}





void select_file(void)
{
  /* Create a new file selection widget */
  filew = gtk_file_selection_new ("File selection");
  gtk_file_selection_hide_fileop_buttons (GTK_FILE_SELECTION (filew));

  /* Connect the ok_button to file_ok_sel function */
  g_signal_connect (GTK_FILE_SELECTION (filew)->ok_button, "clicked", G_CALLBACK (file_ok_sel), (gpointer) filew);

  /* Connect the cancel_button to destroy the widget */
  g_signal_connect_swapped (GTK_FILE_SELECTION (filew)->cancel_button, "clicked", G_CALLBACK (gtk_widget_destroy), filew);

  /* Lets set the filename, as if this were a save dialog, and we are giving
   *     a default filename */
  gtk_file_selection_set_filename (GTK_FILE_SELECTION(filew), "progress.log");

  gtk_widget_show (filew);
}


/* Get the selected filename and load it */
static void file_ok_sel( GtkWidget *w, GtkFileSelection *fs )
{
  (void) w;
  //g_print ("%s\n", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
  snprintf (log_file, sizeof(log_file), "%s", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
  printf ("%s\n",  log_file);
  gtk_widget_destroy(filew);
  total_size = 0;
  int ret = read_log_file(log_file);
  if (ret != 0)
  {
    snprintf(tempmessage, sizeof(tempmessage), "error processing log file\n");
    message_now(tempmessage);
  }
  else
  {
    ret = check_log();
    if (ret != 0)
    {
      snprintf(tempmessage, sizeof(tempmessage), "there were errors found in the log file\n");
      message_now(tempmessage);
    }
  }
  char tempchar[1048];
  strcpy (tempchar, "Log= ");
  strcat (tempchar, log_file);
  gtk_label_set_text(GTK_LABEL(progress_log_label), tempchar);
  //redraw_left_vbox();
  //redraw_main_drawing_area();
  // this seems to be needed sometimes after changing the settings and then reloading the log file
  gtk_widget_queue_draw(main_window);
}





void select_domain(void)
{
  /* Create a new file selection widget */
  domainw = gtk_file_selection_new ("File selection");
  gtk_file_selection_hide_fileop_buttons (GTK_FILE_SELECTION (domainw));

  /* Connect the ok_button to file_ok_sel function */
  g_signal_connect (GTK_FILE_SELECTION (domainw)->ok_button, "clicked", G_CALLBACK (domain_ok_sel), (gpointer) domainw);

  /* Connect the cancel_button to destroy the widget */
  g_signal_connect_swapped (GTK_FILE_SELECTION (domainw)->cancel_button, "clicked", G_CALLBACK (gtk_widget_destroy), domainw);

  /* Lets set the filename, as if this were a save dialog, and we are giving
   *     a default filename */
  gtk_file_selection_set_filename (GTK_FILE_SELECTION(domainw), "domain.log");

  gtk_widget_show (domainw);
}


/* Get the selected filename and load it */
static void domain_ok_sel( GtkWidget *w, GtkFileSelection *fs )
{
  (void) w;
  //g_print ("%s\n", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
  snprintf (domain_file, sizeof(domain_file), "%s", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
  printf ("%s\n",  domain_file);
  gtk_widget_destroy(domainw);
  regular_domain = 1;
  int ret = read_domain_file(domain_file);
  if (ret != 0)
  {
    snprintf(tempmessage, sizeof(tempmessage), "error processing domain file\n");
    message_now(tempmessage);
  }
  char tempchar[1048];
  strcpy (tempchar, "Domain= ");
  strcat (tempchar, domain_file);
  gtk_label_set_text(GTK_LABEL(domain_log_label), tempchar);
  //redraw_left_vbox();
  //redraw_main_drawing_area();
  // this seems to be needed sometimes after changing the settings and then reloading the log file
  gtk_widget_queue_draw(main_window);
}




void select_dmde_domain(void)
{
  /* Create a new file selection widget */
  domainw = gtk_file_selection_new ("File selection");
  gtk_file_selection_hide_fileop_buttons (GTK_FILE_SELECTION (domainw));

  /* Connect the ok_button to file_ok_sel function */
  g_signal_connect (GTK_FILE_SELECTION (domainw)->ok_button, "clicked", G_CALLBACK (dmde_domain_ok_sel), (gpointer) domainw);

  /* Connect the cancel_button to destroy the widget */
  g_signal_connect_swapped (GTK_FILE_SELECTION (domainw)->cancel_button, "clicked", G_CALLBACK (gtk_widget_destroy), domainw);

  /* Lets set the filename, as if this were a save dialog, and we are giving
   *     a default filename */
  gtk_file_selection_set_filename (GTK_FILE_SELECTION(domainw), "domain.log");

  gtk_widget_show (domainw);
}


/* Get the selected filename and load it */
static void dmde_domain_ok_sel( GtkWidget *w, GtkFileSelection *fs )
{
  (void) w;
  //g_print ("%s\n", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
  snprintf (domain_file, sizeof(domain_file), "%s", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
  printf ("%s\n",  domain_file);
  gtk_widget_destroy(domainw);
  regular_domain = 0;
  int ret = read_domain_dmde_file(domain_file);
  if (ret != 0)
  {
    snprintf(tempmessage, sizeof(tempmessage), "error processing domain file\n");
    message_now(tempmessage);
  }
  char tempchar[1048];
  strcpy (tempchar, "Domain= ");
  strcat (tempchar, domain_file);
  gtk_label_set_text(GTK_LABEL(domain_log_label), tempchar);
  //redraw_left_vbox();
  //redraw_main_drawing_area();
  // this seems to be needed sometimes after changing the settings and then reloading the log file
  gtk_widget_queue_draw(main_window);
}





gboolean reload_file(__attribute__((unused)) gpointer user_data)
{
  printf ("%s\n",  log_file);
  total_size = 0;
  int ret = read_log_file(log_file);
  if (ret != 0)
  {
    snprintf(tempmessage, sizeof(tempmessage), "error processing log file\n");
    message_now(tempmessage);
  }
  else
  {
    ret = check_log();
    if (ret != 0)
    {
      snprintf(tempmessage, sizeof(tempmessage), "there were errors found in the log file\n");
      message_now(tempmessage);
    }
  }
  if (regular_domain)
  {
    printf ("%s\n",  domain_file);
    domain_lines = 0;
    int ret = read_domain_file(domain_file);
    if (ret != 0)
    {
      snprintf(tempmessage, sizeof(tempmessage), "error processing domain file\n");
      message_now(tempmessage);
    }
  }

  //redraw_left_vbox();
  //redraw_main_drawing_area();
  // this seems to be needed sometimes after changing the settings and then reloading the log file
  gtk_widget_queue_draw(main_window);

  return 1;
}





void set_autoupdate_timer(GtkWidget *w, gpointer data)
{
  (void) w;
  if (autotimer_on)
  {
    g_source_remove (timeout_tag);
    autotimer_on = 0;
  }
  int time = GPOINTER_TO_INT( data );
  if (time > 0)
  {
    timeout_tag = g_timeout_add (time, (GSourceFunc) reload_file, NULL);
    autotimer_on = 1;
  }
}





void toggle_showbad(GtkWidget *w, gpointer data)
{
  (void) data;
  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(w)))
  {
    show_bad_head = 1;
  } else
  {
    show_bad_head = 0;
  }
  //redraw_top_drawing_area();
  //redraw_main_drawing_area();
  gtk_widget_queue_draw(main_window);
}




void toggle_showgood(GtkWidget *w, gpointer data)
{
  (void) data;
  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(w)))
  {
    show_good_data = 1;
  } else
  {
    show_good_data = 0;
  }
  //redraw_top_drawing_area();
  //redraw_main_drawing_area();
  gtk_widget_queue_draw(main_window);
}





void set_show_timing(GtkWidget *w, gpointer data)
{
  (void) w;
  show_timing = GPOINTER_TO_INT( data );
  gtk_widget_queue_draw(main_window);
}




void toggle_showdomain(GtkWidget *w, gpointer data)
{
  (void) data;
  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(w)))
  {
    show_domain = 1;
  } else
  {
    show_domain = 0;
  }
  //redraw_top_drawing_area();
  //redraw_main_drawing_area();
  gtk_widget_queue_draw(main_window);
}





void redraw_top_drawing_area(void)
{
  cairo_t *cr;
  double x, y, w, l, r, g, b;
  cr = gdk_cairo_create(top_drawing_area->window);

  get_rgb_color(WHITE);
  r = rcolor;
  g = gcolor;
  b = bcolor;
  x = 0;
  y = 0;
  w = top_drawing_area_width;
  l = top_drawing_area_height;
  cairo_set_source_rgb(cr, r, g, b);
  cairo_rectangle(cr, x, y, w, l);
  cairo_fill(cr);

  cairo_select_font_face(cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, 10);

  get_rgb_color(nontrimmed_color);
  r = rcolor;
  g = gcolor;
  b = bcolor;
  x = 5;
  y = 5;
  w = 10;
  l = 10;
  cairo_set_source_rgb(cr, r, g, b);
  cairo_rectangle(cr, x, y, w, l);
  cairo_fill(cr);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_move_to(cr, x + 15, y + 9);
  cairo_show_text(cr, curlang[LANGNONTRIMMED]);

  get_rgb_color(nondivided_color);
  r = rcolor;
  g = gcolor;
  b = bcolor;
  x = 5;
  y = 20;
  w = 10;
  l = 10;
  cairo_set_source_rgb(cr, r, g, b);
  cairo_rectangle(cr, x, y, w, l);
  cairo_fill(cr);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_move_to(cr, x + 15, y + 9);
  cairo_show_text(cr, curlang[LANGNONDIVIDED]);

  get_rgb_color(nonscraped_color);
  r = rcolor;
  g = gcolor;
  b = bcolor;
  x = 5;
  y = 35;
  w = 10;
  l = 10;
  cairo_set_source_rgb(cr, r, g, b);
  cairo_rectangle(cr, x, y, w, l);
  cairo_fill(cr);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_move_to(cr, x + 15, y + 9);
  cairo_show_text(cr, curlang[LANGNONSCRAPED]);

  get_rgb_color(bad_color);
  r = rcolor;
  g = gcolor;
  b = bcolor;
  x = 5;
  y = 50;
  w = 10;
  l = 10;
  cairo_set_source_rgb(cr, r, g, b);
  cairo_rectangle(cr, x, y, w, l);
  cairo_fill(cr);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_move_to(cr, x + 15, y + 9);
  cairo_show_text(cr, curlang[LANGBAD]);

  get_rgb_color(nontried_color);
  r = rcolor;
  g = gcolor;
  b = bcolor;
  x = 5;
  y = 65;
  w = 10;
  l = 10;
  cairo_set_source_rgb(cr, r, g, b);
  cairo_rectangle(cr, x, y, w, l);
  cairo_fill(cr);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_move_to(cr, x + 15, y + 9);
  cairo_show_text(cr, curlang[LANGNONTRIED]);

  get_rgb_color(good_color);
  r = rcolor;
  g = gcolor;
  b = bcolor;
  x = 5;
  y = 80;
  w = 10;
  l = 10;
  cairo_set_source_rgb(cr, r, g, b);
  cairo_rectangle(cr, x, y, w, l);
  cairo_fill(cr);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_move_to(cr, x + 15, y + 9);
  cairo_show_text(cr, curlang[LANGFINISHED]);

  get_rgb_color(current_color_outer);
  r = rcolor;
  g = gcolor;
  b = bcolor;
  x = 200;
  y = 5;
  w = 10;
  l = 10;
  cairo_set_source_rgb(cr, r, g, b);
  cairo_rectangle(cr, x, y, w, l);
  cairo_fill(cr);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_move_to(cr, x + 15, y + 9);
  cairo_show_text(cr, curlang[LANGCURRENT]);

  if (show_good_data)
  {
    get_rgb_color(good_color);
    r = rcolor;
    g = gcolor;
    b = bcolor;
    x = 203;
    y = 23;
    w = 4;
    l = 4;
    cairo_set_source_rgb(cr, r, g, b);
    cairo_rectangle(cr, x, y, w, l);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, x + 12, y + 6);
    cairo_show_text(cr, curlang[LANGGOODDATA]);
  }

  if (show_bad_head)
  {
    get_rgb_color(bad_head_color);
    r = rcolor;
    g = gcolor;
    b = bcolor;
    x = 203;
    y = 38;
    w = 4;
    l = 4;
    cairo_set_source_rgb(cr, r, g, b);
    cairo_rectangle(cr, x, y, w, l);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, x + 12, y + 6);
    cairo_show_text(cr, curlang[LANGBADHEAD]);
  }

  if (show_timing)
  {
    get_rgb_color(time_color);
    r = rcolor;
    g = gcolor;
    b = bcolor;
    x = 203;
    y = 53;
    w = 4;
    l = 4;
    cairo_set_source_rgb(cr, r, g, b);
    cairo_rectangle(cr, x, y, w, l);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, x + 12, y + 6);
    cairo_show_text(cr, curlang[LANGTIMING]);
  }

  if (show_domain)
  {
    get_rgb_color(domain_color);
    r = rcolor;
    g = gcolor;
    b = bcolor;
    x = 203;
    y = 68;
    w = 4;
    l = 4;
    cairo_set_source_rgb(cr, r, g, b);
    cairo_rectangle(cr, x, y, w, l);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, x + 12, y + 6);
    cairo_show_text(cr, curlang[LANGDOMAIN]);
  }

  cairo_destroy(cr);
}





void change_left_resolution(GtkWidget *w, gpointer data)
{
  (void) w;
  g_print( "%d\n", GPOINTER_TO_INT( data ) );
  left_square_size = GPOINTER_TO_INT( data );
  redraw_left_vbox();
}





void redraw_left_vbox(void)
{
  cairo_t *cr;
  double x, y, w, l, r, g, b;
  cr = gdk_cairo_create(left_drawing_area->window);

  get_rgb_color(WHITE);
  r = rcolor;
  g = gcolor;
  b = bcolor;
  x = 0;
  y = 0;
  w = left_drawing_area_width;
  l = left_drawing_area_height;
  cairo_set_source_rgb(cr, r, g, b);
  cairo_rectangle(cr, x, y, w, l);
  cairo_fill(cr);

  if (total_size > 0)
  {
    snprintf(tempmessage, sizeof(tempmessage), "redrawing left width = %d, height = %d\n", left_drawing_area_width, left_drawing_area_height);
    message_debug(tempmessage, 0);
    int columns = left_drawing_area_width / left_square_size;
    int rows = left_drawing_area_height / left_square_size;
    int squares = columns * rows;
    long long blocks_per_square = total_size / (squares - 1);

    int count = 0;
    int i = 0;
    int n = 0;

    while (count < squares)
    {
      int color = 0;
      long long position = count * blocks_per_square;
      int status_bits = get_block_status (position, blocks_per_square);
      if (status_bits & NONTRIMMED_BIT)
      {
        color = nontrimmed_color;
      }
      else if (status_bits & NONDIVIDED_BIT)
      {
        color = nondivided_color;
      }
      else if (status_bits & NONSCRAPED_BIT)
      {
        color = nonscraped_color;
      }
      else if (status_bits & BAD_BIT)
      {
        color = bad_color;
      }
      else if (status_bits & NONTRIED_BIT)
      {
        color = nontried_color;
      }
      else if (status_bits & FINISHED_BIT)
      {
        color = good_color;
      }
      else if (status_bits & UNKNOWN_BIT)
      {
        color = unknown_color;
      }
      get_rgb_color(color);
      r = rcolor;
      g = gcolor;
      b = bcolor;
      cairo_set_source_rgb(cr, r, g, b);
      cairo_rectangle(cr, n * left_square_size, i * left_square_size, left_square_size, left_square_size);
      cairo_fill(cr);
      count++;
      if (count > squares)
      {
        snprintf(tempmessage, sizeof(tempmessage), "left count out of range\n");
        message_debug(tempmessage, 0);
        break;
      }
      n++;
      if (n >= columns)
      {
        n = 0;
        i++;
        if (i >= rows)
        {
          snprintf(tempmessage, sizeof(tempmessage), "left rows out of range\n");
          message_debug(tempmessage, 0);
          break;
        }
      }
      position = (count + 1) * blocks_per_square;
    }


    long long position = current_position / blocks_per_square;
    count = 0;
    for (i = 0; i < rows; i++)
    {
      for (n = 0; n < columns; n++)
      {
        if (position == count)
        {
          get_rgb_color(current_color_outer);
          r = rcolor;
          g = gcolor;
          b = bcolor;
          cairo_set_source_rgb(cr, r, g, b);
          cairo_rectangle(cr, n * left_square_size, i * left_square_size, left_square_size, left_square_size);
          cairo_fill(cr);
        }
        count++;
      }
    }

  }
  cairo_destroy(cr);
}






void change_main_resolution(GtkWidget *w, gpointer data)
{
  (void) w;
  snprintf(tempmessage, sizeof(tempmessage), "%d\n", GPOINTER_TO_INT( data ) );
  message_debug(tempmessage, 0);
  main_square_size = GPOINTER_TO_INT( data );
  redraw_main_drawing_area();
}



void change_main_grid_size(GtkWidget *w, gpointer data)
{
  (void) w;
  snprintf(tempmessage, sizeof(tempmessage), "%d\n", GPOINTER_TO_INT( data ) );
  message_debug(tempmessage, 0);
  main_grid_size = GPOINTER_TO_INT( data );
  redraw_main_drawing_area();
}





void redraw_main_drawing_area(void)
{
  double total_squares = main_grid_size;
  double pixels = total_squares * main_square_size * main_square_size;

  double x, y, w, l, r, g, b;
  int max_width = main_scrolled_window_width - 25;
  int adjusted_height = (pixels / max_width) + 1;
  gtk_widget_set_size_request (GTK_WIDGET (main_drawing_area), max_width, adjusted_height + 1);

  //printf("redrawing main width = %d, height = %d\n", main_drawing_area_width, main_drawing_area_height);
  cairo_t *cr;
  cr = gdk_cairo_create(main_drawing_area->window);

  get_rgb_color(WHITE);
  r = rcolor;
  g = gcolor;
  b = bcolor;
  x = 0;
  y = 0;
  w = main_drawing_area_width;
  l = main_drawing_area_height;
  cairo_set_source_rgb(cr, r, g, b);
  cairo_rectangle(cr, x, y, w, l);
  cairo_fill(cr);

  if (total_size > 0)
  {
    snprintf(tempmessage, sizeof(tempmessage), "redrawing main width = %d, height = %d\n", main_drawing_area_width, main_drawing_area_height);
    message_debug(tempmessage, 0);
    gdouble scroll_position = gtk_adjustment_get_value (GTK_ADJUSTMENT (gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW (main_scrolled_window))));
    snprintf(tempmessage, sizeof(tempmessage), "scroll = %f\n",  scroll_position);
    message_debug(tempmessage, 0);
    int scroll_row_start = scroll_position / main_square_size;
    int scroll_row_end = scroll_row_start + (main_drawing_vbox_height / main_square_size);
    int columns = main_drawing_area_width / main_square_size;
    int rows = main_drawing_area_height / main_square_size;
    int squares = columns * rows;
    long long blocks_per_square = total_size / (squares - 1);
    square_adjust = main_square_size / 32;
    int adjustment = 1;
    while (total_size > squares * blocks_per_square)
    {
      adjustment++;
      blocks_per_square = total_size / (squares - adjustment);
    }
    snprintf(tempmessage, sizeof(tempmessage), "total_size=%lld, squares*blocks_per_square=%lld\n", total_size, squares*blocks_per_square);
    message_debug(tempmessage, 0);

    int count = 0;
    int i = 0;
    int n = 0;

    while (count < squares)
      {
        if (i >= scroll_row_start && i <= scroll_row_end)
        {
          int color = 0;
          int bad_head = 0;
          int good_data = 0;
          long long position = count * blocks_per_square;
          int status_bits = get_block_status (position, blocks_per_square);
          int time_bits = get_block_timing (position, blocks_per_square);
          int in_domain = process_domain(position, blocks_per_square, FINISHED, FINISHED);
          if (status_bits & NONTRIMMED_BIT)
          {
            color = nontrimmed_color;
          }
          else if (status_bits & NONDIVIDED_BIT)
          {
            color = nondivided_color;
          }
          else if (status_bits & NONSCRAPED_BIT)
          {
            color = nonscraped_color;
          }
          else if (status_bits & BAD_BIT)
          {
            color = bad_color;
          }
          else if (status_bits & NONTRIED_BIT)
          {
            color = nontried_color;
          }
          else if (status_bits & FINISHED_BIT)
          {
            color = good_color;
          }
          else if (status_bits & UNKNOWN_BIT)
          {
            color = unknown_color;
          }
          if (status_bits & BAD_HEAD_BIT)
          {
            bad_head = 1;
          }
          if (status_bits & FINISHED_BIT)
          {
            good_data = 1;
          }
          get_rgb_color(color);
          r = rcolor;
          g = gcolor;
          b = bcolor;
          cairo_set_source_rgb(cr, r, g, b);
          cairo_rectangle(cr, (n * main_square_size) + 1 + square_adjust, (i * main_square_size) + 1 + square_adjust, main_square_size - 1 - (square_adjust * 2), main_square_size - 1 - (square_adjust * 2));
          cairo_fill(cr);

          if (bad_head && show_bad_head)
          {
            int spot_size = (main_square_size / 4) + 1;
            int spot_adjust = (main_square_size / 2) - (spot_size / 2);
            get_rgb_color(bad_head_color);
            r = rcolor;
            g = gcolor;
            b = bcolor;
            cairo_set_source_rgb(cr, r, g, b);
            cairo_rectangle(cr, (n * main_square_size) + spot_adjust, (i * main_square_size) + spot_adjust, spot_size, spot_size);
            cairo_fill(cr);
          }

          if (good_data && show_good_data)
          {
            int spot_size = (main_square_size / 4) + 1;
            int spot_adjust = (main_square_size / 2) - (spot_size / 2);
            get_rgb_color(good_color);
            r = rcolor;
            g = gcolor;
            b = bcolor;
            cairo_set_source_rgb(cr, r, g, b);
            cairo_rectangle(cr, (n * main_square_size) + spot_adjust, (i * main_square_size) + spot_adjust, spot_size, spot_size);
            cairo_fill(cr);
          }

          if ((time_bits >= show_timing) && show_timing)
          {
            int spot_size = (main_square_size / 4) + 1;
            int spot_adjust = (main_square_size / 2) - (spot_size / 2);
            get_rgb_color(time_color);
            r = rcolor;
            g = gcolor;
            b = bcolor;
            cairo_set_source_rgb(cr, r, g, b);
            cairo_rectangle(cr, (n * main_square_size) + spot_adjust, (i * main_square_size) + spot_adjust, spot_size, spot_size);
            cairo_fill(cr);
          }

          if ((in_domain) && show_domain)
          {
            int spot_size = (main_square_size / 4) + 1;
            int spot_adjust = (main_square_size / 2) - (spot_size / 2);
            get_rgb_color(domain_color);
            r = rcolor;
            g = gcolor;
            b = bcolor;
            cairo_set_source_rgb(cr, r, g, b);
            cairo_rectangle(cr, (n * main_square_size) + spot_adjust, (i * main_square_size) + spot_adjust, spot_size, spot_size);
            cairo_fill(cr);
          }

        }
        count++;
        if (count > squares)
        {
          snprintf(tempmessage, sizeof(tempmessage), "main count out of range\n");
          message_debug(tempmessage, 0);
          break;
        }
        n++;
        if (n >= columns)
        {
          n = 0;
          i++;
          if (i >= rows)
          {
            snprintf(tempmessage, sizeof(tempmessage), "main rows out of range\n");
            message_debug(tempmessage, 0);
            break;
          }
        }
      }

    long long position = current_position / blocks_per_square;
    count = 0;
    for (i = 0; i < rows; i++)
    {
      for (n = 0; n < columns; n++)
      {
        if (position == count)
        {
          get_rgb_color(current_color_outer);
          r = rcolor;
          g = gcolor;
          b = bcolor;
          cairo_set_source_rgb(cr, r, g, b);
          cairo_rectangle(cr, n * main_square_size + 2 + (square_adjust * 2), i * main_square_size + 2 + (square_adjust * 2), main_square_size - 3 - (square_adjust * 4), main_square_size - 3 - (square_adjust * 4));
          cairo_fill(cr);

          int spot_size = (main_square_size / 4) + 1;
          int spot_adjust = (main_square_size / 2) - (spot_size / 2);
          get_rgb_color(current_color_inner);
          r = rcolor;
          g = gcolor;
          b = bcolor;
          cairo_set_source_rgb(cr, r, g, b);
          cairo_rectangle(cr, (n * main_square_size) + spot_adjust, (i * main_square_size) + spot_adjust, spot_size, spot_size);
          cairo_fill(cr);
        }

        //int xl = (n * main_square_size) + 1 + square_adjust;
        //int yl = (i * main_square_size) + 1 + square_adjust;
        //int xh = xl + main_square_size - 1 - (square_adjust * 2);
        //int yh = yl + main_square_size - 1 - (square_adjust * 2);
        int xl = (n * main_square_size) + square_adjust;
        int yl = (i * main_square_size) + square_adjust;
        int xh = xl + main_square_size - (square_adjust * 2);
        int yh = yl + main_square_size - (square_adjust * 2);
        if ( mouse_x != mouse_x_old && mouse_y != mouse_y_old && mouse_x >= xl && mouse_x <= xh && mouse_y >= yl && mouse_y <= yh )
        {
          int spot_size = (main_square_size / 4) + 1;
          int spot_adjust = (main_square_size / 2) - (spot_size / 2);
          get_rgb_color(selected_color);
          r = rcolor;
          g = gcolor;
          b = bcolor;
          cairo_set_source_rgb(cr, r, g, b);
          cairo_rectangle(cr, (n * main_square_size) + spot_adjust, (i * main_square_size) + spot_adjust, spot_size, spot_size);
          cairo_fill(cr);
          get_block_information(blocks_per_square * count, blocks_per_square);
          mouse_x_old = mouse_x;
          mouse_y_old = mouse_y;
        }

        count++;
      }
    }

  }
  cairo_destroy(cr);
}





void get_rgb_color(int color)
{
  bcolor = (double)(color & 0xff) / 255;
  gcolor = (double)( (color >> 8) & 0xff) / 255;
  rcolor = (double)( (color >> 16) & 0xff) / 255;
}





int get_color_from_status(int status_bits)
{
  int count = 0;
  int color = 0;
  int red = 0;
  int green = 0;
  int blue = 0;
  if (status_bits & NONTRIED_BIT)
  {
    int new_color = nontried_color;
    blue += new_color & 0xff;
    green += (new_color >> 8) & 0xff;
    red += (new_color >> 16) & 0xff;
    count++;
  }
  if (status_bits & NONTRIMMED_BIT)
  {
    int new_color = nontrimmed_color;
    blue += new_color & 0xff;
    green += (new_color >> 8) & 0xff;
    red += (new_color >> 16) & 0xff;
    count++;
  }
  if (status_bits & NONDIVIDED_BIT)
  {
    int new_color = nondivided_color;
    blue += new_color & 0xff;
    green += (new_color >> 8) & 0xff;
    red += (new_color >> 16) & 0xff;
    count++;
  }
  if (status_bits & NONSCRAPED_BIT)
  {
    int new_color = nonscraped_color;
    blue += new_color & 0xff;
    green += (new_color >> 8) & 0xff;
    red += (new_color >> 16) & 0xff;
    count++;
  }
  if (status_bits & BAD_BIT)
  {
    int new_color = bad_color;
    blue += new_color & 0xff;
    green += (new_color >> 8) & 0xff;
    red += (new_color >> 16) & 0xff;
    count++;
  }
  if (status_bits & FINISHED_BIT)
  {
    int new_color = good_color;
    blue += new_color & 0xff;
    green += (new_color >> 8) & 0xff;
    red += (new_color >> 16) & 0xff;
    //printf ("good color= 0x%x blue= %d green = %d red= %d\n", new_color, blue, green, red);
    count++;
  }
  if (status_bits & UNKNOWN_BIT)
  {
    int new_color = unknown_color;
    blue += new_color & 0xff;
    green += (new_color >> 8) & 0xff;
    red += (new_color >> 16) & 0xff;
    count++;
  }
  if (count)
  {
    red = red / count;
    green = green / count;
    blue = blue / count;
  }
  if (status_bits & BAD_HEAD_BIT)
  {
    red = red / 2;
    green = green / 2;
    blue = blue / 2;
  }
  color = blue + (green << 8) + (red << 16);
  //printf ("status= 0x%x color= 0x%x\n", status_bits, color);
  return color;
}






// function to initialize needed memory
int initialize_memory(void)
{
  // assign memory for log
  lposition=malloc(sizeof(*lposition) * log_rows);
  if (lposition == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Error allocating log memory (%s)\n", strerror(errno));
    message_now(tempmessage);
    return (-1);
  }
  lsize=malloc(sizeof(*lsize) * log_rows);
  if (lsize == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Error allocating log memory (%s)\n", strerror(errno));
    message_now(tempmessage);
    return (-1);
  }
  lstatus=malloc(sizeof(*lstatus) * log_rows);
  if (lstatus == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Error allocating log memory (%s)\n", strerror(errno));
    message_now(tempmessage);
    return (-1);
  }

  // assign memory for domain
  dposition=malloc(sizeof(*dposition) * domain_rows);
  if (dposition == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Error allocating domain memory (%s)\n", strerror(errno));
    message_now(tempmessage);
    return (-1);
  }
  dsize=malloc(sizeof(*dsize) * domain_rows);
  if (dsize == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Error allocating domain memory (%s)\n", strerror(errno));
    message_now(tempmessage);
    return (-1);
  }
  dstatus=malloc(sizeof(*dstatus) * domain_rows);
  if (dstatus == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Error allocating domain memory (%s)\n", strerror(errno));
    message_now(tempmessage);
    return (-1);
  }

  return 0;
}





int increase_log_memory(int new_lines)
{
  int found_error = 0;
  log_rows += new_lines;
  temp_lposition = realloc(lposition, log_rows * sizeof(*lposition));
  if (temp_lposition == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Error allocating log memory (%s)\n", strerror(errno));
    message_now(tempmessage);
    found_error = -2;
  }
  lposition = temp_lposition;

  temp_lsize = realloc(lsize, log_rows * sizeof(*lsize));
  if (temp_lsize == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Error allocating log memory (%s)\n", strerror(errno));
    message_now(tempmessage);
    found_error = -2;
  }
  lsize = temp_lsize;

  temp_lstatus = realloc(lstatus, log_rows * sizeof(*lstatus));
  if (temp_lstatus == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Error allocating log memory (%s)\n", strerror(errno));
    message_now(tempmessage);
    found_error = -2;
  }
  lstatus = temp_lstatus;
  return found_error;
}





int increase_domain_memory(int new_lines)
{
  int found_error = 0;
  domain_rows += new_lines;
  temp_dposition = realloc(dposition, domain_rows * sizeof(*dposition));
  if (temp_dposition == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Error allocating domain memory (%s)\n", strerror(errno));
    message_now(tempmessage);
    found_error = -2;
  }
  dposition = temp_dposition;

  temp_dsize = realloc(dsize, domain_rows * sizeof(*dsize));
  if (temp_dsize == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Error allocating domain memory (%s)\n", strerror(errno));
    message_now(tempmessage);
    found_error = -2;
  }
  dsize = temp_dsize;

  temp_dstatus = realloc(dstatus, domain_rows * sizeof(*dstatus));
  if (temp_dstatus == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Error allocating domain memory (%s)\n", strerror(errno));
    message_now(tempmessage);
    found_error = -2;
  }
  dstatus = temp_dstatus;
  return found_error;
}





// function to read the log file into memory
int read_log_file(char *log_file)
{
  if (log_file == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Error: No log file specified.\n");
    message_now(tempmessage);
    return (1);
  }

  FILE *readfile;
  readfile = fopen(log_file, "r");
  if (readfile == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Cannot open %s for reading (%s).\n", log_file, strerror(errno));
    message_now(tempmessage);
    return (1);
  }

  char line[MAX_LINE_LENGTH];
  int i = 0;
  int real_line_number = 0;
  int found_current = 0;
  int found_error = 0;
  ddrescue_log = 0;
  while (fgets(line, sizeof line, readfile))
  {
    real_line_number++;
    // process the line here
    int scanline;
    long long temp_position = 0;
    long long temp_size = 0;
    unsigned char temp_status = 0;
    char raw_position[MAX_LINE_LENGTH];
    char raw_size[MAX_LINE_LENGTH];
    char raw_status[MAX_LINE_LENGTH];
    char raw_info[MAX_LINE_LENGTH];
    char raw_errstat[MAX_LINE_LENGTH];
    char rest_of_line[MAX_LINE_LENGTH];
    strcpy (raw_position, "");
    strcpy (raw_size, "");
    strcpy (raw_status, "");
    strcpy (rest_of_line, "");
    scanline = sscanf(line, "%s %s %s %s %s %[^\n]", raw_position, raw_size, raw_status, raw_info, raw_errstat, rest_of_line);
    // ignore blank lines
    if (scanline > 0)
    {
      // if the line doesn't start with a comment then process it
      if (raw_position[0] != '#')
      {
        // the first line should be the current position and status
        // new ddrescue format has copy pass added to first line
        if ( (!found_current && scanline == 2) || (!found_current && scanline == 3) )
        {
          char* endptr;
          current_position = strtoull(raw_position, &endptr, 0);
          if (*endptr)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing position on line %d\n", real_line_number);
            message_now(tempmessage);
            snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
            message_now(tempmessage);
            found_error = 1;
          }
          current_status = strtoull(raw_size, &endptr, 0);
          if (*endptr)
          {
            // check if ddrescue format
            scanline = sscanf(raw_size, "%c %[^\n]", &temp_status, rest_of_line);
            if (scanline == 1)
            {
              if (temp_status == '?')
              {
                current_status = NONTRIED;
              }
              else if (temp_status == '+')
              {
                current_status = FINISHED;
              }
              else if (temp_status == '*')
              {
                current_status = NONTRIMMED;
              }
              else if (temp_status == '/')
              {
                current_status = NONSCRAPED;
              }
              else if (temp_status == '-')
              {
                current_status = BAD;
              }
              else
              {
                snprintf(tempmessage, sizeof(tempmessage), "error processing line %d, status not recognized\n", real_line_number);
                message_now(tempmessage);
                snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
                message_now(tempmessage);
                found_error = -1;
              }
            }
            else
            {
              snprintf(tempmessage, sizeof(tempmessage), "error processing status on line %d\n", real_line_number);
              message_now(tempmessage);
              snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
              message_now(tempmessage);
              found_error = 1;
            }
          }
          found_current = 1;
          //fprintf (stdout, "%d=  %d  0x%08llx  0x%08llx\n", real_line_number, i, current_position, current_status);    //debug
        }
        else if (!found_current)
        {
          snprintf(tempmessage, sizeof(tempmessage), "error processing progress log file line %d, expecting current status\n", real_line_number);
          message_now(tempmessage);
          snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
          message_now(tempmessage);
          found_error = 1;
        }

        // hddclone progress file format
        else if (scanline == 5)
        {
          char* endptr;
          lposition[i] = strtoull(raw_position, &endptr, 0);
          if (*endptr)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing position on line %d\n", real_line_number);
            message_now(tempmessage);
            snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
            message_now(tempmessage);
            found_error = 1;
          }
          lsize[i] = strtoull(raw_size, &endptr, 0);
          if (*endptr)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing size on line %d\n", real_line_number);
            message_now(tempmessage);
            snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
            message_now(tempmessage);
            found_error = 1;
          }
          lstatus[i] = strtoll(raw_status, &endptr, 0);
          if (*endptr)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing status on line %d\n", real_line_number);
            message_now(tempmessage);
            snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
            message_now(tempmessage);
            found_error = 1;
          }
          lstatus[i] += (strtoll(raw_info, &endptr, 0)) << 8;
          if (*endptr)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing info on line %d\n", real_line_number);
            message_now(tempmessage);
            snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
            message_now(tempmessage);
            found_error = 1;
          }
          lstatus[i] += (strtoll(raw_errstat, &endptr, 0)) << 32;
          if (*endptr)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing err/stat on line %d\n", real_line_number);
            message_now(tempmessage);
            snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
            message_now(tempmessage);
            found_error = 1;
          }
          //fprintf (stdout, "%d=  %d  0x%08llx  0x%08llx  0x%08llx\n", real_line_number, i, lposition[i], lsize[i], lstatus[i]);    //debug
          i++;
        }

        //ddrescue log format
        else if (scanline == 3)
        {
          char* endptr;
          temp_position = strtoull(raw_position, &endptr, 0);
          if (*endptr)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing position on line %d\n", real_line_number);
            message_now(tempmessage);
            snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
            message_now(tempmessage);
            found_error = -1;
          }
          temp_size = strtoull(raw_size, &endptr, 0);
          if (*endptr)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing size on line %d\n", real_line_number);
            message_now(tempmessage);
            snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
            message_now(tempmessage);
            found_error = -1;
          }
          scanline = sscanf(raw_status, "%c %[^\n]", &temp_status, rest_of_line);
          if (scanline != 1)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing status on line %d\n", real_line_number);
            message_now(tempmessage);
            found_error = -1;
          }

          if (!found_error)
          {
            lposition[i] = temp_position;
            lsize[i] = temp_size;

            if (temp_status == '?')
            {
              lstatus[i] = NONTRIED;
            }
            else if (temp_status == '+')
            {
              lstatus[i] = FINISHED;
            }
            else if (temp_status == '*')
            {
              lstatus[i] = NONTRIMMED;
            }
            else if (temp_status == '/')
            {
              lstatus[i] = NONSCRAPED;
            }
            else if (temp_status == '-')
            {
              lstatus[i] = BAD;
            }
            else
            {
              snprintf(tempmessage, sizeof(tempmessage), "error processing line %d, status not recognized\n", real_line_number);
              message_now(tempmessage);
              snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
              message_now(tempmessage);
              found_error = -1;
            }
            if (!found_error)
            {
              ddrescue_log = 1;
            }
          }

          //fprintf (stdout, "%d=  %d  0x%08llx  0x%08llx  0x%08llx\n", real_line_number, i, lposition[i], lsize[i], lstatus[i]);    //debug
          i++;
        }

        else
        {
          snprintf(tempmessage, sizeof(tempmessage), "error processing progress log file line %d\n", real_line_number);
          message_now(tempmessage);
          snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
          message_now(tempmessage);
          found_error = 1;
        }
      }
      // line starts with # so check if sector size
      else
      {
        if (strcmp (raw_size, "sectorsize") == 0)
        {
          sector_size = strtoll(raw_status, NULL, 0);
          //fprintf (stdout, "found sector size of %d\n", sector_size);
        }
      }
    }

    // if used up allocated memory then increase it
    if (i >= log_rows)
    {
      increase_log_memory(1000);
    }

    if (found_error)
    {
      break;
    }
  }
  total_lines = i;

  fclose(readfile);
  return (found_error);
}





// function to read the domain file into memory
int read_domain_file(char *domain_file)
{
  if (domain_file == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Error: No domain file specified.\n");
    message_now(tempmessage);
    return (1);
  }

  FILE *readfile;
  readfile = fopen(domain_file, "r");
  if (readfile == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Cannot open %s for reading (%s).\n", domain_file, strerror(errno));
    message_now(tempmessage);
    return (1);
  }

  char line[MAX_LINE_LENGTH];
  int i = 0;
  int real_line_number = 0;
  int found_current = 0;
  int found_error = 0;
  while (fgets(line, sizeof line, readfile))
  {
    found_error = 0;
    real_line_number++;
    // process the line here
    //fprintf (stdout, "line%d= %s", real_line_number, line);    //debug

    int scanline;
    long long temp_position = 0;
    long long temp_size = 0;
    unsigned char temp_status = 0;
    char raw_position[MAX_LINE_LENGTH];
    char raw_size[MAX_LINE_LENGTH];
    char raw_status[MAX_LINE_LENGTH];
    char raw_info[MAX_LINE_LENGTH];
    char raw_errstat[MAX_LINE_LENGTH];
    char rest_of_line[MAX_LINE_LENGTH];
    strcpy (raw_position, "");
    strcpy (raw_size, "");
    strcpy (raw_status, "");
    strcpy (rest_of_line, "");
    scanline = sscanf(line, "%s %s %s %s %s %[^\n]", raw_position, raw_size, raw_status, raw_info, raw_errstat, rest_of_line);
    // ignore blank lines
    if (scanline > 0)
    {
      // if the line doesn't start with a comment then process it
      if (raw_position[0] != '#')
      {
        // the first line should be the current position and status
        // new ddrescue format has copy pass added to first line
        if ( (!found_current && scanline == 2) || (!found_current && scanline == 3) )
        {
          found_current = 1;
        }

        // hddclone progress file format
        else if (scanline == 5)
        {
          char* endptr;
          dposition[i] = strtoull(raw_position, &endptr, 0);
          if (*endptr)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing domain position on line %d\n", real_line_number);
            message_now(tempmessage);
            snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
            message_now(tempmessage);
            found_error = -1;
          }
          dsize[i] = strtoull(raw_size, &endptr, 0);
          if (*endptr)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing domain size on line %d\n", real_line_number);
            message_now(tempmessage);
            snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
            message_now(tempmessage);
            found_error = -1;
          }
          dstatus[i] = strtoll(raw_status, &endptr, 0);
          if (*endptr)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing domain status on line %d\n", real_line_number);
            message_now(tempmessage);
            snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
            message_now(tempmessage);
            found_error = -1;
          }
          dstatus[i] += (strtoll(raw_info, &endptr, 0)) << 8;
          if (*endptr)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing domain info on line %d\n", real_line_number);
            message_now(tempmessage);
            snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
            message_now(tempmessage);
            found_error = -1;
          }
          dstatus[i] += (strtoll(raw_errstat, &endptr, 0)) << 32;
          if (*endptr)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing domain err/stat on line %d\n", real_line_number);
            message_now(tempmessage);
            snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
            message_now(tempmessage);
            found_error = -1;
          }
          //fprintf (stdout, "%d=  %d  0x%08llx  0x%08llx  0x%08llx\n", real_line_number, i, lposition[i], lsize[i], lstatus[i]);    //debug
          i++;
        }

        //ddrescue log format
        else if (scanline == 3)
        {
          char* endptr;
          temp_position = strtoull(raw_position, &endptr, 0);
          if (*endptr)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing domain position on line %d\n", real_line_number);
            message_now(tempmessage);
            snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
            message_now(tempmessage);
            found_error = -1;
          }
          temp_size = strtoull(raw_size, &endptr, 0);
          if (*endptr)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing domain size on line %d\n", real_line_number);
            message_now(tempmessage);
            snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
            message_now(tempmessage);
            found_error = -1;
          }
          scanline = sscanf(raw_status, "%c %[^\n]", &temp_status, rest_of_line);
          if (scanline != 1)
          {
            snprintf(tempmessage, sizeof(tempmessage), "error processing domain status on line %d\n", real_line_number);
            message_now(tempmessage);
            found_error = -1;
          }

          if (!found_error)
          {
            dposition[i] = temp_position;
            dsize[i] = temp_size;

            if (temp_status == '?')
            {
              dstatus[i] = NONTRIED;
            }
            else if (temp_status == '+')
            {
              dstatus[i] = FINISHED;
            }
            else if (temp_status == '*')
            {
              dstatus[i] = NONTRIMMED;
            }
            else if (temp_status == '/')
            {
              dstatus[i] = NONSCRAPED;
            }
            else if (temp_status == '-')
            {
              dstatus[i] = BAD;
            }
            else
            {
              snprintf(tempmessage, sizeof(tempmessage), "error processing line %d, domain status not recognized\n", real_line_number);
              message_now(tempmessage);
              snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
              message_now(tempmessage);
              found_error = -1;
            }

            // if position did not align then increase position and also decrease size if it is not 0
            if ( (found_error & 1) )
            {
              dposition[i]++;
              if (dsize[i] > 0)
              {
                dsize[i]--;
              }
            }
            // if only size did not align then do nothing as it was already cut by the divide
          }

          //fprintf (stdout, "%d=  %d  0x%08llx  0x%08llx  0x%08llx\n", real_line_number, i, lposition[i], lsize[i], lstatus[i]);    //debug
          i++;
        }

        else
        {
          snprintf(tempmessage, sizeof(tempmessage), "error processing domain file line %d\n", real_line_number);
          message_now(tempmessage);
          snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
          message_now(tempmessage);
          found_error = -1;
        }
        found_current = 1;
      }
    }

    // if used up allocated memory then increase it
    if (i >= domain_rows)
    {
      increase_domain_memory(1000);
    }

    if (found_error < 0)
    {
      break;
    }
  }
  domain_lines = i;
  if (found_error < 0)
  {
    domain_lines = 0;
  }

  fclose(readfile);
  return (found_error);
}




// function to read a dmde file as domain
int read_domain_dmde_file(char *dmde_file)
{
  if (dmde_file == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Error: No file specified.\n");
    message_error(tempmessage);
    return (1);
  }
  //snprintf(tempmessage, sizeof(tempmessage), "Reading %s into memory...\n", dmde_file);    //debug

  FILE *readfile;
  readfile = fopen(dmde_file, "r");
  if (readfile == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "Cannot open %s for reading (%s).\n", dmde_file, strerror(errno));
    message_error(tempmessage);
    return (1);
  }

  domain_lines = 1;
  dposition[0] = 0;
  dsize[0] = total_size;
  dstatus[0] = 0;

  char line[MAX_LINE_LENGTH];
  int real_line_number = 0;
  int found_error = 0;
  while (fgets(line, sizeof line, readfile))
  {
    found_error = 0;
    real_line_number++;
    // process the line here
    //snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);    //debug
    int scanline;
    long long temp_position = 0;
    long long temp_size = 0;
    char raw_position[MAX_LINE_LENGTH];
    char raw_size[MAX_LINE_LENGTH];
    char rest_of_line[MAX_LINE_LENGTH];
    strcpy (raw_position, "");
    strcpy (raw_size, "");
    strcpy (rest_of_line, "");
    scanline = sscanf(line, "%s %s %[^\n]", raw_position, raw_size, rest_of_line);
    // ignore blank lines
    if (scanline > 0)
    {
      if (scanline == 2)
      {
        int good = 1;
        char* endptr;
        temp_position = strtoull(raw_position, &endptr, 0);
        if (*endptr)
        {
          snprintf(tempmessage, sizeof(tempmessage), "error processing position on line %d\n", real_line_number);
          message_now(tempmessage);
          snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
          message_now(tempmessage);
          found_error = -1;
          good = 0;
        }
        temp_size = strtoull(raw_size, &endptr, 0);
        if (*endptr)
        {
          snprintf(tempmessage, sizeof(tempmessage), "error processing size on line %d\n", real_line_number);
          message_now(tempmessage);
          snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
          message_now(tempmessage);
          found_error = -1;
          good = 0;
        }
        if (good)
        {
          long long position = temp_position / sector_size;
          if (temp_position % sector_size)
          {
            position = position + 1;
          }
          long long size = temp_size / sector_size;
          if (temp_size % sector_size)
          {
            size = size + 1;
          }
          if (position + size > total_size)
          {
            snprintf(tempmessage, sizeof(tempmessage), "end position greater than source size on line %d\n", real_line_number);
            message_now(tempmessage);
            snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
            message_now(tempmessage);
            found_error = -1;
          }
          else if (position > 0 && size > 0)
          {
            int ret = add_to_domain(position, size);
            if (ret)
            {
              snprintf(tempmessage, sizeof(tempmessage), "error adding to domain from line %d\n", real_line_number);
              message_now(tempmessage);
              snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
              message_now(tempmessage);
              found_error = -1;
            }
          }
        }
      }
      else
      {
        snprintf(tempmessage, sizeof(tempmessage), "format error on line %d\n", real_line_number);
        message_now(tempmessage);
        snprintf(tempmessage, sizeof(tempmessage), "line%d= %s", real_line_number, line);
        message_now(tempmessage);
        found_error = -1;
      }
    }
  }

  fclose(readfile);
  return (found_error);
}




int add_to_domain(long long position, long long size)
{
  //fprintf (debug_file, "change_domain_chunk_status 0x%06llx  0x%06llx\n", position, size);    //debug
  int line_number;
  //fprintf (debug_file,"before\n");
  for (line_number = 0; line_number < domain_lines; line_number++)
  {
    //fprintf (debug_file, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition[line_number], dsize[line_number], (dstatus[line_number] & STATUS_MASK), ((dstatus[line_number] & INFO_MASK) >> 8), (dstatus[line_number] >> 32) );
  }

  int block;
  while (1)
  {
    //fprintf (debug_file,"while start position=%06llx  size=0x%06llx\n", position, size);
    block = find_domain_block(position);
    if (block < 0)
    {
      strcpy (tempmessage, curlang[LANGDOMAINBLOCKNOTFOUND]);
      message_error(tempmessage);
      snprintf(tempmessage, sizeof(tempmessage), "\nposition=%06llx  size=0x%06llx", position, size);
      message_error(tempmessage);
      print_gui_error_message(error_message, curlang[LANGERROR], 1);
      clear_error_message();

      //fprintf (debug_file,"error1 position=%06llx  size=0x%06llx\n", position, size);
      for (line_number = 0; line_number < domain_lines; line_number++)
      {
        //fprintf (debug_file, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition[line_number], dsize[line_number], (dstatus[line_number] & STATUS_MASK), ((dstatus[line_number] & INFO_MASK) >> 8), (dstatus[line_number] >> 32) );
      }

      return -1;
    }
    // see if chunk fits within the block
    if ( position >= dposition[block] && (position + size) <= (dposition[block] + dsize[block]) )
    {
      // check if already finished status, if so then nothing to do
      // we are using status mask in case of imported domain file, but extra status will not be kept
      if ( (dstatus[block] & STATUS_MASK) == (FINISHED & STATUS_MASK) )
      {
        return 0;
      }
      // it fits so break to allow that processing below
      else
      {
        break;
      }
    }
    else
    {
      // chunk is bigger than block, need to process
      // check if position matches
      if (position == dposition[block])
      {
        //fprintf (debug_file,"while1\n");
        // if position matches then we can mark the block, then move position forward to next block reduce size and process
        dstatus[block] = FINISHED;
        position += dsize[block];
        size -= dsize[block];
        for (line_number = 0; line_number < 6; line_number++)
        {
          //fprintf (debug_file, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition[line_number], dsize[line_number], (dstatus[line_number] & STATUS_MASK), ((dstatus[line_number] & INFO_MASK) >> 8), (dstatus[line_number] >> 32) );
        }

        // check for same status on next block and merge if needed
        if (block < domain_lines - 1)
        {
          //fprintf (debug_file,"while2\n");
          if ( (dstatus[block] & STATUS_MASK) == (dstatus[block+1] & STATUS_MASK) )
          {
            //fprintf (debug_file,"while3\n");
            dposition[block+1] -= dsize[block];
            dsize[block+1] += dsize[block];
            delete_domain_line(block);
            for (line_number = 0; line_number < 6; line_number++)
            {
              //fprintf (debug_file, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition[line_number], dsize[line_number], (dstatus[line_number] & STATUS_MASK), ((dstatus[line_number] & INFO_MASK) >> 8), (dstatus[line_number] >> 32) );
            }
          }
        }
        // check for same status on previous block and merge if needed
        if (block > 0 && (FINISHED & STATUS_MASK) == (dstatus[block-1] & STATUS_MASK) )
        {
          //fprintf (debug_file,"while4\n");
          dsize[block-1] += dsize[block];
          delete_domain_line(block);
          for (line_number = 0; line_number < 6; line_number++)
          {
            //fprintf (debug_file, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition[line_number], dsize[line_number], (dstatus[line_number] & STATUS_MASK), ((dstatus[line_number] & INFO_MASK) >> 8), (dstatus[line_number] >> 32) );
          }
        }
      }
      // check if position is in middle of the block
      else if (position > dposition[block])
      {
        //fprintf (debug_file,"while5\n");
        // check for same status on next block and merge if needed
        if (block < domain_lines - 1)
        {
          //fprintf (debug_file,"while6\n");
          if ( (FINISHED & STATUS_MASK) == (dstatus[block+1] & STATUS_MASK) )
          {
            //fprintf (debug_file,"while7\n");
            dsize[block+1] += dposition[block+1] - position;
            dposition[block+1] = position;
          }
        }
        // move position ahead reduce size and shorten size
        long long diff = (dposition[block] + dsize[block]) - position;
        //dsize[block] -= diff;
        position += diff;
        size -= diff;
        for (line_number = 0; line_number < 6; line_number++)
        {
          //fprintf (debug_file, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition[line_number], dsize[line_number], (dstatus[line_number] & STATUS_MASK), ((dstatus[line_number] & INFO_MASK) >> 8), (dstatus[line_number] >> 32) );
        }
      }
      // the position is before the block which should not be possible
      else
      {
        strcpy (tempmessage, curlang[LANGDOMAINBLOCKNOTFOUND]);
        message_error(tempmessage);
        snprintf(tempmessage, sizeof(tempmessage), "\nposition=%06llx  size=0x%06llx", position, size);
        message_error(tempmessage);
        print_gui_error_message(error_message, curlang[LANGERROR], 1);
        clear_error_message();

        //fprintf (debug_file,"error2 position=%06llx  size=0x%06llx\n", position, size);
        for (line_number = 0; line_number < domain_lines; line_number++)
        {
          //fprintf (debug_file, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition[line_number], dsize[line_number], (dstatus[line_number] & STATUS_MASK), ((dstatus[line_number] & INFO_MASK) >> 8), (dstatus[line_number] >> 32) );
        }

        return -1;
      }
    }
    //fprintf (debug_file,"while end position=%06llx  size=0x%06llx\n", position, size);
  }

  // the chunk fits in the block
  // if postion and size match the block then only need to change status
  if ( position == dposition[block] && size == dsize[block] )
  {
    dstatus[block] = FINISHED;
    if (block < domain_lines - 1)
    {
      if ( (dstatus[block] & STATUS_MASK) == (dstatus[block+1] & STATUS_MASK) )
      {
        dposition[block+1] -= size;
        dsize[block+1] += size;
        delete_domain_line(block);
      }
    }
    if (block > 0)
    {
      if ( (dstatus[block] & STATUS_MASK) == (dstatus[block-1] & STATUS_MASK) )
      {
        dsize[block-1] += dsize[block];
        delete_domain_line(block);
      }
    }
    //return 0;
  }
  // if position matches then check line above and merge or insert new line
  else if (position == dposition[block])
  {
    if (block > 0 && (FINISHED & STATUS_MASK) == (dstatus[block-1] & STATUS_MASK) )
    {
      dsize[block-1] += size;
      dposition[block] += size;
      dsize[block] -= size;
      //return 0;
    }
    else
    {
      insert_domain_line(block+1, position + size, dsize[block] - size, dstatus[block]);
      dposition[block] = position;
      dsize[block] = size;
      dstatus[block] = FINISHED;
      //return 0;
    }
  }
  // if end position matches then check line below and merge or insert new line
  else if (position + size == dposition[block] + dsize[block])
  {
    if (block < domain_lines - 1 && (FINISHED & STATUS_MASK) == (dstatus[block+1] & STATUS_MASK) )
    {
      dposition[block+1] -= size;
      dsize[block+1] += size;
      dsize[block] -= size;
      //return 0;
    }
    else
    {
      insert_domain_line(block, dposition[block], dsize[block] - size, dstatus[block]);
      dposition[block+1] = dposition[block] + dsize[block];
      dsize[block+1] = size;
      dstatus[block+1] = FINISHED;
      //return 0;
    }
  }
  // if chunk is in the middle of the block then insert 2 lines and adjust
  else
  {
    long long block_end = dposition[block] + dsize[block];
    insert_domain_line(block+1, position + size, block_end - (position + size), dstatus[block]);
    insert_domain_line(block, dposition[block], position - dposition[block], dstatus[block]);
    dposition[block+1] = position;
    dsize[block+1] = size;
    dstatus[block+1] = FINISHED;
  }

  //fprintf (debug_file,"after\n");
  for (line_number = 0; line_number < domain_lines; line_number++)
  {
    //fprintf (debug_file, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition[line_number], dsize[line_number], (dstatus[line_number] & STATUS_MASK), ((dstatus[line_number] & INFO_MASK) >> 8), (dstatus[line_number] >> 32) );
  }

  return 0;
}




int insert_domain_line(int line, long long position, long long size, long long status)
{
  if (domain_lines+1 >= domain_rows)
  {
    int ret = increase_domain_memory(1000);
    if (ret)
    {
      return ret;
    }
  }
  int i;
  for (i = domain_lines; i > line && i>0; i--)
  {
    dposition[i] = dposition[i-1];
    dsize[i] = dsize[i-1];
    dstatus[i] = dstatus[i-1];
  }
  dposition[i] = position;
  dsize[i] = size;
  dstatus[i] = status;
  domain_lines++;
  return (0);
}




int find_domain_block(long long position)
{
  int first = 0;
  int last = domain_lines>0?domain_lines - 1:0;
  int middle = (first + last) / 2;
  while (first <= last)
  {
    //fprintf (stdout, "pos=0x%llx\n", position);
    if (position >= dposition[middle] && position < dposition[middle] + dsize[middle])
    {
      return (middle);
    }
    else if (position > dposition[middle])
    {
      first = middle + 1;
    }
    else
    {
      last = middle - 1;
    }
    middle = (first + last) / 2;
  }
  return (-1);
}




int delete_domain_line(int line)
{
  int i;
  if(line>=0 && domain_lines>0)
  {
    for (i = line; i < domain_lines; i++)
    {
      dposition[i] = dposition[i+1];
      dsize[i] = dsize[i+1];
      dstatus[i] = dstatus[i+1];
    }
    domain_lines--;
  }
  return (0);
}





int check_log(void)
{
  snprintf(tempmessage, sizeof(tempmessage), "Checking progress log file...\n");
  message_now(tempmessage);
  int i;
  int fail = 0;

  // check that first position is 0
  if (lposition[0] != 0)
  {
    snprintf(tempmessage, sizeof(tempmessage), "The first position is not 0\n");
    message_now(tempmessage);
    fail = 1;
  }

  for (i = 0; i < total_lines-1; i++)
  {
    // check if there is an overlap
    if ( (lposition[i] + lsize[i]) > lposition[i+1] )
    {
      snprintf(tempmessage, sizeof(tempmessage), "Overlap found on line %d\n", i+1);
      message_now(tempmessage);
      fail = 2;
    }
    // check if size is 0
    if (lsize[i] == 0)
    {
      snprintf(tempmessage, sizeof(tempmessage), "Size of 0 found on line %d\n", i+1);
      message_now(tempmessage);
      fail = 3;
    }
    // check if there is a gap
    if ( (lposition[i] + lsize[i]) < lposition[i+1])
    {
      snprintf(tempmessage, sizeof(tempmessage), "Gap found on line %d\n", i+1);
      message_now(tempmessage);
      fail = 4;
    }
    // check if the next status is the same
    if (lstatus[i] == lstatus[i+1])
    {
      snprintf(tempmessage, sizeof(tempmessage), "Same status found on line %d\n", i+1);
      message_now(tempmessage);
      fail = 5;
    }
  }

  // get total size from last position
  total_size = total_lines > 0 ? (lposition[total_lines-1] + lsize[total_lines-1]) : 0;
  snprintf(tempmessage, sizeof(tempmessage), "total size = %lld\n", total_size);
  message_now(tempmessage);

  return fail;
}




int check_domain(void)
{
  snprintf(tempmessage, sizeof(tempmessage), "Checking domain file...\n");
  message_now(tempmessage);
  int i;
  int fail = 0;
  for (i = 0; i < domain_lines-1; i++)
  {
    // check if there is an overlap
    if ( (dposition[i] + dsize[i]) > dposition[i+1] )
    {
      snprintf(tempmessage, sizeof(tempmessage), "Overlap found on domain line %d\n", i+1);
      message_now(tempmessage);
      fail = 2;
    }
  }
  return fail;
}





int get_block_status(long long position, long long size)
{
  int status = 0;
  int line = find_block(position);
  if (line == -1)
  {
    //snprintf(tempmessage, sizeof(tempmessage), "Error: Position 0x%llx not found in progress log file\n", position);
    //message_now(tempmessage);
    return -1;
  }
  // process current status
  status = status | process_status(line);
  // check if the chunk fits in the block and if not then get next block
  // but only get the status of up to 255 more chunks before moving on
  // otherwise it can take a long time with a large log file
  long long block_end = lposition[line] + lsize[line];
  size -= lposition[line] - position;
  int chunk_count = 255;
  while(chunk_count > 0)
  {
    chunk_count--;
    if (lposition[line] + size > block_end || lposition[line] > block_end)
    {
      size -= lsize[line];
      line++;
      if (line > total_lines - 1)
      {
        break;
      }
      block_end = lposition[line] + lsize[line];
      status = status | process_status(line);
    }
    else
    {
      break;
    }
    //break;
  }

  return status;
}





int get_block_timing(long long position, long long size)
{
  long long new_size = size;
  long long time = 0;
  int high_time = 0;
  int line = find_block(position);
  if (line == -1)
  {
    //snprintf(tempmessage, sizeof(tempmessage), "Error: Position 0x%llx not found in progress log file\n", position);
    //message_now(tempmessage);
    return -1;
  }
  // process current time
  time = (lstatus[line] >> 32) & 0xff;
  if (time > high_time)
  {
    high_time = time;
  }
  // check if the chunk fits in the block and if not then get next block
  // but only get the status of up to 255 more chunks before moving on
  // otherwise it can take a long time with a large log file
  long long block_end = lposition[line] + lsize[line];
  new_size -= lposition[line] - position;
  int chunk_count = 255;
  while(chunk_count > 0)
  {
    chunk_count--;
    if (lposition[line] + new_size > block_end || lposition[line] > block_end)
    {
      new_size -= lsize[line];
      line++;
      if (line > total_lines - 1)
      {
        break;
      }
      block_end = lposition[line] + lsize[line];
      time = (lstatus[line] >> 32) & 0xff;
      if (time > high_time)
      {
        high_time = time;
      }
    }
    else
    {
      break;
    }
    //break;
  }

  return high_time;
}





int get_block_information(long long position, long long size)
{ long long new_size = size;
  long long ret = 0;
  int line = find_block(position);
  if (line == -1)
  {
    //snprintf(tempmessage, sizeof(tempmessage), "Error: Position 0x%llx not found in progress log file\n", position);
    //message_now(tempmessage);
    return -1;
  }
  int maxcount = 64;
  long long nontried = 0;
  long long nontrimmed = 0;
  long long nondivided = 0;
  long long nonscraped = 0;
  long long bad = 0;
  long long finished = 0;
  long long hightime = 0;
  char tempchar[2148];
  char lines[maxcount*sizeof(tempchar)];
  strcpy (lines, "");
  // process current status
  //fprintf (stdout, "1 pos=%lld siz=%lld nsiz=%lld lin=%d lpos=%lld lsiz=%lld\n", position, size, new_size, line, lposition[line], lsize[line]);    //debug
  ret = process_information(position, size, line);
  if (ret > 0)
  {
    nontried = nontried + temp_nontried;
    nontrimmed = nontrimmed + temp_nontrimmed;
    nondivided = nondivided + temp_nondivided;
    nonscraped = nonscraped + temp_nonscraped;
    bad = bad + temp_bad;
    finished = finished + temp_finished;
    if (temp_hightime > hightime)
    {
      hightime = temp_hightime;
    }
    if (ddrescue_log)
    {
      snprintf (tempchar, sizeof(tempchar), "0x%06llx \t0x%06llx \t%c\n", lposition[line], lsize[line], ddrstatus);
    }
    else
    {
      snprintf (tempchar, sizeof(tempchar), "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", lposition[line], lsize[line], (lstatus[line] & STATUS_MASK), ((lstatus[line] & INFO_MASK) >> 8), (lstatus[line] >> 32) );
    }
    strcat (lines, tempchar);
  }
  // check if the chunk fits in the block and if not then get next block
  long long block_end = lposition[line] + lsize[line];
  new_size -= lposition[line] - position;
  int chunk_count = maxcount;
  while(chunk_count > 0)
  {
    //fprintf (stdout, "2 pos=%lld siz=%lld nsiz=%lld lin=%d lpos=%lld lsiz=%lld\n", position, size, new_size, line, lposition[line], lsize[line]);    //debug
    chunk_count--;
    if (lposition[line] + new_size > block_end || lposition[line] > block_end)
    {
      new_size -= lsize[line];
      line++;
      if (line > total_lines - 1)
      {
        break;
      }
      block_end = lposition[line] + lsize[line];
      //fprintf (stdout, "3 pos=%lld siz=%lld nsiz=%lld lin=%d lpos=%lld lsiz=%lld end=%lld\n", position, size, new_size, line, lposition[line], lsize[line], block_end);    //debug
      ret = process_information(position, size, line);
      if (ret > 0)
      {
        nontried = nontried + temp_nontried;
        nontrimmed = nontrimmed + temp_nontrimmed;
        nondivided = nondivided + temp_nondivided;
        nonscraped = nonscraped + temp_nonscraped;
        bad = bad + temp_bad;
        finished = finished + temp_finished;
        if (temp_hightime > hightime)
        {
          hightime = temp_hightime;
        }
        if (ddrescue_log)
        {
          snprintf (tempchar, sizeof(tempchar), "0x%06llx \t0x%06llx \t%c\n", lposition[line], lsize[line], ddrstatus);
        }
        else
        {
          snprintf (tempchar, sizeof(tempchar), "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", lposition[line], lsize[line], (lstatus[line] & STATUS_MASK), ((lstatus[line] & INFO_MASK) >> 8), (lstatus[line] >> 32) );
        }
        strcat (lines, tempchar);
      }
    }
    else
    {
      break;
    }
    //break;
  }

  if (chunk_count == 0)
  {
    fprintf (stdout, "block line count exceeded\n");
  }
  long long total = nontried + nontrimmed + nondivided + nonscraped + bad + finished;
  if (total != size)
  {
    fprintf (stdout, "block sum %lld does not equal block total %lld\n", total, size);
  }

  char label[8092+(maxcount*sizeof(tempchar))];
  strcpy (label, "");
  snprintf (tempchar, sizeof(tempchar), "0x%llx-0x%llx  (%lld-%lld)\n", position, (position + size) - 1, position, (position + size) - 1);
  strcat (label, tempchar);
  snprintf (tempchar, sizeof(tempchar), "%s=%lld  ", curlang[LANGFINISHED], finished);
  strcat (label, tempchar);
  snprintf(tempchar, sizeof(tempchar), "%s=%lld\n", curlang[LANGBAD], bad);
  strcat (label, tempchar);
  snprintf(tempchar, sizeof(tempchar), "%s=%lld  ", curlang[LANGNONTRIED], nontried);
  strcat (label, tempchar);
  snprintf(tempchar, sizeof(tempchar), "%s=%lld\n", curlang[LANGNONTRIMMED], nontrimmed);
  strcat (label, tempchar);
  snprintf(tempchar, sizeof(tempchar), "%s=%lld  ", curlang[LANGNONDIVIDED], nondivided);
  strcat (label, tempchar);
  snprintf(tempchar, sizeof(tempchar), "%s=%lld\n", curlang[LANGNONSCRAPED], nonscraped);
  strcat (label, tempchar);
  snprintf(tempchar, sizeof(tempchar), "%s=%d  ", curlang[LANGAREAS], maxcount - chunk_count);
  strcat (label, tempchar);
  snprintf(tempchar, sizeof(tempchar), "%s=%lld\n", curlang[LANGTIMING], hightime);
  strcat (label, tempchar);
  strcat (label, lines);
  //fprintf (stdout, "%s", label);
  //fprintf (stdout, "count=%d\n", maxcount - chunk_count);
  gtk_label_set_text(GTK_LABEL(block_information_label), label);

  return 0;
}





int process_domain(long long position, int size, int status, int status_mask)
{
  if (show_domain)
  {
    int first = 0;
    int last = domain_lines - 1;
    int middle = (first + last) / 2;
    while (first <= last)
    {
      //fprintf (stdout, "position=0x%llx size=0x%x dposition=0x%llx dsize=0x%llx\n", position, size, dposition[middle], dsize[middle]);    //debug
      //fprintf (stdout, "first=%d middle=%d end=%d\n", first, middle, last);    //debug
      // check if any part of the chunk is in the domain
      if ( ( (position >= dposition[middle] && position < dposition[middle] + dsize[middle]) || (dposition[middle] >= position && dposition[middle] < position + size) ) && ((dstatus[middle] & status_mask) == status) )
      {
        // found a matching block
        return 1;
      }
      else if (position > dposition[middle])
      {
        first = middle + 1;
      }
      else
      {
        last = middle - 1;
      }
      middle = (first + last) / 2;
    }
  }
  return 0;
}





int process_status(int line)
{
  int status = 0;
  int mask = 0x7f;
  if ( (lstatus[line] & mask) == (NONTRIED & mask) )
  {
    status = NONTRIED_BIT;
  }
  else if ( (lstatus[line] & mask) == (NONTRIMMED & mask) )
  {
    status = NONTRIMMED_BIT;
  }
  else if ( (lstatus[line] & mask) == (NONDIVIDED & mask) )
  {
    status = NONDIVIDED_BIT;
  }
  else if ( (lstatus[line] & mask) == (NONSCRAPED & mask) )
  {
    status = NONSCRAPED_BIT;
  }
  else if ( (lstatus[line] & mask) == (BAD & mask) )
  {
    status = BAD_BIT;
  }
  else if ( (lstatus[line] & mask) == (FINISHED & mask) )
  {
    status = FINISHED_BIT;
  }
  else
  {
    status = UNKNOWN_BIT;
  }
  if ( lstatus[line] & BAD_HEAD)
  {
    status = status | BAD_HEAD_BIT;
  }
  return status;
}





long long process_information(long long position, long long size, int line)
{
  temp_nontried = 0;
  temp_nontrimmed = 0;
  temp_nondivided = 0;
  temp_nonscraped = 0;
  temp_bad = 0;
  temp_finished = 0;
  temp_hightime = 0;
  long long start = position;
  if (lposition[line] > position)
  {
    start = lposition[line];
  }
  long long end = (position + size);
  if ((lposition[line] + lsize[line]) < end)
  {
    end = (lposition[line] + lsize[line]);
  }
  long long total = end - start;
  if (total < 0)
  {
    total = 0;
  }
  ddrstatus = ' ';
  int mask = 0x7f;
  if ( (lstatus[line] & mask) == (NONTRIED & mask) )
  {
    temp_nontried = total;
    ddrstatus = '?';
    //fprintf (stdout, "temp_nontried=%lld\n", total);    // debug
  }
  else if ( (lstatus[line] & mask) == (NONTRIMMED & mask) )
  {
    temp_nontrimmed = total;
    ddrstatus = '*';
    //fprintf (stdout, "temp_nontrimmed=%lld\n", total);    // debug
  }
  else if ( (lstatus[line] & mask) == (NONDIVIDED & mask) )
  {
    temp_nondivided = total;
    ddrstatus  = '*';
    //fprintf (stdout, "temp_nondivided=%lld\n", total);    // debug
  }
  else if ( (lstatus[line] & mask) == (NONSCRAPED & mask) )
  {
    temp_nonscraped = total;
    ddrstatus  = '/';
    //fprintf (stdout, "temp_nonscraped=%lld\n", total);    // debug
  }
  else if ( (lstatus[line] & mask) == (BAD & mask) )
  {
    temp_bad = total;
    ddrstatus  = '-';
    //fprintf (stdout, "temp_bad=%lld\n", total);    // debug
  }
  else if ( (lstatus[line] & mask) == (FINISHED & mask) )
  {
    temp_finished = total;
    ddrstatus  = '+';
    //fprintf (stdout, "temp_finished=%lld\n", total);    // debug
  }
  temp_hightime  = (lstatus[line] >> 32) & 0xff;
  return total;
}





int find_block(long long position)
{
  int first = 0;
  int last = total_lines - 1;
  int middle = (first + last) / 2;
  while (first <= last)
  {
    if (position >= lposition[middle] && position < lposition[middle] + lsize[middle])
    {
      return (middle);
    }
    else if (position > lposition[middle])
    {
      first = middle + 1;
    }
    else
    {
      last = middle - 1;
    }
    middle = (first + last) / 2;
  }
  return (-1);
}





int find_next_block(int line, long long status, long long mask)
{
  int i;
  for (i = line+1; i < total_lines; i++)
  {
    if ( (lstatus[i] & mask) == (status & mask) )
    {
      return i;
    }
  }
  return (-1);
}





int do_nanosleep(unsigned long long time)
{
  // sleep time in nanoseconds
  struct timespec tim, tim2;
  tim.tv_sec = time / 1000000000;
  tim.tv_nsec = time % 1000000000;
  nanosleep(&tim , &tim2);
  return (0);
}








int message_exit(char *message)
{
  fprintf (stdout, "%s", message);
  strncat(exit_message, message, EXIT_MESSAGE_SIZE - strlen(exit_message) - 1);
  return 0;
}


int message_display(char *message)
{
  fprintf (stdout, "%s", message);
  strncat(display_message, message, DISPLAY_MESSAGE_SIZE - strlen(display_message) - 1);
  return 0;
}


int message_now(char *message)
{
  fprintf (stdout, "%s", message);
  return 0;
}

int message_debug(char *message, int level)
{
  if (debug > 0 && debug > level)
  {
    fprintf (stdout, "%s", message);
  }
  return 0;
}

int message_error(char *message)
{
  if (debug > 0)
  {
    fprintf (stdout, "%s", message);
  }
  strncat(error_message, message, ERROR_MESSAGE_SIZE - strlen(error_message) - 1);
  return 0;
}

int clear_error_message(void)
{
  strcpy(error_message, "\0");
  return 0;
}




// function to display help
void help(void)
{
  fprintf (stdout, "\n");
  fprintf (stdout, "Usage: hddscviewer [options]\n");
  fprintf (stdout, "\n");
  fprintf (stdout, "OPTIONS:\n");
  fprintf (stdout, " -v, --version              Show version and exit\n");
  fprintf (stdout, " -h, --help                 Show this help and exit\n");
  fprintf (stdout, "\n");
  fprintf (stdout, "Report bugs to sdcomputingservice@gmail.com\n");
  fprintf (stdout, "Please be sure to include 'hddscviewer' in the subject line. \n");
  fprintf (stdout, "\n");
}



// function to display version
void version(void)
{
  fprintf (stdout, "%s %s\n", title, version_number);
  fprintf (stdout, "Copyright (C) %d Scott Dwyer.\n", copyright_year);
  fprintf (stdout, "License type: FREEWARE\n");
  fprintf (stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
}





int set_language(void)
{
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  //gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
  gtk_window_set_title(GTK_WINDOW(window), "Language Options");
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  gint handler_id1 = g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  GtkWidget *box = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), box);

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
  //gint handler_id3 = g_signal_connect (exbutton, "clicked", G_CALLBACK (export_language_file), NULL);
  gint handler_id4 = g_signal_connect (imbutton, "clicked", G_CALLBACK (import_language_file), NULL);

  gtk_widget_show_all(window);
  gtk_main();
  g_signal_handler_disconnect(window, handler_id1);
  g_signal_handler_disconnect(enbutton, handler_id2);
  //g_signal_handler_disconnect(exbutton, handler_id3);
  g_signal_handler_disconnect(imbutton, handler_id4);
  gtk_widget_hide_all(window);
  gtk_widget_destroy(window);

  return 0;
}





int print_gui_error_message(char *message, char *title, int type)
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
    //dialog = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "%s", message);
  }
  else
  {
    message_type = GTK_MESSAGE_INFO;
    //dialog = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", message);
  }
  dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, message_type, GTK_BUTTONS_OK, "%s", message);
  gtk_window_set_keep_above(GTK_WINDOW(dialog), TRUE);
  gtk_window_set_title(GTK_WINDOW(dialog), title);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  gtk_widget_destroy(window);

  return 0;
}





void export_language_file(void)
{
  /* Create a new file selection widget */
  filelangex = gtk_file_selection_new (curlang[LANGEXPORTLANG]);
  gtk_file_selection_hide_fileop_buttons (GTK_FILE_SELECTION (filelangex));

  /* Connect the ok_button to file_ok_sel function */
  g_signal_connect (GTK_FILE_SELECTION (filelangex)->ok_button, "clicked", G_CALLBACK (file_export_sel), (gpointer) filelangex);

  /* Connect the cancel_button to destroy the widget */
  g_signal_connect_swapped (GTK_FILE_SELECTION (filelangex)->cancel_button, "clicked", G_CALLBACK (gtk_widget_destroy), filelangex);

  // set default file name
  //gtk_file_selection_set_filename (GTK_FILE_SELECTION(filew), "progress.log");

  gtk_widget_show (filelangex);
}





// get the export filename and load it
static void file_export_sel( GtkWidget *w, GtkFileSelection *fs )
{
  (void) w;
  //g_print ("%s\n", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
  char export_file[1024] = "";
  snprintf (export_file, sizeof(export_file), "%s", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
  printf ("%s\n",  export_file);
  gtk_widget_destroy(filelangex);

  FILE *writefile;
  writefile = fopen(export_file, "w");
  if (writefile == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "%s: %s (%s)", curlang[LANGLANGEXPORTERR], export_file, strerror(errno));
    message_error(tempmessage);
    print_gui_error_message(error_message, curlang[LANGERROR], 1);
    clear_error_message();
  }

  fprintf(writefile, "%s", program_title);
  int i;
  for (i = 0; i < LANGCOUNT; i++)
  {
    fprintf(writefile, "\n|_|%d|_|\n", i);
    fprintf(writefile, "%s", enlang[i]);
  }

  fclose(writefile);
}





void import_language_file(void)
{
  GtkWidget *dialog;
  dialog = gtk_file_chooser_dialog_new (curlang[LANGIMPORTLANG],
                                        GTK_WINDOW(language_window),
                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                        NULL);
  if ( access( "/usr/local/share/doc/hddscviewer/Language/English", F_OK ) == 0 )
  {
    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), "/usr/local/share/doc/hddscviewer/Language");
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
    file_import_sel(filename);
    g_free (filename);
  }
  gtk_widget_destroy (dialog);
}





// get the import filename and load it
static void file_import_sel( char *import_file )
{
  FILE *readfile;
  readfile = fopen(import_file, "r");
  if (readfile == NULL)
  {
    snprintf(tempmessage, sizeof(tempmessage), "%s: %s (%s)", curlang[LANGLANGIMPORTERR], import_file, strerror(errno));
    message_error(tempmessage);
    print_gui_error_message(error_message, curlang[LANGERROR], 1);
    clear_error_message();
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
        strcpy (found_title, line);
        firstline = 0;
      }
      else
      {
        int scanline;
        int new_count = -1;
        scanline = sscanf(line, "| _ | %d | _ |", &new_count);
        if (scanline == 1)
        {
          //fprintf (stdout, "num=%d, line=%s", new_count, line);
          // remove the last new line from the end
          if (found_count >= 0)
          {
            int l = strlen(newlang[found_count]);
            if (l > 0 && newlang[found_count][l-1] == '\n')
            {
              newlang[found_count][l-1] = '\0';
            }
          }
          found_count = new_count;
          strcpy (newlang[found_count], "");
          count++;
        }
        else if (found_count >= 0 && found_count < LANGCOUNT)
        {
          strcat (newlang[found_count], line);
          //fprintf (stdout, "%s", newlang[found_count]);
        }
      }
    }

    count--;
    if (count == found_count && count == LANGCOUNT - 1)
    {
      copy_newlanguage();
      snprintf(tempmessage, sizeof(tempmessage), "%s", curlang[LANGLANGCHANGESUCCESS]);
      message_error(tempmessage);
      print_gui_error_message(error_message, curlang[LANGSUCCESS], 0);
      clear_error_message();
    }
    else
    {
      //fprintf (stdout, "count=%d found_count=%d LANGCOUNT=%d\n", count, found_count, LANGCOUNT-1);
      snprintf(tempmessage, sizeof(tempmessage), "%s", curlang[LANGLANGIMPORTERR2]);
      message_error(tempmessage);
      print_gui_error_message(error_message, curlang[LANGERROR], 1);
      clear_error_message();
    }

    fclose(readfile);
  }
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





int translate_all(void)
{
  unsigned int total_language_items = n_languages;
  int translate_failed = 0;
  if (translate >= 80000 && translate < 80010)
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
      snprintf(tempmessage, sizeof(tempmessage), "%s: %s (%s)", curlang[LANGLANGEXPORTERR], export_file, strerror(errno));
      message_error(tempmessage);
      print_gui_error_message(error_message, curlang[LANGERROR], 1);
      clear_error_message();
    }
    fprintf(writefile, "%s", program_title);
    unsigned int i;
    unsigned int n;
    for (i = 0; i < LANGCOUNT; i++)
    {
      fprintf(writefile, "\n|_|%d|_|\n", i);
      fprintf(writefile, "%s", enlang[i]);
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
      fprintf (stdout, "translating %d\n", i/3);
      char *translang = languages[i+1];
      char *language = languages[i];
      const char *native = languages[i+2];
      copy_enlanguage();
      if ( translate_language("en", translang, language, native) )
      {
        fprintf (stdout, "\n*****************************************************\n");
        fprintf (stdout, "Failed to translate to %s\n", language);
        fprintf (stdout, "\n*****************************************************\n");
        translate_failed = 1;
      }
      if (translate == 80009)
      {
        for (n = 0; n < LANGCOUNT; n++)
        {
          strcpy (alllang[i/3][n], newlang[n]);
        }
        copy_newlanguage();
        if (translate_language(translang, "en", language, "english") )
        {
          fprintf (stdout, "\n*****************************************************\n");
          fprintf (stdout, "Failed to reverse translate %s\n", language);
          fprintf (stdout, "\n*****************************************************\n");
          translate_failed = 1;
        }
        for (n = 0; n < LANGCOUNT; n++)
        {
          strcpy (reverselang[i/3][n], newlang[n]);
        }
      }
    }

    if (translate == 80009)
    {
      writefile = fopen(reverse_file, "w");
      if (writefile == NULL)
      {
        snprintf(tempmessage, sizeof(tempmessage), "%s: %s (%s)", curlang[LANGLANGEXPORTERR], reverse_file, strerror(errno));
        message_error(tempmessage);
        print_gui_error_message(error_message, curlang[LANGERROR], 1);
        clear_error_message();
      }
      fprintf(writefile, "\"%s\", ", "English");
      for (i = 0; i < total_language_items; i+=3)
      {
        fprintf(writefile, "\"%s\", ", languages[i]);
      }
      fprintf(writefile, "\n");
      for (n = 0; n < LANGCOUNT; n++)
      {
        fprintf(writefile, "\"%s\", ", enlang[n]);
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
        snprintf(tempmessage, sizeof(tempmessage), "%s: %s (%s)", curlang[LANGLANGEXPORTERR], all_file, strerror(errno));
        message_error(tempmessage);
        print_gui_error_message(error_message, curlang[LANGERROR], 1);
        clear_error_message();
      }
      fprintf(writefile, "\"%s\", ", "English");
      for (i = 0; i < total_language_items; i+=3)
      {
        fprintf(writefile, "\"%s\", ", languages[i]);
      }
      fprintf(writefile, "\n");
      for (n = 0; n < LANGCOUNT; n++)
      {
        fprintf(writefile, "\"%s\", ", enlang[n]);
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





int translate_language(char *fromlang, char *translang, const char *language, const char *native)
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
    snprintf(temp_data, sizeof(temp_data), "\n|_|%04d|_|\n", i);
    strcat(lang_data, temp_data);
    snprintf(temp_data, sizeof(temp_data), "%s", curlang[i]);
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

      strcpy(url_data, "https://translate.googleapis.com/translate_a/single?client=gtx&sl=");
      strcat(url_data, fromlang);
      strcat(url_data, "&tl=");
      strcat(url_data, translang);
      strcat(url_data, "&dt=t&q=");
      strcat(url_data, new_data);

      fprintf (stdout, "%d %s to %s  ", i, fromlang, translang);
      do_nanosleep(TRANSLATETIMERFAST);  // this is a timer to deal with google translator
      const char *data = get_translated_data(url_data);
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
    scanline = sscanf(line, "| _ | %d | _ |", &new_count);
    if (scanline == 1)
    {
      //fprintf (stdout, "num=%d, line=%s\n", new_count, line);
      // remove the last new line from the end
      if (found_count >= 0)
      {
        int l = strlen(newlang[found_count]);
        if (l > 0 && newlang[found_count][l-1] == '\n')
        {
          newlang[found_count][l-1] = '\0';
        }
      }
      found_count = new_count;
      strcpy (newlang[found_count], "");
      count++;
    }
    else if (found_count >= 0 && found_count < LANGCOUNT)
    {
      unsigned int l = strlen(newlang[found_count]);
      strncat (newlang[found_count], line, MAXLANGLENGTH - l - 1);
      strcat (newlang[found_count], "\n");
      //fprintf (stdout, "%s\n", newlang[found_count]);
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
    int retry = translate_language_slow(fromlang, translang, language, native);
    return retry;
  }

  char langfile[256];
  strcpy (langfile, language);
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
    snprintf(tempmessage, sizeof(tempmessage), "%s: %s (%s)", curlang[LANGLANGEXPORTERR], langfile, strerror(errno));
    message_error(tempmessage);
    print_gui_error_message(error_message, curlang[LANGERROR], 1);
    clear_error_message();
  }

  fprintf(writefile, "%s", program_title);
  for (i = 0; i < LANGCOUNT; i++)
  {
    fprintf(writefile, "\n|_|%04d|_|\n", i);
    fprintf(writefile, "%s", newlang[i]);
  }

  fclose(writefile);
  chmod(langfile, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

  return failure;
}





char* get_translated_data(char *url_data)
{
#ifdef DEBUG
#ifndef USE_CURL
  return url_data;
#else
  do_nanosleep(TRANSLATETIMERALL);  // this is a timer to deal with google translator
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





int translate_language_slow(char *fromlang, char *translang, const char *language, const char *native)
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
    snprintf(temp_data, sizeof(temp_data), "%s", curlang[count]);
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

    strcpy(url_data, "https://translate.googleapis.com/translate_a/single?client=gtx&sl=");
    strcat(url_data, fromlang);
    strcat(url_data, "&tl=");
    strcat(url_data, translang);
    strcat(url_data, "&dt=t&q=");
    strcat(url_data, new_data);

    fprintf (stdout, "%d %s to %s  ", count, fromlang, translang);
    do_nanosleep(TRANSLATETIMERSLOW);  // this is a timer to deal with google translator
    const char *data = get_translated_data(url_data);
    //fprintf (stdout, "%s\n", data);
    strcpy (return_data, data);
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
    if (strlen(new_lang_data) >= MAXLANGLENGTH)
    {
      fprintf (stdout, "Warning: language count %d exceeded max length and was truncated.\n", count);
      failure = 1;
    }
    strcpy (newlang[count], new_lang_data);
  }


  char langfile[256];
  strcpy (langfile, language);
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
    snprintf(tempmessage, sizeof(tempmessage), "%s: %s (%s)", curlang[LANGLANGEXPORTERR], langfile, strerror(errno));
    message_error(tempmessage);
    print_gui_error_message(error_message, curlang[LANGERROR], 1);
    clear_error_message();
  }

  fprintf(writefile, "%s", program_title);
  for (int i = 0; i < LANGCOUNT; i++)
  {
    fprintf(writefile, "\n|_|%d|_|\n", i);
    fprintf(writefile, "%s", newlang[i]);
  }

  fclose(writefile);
  chmod(langfile, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

  return failure;
}





int copy_enlanguage(void)
{
  int i;
  for (i = 0; i < LANGCOUNT; i++)
  {
    strcpy (curlang[i], enlang[i]);
  }
  return 0;
}





int copy_newlanguage(void)
{
  int i;
  for (i = 0; i < LANGCOUNT; i++)
  {
    strcpy (curlang[i], newlang[i]);
    //fprintf(stdout, "%d %s", i, newlang[i]);
  }
  return 0;
}






int setup_enlanguage(void)
{
  strcpy (enlang[LANGFILE], "File");
  strcpy (enlang[LANGQUIT], "Quit");
  strcpy (enlang[LANGOPEN], "Open");
  strcpy (enlang[LANGERROR], "Error!");
  strcpy (enlang[LANGWARN], "Warning");
  strcpy (enlang[LANGLANGUAGE], "Language");
  strcpy (enlang[LANGEXPORTLANG], "Export language");
  strcpy (enlang[LANGIMPORTLANG], "Import language");
  strcpy (enlang[LANGLANGEXPORTERR], "Cannot open file for writing");
  strcpy (enlang[LANGLANGIMPORTERR], "Cannot open file for reading");
  strcpy (enlang[LANGLANGIMPORTERR2], "Error processing imported language file");
  strcpy (enlang[LANGLANGCHANGESUCCESS], "Language successfully changed!");
  strcpy (enlang[LANGSUCCESS], "Success!");
  strcpy (enlang[LANGOPTIONS], "Options");
  strcpy (enlang[LANGLEFTRES], "Left panel resolution");
  strcpy (enlang[LANGMAINRES], "Main panel resolution");
  strcpy (enlang[LANGMAINSIZE], "Main grid size");
  strcpy (enlang[LANGAUTOUPDATE], "Auto update");
  strcpy (enlang[LANGSHOWBADHEAD], "Show bad head");
  strcpy (enlang[LANGNONTRIMMED], "Non-trimmed");
  strcpy (enlang[LANGNONDIVIDED], "Non-divided");
  strcpy (enlang[LANGNONSCRAPED], "Non-scraped");
  strcpy (enlang[LANGBAD], "Bad");
  strcpy (enlang[LANGNONTRIED], "Non-tried");
  strcpy (enlang[LANGFINISHED], "Finished");
  strcpy (enlang[LANGCURRENT], "Current");
  strcpy (enlang[LANGBADHEAD], "Bad-head");
  strcpy (enlang[LANG1PT], "1 pt");
  strcpy (enlang[LANG2PT], "2 pt");
  strcpy (enlang[LANG3PT], "3 pt");
  strcpy (enlang[LANG4PT], "4 pt");
  strcpy (enlang[LANG6PT], "6 pt");
  strcpy (enlang[LANG8PT], "8 pt");
  strcpy (enlang[LANG10PT], "10 pt");
  strcpy (enlang[LANG12PT], "12 pt");
  strcpy (enlang[LANG14PT], "14 pt");
  strcpy (enlang[LANG16PT], "16 pt");
  strcpy (enlang[LANG4K], "4K");
  strcpy (enlang[LANG8K], "8K");
  strcpy (enlang[LANG16K], "16K");
  strcpy (enlang[LANG32K], "32K");
  strcpy (enlang[LANG64K], "64K");
  strcpy (enlang[LANG128K], "128K");
  strcpy (enlang[LANG256K], "256K");
  strcpy (enlang[LANG512K], "512K");
  strcpy (enlang[LANG1M], "1M");
  strcpy (enlang[LANGOFF], "Off");
  strcpy (enlang[LANG5SEC], "5 seconds");
  strcpy (enlang[LANG10SEC], "10 seconds");
  strcpy (enlang[LANG30SEC], "30 seconds");
  strcpy (enlang[LANG1MIN], "1 minute");
  strcpy (enlang[LANG2MIN], "2 minutes");
  strcpy (enlang[LANG5MIN], "5 minutes");
  strcpy (enlang[LANGSHOWGOODDATA], "Highlight good data");
  strcpy (enlang[LANGGOODDATA], "Good data");
  strcpy (enlang[LANG2M], "2M");
  strcpy (enlang[LANG4M], "4M");
  strcpy (enlang[LANG8M], "8M");
  strcpy (enlang[LANG16M], "16M");
  strcpy (enlang[LANGSHOWTIMING], "Show high time");
  strcpy (enlang[LANGTIMING], "Timing");
  strcpy (enlang[LANGDOMAIN ], "Domain");
  strcpy (enlang[LANGSHOWDOMAIN], "Show domain");
  strcpy (enlang[LANGDOMAINBLOCKNOTFOUND], "Domain block not found");
  strcpy (enlang[LANGDMDEDOMAIN], "Domain from DMDE bytes file");
  strcpy (enlang[LANGAREAS], "Areas");
  strcpy (enlang[LANGDUMMY], "dummy always put at bottom of language list");
  //strcpy (enlang[], "");

return 0;
}





