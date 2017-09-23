#include "utilities.h"
#include <cmath>
#include <cstring>
#include <random>
#include <cstdarg>
#include <stdio.h>
#ifdef _WIN32
	#include <Windows.h>
#endif

/**
 * Generate random number in given range.
 */
int Utilities::Random(int min, int max)
{
	if (min < 0 && max > 0)
	{
		return (rand() % (abs(min) + max + 1)) - ((abs(min) + max) / 2);
	}
	else if (min >= 0 && max > 0)
	{
		return (rand() % (max - min + 1)) + min;
	}
	else if (min < 0 && max <= 0)
	{
		return ((rand() % (abs(min) - abs(max) + 1)) + abs(max)) * (-1);
	}
	else if (min == max)
	{
		return max;
	}
	else
	{
		return 0;
	}
}

/**
 * Modulo addition. Result is always positive.
 */
unsigned int Utilities::ModuloSum(int a, int b, unsigned int m)
{
	int sum = a + b;

	if (sum >= 0)
	{
		return sum % m;
	}
	else
	{
		while (sum < 0)
		{
			sum += m;
		}
		return sum;
	}
}

/**
 * Creates SDL_Texture from given font and string.
 */
SDL_Texture* Utilities::CreateTextureFromString(SDL_Renderer *renderer,
							TTF_Font *font, SDL_Color *rgba, const char *str)
{
	SDL_Surface *textSurface = nullptr;
	SDL_Texture *newTexture = nullptr;

	textSurface = TTF_RenderText_Blended(font, str, { rgba->r, rgba->g, rgba->b, rgba->a });
	if (!textSurface)
	{
		LOG_ERR("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		return nullptr;
	}

	newTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (!newTexture)
	{
		LOG_ERR("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		return nullptr;
	}
	SDL_FreeSurface(textSurface);

	return newTexture;
}

/**
 * Show OS native error message box.
 */
void Utilities::CrashMessageBox(const char *title, const char *msg, CrashCallback cb)
{
#ifdef _WIN32
	MessageBox(NULL, msg ? msg : "NULL", title ? title : NULL, MB_OK | MB_ICONSTOP);
#endif
	cb();
}

size_t Utilities::GetIniSectionNames(char *out, size_t size, const char *iniPath)
{
	size_t ret = 0;
#ifdef _WIN32
	ret = GetPrivateProfileSectionNames(out, size, iniPath);
#endif
	return ret;
}

size_t Utilities::GetIniStringValue(char *section, char *key, char *default, 
					char *out, size_t size, const char *iniPath)
{
	size_t ret = 0;
#ifdef _WIN32
	ret = GetPrivateProfileString(section, key, default, out, size, iniPath);
#endif
	return ret;
}

uint32_t Utilities::GetIniUintValue(char *section, char *key, uint32_t default, const char *iniPath)
{
	uint32_t ret = default;
#ifdef _WIN32
	ret = GetPrivateProfileInt(section, key, default, iniPath);
#endif
	return ret;
}

bool Utilities::WriteIniSection(char *section, const char *iniPath)
{
	bool ret = false;
#ifdef _WIN32
	ret = (WritePrivateProfileSection(section, "", iniPath) == TRUE);
#endif
	return ret;
}

bool Utilities::WriteIniString(char *section, char *key, char *value, const char *iniPath)
{
	bool ret = false;
#ifdef _WIN32
	ret = (WritePrivateProfileString(section, key, value, iniPath) == TRUE);
#endif
	return ret;
}

bool Utilities::IsHexString(char *str)
{
	if (!str)
	{
		return false;
	}

	while (*str != 0)
	{
		if((*str < '0' || *str > '9')
			&& (*str < 'A' || *str > 'F')
			&& (*str < 'a' || *str > 'f'))
		{
			return false;
		}
		str++;
	}

	return true;
}

bool Utilities::IsBigEndian()
{
	union {
		uint32_t u;
		char c[4];
	} x = { 0x01020304 };

	return x.c[0] == 1;
}

AspectRatio Utilities::GetAspectRatio(int w, int h)
{
	float f = (float)w / h;

	if (f > 1.3f && f < 1.4f)
	{
		return ASPECT_RATIO_4_3;
	}
	else if (f > 1.7f && f < 1.8f)
	{
		return ASPECT_RATIO_16_9;
	}
	else
	{
		return ASPECT_RATIO_UNKNOWN;
	}
}

bool Utilities::FileExists(const char *path)
{
	bool ret = false;

#ifdef _WIN32
	ret = (GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES);
#endif

	return ret;
}

bool Utilities::CreateNewFile(const char *path)
{
	FILE *f = nullptr;

	fopen_s(&f, path, "w+");
	if (!f)
	{
		return false;
	}

	fclose(f);
	return true;
}

bool Utilities::CreateNewDirectory(const char *path)
{
	bool ret = false;
#ifdef _WIN32
	ret = (CreateDirectory(path, nullptr) == TRUE);
#endif

	return ret;
}
