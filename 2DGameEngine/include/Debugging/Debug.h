#pragma once


//Custom asserts
#if _DEBUG

#define DBG_HALT { __debugbreak(); }

#define DBG_ASSERT_MSG(f,s,m) { if(!(f)) { printf((s),(m)); DBG_HALT; } } // f = function, s = string and m = message

#define DBG_ASSERT_MSG_EMPTY(f,s) { if(f == 0) {printf(s); } } // f = function, s = string 

#endif