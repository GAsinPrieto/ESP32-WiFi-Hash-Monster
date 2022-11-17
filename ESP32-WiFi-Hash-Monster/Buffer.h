#ifndef Buffer_h
#define Buffer_h

#include "Arduino.h"
#include "FS.h"
//#include "SD_MMC.h"
#ifdef ARDUINO_M5Stick_C
#include "SdFat.h"
#endif

#if defined ARDUINO_M5Stack_Core_ESP32
  #define BUF_BLOCKS 4
  #define EWH_MALLOC malloc
#else
  #define BUF_BLOCKS 24
  #if defined BOARD_HAS_PSRAM
    #define EWH_MALLOC ps_malloc
  #else
    #define EWH_MALLOC malloc
  #endif
#endif

#define BUF_SIZE BUF_BLOCKS * 1024
#define SNAP_LEN 2324 // max len of each recieved packet

extern bool useSD;

class Buffer {
  public:
    Buffer();
    bool init();
  #ifndef ARDUINO_M5Stick_C
    void checkFS(fs::FS* fs);
    bool open(fs::FS* fs);
    void close(fs::FS* fs);
    void save(fs::FS* fs);
    void forceSave(fs::FS* fs);
    void pruneZeroFiles(fs::FS* fs);
  #else
    void checkFS(SdFs* fs);
    bool open(SdFs* fs);
    void close(SdFs* fs);
    void save(SdFs* fs);
    void forceSave(SdFs* fs);
    void pruneZeroFiles(SdFs* fs);
  #endif
    void addPacket(uint8_t* buf, uint32_t len);    

  private:
    void write(int32_t n);
    void write(uint32_t n);
    void write(uint16_t n);
    void write(uint8_t* buf, uint32_t len);

    uint64_t micros64();

    uint8_t* bufA;
    uint8_t* bufB;

    uint32_t bufSizeA = 0;
    uint32_t bufSizeB = 0;

    bool writing = false; // acceppting writes to buffer
    bool useA = true; // writing to bufA or bufB
    bool saving = false; // currently saving onto the SD card

    char fileNameStr[32] = {0};
    const char *folderName = "/pcap"; // no trailing slash
    const char *fileNameTpl = "%s/%04X.pcap"; // hex is better for natural sorting, assume max 65536 files
  #ifndef ARDUINO_M5Stick_C
    File file;
  #else
    FsFile file;
  #endif

    uint32_t previous_micros = 0;
    uint32_t micros_high = 0;
};

#endif
