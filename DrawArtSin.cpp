/************************************************************
************************************************************/
#include "DrawArtSin.h"

/************************************************************
************************************************************/

/******************************
******************************/
DRAW_ARTSIN::DRAW_ARTSIN()
{
	font.load("font/RictyDiminished-Regular.ttf", 20, true, true, false, 0.3f, 72);
}

/******************************
******************************/
DRAW_ARTSIN::~DRAW_ARTSIN()
{
}

/******************************
******************************/
void DRAW_ARTSIN::setup(const FFT& fft)
{
	/********************
	********************/
	print_separatoin();
	printf("> setup : Draw ArtSin 2D\n");
	fflush(stdout);
	
	img_Frame.load("img_frame/Frame.png");
	
	/********************
	********************/
	VboSet.setup(AUDIO_BUF_SIZE * 4);
	Refresh_vboVerts(fft);
	Refresh_vboColor();
	VboSet.update_vertex_color();
	
	LastCol = Gui_Global->col_ArtSin2D;
}

/******************************
1--2
|  |
0--3
******************************/
void DRAW_ARTSIN::Refresh_vboVerts(const FFT& fft)
{
	const float width_of_bar = 1;
	const float space_per_bar = 3;
	const float BarHeight = Gui_Global->ArtSin2D_BarHeight;
	
	for(int i = 0; i < AUDIO_BUF_SIZE; i++){
		float _y = sj_max( ofs_Graph.y - fft.get_artSin(i) * BarHeight, ofs_Graph.y - 200);
		
		VboSet.set_vboVerts( i * 4 + 0, ofs_Graph.x + i * space_per_bar,				ofs_Graph.y );
		VboSet.set_vboVerts( i * 4 + 1, ofs_Graph.x + i * space_per_bar,				_y );
		VboSet.set_vboVerts( i * 4 + 2, ofs_Graph.x + i * space_per_bar + width_of_bar,	_y);
		VboSet.set_vboVerts( i * 4 + 3, ofs_Graph.x + i * space_per_bar + width_of_bar,	ofs_Graph.y );
	}
	
	VboSet.update_vertex();
}

/******************************
******************************/
void DRAW_ARTSIN::Refresh_vboColor()
{
	VboSet.set_singleColor(Gui_Global->col_ArtSin2D);
	VboSet.update_color();
	
	return;
}

/******************************
******************************/
void DRAW_ARTSIN::update(const FFT& fft)
{
	/********************
	********************/
	Refresh_vboVerts(fft);
	
	/********************
	********************/
	ofColor col_gui = Gui_Global->col_ArtSin2D;
	if(col_gui != LastCol){
		Refresh_vboColor();
		LastCol = Gui_Global->col_ArtSin2D;
	}
}

/******************************
******************************/
void DRAW_ARTSIN::draw(ofFbo& fbo)
{
	fbo.begin();
		/********************
		********************/
		ofEnableAntiAliasing();
		ofEnableBlendMode(OF_BLENDMODE_ADD); // OF_BLENDMODE_DISABLED, OF_BLENDMODE_ALPHA, OF_BLENDMODE_ADD
		
		/********************
		********************/
		ofSetColor(255);
		img_Frame.draw(0, 0);
		
		/********************
		********************/
		ofFill();
		
		ofSetColor(255);
		// glPointSize(Gui_Global->particleSize);
		VboSet.draw(GL_QUADS);
		
		/********************
		********************/
		float thresh = get_val_top_of_artsin_window();
		
		ofSetColor(255);
		char buf[BUF_SIZE_S];
		sprintf(buf, "%.2f", thresh);
		font.drawString(buf, 134, 705);
		
	fbo.end();
}

