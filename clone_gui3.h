// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


#include <gtk/gtk.h>
#include "conditional.h"
#ifdef DEBUG
#include <curl/curl.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hddsuperclone_EULA.h"


#define TRANSLATETIMERALL  1000000000    // nanosecond delay for every translate call
#define TRANSLATETIMERFAST 10000000000    // nanosecond delay for each normal translate call
#define TRANSLATETIMERSLOW 2000000000    // nanosecond delay for each slow translate call


#define  GTKCOLOR_BLACK      "#000000"        // (0,0,0)
#define  GTKCOLOR_WHITE      "#FFFFFF"        // (255,255,255)
#define  GTKCOLOR_RED        "#FF0000"        // (255,0,0)
#define  GTKCOLOR_LIME       "#00FF00"        // (0,255,0)
#define  GTKCOLOR_BLUE       "#0000FF"        // (0,0,255)
#define  GTKCOLOR_YELLOW     "#FFFF00"        // (255,255,0)
#define  GTKCOLOR_CYAN       "#00FFFF"        // (0,255,255)
#define  GTKCOLOR_MAGENTA    "#FF00FF"        // (255,0,255)
#define  GTKCOLOR_SILVER     "#C0C0C0"        // (192,192,192)
#define  GTKCOLOR_LIGHTGRAY  "#D3D3D3"        // (211, 211, 211)
#define  GTKCOLOR_DARKGREY   "#A9A9A9"        // (169, 169, 169)
#define  GTKCOLOR_GRAY       "#808080"        // (128,128,128)
#define  GTKCOLOR_MAROON     "#800000"        // (128,0,0)
#define  GTKCOLOR_OLIVE      "#808000"        // (128,128,0)
#define  GTKCOLOR_GREEN      "#008000"        // (0,128,0)
#define  GTKCOLOR_PURPLE     "#800080"        // (128,0,128)
#define  GTKCOLOR_TEAL       "#008080"        // (0,128,128)
#define  GTKCOLOR_NAVY       "#000080"        // (0,0,128)

#define MAX_BUTTON_LABEL_SIZE 1024

#define BUTTONID_PHASE1    1
#define BUTTONID_PHASE2    2
#define BUTTONID_PHASE3    3
#define BUTTONID_PHASE4    4
#define BUTTONID_DIVIDE    5
#define BUTTONID_DIVIDE2   6
#define BUTTONID_TRIM      7
#define BUTTONID_SCRAPE    8
#define BUTTONID_MARKBAD   9
#define BUTTONID_READBAD   10
#define BUTTONID_REVERSE   11
#define BUTTONID_SKIPFAST  12
#define BUTTONID_NOLOGBACK 13
#define BUTTONID_FORCE     14
#define BUTTONID_OFFSET    15
#define BUTTONID_FORCEDANGEROUS 16
#define BUTTONID_PIOMODE 17
#define BUTTONID_PHASETIMERS 18
#define BUTTONID_ENABLEPROCESSCHUNK 19
#define BUTTONID_ENABLEPHASELOGS 20
#define BUTTONID_ENABLESCSIWRITE 21
#define BUTTONID_ENABLEOUTPUTSECTOR 22
#define BUTTONID_USEPHYSICALSECTORSIZE 23
#define BUTTONID_COLORSTATUSBAR 24
#define BUTTONID_ENABLEREADTWICE 25
#define BUTTONID_POSITION 26
#define BUTTONID_IOSCSIONLY 27
#define BUTTONID_ENABLEREBUILDASSIST 28
#define BUTTONID_REBUILDASSIST 29



GtkWidget *language_window_ccc;
GtkWidget *main_label;
GtkWidget *main_frame;
GtkWidget *bottom_status_bar_ccc;

GtkWidget *main_window_ccc;
GtkWidget *main_vbox_ccc;
GtkWidget *main_hbox_ccc;

GtkWidget *button_bbk_ccc;
GtkWidget *button_unc_ccc;
GtkWidget *button_mc_ccc;
GtkWidget *button_idnf_ccc;
GtkWidget *button_mcr_ccc;
GtkWidget *button_abrt_ccc;
GtkWidget *button_tk0nf_ccc;
GtkWidget *button_amnf_ccc;
GtkWidget *button_bsy_ccc;
GtkWidget *button_drdy_ccc;
GtkWidget *button_df_ccc;
GtkWidget *button_dsc_ccc;
GtkWidget *button_drq_ccc;
GtkWidget *button_corr_ccc;
GtkWidget *button_idx_ccc;
GtkWidget *button_err_ccc;
GtkWidget *button_status_separator_ccc;

GdkRGBA button_color_ccc;


// file menu
GtkWidget *topmenubar_ccc;
GtkWidget *filemenu_ccc;
GtkWidget *filemi_ccc;
GtkWidget *quitmi_ccc;
GtkWidget *openmi_ccc;
GtkWidget *newmi_ccc;
GtkWidget *sourcemi_ccc;
GtkWidget *destinationmi_ccc;
GtkWidget *drivesmi_ccc;
GtkWidget *blockmi_ccc;
GtkWidget *drivemi_ccc;
GtkWidget *imagemi_ccc;
GtkWidget *devnullmi_ccc;
GtkWidget *filew_ccc;
GtkWidget *modemi_ccc;
GtkWidget *copyimagemi_ccc;
GtkWidget *genericmi_ccc;
GtkWidget *pass_automi_ccc;
GtkWidget *pass_scsimi_ccc;
GtkWidget *pass_atrami_ccc;
GtkWidget *directmi_ccc;
GtkWidget *ahcimi_ccc;
GtkWidget *usbmi_ccc;
GtkWidget *usbatami_ccc;
GtkWidget *settingsmi_ccc;
GtkWidget *advancedmi_ccc;
GtkWidget *clonemi_ccc;
GtkWidget *savemi_ccc;
GtkWidget *saveasmi_ccc;
GtkWidget *resetstatusmi_ccc;
GtkWidget *toolsmi_ccc;
GtkWidget *resetlogmi_ccc;
GtkWidget *repairlogmi_ccc;
GtkWidget *fillzeromi_ccc;
GtkWidget *fillmarkmi_ccc;
GtkWidget *fillerasemi_ccc;
GtkWidget *domainmi_ccc;
GtkWidget *importddmi_ccc;
GtkWidget *exportddmi_ccc;
GtkWidget *cleardomainmi_ccc;
GtkWidget *helpmi_ccc;
GtkWidget *aboutmi_ccc;
GtkWidget *timermi_ccc;
GtkWidget *installdrivermi_ccc;
GtkWidget *uninstalldrivermi_ccc;
GtkWidget *driveronlymi_ccc;
GtkWidget *adddomainmi_ccc;
GtkWidget *newdomainmi_ccc;
GtkWidget *savedomainmi_ccc;
GtkWidget *savedomainasmi_ccc;
GtkWidget *disableportsmi_ccc;
GtkWidget *restoreportsmi_ccc;
GtkWidget *displayidentifymi_ccc;
GtkWidget *displayanalyzemi_ccc;
GtkWidget *displaysmartmi_ccc;
GtkWidget *helphtmlmi_ccc;
GtkWidget *helptextmi_ccc;
GtkWidget *primaryrelaymi_ccc;
GtkWidget *chooseprimaryrelaymi_ccc;
GtkWidget *disableusbmassmi_ccc;
GtkWidget *restoreusbmassmi_ccc;


GtkWidget *choose_source_dialog_window_ccc;
GtkWidget *choose_source_scrolled_window_ccc;
GtkWidget *choose_source_vbox_ccc;

GtkWidget *choose_destination_dialog_window_ccc;
GtkWidget *choose_destination_scrolled_window_ccc;
GtkWidget *choose_destination_vbox_ccc;

GtkWidget *choose_usb_dialog_window_ccc;
GtkWidget *choose_usb_scrolled_window_ccc;
GtkWidget *choose_usb_vbox_ccc;

GtkWidget *connect_button_ccc;
GtkWidget *disconnect_button_ccc;
GtkWidget *start_button_ccc;
GtkWidget *stop_button_ccc;
GtkWidget *analyze_button_ccc;
GtkWidget *analyze_long_button_ccc;
GtkWidget *smart_button_ccc;
GtkWidget *clone_mode_button_ccc;
GtkWidget *agressive_driver_checkbutton_ccc;
GtkWidget *driver_mode1_button_ccc;
GtkWidget *driver_mode2_button_ccc;
GtkWidget *driver_mode3_button_ccc;
GtkWidget *driver_mode4_button_ccc;
GtkWidget *driver_mode5_button_ccc;
GtkWidget *soft_reset_button_ccc;
GtkWidget *hard_reset_button_ccc;

GtkWidget *phase_control_label_ccc;
GtkWidget *phase1_check_button_ccc;
GtkWidget *phase2_check_button_ccc;
GtkWidget *phase3_check_button_ccc;
GtkWidget *phase4_check_button_ccc;
GtkWidget *divide_check_button_ccc;
GtkWidget *divide2_check_button_ccc;
GtkWidget *trim_check_button_ccc;
GtkWidget *scrape_check_button_ccc;
GtkWidget *markbad_check_button_ccc;
GtkWidget *readbad_check_button_ccc;
GtkWidget *rebuild_assist_check_button_ccc;
GtkWidget *reverse_check_button_ccc;
GtkWidget *skipfast_check_button_ccc;
GtkWidget *retries_button_label_ccc;
GtkWidget *retries_spin_button_ccc;
GtkWidget *clustersize_button_label_ccc;
GtkWidget *clustersize_spin_button_ccc;
GtkWidget *inputoffset_button_label_ccc;
GtkWidget *inputoffset_spin_button_ccc;
GtkWidget *size_button_label_ccc;
GtkWidget *size_spin_button_ccc;
GtkWidget *blocksize_button_label_ccc;
GtkWidget *blocksize_spin_button_ccc;
GtkWidget *skipsize_button_label_ccc;
GtkWidget *skipsize_spin_button_ccc;
GtkWidget *maxskipsize_button_label_ccc;
GtkWidget *maxskipsize_spin_button_ccc;
GtkWidget *skipthreshold_button_label_ccc;
GtkWidget *skipthreshold_spin_button_ccc;
GtkWidget *rateskip_button_label_ccc;
GtkWidget *rateskip_spin_button_ccc;
GtkWidget *exitonslow_button_label_ccc;
GtkWidget *exitonslow_spin_button_ccc;
GtkWidget *exitonslowtime_button_label_ccc;
GtkWidget *exitonslowtime_spin_button_ccc;
GtkWidget *no_log_backup_checkbutton_ccc;
GtkWidget *force_mounted_checkbutton_ccc;
GtkWidget *force_dangerous_checkbutton_ccc;
GtkWidget *enable_output_offset_checkbutton_ccc;
GtkWidget *output_offset_spinbutton_ccc;
GtkWidget *output_offset_button_label_ccc;
GtkWidget *enable_current_position_checkbutton_ccc;
GtkWidget *current_position_spinbutton_ccc;
GtkWidget *current_position_button_label_ccc;
GtkWidget *action_on_error_label_ccc;
GtkWidget *stop_on_error_radio_button_ccc;
GtkWidget *call_command_radio_button_ccc;
GtkWidget *command_to_call_text_ccc;
GtkWidget *test_command_button_ccc;
GtkWidget *primary_relay_on_error_radio_button_ccc;
GtkWidget *write_buffer_disable_radio_button_ccc;
GtkWidget *write_buffer_enable_radio_button_ccc;
GtkWidget *write_buffer_label_ccc;
GtkWidget *timer_settings_label_ccc;
GtkWidget *timer_settings_info_label_ccc;
GtkWidget *initial_busy_wait_time_button_label_ccc;
GtkWidget *initial_busy_wait_time_spinbutton_ccc;
GtkWidget *busy_wait_time_button_label_ccc;
GtkWidget *busy_wait_time_spinbutton_ccc;
GtkWidget *soft_reset_time_button_label_ccc;
GtkWidget *soft_reset_time_spinbutton_ccc;
GtkWidget *hard_reset_time_button_label_ccc;
GtkWidget *hard_reset_time_spinbutton_ccc;
GtkWidget *power_cycle_time_button_label_ccc;
GtkWidget *power_cycle_time_spinbutton_ccc;
GtkWidget *general_timout_button_label_ccc;
GtkWidget *general_timeout_spinbutton_ccc;
GtkWidget *action_power_cycle_label_ccc;
GtkWidget *stop_on_power_cycle_radio_button_ccc;
GtkWidget *call_power_command_radio_button_ccc;
GtkWidget *power_command_to_call_text_ccc;
GtkWidget *primary_relay_power_cycle_radio_button_ccc;
GtkWidget *test_power_command_button_ccc;
GtkWidget *pio_mode_checkbutton_ccc;
GtkWidget *phase_timers_label_ccc;
GtkWidget *phase_timers_checkbutton_ccc;
GtkWidget *p12_soft_reset_time_button_label_ccc;
GtkWidget *p12_soft_reset_time_spinbutton_ccc;
GtkWidget *p3_soft_reset_time_button_label_ccc;
GtkWidget *p3_soft_reset_time_spinbutton_ccc;
GtkWidget *p4_soft_reset_time_button_label_ccc;
GtkWidget *p4_soft_reset_time_spinbutton_ccc;
GtkWidget *td_soft_reset_time_button_label_ccc;
GtkWidget *td_soft_reset_time_spinbutton_ccc;
GtkWidget *d2_soft_reset_time_button_label_ccc;
GtkWidget *d2_soft_reset_time_spinbutton_ccc;
GtkWidget *sc_soft_reset_time_button_label_ccc;
GtkWidget *sc_soft_reset_time_spinbutton_ccc;
GtkWidget *rt_soft_reset_time_button_label_ccc;
GtkWidget *rt_soft_reset_time_spinbutton_ccc;
GtkWidget *always_wait_for_reset_timers_checkbutton_ccc;
GtkWidget *usb_resets_label_ccc;
GtkWidget *enable_usb_bulk_reset_checkbutton_ccc;
GtkWidget *enable_usb_soft_reset_checkbutton_ccc;
GtkWidget *enable_usb_hard_reset_checkbutton_ccc;
GtkWidget *enable_usb_port_reset_checkbutton_ccc;
GtkWidget *sectorsize_button_label_ccc;
GtkWidget *sectorsize_spinbutton_ccc;
GtkWidget *alignment_button_label_ccc;
GtkWidget *alignment_spinbutton_ccc;
GtkWidget *enable_rebuild_assist_checkbutton_ccc;
GtkWidget *enable_process_chunk_checkbutton_ccc;
GtkWidget *enable_read_twice_checkbutton_ccc;
GtkWidget *enable_phase_log_checkbutton_ccc;
GtkWidget *confirmation_box_label_ccc;
GtkWidget *disable_ports_label_ccc;
GtkWidget *disable_ports_info_label_ccc;
GtkWidget *current_ports_label_ccc;
GtkWidget *current_ports_info_label_ccc;
GtkWidget *set_disabled_ports_label_ccc;
GtkWidget *set_disabled_ports_text_ccc;
GtkWidget *updated_ports_label_ccc;
GtkWidget *updated_ports_info_label_ccc;
GtkWidget *update_ports_button_ccc;
GtkWidget *select_lun_label_ccc;
GtkWidget *select_lun_info_label_ccc;
GtkWidget *lun_values_label_ccc;
GtkWidget *lun_values_info_label_ccc;
GtkWidget *set_current_lun_label_ccc;
GtkWidget *current_lun_spinbutton_ccc;
GtkWidget *enable_scsi_write_checkbutton_ccc;
GtkWidget *enable_output_sector_size_checkbutton_ccc;
GtkWidget *output_sector_size_button_label_ccc;
GtkWidget *driver_io_scsi_only_checkbutton_ccc;
GtkWidget *use_physical_sector_size_for_virtual_checkbutton_ccc;
GtkWidget *output_sector_size_spinbutton_ccc;
GtkWidget *driver_error_options_label_ccc;
GtkWidget *driver_return_error_radio_button_ccc;
GtkWidget *driver_return_zeros_radio_button_ccc;
GtkWidget *driver_return_marked_radio_button_ccc;
GtkWidget *virtual_disk_device_name_label_ccc;
GtkWidget *virtual_disk_device_name_text_ccc;
GtkWidget *driver_minimum_cluster_size_button_label_ccc;
GtkWidget *driver_minimum_cluster_size_spinbutton_ccc;
GtkWidget *enable_extended_analyze_checkbutton_ccc;
GtkWidget *use_color_statusbar_checkbutton_ccc;
GtkWidget *primary_relay_settings_label_ccc;
GtkWidget *primary_relay_settings_info_label_ccc;
GtkWidget *label_current_primary_relay_board_ccc;
GtkWidget *data_current_relay_board_a_ccc;
GtkWidget *primary_relay_board_settings_label_ccc;
GtkWidget *primary_relay_activation_label_ccc;
GtkWidget *activate_primary_relay1_checkbutton_ccc;
GtkWidget *activate_primary_relay2_checkbutton_ccc;
GtkWidget *activate_primary_relay3_checkbutton_ccc;
GtkWidget *activate_primary_relay4_checkbutton_ccc;
GtkWidget *activate_primary_relay5_checkbutton_ccc;
GtkWidget *activate_primary_relay6_checkbutton_ccc;
GtkWidget *activate_primary_relay7_checkbutton_ccc;
GtkWidget *activate_primary_relay8_checkbutton_ccc;
GtkWidget *primary_relay_deactivation_label_ccc;
GtkWidget *deactivate_primary_relay1_checkbutton_ccc;
GtkWidget *deactivate_primary_relay2_checkbutton_ccc;
GtkWidget *deactivate_primary_relay3_checkbutton_ccc;
GtkWidget *deactivate_primary_relay4_checkbutton_ccc;
GtkWidget *deactivate_primary_relay5_checkbutton_ccc;
GtkWidget *deactivate_primary_relay6_checkbutton_ccc;
GtkWidget *deactivate_primary_relay7_checkbutton_ccc;
GtkWidget *deactivate_primary_relay8_checkbutton_ccc;
GtkWidget *primary_relay_activate_time_label_ccc;
GtkWidget *primary_relay_energize_time_spinbutton_ccc;
GtkWidget *primary_relay_delay_time_label_ccc;
GtkWidget *primary_relay_delay_time_spinbutton_ccc;
GtkWidget *test_primary_relay_button_ccc;
GtkWidget *activate_primary_relay_button_ccc;
GtkWidget *deactivate_primary_relay_button_ccc;


GtkWidget *label_project_ccc;
GtkWidget *data_project_ccc;
GtkWidget *label_domain_ccc;
GtkWidget *data_domain_ccc;
GtkWidget *label_source_ccc;
GtkWidget *data_source_ccc;
GtkWidget *label_destination_ccc;
GtkWidget *data_destination_ccc;
GtkWidget *label_totallba_ccc;
GtkWidget *data_totallba_ccc;
GtkWidget *label_lbatoread_ccc;
GtkWidget *data_lbatoread_ccc;
GtkWidget *label_domainsize_ccc;
GtkWidget *data_domainsize_ccc;
GtkWidget *label_runtime_ccc;
GtkWidget *data_runtime_ccc;
GtkWidget *label_remainingtime_ccc;
GtkWidget *data_remainingtime_ccc;
GtkWidget *label_currentposition_ccc;
GtkWidget *data_currentposition_ccc;
GtkWidget *label_currentstatus_ccc;
GtkWidget *data_currentstatus_ccc;
GtkWidget *label_currentrate_ccc;
GtkWidget *data_currentrate_ccc;
GtkWidget *label_recentrate_ccc;
GtkWidget *data_recentrate_ccc;
GtkWidget *label_totalrate_ccc;
GtkWidget *data_totalrate_ccc;
GtkWidget *label_skipsize_ccc;
GtkWidget *data_skipsize_ccc;
GtkWidget *label_slowskips_ccc;
GtkWidget *data_slowskips_ccc;
GtkWidget *label_skips_ccc;
GtkWidget *data_skips_ccc;
GtkWidget *label_skipruns_ccc;
GtkWidget *data_skipruns_ccc;
GtkWidget *label_skipresets_ccc;
GtkWidget *data_skipresets_ccc;
GtkWidget *label_runsize_ccc;
GtkWidget *data_runsize_ccc;
GtkWidget *label_finished_ccc;
GtkWidget *data_finished_ccc;
GtkWidget *label_nontried_ccc;
GtkWidget *data_nontried_ccc;
GtkWidget *label_nontrimmed_ccc;
GtkWidget *data_nontrimmed_ccc;
GtkWidget *label_nondivided_ccc;
GtkWidget *data_nondivided_ccc;
GtkWidget *label_nonscraped_ccc;
GtkWidget *data_nonscraped_ccc;
GtkWidget *label_bad_ccc;
GtkWidget *data_bad_ccc;
GtkWidget *label_datapreview_ccc;
GtkWidget *data_datapreview_ccc;
GtkWidget *label_longestreadtime_ccc;
GtkWidget *data_longestreadtime_ccc;
GtkWidget *label_filled_ccc;
GtkWidget *data_filled_ccc;
GtkWidget *label_retried_ccc;
GtkWidget *data_retried_ccc;
GtkWidget *label_retrypasses_ccc;
GtkWidget *data_retrypasses_ccc;
GtkWidget *label_drivermode_ccc;
GtkWidget *data_drivermode_ccc;
//GtkWidget *label__ccc;
//GtkWidget *data__ccc;


GtkAdjustment *gtk_adjustment_ccc;

gint display_status_timer_ccc = 0;
int display_status_timer_running_ccc = 0;

char program_title[256];
int source_chosen_ccc = 0;
int destination_chosen_ccc = 0;
int destination_size_valid_ccc = 0;
int project_chosen_ccc = 0;
int connected_ccc = 0;
int running_ccc = 0;
int new_project_ccc = 0;
int color_switch_flag_ccc = 0;
bool destination_is_generic_ccc = false;
int disabled_ports_changed_ccc = 0;
int usbr1_chosen_ccc = 0;



void select_file_ccc(void);

void select_ddrescue_file_ccc(void);

void export_ddrescue_file_ccc(void);

void new_file_ccc(void);

void save_file_ccc(void);

void save_file_as_ccc(void);

void save_domain_ccc(void);

void save_domain_as_ccc(void);

static void new_domain_file_ccc( char *log_file );

static void load_log_file_ccc( char *log_file );

static void add_domain_file_ccc( char *log_file );

static void load_domain_file_ccc( char *log_file );

static void new_log_file_ccc( char *log_file );

static void load_ddrescue_log_file_ccc( char *log_file );

void export_language_file_ccc(void);

static void file_export_sel_ccc( char *export_file );

void import_language_file_ccc(void);

static void file_import_sel_ccc( char *import_file );

int set_language_ccc(void);

void choose_source_ccc(void);

void get_source_from_button_ccc (GtkWidget *widget, gpointer data);

void choose_destination_ccc(void);

void get_destination_from_button_ccc (const GtkWidget *widget, gpointer data);

void choose_image_ccc (void);

void choose_null_ccc (void);

void set_connected_ccc (void);

void set_disconnected_ccc (void);

void clear_destination_ccc (void);

void set_stop_signal_ccc (void);

void start_cloning_ccc (void);

void quit_and_close_ccc (void);

void open_clone_settings_dialog_ccc (void);

void open_advanced_settings_dialog_ccc (void);

void open_timer_settings_dialog_ccc (void);

void activate_status_buttons_ccc (void);

void deactivate_status_buttons_ccc (void);

void set_mode_ccc (void);

void update_status_buttons_ccc (void);

void set_state_from_button_ccc (GtkWidget *widget, gpointer data);

void do_reset_status_ccc(void);

void do_reset_log_status_ccc(void);

void do_repair_log_ccc(void);

void about_ccc (void);

void start_display_status_update_timer_ccc (void);

gint display_status_update_action_ccc (gpointer data);

void set_driver_mode_button_status_ccc (bool active);

void check_driver_null_condition_ccc (void);

void remove_activation_ccc (void);

void update_ports_ccc (void);

void start_analyzing_ccc (void);

void display_analyze_results_ccc(void);

void get_smart_data_ccc(void);

void display_smart_data_ccc(void);

void display_identify_data_ccc(void);

void choose_primary_usb_ccc(void);

void get_usb_from_button_ccc (const GtkWidget *w, gpointer data);

void clear_usbr1_ccc (void);

void clear_usbd1_ccc (void);

void map_heads_ccc (void);

int set_data_dump_filename_ccc(void);

void get_data_dump_filename_ccc(void);

void analyze_quick_ccc (void);

void analyze_long_ccc (void);



struct MemoryStruct {
  char *memory;
  size_t size;
};




char * languages_ccc[] = {
//    "Afrikaans","af","Afrikaans",
//    "Albanian","sq","shqiptar",
//    "Amharic","am","አማርኛ",
    "Arabic","ar","عربى",
//    "Armenian","hy","հայերեն",
//    "Azeerbaijani","az","Azeerbaijani",
//    "Basque","eu","Euskal",
//    "Belarusian","be","беларускі",
//    "Bengali","bn","বাঙালি",
//    "Bosnian","bs","bosanski",
//    "Bulgarian","bg","български",
//    "Catalan","ca","català",
//    "Cebuano","ceb","Cebuano",
//    "Chichewa","ny","Chichewa",
    "Chinese (Simplified)","zh-CN","简体中文）",
//    "Chinese (Traditional)","zh-TW","中國傳統的）",
//    "Corsican","co","Corsu",
//    "Croatian","hr","hrvatski",
//    "Czech","cs","čeština",
//    "Danish","da","dansk",
//    "Dutch","nl","Nederlands",
//    "Esperanto","eo","Esperanto",
//    "Estonian","et","eesti",
//    "Filipino","tl","Pilipino",
//    "Finnish","fi","Suomalainen",
    "French","fr","français",
//    "Frisian","fy","Frysk",
//    "Galician","gl","galego",
//    "Georgian","ka","ქართული",
    "German","de","Deutsche",
//    "Greek","el","Ελληνικά",
//    "Gujarati","gu","ગુજરાતી",
//    "Haitian Creole","ht","kreyòl ayisyen",
//    "Hausa","ha","Hausa",
//    "Hawaiian","haw","ʻŌlelo Hawaiʻi",
//    "Hebrew","iw","עִברִית",
    "Hindi","hi","हिंदी",
//    "Hmong","hmn","Hmong",
//    "Hungarian","hu","Magyar",
//    "Icelandic","is","icelandic",
//    "Igbo","ig","Igbo",
    "Indonesian","id","bahasa Indonesia",
//    "Irish","ga","Gaeilge",
    "Italian","it","italiano",
    "Japanese","ja","日本語",
//    "Javanese","jw","Jawa",
//    "Kannada","kn","ಕನ್ನಡ",
//    "Kazakh","kk","Қазақ",
//    "Khmer","km","ភាសាខ្មែរ",
//    "Korean","ko","한국어",
//    "Kurdish","ku","Kurdî",
//    "Kyrgyz","ky","Кыргызча",
//    "Lao","lo","ລາວ",
//    "Latin","la","Latine",
//    "Latvian","lv","Latvijas",
//    "Lithuanian","lt","Lietuvos",
//    "Luxembourgish","lb","lëtzebuergesch",
//    "Macedonian","mk","Македонски",
//    "Malagasy","mg","Malagasy",
//    "Malay","ms","Malay",
//    "Malayalam","ml","മലയാളം",
//    "Maltese","mt","Malti",
//    "Maori","mi","Maori",
//    "Marathi","mr","मराठी",
//    "Mongolian","mn","Монгол",
//    "Burmese","my","မြန်မာ",
//    "Nepali","ne","नेपाली",
//    "Norwegian","no","norsk",
//    "Pashto","ps","پښتو",
//    "Persian","fa","فارسی",
//    "Polish","pl","Polskie",
    "Portuguese","pt","Português",
//    "Punjabi","pa","ਪੰਜਾਬੀ",
//    "Romanian","ro","Română",
    "Russian","ru","русский",
//    "Samoan","sm","Samoa",
//    "Scots Gaelic","gd","Gàidhlig",
//    "Serbian","sr","Српски",
//    "Sesotho","st","Sesotho",
//    "Shona","sn","Shona",
//    "Sindhi","sd","سنڌي",
//    "Sinhala","si","සිංහල",
//    "Slovak","sk","slovenský",
//    "Slovenian","sl","slovenski",
//    "Somali","so","Soomaali",
    "Spanish","es","Español",
//    "Sundanese","su","Sunda",
//    "Swahili","sw","Kiswahili",
//    "Swedish","sv","svenska",
//    "Tajik","tg","Тоҷикистон",
//    "Tamil","ta","தமிழ்",
//    "Telugu","te","తెలుగు",
//    "Thai","th","ไทย",
//    "Turkish","tr","Türk",
//    "Ukrainian","uk","український",
//    "Urdu","ur","اردو",
//    "Uzbek","uz","O'zbekiston",
//    "Vietnamese","vi","Tiếng Việt",
//    "Welsh","cy","Cymraeg",
//    "Xhosa","xh","isiXhosa",
//    "Yiddish","yi","ייִדיש",
//    "Yoruba","yo","yorùbá",
//    "Zulu","zu","Zulu"
};

#define n_languages_ccc (sizeof (languages_ccc) / sizeof (const char *))



