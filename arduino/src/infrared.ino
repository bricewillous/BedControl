// ******* Infrared reception objects declaration ******* //
IRrecv irrecv (PIN_LED_INFRARED);
decode_results results;

// ******* Infrared codes and corresponding RGB code array ******* //
unsigned long color[N_COLOR][3] =
{
	// RGB Code   Code 1     Code 2
	{ 0xFFFFFF, 0xFFA857, 0xA3C8EDDB }, // WHITE
	{ 0xFF0000, 0xFF9867, 0x97483BFB }, // R1
	{ 0xFF5300, 0xFFE817, 0x5BE75E7F }, // R2
	{ 0xFF3C00, 0xFF02FD, 0xD7E84B1B }, // R3
	{ 0xFFC400, 0xFF50AF, 0x2A89195F }, // R4
	{ 0xFFFF00, 0xFF38C7, 0x488F3CBB }, // R5

	{ 0x00FF00, 0xFFD827, 0x86B0E697 }, // G1
	{ 0x20FF5D, 0xFF48B7, 0xF377C5B7 }, // G2
	{ 0x34FFF6, 0xFF32CD, 0xEE4ECCFB }, // G3
	{ 0x21E7FF, 0xFF7887, 0xF63C8657 }, // G4
	{ 0x00BDFF, 0xFF28D7, 0x13549BDF }, // G5

	{ 0x0000FF, 0xFF8877, 0x9EF4941F }, // B1
	{ 0x0068FF, 0xFF6897, 0xC101E57B }, // B2
	{ 0x8068FF, 0xFF20DF, 0x51E43D1B }, // B3
	{ 0xDB89FF, 0xFF708F, 0x44C407DB }, // B4
	{ 0xFF7B92, 0xFFF00F, 0x35A9425F } // B5
};

// ******* readInfrared ******* //
unsigned long lastIRCode; // IR code in previous loop - Allows continious power / strobe speed increasion / dicreasion
unsigned long IRCode;     // Current IR code

void initInfrared ()
{
	if (!INFRARED_ENABLED)
		return;

	lastIRCode = 0;

	pinMode (PIN_SOUND, INPUT); // Setting sound detector as an input
	irrecv.enableIRIn();        // Initialize IR communication
}

// Read the in-comming IR signal if present
void readInfrared ()
{
	if (!INFRARED_ENABLED)
		return;

	// If something is comming from IR reciever
	if (irrecv.decode (&results))
	{
		// We save the IR word in IRCode
		IRCode = results.value;

		// [DEBUG] Print the incomming IR value
		print ("Incomming IR: ");
		printNoPrefix (IRCode, HEX);
		if (IRCode == 0xFFFFFFFF)
		{
			printNoPrefix (" (");
			printNoPrefix (lastIRCode, HEX);
			printNoPrefix (")");
		}
		printlnNoPrefix();

		// REPEAT (When button is pressed continiously, sent value is 0xFFFFFFFF, so we change it with the latest code that we recieved
		if (IRCode == 0xFFFFFFFF)
			IRCode = lastIRCode;

		// ON
		if (IRCode == 0xFFB04F || IRCode == 0xF0C41643)
		{
			on         = true;
			lastIRCode = 0; // We don't save value in lastIRCode because don't care if we keep on button pressed
			printlnNoPrefix();
			println ("Switch ON\n");
		}


		// If the system is off, ignore incomming infrared (Except ON of course, he is just above)
		if (!on)
		{
			irrecv.resume();
			return;
		}

		// Test incomming value
		switch (IRCode)
		{
			// OFF
			case 0xFFF807:
			case 0xE721C0DB:
				on         = false;
				lastIRCode = 0;
				printlnNoPrefix();
				println ("Switch OFF\n");
				break;
				break;

			// DOWN
			case 0xFFB847:
			case 0xA23C94BF:
				if (mode == MODE_FLASH) // If we are in flash mode, decrease speed instead of power
				{
					if (flashSpeed - IR_FLASH_CHANGE_SPEED >= MIN_FLASH)
						flashSpeed -= IR_FLASH_CHANGE_SPEED;
					else
						flashSpeed = MIN_FLASH;
				}
				else if (mode == MODE_STROBE) // If we are in STROBE mode, decrease speed instead of power
				{
					if (strobeSpeed - IR_STROBE_CHANGE_SPEED >= MIN_STROBE)
						strobeSpeed -= IR_STROBE_CHANGE_SPEED;
					else
						strobeSpeed = MIN_STROBE;
				}
				else if (mode == MODE_FADE) // If we are in fade mode, decrease speed instead of power
				{
					if (fadeSpeed - IR_FADE_CHANGE_SPEED >= MIN_FADE)
						fadeSpeed -= IR_FADE_CHANGE_SPEED;
					else
						fadeSpeed = MIN_FADE;
				}
				else if (mode == MODE_SMOOTH) // If we are in smooth mode, decrease speed instead of power
				{
					if (smoothSpeed - IR_SMOOTH_CHANGE_SPEED >= MIN_SMOOTH)
						smoothSpeed -= IR_SMOOTH_CHANGE_SPEED;
					else
						smoothSpeed = MIN_SMOOTH;
				}
				else
				{
					if (power - POWER_CHANGE_SPEED >= MIN_POWER)
						power -= POWER_CHANGE_SPEED;
					else
						power = MIN_POWER;
				}

				rgb2color();
				lastIRCode = IRCode;

				// [DEBUG] Print current color and RED, GREEN, BLUE values
				print ("Power: ");
				printlnNoPrefix (power, DEC);
				print ("RED: ");
				printNoPrefix (red, DEC);
				printNoPrefix (" / GREEN: ");
				printNoPrefix (green, DEC);
				printNoPrefix (" / BLUE: ");
				printlnNoPrefix (blue, DEC);
				printlnNoPrefix();

				break;
				break;

			// UP
			case 0xFF906F:
			case 0xE5CFBD7F:
				if (mode == MODE_FLASH) // If we are in flash mode, increase speed instead of power
				{
					if (flashSpeed + IR_FLASH_CHANGE_SPEED <= MAX_FLASH)
						flashSpeed += IR_FLASH_CHANGE_SPEED;
					else
						flashSpeed = MAX_FLASH;
				}
				else if (mode == MODE_STROBE) // If we are in strobe mode, increase speed instead of power
				{
					if (strobeSpeed + IR_STROBE_CHANGE_SPEED <= MAX_STROBE)
						strobeSpeed += IR_STROBE_CHANGE_SPEED;
					else
						strobeSpeed = MAX_STROBE;
				}
				else if (mode == MODE_FADE) // If we are in fade mode, increase speed instead of power
				{
					if (fadeSpeed + IR_FADE_CHANGE_SPEED <= MAX_FADE)
						fadeSpeed += IR_FADE_CHANGE_SPEED;
					else
						fadeSpeed = MAX_FADE;
				}
				else if (mode == MODE_SMOOTH) // If we are in smooth mode, increase speed instead of power
				{
					if (smoothSpeed + IR_SMOOTH_CHANGE_SPEED <= MAX_SMOOTH)
						smoothSpeed += IR_SMOOTH_CHANGE_SPEED;
					else
						smoothSpeed = MAX_SMOOTH;
				}
				else
				{
					if (power + POWER_CHANGE_SPEED <= MAX_POWER)
						power += POWER_CHANGE_SPEED;
					else
						power = MAX_POWER;
				}

				rgb2color();
				lastIRCode = IRCode;

				// [DEBUG] Print current color and red, green, blue values

				print ("Power: ");
				printlnNoPrefix (power, DEC);
				print ("RED: ");
				printNoPrefix (red, DEC);
				printNoPrefix (" / GREEN: ");
				printNoPrefix (green, DEC);
				printNoPrefix (" / BLUE: ");
				printlnNoPrefix (blue, DEC);
				printlnNoPrefix();

				break;
				break;

			// FLASH
			case 0xFFB24D:
			case 0x7EC31EF7:
				mode = MODE_FLASH;
				break;
				break;

			// STROBE
			case 0xFF00FF:
			case 0xFA3F159F:
				mode = MODE_STROBE;
				break;
				break;

			// FADE
			case 0xFF58A7:
			case 0xDC0197DB:
				mode = MODE_FADE;
				break;
				break;

			// SMOOTH
			case 0xFF30CF:
			case 0x9716BE3F:
				mode = MODE_SMOOTH;
				break;
				break;

			// COLORS
			default:
				lastIRCode = 0;
				for (int i = 0; i < N_COLOR; i++)
					if (results.value == color[i][1] || results.value == color[i][2])
					{
						mode = MODE_DEFAULT;
						rgb  = color[i][0];
					}
				break;
		}
		irrecv.resume();
	}
} // readInfrared
