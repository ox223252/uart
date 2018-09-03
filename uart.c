#include "uart.h"

#ifdef __linux__
#include <termios.h>
// ressources :
// http://manpagesfr.free.fr/man/man3/termios.3.html
#else
// ressources :
// https://msdn.microsoft.com/en-us/library/ff802693.aspx
// https://code.msdn.microsoft.com/windowsdesktop/Serial-Port-Sample-e8accf30/sourcecode?fileId=67164&pathId=1394200469
#endif

#ifdef __linux__
int uartInit ( int bus, uint32_t speed, UART_INIT_FLAGS flags )
{
	uint16_t i = 0;
	struct termios uart;

	// get old mask
	if ( tcgetattr ( bus, &uart ) )
	{
		return ( __LINE__ );
	}

	// set speed
	cfsetospeed ( &uart, speed );
	cfsetispeed ( &uart, speed );

	// set config
	for ( i = 1; i != 0; i <<= 1 )
	{
		switch ( flags )
		{
			case UART_DATA_5:
			{
				uart.c_cflag &= ~CSIZE;
				uart.c_cflag |= CS5;
				break;
			}
			case UART_DATA_6:
			{
				uart.c_cflag &= ~CSIZE;
				uart.c_cflag |= CS6;
				break;
			}
			case UART_DATA_7:
			{
				uart.c_cflag &= ~CSIZE;
				uart.c_cflag |= CS7;
				break;
			}
			case UART_DATA_8:
			{
				uart.c_cflag &= ~CSIZE;
				uart.c_cflag |= CS8;
				break;
			}
			case UART_PARITY_N:
			{
				uart.c_cflag &= ~PARENB;
				break;
			}
			case UART_PARITY_O:
			{
				uart.c_cflag |= PARENB;
				uart.c_cflag |= PARODD;
				break;
			}
			case UART_PARITY_E:
			{
				uart.c_cflag |= PARENB;
				uart.c_cflag &= ~PARODD;
				break;
			}
			case UART_STOP_1:
			{
				uart.c_cflag &= ~CSTOPB;
				break;
			}
			case UART_STOP_2:
			{
				uart.c_cflag |= CSTOPB;
				break;
			}
			default:
			{
				break;
			}
		}
	}

	// clean rest of struct
	uart.c_iflag = 0;
	uart.c_lflag = 0;
	uart.c_oflag = 0;

	return ( tcsetattr ( bus, TCSANOW, &uart ) );
}

int uartSetReadTimeout ( int bus, uint8_t time, uint8_t min )
{
	static struct termios uart;

	tcgetattr ( bus, &uart );
	
	uart.c_cc[ VMIN ] = min;
	uart.c_cc[ VTIME ] = time;

	return ( tcsetattr( bus, TCSANOW, &uart ) );
}
#else
int uartInit ( char * busName, uint32_t speed, UART_INIT_FLAGS flags )
{
	uint8_t data = 8;
	char parity = 'N';
	uint8_t stop = 1;
	int i;

	char cmd[ 32 ] = { 0 };

	// set config
	for ( i = 1; i != 0; i <<= 1 )
	{
		switch ( flags )
		{
			case UART_DATA_5:
			{
				data = 5;
				break;
			}
			case UART_DATA_6:
			{
				data = 6;
				break;
			}
			case UART_DATA_7:
			{
				data = 7;
				break;
			}
			case UART_DATA_8:
			{
				data = 8;
				break;
			}
			case UART_PARITY_N:
			{
				parity = 'N';
				break;
			}
			case UART_PARITY_O:
			{
				parity = 'O';
				break;
			}
			case UART_PARITY_E:
			{
				parity = 'E';
				break;
			}
			case UART_STOP_1:
			{
				stop = 1;
				break;
			}
			case UART_STOP_2:
			{
				stop = 2;
				break;
			}
			default:
			{
				break;
			}
		}
	}

	sprintf ( cmd, "MODE %s:%d,%c,%d,%d", busName, speed, parity, data, stop );

	// Set new state.
	return ( system ( cmd ) );
}

int uartSetReadTimeout ( int bus, uint8_t time, uint8_t min )
{
	return ( 1 );
}


#endif
