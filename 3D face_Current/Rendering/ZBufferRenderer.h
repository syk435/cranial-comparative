#ifndef ZBUFFER_RENDERER_H
#define ZBUFFER_RENDERER_H

#include "Rendering/RenderingPrimitives.h" 
#include <FL/gl.h>
#include <FL/glu.h> 

//TODO: write your own z buffer renderer
class ZBufferRenderer : public Renderer {
protected: 
	GLfloat* _pixels;
	int _width, _height; 
public: 
	ZBufferRenderer() : Renderer(){ 
		_pixels = NULL; 
	}
	~ZBufferRenderer() { 
		if(_pixels) delete [] _pixels; 
	}

	virtual void draw(); 


	// override setView and setScene  to setup the 
	// scene in your own way and to build 
	// your own data structures (a z buffer for example)

	// remember to call the parent's setView and setScene

	//virtual void setView(View* view); 
	//virtual void setScene(Scene* scene);

protected: 
	virtual void rasterize(); 
	virtual void setPixel(int x, int y, const Color& c); 
	virtual void setPixel(const Grid2& p, const Color& c); 

}; 

#endif
