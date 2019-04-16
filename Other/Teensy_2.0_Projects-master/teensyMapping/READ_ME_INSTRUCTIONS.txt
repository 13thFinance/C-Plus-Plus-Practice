defines the pin numbers of the teensy++ 2.0 to match the silkscreen on board,
no longer do you have to refer to pinout sheets.

to add to an arduino sketch:

1: Navigate to Documents > Arduino > libraries

2: extract teensyMapping.zip there

2.5: By the end your folder path should look like this
	Documents > Arduino > libraries > teensyMapping > teensyMapping.c
	
3: restart Arduino IDE if you have not yet

4: add #include "teensyMapping.h" to your sketch