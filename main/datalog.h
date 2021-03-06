#ifndef DATALOG_H_
#define DATALOG_H_

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "json.h"

#include "pins.h"

void datalog_init();
void datalog_write(const String & str);

#endif
