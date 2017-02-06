#include "Rendering/ObjectViewer.h"
#include "Common/Matrix.h"
#include "Common/Common.h" 
using namespace Util; 

#include <FL/gl.h> 
#include <FL/glu.h>

#include <iostream>
using namespace std; 

extern "C"{
#include "Common/bmpfile.h"
}

void ObjectViewer::updateModelView(){
	if(_scene){
		Mat4* mv = _view.getModelView(); 
		Mat4* trans = &_trans;
		Mat4* rot = &_thisRot; 
		(*mv) = (*rot)*(*trans); 
	}
}

ObjectViewer::ObjectViewer(string name,int x, int y, int w, int h, const char* l)
: Fl_Gl_Window(x,y,w,h,l){
	Fl::repeat_timeout(REFRESH_RATE,ObjectViewer::updateCb,this);
	_w = w; 
	_h = h; 
	this->border(5); 


	double* vp = _view.getViewport(); 
	vp[0]=0;
	vp[1]=0;
	vp[2]=_w;
	vp[3]=_h;

	_arcBall = new ArcBall::ArcBall_t((float)w,(float)h); 

	_scene = NULL; 

	openFile("files/"+name); 
	_openglRenderer.setShadingModel(SHADE_FLAT); 
}

void ObjectViewer::openFile(const string& fname){
	if(_scene) {
		delete _scene; 
	}
	_scene = RenderingUtils::readScene(fname); 

	if(!_scene)
		cout<<"Failed at reading "<<fname<<endl;
	else{
		_openglRenderer.setScene(_scene); 
		_openglRenderer.setView(&_view); 
	}
}

ObjectViewer::~ObjectViewer(){
	Fl::remove_timeout(ObjectViewer::updateCb,this); 
}

void ObjectViewer::draw(){
	if(!valid())
		init();

	if(_scene){
		glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT); 

		glMatrixMode(GL_PROJECTION); 
		glLoadIdentity(); 
		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity(); 
		_openglRenderer.draw();
		swap_buffers(); 
	}
}

int ObjectViewer::handle(int ev){

	if(ev==FL_MOVE){
		int x = Fl::event_x(); 
		int y = Fl::event_y(); 
		_prevMy = y;
	}
	else if(ev==FL_PUSH){
		int x = Fl::event_x(); 
		int y = Fl::event_y(); 
		if(Fl::event_button1()){
			handleRot(x,y,true); 
		}
		else if(Fl::event_button3()){
			handleZoom(x,y,true); 
		}
		_prevMy = y; 

		return 1; 
	}
	else if(ev==FL_DRAG){

		int x = Fl::event_x(); 
		int y = Fl::event_y(); 
		if(Fl::event_button1()){
			handleRot(x,y,false); 
		}
		else if(Fl::event_button3()){
			handleZoom(x,y,false); 
		}

		_prevMy = y; 

		return 1; 
	}
	else if(ev==FL_KEYUP){
		int key = Fl::event_key(); 
		if(key=='o'){
			if(Fl::event_ctrl()){
				// Control 'o' to load a model file
				char* newfile = fl_file_chooser("Open Model", ".txt (*.txt)", "./files", 0);
				if(newfile)
					openFile(string(newfile)); 
			}
		}
		else if(key=='f'){
			_openglRenderer.setShadingModel(SHADE_FLAT);  
		}
		else if(key=='g'){
			_openglRenderer.setShadingModel(SHADE_GOURAUD); 
		}
		else if(key=='n'){
			_openglRenderer.setShadingModel(SHADE_GOURAUD); 
		}
		else if(key=='0'){
			_openglRenderer.setColorMode(COLOR_Default,false);
		}
		else if(key=='1'){
			_openglRenderer.setColorMode(COLOR_Curvature_Max,false);
		}
		else if(key=='2'){
			_openglRenderer.setColorMode(COLOR_Curvature_Min,false);
		}
		else if(key=='3'){
			_openglRenderer.setColorMode(COLOR_Curvature_Mean,false);
		}
		else if(key=='4'){
			_openglRenderer.setColorMode(COLOR_Curvature_Gaussian,false);
		}
		else if(key=='5'){
			_openglRenderer.setColorMode(COLOR_Curvature_Max_Double,false);
		}
		else if(key=='6'){
			_openglRenderer.setColorMode(COLOR_Curvature_Min_Double,false);
		}
		else if(key=='7'){
			_openglRenderer.setColorMode(COLOR_Curvature_Mean_Double,false);
		}
		else if(key=='8'){
			_openglRenderer.setColorMode(COLOR_Curvature_Gaussian_Double,false);
		}
		else if(key=='9'){
			_openglRenderer.setColorMode(COLOR_NormalVector,false);
		}
		else if(key=='v'){
			//_openglRenderer.getsence()->outputNormalVector("output/test_SourceMeshNormalVector.txt",true);
			_openglRenderer.getsence()->outputNormalVector("output/test_TargetMeshNormalVector.txt",false);
		}
		else if(key=='d'){
			_openglRenderer.setColorMode(_openglRenderer.getcolorMode(),true);
		}
		else if(key=='m'){
			_openglRenderer.setColorMode(COLOR_Gauss_Map,true);
		}
		else if(key=='b'){
			_openglRenderer.getsence()->computeNomorlvectorBins(2.5,12);
		}
	}

	return Fl_Gl_Window::handle(ev);
}

void ObjectViewer::init(){
	glClearColor(.8f,.8f,.8f,1.f); 
	glEnable(GL_POLYGON_SMOOTH); 
	glEnable(GL_LINE_SMOOTH); 
	glEnable(GL_POINT_SMOOTH); 

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE); 
	glEnable(GL_CULL_FACE); 
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluPerspective(45.f,1.f,.1f,200.f); 
	mglReadMatrix(GL_PROJECTION_MATRIX,*_view.getProjection()); 

	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	glTranslatef(0,0,-2.f); 
	mglReadMatrix(GL_MODELVIEW_MATRIX,_trans); 

	updateModelView(); 
}

void ObjectViewer::resize(int x, int y, int width, int height){
	make_current();
	_w = width; 
	_h = height; 

	glViewport(0,0,_w,_h);
	if(_arcBall)
		_arcBall->setBounds((float)width,(float)height); 

	double* vp = _view.getViewport(); 
	vp[0]=0;
	vp[1]=0;
	vp[2]=_w;
	vp[3]=_h;

	Fl_Gl_Window::resize(x,y,width,height);
}

void ObjectViewer::handleRot(int x, int y, bool beginRot){
	if(beginRot){
		ArcBall::Tuple2f_t mousePt; 
		mousePt.s.X = (float) x; 
		mousePt.s.Y = (float) y; 

		_lastRot = _thisRot; 
		_arcBall->click(&mousePt);	
	}
	else{
		ArcBall::Tuple2f_t mousePt; 
		mousePt.s.X = (float)x; 
		mousePt.s.Y = (float)y; 

		ArcBall::Quat4fT thisQuat;
		ArcBall::Matrix3f_t thisRot; 
		_arcBall->drag(&mousePt, &thisQuat);					
		ArcBall::Matrix3fSetRotationFromQuat4f(&thisRot, &thisQuat);		

		convertMat(thisRot,_thisRot); 
		_thisRot = _lastRot * _thisRot; 
		updateModelView(); 
	}
}

// handles the camera zoom
void ObjectViewer::handleZoom(int x, int y, bool beginZoom){

	int dy = y - _prevMy;

	if(beginZoom){}
	else{
		_trans[3][2]+=dy*.01f; 
		updateModelView(); 
	}
}


void ObjectViewer::saveImageBufferCb(Fl_Widget* widget, void* userdata){
	ObjectViewer* viewer = (ObjectViewer*) userdata; 

	if(viewer){
		char* newfile = fl_file_chooser("Save image", ".bmp (*.bmp)", "./images", 0);
		if(!newfile) return; 

		int h = viewer->getHeight(); 
		int w = viewer->getWidth(); 

		GLubyte *data = new GLubyte[4*h*w]; 
		glReadPixels(0,0,w,h, GL_RGBA, GL_UNSIGNED_BYTE, data);

		bmpfile_t* bfile = bmp_create(w,h,32); 
		for(int j=0;j<h;j++){
			for(int i=0;i<w;i++){
				rgb_pixel_t pix = {data[(j*w+i)*4+2],data[(j*w+i)*4+1],data[(j*w+i)*4],data[(j*w+i)*4+3]}; 
				bmp_set_pixel(bfile,i,(h-1)-j,pix); 
			}
		}
		bmp_save(bfile,newfile); 
		bmp_destroy(bfile); 

		delete [] data; 
	}
}




