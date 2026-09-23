#pragma once

#ifdef _WIN32
#include <conio.h>
#include <Ws2tcpip.h>
#include <Windows.h>
#elif defined(__APPLE__)
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <libkern/OSByteOrder.h>
#define bswap_16(x) OSSwapInt16(x)
#define bswap_32(x) OSSwapInt32(x)
#define bswap_64(x) OSSwapInt64(x)
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <byteswap.h>
#endif

#define CHAR8 char
#define UINT8 unsigned char
#define UINT16 unsigned short
#define UINT32 unsigned int
#define UINTN unsigned long long
#define INTN long long
#define CHAR16 unsigned short
#define XBool bool
#define CONST const
#define INT32 int

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE true
#endif
#ifndef FALSE
#define FALSE false
#endif // !FALSE

#define VOID void
#define IN
#define OUT
#define OPTIONAL
#define EFIAPI
#define EFI_STATUS int
#define EFI_SUCCESS 0
#define EFI_NOT_FOUND -1
#define RETURN_STATUS int
#define RETURN_SUCCESS 0
#define RETURN_INVALID_PARAMETER 1
#define RETURN_UNSUPPORTED -2
#define RETURN_BAD_BUFFER_SIZE -3
#define SAFE_STRING_CONSTRAINT_CHECK(condition, errorCode) \
  do                                                       \
  {                                                        \
    if (!(condition))                                      \
    {                                                      \
      return (errorCode);                                  \
    }                                                      \
  } while (0)

#define EFI_ERROR(status) ((int)status < 0)

#define MAX(a, b)                       \
  (((a) > (b)) ? (a) : (b))

#define MIN(a, b)                       \
  (((a) < (b)) ? (a) : (b))


#define AllocatePool(size) malloc(size)
#define AllocateZeroPool(size) calloc(1, size)
#define AllocateCopyPool(size, ref) memcpy(malloc(size), ref, size)
#define ReallocatePool(oldsize, newsize, ref) realloc(ref, newsize)
#define FreePool(buffer) free(buffer)

#define CopyMem(dest,source,count) memcpy(dest, (void*)source,(size_t)count) 
#define SetMem(dest, count, value) memset(dest, value, (size_t)count)
//#define CompareMem memcmp

// Unicode
#define IS_COMMA(a) ((a) == L',')
#define IS_HYPHEN(a) ((a) == L'-')
#define IS_DOT(a) ((a) == L'.')
#define IS_LEFT_PARENTH(a) ((a) == L'(')
#define IS_RIGHT_PARENTH(a) ((a) == L')')
#define IS_SLASH(a) ((a) == L'/')
#define IS_NULL(a) ((a) == L'\0')
// Ascii
#define IS_DIGIT(a) (((a) >= '0') && ((a) <= '9'))
#define IS_HEX(a) ((((a) >= 'a') && ((a) <= 'f')) || (((a) >= 'A') && ((a) <= 'F')))
#define IS_UPPER(a) (((a) >= 'A') && ((a) <= 'Z'))
#define IS_ALFA(x) (((x >= 'a') && (x <= 'z')) || ((x >= 'A') && (x <= 'Z')))
#define IS_ASCII(x) ((x >= 0x20) && (x <= 0x7F))
#define IS_PUNCT(x) ((x == '.') || (x == '-'))
#define IS_BLANK(x) ((x == ' ') || (x == '\t'))

#ifdef _WIN32
#define __unused 
#endif
