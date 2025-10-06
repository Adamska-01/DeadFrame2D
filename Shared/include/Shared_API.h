#pragma once


#ifdef _MSC_VER
	#pragma warning(disable : 4251) // STL types in exported classes
	#pragma warning(disable : 4275) // base class with no dll-interface
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


#if defined(SHARED_STATIC)
  // Static build → no special export/import
  #define SHARED_API
#elif defined(SHARED_DYNAMIC)
  // Dynamic build
  #if defined(_WIN32) || defined(__CYGWIN__)
	#if defined(SHARED_DYNAMIC_BUILD)
	  // Building DLL
	  #if defined(__GNUC__)
		#define SHARED_API __attribute__((dllexport))
	  #else
		#define SHARED_API __declspec(dllexport)
	  #endif
	#else
	  // Consuming DLL
	  #if defined(__GNUC__)
		#define SHARED_API __attribute__((dllimport))
	  #else
		#define SHARED_API __declspec(dllimport)
	  #endif
	#endif
  #else
	#if __GNUC__ >= 4
	  // GCC/Clang shared library
	  #define SHARED_API __attribute__((visibility("default")))
	#else
	  #define SHARED_API
	#endif
  #endif
#else
  // Default fallback → treat as static build
  #define SHARED_API
#endif
