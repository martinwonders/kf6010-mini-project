/* LED class
 */
#include  <mbed.h>
class LED 
{
	DigitalOut pin;
	enum {ON,OFF};
	public:
	LED(PinName p) : pin(p,OFF) {}
	void on() { pin=ON; }
	void off() { pin=OFF; }
};

