# uart

code interface between std function and user for Windows and Linux

## Warning:
After many docs read, i thnk you should never read msdn, and never do something with Windows, but i did it.

## Exemple:
```C
#include "lib/uart/uart.h"

int main ( void )
{
	uartHandler uart = 0; // uart handler
	char busName[ 64 ] = { 0 }; // bus name /dev/ttyACMx or COMx
	uint8_t buffer[  1024 ] = { 0 }; // buffer for data
	
	...

	// system call, need to be done before openning uart
	if ( uartInit ( busName, 115200, UART_DATA_8 | UART_PARITY_N | UART_STOP_1 ) )
	{
		printf ( "cna't config %s\n", busName );
		return ( __LINE__ );
	}
	
	uart = uartOpen ( busName, O_RDWR );
	if ( !uartValide( uart ) )
	{
		printf ( "can't open %s\n", busName );
		return ( __LINE__ );
	}

	uartWrite ( uart, buffer, bufferSizeMax );

	uartRead ( uart, buffer, bufferSizeMax );

	uartClose ( uart );

	return ( 0 );
}