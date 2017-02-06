#ifndef COMMON_H
#define COMMON_H

#include <Fl/Fl.H>
#include <FL/gl.h> 
#include <string> 
#include <vector>
#include <sstream> 

#include "Rendering/ArcBall.h"
#include "Rendering/RenderingPrimitives.h" 

using namespace std; 

const Fl_Color WIN_COLOR = fl_rgb_color(244,247,251); 

namespace Str{
	double parseDouble(const string& str); 
	double parseInt(const string& str); 
	string toString(int i); 
	string toString(double  d); 
	bool isLetter(char c); 
	bool isNumber(char c); 
	vector<string> split(const string& s, char c); 
}

namespace Util{
	void convertMat(const ArcBall::Matrix3f_t& mi, Mat4& mout); 
	void mglLoadMatrix(const Mat4& mat); 
	void mglReadMatrix(GLenum glmat, Mat4& mat); 
	void mLoadMatrix(const Mat4& mat, GLdouble m[16]); 
	void mReadMatrix(GLdouble m[16], Mat4& mat); 
}

#define p4(v) v[0]<<" "<<v[1]<<" "<<v[2]<<" "<<v[3]

#define EPS32 1e-6

#endif