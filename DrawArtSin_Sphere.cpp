/************************************************************
************************************************************/
#include "DrawArtSin_Sphere.h"

/************************************************************
************************************************************/

/******************************
******************************/
DRAW_ARTSIN_SPHERE::DRAW_ARTSIN_SPHERE()
{
}

/******************************
******************************/
DRAW_ARTSIN_SPHERE::~DRAW_ARTSIN_SPHERE()
{
}

/******************************
******************************/
void DRAW_ARTSIN_SPHERE::setup(const FFT& fft)
{
	/********************
	********************/
	print_separatoin();
	printf("> setup : Draw ArtSin Sphere\n");
	fflush(stdout);
	
	/********************
	********************/
	VboSet_Points.setup(AUDIO_BUF_SIZE * 1);
	VboSet_Tri.setup(AUDIO_BUF_SIZE * 2 + 2);
	
	Refresh_vboVerts(fft);
	Refresh_vboColor(fft);
	
	LastCol = Gui_Global->col_ArtSinSphere;
}

/******************************
1--2
|  |
0--3
******************************/
void DRAW_ARTSIN_SPHERE::Refresh_vboVerts(const FFT& fft)
{
	/********************
	********************/
	float d_theta = 2 * _PI / AUDIO_BUF_SIZE;
	float theta = _PI * 3.0 / 2.0;
	
	const float r = Gui_Global->ArtSinSphere_r;
	const float H = Gui_Global->ArtSinSphere_Height;
	
	/********************
	********************/
	for(int i = 0; i < AUDIO_BUF_SIZE; i++){
		float val = r + fft.get_artSin(i) * H;
		if(Gui_Global->b_Draw_ArtSinSphere_Tri){
			VboSet_Tri.set_vboVerts(i * 2 + 0, r * cos(theta), 		r * sin(theta),		0);
			VboSet_Tri.set_vboVerts(i * 2 + 1, val * cos(theta),	val * sin(theta),	0);
		}
		if(Gui_Global->b_Draw_ArtSinSphere_Points){
			VboSet_Points.set_vboVerts(i, val * cos(theta),			val * sin(theta),	0);
		}
		
		theta -= d_theta;
	}
	
	float val = r + fft.get_artSin(0) * H;
	VboSet_Tri.set_vboVerts( (AUDIO_BUF_SIZE * 2 + 2) - 2, r * cos(theta), r * sin(theta), 0 );
	VboSet_Tri.set_vboVerts( (AUDIO_BUF_SIZE * 2 + 2) - 1, val * cos(theta), val * sin(theta), 0 );
	
	/********************
	********************/
	if(Gui_Global->b_Draw_ArtSinSphere_Tri)		VboSet_Tri.update_vertex();
	if(Gui_Global->b_Draw_ArtSinSphere_Points)	VboSet_Points.update_vertex();
}

/******************************
description
	本関数内を
		if(Gui_Global->b_Draw_ArtSin3D_Quads)
		if(Gui_Global->b_Draw_ArtSin3D_Points)
	で分岐してしまうと、color変更の瞬間に設定されていたmodeのみcolorをupdateすることになるので、
	ここは、分岐なし
******************************/
void DRAW_ARTSIN_SPHERE::Refresh_vboColor(const FFT& fft)
{
	/********************
	********************/
	VboSet_Tri.set_singleColor(Gui_Global->col_ArtSinSphere);
	VboSet_Points.set_singleColor(Gui_Global->col_ArtSinSphere);
	
	/********************
	以下を
		if(Gui_Global->b_Draw_ArtSin3D_Quads)
		if(Gui_Global->b_Draw_ArtSin3D_Points)
	で分岐してしまうと、color変更の瞬間に設定されていたmodeのみcolorをupdateすることになるので、
	ここは、分岐なし
	********************/
	VboSet_Tri.update_color();
	VboSet_Points.update_color();
	
	return;
}

/******************************
******************************/
void DRAW_ARTSIN_SPHERE::update(const FFT& fft)
{
	Refresh_vboVerts(fft);
	
	ofColor col_gui = Gui_Global->col_ArtSinSphere;
	if(col_gui != LastCol){
		Refresh_vboColor(fft);
		
		LastCol = Gui_Global->col_ArtSinSphere;
	}
}

/******************************
******************************/
void DRAW_ARTSIN_SPHERE::draw_core(ofFbo& fbo, ofCamera camera)
{
	/********************
	********************/
	if(Gui_Global->b_SphereCore_AlignColor){
		Gui_Global->b_SphereCore_AlignColor = false;
		Gui_Global->col_SphareCore = ofColor(Gui_Global->col_ArtSinSphere);
	}
	
	/********************
	********************/
	fbo.begin();
	camera.begin();
	
	ofPushMatrix();
		/********************
		********************/
		ofTranslate(Gui_Global->ArtSinSphere_pos);
		
		/********************
		********************/
		ofEnableAntiAliasing();
		ofEnableBlendMode(OF_BLENDMODE_ADD); // OF_BLENDMODE_DISABLED, OF_BLENDMODE_ALPHA, OF_BLENDMODE_ADD
		
		glLineWidth(Gui_Global->SphereCore_LineWidth);
		//glPointSize(Gui_Global->ArtSinShpere_PointSize);
		
		sphere_core.set(Gui_Global->ArtSinSphere_r * Gui_Global->SphereCore_SizeDown, Gui_Global->SphereCore_Resolution);
		sphere_core.setPosition(0, 0, 0);
		
		ofSetColor(Gui_Global->col_SphareCore);
		if(Gui_Global->b_SphereCore_Draw_WireFrame)	{ sphere_core.drawWireframe(); }
		else										{ ofFill(); sphere_core.draw(); }
	ofPopMatrix();
	
	camera.end();
	fbo.end();
}

/******************************
******************************/
void DRAW_ARTSIN_SPHERE::do_draw(ofFbo& fbo, ofCamera camera, bool b_Rotate_X)
{
	float d_theta = 180.0 / int(Gui_Global->ArtSinShpere_NumDraw);
	
	fbo.begin();
	camera.begin();
	
	ofPushMatrix();
		/********************
		********************/
		ofTranslate(Gui_Global->ArtSinSphere_pos);
		
		/********************
		********************/
		ofEnableAntiAliasing();
		ofEnableBlendMode(OF_BLENDMODE_ADD); // OF_BLENDMODE_DISABLED, OF_BLENDMODE_ALPHA, OF_BLENDMODE_ADD
		
		glLineWidth(Gui_Global->ArtSinShpere_LineWidth);
		glPointSize(Gui_Global->ArtSinShpere_PointSize);
		
		ofSetColor(255);
		
		int num = int(Gui_Global->ArtSinShpere_NumDraw);
		if(b_Rotate_X) num *= 2;
		for(int i = 0; i < num; i++){
			/********************
			********************/
			if(Gui_Global->b_Draw_ArtSinSphere_Tri){
				if(Gui_Global->b_Draw_ArtSinSphere_Tri_Fill)	ofFill();
				else											ofNoFill();
				
				VboSet_Tri.draw(GL_TRIANGLE_STRIP);
			}
			
			if(Gui_Global->b_Draw_ArtSinSphere_Points){
				VboSet_Points.draw(GL_POINTS);
			}
			
			/********************
			********************/
			if(b_Rotate_X)	ofRotateXDeg(d_theta);
			else			ofRotateYDeg(d_theta);
		}
	ofPopMatrix();
	
	camera.end();
	fbo.end();
}

/******************************
******************************/
void DRAW_ARTSIN_SPHERE::draw(ofFbo& fbo, ofCamera camera)
{
	/********************
	********************/
	if(Gui_Global->b_SphereCore_Draw)			draw_core(fbo, camera);
	
	if(Gui_Global->ArtSinShpere_Draw__Rotate_X)	do_draw(fbo, camera, true);
	if(Gui_Global->ArtSinShpere_Draw__Rotate_Y)	do_draw(fbo, camera, false);
}

