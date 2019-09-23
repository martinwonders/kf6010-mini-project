/* LED class
 */
class LED 
{
	DigitalOut pin;
	public:
	LED(PinName p);
	void on();
	void off();
};

