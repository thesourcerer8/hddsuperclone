// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


// This was taken from:
// --------------------
// https://aticleworld.com/ssl-server-client-using-openssl-in-c/
// gcc -Wall -o server server.c -L/usr/lib -lssl -lcrypto
// sudo ./server <portnum>

// this MIGHT be based off from:
// https://wiki.openssl.org/index.php/Simple_TLS_Server
//
// If you want to get a real certificate, go through these instructions.
//   https://certbot.eff.org/docs/intro.html
// Note,  this is much easier to do if you have Apache running,  and
// you need a dynamic name service such as: https://www.dynu.com

#include "common.h"


#ifdef GODMODE


#include "server.h"
#include "smtp.h"

#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include <netdb.h>

#include "openssl/ssl.h"
#include "openssl/err.h"

#define FAIL    -1

#define LOCAL_ABORT()                              \
do                                                 \
{                                                  \
  printf ("Abort at %s:%d\n", __FILE__, __LINE__); \
  return 1;                                        \
} while (0)

#define LOCAL_REPORT()                             \
do                                                 \
{                                                  \
  printf ("At %s:%d\n", __FILE__, __LINE__);       \
} while (0)

// 7z pass = makeyourownpassword
// command = 7z a license_files.7z license_files -t7z -pmakeyourownpassword

#define PATH "/cgi-bin/webscr"
#ifdef SIMULATION
#define HOSTNAME "ipnpb.sandbox.paypal.com"
#else
#define HOSTNAME "ipnpb.paypal.com"
#endif

#define MY_HOST_NAME "make-your-own-hostname.ddnsgeek.com"
#define MY_HOST_PATH "/make-your-own-hostpath"

#define HDDTEST_ITEM_NAME "HDD Test"
#define HDDSHORT_ITEM_NAME "HDDSuperClone 60 Day License"
#define HDDSHORTSPECIAL_ITEM_NAME "HDDSuperClone 60 Day License Special Price"
#define HDDLONG_ITEM_NAME "HDDSuperClone Long Term License"
#define HDDLONGSPECIAL_ITEM_NAME "HDDSuperClone Long Term License Special Price"

#define HDDTEST_PRICE 1
#define HDDSHORT_PRICE 20
#define HDDLONG_PRICE 200

#define RECEIVER_EMAIL "your-email@gmail.com"

#ifdef USE_GMAIL
#define REPORT_EMAIL_USER "your-email@gmail.com"
#define REPORT_EMAIL_PASS "your-password"    // this is an app password
#define REPORT_EMAIL_FROM "your-email@gmail.com"
#define REPORT_EMAIL_FROM_NAME "your-name"
#define REPORT_EMAIL_TO "your-email@gmail.com"
#define REPORT_EMAIL_TO_NAME "your-name"
#define REPORT_EMAIL_SERVER "smtp.gmail.com"
#define REPORT_EMAIL_PORT "465"
#define REPORT_MAIL_SECURITY SMTP_SECURITY_TLS
#define REPORT_MAIL_FLAGS SMTP_NO_CERT_VERIFY /* Do not verify cert. */
#define REPORT_MAIL_AUTH SMTP_AUTH_PLAIN
#else
#define REPORT_EMAIL_USER "postmaster@get-your-own-account.mailgun.org"
#define REPORT_EMAIL_PASS "get-your-own-password"
#define REPORT_EMAIL_FROM "your-email@gmail.com"
#define REPORT_EMAIL_FROM_NAME "your-name"
#define REPORT_EMAIL_TO "your-email@gmail.com"
#define REPORT_EMAIL_TO_NAME "your-name"
#define REPORT_EMAIL_SERVER "smtp.mailgun.org"
#define REPORT_EMAIL_PORT "587"
#define REPORT_MAIL_SECURITY SMTP_SECURITY_NONE
#define REPORT_MAIL_FLAGS SMTP_NO_CERT_VERIFY /* Do not verify cert. */
#define REPORT_MAIL_AUTH SMTP_AUTH_PLAIN
#endif


reply_items_type reply_items;
message_items_type message_items;
body_items_type body_items;


char csv_raw_data[MESSAGE_SIZE];
char csv_short_data[MESSAGE_SIZE];
uint64_t server_license_id;
char license_file[4096];
char license_name[64];
int manual_create;






#ifdef LISTEN_HTTPS
void instructionsForPem (void)
{
  printf ("\n");
  printf ("\n");
  printf ("Did you forget to create your mycert.pem file?\n");
  printf ("\n");
  printf ("Run: openssl req -x509 -nodes -days 365 -newkey rsa:1024 -keyout mycert.pem -out mycert.pem\n");
  printf ("\n");
  printf ("If you haven't, but that's my best guess of what has gone wrong..\n");
  printf ("\n");
  printf ("\n");
}
#endif

int start_serverp (void)
{
  manual_create = 0;
  int server;
  int portnum;
#ifdef LISTEN_HTTPS
  const char *szPemPublic = "mycert.pem";
  const char *szPemPrivate = "mycert.pem";
  const SSL_METHOD *method;
  SSL_CTX *ctx;
  portnum = 443;
#else
  portnum = 80;
#endif

  if (portnum < 1024)
  {
    if (getuid () != 0)
    {
      printf ("This program must be run as root/sudo user since your port # (%d) is < 1024\n", portnum);
      exit (1);
    }
  }

#ifdef LISTEN_HTTPS
  SSL_library_init ();               /* Initialize the SSL library */
  // InitServerCTX ();
  OpenSSL_add_all_algorithms ();     /* load & register all cryptos, etc. */
  SSL_load_error_strings ();         /* load all error messages */
  method = TLSv1_2_server_method (); /* create new server-method instance */
  ctx = SSL_CTX_new (method);        /* create new context from method */
  if (ctx == NULL)
  {
    ERR_print_errors_fp (stderr);
    LOCAL_ABORT ();
  }
  /* set the local certificate from CertFile */
  if (SSL_CTX_use_certificate_file (ctx, szPemPublic, SSL_FILETYPE_PEM) <= 0)
  {
    ERR_print_errors_fp (stderr);
    instructionsForPem ();
    LOCAL_ABORT ();
  }
  /* set the private key from KeyFile (may be the same as CertFile) */
  if (SSL_CTX_use_PrivateKey_file (ctx, szPemPrivate, SSL_FILETYPE_PEM) <= 0)
  {
    ERR_print_errors_fp (stderr);
    instructionsForPem ();
    LOCAL_ABORT ();
  }
  /* verify private key */
  if (!SSL_CTX_check_private_key (ctx))
  {
    fprintf (stderr, "Private key does not match the public certificate\n");
    LOCAL_ABORT ();
  }
  /* set the cipher to something that always works for TLSv1.2 */
  // this was for testing an issue with paypal that did not help
  /*
  if (1 && SSL_CTX_set_cipher_list (ctx, "AES256-SHA256") <= 0)
  {
    ERR_print_errors_fp (stderr);
    instructionsForPem ();
    LOCAL_ABORT ();
  }
  */
#endif

  struct sockaddr_in addr;

  server = socket (PF_INET, SOCK_STREAM, 0);
  bzero (&addr, sizeof (addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons (portnum);
  addr.sin_addr.s_addr = INADDR_ANY;
  if (bind (server, (struct sockaddr*)&addr, sizeof (addr)) != 0)
  {
    perror ("can't bind port");
    LOCAL_ABORT ();
  }
  if (listen (server, 10) != 0)
  {
    perror ("Can't configure listening port");
    LOCAL_ABORT ();
  }

  //critical_process_ccc = true;

  archive_serverp();
  system("touch server-running");

  while (1)
  {
    char buf[MESSAGE_SIZE] = {0};
    memset(buf, 0, MESSAGE_SIZE);
    int bytes;
    int status = 0;

    // set the response
    const char *szHttpServerResponse = "HTTP/1.1 200 OK\r\n\r\n";

#ifndef OFFLINE_SIMULATION

    struct sockaddr_in addr;
    socklen_t len = sizeof (addr);
    int client;
    get_the_time_ccc();
    printf ("******************************************************************\n");
    printf ("******************************************************************\n");
    printf ("%s\n", current_date_ccc);
    printf ("Listening...\n");
    client = accept (server, (struct sockaddr*)&addr, &len);  /* accept connection as usual */
    if (client < 0)
    {
      perror ("Accept error");
      LOCAL_ABORT ();
    }
    get_the_time_ccc();
    printf ("%s\n", current_date_ccc);
    printf ("Connection: %s:%d\n", inet_ntoa (addr.sin_addr), ntohs (addr.sin_port));
#ifdef LISTEN_HTTPS
    int sd;
    SSL *ssl;
    ssl = SSL_new (ctx);           /* get new SSL state with context */
    SSL_set_fd (ssl, client);      /* set connection socket to SSL state */
    if (SSL_accept (ssl) == FAIL)  /* do SSL-protocol accept */
    {
      ERR_print_errors_fp (stderr);
      sd = SSL_get_fd (ssl); /* get socket connection */
      SSL_free (ssl);        /* release SSL state */
      close (sd);            /* close connection */
      continue;
    }
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate (ssl); /* Get certificates (if available) */
    if (cert != NULL)
    {
      printf ("Server certificates:\n");
      line = X509_NAME_oneline (X509_get_subject_name (cert), 0, 0);
      printf ("Subject: %s\n", line);
      free (line);
      line = X509_NAME_oneline (X509_get_issuer_name (cert), 0, 0);
      printf ("Issuer: %s\n", line);
      free (line);
      X509_free (cert);
    }
    else
    {
      printf ("No certificates.\n");
    }
    //ShowCerts (ssl);                         /* get any certificates */
    do_nanosleep_ccc(DELAY_TIME);    // to let the buffer get filled by more packets.
    bytes = SSL_read (ssl, buf, sizeof (buf)); /* get request */
    printf ("bytes1=%d\n", bytes);
    // find content size if any
    int csize;
    int content_length = 0;
    if (bytes > 0)
    {
      for (csize = 0; csize < bytes; csize++)
      {
        char command[MESSAGE_SIZE];
        char data[MESSAGE_SIZE];
        char leftover[MESSAGE_SIZE];
        memset (command, 0, MESSAGE_SIZE);
        memset (data, 0, MESSAGE_SIZE);
        sscanf(buf+csize, "%s %s %[^\n]", command, data, leftover);
        if (strcasecmp(command, "Content-Length:") == 0)
        {
          char* endptr;
          content_length = strtoll(data, &endptr, 0);
          break;
        }
      }
    }
    fprintf (stdout, "content-length=%d\n", content_length);
    // find end of header
    int hend = 0;
    if (content_length > 0)
    {
      for (csize = 0; csize < bytes; csize++)
      {
        char c;
        memcpy (&c, buf+csize+0, 1);
        if (c == '\r')
        {
          memcpy (&c, buf+csize+1, 1);
          if (c == '\n')
          {
            memcpy (&c, buf+csize+2, 1);
            if (c == '\r')
            {
              memcpy (&c, buf+csize+3, 1);
              if (c == '\n')
              {
                hend = csize+3;
                break;
              }
            }
          }
        }
      }
    }
    fprintf (stdout, "header-end=%d\n", hend);
    if ( (hend + content_length) > bytes)
    {
      char tempbuf[MESSAGE_SIZE] = {0};
      memset(tempbuf, 0, MESSAGE_SIZE);
      do_nanosleep_ccc(DELAY_TIME);    // to let the buffer get filled by more packets.
      int bytes2 = SSL_read (ssl, tempbuf, sizeof (tempbuf)); /* get request */
      printf ("bytes2=%d\n", bytes2);
      if (bytes2 > 0)
      {
        memcpy(buf+bytes, tempbuf, bytes2);
        bytes = bytes + bytes2;
      }
    }
#else
    bytes = read (client, buf, sizeof (buf));
#endif
    printf ("bytes read = %d\n", bytes);
#else    // offline simulation
    FILE *readfile = fopen(OFFLINE_RECEIVE_FILE, "rb");
    if (readfile == NULL)
    {
      printf ("No offline receive file.\n");
      return 0;
    }
    bytes = fread(buf, 1, sizeof (buf), readfile);
    fclose(readfile);
#endif
    // buf[bytes] = '\0';    // since bytes can be less that what was read we don't do it

    printf ("Client msg:\n[%s]\n", buf);

    if (bytes > 0)
    {
      // check first line for proper message, if not good then don't reply
      int message_length = strlen(buf);
      char line[message_length];
      memset (line, 0, message_length);
      char c;
      int position = 0;
      while (1)
      {
        if (position >= message_length)
        {
          break;
        }
        memcpy (&c, buf+position, 1);
        if (c == '\r')
        {
          position++;
          continue;
        }
        if (c == '\n')
        {
          position++;
          break;
        }
        strncat (line, &c, 1);
        position++;
      }
      char command[message_length];
      char path[message_length];
      char http[message_length];
      char leftover[message_length];
      memset (command, 0, message_length);
      memset (path, 0, message_length);
      memset (http, 0, message_length);
      int ret = sscanf(line, "%s %s %s %[^\n]", command, path, http, leftover);
      if (ret != 3)
      {
        status = status | 0x01;
      }
      if (strcasecmp(command, "POST") != 0)
      {
        status = status | 0x02;
      }
      if (strcasecmp(path, MY_HOST_PATH) != 0)
      {
        status = status | 0x04;
      }
      if (strcmp(command, SERVERP_STOP_COMMAND) == 0 && strcmp(inet_ntoa (addr.sin_addr), "127.0.0.1") == 0)
      {
        status = 0x1000;
      }
      if (strcmp(command, SERVERP_REPORT_COMMAND) == 0 && strcmp(inet_ntoa (addr.sin_addr), "127.0.0.1") == 0)
      {
        status = 0x2000;
      }
      if (strcmp(command, SERVERP_ARCHIVE_COMMAND) == 0 && strcmp(inet_ntoa (addr.sin_addr), "127.0.0.1") == 0)
      {
        status = 0x3000;
      }
      if (strcmp(command, SERVERP_CHECK_COMMAND) == 0 && strcmp(inet_ntoa (addr.sin_addr), "127.0.0.1") == 0)
      {
        status = 0x4000;
      }

      if (status == 0)
      {
        printf ("Reply with:\n[%s]\n", szHttpServerResponse);
#ifndef OFFLINE_SIMULATION
#ifdef LISTEN_HTTPS
        do_nanosleep_ccc(DELAY_TIME);    // trying a delay here to see if it helps
        SSL_write (ssl, szHttpServerResponse, strlen (szHttpServerResponse));
#else
        write (client, szHttpServerResponse, strlen (szHttpServerResponse));
#endif
      }
      else if (status == 0x1000)
      {
        printf ("Reply with:\n[%s]\n", "stopping server");
#ifdef LISTEN_HTTPS
        SSL_write (ssl, "stopping server", strlen (szHttpServerResponse));
#else
        write (client, "stopping server", strlen (szHttpServerResponse));
#endif
        break;
      }
      else if (status == 0x2000)
      {
        printf ("Reply with:\n[%s]\n", "sending report");
#ifdef LISTEN_HTTPS
        SSL_write (ssl, "sending report", strlen (szHttpServerResponse));
#else
        write (client, "sending report", strlen (szHttpServerResponse));
#endif
        report_serverp();
      }
      else if (status == 0x3000)
      {
        printf ("Reply with:\n[%s]\n", "sending archive");
#ifdef LISTEN_HTTPS
        SSL_write (ssl, "sending archive", strlen (szHttpServerResponse));
#else
        write (client, "sending archive", strlen (szHttpServerResponse));
#endif
        archive_serverp();
      }
      else if (status == 0x4000)
      {
        printf ("Reply with:\n[%s]\n", "checking server");
#ifdef LISTEN_HTTPS
        SSL_write (ssl, "checking server", strlen (szHttpServerResponse));
#else
        write (client, "checking server", strlen (szHttpServerResponse));
#endif
#endif
        system("touch server-running");
      }
      else
      {
        printf ("Bad message status 0x%x\n", status);
      }
    }
    else
    {
      printf ("Empty message\n");
      status = status | 0x08;
      ERR_print_errors_fp (stderr);
    }
#ifndef OFFLINE_SIMULATION
#ifdef LISTEN_HTTPS
    do_nanosleep_ccc(DELAY_TIME);    // trying a delay here to see if it helps
    sd = SSL_get_fd (ssl); /* get socket connection */
    SSL_free (ssl);        /* release SSL state */
    close (sd);            /* close connection */
#else
    close (client);
#endif
#endif

    int message_status;
    if (status == 0)
    {
      message_status = process_message(buf);
      if (message_status)
      {
        char temp[8];
        sprintf (temp, "0x%x", message_status);
        send_report_email("hddscerror: process message", temp, "messagebuffer.txt", buf, -1);
      }
    }
    else
    {
      message_status = status;
    }

    int reply_status = 1;
#ifdef ENABLE_REPLY
    if (message_status == 0)
    {
      reply_status = server_reply(message_items.body);
    }
#endif
    if (reply_status == 0)
    {
      process_payment();
    }
#ifdef OFFLINE_SIMULATION
    break;
#endif
  }
  close (server);      /* close server socket */

#ifdef LISTEN_HTTPS
  ERR_free_strings (); /* free memory from SSL_load_error_strings */
  EVP_cleanup ();      /* free memory from OpenSSL_add_all_algorithms */
  SSL_CTX_free (ctx);  /* release context */
#endif

  system("rm -f server-running");

  return 0;
}





int server_reply(const char *message)
{
  char buf[1024*4];
  int bytes;
  char *hostname;
  char *path;
#ifndef OFFLINE_SIMULATION
  int server;
  uint16_t portnum;
  struct hostent *host;
  struct sockaddr_in addr;

#ifdef SEND_HTTPS
  SSL *ssl;
  SSL_CTX *ctx;
  const SSL_METHOD *method;
  portnum = 443;
#else
  portnum = 80;
#endif
#endif
  hostname = HOSTNAME;
  path = PATH;
  printf ("Sending...\n");

#ifdef SEND_HTTPS
  // Initialize the SSL library
  SSL_library_init ();
  OpenSSL_add_all_algorithms ();     /* Load cryptos, et.al. */
  SSL_load_error_strings ();         /* Bring in and register error messages */
  method = TLSv1_2_client_method (); /* Create new client-method instance */
  ctx = SSL_CTX_new (method);        /* Create new context */
  if (ctx == NULL)
  {
    ERR_print_errors_fp (stderr);
    LOCAL_ABORT ();
  }
#endif
#ifndef OFFLINE_SIMULATION
  if ((host = gethostbyname (hostname)) == NULL)
  {
    perror (hostname);
    LOCAL_ABORT ();
  }
  server = socket (PF_INET, SOCK_STREAM, 0);
  bzero (&addr, sizeof (addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons (portnum);
  addr.sin_addr.s_addr = * (long*) (host->h_addr);

  if (connect (server, (struct sockaddr*)&addr, sizeof (addr)) != 0)
  {
    close (server);
    perror (hostname);
    fprintf (stderr, "Is the server running, and on the correct port (%d)?\n", portnum);
    LOCAL_ABORT ();
  }
#endif
#ifdef SEND_HTTPS
  ssl = SSL_new (ctx);      /* create new SSL connection state */
  SSL_set_fd (ssl, server);    /* attach the socket descriptor */
  if (SSL_connect (ssl) <= 0)   /* perform the connection */
  {
    ERR_print_errors_fp (stderr);
    close (server);          /* close socket */
    SSL_CTX_free (ctx);
  }
#endif
  char body[MESSAGE_SIZE+(1024*1)];
  strcpy (body, "");
  strcpy (body, "cmd=_notify-validate&");
  strcat (body, message);
  int size = strlen(body);
  char szRequest[MESSAGE_SIZE+(1024*2)];
  sprintf (szRequest,
           "POST %s HTTP/1.1\r\n"
//           "Accept: */*\r\n"
           "user-agent: My Custom User Agent\r\n"
           "content-type: application/x-www-form-urlencoded\r\n"
           "content-length: %d\r\n"
           "host: %s\r\n"
           "Connection: close\r\n"
           "\r\n"
           "%s", path, size, hostname, body);

  printf ("Sending:\n[%s]\n", szRequest);

#ifndef OFFLINE_SIMULATION
#ifdef SEND_HTTPS
  char *line;
  printf ("\n\nConnected with %s encryption\n", SSL_get_cipher (ssl));
  X509 *cert;
  cert = SSL_get_peer_certificate (ssl); /* get the server's certificate */
  if (cert != NULL)
  {
    printf ("Server certificates:\n");
    line = X509_NAME_oneline (X509_get_subject_name (cert), 0, 0);
    printf ("Subject: %s\n", line);
    free (line);       /* free the malloc'ed string */
    line = X509_NAME_oneline (X509_get_issuer_name (cert), 0, 0);
    printf ("Issuer: %s\n\n", line);
    free (line);       /* free the malloc'ed string */
    X509_free (cert);  /* free the malloc'ed certificate copy */
  }
  else
  {
    printf ("Info: No client certificates configured.\n");
  }
  SSL_write (ssl, szRequest, strlen (szRequest));   /* encrypt & send message */
  printf ("Waiting for reply...\n");
  bytes = SSL_read (ssl, buf, sizeof (buf)); /* get reply & decrypt */
  SSL_free (ssl);        /* release connection state */
  SSL_CTX_free (ctx);      /* release context */
#else
  write (server, szRequest, strlen (szRequest));
  printf ("Waiting for reply...\n");
  bytes = read (server, buf, sizeof (buf));
#endif
#else
  FILE *readfile = fopen(OFFLINE_REPLY_FILE, "rb");
  if (readfile == NULL)
  {
    printf ("No offline reply file.\n");
    return 0;
  }
  bytes = fread(buf, 1, sizeof (buf), readfile);
  fclose(readfile);
#endif

  buf[bytes] = '\0';
  printf ("Received (%d bytes):\n[%s]\n", bytes, buf);
#ifndef OFFLINE_SIMULATION
  close (server);          /* close socket */
#endif

  int status = process_reply(buf);
  return status;
}





int process_reply(const char *message)
{
  int message_length = strlen(message);
  int position = 0;
  int firstline = 1;
  int endofheader = 0;
  memset (&reply_items, 0, sizeof(reply_items));
  while (position < message_length)
  {
    char line[message_length];
    memset (line, 0, message_length);
    char c;
    while (1)
    {
      if (position >= message_length)
      {
        break;
      }
      memcpy (&c, message+position, 1);
      if (c == '\r')
      {
        position++;
        continue;
      }
      if (c == '\n')
      {
        position++;
        break;
      }
      strncat (line, &c, 1);
      position++;
    }
    //fprintf (stdout, "line= %s\n", line);    //debug
    char item[MESSAGE_SIZE];
    char data[MESSAGE_SIZE];
    memset (item, 0, MESSAGE_SIZE);
    memset (data, 0, MESSAGE_SIZE);
    sscanf(line, "%s %[^\n]", item, data);
    //fprintf (stdout, "item= %s data= %s\n", item, data);    //debug
    if (strcmp(item, "") == 0)
    {
      //fprintf (stdout, "end of header\n");    //debug
      endofheader = 1;
    }
    else if (firstline)
    {
      strncpy (reply_items.http, item, LINE_LENGTH-1);
      char status_code[LINE_LENGTH];
      char status_text[LINE_LENGTH];
      sscanf(data, "%s %s", status_code, status_text);
      strncpy (reply_items.status_code, status_code, LINE_LENGTH-1);
      strncpy (reply_items.status_text, status_text, LINE_LENGTH-1);
      firstline = 0;
    }
    else
    {
      //fprintf (stdout, "item= %s data= %s\n", item, data);    //debug
      if (strcasecmp (item, "content-length:") == 0)
      {
        strncpy (reply_items.content_length, data, LINE_LENGTH-1);
      }
      else if (strcasecmp (item, "content-type:") == 0)
      {
        strncpy (reply_items.content_type, data, LINE_LENGTH-1);
      }
      else if (strcasecmp (item, "connection:") == 0)
      {
        strncpy (reply_items.connection, data, LINE_LENGTH-1);
      }
      else if (endofheader)
      {
        strncpy (reply_items.body, item, MESSAGE_SIZE-1);
      }
    }
  }
  int body_size = strlen(reply_items.body);
  if (0)
  {
    fprintf (stdout, "http = %s\n", reply_items.http);
    fprintf (stdout, "status_code = %s\n", reply_items.status_code);
    fprintf (stdout, "status_text = %s\n", reply_items.status_text);
    fprintf (stdout, "content-length = %s\n", reply_items.content_length);
    fprintf (stdout, "content-type = %s\n", reply_items.content_type);
    fprintf (stdout, "connection = %s\n", reply_items.connection);
    fprintf (stdout, "body = %s\n", reply_items.body);
    fprintf (stdout, "body-size = %d\n", body_size);
  }
  int reply_status = 0;
  if (strncasecmp(reply_items.http, "HTTP", 4) != 0)
  {
    reply_status = reply_status | 0x01;
  }
  if (strcasecmp(reply_items.status_code, "200") != 0)
  {
    reply_status = reply_status | 0x02;
  }
  if (strcasecmp(reply_items.status_text, "OK") != 0)
  {
    reply_status = reply_status | 0x04;
  }
  if (strcasecmp(reply_items.content_type, "text/html; charset=UTF-8") != 0)
  {
    reply_status = reply_status | 0x08;
  }
  if (reply_status)
  {
    fprintf (stdout, "*** reply error ***\n");
  }
  else if ( strcasecmp(reply_items.body, "VERIFIED") == 0)
  {
    fprintf (stdout, "*** reply verified ***\n");
  }
  else if (strcasecmp(reply_items.body, "INVALID") == 0)
  {
    fprintf (stdout, "*** reply invalid ***\n");
    if (CHECK_VERIFIED_REPLY)
    {
      reply_status = reply_status | 0x10;
    }
  }
  else
  {
    fprintf (stdout, "*** reply not verified or invalid ***\n");
    if (CHECK_VERIFIED_REPLY)
    {
      //TODO turn this back on after paypal gets their shit working again
      //reply_status = reply_status | 0x20;
    }
  }

  fprintf (stdout, "reply status = 0x%x\n", reply_status);
  return reply_status;
}





int process_message(const char *message)
{
  int message_length = strlen(message);
  int position = 0;
  int firstline = 1;
  int endofheader = 0;
  memset (&message_items, 0, sizeof(message_items));
  while (position < message_length)
  {
    char line[message_length];
    memset (line, 0, message_length);
    char c;
    while (1)
    {
      if (position >= message_length)
      {
        break;
      }
      memcpy (&c, message+position, 1);
      if (c == '\r')
      {
        position++;
        continue;
      }
      if (c == '\n')
      {
        position++;
        break;
      }
      strncat (line, &c, 1);
      position++;
    }
    //fprintf (stdout, "line= %s\n", line);    //debug
    char item[MESSAGE_SIZE];
    char data[MESSAGE_SIZE];
    memset (item, 0, MESSAGE_SIZE);
    memset (data, 0, MESSAGE_SIZE);
    sscanf(line, "%s %[^\n]", item, data);
    //fprintf (stdout, "item= %s data= %s\n", item, data);    //debug
    if (strcmp(item, "") == 0)
    {
      //fprintf (stdout, "end of header\n");    //debug
      endofheader = 1;
    }
    else if (firstline)
    {
      strncpy (message_items.command, item, LINE_LENGTH-1);
      char path[LINE_LENGTH];
      char http[LINE_LENGTH];
      sscanf(data, "%s %s", path, http);
      strncpy (message_items.path, path, LINE_LENGTH-1);
      strncpy (message_items.http, http, LINE_LENGTH-1);
      firstline = 0;
    }
    else
    {
      //fprintf (stdout, "item= %s data= %s\n", item, data);    //debug
      if (strcasecmp (item, "content-length:") == 0)
      {
        strncpy (message_items.content_length, data, LINE_LENGTH-1);
      }
      else if (strcasecmp (item, "content-type:") == 0)
      {
        strncpy (message_items.content_type, data, LINE_LENGTH-1);
      }
      else if (strcasecmp (item, "host:") == 0)
      {
        strncpy (message_items.host, data, LINE_LENGTH-1);
      }
      else if (strcasecmp (item, "connection:") == 0)
      {
        strncpy (message_items.connection, data, LINE_LENGTH-1);
      }
      else if (strcasecmp (item, "user-agent:") == 0)
      {
        strncpy (message_items.user_agent, data, LINE_LENGTH-1);
      }
      else if (strcasecmp (item, "accept:") == 0)
      {
        strncpy (message_items.accept, data, LINE_LENGTH-1);
      }
      else if (endofheader)
      {
        strncpy (message_items.body, item, MESSAGE_SIZE-1);
      }
    }
  }
  int body_size = strlen(message_items.body);
  if (0)
  {
    fprintf (stdout, "command = %s\n", message_items.command);
    fprintf (stdout, "path = %s\n", message_items.path);
    fprintf (stdout, "http = %s\n", message_items.http);
    fprintf (stdout, "accept = %s\n", message_items.accept);
    fprintf (stdout, "user-agent = %s\n", message_items.user_agent);
    fprintf (stdout, "content-length = %s\n", message_items.content_length);
    fprintf (stdout, "content-type = %s\n", message_items.content_type);
    fprintf (stdout, "host = %s\n", message_items.host);
    fprintf (stdout, "connection = %s\n", message_items.connection);
    fprintf (stdout, "body = %s\n", message_items.body);
    fprintf (stdout, "body-size = %d\n", body_size);
  }
  int request_status = 0;
  if (strcasecmp(message_items.command, "POST") != 0)
  {
    request_status = request_status | 0x01;
  }
  if (strcasecmp(message_items.host, MY_HOST_NAME) != 0)
  {
    request_status = request_status | 0x02;
  }
  if (strcasecmp(message_items.path, MY_HOST_PATH) != 0)
  {
    request_status = request_status | 0x04;
  }
  if (strcasecmp(message_items.content_type, "application/x-www-form-urlencoded") != 0)
  {
    request_status = request_status | 0x08;
  }
  fprintf (stdout, "request status = 0x%x\n", request_status);
  return request_status;
}





int process_body(const char *body)
{
  // decode urlencoded
  char decoded_body[strlen(body)+1];
  memset (decoded_body, 0, sizeof(decoded_body));
  char *o;
  const char *end = body + strlen(body);
  int c;
  int decode_status = 0;
  for (o = decoded_body; body <= end; o++)
  {
    c = *body++;
    if (c == '+')
    {
      c = ' ';
    }
    else if (c == '%' && (  !isxdigit(*body++) || !isxdigit(*body++) || !sscanf(body - 2, "%2x", &c)))
    {
      decode_status = 1;
    }
    *o = c;
  }
  if (decode_status)
  {
    fprintf (stdout, "Error decoding\n");
    return -1;
  }
  //fprintf (stdout, "Decoded:\n%s\n", decoded_body);    //debug

  memset (&body_items, 0, sizeof(body_items));
  int body_length = strlen(decoded_body);
  int position = 0;
  int non_printable_character = 0;
  while (position < body_length)
  {
    char line[body_length+1];
    memset (line, 0, body_length);
    char c;
    while (1)
    {
      if (position >= body_length)
      {
        break;
      }
      memcpy (&c, decoded_body+position, 1);
      if (isprint(c) == 0)
      {
        c = '.';
        non_printable_character++;
      }
      if (c == '&')
      {
        c = '\n';
        strncat (line, &c, 1);
        position++;
        break;
      }
      strncat (line, &c, 1);
      position++;
    }
    //fprintf (stdout, "line= %s", line);    //debug

    char item[MESSAGE_SIZE];
    char data[MESSAGE_SIZE];
    memset (item, 0, MESSAGE_SIZE);
    memset (data, 0, MESSAGE_SIZE);
    sscanf(line, "%[^'=']=%[^\n]", item, data);
    //fprintf (stdout, "item= %s data= %s\n", item, data);    //debug
    if (strcasecmp (item, "receiver_email") == 0)
    {
      strncpy (body_items.receiver_email, data, LINE_LENGTH-1);
      fprintf (stdout, "receiver_email= %s\n", body_items.receiver_email);
    }
    else if (strcasecmp (item, "receiver_id") == 0)
    {
      strncpy (body_items.receiver_id, data, LINE_LENGTH-1);
      fprintf (stdout, "receiver_id= %s\n", body_items.receiver_id);
    }
    else if (strcasecmp (item, "residence_country") == 0)
    {
      strncpy (body_items.residence_country, data, LINE_LENGTH-1);
      fprintf (stdout, "residence_country= %s\n", body_items.residence_country);
    }
    else if (strcasecmp (item, "test_ipn") == 0)
    {
      strncpy (body_items.test_ipn, data, LINE_LENGTH-1);
      fprintf (stdout, "test_ipn= %s\n", body_items.test_ipn);
    }
    else if (strcasecmp (item, "transaction_subject") == 0)
    {
      strncpy (body_items.transaction_subject, data, LINE_LENGTH-1);
      fprintf (stdout, "transaction_subject= %s\n", body_items.transaction_subject);
    }
    else if (strcasecmp (item, "txn_id") == 0)
    {
      strncpy (body_items.txn_id, data, LINE_LENGTH-1);
      fprintf (stdout, "txn_id= %s\n", body_items.txn_id);
    }
    else if (strcasecmp (item, "txn_type") == 0)
    {
      strncpy (body_items.txn_type, data, LINE_LENGTH-1);
      fprintf (stdout, "txn_type= %s\n", body_items.txn_type);
    }
    else if (strcasecmp (item, "payer_email") == 0)
    {
      strncpy (body_items.payer_email, data, LINE_LENGTH-1);
      fprintf (stdout, "payer_email= %s\n", body_items.payer_email);
    }
    else if (strcasecmp (item, "payer_id") == 0)
    {
      strncpy (body_items.payer_id, data, LINE_LENGTH-1);
      fprintf (stdout, "payer_id= %s\n", body_items.payer_id);
    }
    else if (strcasecmp (item, "payer_status") == 0)
    {
      strncpy (body_items.payer_status, data, LINE_LENGTH-1);
      fprintf (stdout, "payer_status= %s\n", body_items.payer_status);
    }
    else if (strcasecmp (item, "first_name") == 0)
    {
      strncpy (body_items.first_name, data, LINE_LENGTH-1);
      fprintf (stdout, "first_name= %s\n", body_items.first_name);
    }
    else if (strcasecmp (item, "last_name") == 0)
    {
      strncpy (body_items.last_name, data, LINE_LENGTH-1);
      fprintf (stdout, "last_name= %s\n", body_items.last_name);
    }
    else if (strcasecmp (item, "address_city") == 0)
    {
      strncpy (body_items.address_city, data, LINE_LENGTH-1);
      fprintf (stdout, "address_city= %s\n", body_items.address_city);
    }
    else if (strcasecmp (item, "address_country") == 0)
    {
      strncpy (body_items.address_country, data, LINE_LENGTH-1);
      fprintf (stdout, "address_country= %s\n", body_items.address_country);
    }
    else if (strcasecmp (item, "address_state") == 0)
    {
      strncpy (body_items.address_state, data, LINE_LENGTH-1);
      fprintf (stdout, "address_state= %s\n", body_items.address_state);
    }
    else if (strcasecmp (item, "address_status") == 0)
    {
      strncpy (body_items.address_status, data, LINE_LENGTH-1);
      fprintf (stdout, "address_status= %s\n", body_items.address_status);
    }
    else if (strcasecmp (item, "address_country_code") == 0)
    {
      strncpy (body_items.address_country_code, data, LINE_LENGTH-1);
      fprintf (stdout, "address_country_code= %s\n", body_items.address_country_code);
    }
    else if (strcasecmp (item, "address_name") == 0)
    {
      strncpy (body_items.address_name, data, LINE_LENGTH-1);
      fprintf (stdout, "address_name= %s\n", body_items.address_name);
    }
    else if (strcasecmp (item, "address_street") == 0)
    {
      strncpy (body_items.address_street, data, LINE_LENGTH-1);
      fprintf (stdout, "address_street= %s\n", body_items.address_street);
    }
    else if (strcasecmp (item, "address_zip") == 0)
    {
      strncpy (body_items.address_zip, data, LINE_LENGTH-1);
      fprintf (stdout, "address_zip= %s\n", body_items.address_zip);
    }
    else if (strcasecmp (item, "custom") == 0)
    {
      strncpy (body_items.custom, data, LINE_LENGTH-1);
      fprintf (stdout, "custom= %s\n", body_items.custom);
    }
    else if (strcasecmp (item, "handling_amount") == 0)
    {
      strncpy (body_items.handling_amount, data, LINE_LENGTH-1);
      fprintf (stdout, "handling_amount= %s\n", body_items.handling_amount);
    }
    else if (strcasecmp (item, "item_name") == 0)
    {
      strncpy (body_items.item_name, data, LINE_LENGTH-1);
      fprintf (stdout, "item_name= %s\n", body_items.item_name);
    }
    else if (strcasecmp (item, "item_number") == 0)
    {
      strncpy (body_items.item_number, data, LINE_LENGTH-1);
      fprintf (stdout, "item_number= %s\n", body_items.item_number);
    }
    else if (strcasecmp (item, "mc_currency") == 0)
    {
      strncpy (body_items.mc_currency, data, LINE_LENGTH-1);
      fprintf (stdout, "mc_currency= %s\n", body_items.mc_currency);
    }
    else if (strcasecmp (item, "mc_fee") == 0)
    {
      strncpy (body_items.mc_fee, data, LINE_LENGTH-1);
      fprintf (stdout, "mc_fee= %s\n", body_items.mc_fee);
    }
    else if (strcasecmp (item, "mc_gross") == 0)
    {
      strncpy (body_items.mc_gross, data, LINE_LENGTH-1);
      fprintf (stdout, "mc_gross= %s\n", body_items.mc_gross);
    }
    else if (strcasecmp (item, "payment_date") == 0)
    {
      strncpy (body_items.payment_date, data, LINE_LENGTH-1);
      fprintf (stdout, "payment_date= %s\n", body_items.payment_date);
    }
    else if (strcasecmp (item, "payment_fee") == 0)
    {
      strncpy (body_items.payment_fee, data, LINE_LENGTH-1);
      fprintf (stdout, "payment_fee= %s\n", body_items.payment_fee);
    }
    else if (strcasecmp (item, "payment_gross") == 0)
    {
      strncpy (body_items.payment_gross, data, LINE_LENGTH-1);
      fprintf (stdout, "payment_gross= %s\n", body_items.payment_gross);
    }
    else if (strcasecmp (item, "payment_status") == 0)
    {
      strncpy (body_items.payment_status, data, LINE_LENGTH-1);
      fprintf (stdout, "payment_status= %s\n", body_items.payment_status);
    }
    else if (strcasecmp (item, "payment_type") == 0)
    {
      strncpy (body_items.payment_type, data, LINE_LENGTH-1);
      fprintf (stdout, "payment_type= %s\n", body_items.payment_type);
    }
    else if (strcasecmp (item, "protection_eligibility") == 0)
    {
      strncpy (body_items.protection_eligibility, data, LINE_LENGTH-1);
      fprintf (stdout, "protection_eligibility= %s\n", body_items.protection_eligibility);
    }
    else if (strcasecmp (item, "quantity") == 0)
    {
      strncpy (body_items.quantity, data, LINE_LENGTH-1);
      fprintf (stdout, "quantity= %s\n", body_items.quantity);
    }
    else if (strcasecmp (item, "shipping") == 0)
    {
      strncpy (body_items.shipping, data, LINE_LENGTH-1);
      fprintf (stdout, "shipping= %s\n", body_items.shipping);
    }
    else if (strcasecmp (item, "tax") == 0)
    {
      strncpy (body_items.tax, data, LINE_LENGTH-1);
      fprintf (stdout, "tax= %s\n", body_items.tax);

    }
    else if (strcasecmp (item, "notify_version") == 0)
    {
      strncpy (body_items.notify_version, data, LINE_LENGTH-1);
      fprintf (stdout, "notify_version= %s\n", body_items.notify_version);
    }
    else if (strcasecmp (item, "charset") == 0)
    {
      strncpy (body_items.charset, data, LINE_LENGTH-1);
      fprintf (stdout, "charset= %s\n", body_items.charset);
    }
    else if (strcasecmp (item, "verify_sign") == 0)
    {
      strncpy (body_items.verify_sign, data, LINE_LENGTH-1);
      fprintf (stdout, "verify_sign= %s\n", body_items.verify_sign);
    }
    else if (strcasecmp (item, "business") == 0)
    {
      strncpy (body_items.business, data, LINE_LENGTH-1);
      fprintf (stdout, "business= %s\n", body_items.business);
    }
    else if (strcasecmp (item, "shipping_discount") == 0)
    {
      strncpy (body_items.shipping_discount, data, LINE_LENGTH-1);
      fprintf (stdout, "shipping_discount= %s\n", body_items.shipping_discount);
    }
    else if (strcasecmp (item, "insurance_amount") == 0)
    {
      strncpy (body_items.insurance_amount, data, LINE_LENGTH-1);
      fprintf (stdout, "insurance_amount= %s\n", body_items.insurance_amount);
    }
    else if (strcasecmp (item, "discount") == 0)
    {
      strncpy (body_items.discount, data, LINE_LENGTH-1);
      fprintf (stdout, "discount= %s\n", body_items.discount);
    }
    else if (strcasecmp (item, "shipping_method") == 0)
    {
      strncpy (body_items.shipping_method, data, LINE_LENGTH-1);
      fprintf (stdout, "shipping_method= %s\n", body_items.shipping_method);
    }
    else if (strcasecmp (item, "ipn_track_id") == 0)
    {
      strncpy (body_items.ipn_track_id, data, LINE_LENGTH-1);
      fprintf (stdout, "ipn_track_id= %s\n", body_items.ipn_track_id);
    }
    else if (strcasecmp (item, "XXXX") == 0)
    {
      //strncpy (body_items.XXXX, data, LINE_LENGTH-1);
      //fprintf (stdout, "XXXX= %s\n", body_items.XXXX);
    }
    else
    {
      fprintf (stdout, "UNRECOGNIZED item=%s   data=%s\n", item, data);
    }
  }

  if (non_printable_character)
  {
    fprintf (stdout, "***WARNING*** There were %d non-printable characters.\n", non_printable_character);
    send_report_email("hddscerror: non-printable character", "hddscerror: non-printable character", "messagebody.txt", message_items.body, -1);
  }

  return 0;
}





int process_fields(void)
{
  int status = 0;
  if (strcasecmp(body_items.receiver_email, "scott.dwyer@yahoo.com") != 0)
  {
    status = status | 0x01;
  }
  if (strcasecmp(body_items.payment_status, "Completed") != 0)
  {
    status = status | 0x02;
  }
  return 0;
}





void set_raw_csv_data(char *license, int version, int type, int days, char *expire_date)
{
  memset (csv_raw_data, 0, sizeof(csv_raw_data));
  get_the_time_ccc();
  char temp[64];

  strcpy (csv_raw_data, "timestamp");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "txn_id");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "license");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "version");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "type");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "days");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "expire_date");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "first_name");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "last_name");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "payer_email");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "address_name");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "address_street");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "address_city");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "address_state");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "address_zip");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "address_country");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "address_country_code");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "residence_country");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "address_status");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "item_name");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "item_number");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "quantity");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "mc_gross");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "tax");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "mc_fee");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "mc_currency");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "payment_date");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "payment_status");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "protection_eligibility");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "dmde_coupon_code");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "transaction_subject");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "txn_type");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "payer_id");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "payer_status");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "receiver_email");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "receiver_id");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "test_ipn");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "custom");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "handling_amount");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "payment_fee");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "payment_gross");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "payment_type");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "shipping");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "notify_version");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "charset");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "verify_sign");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "business");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "shipping_discount");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "insurance_amount");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "discount");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "shipping_method");
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, "ipn_track_id");
  strcat (csv_raw_data, "\n");


  strcat (csv_raw_data, current_date_ccc);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.txn_id);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, license);
  strcat (csv_raw_data, ",");

  sprintf (temp, "%d", version);
  strcat (csv_raw_data, temp);
  strcat (csv_raw_data, ",");

  sprintf (temp, "%d", type);
  strcat (csv_raw_data, temp);
  strcat (csv_raw_data, ",");

  sprintf (temp, "%d", days);
  strcat (csv_raw_data, temp);
  strcat (csv_raw_data, ",");

  if (type == 2)
  {
    strcat (csv_raw_data, "never");
  }
  else
  {
    strcat (csv_raw_data, expire_date);
  }
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.first_name);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.last_name);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.payer_email);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.address_name);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.address_street);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.address_city);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.address_state);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.address_zip);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.address_country);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.address_country_code);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.residence_country);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.address_status);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.item_name);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.item_number);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.quantity);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.mc_gross);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.tax);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.mc_fee);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.mc_currency);
  strcat (csv_raw_data, ",\"");

  strcat (csv_raw_data, body_items.payment_date);
  strcat (csv_raw_data, "\",");

  strcat (csv_raw_data, body_items.payment_status);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.protection_eligibility);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.dmde_coupon_code);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.transaction_subject);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.txn_type);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.payer_id);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.payer_status);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.receiver_email);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.receiver_id);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.test_ipn);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.custom);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.handling_amount);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.payment_fee);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.payment_gross);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.payment_type);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.shipping);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.notify_version);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.charset);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.verify_sign);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.business);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.shipping_discount);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.insurance_amount);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.discount);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.shipping_method);
  strcat (csv_raw_data, ",");

  strcat (csv_raw_data, body_items.ipn_track_id);
  strcat (csv_raw_data, "\n");
}





void set_short_csv_data(char *license, int version, int type, int days, char *expire_date)
{
  memset (csv_short_data, 0, sizeof(csv_short_data));
  get_the_time_ccc();
  char temp[64];

  strcpy (csv_short_data, "timestamp");
  strcat (csv_short_data, ",");

  strcat (csv_short_data, "txn_id");
  strcat (csv_short_data, ",");

  strcat (csv_short_data, "license");
  strcat (csv_short_data, ",");

  //strcat (csv_short_data, "version");
  //strcat (csv_short_data, ",");

  //strcat (csv_short_data, "type");
  //strcat (csv_short_data, ",");

  strcat (csv_short_data, "days");
  strcat (csv_short_data, ",");

  strcat (csv_short_data, "expire_date");
  strcat (csv_short_data, ",");

  strcat (csv_short_data, "payer_name");
  strcat (csv_short_data, ",");

  strcat (csv_short_data, "payer_email");
  strcat (csv_short_data, ",");

  strcat (csv_short_data, "address_name");
  strcat (csv_short_data, ",");

  //strcat (csv_short_data, "address_street");
  //strcat (csv_short_data, ",");

  strcat (csv_short_data, "address_city");
  strcat (csv_short_data, ",");

  strcat (csv_short_data, "address_state");
  strcat (csv_short_data, ",");

  //strcat (csv_short_data, "address_zip");
  //strcat (csv_short_data, ",");

  strcat (csv_short_data, "address_country");
  strcat (csv_short_data, ",");

  //strcat (csv_short_data, "address_country_code");
  //strcat (csv_short_data, ",");

  //strcat (csv_short_data, "residence_country");
  //strcat (csv_short_data, ",");

  //strcat (csv_short_data, "address_status");
  //strcat (csv_short_data, ",");

  strcat (csv_short_data, "item_name");
  strcat (csv_short_data, ",");

  //strcat (csv_short_data, "item_number");
  //strcat (csv_short_data, ",");

  strcat (csv_short_data, "quantity");
  strcat (csv_short_data, ",");

  strcat (csv_short_data, "mc_gross");
  strcat (csv_short_data, ",");

  //strcat (csv_short_data, "tax");
  //strcat (csv_short_data, ",");

  //strcat (csv_short_data, "mc_fee");
  //strcat (csv_short_data, ",");

  strcat (csv_short_data, "mc_currency");
  strcat (csv_short_data, ",");

  strcat (csv_short_data, "payment_date");
  strcat (csv_short_data, ",");

  //strcat (csv_short_data, "payment_status");
  //strcat (csv_short_data, ",");

  //strcat (csv_short_data, "protection_eligibility");
  //strcat (csv_short_data, "\n");

  strcat (csv_short_data, "dmde_coupon_code");
  strcat (csv_short_data, "\n");


  strcat (csv_short_data, current_date_ccc);
  strcat (csv_short_data, ",");

  strcat (csv_short_data, body_items.txn_id);
  strcat (csv_short_data, ",");

  strcat (csv_short_data, license);
  strcat (csv_short_data, ",");

  sprintf (temp, "%d", version);
  //strcat (csv_short_data, temp);
  ///strcat (csv_short_data, ",");

  sprintf (temp, "%d", type);
  //strcat (csv_short_data, temp);
  //strcat (csv_short_data, ",");

  sprintf (temp, "%d", days);
  strcat (csv_short_data, temp);
  strcat (csv_short_data, ",");

  if (type == 2)
  {
    strcat (csv_short_data, "never");
  }
  else
  {
    strcat (csv_short_data, expire_date);
  }
  strcat (csv_short_data, ",");

  strcat (csv_short_data, body_items.first_name);
  strcat (csv_short_data, " ");

  strcat (csv_short_data, body_items.last_name);
  strcat (csv_short_data, ",");

  strcat (csv_short_data, body_items.payer_email);
  strcat (csv_short_data, ",");

  strcat (csv_short_data, body_items.address_name);
  strcat (csv_short_data, ",");

  //strcat (csv_short_data, body_items.address_street);
  //strcat (csv_short_data, ",");

  strcat (csv_short_data, body_items.address_city);
  strcat (csv_short_data, ",");

  strcat (csv_short_data, body_items.address_state);
  strcat (csv_short_data, ",");

  //strcat (csv_short_data, body_items.address_zip);
  //strcat (csv_short_data, ",");

  strcat (csv_short_data, body_items.address_country);
  strcat (csv_short_data, ",");

  //strcat (csv_short_data, body_items.address_country_code);
  //strcat (csv_short_data, ",");

  //strcat (csv_short_data, body_items.residence_country);
  //strcat (csv_short_data, ",");

  //strcat (csv_short_data, body_items.address_status);
  //strcat (csv_short_data, ",");

  strcat (csv_short_data, body_items.item_name);
  strcat (csv_short_data, ",");

  //strcat (csv_short_data, body_items.item_number);
  //strcat (csv_short_data, ",");

  strcat (csv_short_data, body_items.quantity);
  strcat (csv_short_data, ",");

  strcat (csv_short_data, body_items.mc_gross);
  strcat (csv_short_data, ",");

  //strcat (csv_short_data, body_items.tax);
  //strcat (csv_short_data, ",");

  //strcat (csv_short_data, body_items.mc_fee);
  //strcat (csv_short_data, ",");

  strcat (csv_short_data, body_items.mc_currency);
  strcat (csv_short_data, ",\"");

  strcat (csv_short_data, body_items.payment_date);
  strcat (csv_short_data, "\",");

  //strcat (csv_short_data, body_items.payment_status);
  //strcat (csv_short_data, ",");

  //strcat (csv_short_data, body_items.protection_eligibility);
  //strcat (csv_short_data, "\n");

  strcat (csv_short_data, body_items.dmde_coupon_code);
  strcat (csv_short_data, "\n");
}





int append_data_to_file(char *filename, char *data)
{
  FILE *writefile = fopen(filename, "a+");
  if (writefile == NULL)
  {
    fprintf (stdout, "Cannot open %s for writing (%s).\n", filename, strerror(errno));
    return -1;
  }
  fprintf (writefile, "%s", data);
  fclose(writefile);
  return 0;
}





int process_payment(void)
{
  if (!manual_create)
  {
    // process the message body
    if (process_body(message_items.body))
    {
      fprintf (stdout, "hddscerror: process body\n");
      send_report_email("hddscerror: process body", "hddscerror: process body", "messagebody.txt", message_items.body, -1);
      return -1;
    }

    if (CHECK_RECIEVER_EMAIL)
    {
      // check for proper receiver email address
      if (strcasecmp(body_items.receiver_email, RECEIVER_EMAIL) != 0)
      {
        fprintf (stdout, "hddscerror: wrong receiver email\n");
        send_report_email("hddscerror: wrong receiver email", body_items.receiver_email, "messagebody.txt", message_items.body, -1);
        return -1;
      }
    }

    if (1)
    {
      // check for completed payment
      if (strcasecmp(body_items.payment_status, "Completed") != 0)
      {
        fprintf (stdout, "hddscerror: payment not completed\n");
        send_report_email("hddscerror: payment not completed", body_items.payment_status, "messagebody.txt", message_items.body, -1);
        return -1;
      }
    }
    if (check_duplicate_id())
    {
      fprintf (stdout, "hddscerror: duplicate transaction id\n");
      send_report_email("hddscerror: duplicate transaction id", body_items.txn_id, "messagebody.txt", message_items.body, -1);
      return -1;
    }
    if (strcmp(body_items.quantity, "1") != 0)
    {
      fprintf (stdout, "hddscerror: quantity other than 1\n");
      send_report_email("hddscerror: quantity other than 1", body_items.quantity, "messagebody.txt", message_items.body, -1);
    }
  }


  // to create temp license: hddsuperclone --license-version 1 --license-type 1 --license-time 65 --license-make 1
  // to create full license: hddsuperclone --license-version 1 --license-type 2 --license-time 0 --license-make 1
  int version = 1;
  int days;
  int type;
  char expire_date[64];

  if (strcasecmp(body_items.item_name, HDDTEST_ITEM_NAME) == 0)
  {
    char gross[LINE_LENGTH];
    strcpy (gross, body_items.mc_gross);
    if (gross[0] == '$')
    {
      gross[0] = ' ';
    }
    int price_paid = strtol(gross, NULL, 10);
    if (price_paid < HDDTEST_PRICE)
    {
      fprintf (stdout, "Error: price paid too low %d %s\n", price_paid, body_items.mc_gross);
      send_report_email("hddscerror: hddtest price paid too low", body_items.mc_gross, "messagebody.txt", message_items.body, -1);
      return -1;
    }
    days = 1;
    type = 1;
    strcpy (expire_date, get_future_date_ccc(days));
    if (do_make_license_file (version, type, days))
    {
      fprintf (stdout, "hddscerror: failed to make license\n");
      send_report_email("hddscerror: failed to make license", "hddscerror: failed to make license", "messagebody.txt", message_items.body, -1);
      return -1;
    }
    char *coupon;
    if (DMDE_COUPONS_ENABLED)
    {
      coupon = get_dmde_coupon(DMDE_15USD_NAME);
      if (coupon == NULL)
      {
        fprintf (stdout, "hddscerror: failed to get dmde coupon\n");
        send_report_email("hddscerror: failed to get dmde coupon", "hddscerror: failed to get dmde coupon", "messagebody.txt", message_items.body, -1);
        return -1;
      }
    }
    char subject[256];
    strcpy (subject, "HDDSuperClone 1 Day Activation License");
    char message_to_send[MESSAGE_SIZE];
    strcpy (message_to_send, "Thank you for purchasing HDDSuperClone.\n\n"
    "Your 1 day activation license is attached, and also included as text in this email.\n"
    "Please keep this activation license safe and secure.\n"
    "Replacement licenses will only be sent to the original purchase email address, no exceptions.\n\n");

    if (DMDE_COUPONS_ENABLED)
    {
      strcpy (subject, "HDDSuperClone 1 Day Activation License With DMDE Coupon");
      strcat (message_to_send, "As a bonus you also get a discount coupon for DMDE.\n"
      "Please note that this coupon code may expire within 2 months of receipt if not used.\n"
      "More information about the DMDE coupon can be found at the following link:\n"
      "www.sdcomputingservice.com/hddsuperclone/dmde\n\n"
      "Your DMDE coupon code is:  ");
      strcat (message_to_send, coupon);
      strcat (message_to_send, "\n\n");
      free (coupon);
    }
    strcat (message_to_send, "Regards,\n"
    "Scott Dwyer\n\n"
    "In HDDSuperClone click on Help in the upper right corner, and choose Load Activation File.\n\n"
    "If you have a problem with the attachment, copy and paste the following text into a plain text file:\n\n");
    strcat (message_to_send, license_file);

    if (send_customer_email(body_items.address_name, body_items.payer_email, subject, message_to_send, license_name, license_file))
    {
      fprintf (stdout, "ERROR: Failed to send customer email.\n");
      send_report_email("hddscerror: Failed to send customer email", body_items.payer_email, "messagebody.txt", message_items.body, -1);
    }
  }

  else if (strcasecmp(body_items.item_name, HDDSHORT_ITEM_NAME) == 0 || strncasecmp(body_items.item_name, HDDSHORTSPECIAL_ITEM_NAME, 42) == 0)
  {
    char gross[LINE_LENGTH];
    strcpy (gross, body_items.mc_gross);
    if (gross[0] == '$')
    {
      gross[0] = ' ';
    }
    int price_paid = strtol(gross, NULL, 10);
    if (price_paid < HDDSHORT_PRICE)
    {
      fprintf (stdout, "Error: price paid too low %d %s\n", price_paid, body_items.mc_gross);
      send_report_email("hddscerror: hddshort price paid too low", body_items.mc_gross, "messagebody.txt", message_items.body, -1);
      if (strcasecmp(body_items.item_name, HDDSHORT_ITEM_NAME) == 0)
      {
        return -1;
      }
    }
    days = 65;
    type = 1;
    strcpy (expire_date, get_future_date_ccc(days));
    if (do_make_license_file (version, type, days))
    {
      fprintf (stdout, "hddscerror: failed to make license\n");
      send_report_email("hddscerror: failed to make license", "hddscerror: failed to make license", "messagebody.txt", message_items.body, -1);
      return -1;
    }
    char *coupon;
    if (DMDE_COUPONS_ENABLED)
    {
      coupon = get_dmde_coupon(DMDE_15USD_NAME);
      if (coupon == NULL)
      {
        fprintf (stdout, "hddscerror: failed to get dmde coupon\n");
        send_report_email("hddscerror: failed to get dmde coupon", "hddscerror: failed to get dmde coupon", "messagebody.txt", message_items.body, -1);
        return -1;
      }
    }
    char subject[256];
    strcpy (subject, "HDDSuperClone 60 Day Activation License");
    char message_to_send[MESSAGE_SIZE];
    strcpy (message_to_send, "Thank you for purchasing HDDSuperClone.\n\n"
    "Your 60 day activation license is attached, and also included as text in this email.\n"
    "Please keep this activation license safe and secure.\n"
    "Replacement licenses will only be sent to the original purchase email address, no exceptions.\n\n");

    if (DMDE_COUPONS_ENABLED)
    {
      strcpy (subject, "HDDSuperClone 60 Day Activation License With DMDE Coupon");
      strcat (message_to_send, "As a bonus you also get a discount coupon for DMDE.\n"
      "Please note that this coupon code may expire within 2 months of receipt if not used.\n"
      "More information about the DMDE coupon can be found at the following link:\n"
      "www.sdcomputingservice.com/hddsuperclone/dmde\n\n"
      "Your DMDE coupon code is:  ");
      strcat (message_to_send, coupon);
      strcat (message_to_send, "\n\n");
      free (coupon);
    }
    strcat (message_to_send, "Regards,\n"
    "Scott Dwyer\n\n"
    "In HDDSuperClone click on Help in the upper right corner, and choose Load Activation File.\n\n"
    "If you have a problem with the attachment, copy and paste the following text into a plain text file:\n\n");
    strcat (message_to_send, license_file);

    if (send_customer_email(body_items.address_name, body_items.payer_email, subject, message_to_send, license_name, license_file))
    {
      fprintf (stdout, "ERROR: Failed to send customer email.\n");
      send_report_email("hddscerror: Failed to send customer email", body_items.payer_email, "messagebody.txt", message_items.body, -1);
    }
  }

  else if (strcasecmp(body_items.item_name, HDDLONG_ITEM_NAME) == 0 || strncasecmp(body_items.item_name, HDDLONGSPECIAL_ITEM_NAME, 45) == 0)
  {
    char gross[LINE_LENGTH];
    strcpy (gross, body_items.mc_gross);
    if (gross[0] == '$')
    {
      gross[0] = ' ';
    }
    int price_paid = strtol(gross, NULL, 10);
    if (price_paid < HDDLONG_PRICE)
    {
      fprintf (stdout, "Error: price paid too low %d %s\n", price_paid, body_items.mc_gross);
      send_report_email("hddscerror: hddlong price paid too low", body_items.mc_gross, "messagebody.txt", message_items.body, -1);
      if (strcasecmp(body_items.item_name, HDDLONG_ITEM_NAME) == 0)
      {
        return -1;
      }
    }
    days = 0;
    type = 2;
    strcpy (expire_date, get_future_date_ccc(days));
    if (do_make_license_file (version, type, days))
    {
      fprintf (stdout, "hddscerror: failed to make license\n");
      send_report_email("hddscerror: failed to make license", "hddscerror: failed to make license", "messagebody.txt", message_items.body, -1);
      return -1;
    }
    char *coupon;
    if (DMDE_COUPONS_ENABLED)
    {
      coupon = get_dmde_coupon(DMDE_50PERC_NAME);
      if (coupon == NULL)
      {
        fprintf (stdout, "hddscerror: failed to get dmde coupon\n");
        send_report_email("hddscerror: failed to get dmde coupon", "hddscerror: failed to get dmde coupon", "messagebody.txt", message_items.body, -1);
        return -1;
      }
    }
    char subject[256];
    strcpy (subject, "HDDSuperClone Extended Long Term Activation License");
    char message_to_send[MESSAGE_SIZE];
    strcpy (message_to_send, "Thank you for purchasing HDDSuperClone.\n\n"
    "Your extended long term activation license is attached, and also included as text in this email.\n"
    "Please keep this activation license safe and secure.\n"
    "Replacement licenses will only be sent to the original purchase email address, no exceptions.\n\n");

    if (DMDE_COUPONS_ENABLED)
    {
      strcpy (subject, "HDDSuperClone Extended Long Term Activation License With DMDE Coupon");
      strcat (message_to_send, "As a bonus you also get a discount coupon for DMDE.\n"
      "Please note that this coupon code may expire within 2 months of receipt if not used.\n"
      "More information about the DMDE coupon can be found at the following link:\n"
      "www.sdcomputingservice.com/hddsuperclone/dmde\n\n"
      "Your DMDE coupon code is:  ");
      strcat (message_to_send, coupon);
      strcat (message_to_send, "\n\n");
      free (coupon);
    }
    strcat (message_to_send, "Regards,\n"
    "Scott Dwyer\n\n"
    "In HDDSuperClone click on Help in the upper right corner, and choose Load Activation File.\n\n"
    "If you have a problem with the attachment, copy and paste the following text into a plain text file:\n\n");
    strcat (message_to_send, license_file);

    if (send_customer_email(body_items.address_name, body_items.payer_email, subject, message_to_send, license_name, license_file))
    {
      fprintf (stdout, "ERROR: Failed to send customer email.\n");
      send_report_email("hddscerror: Failed to send customer email", body_items.payer_email, "messagebody.txt", message_items.body, -1);
    }
  }

  else
  {
    fprintf (stdout, "Invalid item ID %s.\n", body_items.item_name);
    send_report_email("hddscerror: Invalid item ID", body_items.item_name, NULL, NULL, -1);
    return -1;
  }

  char name[64];
  sprintf (name, "%016llx", (unsigned long long)server_license_id);
  set_raw_csv_data(name, version, type, days, expire_date);
  set_short_csv_data(name, version, type, days, expire_date);
  append_data_to_file(RAW_CSV_FILE_NAME, csv_raw_data);
  append_data_to_file(SHORT_CSV_FILE_NAME, csv_short_data);

  archive_serverp();

  return 0;
}





int archive_serverp(void)
{
  get_the_time_ccc();
  system("rm -f license_files.7z");
  if (system("7z a license_files.7z license_files -t7z -pmakeyourownpassword > /dev/null"))
  {
    fprintf (stdout, "Error compressing license files\n");
  }
  char *archive_file;
  int archive_file_size = 0;
  FILE *readfile = fopen("license_files.7z", "rb");
  if (readfile == NULL)
  {
    fprintf (stdout, "Cannot open %s for reading (%s).\n", "license_files.7z", strerror(errno));
    send_report_email("hddscerror: failed to read license archive", "license_files.7z", NULL, NULL, -1);
    return -1;
  }
  else
  {
    fseek(readfile, 0, SEEK_END);
    long fsize = ftell(readfile);
    fseek(readfile, 0, SEEK_SET);
    archive_file = malloc(fsize);
    fread(archive_file, 1, fsize, readfile);
    fclose(readfile);
    archive_file_size = fsize;
  }

  char archive_name[64];
  sprintf (archive_name, "license_files_%s.7z", current_date_ccc);
  send_report_email("hddscreport: archive data", archive_name, archive_name, archive_file, archive_file_size);
  free(archive_file);
  return 0;
}





int do_make_license_file (int version, int type, int days)
{
  int ret = make_license_files_ccc (version, type, days, 1);
  if (ret)
  {
    char temp[8];
    sprintf (temp, "%d", ret);
    send_report_email("hddscerror: failed to create license", temp, "messagebody.txt", message_items.body, -1);
    return -1;
  }

  system("mv license_files/new/* license_files/active/");

  memset (license_name, 0, sizeof(license_name));
  sprintf (license_name, "%016llx", (unsigned long long)server_license_id);
  strcat(license_name, ".txt");
  fprintf(stdout, "made license %s\n", license_name);
  process_active_licenses_ccc();

  system("./ftp.sh");

  memset (license_file, 0, sizeof(license_file));

  DIR *d;
  struct dirent *dir;
  d = opendir("license_files/private");
  if (d)
  {
    int success = 0;
    while ((dir = readdir(d)) != NULL)
    {
      if (dir->d_type == DT_REG)
      {
        if (strcasecmp(dir->d_name, license_name) == 0)
        {
          printf("found match %s\n", dir->d_name);
          char full_name[384];
          sprintf (full_name, "license_files/private/%s", dir->d_name);

          FILE *readfile = fopen(full_name, "rb");
          if (readfile == NULL)
          {
            fprintf (stdout, "Cannot open %s for reading (%s).\n", full_name, strerror(errno));
            send_report_email("hddscerror: failed to read license", full_name, "messagebody.txt", message_items.body, -1);
            return -1;
          }
          else
          {
            fseek(readfile, 0, SEEK_END);
            long fsize = ftell(readfile);
            fseek(readfile, 0, SEEK_SET);
            fread(license_file, 1, fsize, readfile);
            fclose(readfile);
            license_file[fsize] = 0;
          }

          success = 1;
          break;
        }
      }
    }
    closedir(d);
    if (!success)
    {
      fprintf (stdout, "hddscerror: failed to find license (%s).\n", license_name);
      send_report_email("hddscerror: failed to find license", license_name, "messagebody.txt", message_items.body, -1);
      return -1;
    }
  }
  else
  {
    fprintf (stdout, "hddscerror: failed to open license directory.\n");
    send_report_email("hddscerror: failed to open license directory", license_name, "messagebody.txt", message_items.body, -1);
    return -1;
  }
  return 0;
}





int send_report_email(char *subject, char *body, char *attachment_name, char *attachment_body, int body_size)
{
  char *user = REPORT_EMAIL_USER;
  char *pass = REPORT_EMAIL_PASS;
  char *from = REPORT_EMAIL_FROM;
  char *from_name = REPORT_EMAIL_FROM_NAME;
  char *to = REPORT_EMAIL_TO;
  char *to_name = REPORT_EMAIL_TO_NAME;
  char *server = REPORT_EMAIL_SERVER;
  char *port = REPORT_EMAIL_PORT;
  unsigned char security = REPORT_MAIL_SECURITY;
  unsigned char flags = REPORT_MAIL_FLAGS;
  unsigned char auth = REPORT_MAIL_AUTH;

  struct smtp *smtp;
  int rc;
  rc = smtp_open_ccc(server, port, security, flags, NULL, &smtp);
  rc = smtp_auth_ccc(smtp, auth, user, pass);
  rc = smtp_address_add_ccc(smtp, SMTP_ADDRESS_FROM, from, from_name);
  rc = smtp_address_add_ccc(smtp, SMTP_ADDRESS_TO, to, to_name);
  rc = smtp_header_add_ccc(smtp, "Subject", subject);
  if (attachment_name)
  {
    rc = smtp_attachment_add_mem_ccc(smtp, attachment_name, attachment_body, body_size);
  }
  rc = smtp_mail_ccc(smtp, body);
  rc = smtp_close_ccc(smtp);
  if(rc != SMTP_STATUS_OK)
  {
    fprintf(stdout, "smtp failed: %s\n", smtp_status_code_errstr_ccc(rc));
    return -1;
  }
  return 0;
}





int send_customer_email(char *name, char *email, char *subject, char *body, char *attachment_name, char *attachment_body)
{
  char *user = REPORT_EMAIL_USER;
  char *pass = REPORT_EMAIL_PASS;
  char *from = REPORT_EMAIL_FROM;
  char *from_name = REPORT_EMAIL_FROM_NAME;
  char *to = email;
  char *to_name = name;
  char *cc = REPORT_EMAIL_TO;
  char *cc_name = REPORT_EMAIL_TO_NAME;
  char *server = REPORT_EMAIL_SERVER;
  char *port = REPORT_EMAIL_PORT;
  unsigned char security = REPORT_MAIL_SECURITY;
  unsigned char flags = REPORT_MAIL_FLAGS;
  unsigned char auth = REPORT_MAIL_AUTH;

  struct smtp *smtp;
  int rc;
  rc = smtp_open_ccc(server, port, security, flags, NULL, &smtp);
  rc = smtp_auth_ccc(smtp, auth, user, pass);
  rc = smtp_address_add_ccc(smtp, SMTP_ADDRESS_FROM, from, from_name);
  rc = smtp_address_add_ccc(smtp, SMTP_ADDRESS_TO, to, to_name);
  rc = smtp_address_add_ccc(smtp, SMTP_ADDRESS_CC, cc, cc_name);
  rc = smtp_header_add_ccc(smtp, "Subject", subject);
  if (attachment_name)
  {
    rc = smtp_attachment_add_mem_ccc(smtp, attachment_name, attachment_body, -1);
  }
  rc = smtp_mail_ccc(smtp, body);
  rc = smtp_close_ccc(smtp);
  if(rc != SMTP_STATUS_OK)
  {
    fprintf(stdout, "smtp failed: %s\n", smtp_status_code_errstr_ccc(rc));
    return -1;
  }
  return 0;
}





char *get_dmde_coupon(char *name)
{
  char dmde_coupon_code[MAX_DMDE_COUPONS][32];
  char dmde_coupon_date[MAX_DMDE_COUPONS][32];
  memset (dmde_coupon_code, 0, sizeof(dmde_coupon_code));
  memset (dmde_coupon_date, 0, sizeof(dmde_coupon_date));
  int count = 0;
  int found_coupon = 0;
  int coupon_location = 0;
  char *coupon_code = malloc(32);
  FILE *readfile = fopen(name, "r");
  if (readfile == NULL)
  {
    fprintf (stdout, "Cannot open %s for reading (%s).\n", name, strerror(errno));
    return NULL;
  }
  char line[LINE_LENGTH];
  while (fgets(line, sizeof line, readfile))
  {
    int scan = sscanf(line, "%s %s", dmde_coupon_code[count], dmde_coupon_date[count]);
    //fprintf (stdout, "%s\n", line);
    //fprintf (stdout, "%s %s\n", dmde_coupon_code[count], dmde_coupon_date[count]);
    if (scan == 1 && strcmp(dmde_coupon_date[count], "") == 0 && !found_coupon)
    {
      found_coupon = 1;
      coupon_location = count;
      strcpy (coupon_code, dmde_coupon_code[count]);
      fprintf (stdout, "found coupon %s\n", coupon_code);
    }
    count++;
  }
  fclose (readfile);

  if (found_coupon)
  {
    char newname[128];
    sprintf (newname, "%s.temp", name);

    FILE *writefile = fopen(newname, "w");
    if (writefile == NULL)
    {
      fprintf (stdout, "Cannot open %s for writing (%s).\n", newname, strerror(errno));
      return NULL;
    }
    int i;
    for (i = 0; i < count; i++)
    {
      int ret;
      if (i == coupon_location)
      {
        ret = fprintf (writefile, "%s %s\n", dmde_coupon_code[i], current_date_ccc);
      }
      else
      {
        ret = fprintf (writefile, "%s %s\n", dmde_coupon_code[i], dmde_coupon_date[i]);
      }
      if (ret < 0)
      {
        fprintf (stdout, "Error writing to %s.\n", newname);
        fclose(writefile);
        return NULL;
      }
    }
    fclose(writefile);

    char command[256];
    sprintf (command, "mv -f %s %s", newname, name);
    system(command);

    strcpy(body_items.dmde_coupon_code, coupon_code);

    return coupon_code;
  }

  return NULL;
}





int process_manual_file(char *full_name)
{
  memset (&body_items, 0, sizeof(body_items));
  FILE *readfile = fopen(full_name, "r");
  if (readfile == NULL)
  {
    fprintf (stdout, "Cannot open %s for reading (%s).\n", full_name, strerror(errno));
    return -1;
  }
  else
  {
    char line[DATA_LINE_LENGTH];
    int get_name = 0;
    int get_email = 0;
    int get_address = 0;
    int address_count = 0;
    int get_item_name = 0;
    int get_date = 0;
    char address[6][DATA_LINE_LENGTH];
    memset(address, 0, sizeof(address));
    while (fgets(line, sizeof line, readfile))
    {
      if (get_item_name)
      {
        get_item_name = 0;
        char raw_item_name[DATA_LINE_LENGTH] = {0};
        sscanf(line, "%[^\n]", raw_item_name);
        strcpy(body_items.item_name, raw_item_name);
      }
      else if (get_date)
      {
        get_date = 0;
        char raw_date[DATA_LINE_LENGTH] = {0};
        sscanf(line, "%[^\n]", raw_date);
        strcpy(body_items.payment_date, raw_date);
      }
      else if (get_address)
      {
        char raw1[DATA_LINE_LENGTH] = {0};
        char raw2[DATA_LINE_LENGTH] = {0};
        sscanf(line, "%s %s", raw1, raw2);
        if (strcasecmp(raw1, "Shipping") == 0 && strcasecmp(raw2, "details") == 0)
        {
          get_address = 0;
        }
        else
        {
          char raw_address[DATA_LINE_LENGTH] = {0};
          sscanf(line, "%[^\n]", raw_address);
          strcpy(address[address_count], raw_address);
          if (address_count >= 5)
          {
            get_address = 0;
          }
          address_count++;
        }
        if (!get_address)
        {
          strcpy(body_items.address_name, address[0]);

          strcpy(body_items.address_street, "\"");
          strcat(body_items.address_street, address[1]);
          strcat(body_items.address_street, "\"");

          strcpy(body_items.address_city, "\"");
          strcat(body_items.address_city, address[2]);
          strcat(body_items.address_city, "\"");

          strcpy(body_items.address_state, "\"");
          strcat(body_items.address_state, address[3]);
          strcat(body_items.address_state, "\"");

          strcpy(body_items.address_zip, "\"");
          strcat(body_items.address_zip, address[4]);
          strcat(body_items.address_zip, "\"");

          strcpy(body_items.address_country, "\"");
          strcat(body_items.address_country, address[address_count-1]);
          strcat(body_items.address_country, "\"");
        }
      }
      else if (get_email)
      {
        get_email = 0;
        char raw_email[DATA_LINE_LENGTH] = {0};
        sscanf(line, "%s", raw_email);
        strcpy(body_items.payer_email, raw_email);
      }
      else if (get_name)
      {
        get_name = 0;
        get_email = 1;
        char raw_first_name[DATA_LINE_LENGTH] = {0};
        char raw_last_name[DATA_LINE_LENGTH] = {0};
        sscanf(line, "%s %s", raw_first_name, raw_last_name);
        strcpy(body_items.first_name, raw_first_name);
        strcpy(body_items.last_name, raw_last_name);
      }
      else if (strncasecmp(line, "Transaction ID:", 15) == 0)
      {
        char raw1[DATA_LINE_LENGTH];
        char raw2[DATA_LINE_LENGTH];
        char raw_id[DATA_LINE_LENGTH] = {0};
        sscanf(line, "%s %s %s", raw1, raw2, raw_id);
        strcpy(body_items.txn_id, raw_id);
      }
      else if (strncasecmp(line, "Date:", 5) == 0)
      {
        get_date = 1;
      }
      else if (strncasecmp(line, "Buyer", 5) == 0)
      {
        get_name = 1;
      }
      else if (strncasecmp(line, "Shipping address", 16) == 0)
      {
        get_address = 1;
      }
      else if (strncasecmp(line, "Description", 11) == 0)
      {
        get_item_name = 1;
      }
      else if (strncasecmp(line, "Total", 5) == 0)
      {
        char raw1[DATA_LINE_LENGTH];
        char raw_total[DATA_LINE_LENGTH] = {0};
        char raw_currency[DATA_LINE_LENGTH] = {0};
        sscanf(line, "%s %s %s", raw1, raw_total, raw_currency);
        strcpy(body_items.mc_gross, raw_total);
        strcpy(body_items.mc_currency, raw_currency);
      }
    }
    fclose (readfile);
  }
  return 0;
}





int check_duplicate_id(void)
{
  if (CHECK_DUPLICATE_ID)
  {
    // check for duplicate transaction id
    FILE *readfile = fopen(RAW_CSV_FILE_NAME, "r");
    if (readfile == NULL)
    {
      fprintf (stdout, "Cannot open %s for reading (%s).\n", RAW_CSV_FILE_NAME, strerror(errno));
    }
    else
    {
      char line[DATA_LINE_LENGTH];
      while (fgets(line, sizeof line, readfile))
      {
        char raw_date[DATA_LINE_LENGTH];
        char raw_id[DATA_LINE_LENGTH];
        char rest_of_line[DATA_LINE_LENGTH];
        strcpy (raw_date, "");
        strcpy (raw_id, "");
        strcpy (rest_of_line, "");
        sscanf(line, "%[^,],%[^,],%[^\n]", raw_date, raw_id, rest_of_line);
        //fprintf (stdout, "%s\n", line);
        fprintf (stdout, "%s  %s\n", raw_date, raw_id);
        if (strcasecmp(raw_id, body_items.txn_id) == 0)
        {
          fprintf (stdout, "Duplicate transaction id found %s\n", body_items.txn_id);
          fclose (readfile);
          return 1;
        }
      }
      fclose (readfile);
    }
  }
  return 0;
}




int import_transaction_files(void)
{
  DIR *d;
  struct dirent *dir;
  d = opendir("license_files/manual_import");
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      if (dir->d_type == DT_REG)
      {
        printf("%s\n", dir->d_name);
        char full_name[384];
        sprintf (full_name, "license_files/manual_import/%s", dir->d_name);
        if (process_manual_file(full_name))
        {
          fprintf (stdout, "hddscerror: failed to import (%s).\n", full_name);
          closedir(d);
          return -1;
        }
        if (check_duplicate_id())
        {
          fprintf (stdout, "hddscerror: duplicate id (%s).\n", full_name);
          closedir(d);
          return -1;
        }

        fprintf (stdout, "txn_id=%s\n", body_items.txn_id);
        fprintf (stdout, "first_name=%s\n", body_items.first_name);
        fprintf (stdout, "last_name=%s\n", body_items.last_name);
        fprintf (stdout, "payer_email=%s\n", body_items.payer_email);
        fprintf (stdout, "address_name=%s\n", body_items.address_name);
        fprintf (stdout, "address_street=%s\n", body_items.address_street);
        fprintf (stdout, "address_city=%s\n", body_items.address_city);
        fprintf (stdout, "address_state=%s\n", body_items.address_state);
        fprintf (stdout, "address_zip=%s\n", body_items.address_zip);
        fprintf (stdout, "address_country=%s\n", body_items.address_country);
        fprintf (stdout, "item_name=%s\n", body_items.item_name);
        fprintf (stdout, "mc_gross=%s\n", body_items.mc_gross);
        fprintf (stdout, "mc_currency=%s\n", body_items.mc_currency);
        fprintf (stdout, "payment_date=%s\n", body_items.payment_date);

        char input_text[32];
        char raw_value[32];
        fprintf (stdout, "Process license? ");
        fflush(stdout);
        if ( fgets(input_text, sizeof input_text, stdin) != NULL )
        {
          sscanf(input_text, "%s", raw_value);
          if (strcasecmp(raw_value, "y") == 0)
          {
            manual_create = 1;
            if (process_payment())
            {
              fprintf (stdout, "hddscerror: failed to process payment (%s).\n", full_name);
              closedir(d);
              return -1;
            }
          }
        }
      }
    }
    closedir(d);
  }
  return 0;
}





int send_serverp_command(char *command)
{
#ifdef LISTEN_HTTPS
  SSL_CTX *ctx;
  int server;
  SSL *ssl;
  static char buf[1024*1024];
  int bytes;
  char *hostname = "localhost";
  uint16_t portnum = 443;
  struct hostent *host;
  struct sockaddr_in addr;
  const SSL_METHOD *method;

  // Initialize the SSL library
  SSL_library_init ();

  OpenSSL_add_all_algorithms ();     /* Load cryptos, et.al. */
  SSL_load_error_strings ();         /* Bring in and register error messages */
  method = TLSv1_2_client_method (); /* Create new client-method instance */
  ctx = SSL_CTX_new (method);        /* Create new context */
  if (ctx == NULL)
  {
    ERR_print_errors_fp (stderr);
    LOCAL_ABORT ();
  }

  if ((host = gethostbyname (hostname)) == NULL)
  {
    perror (hostname);
    LOCAL_ABORT ();
  }
  server = socket (PF_INET, SOCK_STREAM, 0);
  bzero (&addr, sizeof (addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons (portnum);
  addr.sin_addr.s_addr = * (long*) (host->h_addr);

  if (connect (server, (struct sockaddr*)&addr, sizeof (addr)) != 0)
  {
    close (server);
    perror (hostname);
    fprintf (stderr, "Is the server running, and on the correct port (%d)?\n", portnum);
    LOCAL_ABORT ();
  }

  ssl = SSL_new (ctx);      /* create new SSL connection state */
  SSL_set_fd (ssl, server);    /* attach the socket descriptor */
  if (SSL_connect (ssl) <= 0)   /* perform the connection */
  {
    ERR_print_errors_fp (stderr);
  }
  else
  {
    X509 *cert;
    char *line;
    char szRequest[4096];
    strcpy (szRequest, command);

    printf ("Sending:\n[%s]\n", szRequest);

    printf ("\n\nConnected with %s encryption\n", SSL_get_cipher (ssl));

    cert = SSL_get_peer_certificate (ssl); /* get the server's certificate */
    if (cert != NULL)
    {
      printf ("Server certificates:\n");
      line = X509_NAME_oneline (X509_get_subject_name (cert), 0, 0);
      printf ("Subject: %s\n", line);
      free (line);       /* free the malloc'ed string */
      line = X509_NAME_oneline (X509_get_issuer_name (cert), 0, 0);
      printf ("Issuer: %s\n\n", line);
      free (line);       /* free the malloc'ed string */
      X509_free (cert);  /* free the malloc'ed certificate copy */
    }
    else
    {
      printf ("Info: No client certificates configured.\n");
    }

    SSL_write (ssl, szRequest, strlen (szRequest));   /* encrypt & send message */

    bytes = SSL_read (ssl, buf, sizeof (buf)); /* get reply & decrypt */
    buf[bytes] = 0;
    printf ("Received (%d bytes):\n[%s]\n", bytes, buf);

    SSL_free (ssl);        /* release connection state */
  }

  close (server);          /* close socket */
  SSL_CTX_free (ctx);      /* release context */

  #else
  int server;
  static char buf[1024*1024];
  int bytes;
  char *hostname = "localhost";
  uint16_t portnum = 80;
  struct hostent *host;
  struct sockaddr_in addr;

  // Initialize the SSL library
  SSL_library_init ();

  if ((host = gethostbyname (hostname)) == NULL)
  {
    perror (hostname);
    LOCAL_ABORT ();
  }
  server = socket (PF_INET, SOCK_STREAM, 0);
  bzero (&addr, sizeof (addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons (portnum);
  addr.sin_addr.s_addr = * (long*) (host->h_addr);

  if (connect (server, (struct sockaddr*)&addr, sizeof (addr)) != 0)
  {
    close (server);
    perror (hostname);
    fprintf (stderr, "Is the server running, and on the correct port (%d)?\n", portnum);
    LOCAL_ABORT ();
  }
  char szRequest[4096];
  strcpy (szRequest, command);
  printf ("Sending:\n[%s]\n", szRequest);
  send(server, szRequest, strlen (szRequest) , 0 );

  bytes = read(server, buf, 1024);
  printf ("Received (%d bytes):\n[%s]\n", bytes, buf);

  close (server);          /* close socket */

  #endif
  return 0;
}





int report_serverp(void)
{
  get_the_time_ccc();
  char name[64];
  sprintf (name, "tailp_%s", current_date_ccc);
  char log_name[92];
  sprintf (log_name, "%s.log", name);
  char zip_name[128];
  sprintf (zip_name, "%s.7z", log_name);
  char command[256];
  sprintf (command, "tail -n10000 serverp.log > %s", log_name);
  system(command);
  sprintf (command, "7z a %s %s -t7z > /dev/null", zip_name, log_name);
  if (system(command))
  {
    fprintf (stdout, "Error compressing log file\n");
  }
  sprintf (command, "rm -f %s", log_name);
  system(command);
  char *archive_file;
  int archive_file_size = 0;
  FILE *readfile = fopen(zip_name, "rb");
  if (readfile == NULL)
  {
    fprintf (stdout, "Cannot open %s for reading (%s).\n", zip_name, strerror(errno));
    send_report_email("hddscerror: failed to read tailp log", zip_name, NULL, NULL, -1);
    return -1;
  }
  else
  {
    fseek(readfile, 0, SEEK_END);
    long fsize = ftell(readfile);
    fseek(readfile, 0, SEEK_SET);
    archive_file = malloc(fsize);
    fread(archive_file, 1, fsize, readfile);
    fclose(readfile);
    archive_file_size = fsize;
  }

  send_report_email("hddscreport: log data", zip_name, zip_name, archive_file, archive_file_size);
  free(archive_file);
  sprintf (command, "rm -f %s", zip_name);
  system(command);

  return 0;
}





#endif







