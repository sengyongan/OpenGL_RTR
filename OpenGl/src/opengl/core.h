#pragma once
//ºËÐÄ
#ifdef BUILD_DLL
	#define BUILD_API __declspec(dllexport)
#else
	#define BUILD_API __declspec(dllimport)

#endif