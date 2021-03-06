#include "Utils.h"
#include "Time.h"
#include "Global.h"

const char * Utils::modName (int mod, int caps)
{
	switch (mod)
	{
		case MOD_DEFAULT:
			return caps == 0 ? "default" : caps == 1 ? "Default" : "DEFAULT";

		case MOD_FLASH:
			return caps == 0 ? "flash" : caps == 1 ? "Flash" : "FLASH";

		case MOD_STROBE:
			return caps == 0 ? "strobe" : caps == 1 ? "Strobe" : "STROBE";

		case MOD_FADE:
			return caps == 0 ? "fade" : caps == 1 ? "Fade" : "FADE";

		case MOD_SMOOTH:
			return caps == 0 ? "smooth" : caps == 1 ? "Smooth" : "SMOOTH";

		case MOD_DAWN:
			return caps == 0 ? "wake up" : caps == 1 ? "Wake up" : "WAKE UP";

		default:
			return caps == 0 ? "unknown" : caps == 1 ? "Unknown" : "UNKNOWN";
	}
}

const char * Utils::infoTypeName (int infoType, boolean shortened)
{
	switch (infoType)
	{
		case TYPE_TIM:
			return shortened ? "TIM" : "Time";

		case TYPE_RGB:
			return "RGB";

		case TYPE_ONF:
			return shortened ? "ONF" : "On";

		case TYPE_POW:
			return shortened ? "POW" : "Power";

		case TYPE_MOD:
			return shortened ? "MOD" : "Mod";

		case TYPE_PRT:
			return shortened ? "PRT" : "Prayer time";

		case TYPE_SPE:
			return shortened ? "SPE" : "Speed";

		default:
			return shortened ? "UNK" : "Unknown";
	}
}

const char * Utils::errorTypeName (int infoType, boolean shortened)
{
	switch (infoType)
	{
		case ERR_NOE:
			return "No error";

		case ERR_OOB:
			return shortened ? "Out of bounds" : "Error: Value is out of bounds";

		case ERR_UKM:
			return shortened ? "Unknowm mod" : "Error: Unknowm mod";

		case ERR_UKR:
			return shortened ? "Unknown request type" : "Error: Unknown request type";

		case ERR_UKP:
			return shortened ? "Unknown prayer" : "Error: Unknown prayer";

		default:
			return "Unknown error";
	}
}

// Requires a 23-char buffer
char * Utils::clock (char * buf)
{
	sprintf (buf, "%.2d/%.2d/%.4d %.2d:%.2d:%.2d:%.2ld", day(), month(), year(), hour(), minute(), second(), (millis() - now() * 1000) / 10);

	return buf;
}

void Utils::softwareReset () // Just in case
{
	asm volatile ("  jmp 0");
}

int Utils::convertBoundaries (float input, float inMin, float inMax, float outMin, float outMax)
{
	// Thanks to Alok Singhal from stackoverflow (https://stackoverflow.com/questions/5731863/mapping-a-numeric-range-onto-another/5732390#5732390)
	double slope = 1.0 * (outMax - outMin) / (inMax - inMin);

	return outMin + round (slope * (input - inMin));
}

char * Utils::reduceCharArray (char ** array, int length)
{
	*array += length;

	return *array;
}

Utils utils = Utils();
