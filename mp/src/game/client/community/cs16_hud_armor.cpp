#include "cbase.h"
#include "hud.h"
#include "hud_macros.h"
#include "view.h"

#include "iclientmode.h"

#include <KeyValues.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui_controls/Panel.h>

using namespace vgui;

#include "hudelement.h"
#include "cs16_hud_numbers.h"


#include "ConVar.h"

#define FADE_TIME	100
#define MIN_ALPHA	100	

//-----------------------------------------------------------------------------
// Purpose: Armor panel
//-----------------------------------------------------------------------------
class CHudArmor : public CHudElement, public CHL1HudNumbers
{
	DECLARE_CLASS_SIMPLE( CHudArmor, CHL1HudNumbers );

public:
	CHudArmor( const char *pElementName );

	void			Init( void );
	void			Reset( void );
	void			VidInit( void );

private:
	void	Paint( void );
	void	ApplySchemeSettings(vgui::IScheme *pScheme);

private:
	CHudTexture		*icon_suit_empty;
	CHudTexture		*icon_suit_full;
	int				m_iBattery;
	float			m_flFade;
};	

DECLARE_HUDELEMENT( CHudArmor );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudArmor::CHudArmor( const char *pElementName ) : CHudElement( pElementName ), BaseClass(NULL, "HudArmor")
{
	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudArmor::Init()
{
	Reset();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudArmor::Reset()
{
	m_iBattery	= 0;
	m_flFade	= 0;
}

void CHudArmor::VidInit()
{
	Reset();

	BaseClass::VidInit();
}

void CHudArmor::Paint()
{
	Color	clrHealth;
	int		a;
	int		x;
	int		y;

	BaseClass::Paint();

	if ( !icon_suit_empty )
	{
		icon_suit_empty = gHUD.GetIcon( "suit_empty" );
	}

	if ( !icon_suit_full )
	{
		icon_suit_full = gHUD.GetIcon( "suit_full" );
	}

	if ( !icon_suit_empty || !icon_suit_full )
	{
		return;
	}

	// Has health changed? Flash the health #
	if ( m_flFade )
	{
		if (m_flFade > FADE_TIME)
			m_flFade = FADE_TIME;

		m_flFade -= ( gpGlobals->frametime * 20 );
		if ( m_flFade <= 0 )
		{
			a = 128;
			m_flFade = 0;
		}
		else
		{
			// Fade the health number back to dim
			a = MIN_ALPHA + ( m_flFade / FADE_TIME ) * 128;
		}
	}
	else
	{
		a = MIN_ALPHA;
	}

	int r, g, b, nUnused;
	(gHUD.m_clrYellowish).GetColor( r, g, b, nUnused );
	clrHealth.SetColor( r, g, b, a );

	int nFontHeight	= GetNumberFontHeight();

	int nHudElemWidth, nHudElemHeight;
	GetSize( nHudElemWidth, nHudElemHeight );

	int iOffset = icon_suit_empty->Height() / 6;

	x = nHudElemWidth / 5;
	y = nHudElemHeight - ( nFontHeight * 1.5 );

	icon_suit_empty->DrawSelf( x, y - iOffset, clrHealth );

	if ( m_iBattery > 0 )
	{
		int nSuitOffset = icon_suit_full->Height() * ((float)(100-(min(100,m_iBattery))) * 0.01);	// battery can go from 0 to 100 so * 0.01 goes from 0 to 1
		icon_suit_full->DrawSelfCropped( x, y - iOffset + nSuitOffset, 0, nSuitOffset, icon_suit_full->Width(), icon_suit_full->Height() - nSuitOffset, clrHealth );
	}

	x += icon_suit_empty->Width();
	DrawHudNumber( x, y, m_iBattery, clrHealth );
}

void CHudArmor::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
	SetPaintBackgroundEnabled(false);
}
