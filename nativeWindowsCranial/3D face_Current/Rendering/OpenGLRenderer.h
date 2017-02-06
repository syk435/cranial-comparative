#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include "Rendering/RenderingPrimitives.h" 

#define COLOR_Default 0
#define COLOR_Curvature_Max 1
#define COLOR_Curvature_Min 2
#define COLOR_Curvature_Mean 3
#define COLOR_Curvature_Gaussian 4
#define COLOR_Curvature_Max_Double 5
#define COLOR_Curvature_Min_Double 6
#define COLOR_Curvature_Mean_Double 7
#define COLOR_Curvature_Gaussian_Double 8
#define COLOR_NormalVector 9
#define COLOR_Gauss_Map 10


class OpenGLRenderer : public Renderer {
protected:
	int _colorMode;
public: 
	OpenGLRenderer() : Renderer(){}
	virtual void setView(View* view); 
	virtual void draw(); 
	Scene* getsence(){
		return _scene;
	}

	int getcolorMode(){
		return _colorMode;
	}

	void setColorMode(int m,bool outputFlag){
		_colorMode = m;
		cout<<"Set colorMode: " <<m<<endl;
		this->_scene->computeColorUnNormalized(outputFlag,m,2.50);
	}


protected: 
	virtual void initLights(); 
}; 

#endif