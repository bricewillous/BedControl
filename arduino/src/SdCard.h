#ifndef SDCARD_H
#define SDCARD_H

#include <SD.h>

const float FILE_CLOSE_TIME = 1.5;

class SdCard
{
public:
	SdCard();
	void init (int pin);
	File * getFile ();
	boolean isEnabled ();
	boolean fileIsOpened ();
	boolean fileIsClosed ();
	void openFile ();
	void closeFile ();

private:
	int pin;
	File logFile;
	boolean logFileAvailable, fileOpened;
	char sdFileName[13];
	unsigned long fileLastOpened;
	boolean enabled;
	long creationDate; // La date à laquelle le fichier a été créé

	boolean createLogFile ();
	void getLogFileName (char * buf);
};

extern SdCard sd;

#endif // ifndef SDCARD_H
