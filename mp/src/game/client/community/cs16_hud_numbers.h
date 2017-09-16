#ifndef CS16_HUD_NUMBERS_H
#define CS16_HUD_NUMBERS_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Panel.h>

class CCSHudNumbers : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CCSHudNumbers, vgui::Panel );

public:
	CCSHudNumbers( vgui::Panel *parent, const char *name );
	void	VidInit( void );

protected:
	int		DrawHudNumber( int x, int y, int iNumber, Color &clrDraw );
	int		GetNumberFontHeight( void );
	int		GetNumberFontWidth( void );

private:
	CHudTexture *icon_digits[ 10 ];
};


#endif // CS16_HUD_NUMBERS_H
