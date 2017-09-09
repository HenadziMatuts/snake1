#include "resourceManager.h"
#include "utilities.h"
#include <Windows.h>

const char *PATH_FONT = "assets\\FFFFORWA.TTF";
const char *PATH_COLOR_SCHEME = "assets\\colorScheme.ini";

bool ResourceManager::LoadResources()
{
	/* load font */
	if (!LoadFont())
	{
		return false;
	}

	/* load color schemes */
	if (!LoadColorSchemes())
	{
		return false;
	}

	return true;
}

void ResourceManager::UnloadResources()
{
	TTF_CloseFont(m_Font);
	Globals::DestroyColorSchemes();
}

TTF_Font* ResourceManager::GetFont()
{
	return m_Font;
}

bool ResourceManager::LoadFont()
{
	m_Font = TTF_OpenFont(PATH_FONT, 60);
	if (!m_Font)
	{
		LOG_ERR("Failed to load font! SDL_ttf Error: %s", TTF_GetError());
		return false;
	}

	return true;
}

enum ColorSchemeIniKeys
{
	COLOR_SCHEME_KEY_BACKGROUND = 0,
	COLOR_SCHEME_KEY_TEXT,
	COLOR_SCHEME_KEY_SELECTOR,
	COLOR_SCHEME_KEY_DIALOG,
	COLOR_SCHEME_KEY_BORDER,
	COLOR_SCHEME_KEY_CELL_EMPTY,
	COLOR_SCHEME_KEY_CELL_SNAKE,
	COLOR_SCHEME_KEY_CELL_FOOD,
	COLOR_SCHEME_KEY_CELL_DEMO,
	COLOR_SCHEME_KEY_TOTAL
};
static char *colorSchemeIniKeys[COLOR_SCHEME_KEY_TOTAL] =
{
	"background", "text", "selector", "dialog", "border",
	"cellEmpty", "cellSnake", "cellFood", "cellDemo"
};

bool ResourceManager::LoadColorSchemes()
{
	size_t bufferSize = 256, ret = 0;
	bool isBigEndian = Utilities::IsBigEndian();

	char *names = (char*)calloc(1, bufferSize);
	if (!names)
	{
		LOG_ERR("Memory allocation error");
		return false;
	}

	ret = Utilities::GetIniSectionNames(names, bufferSize, PATH_COLOR_SCHEME);
	if (ret == (bufferSize - 2))
	{
		do
		{
			bufferSize <<= 1;

			names = (char*)realloc(names, bufferSize);
			if (!names)
			{
				LOG_ERR("Memory allocation error");
				return false;
			}

			ret = Utilities::GetIniSectionNames(names, bufferSize, PATH_COLOR_SCHEME);

		} while (ret == bufferSize - 2);
	}
	else if (ret == 0)
	{
		LOG_WARN("%s file is empty or does'n exist", PATH_COLOR_SCHEME);
		free(names);
		return true;
	}

	size_t l = 0;
	char *p = names;

	while (l = strlen(p))
	{
		if (l > 31)
		{
			LOG_WARN("Skipping \"%s\" section, name is too long", p);
			p += l + 1;
			continue;
		}

		if (!Globals::IsUniqueColorSchemeName(p))
		{
			LOG_WARN("Skipping \"%s\" section, such scheme already exists", p);
			p += l + 1;
			continue;
		}

		ColorScheme *newScheme = nullptr;
		char colorCodeString[10];
		bool skip = false;

		newScheme = new ColorScheme();
		if (!newScheme)
		{
			LOG_ERR("Memory allocation error");
			return false;
		}

		strcpy_s(newScheme->m_Name, p);

		for (int i = 0; i < COLOR_SCHEME_KEY_TOTAL; i++)
		{
			uint32_t colorCode;

			ret = Utilities::GetIniStringValue(newScheme->m_Name, colorSchemeIniKeys[i],
					"undefined", colorCodeString, 10, PATH_COLOR_SCHEME);
			if (ret != 8 || !Utilities::IsHexString(colorCodeString))
			{
				LOG_WARN("Invalid \"%s\" color code (\"%s\") in \"%s\" scheme ",
					colorSchemeIniKeys[i], colorCodeString, newScheme->m_Name);
				skip = true;
				break;
			}

			if (!strcmp(colorCodeString, "00000000"))
			{
				colorCode = 0;
			}
			else
			{
				colorCode = strtoul(colorCodeString, nullptr, 16);
				if (colorCode == 0)
				{
					LOG_WARN("Invalid \"%s\" color code (\"%s\") in \"%s\" scheme",
						colorSchemeIniKeys[i], colorCodeString, newScheme->m_Name);
					skip = true;
					break;
				}

				if (!isBigEndian)
				{
					colorCode = _byteswap_ulong(colorCode);
				}
			}

			switch (i)
			{
				case COLOR_SCHEME_KEY_BACKGROUND:
					memmove(&newScheme->m_Bg, &colorCode, 4);
					break;
				case COLOR_SCHEME_KEY_TEXT:
					memmove(&newScheme->m_Text, &colorCode, 4);
					break;
				case COLOR_SCHEME_KEY_SELECTOR:
					memmove(&newScheme->m_ButtonSelector, &colorCode, 4);
					break;
				case COLOR_SCHEME_KEY_DIALOG:
					memmove(&newScheme->m_DialogBox, &colorCode, 4);
					break;
				case COLOR_SCHEME_KEY_BORDER:
					memmove(&newScheme->m_Border, &colorCode, 4);
					break;
				case COLOR_SCHEME_KEY_CELL_EMPTY:
					memmove(&newScheme->m_CellEmpty, &colorCode, 4);
					break;
				case COLOR_SCHEME_KEY_CELL_SNAKE:
					memmove(&newScheme->m_CellSnake, &colorCode, 4);
					break;
				case COLOR_SCHEME_KEY_CELL_FOOD:
					memmove(&newScheme->m_CellFood, &colorCode, 4);
					break;
				case COLOR_SCHEME_KEY_CELL_DEMO:
					memmove(&newScheme->m_CellDemo, &colorCode, 4);
					break;
			}
		}

		if (skip)
		{
			LOG_WARN("Skipping \"%s\" section due to previous error", newScheme->m_Name);
			
			delete newScheme;
			p += l + 1;

			continue;
		}
		
		Globals::AddColorScheme(newScheme);

		p += l + 1;
	}

	free(names);

	return true;
}
