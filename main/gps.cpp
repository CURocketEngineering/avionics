/*
 * Definition file for gps
 */

/* LIBRARIES */
#include <Arduino.h>

#include "pins.h"
#include "gps.h"
#include "util.h"
#include "datalog.h"

struct gps_s gps;

void gps_init() {
     Serial1.begin(9600);
}

void gps_read() {
     static char sentence[80];
     static char field[20];

     while (Serial1.available()) {
          const char * ptr = sentence;

          util_readLine(Serial1, sentence, sizeof(sentence));

          // ID
          ptr = util_getField(ptr, field, sizeof(field));
	  //datalog_write(sentence);
	  char test54[20];
	  

          if (strcmp(field, "$GPRMC") == 0) {
               // Time
               ptr = util_getField(ptr, field, sizeof(field));
               long time = atoi(field);
               gps.hour = time/10000 % 100;
               gps.min = time/100 % 100;
               gps.sec = time % 100;

               // Status
               ptr = util_getField(ptr, field, sizeof(field));

               // Latitude
               ptr = util_getField(ptr, field, sizeof(field));
               gps.lat = atof(field)/100.0;
	       sprintf(test54,"%f",atof(field));
	       //test54 << atof(field)/100.0;
	       //datalog_write(test54);

               // N/S
               ptr = util_getField(ptr, field, sizeof(field));
               if (field[0] == 'S') {
                    gps.lat = -gps.lat;
               }

               // Longitude
               ptr = util_getField(ptr, field, sizeof(field));
               gps.lon = atof(field)/100.0;

               // E/W
               ptr = util_getField(ptr, field, sizeof(field));
               if (field[0] == 'W') {
                    gps.lon = -gps.lon;
               }

               // ?
               ptr = util_getField(ptr, field, sizeof(field));

               // ?
               ptr = util_getField(ptr, field, sizeof(field));

               // Date
               ptr = util_getField(ptr, field, sizeof(field));
               long date = atoi(field);
               gps.day = date/10000 % 100;
               gps.mon = date/100 % 100;
               gps.year = 2000 + date % 100;

               // ?
               ptr = util_getField(ptr, field, sizeof(field));

               // ?
               ptr = util_getField(ptr, field, sizeof(field));

               // ?
               ptr = util_getField(ptr, field, sizeof(field));
          }
     }
}
