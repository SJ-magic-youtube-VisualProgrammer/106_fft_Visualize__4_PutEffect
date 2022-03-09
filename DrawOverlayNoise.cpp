/************************************************************
************************************************************/
#include "DrawOverlayNoise.h"

/************************************************************
************************************************************/

/******************************
******************************/
DRAW_OVERLAY_NOISE::DRAW_OVERLAY_NOISE()
{
}

/******************************
******************************/
DRAW_OVERLAY_NOISE::~DRAW_OVERLAY_NOISE()
{
}

/******************************
******************************/
void DRAW_OVERLAY_NOISE::setup(const FFT& fft)
{
	/********************
	********************/
	print_separatoin();
	printf("> setup : Draw OverlayNoise\n");
	fflush(stdout);
	
	/********************
	********************/
	VboSet.setup(AUDIO_BUF_SIZE * 4);
	
	Refresh_vboVerts();
	Refresh_vboColor(fft);
}

/******************************
1--2
|  |
0--3
******************************/
void DRAW_OVERLAY_NOISE::Refresh_vboVerts()
{
	const int height_of_bar = 2;
	int ofs = (MAIN_WINDOW_H - AUDIO_BUF_SIZE * height_of_bar) / 2;
	
	for(int i = 0; i < AUDIO_BUF_SIZE; i++){
		VboSet.set_vboVerts( i * 4 + 0, 0, height_of_bar * i + height_of_bar);
		VboSet.set_vboVerts( i * 4 + 1, 0, height_of_bar * i);
		VboSet.set_vboVerts( i * 4 + 2, MAIN_WINDOW_W, height_of_bar * i);
		VboSet.set_vboVerts( i * 4 + 3, MAIN_WINDOW_W, height_of_bar * i + height_of_bar);
	}
	
	VboSet.update_vertex();
}

/******************************
******************************/
void DRAW_OVERLAY_NOISE::Refresh_vboColor(const FFT& fft)
{
	float thresh = get_val_top_of_artsin_window();
	
	ofColor col = Gui_Global->OverlayNoise_col;
	
	for(int i = 0; i < AUDIO_BUF_SIZE; i++){
		float val = ofMap(fft.get_artSin(i), 0, thresh, 0, col.a, true);
		VboSet.setColor_perShape(4, i, ofColor(col.r, col.g, col.b, val));
	}
	
	VboSet.update_color();
}

/******************************
******************************/
void DRAW_OVERLAY_NOISE::update(const FFT& fft)
{
	Refresh_vboColor(fft);
}

/******************************
******************************/
void DRAW_OVERLAY_NOISE::draw(ofFbo& fbo)
{
	fbo.begin();
		/********************
		********************/
		ofEnableAntiAliasing();
		ofEnableBlendMode(OF_BLENDMODE_ADD); // OF_BLENDMODE_DISABLED, OF_BLENDMODE_ALPHA, OF_BLENDMODE_ADD
		
		ofSetColor(255);
		ofFill();
		VboSet.draw(GL_QUADS);
	fbo.end();
}

