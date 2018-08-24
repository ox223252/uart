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
int uartInit ( uartHandler bus, uint32_t speed, UART_INIT_FLAGS flags )
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

int uartSetReadTimeout ( uartHandler bus, uint8_t time, uint8_t min )
{
	static struct termios uart;

	tcgetattr ( bus, &uart );
	
	uart.c_cc[ VMIN ] = min;
	uart.c_cc[ VTIME ] = time;

	return ( tcsetattr( bus, TCSANOW, &uart ) );
}
#else
uartHandler uartOpen ( const char * const busName, UART_OPEN_FLAGS flags )
{
	DWORD dwDesiredAccess = 0;
	DWORD dwCreationDisposition = 0;

	switch ( flags & ( UART_RDONLY | UART_WRONLY | UART_RDWR ) )
	{
		case UART_RDONLY:
		{
			dwDesiredAccess = GENERIC_READ;
			break;
		}
		case UART_WRONLY:
		{
			dwDesiredAccess = GENERIC_WRITE;
			break;
		}
		case UART_RDWR:
		{
			dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
			break;
		}
		default:
		{
			return ( INVALID_HANDLE_VALUE );
		}
	}
	
	return ( CreateFile ( busName, dwDesiredAccess, 0, NULL, OPEN_EXISTING, 0, NULL) );
}

int uartInit ( uartHandler bus, uint32_t speed, UART_INIT_FLAGS flags )
{
	uint16_t i = 0;
	DCB dcb;

	FillMemory ( &dcb, sizeof ( dcb ), 0 );

	// get old mask
	if ( GetCommState ( bus, &dcb ) == 0 )
	{
		return ( __LINE__ );
	}

	// set speed
	dcb.BaudRate = speed ;

	// set config
	for ( i = 1; i != 0; i <<= 1 )
	{
		switch ( flags )
		{
			case UART_DATA_5:
			{
				dcb.ByteSize = 5;
				break;
			}
			case UART_DATA_6:
			{
				dcb.ByteSize = 6;
				break;
			}
			case UART_DATA_7:
			{
				dcb.ByteSize = 7;
				break;
			}
			case UART_DATA_8:
			{
				dcb.ByteSize = 8;
				break;
			}
			case UART_PARITY_N:
			{
				dcb.fParity = 0;
				dcb.Parity = NOPARITY;
				break;
			}
			case UART_PARITY_O:
			{
				dcb.fParity = 1;
				dcb.Parity = ODDPARITY;
				break;
			}
			case UART_PARITY_E:
			{
				dcb.fParity = 1;
				dcb.Parity = EVENPARITY;
				break;
			}
			case UART_STOP_1:
			{
				dcb.StopBits = ONESTOPBIT;
				break;
			}
			case UART_STOP_2:
			{
				dcb.StopBits = TWOSTOPBITS;
				break;
			}
			default:
			{
				break;
			}
		}
	}

	// windows require that was true
	dcb.fBinary = 1;
	
	// clean rest of struct
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fOutxCtsFlow = 0;
	dcb.fOutxDsrFlow = 0;
	dcb.fDsrSensitivity = 0;
	dcb.fOutX = 0;
	dcb.fInX = 0;
	dcb.fNull = 0;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;

	// Set new state.
	return ( SetCommState ( bus, &dcb ) == 0 );
}

int uartSetReadTimeout ( uartHandler bus, uint8_t time, uint8_t min )
{
	COMMTIMEOUTS timeouts;

	timeouts.ReadIntervalTimeout = time * 100; 
	timeouts.ReadTotalTimeoutMultiplier = ( time )? min: 0;
	timeouts.ReadTotalTimeoutConstant = ( time )? 1: 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;

	return ( SetCommTimeouts ( bus, &timeouts ) == 0 );
}


#endif
