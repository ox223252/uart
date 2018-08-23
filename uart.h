#ifndef __UART_H__
#define __UART_H__

#ifdef __linux__
#include <unistd.h>
typedef int uartHandler;
#else
#include <windows.h>
typedef HANDLE uartHandler;
#endif

uartHandler uartOpen ( const char * const busName , int flags );

void uartWrite ( uartHandler bus, uint8_t * buf, size_t size );
void uartRead ( uartHandler bus, uint8_t * buf, size_t size );
void uartClose ( uartHandler bus );
bool uartValide ( uartHandler bus );

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