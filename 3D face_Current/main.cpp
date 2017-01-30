//#include "GUI/MainWindow.h" 
#include "Common/Common.h" 
//#include "GUI/FrameWindow.h" 
#include "Rendering/ObjectViewer.h" 
#include "FL/Fl.H"
#include "FL/Fl_Button.H"

#include <iostream> 
#include <string> 
#include <vector> 
using namespace std; 

void escapeButtonCb(Fl_Widget* widget, void* win){
	exit(1);
}

int main(int argc, char *argv[]) { 


	Fl_Window m(50,50,930,930+30*2,"Hidden Surface Removal"); 
	m.callback(escapeButtonCb,&m); 
	m.color(WIN_COLOR);

	ObjectViewer w2(argv[1],10,10,900,900,"opengl"); 
	w2.box(FL_FLAT_BOX); 
	w2.color(FL_BLACK); 
	w2.end();  

	Fl_Button* saveImageBt = new Fl_Button(410, 920, 100, 20, "Save Image");
	saveImageBt->callback(ObjectViewer::saveImageBufferCb, &w2);
	saveImageBt->color(WIN_COLOR); 
	saveImageBt->box(FL_BORDER_BOX); 

	
	//w2.show(); 
	m.end(); 
	m.show(); 
	return Fl::run(); 
}