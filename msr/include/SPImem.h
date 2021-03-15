#ifndef SPIMEM_H_
#define SPIMEM_H_

#include <Arduino.h>
#include "config.h"
#include <SPIFFS.h>

class SMEM{
  public:
    void init(void);
    String readFile(fs::FS &fs, const char * path);
    void writeFile(fs::FS &fs, const char * path, const char * message);

  private:

};

#endif
