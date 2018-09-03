#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>

#ifndef __lineux__
#include <windows.h>
#endif

typedef enum
{
	UART_DATA_5 = 0x01,
	UART_DATA_6 = 0x02,
	UART_DATA_7 = 0x04,
	UART_DATA_8 = 0x08,
	UART_PARITY_N = 0x10, // no parity
	UART_PARITY_O = 0x20, // parity odd
	UART_PARITY_E = 0x40, // parity even
	UART_STOP_1 = 0x80,
	UART_STOP_2 = 0x100
}
UART_INIT_FLAGS;

////////////////////////////////////////////////////////////////////////////////
/// \fn int uartOpen ( const char * const busName, 
///     const UART_OPEN_FLAGS flags );
/// \param[ in ] busName : string what contain bus name
///     Linux: "/dev/ttyACMx"
///     Windows : "COMx"
/// \param[ in ] flags : 
/// \brief this function open uart
/// \retrun uart handler or error code need to be tested by uartValide ( )
////////////////////////////////////////////////////////////////////////////////
int uartOpen ( const char * const busName, const int flags );

////////////////////////////////////////////////////////////////////////////////
/// \fn int uartInit ( int bus, uint32_t speed, UART_INIT_FLAGS flags );
/// \param[ in ] bus : handler returned by uartOpen ( )
/// \param[ in ] speed : baudrate available
///     50, 75, 110,  134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600
///     19200, 38400, 57600, 115200, 230400
/// \param[ in ] flags : flag to config, if an element is not set 
///     it will not change
/// \brief this will set uart port to the defined config
/// \retrun if 0 then OK else error
////////////////////////////////////////////////////////////////////////////////
int uartInit ( char * busName, uint32_t speed, UART_INIT_FLAGS flags );

////////////////////////////////////////////////////////////////////////////////
/// \fn int uartSetReadTimeout ( int bus, uint8_t time, uint8_t min );
/// \param[ in ] bus : handler returned by uartOpen ( )
/// \param[ in ] time : max time between two bytes
/// \param[ in ] min : nb min of bytes read before return
/// \biref set the number min for a read and the max time bewteen two byte.
///     one the nb min of byte read if the if the timeout is reach the function
///     return
/// \return if 0 OK else error
/// \bug i'm not sure this function fully works on Windows
////////////////////////////////////////////////////////////////////////////////
int uartSetReadTimeout ( int bus, uint8_t time, uint8_t min );

////////////////////////////////////////////////////////////////////////////////
/// \fn void uartWrite ( int bus, uint8_t * buf, size_t size );
/// \param[ in ] bus : handler returned by uartOpen ( )
/// \param[ in ] buf : buffer of data
/// \param[ in ] size : buffer max size
/// \biref write data on uart from a buffer
////////////////////////////////////////////////////////////////////////////////
void uartWrite ( int bus, uint8_t * buf, size_t size );

////////////////////////////////////////////////////////////////////////////////
/// \fn void uartRead ( int bus, uint8_t * buf, size_t size );
/// \param[ in ] bus : handler returned by uartOpen ( )
/// \param[ in ] buf : buffer to store read data
/// \param[ in ] size : buffer max size
/// \biref read data from uart to a buffer
////////////////////////////////////////////////////////////////////////////////
int uartRead ( int bus, uint8_t * buf, size_t size );

////////////////////////////////////////////////////////////////////////////////
/// \fn void uartClose ( int bus );
/// \brief close uart handler
////////////////////////////////////////////////////////////////////////////////
void uartClose ( int bus );

////////////////////////////////////////////////////////////////////////////////
/// \fn int uartValide ( int bus );
/// \param[ in ] bus : handler returned by uartOpen ( )
/// \biref test if handler is valide
/// \return if 0 then handler is valide else it's not valide
////////////////////////////////////////////////////////////////////////////////
int uartValide ( int bus );


#define uartOpen(busName,O_RDWR) open(busName,O_RDWR) 
#define uartWrite(bus,buf,size) write(bus,buf,size) 
#define uartRead(bus,buf,size) read(bus,buf,size) 
#define uartClose(bus) close(bus) 
#define uartValide(bus) ( bus > 0 )

#endif