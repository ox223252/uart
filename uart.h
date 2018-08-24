#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>

#ifdef __linux__
#include <unistd.h>
#else
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
UART_FLAGS;

/// \typedef uartHandler
/// \brief var type to store uart handler
#ifdef __linux__
typedef int uartHandler;
#else
typedef HANDLE uartHandler;
#endif

////////////////////////////////////////////////////////////////////////////////
/// \fn uartHandler uartOpen ( const char * const busName, int flags );
/// \param[ in ] busName : string what contain bus name
///     Linux: "/dev/ttyACMx"
///     Windows : "COMx"
/// \param[ in ] flags : 
/// \brief this function open uart
/// \retrun uart handler or error code need to be tested by uartValide ( )
////////////////////////////////////////////////////////////////////////////////
uartHandler uartOpen ( const char * const busName, int flags );

////////////////////////////////////////////////////////////////////////////////
/// \fn int uartInit ( uartHandler bus, uint32_t speed, UART_FLAGS flags );
/// \param[ in ] bus : handler returned by uartOpen ( )
/// \param[ in ] speed : baudrate available
///     50, 75, 110,  134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600
///     19200, 38400, 57600, 115200, 230400
/// \param[ in ] flags : flag to config, if an element is not set 
///     it will not change
/// \brief this will set uart port to the defined config
/// \retrun if 0 then OK else error
////////////////////////////////////////////////////////////////////////////////
int uartInit ( uartHandler bus, uint32_t speed, UART_FLAGS flags );

////////////////////////////////////////////////////////////////////////////////
/// \fn int uartSetReadTimeout ( uartHandler bus, uint8_t time, uint8_t min );
/// \param[ in ] bus : handler returned by uartOpen ( )
/// \param[ in ] time : max time between two bytes
/// \param[ in ] min : nb min of bytes read before return
/// \biref set the number min for a read and the max time bewteen two byte.
///     one the nb min of byte read if the if the timeout is reach the function
///     return
/// \return if 0 OK else error
/// \bug i'm not sure this function fully works on Windows
////////////////////////////////////////////////////////////////////////////////
int uartSetReadTimeout ( uartHandler bus, uint8_t time, uint8_t min );

////////////////////////////////////////////////////////////////////////////////
/// \fn void uartWrite ( uartHandler bus, uint8_t * buf, size_t size );
/// \param[ in ] bus : handler returned by uartOpen ( )
/// \param[ in ] buf : buffer of data
/// \param[ in ] size : buffer max size
/// \biref write data on uart from a buffer
////////////////////////////////////////////////////////////////////////////////
void uartWrite ( uartHandler bus, uint8_t * buf, size_t size );

////////////////////////////////////////////////////////////////////////////////
/// \fn void uartRead ( uartHandler bus, uint8_t * buf, size_t size );
/// \param[ in ] bus : handler returned by uartOpen ( )
/// \param[ in ] buf : buffer to store read data
/// \param[ in ] size : buffer max size
/// \biref read data from uart to a buffer
////////////////////////////////////////////////////////////////////////////////
void uartRead ( uartHandler bus, uint8_t * buf, size_t size );

////////////////////////////////////////////////////////////////////////////////
/// \fn void uartClose ( uartHandler bus );
/// \brief close uart handler
////////////////////////////////////////////////////////////////////////////////
void uartClose ( uartHandler bus );

////////////////////////////////////////////////////////////////////////////////
/// \fn int uartValide ( uartHandler bus );
/// \param[ in ] bus : handler returned by uartOpen ( )
/// \biref test if handler is valide
/// \return if 0 then handler is valide else it's not valide
////////////////////////////////////////////////////////////////////////////////
int uartValide ( uartHandler bus );

#ifdef __linux__
#define uartWrite(bus,buf,size) write(bus,buf,size) 
#define uartRead(bus,buf,size) read(bus,buf,size) 
#define uartClose(bus) close(bus) 
#define uartOpen(busName,O_RDWR) open(busName,O_RDWR) 
#define uartValide(bus) (bus)
#else
#define uartWrite(bus,buf,size) WriteFile(bus,buf,size,NULL,NULL)
#define uartRead(bus,buf,size) ReadFile(bus,buf,size,NULL,NULL)
#define uartClose(bus) CloseHandle(bus)
#define uartOpen(busName,O_RDWR) CreateFile(busName,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL)
#define uartValide(bus) ( bus != INVALID_HANDLE_VALUE )
#endif

#endif