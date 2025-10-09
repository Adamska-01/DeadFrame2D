#pragma once


#ifdef _MSC_VER
	#pragma warning(disable : 4251) // STL types in exported classes
#endif


/*
| -------------------------------------	| -----------------	|
|					Type				| Needs `DF2D_API`? |
| -------------------------------------	| -----------------	|
| Template classes						|		❌ No		|
| Header - only POD struct / class		|		❌ No		|
| Class with virtual functions			|		✅ Yes		|
| Non - template class with `.cpp` file |		✅ Yes		|
| Inline - only functions / classes		|		❌ No		|
| Static members in class				|		✅ Yes		|
| -------------------------------------	| -----------------	|
*/


#if defined(DF2D_STATIC)
  // Static build → no special export/import
  #define DF2D_API
#elif defined(DF2D_DYNAMIC)
  // Dynamic build
  #if defined(_WIN32) || defined(__CYGWIN__)
	#if defined(DF2D_DYNAMIC_BUILD)
	  // Building DLL
	  #if defined(__GNUC__)
		#define DF2D_API __attribute__((dllexport))
	  #else
		#define DF2D_API __declspec(dllexport)
	  #endif
	#else
	  // Consuming DLL
	  #if defined(__GNUC__)
		#define DF2D_API __attribute__((dllimport))
	  #else
		#define DF2D_API __declspec(dllimport)
	  #endif
	#endif
  #else
	#if __GNUC__ >= 4
	  // GCC/Clang shared library
	  #define DF2D_API __attribute__((visibility("default")))
	#else
	  #define DF2D_API
	#endif
  #endif
#else
  // Default fallback → treat as static build
  #define DF2D_API
#endif