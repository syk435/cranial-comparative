#ifndef OBJECT_VIEWER_H
#define OBJECT_VIEWER_H

#include "FL/Fl_Gl_Window.H"
#include "FL/gl.h"
#include "FL/Fl.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_File_Chooser.H"

#include <vector> 
#include <fstream> 

#include "Rendering/ArcBall.h"
#include "Rendering/RenderingPrimitives.h" 
#include "OpenGLRenderer.h" 


#define REFRESH_RATE .001



class ObjectViewer : public Fl_Gl_Window{
protected: 
	int _w,_h; 

	ArcBall::ArcBall_t* _arcBall; 
	Scene* _scene; 
	View _view; 

	int _prevMy; 

	Mat4 _lastRot; 
	Mat4 _thisRot; 
	Mat4 _trans; 

	OpenGLRenderer _openglRenderer; 

public: 
	ObjectViewer(string name,int x, int y, int w, int h, const char* l=0);
	~ObjectViewer(); 
	void draw(); 
	int handle(int flag); 
	void init();


	static void updateCb(void* userdata){
		ObjectViewer* viewer = (ObjectViewer*) userdata; 
		viewer->redraw(); 
		Fl::repeat_timeout(REFRESH_RATE,ObjectViewer::updateCb,userdata);
	}

	void handleRot(int x, int y, bool beginRot); 
	void handleZoom(int x, int y, bool beginZoom); 
	void updateModelView(); 
	void resize(int x, int y, int width, int height);

	void change3dimage();


	static void saveImageBufferCb(Fl_Widget* widget,void* userdata); 

protected: 

	inline int getWidth() { return _w; } 
	inline int getHeight() { return _h; } 
	void openFile(const string& fname); 

};


#endif 