/************************************************************
************************************************************/
#include "sj_common.h"

/************************************************************
************************************************************/
/********************
********************/
int GPIO_0 = 0;
int GPIO_1 = 0;

const float _PI = 3.1415;

/********************
********************/
GUI_GLOBAL* Gui_Global = NULL;

FILE* fp_Log = nullptr;


/************************************************************
func
************************************************************/
/******************************
******************************/
double LPF(double LastVal, double CurrentVal, double Alpha_dt, double dt)
{
	double Alpha;
	if((Alpha_dt <= 0) || (Alpha_dt < dt))	Alpha = 1;
	else									Alpha = 1/Alpha_dt * dt;
	
	return CurrentVal * Alpha + LastVal * (1 - Alpha);
}

/******************************
******************************/
double LPF(double LastVal, double CurrentVal, double Alpha)
{
	if(Alpha < 0)		Alpha = 0;
	else if(1 < Alpha)	Alpha = 1;
	
	return CurrentVal * Alpha + LastVal * (1 - Alpha);
}

/******************************
******************************/
double sj_max(double a, double b)
{
	if(a < b)	return b;
	else		return a;
}

/******************************
******************************/
bool checkIf_ContentsExist(char* ret, char* buf)
{
	if( (ret == NULL) || (buf == NULL)) return false;
	
	string str_Line = buf;
	Align_StringOfData(str_Line);
	vector<string> str_vals = ofSplitString(str_Line, ",");
	if( (str_vals.size() == 0) || (str_vals[0] == "") ){ // no_data or exist text but it's",,,,,,,".
		return false;
	}else{
		return true;
	}
}

/******************************
******************************/
void Align_StringOfData(string& s)
{
	size_t pos;
	while((pos = s.find_first_of(" 　\t\n\r")) != string::npos){ // 半角・全角space, \t 改行 削除
		s.erase(pos, 1);
	}
}

/******************************
******************************/
void print_separatoin()
{
	printf("---------------------------------\n");
}

/******************************
******************************/
void ClearFbo(ofFbo& fbo)
{
	fbo.begin();
		ofClear(0, 0, 0, 255);
	fbo.end();
}

/******************************
******************************/
float toRad(float val){
	return val * 3.1415 / 180.0;
}

/******************************
******************************/
float toDeg(float val){
	return val * 180.0 / 3.1415;
}

/******************************
******************************/
float get_val_top_of_artsin_window(){
	if(Gui_Global->ArtSin2D_BarHeight == 0) return 1e4;
	
	const float Window_H = 200.0;
	return 1.0 * Window_H / Gui_Global->ArtSin2D_BarHeight;
}

/************************************************************
class
************************************************************/

/******************************
******************************/
void GUI_GLOBAL::setup(string GuiName, string FileName, float x, float y)
{
	/********************
	********************/
	gui.setup(GuiName.c_str(), FileName.c_str(), x, y);
	
	/********************
	********************/
	Group_Audio.setup("Audio");
		Group_Audio.add(b_Audio_Start.setup("Start", false));
		Group_Audio.add(b_Audio_Stop.setup("Stop", false));
		Group_Audio.add(b_Audio_Reset.setup("Reset", false));
	gui.add(&Group_Audio);
	
	Group_CamParam.setup("CamParam");
		{
			ofVec3f initVec3f = ofVec3f(0, 0, 700);
			ofVec3f minVec3f = ofVec3f(-3000, -3000, -3000);
			ofVec3f maxVec3f = ofVec3f(3000, 3000, 3000);
			Group_CamParam.add(Cam_Pos.setup("cam:Pos", initVec3f, minVec3f, maxVec3f));
		}
		Group_CamParam.add(Cam_FOV.setup("cam:FOV", 60.0, 20.0, 120.0));
		Group_CamParam.add(Cam_NearClip.setup("cam:NearClip", 5.0, 0.0, 100.0));
		Group_CamParam.add(Cam_FarClip.setup("cam:FarClip", 4000.0, 0.0, 6000.0));
		Group_CamParam.add(Cam_LookAt_alignToSphere.setup("cam:LookAt = Sphere", false));
		{
			ofVec3f initVec3f = ofVec3f(0, 0, 0);
			ofVec3f minVec3f = ofVec3f(-100, -100, -100);
			ofVec3f maxVec3f = ofVec3f(100, 100, 100);
			Group_CamParam.add(Cam_LookAt.setup("cam:LookAt", initVec3f, minVec3f, maxVec3f));
		}
		{
			ofVec3f initVec3f = ofVec3f(0, 1, 0);
			ofVec3f minVec3f = ofVec3f(-1, -1, -1);
			ofVec3f maxVec3f = ofVec3f(1, 1, 1);
			Group_CamParam.add(Cam_Up.setup("cam:UpVector", initVec3f, minVec3f, maxVec3f));
		}
		Group_CamParam.add(Cam_AspectRatio.setup("cam:AspectRatio", 1.78, 0.5625, 2.0));
		Group_CamParam.add(Cam_Ortho.setup("cam:Ortho", false));
		Group_CamParam.add(Cam_V_Flip.setup("cam:V_Flip", false));
	gui.add(&Group_CamParam);
	
	Group_CamWork.setup("CamWork");
		Group_CamWork.add(b_CamWork.setup("camwork", false));
		Group_CamWork.add(CamWork__T_rotate.setup("camwork:T rot", 40, 1, 70));
		Group_CamWork.add(CamWork__r.setup("camwork:r", 700, 100, 3000));
		Group_CamWork.add(CamWork__T_perlin_for_y.setup("camwork:T of y", 40, 1, 70));
		Group_CamWork.add(CamWork__Amp_y.setup("camwork:amp y", 150, 0, 500));
	gui.add(&Group_CamWork);
	
	Group_FFT.setup("FFT");
		Group_FFT.add(FFT__SoftGain.setup("FFT__SoftGain", 1.0, 1.0, 5.0));
		Group_FFT.add(FFT__k_smooth.setup("FFT__k_smooth", 0.95, 0.8, 1.0));
		Group_FFT.add(FFT__dt_smooth_2.setup("FFT__dt_smooth_2", 167, 10, 300));
		Group_FFT.add(FFT__b_Window.setup("FFT__b_Window", true));
	gui.add(&Group_FFT);
	
	Group_ArtSin.setup("ArtSin");
		Group_ArtSin.add(ArtSin_Band_min.setup("ArtSin_Band_min", 1.0, 1.0, 255.0));
		Group_ArtSin.add(ArtSin_Band_max.setup("ArtSin_Band_max", 1.0, 1.0, 255.0));
		Group_ArtSin.add(ArtSin_PhaseMap_k.setup("ArtSin_PhaseMap_k", 1.0, 0.0, 2.0));
		Group_ArtSin.add(b_ArtSin_abs.setup("b_ArtSin_abs", false));
		Group_ArtSin.add(b_Window_artSin.setup("b_Window_artSin", false));
		Group_ArtSin.add(Tukey_alpha.setup("Tukey_alpha", 0.3, 0.0, 1.0));
	gui.add(&Group_ArtSin);
	
	Group_ArtSin2D.setup("ArtSin2D");
		Group_ArtSin2D.add(b_Draw_ArtSin2D.setup("ArtSin2D:b_Draw", true));
		Group_ArtSin2D.add(ArtSin2D_BarHeight.setup("ArtSin2D:BarHeight", 200, 0.0, 1000));
		{
			ofColor initColor = ofColor(255, 255, 255, 140);
			ofColor minColor = ofColor(0, 0, 0, 0);
			ofColor maxColor = ofColor(255, 255, 255, 255);
			Group_ArtSin2D.add(col_ArtSin2D.setup("ArtSin2D:col", initColor, minColor, maxColor));
		}
	gui.add(&Group_ArtSin2D);
	
	Group_ArtSinSphere.setup("ArtSinSphere");
		Group_ArtSinSphere.add(b_Draw_ArtSinSphere_Tri.setup("ArtSin3D:b_Draw_Tri", true));
		Group_ArtSinSphere.add(b_Draw_ArtSinSphere_Tri_Fill.setup("ArtSin3D:Fill", true));
		Group_ArtSinSphere.add(b_Draw_ArtSinSphere_Points.setup("ArtSin3D:b_Draw_Points", false));
		Group_ArtSinSphere.add(ArtSinShpere_Draw__Rotate_X.setup("draw Rotate_X", false));
		Group_ArtSinSphere.add(ArtSinShpere_Draw__Rotate_Y.setup("draw Rotate_Y", true));
		{
			ofColor initColor = ofColor(255, 255, 255, 140);
			ofColor minColor = ofColor(0, 0, 0, 0);
			ofColor maxColor = ofColor(255, 255, 255, 255);
			Group_ArtSinSphere.add(col_ArtSinSphere.setup("ArtSin3D:col", initColor, minColor, maxColor));
		}
		Group_ArtSinSphere.add(ArtSinSphere_r.setup("ArtSinSphere_r", 150, 0.0, 500));
		Group_ArtSinSphere.add(ArtSinSphere_Height.setup("ArtSinSphere_H", 80, 0.0, 500));
		
		Group_ArtSinSphere.add(ArtSinShpere_LineWidth.setup("ArtSin3D:LineWidth", 1.0, 1.0, 10.0));
		Group_ArtSinSphere.add(ArtSinShpere_PointSize.setup("ArtSin3D:PointSize", 1.5, 1.0, 10.0));
		Group_ArtSinSphere.add(ArtSinShpere_NumDraw.setup("ArtSin3D:NumDraw", 1.0, 1.0, 300.0));
		{
			ofVec3f initVec3f = ofVec3f(0, 65, 0);
			ofVec3f minVec3f = ofVec3f(-3000, -3000, -3000);
			ofVec3f maxVec3f = ofVec3f(3000, 3000, 3000);
			Group_ArtSinSphere.add(ArtSinSphere_pos.setup("ArtSin3D:Pos", initVec3f, minVec3f, maxVec3f));
		}
	gui.add(&Group_ArtSinSphere);
	
	Group_Sphere_core.setup("SphareCore");
		Group_Sphere_core.add(b_SphereCore_Draw.setup("SphereCore:draw", false));
		Group_Sphere_core.add(b_SphereCore_Draw_WireFrame.setup("draw Wireframe", true));
		Group_Sphere_core.add(SphereCore_SizeDown.setup("SphereCore:SizeDown", 0.3, 0.0, 1.0));
		Group_Sphere_core.add(SphereCore_Resolution.setup("SphereCore:Resolution", 16, 4, 64));
		Group_Sphere_core.add(SphereCore_LineWidth.setup("SphereCore:LineWidth", 1.0, 1.0, 10.0));
		Group_Sphere_core.add(b_SphereCore_AlignColor.setup("SphereCore:AlignColor", false));
		{
			ofColor initColor = ofColor(255, 255, 255, 140);
			ofColor minColor = ofColor(0, 0, 0, 0);
			ofColor maxColor = ofColor(255, 255, 255, 255);
			Group_Sphere_core.add(col_SphareCore.setup("SphereCore:col", initColor, minColor, maxColor));
		}
	gui.add(&Group_Sphere_core);
		
	Group_OverlayNoise.setup("OverlayNoise");
		Group_OverlayNoise.add(b_Draw_OverlayNoise.setup("Overlay:draw", false));
		{
			ofColor initColor = ofColor(255, 255, 255, 120);
			ofColor minColor = ofColor(0, 0, 0, 0);
			ofColor maxColor = ofColor(255, 255, 255, 255);
			Group_OverlayNoise.add(OverlayNoise_col.setup("Overlay:col", initColor, minColor, maxColor));
		}
	gui.add(&Group_OverlayNoise);
	
	Group_EFFECT.setup("Effect");
		Group_EFFECT.add(b_AutoEffect.setup("b_AutoEffect", false));
		Group_EFFECT.add(EffectType.setup("EffectType", 0, 0.0, 17.0));
		Group_EFFECT.add(Effect_thresh_0.setup("thresh_0", 0.35, 0.0, 1.0));
		Group_EFFECT.add(Effect_thresh_1.setup("thresh_1", 0.23, 0.0, 1.0));
		Group_EFFECT.add(Effect_k_smooth.setup("k_smooth", 0.6, 0.01, 1.0));
	gui.add(&Group_EFFECT);
	
	Group_img.setup("img");
		Group_img.add(img_alpha.setup("img:alpha", 20, 0.0, 255.0));
	gui.add(&Group_img);
	
	/********************
	********************/
	gui.minimizeAll();
}

