//--------------------------------------------------------------
// File     : main.c
// Datum    : 16.06.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : USB to UART Bridge
//              Die CPU kann per USB-OTG-Buchse
//              an einen PC angeschlossen werden
//              (Virtueller COM-Port)
//              Alle eingehenden Daten vom PC werden an einer
//              UART der CPU ausgegeben und alle eingehenden
//              Daten an der UART werden an den PC gesendet
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//
// Librarys : Die eingesetzen Library f�r USB-CDC und UART
//            sind abge�nderte Versionen und tragen daher
//            eine Alpha-Versions-Nr (V:0.1)
//
// Portpins :
//              PA8   -> USB_OTG_SOF (wird nicht benutzt)
//              PA9   -> USB_OTG_VBUS
//              PA10  -> USB_OTG_ID
//              PA11  -> USB_OTG_DM
//              PA12  -> USB_OTG_DP
//
//              PC11  -> UART_TX  [115200 Baud, 8N1]
//              PC12  -> UART_RX
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_usb_cdc.h"
#include "stm32_ub_uart.h"

int main(void) {
	USB_CDC_STATUS_t check_usb;
	uint16_t rx_usb;
	uint16_t rx_uart;

	char string_buf[RX_BUF_SIZE];

	SystemInit();   // Quarz Einstellungen aktivieren

	/**< All used GPIOs should be initialized by this call */
	MD_GPIO_Init(MD_DISCO_LED_PORT, LED_ALL, MD_GPIO_Mode_OUT, MD_GPIO_OType_PP, MD_GPIO_PuPd_NOPULL, MD_GPIO_Speed_High);

	//  beamCtrl(BEAM_CTRL_SOURCE_MANUAL, GPIO_OFF );
	/**< Initialize discovery button and leds */
	MD_DISCO_ButtonInit();

	/**< Initialize Leds */
	MD_DISCO_LedInit();

	/**< Initialize delay library */
	MD_DELAY_Init();

	MD_DISCO_LedOn(LED_GREEN);
	Delayms(500);
	MD_DISCO_LedOff(LED_GREEN);
	MD_DISCO_LedOn(LED_RED);
	Delayms(500);
	MD_DISCO_LedOff(LED_RED);

//	MD_GPIO_On(LED_GREEN_A);
//	MD_GPIO_Off(LED_RED_A);

//	while (1) {
//		Delayms(500);
//		MDB_GPIO_Toggle(LED_GREEN_A);
//		MDB_GPIO_Toggle_m2(LED_RED_A);
//	}
	Delayms(200);

	// init vom CDC
	UB_USB_CDC_Init(USB_OTG_HS_CORE_ID);

	// init vom UART
	UB_Uart_Init();

	while (1) {
		// test ob USB mit PC verbunden ist
		check_usb = UB_USB_CDC_GetStatus();
		if (check_usb == USB_CDC_CONNECTED) {
			// USB -> UART
			rx_usb = UB_USB_CDC_ReceiveData(string_buf);
			if (rx_usb > 0) {
				// wenn Daten per USB empfangen wurden
				// -> alle Daten per UART senden
				UB_Uart_SendData(COM3, string_buf, rx_usb);
			}

			// UART -> USB
			rx_uart = UB_Uart_ReceiveData(COM3, string_buf);
			if (rx_uart > 0) {
				// wenn Daten per UART empfangen wurden
				// -> alle Daten per USB senden
				UB_USB_CDC_SendData(string_buf, rx_uart);
			}
			MD_DISCO_LedOn(LED_GREEN);
		}
		else {
			MD_DISCO_LedOff(LED_GREEN);
		}
	}
}

