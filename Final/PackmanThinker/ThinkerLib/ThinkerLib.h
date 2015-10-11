// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the THINKERLIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// THINKERLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.


#ifdef THINKERLIB_EXPORTS
#define THINKERLIB_API __declspec(dllexport)
#else
#define THINKERLIB_API __declspec(dllimport)
#endif

#include <math.h>
#include "Vector2.h"
#include <istream>

class World;
struct Pawn;

// THINKERLIB_API int GenerateWorld();
