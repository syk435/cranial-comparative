#include "Rendering/RenderingPrimitives.h" 

#include <fstream> 
#include <iostream> 


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


using namespace std; 

const double MoutlierrateH=0.08;
const double MoutlierrateL=0.05;
const double GoutlierrateH=0.05;
const double GoutlierrateL=0.05;


void quickSort(double arr[], int left, int right)
{
      int i = left, j = right;
      double tmp;
      double pivot = arr[(left + right) / 2];
      /* partition */
      while (i <= j)
	  {
		  while (arr[i] < pivot)
		  {
			  i++;
		  }
          while (arr[j] > pivot)
		  {
              j--;
		  }
          if (i<= j) 
		  {
              tmp = arr[i];
              arr[i] = arr[j];
              arr[j] = tmp;
              i++;
              j--;
          }
      }
      /* recursion */
      if (left < j)
	  {
		  quickSort(arr, left, j);
	  }
      if (i < right)
      {
		  quickSort(arr, i, right);
	  }
}



/*
ColorArray* Mesh::computeColor(char* source,char* mode)
{
	ColorArray* vcolors = new ColorArray();
	
    int csize = _pts->size();
	double *colorvalues = new double[csize];
	double max;
	double min;
	double middle;
	if(strcmp(source,"Gaussian")==0)
	{
		cout<<"Use Gaussian curvature to determines the colors"<<endl;
		middle = (_gaussK->MaxValue+_gaussK->MinValue)/2;
	    max = _gaussK->MaxValue;
		min = _gaussK->MinValue;
		for(int i=0; i< csize; i++){
			colorvalues[i]= _gaussK->buffer[i];
		}    
	}
	else if(strcmp(source,"mean")==0)
	{
		cout<<"Use mean curvature to determines the colors"<<endl;
		middle = (_meanK->MaxValue+_meanK->MinValue)/2;
		max = _meanK->MaxValue;
		min = _meanK->MinValue;
		for(int i=0; i< csize; i++){
			colorvalues[i]= _meanK->buffer[i];
		}    
	}
	else if(strcmp(source,"max")==0)
	{
		cout<<"Use max principal curvature to determines the colors"<<endl;
		middle = (_maxPrincipleK->MaxValue+_maxPrincipleK->MinValue)/2;
		max = _maxPrincipleK->MaxValue;
		min = _maxPrincipleK->MinValue;
		for(int i=0; i< csize; i++){
			colorvalues[i]= _maxPrincipleK->buffer[i];
		}    
	}
	else if(strcmp(source,"min")==0)
	{
		cout<<"Use min principal curvature to determines the colors"<<endl;
		middle = (_minPrincipleK->MaxValue+_minPrincipleK->MinValue)/2;
		max = _minPrincipleK->MaxValue;
		min = _minPrincipleK->MinValue;
		for(int i=0; i< csize; i++){
			colorvalues[i]= _minPrincipleK->buffer[i];
		}    
	}
	else{
		max = 1;
		min = 0;
		cout<<"To be added!"<<endl;
	}
	if(strcmp(mode,"simple")==0){
		for(int i=0;i<csize;i++){
			Color c;
		    c[0]=(colorvalues[i]-min)/(max-min);
		    c[1]=(colorvalues[i]-min)/(max-min);
		    c[2]=(colorvalues[i]-min)/(max-min);
		    cout<<max<<" "<<min<<" "<<c[0]<<endl;
		    vcolors->add(c);
		}
	}
	else if(strcmp(mode,"sqrt")==0){
		for(int i=0;i<csize;i++){
			Color c;
		    c[0]=sqrt((colorvalues[i]-min)/(max-min));
		    c[1]=sqrt((colorvalues[i]-min)/(max-min));
		    c[2]=sqrt((colorvalues[i]-min)/(max-min));
		    cout<<max<<" "<<min<<" "<<c[0]<<endl;
		    vcolors->add(c);
		}
	}
	else if(strcmp(mode,"simplecolor")==0){
		for(int i=0;i<csize;i++){
			Color c;
			if(colorvalues[i]>middle){
				c[0]=(colorvalues[i]-middle)/(max-middle);
		        c[1]=(max-colorvalues[i])/(max-middle);
		        c[2]=0;
			}
			else{
				c[0]=0;
		        c[1]=(colorvalues[i]-min)/(middle-min);
		        c[2]=(middle-colorvalues[i])/(middle-min);
			}

		    //cout<<max<<" "<<min<<" "<<c[0]<<endl;
		    vcolors->add(c);
		}
	}
	else if(strcmp(mode,"logcolor")==0){
		//move all value larger than 1;
		for(int i=0;i<csize;i++){
			colorvalues[i]=log(colorvalues[i]-min+1);
		}
		max = log(max - min +1);
		middle = max/2;
		min = 0;

		for(int i=0;i<csize;i++){
			Color c;
			
			if(colorvalues[i]>middle){
				c[0]=(colorvalues[i]-middle)/(max-middle);
		        c[1]=(max-colorvalues[i])/(max-middle);
		        c[2]=0;
			}
			else{
				c[0]=0;
		        c[1]=(colorvalues[i]-min)/(middle-min);
		        c[2]=(middle-colorvalues[i])/(middle-min);
			}

		    //cout<<max<<" "<<min<<" "<<c[0]<<endl;
		    vcolors->add(c);
		}
	}
	else if(strcmp(mode,"denosiecolor")==0){
		//move all value larger than 1;
		max = 200;
		min = -200;
		for(int i=0;i<csize;i++){
			if(colorvalues[i]>max){
				colorvalues[i]=max;
			}
			if(colorvalues[i]<min){
				colorvalues[i]=min;
			}
		}
		middle = 0;

		for(int i=0;i<csize;i++){
			Color c;
			
			if(colorvalues[i]>middle){
				c[0]=(colorvalues[i]-middle)/(max-middle);
		        c[1]=(max-colorvalues[i])/(max-middle);
		        c[2]=0;
			}
			else{
				c[0]=0;
		        c[1]=(colorvalues[i]-min)/(middle-min);
		        c[2]=(middle-colorvalues[i])/(middle-min);
			}

		    //cout<<max<<" "<<min<<" "<<c[0]<<endl;
		    vcolors->add(c);
		}
	}

	else{
		cout<<"Mode To be added!"<<endl;
	}
	cout<<"Compute color is done!"<<endl;
	return vcolors;
}
*/



Material* RenderingUtils::readMaterial(std::istream& str){
	Color amb,diff,spec; 
	double spece; 
	for(int j=0;j<3;j++) str>>amb[j];
	for(int j=0;j<3;j++) str>>diff[j];
	for(int j=0;j<3;j++) str>>spec[j];
	str>>spece; 

	Material* ret = new Material(); 
	ret->setAmbient(amb); 
	ret->setDiffuse(diff); 
	ret->setSpecular(spec); 
	ret->setSpecExponent(spece); 
	return ret; 
}

Light* RenderingUtils::readLight(std::istream& str){
	Pt3 pos(0,0,0); 
	Color c; 

	str>>pos[0]>>pos[1]>>pos[2]; 
	str>>c[0]>>c[1]>>c[2]; 
	Light* ret = new Light(); 
	ret->setColor(c); 
	ret->setPos(pos); 
	return ret; 
}


double Pt3toPt3distance(Pt3 a, Pt3 b){
	double s;
	s=(a[0]-b[0])*(a[0]-b[0])+(a[1]-b[1])*(a[1]-b[1])+(a[2]-b[2])*(a[2]-b[2]);
	return sqrt(s);
}



void Mesh::loadEvenDistributedVertices(string filename){
	/*
	int psize = _pts->size();
	ifstream inputEvenDistributedVertices;
	inputEvenDistributedVertices.open(filename+".txt");
	if(inputEvenDistributedVertices.good()){
		int size;
		inputEvenDistributedVertices>>size;
		assert(size<=psize);
		for(int i = 0; i < ){
		
		}
	}
	else{
		cout<<"Can not open ("+filename+".txt)"<<endl;
	}
	*/
}



void Mesh::loadGaussK(string filename){
	int psize = _pts->size();
	ifstream inputCurvature;
	inputCurvature.open(filename+".txt");
	if(inputCurvature.good()){
		int size;
		inputCurvature>>size;
		assert(size==psize);
		_gaussK= new Curvature(size);
		double max = 0;
		double min = 1;
		for(int i=0;i<size;i++){
			double temp;
			inputCurvature>>temp;
			if(temp>max){
				max = temp;
			}
			if(temp<min){
				min = temp;
			}
			_gaussK->buffer[i] = temp;
		}
		_gaussK->MaxValue = max;
		_gaussK->MinValue = min;
		cout<<"Read Gaussian Curvature value is done!("<<size<<")"<<"Range:["<<min<<","<<max<<"]."<<endl;
	}
	else{
		cout<<"Can not open ("+filename+".txt)"<<endl;
	}
}

void Mesh::loadMeanK(string filename){
	int psize = _pts->size();
	ifstream inputCurvature;
	inputCurvature.open(filename+".txt");
	if(inputCurvature.good()){
		int size;
		inputCurvature>>size;
		assert(size==psize);
		_meanK = new Curvature(size);
		double max = 0;
		double min = 1;
		for(int i=0;i<size;i++){
			double temp;
			inputCurvature>>temp;
			if(temp>max){
				max = temp;
			}
			if(temp<min){
				min = temp;
			}
			_meanK->buffer[i] = temp;
		}
		_meanK->MaxValue = max;
		_meanK->MinValue = min;
		cout<<"Read mean Curvature value is done!("<<size<<")"<<"Range:["<<min<<","<<max<<"]."<<endl;
	}
	else{
		cout<<"Can not open ("+filename+".txt)"<<endl;
	}
}

void Mesh::loadMaxPrincipleK(string filename){
	int psize = _pts->size();
	ifstream inputCurvature;
	inputCurvature.open(filename+".txt");
	if(inputCurvature.good()){
		int size;
		inputCurvature>>size;
		assert(size==psize);
		_maxPrincipleK= new Curvature(size);
		double max = 0;
		double min = 1;
		for(int i=0;i<size;i++){
			double temp;
			inputCurvature>>temp;
			if(temp>max){
				max = temp;
			}
			if(temp<min){
				min = temp;
			}
			_maxPrincipleK->buffer[i] = temp;
		}
		_maxPrincipleK->MaxValue = max;
		_maxPrincipleK->MinValue = min;
		cout<<"Read Max Curvature value is done!("<<size<<")"<<"Range:["<<min<<","<<max<<"]."<<endl;
	}
	else{
		cout<<"Can not open ("+filename+".txt)"<<endl;
	}
}

void Mesh::loadMinPrincipleK(string filename){
	int psize = _pts->size();
	ifstream inputCurvature;
	inputCurvature.open(filename+".txt");
	if(inputCurvature.good()){
		int size;
		inputCurvature>>size;
		assert(size==psize);
		_minPrincipleK= new Curvature(size);
		double max = 0;
		double min = 1;
		for(int i=0;i<size;i++){
			double temp;
			inputCurvature>>temp;
			if(temp>max){
				max = temp;
			}
			if(temp<min){
				min = temp;
			}
			_minPrincipleK->buffer[i] = temp;
		}
		_minPrincipleK->MaxValue = max;
		_minPrincipleK->MinValue = min;
		cout<<"Read Min Curvature value is done!("<<size<<")"<<"Range:["<<min<<","<<max<<"]."<<endl;
	}
	else{
		cout<<"Can not open ("+filename+".txt)"<<endl;
	}
}


// read a triangle mesh
Mesh* RenderingUtils::readMesh(std::string name,std::istream& str){
	int nverts, ntris; 
	str>>nverts; 
	Pt3Array* pts = new Pt3Array(); 
	TriIndArray* inds = new TriIndArray(); 

	for(int j=0;j<nverts;j++){
		Pt3 p(0,0,0); 
		str>>p[0]>>p[1]>>p[2]; 
		pts->add(p);
		//p.print();cout<<endl;
	}
	cout<<"Reading vertexs is done!"<<"("<<nverts<<")"<<endl;

	str>>ntris; 

	for(int j=0;j<ntris;j++){
		TriInd t; 
		
		str>>t[0]>>t[1]>>t[2]; 
		assert(t[0] < nverts && t[1] < nverts && t[2] < nverts);
		inds->add(t); 
	}
	cout<<"Reading faces is done!"<<"("<<ntris<<")"<<endl;

	// compute the normals
	Vec3Array* vnorms = RenderingUtils::perVertexNormals(pts,inds); 
	Vec3Array* fnorms = RenderingUtils::perFaceNormals(pts,inds); 

	Mesh* ret = new Mesh(pts,inds); 
	ret->setFNormals(fnorms); 
	ret->setVNormals(vnorms); 


	ret->loadGaussK(name+"_Gaussian");
	ret->loadMeanK(name+"_mean");
	ret->loadMaxPrincipleK(name+"_max");
	ret->loadMinPrincipleK(name+"_min");

	ColorArray * color = new ColorArray;
	Color c(0,0,1,0);
	for(int i =0;i<nverts;i++){
		color->add(c);
	}
	ret->setColor(color);
	return ret; 
}

vector<int>* RenderingUtils::readCpdMap(std::istream& is){
	cout<<"Call readcpdMap"<<endl;
	if(is.good()){
		vector<int>* ret = new vector<int>();
		while(!is.eof()){
			int temp;
			is>>temp;
			//cout<<temp<<endl;
			ret->push_back(temp-1);
		}
		ret->pop_back();
		cout<<"Reading correspondence Map is done!"<<endl;
		return ret;
	}
	else{
		cout<<"Can not open correspondence Map"<<endl;
		return NULL;
	}
}




Scene* RenderingUtils::readScene(const string& file){
	string name = file + ".txt";
	ifstream is(name); 
	if(is.good()){
		Scene* ret = new Scene(); 
		ret->setMaterial(readMaterial(is)); 
		Color amb; 
		is>>amb[0]>>amb[1]>>amb[2]; 
		int nlights; 
		is>>nlights; 
		for(int j=0;j<nlights;j++){
			Light* l = readLight(is); 
			l->setAmbient(amb); 
			ret->addLight(l); 
		}
		ret->setSourceMesh(readMesh(file+"_source",is));
		ret->setTargetMesh(readMesh(file+"_target",is));
		ret->setCdpMap(readCpdMap(is));
		
		//cout<<ret->getSourceMesh()->getPoints()->size()<<endl;
		//cout<< ret->getcpdMap()->size()<<endl;
		ret->computeColorUnNormalized(COLOR_Default,false);

		if(ret->getSourceMesh()->getPoints()->size() == ret->getcpdMap()->size()){
			return ret;
		}
		else{
			cout<<"Wrong Number!"<<ret->getcpdMap()->size()<<endl;
			return NULL;
		}
	}
	return NULL; 
}



Vec3Array* RenderingUtils::perVertexNormals(Pt3Array* pts, TriIndArray* tris){
	int ntris = tris->size(); 
	int nverts = pts->size(); 

	Vec3Array* norms = new Vec3Array(); 
	norms->resize(nverts); 

	for(int j=0;j<ntris;j++){
		TriInd& tri = *tris->get(j); 
		assert((tri[0] <= nverts));
		Pt3& a = *pts->get(tri[0]); 
		assert((tri[1] <= nverts));
		Pt3& b = *pts->get(tri[1]);
		assert((tri[2] <= nverts));
		Pt3& c = *pts->get(tri[2]); 

		Vec3 ba = b-a; 
		Vec3 ca = c-a; 
		Vec3 n = cross(ba,ca); 

		for(int k=0;k<3;k++){
			Vec3& vn = *norms->get(tri[k]); 
			vn+=n; 
		}
	}

	for(int j=0;j<nverts;j++)
		norms->get(j)->normalize(); 

	return norms; 
}


Vec3Array* RenderingUtils::perFaceNormals(Pt3Array* pts, TriIndArray* tris){
	int ntris = tris->size(); 
	int nverts = pts->size(); 

	Vec3Array* norms = new Vec3Array(); 

	for(int j=0;j<ntris;j++){
		TriInd& tri = *tris->get(j); 
		Pt3& a = *pts->get(tri[0]); 
		Pt3& b = *pts->get(tri[1]); 
		Pt3& c = *pts->get(tri[2]); 

		Vec3 ba = b-a; 
		Vec3 ca = c-a; 
		Vec3 n = cross(ba,ca); 
		n.normalize(); 
		norms->add(n); 
	}

	return norms; 
}



void Scene::computeColorNormalized(int mode){
	int sourceSize =  _sourceMesh->getPoints()->size();
	int targetSize = _targetMesh->getPoints()->size();
	if(mode == COLOR_Default){
		double rs = (double)(rand())/(double)(RAND_MAX);
		double rt = (double)(rand())/(double)(RAND_MAX);
		//int sourceSize =  _sourceMesh->getPoints()->size();
		for(int i=0;i<sourceSize;i++){
			Color c(0,rs,1-rs,0);
			_sourceMesh->setColorPos(i,c);
		}
		//int targetSize = _targetMesh->getPoints()->size();
		for(int i=0;i<targetSize;i++){
			Color c(rt,1-rt,0);
			_targetMesh->setColorPos(i,c);
		}
		for(int i=0;i<targetSize;i++){
			Pt3 pre = *(_targetMesh->getPoints()->get(i));
			//pre.print();cout<<endl;
			pre[0] = pre[0]-moveLength;
			//pre.print();cout<<endl;
			_targetMesh->getPoints()->setValuePos(i,pre);
		}


	}
	else if(mode<10){
		vector<double> colorTemp;
		float  max = 0;
		float  min = 1;
		if(mode == COLOR_Curvature_Max){
			Curvature* sourceMaxCurvature = _sourceMesh->getMaxCurvature();
			Curvature* targetMaxCurvature = _targetMesh->getMaxCurvature();
			for(int i=0;i<sourceSize;i++){
				double tempSource = sourceMaxCurvature->buffer[i];
				double tempTarget = targetMaxCurvature->buffer[(*_cpdMap)[i]];
				double temp = sqrt(abs((tempSource-tempTarget)/(tempSource+tempTarget)));
				if(temp > max){
					max = temp;
				}
				if(temp < min){
					min = temp;
				}
				colorTemp.push_back(temp);
			}
		}
		else if(mode == COLOR_Curvature_Min){
			Curvature* sourceMinCurvature = _sourceMesh->getMinCurvature();
			Curvature* targetMinCurvature = _targetMesh->getMinCurvature();
			for(int i=0;i<sourceSize;i++){
				double tempSource = sourceMinCurvature->buffer[i];
				double tempTarget = targetMinCurvature->buffer[(*_cpdMap)[i]];
				double temp = sqrt(abs((tempSource-tempTarget)/(tempSource+tempTarget)));
				if(temp > max){
					max = temp;
				}
				if(temp < min){
					min = temp;
				}
				colorTemp.push_back(temp);
			}
		}
		else if(mode == COLOR_Curvature_Mean){
			Curvature* sourceMeanCurvature = _sourceMesh->getMeanCurvature();
			Curvature* targetMeanCurvature = _targetMesh->getMeanCurvature();
			for(int i=0;i<sourceSize;i++){
				double tempSource = sourceMeanCurvature->buffer[i];
				double tempTarget = targetMeanCurvature->buffer[(*_cpdMap)[i]];
				double temp = sqrt(abs((tempSource-tempTarget)/(tempSource+tempTarget)));
				if(temp > max){
					max = temp;
				}
				if(temp < min){
					min = temp;
				}
				colorTemp.push_back(temp);
			}
		}
		else if(mode == COLOR_Curvature_Gaussian){
			Curvature* sourceGaussianCurvature = _sourceMesh->getGaussianCurvature();
			Curvature* targetGaussianCurvature = _targetMesh->getGaussianCurvature();
			for(int i=0;i<sourceSize;i++){
				double tempSource = sourceGaussianCurvature->buffer[i];
				double tempTarget = targetGaussianCurvature->buffer[(*_cpdMap)[i]];
				double temp = sqrt(abs((tempSource-tempTarget)/(tempSource+tempTarget)));
				if(temp > max){
					max = temp;
				}
				if(temp < min){
					min = temp;
				}
				colorTemp.push_back(temp);
			}
		}
		else{
			cout<<"Worng Condition!"<<endl;
			system("pause");
		}
		std::cout<<"Curvature range ("<<min<<", "<<max<<")"<<std::endl;
		float range = max - min;
		for(int i = 0; i<sourceSize;i++){
			colorTemp[i] = (colorTemp[i] - min)/range;
		}
		for(int i = 0 ; i<sourceSize;i++){
			if(colorTemp[i]>0.5){
				double r = (colorTemp[i] - 0.5)/0.5;
				double g = (1 - colorTemp[i])/0.5;
				double b = 0;
				Color c(r,g,b,0);
				_sourceMesh->setColorPos(i,c);
			}
			else{
				double r = 0;
				double g = colorTemp[i]/0.5;
				double b = (0.5 - colorTemp[i])/0.5;
				Color c(r,g,b,0);
				_sourceMesh->setColorPos(i,c);
			}
		}
	}
	else{

	}
}


void Scene::computeColorUnNormalized(bool outputFlag,int mode,double lambda){
	int sourceSize =  _sourceMesh->getPoints()->size();
	int targetSize = _targetMesh->getPoints()->size();
	if(mode == COLOR_Default){
		double rs = (double)(rand())/(double)(RAND_MAX);
		double rt = (double)(rand())/(double)(RAND_MAX);
		//int sourceSize =  _sourceMesh->getPoints()->size();
		for(int i=0;i<sourceSize;i++){
			Color c(0,rs,1-rs,0.5);
			_sourceMesh->setColorPos(i,c);
		}
		//int targetSize = _targetMesh->getPoints()->size();
		for(int i=0;i<targetSize;i++){
			Color c(rt,1-rt,0,0.5);
			_targetMesh->setColorPos(i,c);
		}
		if(_showDoubleMesh==false){
			for(int i=0;i<targetSize;i++){
				Pt3 pre = *(_targetMesh->getPoints()->get(i));
				//pre.print();cout<<endl;
				pre[0] = pre[0]-moveLength;
				//pre.print();cout<<endl;
				_targetMesh->getPoints()->setValuePos(i,pre);
			}
			for(int i=0;i<sourceSize;i++){
				Pt3 pre = *(_sourceMesh->getPoints()->get(i));
				//pre.print();cout<<endl;
				pre[0] = pre[0]+moveLength;
				//pre.print();cout<<endl;
				_sourceMesh->getPoints()->setValuePos(i,pre);
			}
			_showDoubleMesh = true;
		}
	}
	else if(mode < 5){
		if(_showDoubleMesh==true){
			for(int i=0;i<targetSize;i++){
				Pt3 pre = *(_targetMesh->getPoints()->get(i));
				//pre.print();cout<<endl;
				pre[0] = pre[0]+moveLength;
				//pre.print();cout<<endl;
				_targetMesh->getPoints()->setValuePos(i,pre);
			}
			for(int i=0;i<sourceSize;i++){
				Pt3 pre = *(_sourceMesh->getPoints()->get(i));
				//pre.print();cout<<endl;
				pre[0] = pre[0]-moveLength;
				//pre.print();cout<<endl;
				_sourceMesh->getPoints()->setValuePos(i,pre);
			}
			_showDoubleMesh = false;
		}
		vector<double> colorTemp;
		if(mode == COLOR_Curvature_Max){
			Curvature* sourceMaxCurvature = _sourceMesh->getMaxCurvature();
			Curvature* targetMaxCurvature = _targetMesh->getMaxCurvature();
			for(int i=0;i<sourceSize;i++){
				double tempSource = sourceMaxCurvature->buffer[i];
				double tempTarget = targetMaxCurvature->buffer[(*_cpdMap)[i]];
				//double temp = sqrt(abs((tempSource-tempTarget)/(tempSource+tempTarget)));
				//double temp = abs(tempSource-tempTarget);
				double temp;
				if(abs(tempSource+tempTarget)<0.00000000001){
					temp = 0;
				}
				temp = abs((tempSource-tempTarget)/(tempSource+tempTarget));
				colorTemp.push_back(temp);
			}
		}
		else if(mode == COLOR_Curvature_Min){
			Curvature* sourceMinCurvature = _sourceMesh->getMinCurvature();
			Curvature* targetMinCurvature = _targetMesh->getMinCurvature();
			for(int i=0;i<sourceSize;i++){
				double tempSource = sourceMinCurvature->buffer[i];
				double tempTarget = targetMinCurvature->buffer[(*_cpdMap)[i]];
				//double temp = sqrt(abs((tempSource-tempTarget)/(tempSource+tempTarget)));
				//double temp = abs(tempSource-tempTarget);
				double temp;
				if(abs(tempSource+tempTarget)<0.00000000001){
					temp = 0;
				}
				temp = abs((tempSource-tempTarget)/(tempSource+tempTarget));
				colorTemp.push_back(temp);
			}
		}
		else if(mode == COLOR_Curvature_Mean){
			Curvature* sourceMeanCurvature = _sourceMesh->getMeanCurvature();
			Curvature* targetMeanCurvature = _targetMesh->getMeanCurvature();
			for(int i=0;i<sourceSize;i++){
				double tempSource = sourceMeanCurvature->buffer[i];
				double tempTarget = targetMeanCurvature->buffer[(*_cpdMap)[i]];
				//double temp = sqrt(abs((tempSource-tempTarget)/(tempSource+tempTarget)));
				//double temp = abs(tempSource-tempTarget);
				double temp;
				if(abs(tempSource+tempTarget)<0.00000000001){
					temp = 0;
				}
				temp = abs((tempSource-tempTarget)/(tempSource+tempTarget));
				colorTemp.push_back(temp);
			}
		}
		else if(mode == COLOR_Curvature_Gaussian){
			Curvature* sourceGaussianCurvature = _sourceMesh->getGaussianCurvature();
			Curvature* targetGaussianCurvature = _targetMesh->getGaussianCurvature();
			for(int i=0;i<sourceSize;i++){
				double tempSource = sourceGaussianCurvature->buffer[i];
				double tempTarget = targetGaussianCurvature->buffer[(*_cpdMap)[i]];
				//double temp = sqrt(abs((tempSource-tempTarget)/(tempSource+tempTarget)));
				//double temp = abs(tempSource-tempTarget);
				double temp;
				if(abs(tempSource+tempTarget)<0.00000000001){
					temp = 0;
				}
				temp = abs((tempSource-tempTarget)/(tempSource+tempTarget));
				colorTemp.push_back(temp);
			}
		}
		else{
			cout<<"Worng Condition!"<<endl;
			system("pause");
		}
		for(int i = 0; i<sourceSize;i++){
			//colorTemp[i] = 1 / (1+ exp(-lambda*colorTemp[i]));
			//colorTemp[i] = (1 - exp(-lambda*colorTemp[i]))/(1 + exp(-lambda*colorTemp[i]));
			colorTemp[i] = 1 - exp(-lambda*colorTemp[i]);
		}
		for(int i = 0 ; i<sourceSize;i++){
			if(colorTemp[i]>0.5){
				double r = (colorTemp[i] - 0.5)/0.5;
				double g = (1 - colorTemp[i])/0.5;
				double b = 0;
				Color c(r,g,b,0);
				_sourceMesh->setColorPos(i,c);
			}
			else{
				double r = 0;
				double g = (colorTemp[i]-0)/0.5;
				double b = (0.5-colorTemp[i])/0.5;
				Color c(r,g,b,0);
				_sourceMesh->setColorPos(i,c);
			}
		}
		ofstream sdiff;
		if(mode==COLOR_Curvature_Max){
			sdiff.open("Relative_Changed_Surface_Area_Max.txt");
		}
		if(mode==COLOR_Curvature_Min){
			sdiff.open("Relative_Changed_Surface_Area_Min.txt");
		}
		if(mode==COLOR_Curvature_Mean){
			sdiff.open("Relative_Changed_Surface_Area_Mean.txt");
		}
		if(mode==COLOR_Curvature_Gaussian){
			sdiff.open("Relative_Changed_Surface_Area_Gaussian.txt");
		}
		cout<<"Combined Curvature Difference:"<<computeCombinedCurvatureDifference(mode,0.25,colorTemp)<<","<<computeCombinedCurvatureDifference(mode,0.5,colorTemp)<<","<<computeCombinedCurvatureDifference(mode,0.75,colorTemp)<<endl;

		sdiff<<"Threshold: .25  .5  .75"<<endl;
		sdiff<<computeCombinedCurvatureDifference(mode,0.25,colorTemp)<<","<<computeCombinedCurvatureDifference(mode,0.5,colorTemp)<<","<<computeCombinedCurvatureDifference(mode,0.75,colorTemp)<<endl;
		sdiff.close();
		if(outputFlag){
			outputCurvatureDiff("Output/test_CurvatureDiff.txt",colorTemp);
		}
	}
	else if(mode < 9){
		if(_showDoubleMesh==false){
			for(int i=0;i<targetSize;i++){
				Pt3 pre = *(_targetMesh->getPoints()->get(i));
				//pre.print();cout<<endl;
				pre[0] = pre[0]-moveLength;
				//pre.print();cout<<endl;
				_targetMesh->getPoints()->setValuePos(i,pre);
			}
			for(int i=0;i<sourceSize;i++){
				Pt3 pre = *(_sourceMesh->getPoints()->get(i));
				//pre.print();cout<<endl;
				pre[0] = pre[0]+moveLength;
				//pre.print();cout<<endl;
				_sourceMesh->getPoints()->setValuePos(i,pre);
			}
			_showDoubleMesh = true;
		}
		vector<double> colorTempSource;
		vector<double> colorTempTarget;
		if(mode == COLOR_Curvature_Max_Double){
			Curvature* sourceMaxCurvature = _sourceMesh->getMaxCurvature();
			Curvature* targetMaxCurvature = _targetMesh->getMaxCurvature();
			for(int i=0;i<sourceSize;i++){
				double tempSource = sourceMaxCurvature->buffer[i];
				colorTempSource.push_back(tempSource);
			}
			for(int i=0;i<targetSize;i++){
				double tempTarget = targetMaxCurvature->buffer[i];
				colorTempTarget.push_back(tempTarget);
			}
		}
		else if(mode == COLOR_Curvature_Min_Double){
			Curvature* sourceMinCurvature = _sourceMesh->getMinCurvature();
			Curvature* targetMinCurvature = _targetMesh->getMinCurvature();
			for(int i=0;i<sourceSize;i++){
				double tempSource = sourceMinCurvature->buffer[i];
				colorTempSource.push_back(tempSource);
			}
			for(int i=0;i<targetSize;i++){
				double tempTarget = targetMinCurvature->buffer[i];
				colorTempTarget.push_back(tempTarget);
			}
		}
		else if(mode == COLOR_Curvature_Mean_Double){
			Curvature* sourceMeanCurvature = _sourceMesh->getMeanCurvature();
			Curvature* targetMeanCurvature = _targetMesh->getMeanCurvature();
			for(int i=0;i<sourceSize;i++){
				double tempSource = sourceMeanCurvature->buffer[i];
				colorTempSource.push_back(tempSource);
			}
			for(int i=0;i<targetSize;i++){
				double tempTarget = targetMeanCurvature->buffer[i];
				colorTempTarget.push_back(tempTarget);
			}
		}
		else if(mode == COLOR_Curvature_Gaussian_Double){
			Curvature* sourceGaussianCurvature = _sourceMesh->getGaussianCurvature();
			Curvature* targetGaussianCurvature = _targetMesh->getGaussianCurvature();
			for(int i=0;i<sourceSize;i++){
				double tempSource = sourceGaussianCurvature->buffer[i];
				colorTempSource.push_back(tempSource);
			}
			for(int i=0;i<targetSize;i++){
				double tempTarget = targetGaussianCurvature->buffer[i];
				colorTempTarget.push_back(tempTarget);
			}
		}
		else{
			cout<<"Worng Condition!"<<endl;
			system("pause");
		}
		for(int i = 0; i<sourceSize;i++){
			colorTempSource[i] = 1- exp(-10*lambda*colorTempSource[i]);
		}
		//Color source
		for(int i = 0 ; i<sourceSize;i++){
			if(colorTempSource[i]>0.5){
				double r = (colorTempSource[i] - 0.5)/0.5;
				double g = (1 - colorTempSource[i])/0.5;
				double b = 0;
				Color c(r,g,b,0);
				_sourceMesh->setColorPos(i,c);
			}
			else{
				double r = 0;
				double g = (colorTempSource[i]-0)/0.5;
				double b = (0.5-colorTempSource[i])/0.5;
				Color c(r,g,b,0);
				_sourceMesh->setColorPos(i,c);
			}
		}
		//Color Target
		for(int i = 0; i<targetSize;i++){
			colorTempTarget[i] = 1- exp(-10*lambda*colorTempTarget[i]);
		}
		for(int i = 0 ; i<targetSize;i++){
			if(colorTempTarget[i]>0.5){
				double r = (colorTempTarget[i] - 0.5)/0.5;
				double g = (1 - colorTempTarget[i])/0.5;
				double b = 0;
				Color c(r,g,b,0);
				_targetMesh->setColorPos(i,c);
			}
			else{
				double r = 0;
				double g = (colorTempTarget[i]-0)/0.5;
				double b = (0.5-colorTempTarget[i])/0.5;
				Color c(r,g,b,0);
				_targetMesh->setColorPos(i,c);
			}
		}
	}
	else if(mode == COLOR_NormalVector){
		if(_showDoubleMesh==true){
			for(int i=0;i<targetSize;i++){
				Pt3 pre = *(_targetMesh->getPoints()->get(i));
				//pre.print();cout<<endl;
				pre[0] = pre[0]+moveLength;
				//pre.print();cout<<endl;
				_targetMesh->getPoints()->setValuePos(i,pre);
			}
			for(int i=0;i<sourceSize;i++){
				Pt3 pre = *(_sourceMesh->getPoints()->get(i));
				//pre.print();cout<<endl;
				pre[0] = pre[0]-moveLength;
				//pre.print();cout<<endl;
				_sourceMesh->getPoints()->setValuePos(i,pre);
			}
			_showDoubleMesh = false;
		}
		Vec3Array * sourceNv = _sourceMesh->getVNormals();
		Vec3Array * targetNv = _targetMesh->getVNormals();
		Curvature* sourceMeanCurvature = _sourceMesh->getMeanCurvature();
		Curvature* targetMeanCurvature = _targetMesh->getMeanCurvature();

		vector<double> colorTemp;
		for(int i=0;i<sourceSize;i++){
			Pt3 sNormal = *(sourceNv->get(i));
			Pt3 tNormal = *(targetNv->get((*_cpdMap)[i]));
			double innerProduct = sNormal[0]*tNormal[0]+sNormal[1]*tNormal[1]+sNormal[2]*tNormal[2];
			double sNLength = sNormal[0]*sNormal[0]+sNormal[1]*sNormal[1]+sNormal[2]*sNormal[2];
			double tNLength = tNormal[0]*tNormal[0]+tNormal[1]*tNormal[1]+tNormal[2]*tNormal[2];
			double theta = acos(innerProduct/(sNLength*tNLength));
			//divided by the curvature
			
			double sCurvature = sourceMeanCurvature->buffer[i];
			double tCurvature = targetMeanCurvature->buffer[(*_cpdMap)[i]];
			theta = theta/PI * (exp(-lambda*abs(sCurvature+tCurvature)));
			
			colorTemp.push_back(theta);
		}
		/*
		for(int i = 0; i<sourceSize;i++){
			colorTemp[i] = 1 - exp(-lambda*colorTemp[i]);
		}
		
		for(int i = 0; i<sourceSize;i++){
			colorTemp[i] = colorTemp[i]/PI;
		}*/
		for(int i = 0 ; i<sourceSize;i++){
			if(colorTemp[i]>0.5){
				double r = (colorTemp[i] - 0.5)/0.5;
				double g = (1 - colorTemp[i])/0.5;
				double b = 0;
				Color c(r,g,b,0);
				_sourceMesh->setColorPos(i,c);
			}
			else{
				double r = 0;
				double g = (colorTemp[i]-0)/0.5;
				double b = (0.5-colorTemp[i])/0.5;
				Color c(r,g,b,0);
				_sourceMesh->setColorPos(i,c);
			}
		}
	}
}


double Scene:: computeCombinedCurvatureDifference(int mode,double threshold, vector<double>& diff){
	Pt3Array* pts = this->getSourceMesh()->getPoints();
	TriIndArray* tris = this->getSourceMesh()->getInds();
	int ntris = tris->size(); 
	int nverts = pts->size();
	assert(diff.size()==nverts);
	vector<set<int> > revisedMap;
	vector<double> areas;
	// Compute the revisedMap and areas
	set<int> emptyTemp;
	for(int i = 0; i < nverts; i++){
		revisedMap.push_back(emptyTemp);
	}
	double totalArea = 0;
	for(int i = 0; i < ntris; i++){
		// Compute revisedMap
		int index1 = (*tris->get(i))[0];
		if(revisedMap[index1].find(i)==revisedMap[index1].end()){
			revisedMap[index1].insert(i);
		}
		int index2 = (*tris->get(i))[1];
		if(revisedMap[index2].find(i)==revisedMap[index2].end()){
			revisedMap[index2].insert(i);
		}
		int index3 = (*tris->get(i))[2];
		if(revisedMap[index3].find(i)==revisedMap[index3].end()){
			revisedMap[index3].insert(i);
		}
		Pt3& a = *(pts->get(index1));
		Pt3& b = *(pts->get(index2));
		Pt3& c = *(pts->get(index3));
		double edgeA = sqrt((b[0]-c[0])*(b[0]-c[0])+(b[1]-c[1])*(b[1]-c[1])+(b[2]-c[2])*(b[2]-c[2]));
		double edgeB = sqrt((a[0]-c[0])*(a[0]-c[0])+(a[1]-c[1])*(a[1]-c[1])+(a[2]-c[2])*(a[2]-c[2]));
		double edgeC = sqrt((b[0]-a[0])*(b[0]-a[0])+(b[1]-a[1])*(b[1]-a[1])+(b[2]-a[2])*(b[2]-a[2]));
		double p = (edgeA + edgeB + edgeC) / 2;
		double triArea = sqrt(p*(p-edgeA)*(p-edgeB)*(p-edgeC));
		areas.push_back(triArea);
		totalArea += triArea;
	}
	//Compute the area with the threshold
	double sum = 0;
	set<int>::iterator iter;
	for(int i=0;i<nverts;i++){
		int temp = 0;
		if(diff[i]>threshold){
			double nearByArea = 0;
			for(iter = revisedMap[i].begin();iter!=revisedMap[i].end();iter++){
				nearByArea += areas[*iter]/3;
			}
			sum += nearByArea*diff[i];
		}
	}
	return sum/totalArea;
}


void Scene:: outputNormalVector(string filename,bool sourceFlag){
	ofstream os(filename);
	if(os.good()){
		if(sourceFlag){
			int size = _sourceMesh->getVNormals()->size();
			os<<size<<endl;
			for(int i=0;i<size;i++){
				Pt3& nv = *(_sourceMesh->getVNormals()->get(i));
				os<<nv[0]<<" "<<nv[1]<<" "<<nv[2]<<endl;
			}
		}
		else{
			int size = _targetMesh->getVNormals()->size();
			os<<size<<endl;
			for(int i=0;i<size;i++){
				Pt3 &nv = *(_targetMesh->getVNormals()->get(i));
				os<<nv[0]<<" "<<nv[1]<<" "<<nv[2]<<endl;
			}
		}
	} 
}


void Scene:: outputCurvatureDiff(string filename,vector<double>& diff){
	ofstream os(filename);
	if(os.good()){
		os<<diff.size()<<endl;
		for(int i=0;i<diff.size();i++){
			os<<diff[i]<<endl;
		}
	}
}

void Scene::computeNomorlvectorBins(double lambda,int binNum){
	//Compute the source mesh
	int sSize = _sourceMesh->getPoints()->size(); 
	Curvature* sourceMean = _sourceMesh->getMeanCurvature();
	Vec3Array* sourceVector = _sourceMesh->getVNormals();
	vector<vector<double> > sourceBin(binNum, vector<double>(binNum));
	for(int i = 0; i<binNum; i++){
		for(int j = 0; j<binNum; j++ ){
			sourceBin[i][j] = 0;
		}
	}
	for(int i = 0; i < sSize; i++){
		Vec3& nV = *(sourceVector->get(i));
		double theta = acos(nV[0]/sqrt(nV[0]*nV[0]+nV[2]*nV[2]));
		if(nV[2]<0){
			theta = -theta;
		}
		double varphi = atan(nV[1]/sqrt(nV[0]*nV[0]+nV[2]*nV[2]));
		int thetaIndex = (int)((theta + PI)/ (2*PI/binNum));
		int varphiIndex =  (int)((varphi + PI/2)/ (PI/binNum));
		
		if(thetaIndex >=0 && thetaIndex< binNum && varphiIndex >=0 && varphiIndex <binNum ){
			double nMean = 1 - exp(-1*lambda*(sourceMean->buffer[i]));
			nMean = nMean>1?1:nMean;
			nMean = nMean<0?0:nMean;
			sourceBin[varphiIndex][thetaIndex] += nMean;
		}
		else{
			cout<<thetaIndex<<" "<<varphiIndex<<endl;
			nV.print();
		}
	}
	ofstream sOutput;
	sOutput.open("sourceBins.txt");
	if(sOutput.good()){
		sOutput<<binNum<<endl;
		for(int i=0; i<binNum;i++){
			for(int j=0; j<binNum;j++){
				sOutput<<sourceBin[i][j]<<endl;
			}
		}
	}

	//Compute the target mesh
	int tSize = _targetMesh->getPoints()->size(); 
	Curvature* targetMean = _targetMesh->getMeanCurvature();
	Vec3Array* targetVector = _targetMesh->getVNormals();
	vector<vector<double> > targetBin(binNum, vector<double>(binNum));
	for(int i = 0; i<binNum; i++){
		for(int j = 0; j<binNum; j++ ){
			targetBin[i][j] = 0;
		}
	}
	for(int i = 0; i < tSize; i++){
		Vec3& nV = *(targetVector->get(i));
		//double theta = atan(nV[2]/nV[0]);
		//double theta = asin(nV[2]/sqrt(nV[0]*nV[0]+nV[2]*nV[2]));
		double theta = acos(nV[0]/sqrt(nV[0]*nV[0]+nV[2]*nV[2]));
		if(nV[2]<0){
			theta = -theta;
		}
		double varphi = atan(nV[1]/sqrt(nV[0]*nV[0]+nV[2]*nV[2]));
		int thetaIndex = (int)((theta + PI)/ (2*PI/binNum));
		int varphiIndex =  (int)((varphi + PI/2)/ (PI/binNum));
		if(thetaIndex >=0 && thetaIndex< binNum && varphiIndex >=0 && varphiIndex <binNum ){
			double nMean = 1 - exp(-1*lambda*(sourceMean->buffer[i]));
			nMean = nMean>1?1:nMean;
			nMean = nMean<0?0:nMean;
			targetBin[varphiIndex][thetaIndex] += nMean;
		}
		else{
			cout<<thetaIndex<<" "<<varphiIndex<<endl;
			nV.print();
		}
	}
	ofstream tOutput;
	tOutput.open("targetBins.txt");
	if(tOutput.good()){
		tOutput<<binNum<<endl;
		for(int i=0; i<binNum;i++){
			for(int j=0; j<binNum;j++){
				tOutput<<targetBin[i][j]<<endl;
			}
		}
	}

	double sLPFS = 0;
	double sRPFS = 0;
	double sLAFS = 0;
	double sRAFS = 0;
	for(int i = 5;i<9;i++){
		sLPFS += (sourceBin[i][1]+sourceBin[i][2]);
		sRPFS += (sourceBin[i][3]+sourceBin[i][4]);
		sLAFS += (sourceBin[i][7]+sourceBin[i][8]);
		sRAFS += (sourceBin[i][9]+sourceBin[i][10]);
	}
	cout<<"For the source:"<<endl;
	cout<<"LPFS:"<<sLPFS<<endl;
	cout<<"RPFS:"<<sRPFS<<endl;
	cout<<"LAFS:"<<sLAFS<<endl;
	cout<<"RAFS:"<<sRAFS<<endl;

	ofstream symmetry;
	symmetry.open("SymmetryAnalysis.txt");
	symmetry<<"Patient 2:"<<endl;
	symmetry<<"LAFS: "<<sLAFS<<endl;
	symmetry<<"RAFS: "<<sRAFS<<endl;
	double sAFS = (sLAFS-sRAFS)/(sLAFS+sRAFS);
	symmetry<<"AFS: "<<sAFS<<endl<<endl;

	double tLPFS = 0;
	double tRPFS = 0;
	double tLAFS = 0;
	double tRAFS = 0;
	for(int i = 5;i<9;i++){
		tLPFS += (targetBin[i][1]+targetBin[i][2]);
		tRPFS += (targetBin[i][3]+targetBin[i][4]);
		tLAFS += (targetBin[i][7]+targetBin[i][8]);
		tRAFS += (targetBin[i][9]+targetBin[i][10]);
	}
	cout<<"For the target:"<<endl;
	cout<<"LPFS:"<<tLPFS<<endl;
	cout<<"RPFS:"<<tRPFS<<endl;
	cout<<"LAFS:"<<tLAFS<<endl;
	cout<<"RAFS:"<<tRAFS<<endl;

	symmetry<<"Patient 1:"<<endl;
	symmetry<<"LAFS: "<<tLAFS<<endl;
	symmetry<<"RAFS: "<<tRAFS<<endl;
	double tAFS = (tLAFS-tRAFS)/(tLAFS+tRAFS);
	symmetry<<"AFS: "<<tAFS<<endl;

	symmetry.close();

	//Compute the comparision
	/*vector<vector<double> > compBin(binNum, vector<double>(binNum));
	for(int i = 0; i<binNum; i++){
		for(int j = 0; j<binNum; j++ ){
			compBin[i][j] = 0;
		}
	}
	for(int i = 0; i < sSize; i++){
		Vec3& nV = *(sourceVector->get(i));
		double theta = atan(nV[2]/nV[0]);
		double varphi = atan(nV[1]/sqrt(nV[0]*nV[0]+nV[2]*nV[2]));
		int thetaIndex = (int)((theta + PI)/ (2*PI/binNum));
		int varphiIndex =  (int)((varphi + PI/2)/ (PI/binNum));
		
		if(thetaIndex >=0 && thetaIndex< binNum && varphiIndex >=0 && varphiIndex <binNum ){
			double rawSM = sourceMean->buffer[i];
			double rawTM = targetMean->buffer[(*_cpdMap)[i]];
			double nMean = 1 - exp(-1*lambda*abs((rawSM-rawTM)/(rawSM+rawTM)));
			nMean = nMean>1?1:nMean;
			nMean = nMean<0?0:nMean;
			compBin[thetaIndex][varphiIndex] += nMean;
		}
		else{
			cout<<thetaIndex<<" "<<varphiIndex<<endl;
			nV.print();
		}
	}
	ofstream cOutput;
	cOutput.open("compareBins.txt");
	if(cOutput.good()){
		cOutput<<binNum<<endl;
		for(int i=0; i<binNum;i++){
			for(int j=0; j<binNum;j++){
				cOutput<<compBin[i][j]<<endl;
			}
		}
	}*/
	cout<<"Bin computation is done"<<endl;
}