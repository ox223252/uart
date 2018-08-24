# uart

code interface between std function and user for Windows and Linux

## Warning:
Not tested yed

## Exemple:
```C
#include "lib/uart/uart.h"

int main ( void )
{
	uartHandler uart = 0; // uart handler
	char busName[ 64 ] = { 0 }; // bus name /dev/ttyACMx or COMx
	uint8_t buffer[  1024 ] = { 0 }; // buffer for data
	
	...
	
	uart = uartOpen ( busName, UART_RDWR );
	if ( !uartValide( uart ) )
	{
		printf ( "can't open %s\n", busName );
		return ( __LINE__ );
	}

	if ( uartInit ( uart, 115200, UART_DATA_8 | UART_PARITY_N | UART_STOP_1 ) )
	{
		printf ( "cna't config %s\n", busName );
		return ( __LINE__ );
	}

	uartWrite ( uart, buffer, bufferSizeMax );

	uartRead ( uart, buffer, bufferSizeMax );

	uartClose ( uart );

	return ( 0 );
}