/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include <stdio.h>

#include <ofMain.h>
#include <ofxGui.h>

/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);

/************************************************************
************************************************************/
enum{
	MAIN_WINDOW_W = 1920,
	MAIN_WINDOW_H = 1080,
};

enum{
	BUF_SIZE_S = 500,
	BUF_SIZE_M = 1000,
	BUF_SIZE_L = 6000,
	BUF_SIZE_LL = 100000,
	UDP_BUF_SIZE = 100000,
};

enum{
	AUDIO_BUF_SIZE = 512,
	
	AUDIO_OUT_CHS = 2,
	AUDIO_IN_CHS = 2,
	AUDIO_BUFFERS = 2,			// 使われてないっぽい
	AUDIO_SAMPLERATE = 44100,
};

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class GUI_GLOBAL{
private:
	/****************************************
	****************************************/
	
public:
	/****************************************
	****************************************/
	void setup(string GuiName, string FileName = "gui.xml", float x = 10, float y = 10);
	
	ofxGuiGroup Group_Audio;
		ofxToggle b_Audio_Start;
		ofxToggle b_Audio_Stop;
		ofxToggle b_Audio_Reset;
		
	ofxGuiGroup Group_CamParam;
		ofxVec3Slider Cam_Pos;
		ofxFloatSlider Cam_FOV;
		ofxFloatSlider Cam_NearClip;
		ofxFloatSlider Cam_FarClip;
		ofxToggle Cam_LookAt_alignToSphere;
		ofxVec3Slider Cam_LookAt;
		ofxVec3Slider Cam_Up;
		ofxFloatSlider Cam_AspectRatio;
		ofxToggle Cam_Ortho;
		ofxToggle Cam_V_Flip;
		
	ofxGuiGroup Group_CamWork;
		ofxToggle b_CamWork;
		ofxFloatSlider CamWork__T_rotate;
		ofxFloatSlider CamWork__r;
		ofxFloatSlider CamWork__T_perlin_for_y;
		ofxFloatSlider CamWork__Amp_y;
	
	ofxGuiGroup Group_FFT;
		ofxFloatSlider FFT__SoftGain;
		ofxFloatSlider FFT__k_smooth;
		ofxFloatSlider FFT__dt_smooth_2;
		ofxToggle FFT__b_Window;
		
	ofxGuiGroup Group_ArtSin;
		ofxFloatSlider ArtSin_Band_min;
		ofxFloatSlider ArtSin_Band_max;
		ofxFloatSlider ArtSin_PhaseMap_k;
		ofxToggle b_ArtSin_abs;
		ofxToggle b_Window_artSin;
		ofxFloatSlider Tukey_alpha;
		
	ofxGuiGroup Group_ArtSin2D;
		ofxToggle b_Draw_ArtSin2D;
		ofxFloatSlider ArtSin2D_BarHeight;
		ofxColorSlider col_ArtSin2D;
		
	ofxGuiGroup Group_ArtSinSphere;
		ofxToggle b_Draw_ArtSinSphere_Tri;
		ofxToggle b_Draw_ArtSinSphere_Tri_Fill;
		ofxToggle b_Draw_ArtSinSphere_Points;
		ofxToggle ArtSinShpere_Draw__Rotate_X;
		ofxToggle ArtSinShpere_Draw__Rotate_Y;
		ofxColorSlider col_ArtSinSphere;
		ofxFloatSlider ArtSinSphere_r;
		ofxFloatSlider ArtSinSphere_Height;
		ofxFloatSlider ArtSinShpere_LineWidth;
		ofxFloatSlider ArtSinShpere_PointSize;
		ofxFloatSlider ArtSinShpere_NumDraw;
		ofxVec3Slider ArtSinSphere_pos;
		
	ofxGuiGroup Group_Sphere_core;
		ofxToggle b_SphereCore_Draw;
		ofxToggle b_SphereCore_Draw_WireFrame;
		ofxFloatSlider SphereCore_SizeDown;
		ofxFloatSlider SphereCore_Resolution;
		ofxFloatSlider SphereCore_LineWidth;
		ofxToggle b_SphereCore_AlignColor;
		ofxColorSlider col_SphareCore;
	
	ofxGuiGroup Group_OverlayNoise;
		ofxToggle b_Draw_OverlayNoise;
		ofxColorSlider OverlayNoise_col;
	
	ofxGuiGroup Group_EFFECT;
		ofxToggle b_AutoEffect;
		ofxFloatSlider EffectType;
		ofxFloatSlider Effect_thresh_0;
		ofxFloatSlider Effect_thresh_1;
		ofxFloatSlider Effect_k_smooth;
		
	ofxGuiGroup Group_img;
		ofxFloatSlider img_alpha;
		
	ofxPanel gui;
	
	bool b_Disp = false;
};

/************************************************************
************************************************************/
double LPF(double LastVal, double CurrentVal, double Alpha_dt, double dt);
double LPF(double LastVal, double CurrentVal, double Alpha);
double sj_max(double a, double b);

bool checkIf_ContentsExist(char* ret, char* buf);
void Align_StringOfData(string& s);
void print_separatoin();
void ClearFbo(ofFbo& fbo);

float toRad(float val);
float toDeg(float val);

float get_val_top_of_artsin_window();

/************************************************************
************************************************************/
extern GUI_GLOBAL* Gui_Global;

extern FILE* fp_Log;

extern int GPIO_0;
extern int GPIO_1;

extern const float _PI;

/************************************************************
************************************************************/

