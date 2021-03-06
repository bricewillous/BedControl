#ifndef ARDUINO_H
#define ARDUINO_H

#include <IRremote.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <DFRobotDFPlayerMini.h>
#include <EEPROM.h>
#include <SD.h>

// ************************************************************************** //
// ************************* Constants declarations ************************* //
// ************************************************************************** //

// **************************************** //
// **************** Global **************** //
// **************************************** //

// Debug levels
const int LEVEL_DEBUG = 2;
const int LEVEL_INFO  = 1;
const int LEVEL_ERROR = 0;

const boolean SERIAL_LOG_ENABLED = true;        // Serial logging
const int SERIAL_LOG_LEVEL       = LEVEL_DEBUG; // Serial log level
const boolean SD_LOG_ENABLED     = true;        // SD logging
const int SD_LOG_LEVEL           = LEVEL_INFO;  // SD logging level

const long ESP_BAUD_RATE   = 9600;   // ESP8266 communication baud rate
const long DEBUG_BAUD_RATE = 250000; // Debug baud rate
const long DFP_BAUD_RATE   = 9600;   // DFPlayer communication baud rate

const boolean WAIT_FOR_TIME         = true;  // If we have to wait for time sync (if true, program will not start until time is synced)
const boolean INFRARED_ENABLED      = true;  // If we allow infrared communication
const boolean SOUND_ENABLED         = false; // Enable sound
const boolean CLAP_ENABLED          = false; // Enable double claping
const boolean PRAYER_ALARM_ENABLED  = false; // Enable prayer alarms
const boolean MORNING_ALARM_ENABLED = true;  // Enable morning alarm

// Pins
const int PIN_SOUND        = 2;  // Sound detector IN pin
const int PIN_LED_INFRARED = 5;  // Infrared IN pin
const int PIN_LED_RED      = 11; // Red LED OUT pin
const int PIN_LED_GREEN    = 7;  // Green LED OUT pin
const int PIN_LED_BLUE     = 4;  // Blue LED OUT pin
const int PIN_SD_CS        = 49; // SD Chip Select

// ************************************* //
// **************** IDs **************** //
// ************************************* //

// Serial reception types
const int TYPE_RTM = -2; // Request : Time
const int TYPE_RIF = -1; // Request : Info
const int TYPE_UNK = 0;  // Unknown type
const int TYPE_TIM = 1;  // Provide : Time
const int TYPE_RGB = 2;  // Provide : RGB
const int TYPE_ONF = 3;  // Provide : On
const int TYPE_POW = 4;  // Provide : Power
const int TYPE_MOD = 5;  // Provide : Mode
const int TYPE_PRT = 6;  // Provide : Prayer time
const int TYPE_SPE = 7;  // Provide : Speed

// Serial reception errors
const int ERR_NOE = 0; // No error
const int ERR_OOB = 1; // Out of bound
const int ERR_UKM = 2; // Unknown mode
const int ERR_UKR = 3; // Unknown request
const int ERR_UKP = 4; // Unknown prayer

// Modes
const int MODE_MIN     = 0; // -Minimum mode value-
const int MODE_DEFAULT = 0; // Default mode
const int MODE_FLASH   = 1; // Flash mode
const int MODE_STROBE  = 2; // Strobe mode
const int MODE_FADE    = 3; // Fade mode
const int MODE_SMOOTH  = 4; // Smooth mode
const int MODE_WAKEUP  = 5; // Wake up mode
const int MODE_MAX     = 5; // -Maximum mode value-
const int N_MODE       = 6; // --Number of different modes--

// Caps
const int CAPS_NONE  = 0; // All letters in lower case
const int CAPS_FIRST = 1; // First letter in upper case
const int CAPS_ALL   = 2; // All letters in upper case

// ************************************************************* //
// **************** Default, min and max values **************** //
// ************************************************************* //

// Default
const unsigned long DEFAULT_RGB[N_MODE] =
{ 0xFFFFFF, 0xFF0000, 0xFFFFFF, 0xFFFFFF, 0xFF0000, 0x0000FF };    // Default color on program startup
const float DEFAULT_POWER[N_MODE] = { 50, 100, 100, 100, 100, 0 }; // Default power on program startup
const int DEFAULT_SPEED[N_MODE] = { -1, 0, 0, 750, 800, 2000 };    // Default speed on program startup
const int DEFAULT_VOLUME = 30;                                     // DFPlayer default volume (0 - 30)

// Min and Max
const int MIN_SPEED[N_MODE] = { -1, 1, 1, 50, 10, 1 };         // Minimum speed or power value for each mode
const int MAX_SPEED[N_MODE] = { -1, 25, 25, 600, 1000, 1000 }; // Maximum speed or power value for each mode
const int MIN_POWER   = 0;                                     // Minimum power value
const int MAX_POWER   = 255;                                   // Maximum power value
const int SEEKBAR_MIN = 0;                                     // Minimum app seek bars value
const int SEEKBAR_MAX = 100;                                   // Maximum app seek bars value
const int MIN_VOLUME  = 0;                                     // Minimum DFPlayer volume
const int MAX_VOLUME  = 30;                                    // Maximum DFPlayer volume

// *************************************** //
// **************** Other **************** //
// *************************************** //

// Wake up
const int WAKEUP_HOURS   = 06;
const int WAKEUP_MINUTES = 30;
const int WAKEUP_SECONDS = 00;

// Prayer
const int PRAYER_FADE_SPEED         = 97; // Fade speed for prayer time
const int PRAYER_ALERT_DURATION     = 10; // Prayer alert duration (in minutes)
const int N_PRAYER                  = 6;  // Number of different prayer (including sunrise)
const String PRAYERS_NAME[N_PRAYER] = { "Fajr", "Sunrise", "Dhuhr", "Asr", "Maghrib", "Isha" };

// EEPROM
const int EEPROM_START      = 0x0000; // EEPROM start address
const byte EEPROM_TEST_BYTE = 42;     // Byte I use to know if EEPROM has been initialized or not

// Infrared
const int N_COLOR        = 16; // Number of different colors
const int IR_CHANGE_STEP = 5;  // increasion or decreasion step value for infrared
const int MIN_IR_POWER   = 5;  // Minimum value in wich the power can go with infrared

// Sound
const int MIN_CLAP_DURATION          = 30;   // Amount of time to ignore after clap started (To avoid sound bounce)
const int MAX_CLAP_DURATION          = 100;  // Maximum clap duration in ms
const int MIN_TIME_BEETWIN_TWO_CLAPS = 400;  // Minimum time before starting second clap
const int MAX_TIME_BEETWIN_TWO_CLAPS = 600;  // Maximum time to start second clap
const int TIME_AFTER_START_OVER      = 1000; // Time to wait after double clap to start over

// ************************************************************************** //
// ************************* Variables declarations ************************* //
// ************************************************************************** //

// Global
boolean on;                  // If the leds are ON or OFF (True: ON / False: OFF)
boolean isInitialized;       // Set to true when program is complitely initialized
unsigned long rgb[N_MODE];   // Current RGB value for each mode (From 0x000000 to 0xFFFFFF)
int power[N_MODE];           // Current lightning power for each mode (from MINPOWER to MAXPOWER)
int speed[N_MODE];           // Current mode speed for each mode
unsigned char red[N_MODE];   // Current red value for each mode including lightning power (From 0 to 255)
unsigned char green[N_MODE]; // Current green value for each mode including lightning power (From 0 to 255)
unsigned char blue[N_MODE];  // Current blue value for each mode including lightning power (From 0 to 255)
unsigned char mode;          // Current lighting mode (MODE_***)

// Prayer
int prayerTime[N_PRAYER][3]; // [0] = Hours / [1] = Minutes / [2] = Hours * 60 + Minutes

// Read Claps
int clapState;              // Same as "state" but for claps
unsigned long endStateTime; // Time position at the end of a state (Allow time counting)

// Sound object declaration
DFRobotDFPlayerMini myDFPlayer;

// Infrared
IRrecv irrecv (PIN_LED_INFRARED);
decode_results results;
unsigned long lastIRCode; // IR code in previous loop - Allows continious power / strobe speed increasion / dicreasion
unsigned long IRCode;     // Current IR code

// Mode
int state;              // Current state used by some modes
unsigned long count;    // Delay counting
unsigned char lastMode; // Mode in previous loop - Allows mode initializations

// ******* Time Alarms ******* //
AlarmId morningAlarm;
AlarmId prayerStartAlarm[N_PRAYER];
AlarmId prayerStopAlarm;
AlarmId timeSyncTimer;

// Variable Change
boolean changeOn;
unsigned long changeRgb;
int changePower[N_MODE];
int changeSpeed[N_MODE];
unsigned char changeMode;

// SD Card
File logFile;
boolean logFileAvailable;
char sdFileName[13];

// ************************************************************************** //
// ************************** Functions prototypes ************************** //
// ************************************************************************** //

void setup ();
void loop ();
void initGlobal ();
void initDFPlayer ();
void initSdCard ();
boolean createLogFile ();
char * getLogFileName ();
void initAlarmsAndTimers ();
void initTimeSyncTimer ();
void initMorningAlarm ();
void initPrayerAlarms ();
void clearAlarms ();
void clearTimeSyncTimer ();
void clearMorningAlarm ();
void clearPrayerAlarms ();
void prayerStart ();
void prayerStop ();
void morningStartAlarm ();
void readClaps ();
void printPrefix (int debugLevel);
void printSdPrefix (int debugLevel);
void debugPrintDigits (int digits);
void sdPrintDigits (int digits);
void print (int debugLevel, const __FlashStringHelper * message, boolean = true);
void print (int debugLevel, const String &message, boolean prefix = true);
void print (int debugLevel, const char * message, boolean prefix = true);
void print (int debugLevel, unsigned char message, int base, boolean prefix = true);
void print (int debugLevel, int message, int base, boolean prefix = true);
void print (int debugLevel, unsigned int message, int base, boolean prefix = true);
void print (int debugLevel, long message, int base, boolean prefix = true);
void print (int debugLevel, unsigned long message, int base, boolean prefix = true);
void print (int debugLevel, double message, int base, boolean prefix = true);
void print (int debugLevel, const Printable &message, boolean prefix = true);
void println (int debugLevel, boolean prefix = true);
void println (int debugLevel, const __FlashStringHelper * message, boolean prefix = true);
void println (int debugLevel, const String &message, boolean prefix = true);
void println (int debugLevel, const char * message, boolean prefix = true);
void println (int debugLevel, unsigned char message, int base, boolean prefix = true);
void println (int debugLevel, int message, int base, boolean prefix = true);
void println (int debugLevel, unsigned int message, int base, boolean prefix = true);
void println (int debugLevel, long message, int base, boolean prefix = true);
void println (int debugLevel, unsigned long message, int base, boolean prefix = true);
void println (int debugLevel, double message, int base, boolean prefix = true);
void println (int debugLevel, const Printable &message, boolean prefix = true);
void decodeRequest (String request, long & result, int & infoMode, int & infotype, int & errorType);
void eepromDump (unsigned int start, unsigned int limit);
void eepromWrite ();
boolean eepromRead ();
void initInfrared ();
void readInfrared ();
void light ();
void rgb2color ();
void initModes ();
void action ();
void initModeFlash ();
void modeFlash ();
void initModeStrobe ();
void modeStrobe ();
void initModeFade ();
void modeFade ();
void initModeSmooth ();
void modeSmooth ();
void initModeWakeup ();
void modeWakeup ();
void initSerial ();
void waitForTime ();
void askForTime ();
void readSerial ();
String modeName (int mode, int caps);
String infoTypeName (int infoType, boolean shortened);
String errorTypeName (int infoType, boolean shortened);
String debugLevelName (int debugLevel);
String debugLevelSpace (int debugLevel);
void digitalClockDisplay (int debugLevel);
void printDigits (int debugLevel, int digits);
void softwareReset ();
int convertBoundaries (float input, float inMin, float inMax, float outMin, float outMax, boolean seekBarIsIn);
void testVariableChange ();
void initVariableChange ();
void sendInfo ();

#endif // ifndef ARDUINO_H
