#pragma once
#include "uiLayout.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <cstring>

enum TextAnchor
{
	TEXT_ANCHOR_TOP_LEFT = 0,
	TEXT_ANCHOR_TOP_CENTER,
	TEXT_ANCROR_TOP_RIGHT,
	TEXT_ANCHOR_MID_LEFT,
	TEXT_ANCHOR_CENTER,
	TEXT_ANCHOR_MID_RIGHT,
	TEXT_ANCHOR_BOTTOM_LEFT,
	TEXT_ANCHOR_BOTTOM_CENTER,
	TEXT_ANCHOR_BOTTOM_RIGHT
};

class Texture {
public:
	Texture();
	~Texture();

	bool Create(char *text, TTF_Font *font, SDL_Color *rgba, SDL_Renderer *renderer);
	bool GetDimensions(int *w, int *h);

	void Render(SDL_Renderer *renderer, SDL_Rect *dst);

private:
	SDL_Texture *m_Texture;
};

class UIWidget {
public:
	virtual ~UIWidget() {};

	virtual UILayout* HandleInput(SDL_Event *event, GameScreen **newScreen, void *userData) = 0;
	virtual void Render(SDL_Renderer *renderer) = 0;
};

class UILabel : public UIWidget {
public:
	bool Create(char *text, TTF_Font *font, SDL_Color *rgba, SDL_Renderer *renderer,
		float x, float y, bool isVisible,
		float scaleFactor = 1, TextAnchor anchor = TEXT_ANCHOR_CENTER);

	void SetText(char *text, TTF_Font *font);
	void SetVisibility(bool isVisible);

	/* false, if dimensions are not updated */
	bool GetDimensions(SDL_Rect *dimensions);

	UILayout* HandleInput(SDL_Event *event, GameScreen **newScreen, void *userData);
	void Render(SDL_Renderer *renderer);

private:
	Texture m_Texture;

	TTF_Font *m_Font;
	SDL_Color m_TextColor;
	char m_Text[128];

	float m_X, m_Y;
	TextAnchor m_Anchor;

	bool m_IsVisible;
	SDL_Rect m_Dimensions;
	float m_ScaleFactor;

	bool m_TextChanged;
};

typedef UILayout* (*pfnButtonEventHandler)(SDL_Event *event, GameScreen **newScreen, void *userData);

class UIButton : public UIWidget {
public:
	bool Create(char *text, TTF_Font *font, SDL_Color *rgbaText, SDL_Color *rgbaSelector,
		SDL_Renderer *renderer, float x, float y,
		bool isVisible, pfnButtonEventHandler eventHandler,
		float scaleFactor = 1, TextAnchor anchor = TEXT_ANCHOR_CENTER);

	UILayout* HandleInput(SDL_Event *event, GameScreen **newScreen, void *userData);
	void Render(SDL_Renderer *renderer);

	void SetText(char *text, TTF_Font *font);
	void SetVisibility(bool isVisible);
	bool IsVisible();
	void Select(bool isSelected);
	bool IsSelected();

private:
	Texture m_Texture;

	TTF_Font *m_Font;
	char m_Text[128];
	SDL_Color m_TextColor;
	SDL_Color m_SelectorColor;

	float m_X, m_Y;
	TextAnchor m_Anchor;

	bool m_IsVisible;
	bool m_IsSelected;
	SDL_Rect m_Dimensions;
	SDL_Rect m_Selector;
	float m_ScaleFactor;

	bool m_TextChanged;

	pfnButtonEventHandler m_EventHandler;
};