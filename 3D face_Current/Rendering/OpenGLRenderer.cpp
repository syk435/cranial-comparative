#include "Rendering/OpenGLRenderer.h" 
#include "FL/gl.h"
#include "Common/Common.h" 

using namespace Util; 

void OpenGLRenderer::setView(View* view){
	Renderer::setView(view); 
	this->setColorMode(0,false);
	initLights(); 
}

void OpenGLRenderer::draw(){
	//cout<<"In draw(): color mode: "<<_colorMode<<endl;
	if(_scene){ 
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_LIGHTING); 
		//glEnable(GL_BLEND);
		//glDisable(GL_DEPTH_TEST);
		//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		//glEnable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION); 
		glPushMatrix(); 
		mglLoadMatrix(*_view->getProjection());
		glMatrixMode(GL_MODELVIEW); 
		glPushMatrix(); 
		mglLoadMatrix(*_view->getModelView()); 
		initLights(); 

		Material* mat = _scene->getMaterial(); 

		Color amb = mat->getAmbient(); 
		Color spec = mat->getSpecular(); 		
		Color diffuse = mat->getDiffuse(); 
		float ambv[4] = {(float)amb[0],(float)amb[1],(float)amb[2],1.f}; 
		float specv[4] = {(float)spec[0],(float)spec[1],(float)spec[2],1.f}; 
		float diffv[4] = {(float)diffuse[0],(float)diffuse[1],(float)diffuse[2],1.f}; 
		float sxp = (float) mat->getSpecExponent(); 

		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffv); 
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specv); 
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambv); 
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,sxp); 
		
		//Draw the mesh pair and make sure the correspondence map is correct!
		if(		this->getcolorMode() == COLOR_Default 
			|| this->getcolorMode() == COLOR_Curvature_Max_Double
			|| this->getcolorMode() == COLOR_Curvature_Min_Double
			|| this->getcolorMode() == COLOR_Curvature_Mean_Double
			|| this->getcolorMode() == COLOR_Curvature_Gaussian_Double){
			//Draw the source Mesh;
			Mesh* sourceMesh = _scene->getSourceMesh();
			Pt3Array* spts = sourceMesh->getPoints(); 
			TriIndArray* sinds = sourceMesh->getInds(); 
			Vec3Array* s_v_norms = sourceMesh->getVNormals(); 
			ColorArray * s_color = sourceMesh->getColor();
			if(getShadingModel()==SHADE_GOURAUD){
				glShadeModel(GL_SMOOTH); 
				glColor3f(1,1,0);
				glBegin(GL_TRIANGLES); 
				for(int j=0;j<sinds->size();j++){
					TriInd* ti = sinds->get(j); 
					for(int k=0;k<3;k++){
						Pt3& p = *spts->get((*ti)[k]); 
						Vec3& n = *s_v_norms->get((*ti)[k]); 
						Color c= *s_color->get((*ti)[k]);
						glColor3dv(&c[0]);
						glNormal3dv(&n[0]); 
						glVertex3dv(&p[0]); 
					}
				}
				glEnd(); 
			}
			else{
				glShadeModel(GL_FLAT); 
				glBegin(GL_TRIANGLES); 
				Vec3Array* s_f_norms = sourceMesh->getFNormals(); 
				for(int j=0;j<sinds->size();j++){
					TriInd* ti = sinds->get(j); 
					Vec3& n = *s_f_norms->get(j);
					glNormal3dv(&n[0]); 
					for(int k=0;k<3;k++){
						Pt3& p = *spts->get((*ti)[k]); 
						Color c= *s_color->get((*ti)[k]);
						glColor3dv(&c[0]);
						glVertex3dv(&p[0]); 
					}
				}
				glEnd(); 
			}
			//Draw the target Mesh;
			Mesh* targetMesh = _scene->getTargetMesh();
			Pt3Array* tpts = targetMesh->getPoints(); 
			TriIndArray* tinds = targetMesh->getInds(); 
			Vec3Array* t_v_norms = targetMesh->getVNormals(); 
			ColorArray* t_color = targetMesh->getColor();
			if(getShadingModel()==SHADE_GOURAUD){
				glShadeModel(GL_SMOOTH); 
				glColor3f(1,1,0);
				glBegin(GL_TRIANGLES); 
				for(int j=0;j<tinds->size();j++){
					TriInd* ti = tinds->get(j); 
					for(int k=0;k<3;k++){
						Pt3& p = *tpts->get((*ti)[k]); 
						Vec3& n = *t_v_norms->get((*ti)[k]); 
						Color c = *t_color->get((*ti)[k]);
						glColor3dv(&c[0]);
						glNormal3dv(&n[0]); 
						glVertex3dv(&p[0]); 
					}
				}
				glEnd(); 
			}
			else{
				glShadeModel(GL_FLAT); 
				glBegin(GL_TRIANGLES); 
				Vec3Array* t_f_norms = targetMesh->getFNormals(); 
				for(int j=0;j<tinds->size();j++){
					TriInd* ti = tinds->get(j); 
					Vec3& n = *t_f_norms->get(j);
					glNormal3dv(&n[0]); 
					for(int k=0;k<3;k++){
						Pt3& p = *tpts->get((*ti)[k]); 
						Color c = *t_color->get((*ti)[k]);;
						glColor3dv(&c[0]);
						glVertex3dv(&p[0]); 
					}
				}
				glEnd(); 
			}
			//Draw the corresponedence line
			assert(_scene->getSourceMesh()->getPoints()->size() == _scene->getcpdMap()->size());
			for(int i=0; i<spts->size();i=i+311){
				Pt3& start = *spts->get(i);
				int tID = _scene->getCorVertexID(i);
				Pt3& end = *tpts->get(tID);
				double colorFactor = double(i)/double(spts->size());
				double r, g, b;
				if(colorFactor > 0.5){
					r = (colorFactor - 0.5)/0.5;
					g = (1 - colorFactor)/0.5;
					b = 0;
				}
				else{
					r = 0;
					g = 2*(colorFactor);
					b = 2*(0.5-colorFactor); 
				}
				//cout<<colorFactor<<" "<<r<<" "<<g<<" "<<b<<endl;
				Color c(r,g,b,1);
				glColor3dv(&c[0]);
				glBegin(GL_LINES);
				glVertex3dv(&start[0]);
				glVertex3dv(&end[0]);
				glEnd();
				
				glColor3dv(&c[0]);
				glPointSize(10);
				glBegin(GL_POINTS);
				glVertex3dv(&start[0]);
				glVertex3dv(&end[0]);
				glEnd();
				
			}
			//Draw points
			/*Color c(1,1,1,1);
			glColor3dv(&c[0]);
			glPointSize(5.0);
			glBegin(GL_POINTS);
			for(int i= 0 ;i<spts->size();i++){
				Pt3& p = *spts->get(i);
				glVertex3dv(&p[0]);
			}
			for(int i= 0 ;i<tpts->size();i++){
				Pt3& p = *tpts->get(i);
				glVertex3dv(&p[0]);
			}
			glEnd();*/
			glMatrixMode(GL_PROJECTION); 
			glPopMatrix(); 
			glMatrixMode(GL_MODELVIEW); 
			glPopMatrix();
		}
		else if(this->getcolorMode() == COLOR_Curvature_Max
				|| this->getcolorMode() == COLOR_Curvature_Min
				|| this->getcolorMode() == COLOR_Curvature_Mean
				|| this->getcolorMode() == COLOR_Curvature_Gaussian
				|| this->getcolorMode() == COLOR_NormalVector){
			//Draw the source Mesh;
			Mesh* sourceMesh = _scene->getSourceMesh();
			Pt3Array* spts = sourceMesh->getPoints(); 
			TriIndArray* sinds = sourceMesh->getInds(); 
			Vec3Array* s_v_norms = sourceMesh->getVNormals(); 
			ColorArray * s_color = sourceMesh->getColor();
			if(getShadingModel()==SHADE_GOURAUD){
				glShadeModel(GL_SMOOTH); 
				glColor3f(1,1,0);
				glBegin(GL_TRIANGLES); 
				for(int j=0;j<sinds->size();j++){
					TriInd* ti = sinds->get(j); 
					for(int k=0;k<3;k++){
						Pt3& p = *spts->get((*ti)[k]); 
						Vec3& n = *s_v_norms->get((*ti)[k]); 
						Color c= *s_color->get((*ti)[k]);
						glColor3dv(&c[0]);
						glNormal3dv(&n[0]); 
						glVertex3dv(&p[0]); 
					}
				}
				glEnd(); 
			}
			else{
				glShadeModel(GL_FLAT); 
				glBegin(GL_TRIANGLES); 
				Vec3Array* s_f_norms = sourceMesh->getFNormals(); 
				for(int j=0;j<sinds->size();j++){
					TriInd* ti = sinds->get(j); 
					Vec3& n = *s_f_norms->get(j);
					glNormal3dv(&n[0]); 
					for(int k=0;k<3;k++){
						Pt3& p = *spts->get((*ti)[k]); 
						Color c= *s_color->get((*ti)[k]);
						glColor3dv(&c[0]);
						glVertex3dv(&p[0]); 
					}
				}
				glEnd(); 
			}
		}
		else if(this->getcolorMode()==COLOR_Gauss_Map){
			GLUquadricObj* sourceSphere;
			//Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//LoadIdentity();
			Color red(1,0,0);
			glColor3dv(&red[0]);
			glTranslatef(1.1,0,0);
			sourceSphere=gluNewQuadric();
			gluSphere(sourceSphere,1,20,20);
			gluDeleteQuadric(sourceSphere);
			// Draw the distribution of the source normal vector;
			Mesh* sourceMesh = _scene->getSourceMesh();
			Pt3Array* spts = sourceMesh->getVNormals(); 
			Color c(1,1,1,1);
			glColor3dv(&c[0]);
			glPointSize(2.0);
			glBegin(GL_POINTS);
			for(int i= 0 ;i<spts->size();i++){
				Pt3& p = *spts->get(i);
				glVertex3dv(&p[0]);
			}
			glEnd();
			

			GLUquadricObj* targetSphere;
			Color blue(0,0,1);
			glColor3dv(&blue[0]);
			glTranslatef(-2.2,0,0);
			targetSphere=gluNewQuadric();
			gluSphere(targetSphere,1,20,20);
			gluDeleteQuadric(targetSphere);
			// Draw the distribution of the target normal vector;
			Mesh* targetMesh = _scene->getTargetMesh();
			Pt3Array* tpts = targetMesh->getVNormals(); 
			glColor3dv(&c[0]);
			glPointSize(2.0);
			glBegin(GL_POINTS);
			for(int i= 0 ;i<tpts->size();i++){
				Pt3& p = *tpts->get(i);
				glVertex3dv(&p[0]);
			}
			glEnd();
		}

	}
}

void OpenGLRenderer::initLights(){
	glEnable(GL_LIGHTING); 

	int nlights = _scene->getNumLights(); 
	if(nlights>0){
		Color amb = _scene->getLight(0)->getAmbient(); 
		float ambv[4] = {(float)amb[0],(float)amb[1],(float)amb[2],1.f}; 

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambv); 
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);

		for(int j=0;j<8;j++){
			int lid = GL_LIGHT0+j; 
			glDisable(lid); 
		}

		for(int j=0;j<_scene->getNumLights();j++){
			Light* l = _scene->getLight(j); 

			int lid = GL_LIGHT0+j; 
			glEnable(lid); 
			Color c = l->getColor(); 
			Pt3 p = l->getPos(); 

			float cv[4] = {(float)c[0],(float)c[1],(float)c[2],1.f}; 
			float pv[4] = {(float)p[0],(float)p[1],(float)p[2],1.f}; 

			glLightfv(lid,GL_DIFFUSE,cv); 
			glLightfv(lid,GL_SPECULAR,cv); 
			glLightfv(lid,GL_POSITION,pv); 
		}
		//glDisable(GL_COLOR_MATERIAL); 	
		//_colorMode = COLOR_Default;
	}
}