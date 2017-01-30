#ifndef BSP_RENDERER_H 
#define BSP_RENDERER_H 

#include "Rendering/RenderingPrimitives.h" 
#include <FL/gl.h>
#include <FL/glu.h> 

// TODO: write your own BSP renderer
class BSPRenderer : public Renderer {
protected: 
public: 
	BSPRenderer() : Renderer(){}
	virtual void draw(); 

	// override setView and setScene  to setup the 
	// scene in your own way and to build 
	// your own data structures (a bsp tree for example)

	// remember to call the parent's setView and setScene

	//virtual void setView(View* view); 
	//virtual void setScene(Scene* scene); 

protected: 
}; 

#endif
