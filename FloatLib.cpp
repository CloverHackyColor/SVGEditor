//
//  FloatLib.c
//  
//
//  Created by Slice on 20.06.2018.
//

//#include <Platform.h>
#include "FloatLib.h"
#include "EfiCommon.h"
#include <stdlib.h> // Добавьте эту строку в начало файла для определения malloc
#include <string.h> // Для определения memcpy

//#define fabsf(x) ((x >= 0.0f)?x:(-x))
//#define fabsf(x) FabsF(x)

//we will assume sqrt(abs(x))
float SqrtF(float X)
{
  /*
  struct FloatInt {
    union {
      INT32 i;
      float f;
    } fi;
  };
  */
  if (X == 0.0f) {
    return 0.0f;
  } else if (X < 0.0f) {
    X = -X;
  }
//  struct FloatInt Y;
  float Yf;
  Yf = X * 0.3f;
//  Y.i = Y.i >> 1; // dirty hack - first iteration
  //do six iterations
  Yf = Yf * 0.5f + X / (Yf * 2.0f);
  Yf = Yf * 0.5f + X / (Yf * 2.0f);
  Yf = Yf * 0.5f + X / (Yf * 2.0f);
  Yf = Yf * 0.5f + X / (Yf * 2.0f);
  Yf = Yf * 0.5f + X / (Yf * 2.0f);
  Yf = Yf * 0.5f + X / (Yf * 2.0f);
  return Yf;
}

float CosF(float X);

//we know sin is odd
float SinF(float X)
{
  INTN Period;
  float X2;
  float Sign = 1.0f;
  
  if (X < 0.0f) {
    X = -X;
    Sign = -1.0f;
  }
  Period = (INTN)(X / PI2);
  X = X - Period * PI2;
  if (X > PI) {
    X = X - PI;
    Sign *= -1.0f;
  }
  if (X > PI5) {
    X = PI - X;
  }
  if (X > PI * 0.25f) {
    return (Sign*CosF(PI5 - X));
  }
  X2 = X * X;
  return (Sign*(X - X2 * X / 6.0f + X2 * X2 * X / 120.0f));
}

//we know cos is even
float CosF(float X)
{
  INTN Period;
  float Sign = 1.0f;
  float X2;
  
  if (X < 0.0f) {
    X = -X;
  }
  Period = (INTN)(X / PI2);
  X = X - Period * PI2;
  if (X > PI) {
    X = PI - X;
    Sign = -1.0f;
  }
  if (X > PI5) {
    X = PI - X;
    Sign *= -1.0f;
  }
  if (X > PI * 0.25f) {
    return (Sign*SinF(PI5 - X));
  }
  X2 = X * X;
  return (Sign * (1.0f - X2 * 0.5f + X2 * X2 / 24.0f));
}

float TanF(float X)
{
  float Y = CosF(X);
  if (Y == 0.0f) {
    Y = 1.0e-37f;
  }
  return SinF(X)/Y;
}

float PowF(float x, INTN n)
{
  float Data = x;
  if (n > 0) {
    while (n > 0) {
      Data *= 10.0f;
      n--;
    }
  } else {
    while (n < 0) {
      Data *= 0.1f;
      n++;
    }
  }
  return Data;
}

float CeilF(float X)
{
  INT32 I = (INT32)X;
  return (float)(++I);
}

float FloorF(float X)
{
  INT32 I = (INT32)X;
  return (float)I;
}

float ModF(float X, float Y)
{
  if (Y == 0.0f) return 0.0f;
  INT32 I = (INT32)(X / Y);
  return (X - (float)I * Y);
}

float AcosF(float X)
{
  float X2 = X * X;
  float res = 0.f, Y = 0.f;
  INTN Sign = 0;

  if (X2 < 0.3f) {
    Y = X * (1.0f + X2 / 6.0f + X2 * X2 * (3.0f / 40.0f));
    return (PI5 - Y);
  } else if (X2 >= 1.0f) {
    return 0.0f;
  } else {
    if (X < 0) {
      X = -X;
      Sign = 1;
    }
    Y = 1.0f - X; //for X ~ 1
    X2 = Y * (2.0f + Y * (1.0f / 3.0f + Y * (4.0f / 45.0f + Y  / 35.0f))); //Dwight, form.508
    res = SqrtF(X2);
    if (Sign) {
      res = PI - res;
    }
  }
  
  return res;
}

float AtanF(float X) //assume 0.0 < X < 1.0
{
  float Eps = 1.0e-8f;
  int i = 1;
  float X2 = X * X;
  float D = X;
  float Y = 0.f;
  float sign = 1.0f;

  if (X > 0.5f) {
    //make here arctg(1-x)
    X = 1.0f - D;
    X2 = X * X;
    Y = PI4 - X * 0.5f - X2 * 0.25f - X * X2 * 0.25f * ( 1.f / 3.f - X2 * (0.1f + X / 12.f + X2 / 28.f));
  } else {
    //  Y = X * (1 - X2 * ( 1.0f / 3.0f - X2 * (1.0f / 5.0f - X2 * ( 1.0f / 7.0f))));
    for (i = 1; i < 50; i += 2) {
      Y += (D * sign / i);
      D *= X2;
      if (D < Eps) {
        break;
      }
      sign = - sign;
    }
  }
  return Y;
}

float Atan2F(float Y, float X)  //result -pi..+pi
{
  float sign = (((X >= 0.0f) && (Y < 0.0f)) ||
                ((X < 0.0f) && (Y >= 0.0f)))?-1.0f:1.0f;
  float PP = 0.f;
  float res = 0.f;
  //1,1 = pi4  1,-1=pi34   -1,-1=-pi34   -1,1=-pi4
  if (X < 0.f) {
    PP = PI;
  }
  X = (X >= 0.0f)?X:(-X);
  Y = (Y >= 0.0f)?Y:(-Y);
  if (Y < X) {
    res = AtanF(Y / X);
  } else if (X == 0.0f) {
    res = PI5;
  } else {
    res = (PI5 - AtanF(X / Y));
  }
  return sign * (res - PP);
}


RETURN_STATUS
AsciiStrToFloat(IN  CONST CHAR8              *String,
                OUT       CHAR8              **EndPointer,  OPTIONAL
                OUT       float              *Data)
{
  UINTN Temp = 0;
  INTN Sign = 1;
  float Mantissa, Ftemp;
  CHAR8* TmpStr = NULL;
  RETURN_STATUS Status = RETURN_SUCCESS;
  if (EndPointer != NULL) {
    *EndPointer = (CHAR8 *) String;
  }
  //
  // Ignore the pad spaces (space or tab)
  //
  while ((*String == ' ') || (*String == '\t')) {
    String++;
  }
  if (*String == '-') {
    Sign = -1;
    String++;
  } else if (*String == '+') {
    String++;
  }

  Status = AsciiStrDecimalToUintnS(String, &TmpStr, &Temp);
  Mantissa = (float)Temp;
  String = TmpStr;
  if (*String == '.') {
    String++;
    Temp = 0;
    Status = AsciiStrDecimalToUintnS(String, &TmpStr, &Temp);
    Ftemp = (float)Temp;
    while (String != TmpStr) {
      if (*String == '\0') {
        break;
      }
      Ftemp *= 0.1f;
      String++;
    }
    Mantissa += Ftemp;
  }
  
  if ((*String == 'E') || (*String == 'e')){
    INTN ExpSign = 1;
    String++;
    if (*String == '-') {
      ExpSign = -1;
      String++;
    } else if (*String == '+') {
      String++;
    }
    Temp = 0;
    Status = AsciiStrDecimalToUintnS(String, &TmpStr, &Temp);
    if ( Temp > MAX_INTN ) return RETURN_UNSUPPORTED;
    if (Status == RETURN_SUCCESS) {
      Ftemp = PowF(10.0f, ExpSign * (INTN)Temp); // cast to avoid warning
      Mantissa *= Ftemp;
    }
  }
  *Data = (Sign > 0)?Mantissa:-Mantissa;
  if (EndPointer != NULL) {
    *EndPointer = (CHAR8 *) TmpStr;
  }
  return RETURN_SUCCESS;
}


//Slice my qsort implementation.
void QuickSortWorker(UINT8* Array, INTN Low, INTN High, INTN Size, 
                    int(*compare)(CONST void* a, CONST void* b), void* Temp)
{
  INTN i = Low, j = High;

  // СОЗДАЕМ ОТДЕЛЬНЫЙ БУФЕР ДЛЯ МЕДИАНЫ
  UINT8* medBuffer = (UINT8*)AllocatePool(Size);
  if (!medBuffer) return;
  UINT8* MedPtr = Array + ((Low + High) / 2) * Size; // Central element, just pointer

  // КОПИРУЕМ ЗНАЧЕНИЕ центрального элемента
  CopyMem(medBuffer, MedPtr, Size);

  // Sort around center
  while (i <= j)
  {
    while (compare((const void*)(Array + i * Size), medBuffer) == -1) ++i;
    while (compare((const void*)(Array + j * Size), medBuffer) == 1) --j;
    // Change
    if (i <= j) {
      CopyMem(Temp, (Array + i * Size), Size);
      CopyMem((Array + i * Size), (Array + j * Size), Size);
      CopyMem((Array + j * Size), Temp, Size);
      i++;
      j--;
    }
  }

  FreePool(medBuffer);

  if (j > Low)    QuickSortWorker(Array, Low, j, Size, compare, Temp);
  if (High > i)   QuickSortWorker(Array, i, High, Size, compare, Temp);
}

void QuickSort(void* Array, INTN Number, INTN Size, int(*compare)(CONST void* a, CONST void* b))
{
  void* Buffer = AllocatePool(Size);
  QuickSortWorker((UINT8*)Array, 0, Number - 1, Size, compare, Buffer);
  FreePool(Buffer);
}

//float hypot(float x, float y) { return SqrtF(x*x + y*y); }


static UINT32 seed = 12345;
float rndf() //expected 0..1
{
//  UINT16 Rand = 0;
//  AsmRdRand16(&Rand);  //it's a pity panic
//  return (float)Rand / 65536.f;
  seed = seed * 214013 + 2531011;
  float x = (float)seed / 4294967296.0f;
  return x;
}

int dither(float x, int level)
{
  if (!level) {
    return (int)x;
  }
  int i = (int)(x) * level;  //5.1 * 4 = 20.4, 5.8 * 4 = 23.2|i=20
  float dx = x * level - (float)(i); //0.4, 3.2
  i /= level;
  if (dx > rndf() * level) {
    i += (int)((0.9999f+rndf())*level); //because rndf has mean value 0.5, but (int)rnd=0
  }
  return i;
}
//there is
#if 0
XBool
EFIAPI
AsmRdRand16 (
             OUT     UINT16                    *Rand
             );
#endif

UINTN
EFIAPI
AsciiStrHexToUintn(
    IN CONST CHAR8 *String)
{
  UINTN Result = 0;
  RETURN_STATUS Status;

  Status = AsciiStrHexToUintnS(String, (CHAR8 **)NULL, &Result);
  if (Status != RETURN_SUCCESS)
  {
    return 0;
  }
  return Result;
}

RETURN_STATUS
EFIAPI
AsciiStrHexToUintnS(
    IN CONST CHAR8 *String,
    OUT CHAR8 **EndPointer, OPTIONAL OUT UINTN *Data)
{
  //
  // 1. Neither String nor Data shall be a null pointer.
  //
  SAFE_STRING_CONSTRAINT_CHECK((String != NULL), RETURN_INVALID_PARAMETER);
  SAFE_STRING_CONSTRAINT_CHECK((Data != NULL), RETURN_INVALID_PARAMETER);

  //
  // 2. The length of String shall not be greater than ASCII_RSIZE_MAX.
  //
  //  if (ASCII_RSIZE_MAX != 0) {
  //    SAFE_STRING_CONSTRAINT_CHECK ((AsciiStrnLenS (String, ASCII_RSIZE_MAX + 1) <= ASCII_RSIZE_MAX), RETURN_INVALID_PARAMETER);
  //  }

  if (EndPointer != NULL)
  {
    *EndPointer = (CHAR8 *)String;
  }

  //
  // Ignore the pad spaces (space or tab)
  //
  while ((*String == ' ') || (*String == '\t'))
  {
    String++;
  }

  //
  // Ignore leading Zeros after the spaces
  //
  while (*String == '0')
  {
    String++;
  }

  if (AsciiCharToUpper(*String) == 'X')
  {
    if (*(String - 1) != '0')
    {
      *Data = 0;
      return RETURN_SUCCESS;
    }
    //
    // Skip the 'X'
    //
    String++;
  }

  *Data = 0;

  while (InternalAsciiIsHexaDecimalDigitCharacter(*String))
  {
    *Data = (*Data << 4) + InternalAsciiHexCharToUintn(*String);
    String++;
  }

  if (EndPointer != NULL)
  {
    *EndPointer = (CHAR8 *)String;
  }
  return RETURN_SUCCESS;
}

BOOLEAN
EFIAPI
InternalIsDecimalDigitCharacter(
    IN CHAR16 Char)
{
  return (BOOLEAN)((Char >= L'0') && (Char <= L'9'));
}

BOOLEAN
EFIAPI
InternalAsciiIsHexaDecimalDigitCharacter(
  IN      CHAR8                    Char
)
{

  return (BOOLEAN)(InternalAsciiIsDecimalDigitCharacter(Char) ||
    ((Char >= 'A') && (Char <= 'F')) ||
    ((Char >= 'a') && (Char <= 'f')));
}

BOOLEAN
EFIAPI
InternalAsciiIsDecimalDigitCharacter(
  IN      CHAR8                     Char
)
{
  return (BOOLEAN)((Char >= '0') && (Char <= '9'));
}

CHAR8
EFIAPI
AsciiCharToUpper(
    IN CHAR8 Chr)
{
  return (UINT8)(((Chr >= 'a') && (Chr <= 'z')) ? Chr - ('a' - 'A') : Chr);
}

/**
  Convert a ASCII character to numerical value.

  This internal function only deal with Unicode character
  which maps to a valid hexadecimal ASII character, i.e.
  '0' to '9', 'a' to 'f' or 'A' to 'F'. For other
  ASCII character, the value returned does not make sense.

  @param  Char  The character to convert.

  @return The numerical value converted.

**/
UINTN
EFIAPI
InternalAsciiHexCharToUintn(
    IN CHAR8 Char)
{
  if (InternalIsDecimalDigitCharacter(Char))
  {
    return Char - '0';
  }

  return (10 + AsciiCharToUpper(Char) - 'A');
}

XBool IsHexDigit(char c)
{
  return (IS_DIGIT(c) || (IS_HEX(c))) ? true : false;
}

UINTN
EFIAPI
AsciiStrDecimalToUintn(
    IN CONST CHAR8 *String)
{
  UINTN Result = 0;
  RETURN_STATUS Status;

  Status = AsciiStrDecimalToUintnS(String, (CHAR8 **)NULL, &Result);
  if (Status != RETURN_SUCCESS)
  {
    return 0;
  }
  return Result;
}

RETURN_STATUS
EFIAPI
AsciiStrDecimalToUintnS(
    IN CONST CHAR8 *String,
    OUT CHAR8 **EndPointer, OPTIONAL OUT UINTN *Data)
{
  //
  // 1. Neither String nor Data shall be a null pointer.
  //
  SAFE_STRING_CONSTRAINT_CHECK((String != NULL), RETURN_INVALID_PARAMETER);
  SAFE_STRING_CONSTRAINT_CHECK((Data != NULL), RETURN_INVALID_PARAMETER);

  //
  // 2. The length of String shall not be greater than ASCII_RSIZE_MAX.
  //
  //  if (ASCII_RSIZE_MAX != 0) {
  //    SAFE_STRING_CONSTRAINT_CHECK ((AsciiStrnLenS (String, ASCII_RSIZE_MAX + 1) <= ASCII_RSIZE_MAX), RETURN_INVALID_PARAMETER);
  //  }

  if (EndPointer != NULL)
  {
    *EndPointer = (CHAR8 *)String;
  }

  //
  // Ignore the pad spaces (space or tab)
  //
  while ((*String == ' ') || (*String == '\t'))
  {
    String++;
  }

  //
  // Ignore leading Zeros after the spaces
  //
  while (*String == '0')
  {
    String++;
  }

  *Data = 0;

  while (InternalAsciiIsDecimalDigitCharacter(*String))
  {
 
    *Data = *Data * 10 + (*String - '0');
    String++;
  }

  if (EndPointer != NULL)
  {
    *EndPointer = (CHAR8 *)String;
  }
  return RETURN_SUCCESS;
}

/*
 Translate VT-UTF8 characters into one Unicode character.

 UTF8 Encoding Table
 Bits per Character | Unicode Character Range | Unicode Binary  Encoding |  UTF8 Binary Encoding
 0-7                |     0x0000 - 0x007F     |     00000000 0xxxxxxx    |   0xxxxxxx
 8-11               |     0x0080 - 0x07FF     |     00000xxx xxxxxxxx    |   110xxxxx 10xxxxxx
 12-16              |     0x0800 - 0xFFFF     |     xxxxxxxx xxxxxxxx    |   1110xxxx 10xxxxxx 10xxxxxx

 $  U+0024    10 0100             00100100                    24
 ¢  U+00A2  1010 0010             11000010 10100010           C2 A2
 €  U+20AC  0010 0000 1010 1100   11100010 10000010 10101100  E2 82 AC
 𐍈  U+10348 1 0000 0011 0100 1000  11110000 10010000 10001101 10001000  F0 90 8D 88
 */

CHAR8 *GetUnicodeChar(CHAR8 *s, CHAR16 *UnicodeChar)
{
  char ValidBytes = 0;
  UINT8 Byte0, Byte1, Byte2;
  UINT8 UnicodeByte0, UnicodeByte1;
  CHAR16 A = L'\0';
  if (*s == '&')
  {
    if (AsciiStrStr(s, "&#x") != 0)
    {
      s += 3;
      while (IS_HEX(*s) || IS_DIGIT(*s))
      {
        A <<= 4;
        if (IS_DIGIT(*s))
        {
          A += *s - 0x30;
        }
        else if (IS_UPPER(*s))
        {
          A += *s - 0x41 + 10;
        }
        else
        {
          A += *s - 0x61 + 10;
        }
        s++;
      }
    }
    else if (AsciiStrStr(s, "&amp;") != 0)
    {
      A = 0x26; //&
      s += 5;
    }
    else if (AsciiStrStr(s, "&quot;") != 0)
    {
      A = 0x22; //"
      s += 6;
    }
    else if (AsciiStrStr(s, "&lt;") != 0)
    {
      A = 0x3C; //<
      s += 4;
    }
    else if (AsciiStrStr(s, "&gt;") != 0)
    {
      A = 0x3E; //>
      s += 4;
    }
    else if (AsciiStrStr(s, "&nbsp;") != 0)
    {
      A = 0xA0; //>
      s += 6;
    }
    *UnicodeChar = A;
  }
  else
  {
    if ((*s & 0x80) == 0)
    {
      ValidBytes = 1;
    }
    else if ((*s & 0xe0) == 0xc0)
    {
      ValidBytes = 2;
    }
    else if ((*s & 0xf0) == 0xe0)
    {
      ValidBytes = 3;
    }
    switch (ValidBytes)
    {
    case 1:
      //
      // one-byte utf8 code
      //
      *UnicodeChar = (UINT16)(*s++);
      break;

    case 2:
      //
      // two-byte utf8 code
      //
      Byte1 = *s++; // c2
      Byte0 = *s++; // a2

      UnicodeByte0 = (UINT8)((Byte1 << 6) | (Byte0 & 0x3f));
      UnicodeByte1 = (UINT8)((Byte1 >> 2) & 0x07);
      *UnicodeChar = (UINT16)(UnicodeByte0 | (UnicodeByte1 << 8));
      break;

    case 3:
      //
      // three-byte utf8 code
      // sample E3 90 A1 = 0x3421
      //
      Byte2 = *s++;
      Byte1 = *s++;
      Byte0 = *s++;

      UnicodeByte0 = (UINT8)((Byte1 << 6) | (Byte0 & 0x3f));
      UnicodeByte1 = (UINT8)((Byte2 << 4) | ((Byte1 >> 2) & 0x0f));
      *UnicodeChar = (UINT16)(UnicodeByte0 | (UnicodeByte1 << 8));

    default:
      break;
    }
  }
  return s;
}

/**
  Copies the string pointed to by Source (including the terminating null char)
  to the array pointed to by Destination.

  This function is similar as strcpy_s defined in C11.

  If an error would be returned, then the function will also ASSERT().

  If an error is returned, then the Destination is unmodified.

  @param  Destination              A pointer to a Null-terminated Ascii string.
  @param  DestMax                  The maximum number of Destination Ascii
                                   char, including terminating null char.
  @param  Source                   A pointer to a Null-terminated Ascii string.

  @retval RETURN_SUCCESS           String is copied.
  @retval RETURN_BUFFER_TOO_SMALL  If DestMax is NOT greater than StrLen(Source).
  @retval RETURN_INVALID_PARAMETER If Destination is NULL.
                                   If Source is NULL.
                                   If PcdMaximumAsciiStringLength is not zero,
                                    and DestMax is greater than
                                    PcdMaximumAsciiStringLength.
                                   If DestMax is 0.
  @retval RETURN_ACCESS_DENIED     If Source and Destination overlap.
**/
RETURN_STATUS
EFIAPI
AsciiStrCpyS(
    OUT CHAR8 *Destination,
    IN UINTN DestMax,
    IN CONST CHAR8 *Source)
{

  //
  // 1. Neither Destination nor Source shall be a null pointer.
  //
  SAFE_STRING_CONSTRAINT_CHECK((Destination != NULL), RETURN_INVALID_PARAMETER);
  SAFE_STRING_CONSTRAINT_CHECK((Source != NULL), RETURN_INVALID_PARAMETER);

  SAFE_STRING_CONSTRAINT_CHECK((DestMax != 0), RETURN_INVALID_PARAMETER);

  while ((*Source != 0) && (--DestMax > 0))
  {
    *(Destination++) = *(Source++);
  }
  *Destination = 0;

  return RETURN_SUCCESS;
}

/**
  Appends a copy of the string pointed to by Source (including the terminating
  null char) to the end of the string pointed to by Destination.

  This function is similar as strcat_s defined in C11.

  If an error would be returned, then the function will also ASSERT().

  If an error is returned, then the Destination is unmodified.

  @param  Destination              A pointer to a Null-terminated Ascii string.
  @param  DestMax                  The maximum number of Destination Ascii
                                   char, including terminating null char.
  @param  Source                   A pointer to a Null-terminated Ascii string.

  @retval RETURN_SUCCESS           String is appended.
  @retval RETURN_BAD_BUFFER_SIZE   If DestMax is NOT greater than
                                   StrLen(Destination).
  @retval RETURN_BUFFER_TOO_SMALL  If (DestMax - StrLen(Destination)) is NOT
                                   greater than StrLen(Source).
  @retval RETURN_INVALID_PARAMETER If Destination is NULL.
                                   If Source is NULL.
                                   If PcdMaximumAsciiStringLength is not zero,
                                    and DestMax is greater than
                                    PcdMaximumAsciiStringLength.
                                   If DestMax is 0.
  @retval RETURN_ACCESS_DENIED     If Source and Destination overlap.
**/
RETURN_STATUS
EFIAPI
AsciiStrCatS(
    IN OUT CHAR8 *Destination,
    IN UINTN DestMax,
    IN CONST CHAR8 *Source)
{
  UINTN DestLen;
  UINTN CopyLen;

  DestLen = AsciiStrnLenS(Destination, DestMax);
  CopyLen = DestMax - DestLen;

  //
  // 1. Neither Destination nor Source shall be a null pointer.
  //
  SAFE_STRING_CONSTRAINT_CHECK((Destination != NULL), RETURN_INVALID_PARAMETER);
  SAFE_STRING_CONSTRAINT_CHECK((Source != NULL), RETURN_INVALID_PARAMETER);

  SAFE_STRING_CONSTRAINT_CHECK((DestMax != 0), RETURN_INVALID_PARAMETER);

  SAFE_STRING_CONSTRAINT_CHECK((CopyLen != 0), RETURN_BAD_BUFFER_SIZE);

  Destination = Destination + DestLen;
  while ((*Source != 0) && (--CopyLen > 0))
  {
    *(Destination++) = *(Source++);
  }
  *Destination = 0;

  return RETURN_SUCCESS;
}

UINTN
EFIAPI
AsciiStrnLenS(
    IN CONST CHAR8 *String,
    IN UINTN MaxSize)
{
  UINTN Length;

  //
  // If String is a null pointer, then the AsciiStrnLenS function returns zero.
  //
  if (String == NULL)
  {
    return 0;
  }

  //
  // Otherwise, the AsciiStrnLenS function returns the number of characters that precede the
  // terminating null character. If there is no null character in the first MaxSize characters of
  // String then AsciiStrnLenS returns MaxSize. At most the first MaxSize characters of String shall
  // be accessed by AsciiStrnLenS.
  //
  Length = 0;
  while (String[Length] != 0)
  {
    if (Length >= MaxSize - 1)
    {
      return MaxSize;
    }
    Length++;
  }
  return Length;
}

// assume len = number of UINT8 values while hex is string of hex values, e.g. "0A 1B 2C 3D" or "0A,1B,2C,3D" or "0A1B2C3D"
UINT32 hex2bin(IN const CHAR8 *hex, OUT UINT8 *bin, UINT32 len) 
{
  CHAR8	*p;
  UINT32	i, outlen = 0;
  CHAR8	buf[3];

  if (hex == NULL || bin == NULL || len <= 0 || AsciiStrLen(hex) < len * 2) {
    //		DBG("[ERROR] bin2hex input error\n"); //this is not error, this is empty value
    return false;
  }

  buf[2] = '\0';
  p = (CHAR8 *) hex;

  for (i = 0; i < len; i++)
  {
    while ( *p == 0x20  ||  *p == ','  ||  *p == '\n'  ||  *p == '\r' ) {
      p++; //skip spaces and commas
    }
    if (*p == 0) {
      break;
    }
    if (!IsHexDigit(p[0]) || !IsHexDigit(p[1])) {
      //MsgLog("[ERROR] bin2hex '%s' syntax error\n", hex);
      return 0;
    }
    buf[0] = *p++;
    buf[1] = *p++;
    bin[i] = hexstrtouint8(buf);
    outlen++;
  }
  //bin[outlen] = 0;
  return outlen;
}

/**
  Returns the first occurrence of a Null-terminated ASCII sub-string
  in a Null-terminated ASCII string.

  This function scans the contents of the ASCII string specified by String
  and returns the first occurrence of SearchString. If SearchString is not
  found in String, then NULL is returned. If the length of SearchString is zero,
  then String is returned.

  If String is NULL, then ASSERT().
  If SearchString is NULL, then ASSERT().

  If PcdMaximumAsciiStringLength is not zero, and SearchString or
  String contains more than PcdMaximumAsciiStringLength Unicode characters
  not including the Null-terminator, then ASSERT().

  @param  String          A pointer to a Null-terminated ASCII string.
  @param  SearchString    A pointer to a Null-terminated ASCII string to search for.

  @retval NULL            If the SearchString does not appear in String.
  @retval others          If there is a match return the first occurrence of SearchingString.
                          If the length of SearchString is zero,return String.

**/
CHAR8*
EFIAPI
AsciiStrStr(
  IN      CONST CHAR8* String,
  IN      CONST CHAR8* SearchString
)
{
  CONST CHAR8* FirstMatch;
  CONST CHAR8* SearchStringTmp;

  if (!String || !SearchString) {
    return NULL;
  }

  if (*SearchString == '\0') {
    return (CHAR8*)String;
  }

  while (*String != '\0') {
    SearchStringTmp = SearchString;
    FirstMatch = String;

    while ((*String == *SearchStringTmp)
      && (*String != '\0')) {
      String++;
      SearchStringTmp++;
    }

    if (*SearchStringTmp == '\0') {
      return (CHAR8*)FirstMatch;
    }

    if (*String == '\0') {
      return NULL;
    }

    String = FirstMatch + 1;
  }

  return NULL;
}

UINTN
EFIAPI
AsciiStrLen(
  IN      CONST CHAR8* String
)
{
  UINTN                             Length;

  if (!String) {
    return 0;
  }

  for (Length = 0; *String != '\0'; String++, Length++) {
    if (Length == 100000000ull) {
      break;
    }
  }
  return Length;
}

UINT8 hexstrtouint8(const CHAR8* buf)
{
  char i = 0;
  if (IS_DIGIT(buf[0]))
    i = buf[0] - '0';
  else if (IS_HEX(buf[0]))
    i = (buf[0] | 0x20) - 'a' + 10;

  if (strlen(buf) == 1) {
    return i;
  }
  i <<= 4;
  if (IS_DIGIT(buf[1]))
    i += buf[1] - '0';
  else if (IS_HEX(buf[1]))
    i += (buf[1] | 0x20) - 'a' + 10;

  return i;
}