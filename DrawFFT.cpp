/************************************************************
************************************************************/
#include "DrawFFT.h"

/************************************************************
************************************************************/

/******************************
******************************/
DRAW_FFT::DRAW_FFT()
{
	font.load("font/RictyDiminished-Regular.ttf", 10, true, true, true);
}

/******************************
******************************/
DRAW_FFT::~DRAW_FFT()
{
}

/******************************
******************************/
void DRAW_FFT::setup(const FFT& fft)
{
	/********************
	********************/
	print_separatoin();
	printf("> setup : Draw FFT\n");
	fflush(stdout);
	
	/********************
	********************/
	VboSet.setup(AUDIO_BUF_SIZE/2 * 4);
	Refresh_vboVerts(fft);
	Refresh_vboColor(fft);
	VboSet.update_vertex_color();
}

/******************************
1--2
|  |
0--3
******************************/
void DRAW_FFT::Refresh_vboVerts(const FFT& fft)
{
	const ofVec2f ofs(0, 1060);
	const int width_of_bar = 4;
	const int space_per_bar = 7;
	const int BarHeight = 100;
	
	for (int i = 0; i < fft.get_sizeof_GainArray(); i++) {
		VboSet.set_vboVerts( i * 4 + 0, ofs.x + i * space_per_bar,					ofs.y );
		VboSet.set_vboVerts( i * 4 + 1, ofs.x + i * space_per_bar,					ofs.y - fft.get_GainSmoothed(i) * BarHeight );
		VboSet.set_vboVerts( i * 4 + 2, ofs.x + i * space_per_bar + width_of_bar,	ofs.y - fft.get_GainSmoothed(i) * BarHeight );
		VboSet.set_vboVerts( i * 4 + 3, ofs.x + i * space_per_bar + width_of_bar,	ofs.y );
	}
	
	VboSet.update_vertex();
}

/******************************
******************************/
void DRAW_FFT::Refresh_vboColor(const FFT& fft)
{
	ofColor col;
	
	for (int i = 0; i < fft.get_sizeof_GainArray(); i++) {
		if( (Gui_Global->ArtSin_Band_min <= i) && (i <= Gui_Global->ArtSin_Band_max) )	col = ofColor( 0, 0, 255, 150 );
		else																			col = ofColor( 255, 255, 255, 150 );
		
		VboSet.setColor_perShape(4, i, col); // setColor_perShape(int NumPerShape, int id, const ofColor& color);
	}
	
	VboSet.update_color();
}

/******************************
******************************/
void DRAW_FFT::update(const FFT& fft)
{
	Refresh_vboVerts(fft);
	Refresh_vboColor(fft);
}

/******************************
******************************/
void DRAW_FFT::draw(ofFbo& fbo)
{
	fbo.begin();
		/********************
		********************/
		ofEnableAntiAliasing();
		ofEnableBlendMode(OF_BLENDMODE_ALPHA); // OF_BLENDMODE_DISABLED, OF_BLENDMODE_ALPHA, OF_BLENDMODE_ADD
		
		/********************
		********************/
		//Draw background rect for spectrum
		const ofVec2f ofs(0, 1060);
		const int line_per = 5;
		const int BarHeight = 100;
		const int width_of_bar = 4;
		const int space_per_bar = 7;
		
		// 座布団 : spectrum
		ofSetColor(255, 255, 255, 5);
		ofFill();
		ofDrawRectangle( 0, ofs.y, fbo.getWidth(), -BarHeight );
		
		// 縦 line
		char buf[256];
		ofSetColor(255, 255, 255, 100);
		for(int i = 0; i * space_per_bar * line_per < fbo.getWidth(); i++){
			int _x = i * space_per_bar * line_per; // i 本目のlineのx座標
			ofDrawLine(_x, ofs.y, _x, ofs.y - BarHeight );
			
			sprintf( buf, "%d", i * line_per);
			font.drawString(buf, _x, ofs.y - BarHeight);
		}
		
		// 横 line
		/*
		if(b_HoriLine){
			ofSetColor(255, 0, 0, 200);
			ofDrawLine(0,ofGetAppPtr()-> mouseY, fbo.getWidth(), ofGetAppPtr()->mouseY);	// ofApp でないので、mouseYは使えず、ofGetAppPtr()->mouseY とする必要がある。
		}
		*/
		
		// vbo
		{
			ofFill();
			
			ofSetColor(255);
			// glPointSize(Gui_Global->particleSize);
			VboSet.draw(GL_QUADS);
		}
	fbo.end();
}
