/* KF6010 -- Distributed real-time systems
 * End of Semester One mini project
 * weeks 11&12 (2hours in labs)
 */

/* Dr Alun Moon
 * November 2018
 */

#include <mbed.h>

enum { On, Off }; /* sneaky hack to handle common anode LEDs */

#include "led.h"
LED red(LED_RED);

/* Locks and signals */
Mutex crossing; /* who holds the mutex can use the crossing */
Semaphore tocross; /* indicate that pedestians want to cross */
Semaphore bleeping;

/* Pedestrian task */
void pedestrians(void)
{
	DigitalOut go(LED_GREEN,Off);
	DigitalOut stop(LED_RED,On);

	while(true) {
		crossing.lock(); /*  cannot proceed until released by the traffic */
		wait(1);
		/* Red -> green */
		stop = Off;
		go = On;
		bleeping.release();
		/* allow pedestrains to cross */
		wait(5);
		bleeping.wait();
		/* green -> red */
		stop = On;
		go = Off;
		wait(2);
		crossing.unlock();
	}
}

/* Traffic task */
void traffic(void)
{
	DigitalOut red(D5, On);
	DigitalOut amber(D8,Off);
	DigitalOut green(D9,Off);
	while(true) {
		red = On;
		amber = green = Off;

		crossing.lock();
		wait(1.5);
		/* Red -> green lights */
		red = amber = On;
		green = Off;
		wait(2.0); /* wait for 3s */
		red = amber = Off;
		green = On;

		tocross.wait(); /* wait for pedestrians wanting to cross */
		
		/* Green -> red lights */
		wait(1);
		red = green = Off;
		amber = On;
		wait(3.0);

		red = On;  /* make sure red is on before releasing lock */
		amber = green = Off;
		wait(1.5);	
		crossing.unlock();
	}
}

/* Interrupt handler for button */
void pressed(void)
{
	/* release the semaphore signalling pedestians waiting */
	tocross.release();
}

void bleeper(void)
{
	DigitalOut beep(LED_BLUE,Off);
	PwmOut b(D6);
	b.period_us(500);
	b = 0;
	while(true) {
		bleeping.wait();
		//beep = On;
		b = 0.5;
		bleeping.release();
		wait(0.6);
		//beep = Off;
		b = 0;
		wait(0.4);
	}
}

int main(void)
{
	InterruptIn button_a(SW2);
	InterruptIn button_b(SW3);
	Thread cars, people, beeps;

	button_a.fall( pressed );
	button_b.fall( pressed );
	cars.start( traffic );
	people.start( pedestrians );
	beeps.start(bleeper);

	cars.join();
}

