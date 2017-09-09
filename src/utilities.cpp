#include "utilities.h"
#include <cmath>
#include <cstring>
#include <random>
#include <cstdarg>
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
