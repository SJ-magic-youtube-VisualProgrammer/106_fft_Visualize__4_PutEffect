//Specify compiler to use GLSL version 1.2
//Enable working with textures of any dimensions
//Declare texture texture0, which is linked when you use fbo.bind(), or any other texture.bind().

//#version 430 compatibility
#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect texture0;     //it can be named in any way, GLSL just links it
uniform sampler2DRect texture1;		//spectrum

uniform float window_w = 1920;
uniform float window_h =1080;
uniform float space = 7.0;

// #define N (256)
// uniform float specArray[N];

void main(){
    vec2 pos = gl_TexCoord[0].st;     
	
    vec4 color0 = texture2DRect(texture0, pos);
	
	vec2 center = vec2(window_w, window_h) / 2.0;
	float dist = distance( center, pos );	//built-in function for distance 

	//Antialiasing
	vec2 spectrumPos = vec2( dist / space, 0.5 );
	vec4 color1 =  texture2DRect(texture1, spectrumPos);	//spectrum values
	float spectrValue = color1.r;		//get spectrum value, normally in [0..1] 

	vec2 delta = pos - center;
	delta *= 1-2*spectrValue;
	vec2 posS = center + delta;

	//Audio responsible
	color0 = texture2DRect(texture0, posS);
	
	/*
	vec4 color;
	//Case: Example
	color = min( color0, color1 );	
	*/
	
	gl_FragColor = color0 * gl_Color;
}
