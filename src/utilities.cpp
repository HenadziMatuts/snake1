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
							TTF_Font *font, const char *str)
{
	SDL_Surface *textSurface = nullptr;
	SDL_Texture *newTexture = nullptr;

	textSurface = TTF_RenderText_Solid(font, str, { 0, 0, 0 });
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