//
//  FloatLib.h
//  
//
//  Created by Slice on 20.06.2018.
//

#ifndef FloatLib_h
#define FloatLib_h

#include "EfiCommon.h"

#define ASCII_RSIZE_MAX 0
#define MAX_INTN 0x7FFFFFFF
#define MAX_UINTN 0xFFFFFFFF
#define BOOLEAN bool

const float PI = 3.1415926536f;
const float PI2 = 6.283185307179586f;
const float PI5 = 1.570796326794897f;
const float PI4 = 0.78539816339745f;
//const float FLT_MAX = 1.0e38f;
//const float FLT_MIN = 1.0e-37f;

float SqrtF(float X);
float PowF(float X, INTN N);
float SinF(float X);
float CosF(float X);
float TanF(float X);
float CeilF(float X);
float FloorF(float X);
float ModF(float X, float Y);
float AcosF(float X);
float Atan2F(float Y, float X); //y=sin(A), x=cos(A), atan2(y,x) == A
float FabsF(float X);
float rndf(void);  //random number from 0 to 1.0f
int dither(float x, int level);
//float nsvg__vmag(float x, float y); //sqrt(x*x+y*y)


inline float FabsF(float x) {
  if (x < 0.f) return -x;
  return x;
}

inline float SqrF(float x) { return x*x; }


RETURN_STATUS
AsciiStrToFloat(IN  CONST CHAR8              *String,
                OUT       CHAR8              **EndPointer,  OPTIONAL
                OUT       float              *Data);

void QuickSort(void* Array, INTN Number, INTN Size, int(*compare)(CONST void* a, CONST void* b));


//float hypot(float x, float y);

BOOLEAN
EFIAPI
InternalIsDecimalDigitCharacter(
    IN CHAR16 Char);

CHAR8
EFIAPI
AsciiCharToUpper(
    IN CHAR8 Chr);

UINTN
EFIAPI
InternalAsciiHexCharToUintn(
    IN CHAR8 Char);

XBool IsHexDigit(char c);

RETURN_STATUS
EFIAPI
AsciiStrDecimalToUintnS(
    IN CONST CHAR8 *String,
    OUT CHAR8 **EndPointer, OPTIONAL OUT UINTN *Data);

UINTN
EFIAPI
AsciiStrDecimalToUintn(
    IN CONST CHAR8 *String);

UINTN
EFIAPI
AsciiStrHexToUintn(
    IN CONST CHAR8 *String);

RETURN_STATUS
EFIAPI
AsciiStrHexToUintnS(
    IN CONST CHAR8 *String,
    OUT CHAR8 **EndPointer, OPTIONAL OUT UINTN *Data);

CHAR8 *GetUnicodeChar(CHAR8 *s, CHAR16 *UnicodeChar);

RETURN_STATUS
EFIAPI
AsciiStrCpyS(
    OUT CHAR8 *Destination,
    IN UINTN DestMax,
    IN CONST CHAR8 *Source);

RETURN_STATUS
EFIAPI
AsciiStrCatS(
    IN OUT CHAR8 *Destination,
    IN UINTN DestMax,
    IN CONST CHAR8 *Source);

UINTN
EFIAPI
AsciiStrnLenS(
    IN CONST CHAR8 *String,
    IN UINTN MaxSize);

UINT32 hex2bin(IN const CHAR8 *hex, OUT UINT8 *bin, UINT32 len);

BOOLEAN
EFIAPI
InternalAsciiIsHexaDecimalDigitCharacter(
  IN      CHAR8                    Char
);

BOOLEAN
EFIAPI
InternalAsciiIsDecimalDigitCharacter(
  IN      CHAR8                     Char
);

CHAR8*
EFIAPI
AsciiStrStr(
  IN      CONST CHAR8* String,
  IN      CONST CHAR8* SearchString
);

UINTN
EFIAPI
AsciiStrLen(
  IN      CONST CHAR8* String
);

UINT8 hexstrtouint8(const CHAR8* buf);

#endif /* FloatLib_h */
