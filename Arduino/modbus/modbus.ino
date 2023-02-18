#include <SoftwareSerial.h>

#include "modbus_slave.hpp"

using protocol::ModbusSlave;

ModbusSlave m(Serial, 115200, 1);

void setup() {
  m.setup();
}

void loop() {
  m.run();
 }
