// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.



#define LISTEN_HTTPS

#define SEND_HTTPS

#define CHECK_VERIFIED_REPLY 1

#define CHECK_RECIEVER_EMAIL 1

#define CHECK_DUPLICATE_ID 1

//#define SIMULATION
//#define OFFLINE_SIMULATION
//#define OFFLINE_RECEIVE_FILE "live_data"
//#define OFFLINE_REPLY_FILE "live_reply"
#define USE_GMAIL

#define ENABLE_REPLY

#define MESSAGE_SIZE 1024*64

#define LINE_LENGTH 256

#define DATA_LINE_LENGTH 4096

#define DELAY_TIME 200000000    // 200ms

#define RAW_CSV_FILE_NAME "license_files/raw.csv"
#define SHORT_CSV_FILE_NAME "license_files/short.csv"

#define MAX_DMDE_COUPONS 1000
#define DMDE_15USD_NAME "license_files/DMDE/DMDE-coupons-15USD.txt"
#define DMDE_50PERC_NAME "license_files/DMDE/DMDE-coupons-50-perc.txt"
#define DMDE_COUPONS_ENABLED 1

#define SERVERP_STOP_COMMAND "hddscstopserver"
#define SERVERP_REPORT_COMMAND "hddscreportserver"
#define SERVERP_ARCHIVE_COMMAND "hddscarchiveserver"
#define SERVERP_CHECK_COMMAND "hddsccheckserver"



typedef struct
{
  char http[LINE_LENGTH];
  char status_code[LINE_LENGTH];
  char status_text[LINE_LENGTH];
  char content_length[LINE_LENGTH];
  char content_type[LINE_LENGTH];
  char connection[LINE_LENGTH];
  char body[MESSAGE_SIZE];
} reply_items_type;
extern reply_items_type reply_items;


typedef struct
{
  char command[LINE_LENGTH];
  char path[LINE_LENGTH];
  char http[LINE_LENGTH];
  char accept[LINE_LENGTH];
  char user_agent[LINE_LENGTH];
  char content_length[LINE_LENGTH];
  char content_type[LINE_LENGTH];
  char host[LINE_LENGTH];
  char connection[LINE_LENGTH];
  char body[MESSAGE_SIZE];
} message_items_type;
extern message_items_type message_items;



typedef struct
{
  char receiver_email[LINE_LENGTH];
  char receiver_id[LINE_LENGTH];
  char residence_country[LINE_LENGTH];
  char test_ipn[LINE_LENGTH];
  char transaction_subject[LINE_LENGTH];
  char txn_id[LINE_LENGTH];
  char txn_type[LINE_LENGTH];
  char payer_email[LINE_LENGTH];
  char payer_id[LINE_LENGTH];
  char payer_status[LINE_LENGTH];
  char first_name[LINE_LENGTH];
  char last_name[LINE_LENGTH];
  char address_city[LINE_LENGTH];
  char address_country[LINE_LENGTH];
  char address_state[LINE_LENGTH];
  char address_status[LINE_LENGTH];
  char address_country_code[LINE_LENGTH];
  char address_name[LINE_LENGTH];
  char address_street[LINE_LENGTH];
  char address_zip[LINE_LENGTH];
  char custom[LINE_LENGTH];
  char handling_amount[LINE_LENGTH];
  char item_name[LINE_LENGTH];
  char item_number[LINE_LENGTH];
  char mc_currency[LINE_LENGTH];
  char mc_fee[LINE_LENGTH];
  char mc_gross[LINE_LENGTH];
  char payment_date[LINE_LENGTH];
  char payment_fee[LINE_LENGTH];
  char payment_gross[LINE_LENGTH];
  char payment_status[LINE_LENGTH];
  char payment_type[LINE_LENGTH];
  char protection_eligibility[LINE_LENGTH];
  char quantity[LINE_LENGTH];
  char shipping[LINE_LENGTH];
  char tax[LINE_LENGTH];
  char notify_version[LINE_LENGTH];
  char charset[LINE_LENGTH];
  char verify_sign[LINE_LENGTH];
  char business[LINE_LENGTH];
  char shipping_discount[LINE_LENGTH];
  char insurance_amount[LINE_LENGTH];
  char discount[LINE_LENGTH];
  char shipping_method[LINE_LENGTH];
  char ipn_track_id[LINE_LENGTH];
  char dmde_coupon_code[LINE_LENGTH];
} body_items_type;
extern body_items_type body_items;


extern char csv_raw_data[MESSAGE_SIZE];
extern char csv_short_data[MESSAGE_SIZE];
extern uint64_t server_license_id;
extern char license_file[4096];
extern char license_name[64];
extern int manual_create;
 
int start_serverp(void);

int server_reply(const char *message);

int process_message(const char *message);

int process_reply(const char *message);

int process_body(const char *body);

int append_data_to_file(char *filename, char *data);

void set_raw_csv_data(char *license, int version, int type, int days, char *expire_date);

void set_short_csv_data(char *license, int version, int type, int days, char *expire_date);

int send_report_email(char *subject, char *body, char *attachment_name, char *attachment_body, int body_size);

int process_payment(void);

int do_make_license_file (int version, int type, int days);

char *get_dmde_coupon(char *name);

int send_customer_email(char *name, char *email, char *subject, char *body, char *attachment_name, char *attachment_body);

int process_manual_file(char *full_name);

int check_duplicate_id(void);

int import_transaction_files(void);

int send_serverp_command(char *command);

int report_serverp(void);

int archive_serverp(void);











