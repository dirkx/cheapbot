![image of a cheapbot](img/board-v100.jpg)

A shonkbot for less than the price of a decent pint of Bristol Beer

 * Original -- http://jarkman.co.uk/catalog/robots/shonkbot.htm
 * Board -- https://oshwlab.com/dirkx/cheapbot
 * Software -- https://github.com/dirkx/cheapbot

# Preparing board 

For 1.00 boards - apply rework #0 first.

# Arduino preparation; install 

	https://github.com/DeqingSun/ch55xduino

and if needed - apply https://github.com/DeqingSun/ch55xduino/pull/163 by: ....

# First flashing 

	Device should be in firmware modus. If not - see Rework #1 below; then
	press boot button during insertion of USB cable.

# Later flashing #

	The standard arduino firmware will put the device in boot mode if the serial
	is set to 1200 baud shortly. So Rework #1 is not needed if your first flashing
	succeeds -and- is with an Arduino build payload.

# Initial bringup #

00_blink	- check if the LEDs works. Do not connect the motors yet - they will get hot.

01_motor	- motor's wired up; pining identical as on the motor connector. 

02_buttons	- solder in buzzer (if you do it earlier - you get an annoying bzz) - button presses should now work.
		  solder jumper P0.6 needs to be in place.

# Board Version 1.00

The side with the CPU (big square chi) is the top side.

Hold the board with the USB connector nearest to you - and the side with Vmotor
away from you. So the USB cable becomes the 'tail'. And the two white LEDs (LR0
and LL0) become the 'headlights' at the front. 

## Rework #0 

The slot for the USB connector was not cut out properly. As a result the
USB connector is at a 45 degree angle. Secure it by soldering a U shapped
piece of stuff wire over it - thus fixing it in position. And then put some
extra solder on the rear 2 tabs. 

![rework 0, bridge over the USB connector](img/rework_00.jpg)

## Rework # 1 

The BOOT switch should have been wired to 3V3; it was wired by accident
to GND. Remove R10 and cut the trace at the back of the board that runs
to the pin-header below R10. Then place a bodge wire from the
left R10 pad (i.e. away from the BOOT switch, 3V3), to the top right
pin of the BOOT switch (nearest to the USB connector). 
	
![rework 01 - remove R10, add bodge wire on top side of board](img/rework_01_top.jpg)
![rework 01 - cut trace bottom board](img/rework_01_bottom.jpg)

## Known flaws of 1.00

 *  The connector are 2mm rather than 2.54mm spaced; so standard motors cannot use their (too long) cables and connectors. They have to be soldered in or to be given a new connector. The pitch is a bit fine for a beginner. The PCB needs to be made about 5mm bigger in every direction to accomdate this.

 * The hole in the middle is a bit small for the `thick' sharpie pent.

 * The batterynest mounting holes need a rethink; with short bolts they obscure the hole of the pen -and- the nuts are a bit fiddly `under' the motor.

 * The motor voltage is not well labeled (top missing)

 * The solder jumper for boot/0.6 is not as nice as the one for the motor; a smaller one should be selected.

 * The battery connector should be one size bigger (it is now more the size common on LiPo -- as opposed to 3 AA or 3 AAA nests).


