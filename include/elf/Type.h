#ifndef TYPE_H
#define TYPE_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

typedef unsigned char      Byte;
typedef unsigned char      Char;

typedef unsigned char      U8;
typedef unsigned short     U16;
typedef unsigned int       U32;
typedef unsigned long long U64;

typedef signed char        I8;
typedef signed short       I16;
typedef signed int         I32;
typedef signed long long  I64;

typedef float              F32;
typedef double             F64;

#ifdef __x86_64__
typedef U64 U;
typedef I64 I;
typedef F64 F;
#else
typedef U32 U;
typedef I32 I;
typedef F32 F;
#endif

typedef U8 Bool;

//Error code
typedef U8 E8;

typedef Byte* Pointer;

typedef signed long Result;

#define ContainerOf(pointer, Member, Container) \
    ((Container*)((Byte*)(pointer) - (Byte*)&((Container*)NULL)->Member))

#endif //TYPE_H
