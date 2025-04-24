#pragma once





typedef unsigned char				uint8;
typedef unsigned short			uint16;
typedef unsigned long				uint32;
typedef unsigned long long	uint64;

typedef signed char					int8;
typedef signed short				int16;
typedef signed long					int32;
typedef signed long long		int64;

typedef uint32 feedback;
constexpr feedback OK		= 0;
constexpr feedback FAIL	= 1;





#define EXCEPTION_VECTOR				__attribute__((section(".exceptionVector")))
#define INTERRUPT_VECTOR				__attribute__((section(".interruptVector")))
#define CODE_RAM								__attribute__((section(".code_ram")))
#define PREINIT_FUNCTION_ARRAY	__attribute__((section(".preInitFunctionArray")))
#define CODE_RESET							__attribute__((section(".code_reset")))



#define SECTION(sectionName)	__attribute__((section("."#sectionName)))
#define NOTHROW								__attribute__((nothrow))
#define ALIAS(symbol)					__attribute__((alias(#symbol)))
#define WEAK									__attribute__((weak))
#define NAKED									__attribute__((naked))





//	Dynamic Memory
extern void* operator new(unsigned int sizeInBytes);
extern void operator delete(void* address);


inline void* operator new[](unsigned int sizeInBytes)
{
	return(operator new(sizeInBytes));
}

inline void operator delete[](void* address)
{
	operator delete(address);
}