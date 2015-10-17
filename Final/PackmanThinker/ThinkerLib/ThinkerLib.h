#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the THINKERLIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// THINKERLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.


#ifdef THINKERLIB_EXPORTS
#define THINKERLIB_API extern "C" __declspec(dllexport)
#else
#define THINKERLIB_API __declspec(dllimport)
#endif

#include "Vector2.h"

struct NativePawn;
struct Pawn;

THINKERLIB_API int CreateWorld();
THINKERLIB_API void CreateNode(int in_worldId, int in_id, NativeVector2 in_position);
THINKERLIB_API void CreateConnection(int in_worldId, int in_fromId, int in_toId, int in_direction, float in_cost);

THINKERLIB_API void DestroyWorld(int in_worldId);

THINKERLIB_API void CreatePointObj(int in_worldId, int in_nodeId, int in_worth, int in_type);
THINKERLIB_API void DestroyPointObj(int in_worldId, int in_nodeId);

THINKERLIB_API void SetPacman(int in_worldId, const NativePawn& in_pawn);
THINKERLIB_API void SetBlinky(int in_worldId, const NativePawn& in_pawn);
THINKERLIB_API void SetPinky(int in_worldId, const NativePawn& in_pawn);
THINKERLIB_API void SetInky(int in_worldId, const NativePawn& in_pawn);
THINKERLIB_API void SetClyde(int in_worldId, const NativePawn& in_pawn);

THINKERLIB_API int ThinkPacman(int in_worldId, float in_deltaTime, float in_totalTime);
THINKERLIB_API int ThinkPacman2(int in_worldId, float in_deltaTime, float in_totalTime);
THINKERLIB_API int ThinkPacman3(int in_worldId, float in_deltaTime, float in_totalTime);
THINKERLIB_API int ThinkBlinky(int in_worldId, float in_deltaTime, float in_totalTime);
THINKERLIB_API int ThinkPinky(int in_worldId, float in_deltaTime, float in_totalTime);
THINKERLIB_API int ThinkInky(int in_worldId, float in_deltaTime, float in_totalTime);
THINKERLIB_API int ThinkClyde(int in_worldId, float in_deltaTime, float in_totalTime);

THINKERLIB_API int Test(int a, int b, Vector2 in_test);