#ifndef DEBUG_MEMORY_H 

#define DEBUG_MEMORY_H 

#define _CRTDBG_MAP_ALLOC 
#include <cstdlib> 
#include <crtdbg.h> 
#ifdef _DEBUG 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif 

#endif // DEBUG_MEMORY_H 