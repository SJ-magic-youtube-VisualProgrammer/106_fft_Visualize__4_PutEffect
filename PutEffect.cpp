/************************************************************
************************************************************/
#include "PutEffect.h"

/************************************************************
************************************************************/

/******************************
******************************/
PUT_EFFECT::PUT_EFFECT()
{
	shader_Invert.load( "sj_shader/Invert.vert", "sj_shader/Invert.frag" );
}

/******************************
******************************/
PUT_EFFECT::~PUT_EFFECT()
{
}

/******************************
******************************/
void PUT_EFFECT::setup()
{
	img_Cursor.load("img_frame/Cursor_Invert.png");
	img_Cursor.setAnchorPercent(0, 0.5);
}

/******************************
******************************/
void PUT_EFFECT::update(const FFT& fft)
{
	cal_Score(fft);
	StateChart();
}

/******************************
******************************/
void PUT_EFFECT::cal_Score(const FFT& fft)
{
	/********************
	********************/
	float thresh = get_val_top_of_artsin_window();
	
	/* */
	float _score = 0;
	_score = 0;
	for(int i = 0; i < AUDIO_BUF_SIZE; i++){
		float val = ofMap(fft.get_artSin(i), 0, thresh, 0, 1.0, true);
		_score += val;
	}
	
	_score = _score / AUDIO_BUF_SIZE;
	
	/********************
	********************/
	score = Gui_Global->Effect_k_smooth * _score + (1 - Gui_Global->Effect_k_smooth) * score;
}

/******************************
******************************/
void PUT_EFFECT::StateChart()
{
	float Effect_thresh_0 = Gui_Global->Effect_thresh_0;
	float Effect_thresh_1 = Gui_Global->Effect_thresh_1;
	float thresh_H = max(Effect_thresh_0, Effect_thresh_1);
	float thresh_L = min(Effect_thresh_0, Effect_thresh_1);
	
	switch(State){
		case STATE__NORMAL:
			if( thresh_H < score ){
				State = STATE__EFFECT;
				
				if(Gui_Global->b_AutoEffect) EnableEffect();
			}
			break;
			
		case STATE__EFFECT:
			if( score < thresh_L ){
				State = STATE__NORMAL;
				DisableAllEffect();
			}
			break;
	}
}

/******************************
******************************/
void PUT_EFFECT::draw_score(ofFbo& fbo)
{
	const ofVec2f ofs_0(85, 900);
	const ofVec2f ofs_1(1815, 900);
	
	fbo.begin();
		/********************
		********************/
		ofEnableAntiAliasing();
		ofEnableBlendMode(OF_BLENDMODE_ADD); // OF_BLENDMODE_DISABLED, OF_BLENDMODE_ALPHA, OF_BLENDMODE_ADD
		
		/********************
		********************/
		const float BarHeight = 200;
		ofSetColor(255, 255, 255, 150);
		ofFill();
		ofDrawRectangle(ofs_0.x, ofs_0.y, 20, -score * BarHeight);
		ofDrawRectangle(ofs_1.x, ofs_1.y, 20, -score * BarHeight);
		
		/********************
		********************/
		ofSetColor(255, 255, 255, 150);
		
		float _y = Gui_Global->Effect_thresh_0 * BarHeight;
		img_Cursor.draw(ofs_1.x + 20, ofs_1.y - _y);
		
		_y = Gui_Global->Effect_thresh_1 * BarHeight;
		img_Cursor.draw(ofs_1.x + 20, ofs_1.y - _y);
	fbo.end();
}

/******************************
******************************/
void PUT_EFFECT::putEffect(ofFbo& fbo_src, ofFbo& fbo_Effected)
{
	/********************
	********************/
	if(b_1st){
		b_1st = false;
		myGlitch.setup(&fbo_Effected);
		DisableAllEffect();
	}
	
	/********************
	********************/
	fbo_Effected.begin();
	if( b_InvertShader ) shader_Invert.begin();
		ofEnableAntiAliasing();
		ofEnableBlendMode(OF_BLENDMODE_DISABLED); // OF_BLENDMODE_DISABLED, OF_BLENDMODE_ALPHA, OF_BLENDMODE_ADD
		
		ofClear(0, 0, 0, 255);
		ofSetColor(255);
		fbo_src.draw(0, 0, fbo_Effected.getWidth(), fbo_Effected.getHeight());
	if( b_InvertShader ) shader_Invert.end();
	fbo_Effected.end();
	
	/********************
	********************/
	myGlitch.generateFx();
}

/******************************
******************************/
void PUT_EFFECT::EnableEffect(){
	switch( int(Gui_Global->EffectType) ){
		case 0:
			b_InvertShader = true;
			break;
		case 1:
			myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE	, true);
			break;
		case 2:
			myGlitch.setFx(OFXPOSTGLITCH_GLOW			, true);
			break;
		case 3:
			myGlitch.setFx(OFXPOSTGLITCH_SHAKER			, true);
			break;
		case 4:
			myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER		, true);
			break;
		case 5:
			myGlitch.setFx(OFXPOSTGLITCH_TWIST			, true);
			break;
		case 6:
			myGlitch.setFx(OFXPOSTGLITCH_OUTLINE		, true);
			break;
		case 7:
			myGlitch.setFx(OFXPOSTGLITCH_NOISE			, true);
			break;
		case 8:
			myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN		, true);
			break;
		case 9:
			myGlitch.setFx(OFXPOSTGLITCH_SWELL			, true);
			break;
		case 10:
			myGlitch.setFx(OFXPOSTGLITCH_INVERT			, true);
			break;
		case 11:
			myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
			break;
		case 12:
			myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE	, true);
			break;
		case 13:
			myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE	, true);
			break;
		case 14:
			myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE	, true);
			break;
		case 15:
			myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT	, true);
			break;
		case 16:
			myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT	, true);
			break;
		case 17:
			myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT	, true);
			break;
	}
}

/******************************
******************************/
void PUT_EFFECT::DisableAllEffect(){
	b_InvertShader = false;
	
	myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE	, false);
	myGlitch.setFx(OFXPOSTGLITCH_GLOW			, false);
	myGlitch.setFx(OFXPOSTGLITCH_SHAKER			, false);
	myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER		, false);
	myGlitch.setFx(OFXPOSTGLITCH_TWIST			, false);
	myGlitch.setFx(OFXPOSTGLITCH_OUTLINE		, false);
	myGlitch.setFx(OFXPOSTGLITCH_NOISE			, false);
	myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN		, false);
	myGlitch.setFx(OFXPOSTGLITCH_SWELL			, false);
	myGlitch.setFx(OFXPOSTGLITCH_INVERT			, false);

	myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
	myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE	, false);
	myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE	, false);
	myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE	, false);
	myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT	, false);
	myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT	, false);
	myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT	, false);
}
