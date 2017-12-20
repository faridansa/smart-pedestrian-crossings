/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <FreeRTOS.h>
#include <stdio.h>
#include <task.h>
#include <timers.h>

#include <string.h>


#define USART_SERIAL_EXAMPLE             &USARTC0
#define USART_SERIAL_EXAMPLE_BAUDRATE    9600
#define USART_SERIAL_CHAR_LENGTH         USART_CHSIZE_8BIT_gc
#define USART_SERIAL_PARITY              USART_PMODE_DISABLED_gc
#define USART_SERIAL_STOP_BIT            false

#define MY_ADC    ADCA
#define MY_ADC_CH ADC_CH0

#define MY_ADC2    ADCA
#define MY_ADC2_CH ADC_CH1

#define MY_ADC3    ADCA
#define MY_ADC3_CH ADC_CH2

uint16_t result = 0;
uint16_t result2 = 0;
uint16_t result3 = 0;
int door = 0;
long increment = 0;
static char strbuf[201];

static portTASK_FUNCTION_PROTO(sendStringAndChar, p_);

static char strbuf[201];
static char reads[100];
//int result = 0;
char in = 'x';

char *str1 = "atas ";
char *str2 = "bawah ";

void setUpSerial()
{
	// Baud rate selection
	// BSEL = (2000000 / (2^0 * 16*9600) -1 = 12.0208... ~ 12 -> BSCALE = 0
	// FBAUD = ( (2000000)/(2^0*16(12+1)) = 9615.384 -> mendekati lah ya
	
	USARTC0_BAUDCTRLB = 0; //memastikan BSCALE = 0
	USARTC0_BAUDCTRLA = 0x0C; // 12
	
	//USARTC0_BAUDCTRLB = 0; //Just to be sure that BSCALE is 0
	//USARTC0_BAUDCTRLA = 0xCF; // 207
	
	
	//Disable interrupts, just for safety
	USARTC0_CTRLA = 0;
	//8 data bits, no parity and 1 stop bit
	USARTC0_CTRLC = USART_CHSIZE_8BIT_gc;
	
	//Enable receive and transmit
	USARTC0_CTRLB = USART_TXEN_bm | USART_RXEN_bm;
}

void sendChar(char c)
{
	
	while( !(USARTC0_STATUS & USART_DREIF_bm) ); //Wait until DATA buffer is empty
	
	USARTC0_DATA = c;
	
}

void sendString(char *text)
{
	while(*text)
	{
		//sendChar(*text++);
		usart_putchar(USART_SERIAL_EXAMPLE, *text++);
	}
}

char receiveChar()
{
	while( !(USARTC0_STATUS & USART_RXCIF_bm) ); //Wait until receive finish
	return USARTC0_DATA;
}

void receiveString()
{
	int i = 0;
	while(1){
		//char inp = receiveChar();
		char inp = usart_getchar(USART_SERIAL_EXAMPLE);
		if(inp=='\n') break;
		else reads[i++] = inp;
	}
	if(strcmp(str1,reads) == 0){
		gpio_set_pin_high(J2_PIN0);
		}else if(strcmp(str2,reads) == 0){
		gpio_set_pin_high(J2_PIN0);
		}else{
		gpio_set_pin_low(J2_PIN0);
	}
}

void vTimerCallback(){
	increment++;
}

void PWM_Init(void)
{
	/* Set output */
	PORTC.DIR |= PIN0_bm;

	/* Set Register */
	TCC0.CTRLA = (PIN2_bm) | (PIN0_bm);
	TCC0.CTRLB = (PIN4_bm) | (PIN2_bm) | (PIN1_bm);
	
	/* Set Period */
	TCC0.PER = 1000;

	/* Set Compare Register value*/
	TCC0.CCA = 375;
}

static void adc_init(void)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;

	adc_read_configuration(&MY_ADC, &adc_conf);
	adcch_read_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);

	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12,ADC_REF_VCC);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_clock_rate(&adc_conf, 200000UL);

	adcch_set_input(&adcch_conf, J3_PIN0, ADCCH_NEG_NONE, 1);

	adc_write_configuration(&MY_ADC, &adc_conf);
	adcch_write_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
}

static void adc_init2(void)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;

	adc_read_configuration(&MY_ADC2, &adc_conf);
	adcch_read_configuration(&MY_ADC2, MY_ADC2_CH, &adcch_conf);

	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12,ADC_REF_VCC);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_clock_rate(&adc_conf, 200000UL);

	adcch_set_input(&adcch_conf, J3_PIN1, ADCCH_NEG_NONE, 1);

	adc_write_configuration(&MY_ADC2, &adc_conf);
	adcch_write_configuration(&MY_ADC2, MY_ADC2_CH, &adcch_conf);
}

static void adc_init3(void)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;

	adc_read_configuration(&MY_ADC3, &adc_conf);
	adcch_read_configuration(&MY_ADC3, MY_ADC3_CH, &adcch_conf);

	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12,ADC_REF_VCC);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_clock_rate(&adc_conf, 200000UL);

	adcch_set_input(&adcch_conf, J3_PIN2, ADCCH_NEG_NONE, 1);

	adc_write_configuration(&MY_ADC3, &adc_conf);
	adcch_write_configuration(&MY_ADC3, MY_ADC3_CH, &adcch_conf);
}

static uint16_t adc_read(){
	uint16_t result;
	adc_enable(&MY_ADC);
	adc_start_conversion(&MY_ADC, MY_ADC_CH);
	adc_wait_for_interrupt_flag(&MY_ADC, MY_ADC_CH);
	result = adc_get_result(&MY_ADC, MY_ADC_CH);
	return result;
}

static uint16_t adc_read2(){
	uint16_t result;
	adc_enable(&MY_ADC2);
	adc_start_conversion(&MY_ADC2, MY_ADC2_CH);
	adc_wait_for_interrupt_flag(&MY_ADC2, MY_ADC2_CH);
	result = adc_get_result(&MY_ADC2, MY_ADC2_CH);
	return result;
}

static uint16_t adc_read3(){
	uint16_t result;
	adc_enable(&MY_ADC3);
	adc_start_conversion(&MY_ADC3, MY_ADC3_CH);
	adc_wait_for_interrupt_flag(&MY_ADC3, MY_ADC3_CH);
	result = adc_get_result(&MY_ADC3, MY_ADC3_CH);
	return result;
}

int main (void)
{
	// Insert system clock initialization code here (sysclk_init()).

	
	board_init();
	pmic_init();
	
	sysclk_init();
    
	adc_init();
	adc_init2();
	adc_init3();
	gfx_mono_init();
    //
    gpio_set_pin_high(LCD_BACKLIGHT_ENABLE_PIN);
   
    PORTC_OUTSET = PIN3_bm; // PC3 as TX
    PORTC_DIRSET = PIN3_bm; //TX pin as output
   
    PORTC_OUTCLR = PIN2_bm; //PC2 as RX
    PORTC_DIRCLR = PIN2_bm; //RX pin as input
   
    setUpSerial();
	
	static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = USART_SERIAL_EXAMPLE_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT
	};
	
	usart_init_rs232(USART_SERIAL_EXAMPLE, &USART_SERIAL_OPTIONS);
	
	ioport_set_pin_dir(J2_PIN0, IOPORT_DIR_OUTPUT);
	
	
	TimerHandle_t timerPing = xTimerCreate("tPing", 2/portTICK_PERIOD_MS, pdTRUE, (void *) 0, vTimerCallback);
	
	xTaskCreate(sendStringAndChar,"",500,NULL,1,NULL);
	
	xTimerStart(timerPing, 0);
	
	vTaskStartScheduler();

	// Insert application code here, after the board has been initialized.
}

static portTASK_FUNCTION(sendStringAndChar, p_){
	//ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN, false);
	
    if(gpio_pin_is_low(GPIO_PUSH_BUTTON_1) && gpio_pin_is_high(GPIO_PUSH_BUTTON_2)){
        gpio_set_pin_low(LED2_GPIO);
        gpio_set_pin_high(LED3_GPIO);
        //sendChar('u');
        sendString("lima \n");
        //usart_putchar(USART_SERIAL_EXAMPLE,'u');
    }else if(gpio_pin_is_low(GPIO_PUSH_BUTTON_2) && gpio_pin_is_high(GPIO_PUSH_BUTTON_1)){
        gpio_set_pin_low(LED3_GPIO);
        gpio_set_pin_high(LED2_GPIO);
        //sendChar('1');
        sendChar("bawah \n");
        //usart_putchar(USART_SERIAL_EXAMPLE,'d');
    }
    receiveString();
    //in = usart_getchar(USART_SERIAL_EXAMPLE);
    //reads[0] = in;
   
    //snprintf(strbuf, sizeof(strbuf), "Read USART : %3d",in);
    gfx_mono_draw_string(reads,0, 0, &sysfont);
    vTaskDelay(10/portTICK_PERIOD_MS);
}
