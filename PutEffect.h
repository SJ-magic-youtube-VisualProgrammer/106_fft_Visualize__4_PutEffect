/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include <ofMain.h>
#include "ofxPostGlitch.h"

#include "fft.h"
#include <ofxVboSet.h>
#include "sj_common.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class PUT_EFFECT{
private:
	/****************************************
	****************************************/
	enum STATE{
		STATE__NORMAL,
		STATE__EFFECT,
	};
	
	/****************************************
	****************************************/
	STATE State = STATE__NORMAL;
	
	ofShader shader_Invert;
	float score = 0;
	
	ofImage img_Cursor;
	
	ofxPostGlitch	myGlitch;
	bool b_InvertShader = false;
	bool b_1st = true;
	
	/****************************************
	****************************************/
	void cal_Score(const FFT& fft);
	void StateChart();
	void EnableEffect();
	void DisableAllEffect();
	
public:
	PUT_EFFECT();
	~PUT_EFFECT();
	void setup();
	
	void update(const FFT& fft);
	void draw_score(ofFbo& fbo);
	void putEffect(ofFbo& fbo_src, ofFbo& fbo_Effected);
};

