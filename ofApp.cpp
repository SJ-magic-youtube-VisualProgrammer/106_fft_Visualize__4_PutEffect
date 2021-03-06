/************************************************************
************************************************************/
#include "ofApp.h"

/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(int _soundStream_Input_DeviceId, int _soundStream_Output_DeviceId)
: b_EnableAudioOut(false)
, soundStream_Input_DeviceId(_soundStream_Input_DeviceId)
, soundStream_Output_DeviceId(_soundStream_Output_DeviceId)
{
}

/******************************
******************************/
ofApp::~ofApp()
{
	delete Gui_Global;
	
	if(fp_Log)	fclose(fp_Log);
}


/******************************
******************************/
void ofApp::exit(){
	soundStream->close();
	delete soundStream;
	
	printf("> Good-bye\n");
	fflush(stdout);
}

/******************************
******************************/
void ofApp::setup(){
	/********************
	********************/
	ofSetBackgroundAuto(true);
	
	ofSetWindowTitle("fft:Visualize");
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofSetWindowShape(MAIN_WINDOW_W, MAIN_WINDOW_H);
	ofSetEscapeQuitsApp(false);
	
	/********************
	********************/
	ofSeedRandom();
	
	/********************
	********************/
	fp_Log = fopen("../../../data/Log.csv", "w");
	
	setup_Gui();
	
	fbo_out.allocate(MAIN_WINDOW_W, MAIN_WINDOW_H, GL_RGBA, 4);
	ClearFbo(fbo_out);
	
	fbo_Effected.allocate(MAIN_WINDOW_W, MAIN_WINDOW_H, GL_RGBA, 4);
	ClearFbo(fbo_Effected);
	
	/********************
	********************/
	fft_thread.setup();
	
	/********************
	********************/
	print_separatoin();
	printf("> sound Device\n");
	fflush(stdout);
	
	/*
	settings.setInListener(this);
	settings.setOutListener(this);
	settings.sampleRate = 44100;
	settings.numInputChannels = 2;
	settings.numOutputChannels = 2;
	settings.bufferSize = bufferSize;
	
	soundStream.setup(settings);
	*/
	soundStream = new ofSoundStream();
	soundStream->printDeviceList();
	
	if( soundStream_Input_DeviceId == -1 ){
		ofExit();
		return;
		
	}else{
		/********************
			soundStream->setup()
		????????????audioIn()/audioOut()???start??????.
		????????????method??????fft_thread???access??????????????????????????????fft_thread??????????????????????????????????????????access????????????????????????.
		********************/
		setup_sound();
	}
	
	/********************
	********************/
	DrawFFT.setup(fft_thread);
	DrawImg.setup();
	DrawArtSin.setup(fft_thread);
	DrawArtSinSphere.setup(fft_thread);
	DrawOverlayNoise.setup(fft_thread);
	PutEffect.setup();
	
	/********************
	Set whether or not the aspect ratio of this camera is forced to a non-default setting.
	The camera's aspect ratio, by default, is the aspect ratio of your viewport. If you have set a non-default value (with ofCamera::setAspectRatio()), you can toggle whether or not this value is applied.
	********************/
	camera.setForceAspectRatio(true);
	
	set_CamParam();
	
	/********************
	********************/
	printf("> start process\n");
	fflush(stdout);
}

/******************************
******************************/
void ofApp::set_CamParam(){
	/********************
	********************/
	float now = float(ofGetElapsedTimeMillis()) / 1000.0;
	static float t_LastUpdate = 0;
	static float theta = 0;
	
	float dt = t_LastUpdate - now;
	
	/********************
	********************/
	if(Gui_Global->Cam_LookAt_alignToSphere){
		Gui_Global->Cam_LookAt_alignToSphere = false;
		Gui_Global->Cam_LookAt = ofVec3f(Gui_Global->ArtSinSphere_pos);
	}
	
	/********************
	********************/
	if(Gui_Global->b_CamWork){
	/*
		float t = fmod(now, Gui_Global->CamWork__T_rotate/1000);
		theta = ofMap(t, 0, Gui_Global->CamWork__T_rotate/1000, 0, 2 * _PI);
		*/
		float d_theta = 2 * _PI / Gui_Global->CamWork__T_rotate * dt;
		theta += d_theta;
		while(2 * _PI <= theta) theta -= 2 * _PI;
		
		float _x = -Gui_Global->CamWork__r * sin(theta);
		float _z =  Gui_Global->CamWork__r * cos(theta);
		
		float _y = Gui_Global->CamWork__Amp_y * ofSignedNoise(now / Gui_Global->CamWork__T_perlin_for_y + 12.34 );
		
		
		camera.setPosition(glm::vec3(_x, _y, _z));
	}else{
		camera.setPosition(Gui_Global->Cam_Pos);
	}
	
	/********************
	lookAt??????pos??????????????????????????????
	********************/
	camera.setNearClip(Gui_Global->Cam_NearClip);
	camera.setFarClip(Gui_Global->Cam_FarClip);
	
	if(camera.getOrtho() && !Gui_Global->Cam_Ortho){
		camera.disableOrtho();
	}else if(!camera.getOrtho() && Gui_Global->Cam_Ortho){
		camera.enableOrtho();
	}
	
	if( (camera.isVFlipped() && !Gui_Global->Cam_V_Flip) || (!camera.isVFlipped() && Gui_Global->Cam_V_Flip) ){
		camera.setVFlip(Gui_Global->Cam_V_Flip);
	}
	
	camera.setFov(Gui_Global->Cam_FOV);
	camera.setAspectRatio(Gui_Global->Cam_AspectRatio);
	
	camera.lookAt(Gui_Global->Cam_LookAt, Gui_Global->Cam_Up);
	
	/********************
	********************/
	t_LastUpdate = now;
}

/******************************
description
	memory???????????????app start?????????????????????
	segmentation fault???????????????????????????
******************************/
void ofApp::setup_Gui()
{
	/********************
	********************/
	Gui_Global = new GUI_GLOBAL;
	Gui_Global->setup("param", "gui.xml", 10, 10);
}

/******************************
******************************/
void ofApp::setup_sound(){
	/********************
	********************/
	vector<ofSoundDevice> devices = soundStream->getDeviceList();
	
	ofSoundStreamSettings settings;
	
	settings.setInDevice(devices[soundStream_Input_DeviceId]);
	if(devices[soundStream_Input_DeviceId].name.find("Native Instruments") != std::string::npos){
		/********************
		"name" ???Device???????????????????????????
		??????block?????????????????????????????????
		********************/
		printf("> name match : Native Instruments selected\n");
		fflush(stdout);
	}
	
	if( soundStream_Output_DeviceId != -1 ){
		settings.setOutDevice(devices[soundStream_Output_DeviceId]);
		b_EnableAudioOut = true;
	}

	settings.setInListener(this);
	settings.numInputChannels = AUDIO_IN_CHS;
	
	if(b_EnableAudioOut){
		settings.setOutListener(this); /* Don't forget this */
		settings.numOutputChannels = AUDIO_OUT_CHS;
	}else{
		settings.numOutputChannels = 0;
	}
	
	settings.sampleRate = AUDIO_SAMPLERATE;
	settings.bufferSize = AUDIO_BUF_SIZE;
	settings.numBuffers = AUDIO_BUFFERS; // ????????????????????????
	
	/********************
		soundStream->setup()
	????????????audioIn()/audioOut()???start??????.
	????????????method??????fft_thread???access??????????????????????????????fft_thread??????????????????????????????????????????access????????????????????????.
	********************/
	soundStream->setup(settings);
}

/******************************
******************************/
void ofApp::update(){
	/********************
	********************/
	if(Gui_Global->b_Audio_Start)	{ Gui_Global->b_Audio_Start = false; Sound__Start(); }
	if(Gui_Global->b_Audio_Stop)	{ Gui_Global->b_Audio_Stop = false; Sound__Stop(); }
	if(Gui_Global->b_Audio_Reset)	{ Gui_Global->b_Audio_Reset = false; Sound__Reset(); }
	
	/********************
	********************/
	set_CamParam();
	
	/********************
	********************/
	fft_thread.update();
	
	DrawFFT.update(fft_thread);
	DrawImg.update();
	DrawArtSin.update(fft_thread);
	DrawArtSinSphere.update(fft_thread);
	DrawOverlayNoise.update(fft_thread);
	PutEffect.update(fft_thread);
}

/******************************
******************************/
void ofApp::draw(){
	/********************
	********************/
	ClearFbo(fbo_out);
	ClearFbo(fbo_Effected);
	
	/********************
	********************/
	DrawImg.draw(fbo_out);
	DrawFFT.draw(fbo_out);
	if(Gui_Global->b_Draw_ArtSin2D)	DrawArtSin.draw(fbo_out);
	DrawArtSinSphere.draw(fbo_out, camera);
	if(Gui_Global->b_Draw_OverlayNoise)	DrawOverlayNoise.draw(fbo_out);
	if(Gui_Global->b_Draw_ArtSin2D)	PutEffect.draw_score(fbo_out);
	
	/********************
	********************/
	PutEffect.putEffect(fbo_out, fbo_Effected);
	
	/********************
	********************/
	ofEnableAntiAliasing();
	ofEnableBlendMode(OF_BLENDMODE_DISABLED); // OF_BLENDMODE_DISABLED, OF_BLENDMODE_ALPHA, OF_BLENDMODE_ADD
	
	ofBackground(0, 0, 0, 255);
	ofSetColor(255, 255, 255, 255);
	
	fbo_Effected.draw(0, 0);
	
	/********************
	********************/
	if(Gui_Global->b_Disp){
		Gui_Global->gui.draw();
		
		ofSetColor(255);
		string info;
		info += "FPS = " + ofToString(ofGetFrameRate(), 2) + "\n";
		ofDrawBitmapString(info, 30, 30);
	}
}

/******************************
******************************/
void ofApp::audioIn(ofSoundBuffer & buffer){
	fft_thread.SetVol(buffer);
}

/******************************
******************************/
void ofApp::audioOut(ofSoundBuffer & buffer){
	fft_thread.GetVol(buffer, b_EnableAudioOut);
}

/******************************
******************************/
void ofApp::Sound__Start(){
	soundStream->start();
	printf("> soundStream : start\n");
	fflush(stdout);
}

/******************************
******************************/
void ofApp::Sound__Stop(){
	soundStream->stop();
	printf("> soundStream : stop\n");
	fflush(stdout);
}

/******************************
******************************/
void ofApp::Sound__Reset(){
	soundStream->close();
	delete soundStream;
	
	soundStream = new ofSoundStream();
	setup_sound();
	
	printf("> soundStream : close->restart\n");
	fflush(stdout);
}

/******************************
******************************/
void ofApp::keyPressed(int key){
	switch(key){
		case 'd':
			Gui_Global->b_Disp = !Gui_Global->b_Disp;
			break;
			
		case 'm':
			Gui_Global->gui.minimizeAll();
			break;
	}
}

/******************************
******************************/
void ofApp::keyReleased(int key){
}

/******************************
******************************/
void ofApp::mouseMoved(int x, int y ){

}

/******************************
******************************/
void ofApp::mouseDragged(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mousePressed(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseReleased(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseEntered(int x, int y){

}

/******************************
******************************/
void ofApp::mouseExited(int x, int y){

}

/******************************
******************************/
void ofApp::windowResized(int w, int h){

}

/******************************
******************************/
void ofApp::gotMessage(ofMessage msg){

}

/******************************
******************************/
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
