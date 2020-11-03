/*
 * Nombre del archivo:   main.c
 * Autor: Elias Ojeda
 *
 * Descripción: 
 *      Configura la UART para trabajar a 9600 bps con 8 bits de datos y 1 bit 
 *      de stop.
 * 
 *      Cuando se presiona la TEC1 debe enviar la letra 'U' por puerto serie.
 *      Cuando se presiona la TEC2 debe enviar la letra 'D' por puerto serie.
 * 
 *      Si se recibe un número (1, 2, 3 ó 4) debe togglear el LED correspondiente.
 */

#include <xc.h>
#include <stdint.h>

/* ------------------------ Definiciones ------------------------------------ */
#define _XTAL_FREQ  4000000L    // Frecuencia de operación del cristal

/* ------------------------ Definición de salidas --------------------------- */
#define PIN_LED1    PORTEbits.RE2
#define TRIS_LED1   TRISEbits.TRISE2

#define PIN_LED2    PORTEbits.RE1
#define TRIS_LED2   TRISEbits.TRISE1

#define PIN_LED3    PORTEbits.RE0
#define TRIS_LED3   TRISEbits.TRISE0

#define PIN_LED4    PORTAbits.RA5
#define TRIS_LED4   TRISAbits.TRISA5

/* ------------------------ Definición de entradas -------------------------- */
#define PIN_TEC1    PORTBbits.RB0
#define TRIS_TEC1   TRISBbits.TRISB0

#define PIN_TEC2    PORTBbits.RB1
#define TRIS_TEC2   TRISBbits.TRISB1

#define PIN_TEC3    PORTBbits.RB2
#define TRIS_TEC3   TRISBbits.TRISB2

#define PIN_TEC4    PORTBbits.RB3
#define TRIS_TEC4   TRISBbits.TRISB3

/* ------------------------ Bits de configuración --------------------------- */
// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits
//#pragma config FOSC = INTRC_NOCLKOUT // Oscillator Selection bits
#pragma config WDTE = OFF       // Watchdog Timer Enable bit
#pragma config PWRTE = OFF      // Power-up Timer Enable bit
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit
#pragma config CP = OFF         // Code Protection bit
#pragma config CPD = OFF        // Data Code Protection bit
#pragma config BOREN = OFF      // Brown Out Reset Selection bits
#pragma config IESO = OFF       // Internal External Switchover bit
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit
#pragma config LVP = OFF        // Low Voltage Programming Enable bit

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits

/* ------------------------ Prototipos de funciones ------------------------- */
void gpio_config();
void uart_config();
void uart_tx_byte( uint8_t dato );
uint8_t uart_rx_byte( uint8_t *dato );

// TODO: Prototipo de una función que permita transmitir un byte (uartWriteByte)
// TODO: Prototipo de una función que permita recibir un byte (uartReadByte)

/* ------------------------ Implementación de funciones --------------------- */
void main(void) {                       // Función principal
    uint8_t dato_recibido;              // Variable donde se almacenan datos
    uint8_t resultado;
    
    gpio_config();                      // Inicializo las entradas y salidas
    uart_config();                      // Configuro la UART
    
    while (1) { // Super loop
        // Ver este link: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg

        // TODO: Completar la encuesta por las teclas
        if (PIN_TEC1 == 0) {
            __delay_ms(40);
            uart_tx_byte('U');
            while (PIN_TEC1 == 0)
                __delay_ms(40);
        }
        if (PIN_TEC2 == 0) {
            __delay_ms(40);
            uart_tx_byte('D');
            while (PIN_TEC2 == 0)
                __delay_ms(40);
        }

        resultado = uart_rx_byte(&dato_recibido);

        if (resultado == 1) {
            switch (dato_recibido) {
                case '1':
                    PIN_LED1 = !PIN_LED1;
                    break;

                case '2':
                    PIN_LED2 = !PIN_LED2;
                    break;

                case '3':
                    PIN_LED3 = !PIN_LED3;
                    break;

                case '4':
                    PIN_LED4 = !PIN_LED4;
                    break;
            }

        }
    }
    
    // NO DEBE LLEGAR NUNCA AQUÍ, debido a que este programa se ejecuta
    // directamente sobre un microcontrolador y no es llamado por un ningún
    // sistema operativo, como en el caso de un programa para PC.
    
    return;
}


void gpio_config() {    
    // TODO: Completar la inicialización de los pines
    ANSEL  = 0;
    ANSELH = 0;
    
    TRIS_TEC1 = 1;
    TRIS_TEC2 = 1;
    
    TRIS_LED1 = 0;
    TRIS_LED2 = 0;
    TRIS_LED3 = 0;
    TRIS_LED4 = 0;
}

void uart_config() {
    // TODO: Configura la UART para trabajar a 9600 bps con 8 bits de datos
    // y 1 bit de stop
    TXSTAbits.TX8_9 = 0;        // Transmision de 8 bits
    TXSTAbits.TXEN  = 1;        // Transmision habilitada
    TXSTAbits.SYNC  = 0;        // Modo asincrónico
    
    TXSTAbits.BRGH    = 0; 
    BAUDCTLbits.BRG16 = 1;
    SPBRG = 25;                 // Baudrate de 9600
    
    RCSTAbits.SPEN = 1;         // Puerto de serie habilitado
    RCSTAbits.RX9  = 0;         // Recepcion de 8 bits
    RCSTAbits.CREN = 1;         // Recepcion habilitado
}


void uart_tx_byte( uint8_t dato ) {
    // TODO: Implementa una función que permita transmitir un byte (uartWriteByte)
    
    while( PIR1bits.TXIF == 0);     // Espero
    TXREG = dato;
}

uint8_t uart_rx_byte( uint8_t *dato ) {
    // TODO: Implementa una función que permita recibir un byte (uartReadByte)
    if( PIR1bits.RCIF == 1){
        *dato = RCREG;
        return 1;
    } else {
        return 0;
    }
}

/* ------------------------ Fin de archivo ---------------------------------- */