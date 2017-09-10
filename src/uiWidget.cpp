#include "uiWidget.h"
#include "game.h"
#include "utilities.h"

Texture::Texture()
{

}

Texture::~Texture()
{
	if (m_Texture)
	{
		SDL_DestroyTexture(m_Texture);
	}
}

bool Texture::Create(char *text, TTF_Font *font, SDL_Color *rgba, SDL_Renderer *renderer)
{
	if (m_Texture)
	{
		SDL_DestroyTexture(m_Texture);
	}

	m_Texture = Utilities::CreateTextureFromString(renderer, font, rgba, text);
	if (!m_Texture)
	{
		return false;
	}

	return true;
}

bool Texture::GetDimensions(int *w, int *h)
{
	if (!m_Texture)
	{
		return false;
	}

	SDL_QueryTexture(m_Texture, NULL, NULL, w, h);

	return true;
}

void Texture::Render(SDL_Renderer *renderer, SDL_Rect *dst)
{
	if (m_Texture)
	{
		SDL_RenderCopy(renderer, m_Texture, NULL, dst);
	}
}

bool UILabel::Create(char *text, TTF_Font *font, SDL_Color *rgba, SDL_Renderer *renderer,
				float x, float y, bool isVisible, float scaleFactor, TextAnchor anchor)
{
	if (!m_Texture.Create(text, font, rgba, renderer))
	{
		return false;
	}

	m_Texture.GetDimensions(&m_Dimensions.w, &m_Dimensions.h);
	m_Dimensions.w = (int)((float)m_Dimensions.w * scaleFactor * Globals::WINDOW_SCALE_FACTOR);
	m_Dimensions.h = (int)((float)m_Dimensions.h * scaleFactor * Globals::WINDOW_SCALE_FACTOR);
	m_Dimensions.x = (int)(x * Globals::SCREEN_WIDTH);
	m_Dimensions.y = (int)(y * Globals::SCREEN_HEIGHT);

	switch (anchor)
	{
		case TEXT_ANCHOR_CENTER:
		case TEXT_ANCHOR_TOP_CENTER:
		case TEXT_ANCHOR_BOTTOM_CENTER:
			m_Dimensions.x -= (m_Dimensions.w / 2);
			break;
		case TEXT_ANCROR_TOP_RIGHT:
		case TEXT_ANCHOR_MID_RIGHT:
		case TEXT_ANCHOR_BOTTOM_RIGHT:
			m_Dimensions.x -= m_Dimensions.w;
			break;
		default:
			break;
	}

	switch (anchor)
	{
		case TEXT_ANCHOR_MID_LEFT:
		case TEXT_ANCHOR_CENTER:
		case TEXT_ANCHOR_MID_RIGHT:
			m_Dimensions.y -= (m_Dimensions.h / 2);
			break;
		case TEXT_ANCHOR_BOTTOM_LEFT:
		case TEXT_ANCHOR_BOTTOM_CENTER:
		case TEXT_ANCHOR_BOTTOM_RIGHT:
			m_Dimensions.y -= m_Dimensions.h;
			break;
		default:
			break;
	}

	m_Font = nullptr;
	m_TextColor = *rgba;
	strcpy_s(m_Text, text);

	m_X = x;
	m_Y = y;
	m_Anchor = anchor;

	m_IsVisible = isVisible;
	m_ScaleFactor = scaleFactor;

	m_TextChanged = false;

	return true;
}

void UILabel::SetText(char *text, TTF_Font *font)
{
	if (strcmp(text, m_Text))
	{
		if (strcpy_s(m_Text, text))
		{
			LOG_ERR("strcpy_s error");
			return;
		}
		m_Font = font;
		m_TextChanged = true;
	}
}

UILayout* UILabel::HandleEvents(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	return nullptr;
}

void UILabel::Render(SDL_Renderer *renderer)
{
	if (m_TextChanged)
	{
		if (!this->Create(m_Text, m_Font, &m_TextColor, renderer,
			m_X, m_Y, m_IsVisible, m_ScaleFactor, m_Anchor))
		{
			LOG_ERR("failed changing label texture");
		}

		m_Font = nullptr;
		m_TextChanged = false;
	}
	if (m_IsVisible)
	{
		m_Texture.Render(renderer, &m_Dimensions);
	}
}

void UILabel::SetVisibility(bool isVisible)
{
	m_IsVisible = isVisible;
}

bool UILabel::GetDimensions(SDL_Rect *dimensions)
{
	*dimensions = m_Dimensions;

	return m_TextChanged ? false : true;
}

bool UIButton::Create(char *text, TTF_Font *font, SDL_Color *rgbaText, SDL_Color *rgbaSelector,
				SDL_Renderer *renderer, float x, float y, bool isVisible,
				pfnButtonEventHandler eventHandler, float scaleFactor, TextAnchor anchor)
{
	if (!m_Texture.Create(text, font, rgbaText, renderer))
	{
		return false;
	}

	m_Texture.GetDimensions(&m_Dimensions.w, &m_Dimensions.h);
	m_Dimensions.w = (int)((float)m_Dimensions.w * scaleFactor * Globals::WINDOW_SCALE_FACTOR);
	m_Dimensions.h = (int)((float)m_Dimensions.h * scaleFactor * Globals::WINDOW_SCALE_FACTOR);
	m_Dimensions.x = (int)(x * Globals::SCREEN_WIDTH);
	m_Dimensions.y = (int)(y * Globals::SCREEN_HEIGHT);

	switch (anchor)
	{
		case TEXT_ANCHOR_CENTER:
		case TEXT_ANCHOR_TOP_CENTER:
		case TEXT_ANCHOR_BOTTOM_CENTER:
			m_Dimensions.x -= (m_Dimensions.w / 2);
			break;
		case TEXT_ANCROR_TOP_RIGHT:
		case TEXT_ANCHOR_MID_RIGHT:
		case TEXT_ANCHOR_BOTTOM_RIGHT:
			m_Dimensions.x -= m_Dimensions.w;
			break;
		default:
			break;
	}

	switch (anchor)
	{
		case TEXT_ANCHOR_MID_LEFT:
		case TEXT_ANCHOR_CENTER:
		case TEXT_ANCHOR_MID_RIGHT:
			m_Dimensions.y -= (m_Dimensions.h / 2);
			break;
		case TEXT_ANCHOR_BOTTOM_LEFT:
		case TEXT_ANCHOR_BOTTOM_CENTER:
		case TEXT_ANCHOR_BOTTOM_RIGHT:
			m_Dimensions.y -= m_Dimensions.h;
			break;
		default:
			break;
	}

	m_Selector = m_Dimensions;

	if (strchr(text, 'g') || strchr(text, 'p')
		|| strchr(text, 'q') || strchr(text, 'y'))
	{
		m_Selector.h += (m_Dimensions.h / 8);
	}
	if (strchr(text, 'b') || strchr(text, 'd') || strchr(text, 'f')
		|| strchr(text, 'h') || strchr(text, 'i') || strchr(text, 'k')
		|| strchr(text, 'l') || strchr(text, 't'))
	{
		m_Selector.y -= (m_Dimensions.h / 8);
		m_Selector.h += (m_Dimensions.h / 8);
	}
	if (strchr(text, 'j'))
	{
		m_Selector.y -= (m_Dimensions.h / 8);
		m_Selector.h += (m_Dimensions.h / 4);
	}

	int minx = 0;
	TTF_GlyphMetrics(font, text[strlen(text)],
		&minx, nullptr, nullptr, nullptr, nullptr);
	m_Selector.x -= minx;
	m_Selector.w += minx;

	strcpy_s(m_Text, text);
	m_SelectorColor = *rgbaSelector;

	m_IsVisible = isVisible;
	m_IsSelected = false;
	m_EventHandler = eventHandler;
	
	return true;
}

UILayout* UIButton::HandleEvents(SDL_Event *event, GameScreen **newScreen, void *userData)
{
	return m_EventHandler ? m_EventHandler(event, newScreen, userData) : nullptr;
}

void UIButton::Render(SDL_Renderer *renderer)
{
	if (m_IsVisible)
	{
		if (m_IsSelected)
		{
			SDL_SetRenderDrawColor(renderer,
				m_SelectorColor.r, m_SelectorColor.g, m_SelectorColor.b, m_SelectorColor.a);
			SDL_RenderFillRect(renderer, &m_Selector);
		}

		m_Texture.Render(renderer, &m_Dimensions);
	}
}

void UIButton::SetVisibility(bool isVisible)
{
	m_IsVisible = isVisible;
}

bool UIButton::IsVisible()
{
	return m_IsVisible;
}

void UIButton::Select(bool isSelected)
{
	m_IsSelected = isSelected;
}

bool UIButton::IsSelected()
{
	return m_IsSelected;
}
