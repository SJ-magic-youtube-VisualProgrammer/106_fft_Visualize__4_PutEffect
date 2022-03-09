/************************************************************
************************************************************/
#include "DrawImg.h"

/************************************************************
************************************************************/

/******************************
******************************/
DRAW_IMG::DRAW_IMG()
{
}

/******************************
******************************/
DRAW_IMG::~DRAW_IMG()
{
}

/******************************
******************************/
void DRAW_IMG::setup()
{
	img.load("img/img.png");
}

/******************************
******************************/
void DRAW_IMG::update()
{
}

/******************************
******************************/
void DRAW_IMG::draw(ofFbo& fbo)
{
	fbo.begin();
		ofEnableAntiAliasing();
		ofEnableBlendMode(OF_BLENDMODE_ADD); // OF_BLENDMODE_DISABLED, OF_BLENDMODE_ALPHA, OF_BLENDMODE_ADD
		
		ofSetColor(255, 255, 255, Gui_Global->img_alpha);
		img.draw(0, 0, fbo.getWidth(), fbo.getHeight());
	fbo.end();
}

