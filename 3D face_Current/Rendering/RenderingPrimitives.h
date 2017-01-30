#ifndef RENDERING_PRIMITIVES_H
#define RENDERING_PRIMITIVES_H
#define CURVATUREMODE 'M'
#define PI 3.14159


#include <iostream> 
#include <fstream>
#include "Common/Matrix.h"
#include <vector>
#include <set>
#include <string> 
#include <map>
#include <math.h>
#include <random>
#include <FL/gl.h>
#include <FL/glut.h> 
#include <FL/glu.h>
#include "Geometry.h"

using namespace std;

const double moveLength = 1;



// using a self-made dynamic array might make things a little faster
template<typename T>
class DynArray{
protected: 
	T* _data;
	int _size, _cap; 
public: 
	DynArray() { 
		_data = NULL; 
		_size = 0; 
		_cap = 0; 
		recap(10); 
	}

	~DynArray(){
		delete [] _data; 
	}


	void resize(int s){
		if(s<_cap){
			_size = s; 
		}
		else{
			recap(_cap*1.5); 
			resize(s);
		}
	}

	int size(){ return _size; }

	void clear() { 
		_size = 0; 
		delete [] _data; 
		_data = NULL; 
		resize(10); 
	}

	int add(const T& p){
		if(_size>=_cap)
			recap((int)(_cap*1.5)); 
		_data[_size++] = p; 
		return _size-1;
	}

	T* get(int i){
		if(i<_size)
		{
			return &_data[i]; 
		}
		cout<<"Wrong parameter:"<<i<<endl;
		return NULL; 
	}

	void setValuePos(int i,T value){
		_data[i] = value;
	}

	T* getData() { return _data; }

protected: 
	void recap(int r){
		_cap = r; 
		T* ndata = new T[_cap]; 
		if(_data!=NULL){
			for(int j=0;j<_size;j++)
				ndata[j] = _data[j]; 
				delete [] _data;
		}
		_data = ndata; 
	}

}; 

typedef DynArray<Pt3> Pt3Array; 
typedef DynArray<Vec3> Vec3Array; 
typedef DynArray<Color> ColorArray; 

typedef Vector<int,3> TriInd;   
//ybh
typedef Vector<double,3> TriAngles;

typedef Vector<int,4> QuadInd; // you will probably not use this

typedef DynArray<int> IndArray; 
typedef DynArray<TriInd> TriIndArray; 
typedef DynArray<QuadInd> QuadIndArray; 
//ybh
typedef DynArray<TriAngles> AnglesArray;


class Material  {
protected: 
	Color _ambient, _diffuse, _specular; 
	float _specExp; 
public: 
	inline Material() {}; 

	inline const Color& getAmbient() const { return _ambient; }
	inline const Color& getDiffuse() const { return _diffuse; }
	inline const Color& getSpecular() const { return _specular; }
	inline float getSpecExponent() const{ return _specExp; }

	inline void setAmbient(const Color& amb)  { _ambient = amb; }
	inline void setDiffuse(const Color& diff)  { _diffuse = diff; }
	inline void setSpecular(const Color& spec)  { _specular = spec; }
	inline void setSpecExponent(float s) { _specExp = s; }
}; 

class Light { 
protected: 
	unsigned int _id; 
	Pt3 _pos; 
	Color _color;
	Color _ambient; // keep ambient is easier to code than using a global ambient
public: 
	inline Light(){
		_color = Color(1,1,1); 
	}

	inline Light(const Pt3& pos, const Color& color)
		: _pos(pos), _color(color){}

	inline const Pt3& getPos() const{ return _pos; }
	inline const Color& getColor() const{ return _color; }
	inline const Color& getAmbient() const{ return _ambient; }
	inline unsigned int getId() { return _id; }

	inline void setPos(const Pt3& p) { _pos = p; }
	inline void setColor(const Color& c) { _color = c; }
	inline void setAmbient(const Color& c) { _ambient = c; }
	inline void setId(unsigned int id) { _id = id; }
};

class Mesh{
protected: 
	Pt3Array* _pts; 
	Vec3Array* _vnormals; 
	Vec3Array* _fnormals; 
	TriIndArray* _tinds; // mesh is made of triangles
	ColorArray* _vcolors;
	Curvature* _gaussK;
	Curvature* _meanK;
	Curvature* _maxPrincipleK;
	Curvature* _minPrincipleK;
	ColorArray* _color;
	map<Pt3,int> evenDistributedVertices;

public: 

	Mesh(){
		_pts = NULL; 
		_vnormals = NULL; 
		_fnormals = NULL; 
		_tinds = NULL; 
		_vcolors = NULL;
		_gaussK = NULL;
		_meanK = NULL;
		_maxPrincipleK = NULL;
		_minPrincipleK = NULL;
		_color = NULL;
	}

	Mesh(Pt3Array* pts, TriIndArray* inds){
		_pts = pts; 
		_tinds = inds; 
	}

	void setColor(ColorArray* color){
		_color = color;
	}


	void setPoints(Pt3Array* p){
		_pts = p; 	
	}

	void setInds(TriIndArray* ti){
		_tinds = ti; 
	}

 	void setVNormals(Vec3Array* n){
		_vnormals = n; 
	}

 	void setFNormals(Vec3Array* n){
		_fnormals = n; 
	}
    
	void setgaussK(Curvature* c){
		_gaussK=c;
	}
	
	void setmeanK(Curvature* c){
		_meanK=c;
	}

	void setmaxPrincipleK(Curvature* c){
		_maxPrincipleK=c;
	}

	void setminPrincipleK(Curvature* c){
		_minPrincipleK=c;
	}

	void setColorPos(int index,Color c){
		_color->setValuePos(index,c);
	}

	void loadGaussK(string filename);

	void loadMeanK(string filename);

	void loadMaxPrincipleK(string filename);

	void loadMinPrincipleK(string filename);

	void loadEvenDistributedVertices(string filename);

	void printKvalue()
	{
		for(int i=0; i<_pts->size();i++)
		{
			//cout<<"G:"<<_gaussK.buffer[i]<<" M:"<<_meanK.buffer[i]<<endl;
		}	
	}

	void NomorlizeKvalue();


	Pt3Array* getPoints() { return _pts; }
	TriIndArray* getInds() { return _tinds; }
	Vec3Array* getVNormals() { return _vnormals; }	
	Vec3Array* getFNormals() { return _fnormals; }
	ColorArray* getColor() { return _color;}
	Curvature* getMaxCurvature() { return _maxPrincipleK;}
	Curvature* getMinCurvature() { return _minPrincipleK;}
	Curvature* getMeanCurvature() { return _meanK;}
	Curvature* getGaussianCurvature() { return _gaussK;}


	// use these delete functions carefully
	void del() { 
		delPts(); 
		delInds(); 
		delVNormals(); 
		delFNormals(); 
	}

protected: 
	void delInds() { 
		if(_tinds)
			delete _tinds; 
		_tinds = NULL; 
	}

	void delPts() { 
		if(_pts)
			delete _pts; 
		_pts = NULL; 
	}

	void delVNormals() { 
		if(_vnormals)
			delete _vnormals; 
		_vnormals = NULL; 
	}

	void delFNormals() { 
		if(_fnormals)
			delete _fnormals; 
		_fnormals = NULL; 
	}
}; 


class View{
protected: 
	Mat4 _modelview; 
	Mat4 _proj; 
	double _viewport[4]; 

public: 

	View(){
		_viewport[0] = _viewport[1] = 0; 
		_viewport[2] = _viewport[3] = 1; 
	}

	Mat4* getModelView() { return &_modelview; }
	Mat4* getProjection(){ return &_proj; }
	double* getViewport() { return _viewport; }
}; 

class Scene{
protected: 
	Material* _mat; 
	std::vector<Light*> _lights; 
	Mesh* _targetMesh;
	Mesh* _sourceMesh;
	vector<int>* _cpdMap;
	bool _showDoubleMesh;

public: 

	Scene() { 
		_mat = NULL; 
		_targetMesh = NULL;
		_sourceMesh = NULL;
		_cpdMap = NULL;
		_showDoubleMesh = false;
	}

	~Scene(){ 
		for(unsigned int j=0;j<_lights.size();j++)
			delete _lights[j]; 
		_lights.clear(); 
		delete _targetMesh; // have to be sure that no one shares this data
		delete _sourceMesh;
		delete _cpdMap;
		delete _mat; 
	}

	void setTargetMesh(Mesh* m) { _targetMesh=m; }

	void setSourceMesh(Mesh* m) { _sourceMesh=m; }

	void setCdpMap(vector<int>* v){
		
		_cpdMap = v;
	}

	int getCorVertexID(int sourceID){
		return (*_cpdMap)[sourceID];
	}

	bool getshowDoubleMesh(){
		return _showDoubleMesh;
	}

	void setMaterial(Material* m) { _mat = m; }		
	void addLight(Light* l) { _lights.push_back(l); }

	Mesh* getTargetMesh(){return _targetMesh;}

	Mesh* getSourceMesh(){return _sourceMesh;}

	vector<int> * getcpdMap(){return _cpdMap;}

	Material* getMaterial() { return _mat; }
	int getNumLights() { return (int)_lights.size(); }
	Light* getLight(int i) { return _lights[i]; }

	void computeColorNormalized(int mode);
	void computeColorUnNormalized(bool outputFlag,int mode,double lambda=1);
	double computeCombinedCurvatureDifference(int mode, double threshold, vector<double>& diff);
	void computeNomorlvectorBins(double lambda = 1,int binNum = 12);
	void outputNormalVector(string filename,bool Sourceflag);
	void outputCurvatureDiff(string filename,vector<double>& diff);


}; 

class RenderingUtils{
public: 
	static Material* readMaterial(std::istream& mat); 
	static Light* readLight(std::istream& str); 
	static Mesh* readMesh(std::string name, std::istream& str);
	static vector<int>* readCpdMap(std::istream& str);
	static Scene* readScene(const std::string& file); 
	
	static Pt3Array* normalizePts(Pt3Array* pts, double scale); 
	static Vec3Array* perVertexNormals(Pt3Array* pts, TriIndArray* tris); 
	static Vec3Array* perFaceNormals(Pt3Array* pts, TriIndArray* tris); 
	static void perVertexKvalue(double*& gaussK, double*& meanK, Pt3Array* pts, TriIndArray* tris,Vec3Array* Vnormls,bool loaddata);
}; 

#define SHADE_FLAT 0 
#define SHADE_GOURAUD 1
#define SHADE_NAIVE_PHONG 2
#define SHADE_SPHERICAL_PHONG 2

class Renderer{
protected: 
	Scene* _scene; 
	View* _view; 
	int _shadingModel; 

public: 
	Renderer() { 
		_scene = NULL; 
		_view = NULL; 
		_shadingModel = SHADE_FLAT; 
	}

	virtual void setScene(Scene* scene) { _scene = scene; }
	virtual void setView(View* view) { _view = view; }
	virtual void draw()=0; 
	virtual void setShadingModel(int m) { _shadingModel = m; }
	virtual  int getShadingModel() { return _shadingModel; }
}; 


#endif 