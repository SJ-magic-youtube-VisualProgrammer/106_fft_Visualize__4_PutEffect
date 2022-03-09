/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include <ofMain.h>
#include "fft.h"
#include <ofxVboSet.h>
#include "sj_common.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class DRAW_ARTSIN_SPHERE{
private:
	/****************************************
	****************************************/
	ofColor LastCol;
	ofx__VBO_SET VboSet_Tri;
	ofx__VBO_SET VboSet_Points;
	
	ofSpherePrimitive sphere_core;
	
	/****************************************
	****************************************/
	void Refresh_vboVerts(const FFT& fft);
	void Refresh_vboColor(const FFT& fft);
	void draw_core(ofFbo& fbo, ofCamera camera);
	void do_draw(ofFbo& fbo, ofCamera camera, bool b_Rotate_X);
	
public:
	DRAW_ARTSIN_SPHERE();
	~DRAW_ARTSIN_SPHERE();
	
	void setup(const FFT& fft);
	void update(const FFT& fft);
	void draw(ofFbo& fbo, ofCamera camera);
};

