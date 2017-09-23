#pragma once
#include "globals.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <ctime>

typedef void (*CrashCallback)();

class Utilities
{
public:
	/**
	 * Generate random number in given range.
	 */
	static int Random(int min, int max);

	/**
	 * Modulo addition. Result is always positive.
	 */
	static unsigned int ModuloSum(int a, int b, unsigned int m);

	/**
	 * Creates SDL_Texture from given font and string.
	 */
	static SDL_Texture* CreateTextureFromString(SDL_Renderer *renderer,
							TTF_Font *font, SDL_Color *rgba, const char *str);

	/**
	 * Show OS native error message box and exit application. 
	 */
	static void CrashMessageBox(const char *title, const char *msg, CrashCallback cb);

	/**
	 * A wrapper around WinAPI's GetPrivateProfileSectionNames().
	 */
	static size_t GetIniSectionNames(char *out, size_t size, const char *iniPath);

	/**
	 * A wrapper around WinAPI's GetPrivateProfileString().
	 */
	static size_t GetIniStringValue(char *section, char *key, char *default,
					char *out, size_t size, const char *iniPath);

	/**
	 *
	 */
	static uint32_t GetIniUintValue(char *section, char *key, uint32_t default, const char *iniPath);
	
	/**
	 *
	 */
	static bool WriteIniSection(char *section, const char *iniPath);

	/**
	 *
	 */
	static bool WriteIniString(char *section, char *key, char *value, const char *iniPath);


	static bool IsHexString(char *str);

	static bool IsBigEndian();

	static AspectRatio GetAspectRatio(int w, int h);


	static bool FileExists(const char *path);
	static bool CreateNewFile(const char *path);
	static bool CreateNewDirectory(const char *path);
};

#define LOG_INFO(format, ...) fprintf(Globals::LOG, "[%s] INFO: "##format##"\n", Globals::Time(), __VA_ARGS__)
#define LOG_WARN(format, ...) fprintf(Globals::LOG, "[%s] WARNING: "##format##"\n", Globals::Time(), __VA_ARGS__)
#define LOG_ERR(format, ...) fprintf(Globals::LOG, "[%s] ERROR: "##format##"\n", Globals::Time(), __VA_ARGS__)
#define LOG_FATAL(format, ...) fprintf(Globals::LOG, "[%s] FATAL: "##format##"\n", Globals::Time(), __VA_ARGS__)

#define CRASH(cb) Utilities::CrashMessageBox("Ooops!", "The game just crashed.", cb);