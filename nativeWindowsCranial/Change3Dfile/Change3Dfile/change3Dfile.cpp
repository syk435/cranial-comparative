#include<iostream>
#include<fstream>
#include<string>

using namespace std;


/*
 * The obj format file change to the form that can be open by OpenGl
 * string s defines the name of the input file without 
 */
void MayatoOpenGLfileconvertor(string s, double Scalesize)
{
	cout<<"Change the .obj file to the format that can be load by OpenGL."<<endl;
	ifstream objfile;
	objfile.open(s + ".obj");
	if(objfile.is_open())
	{
		cout<<"Opening obj file succeeds!"<<endl;
	}
	else
	{
		cout<<"Opening obj file fails"<<endl;
		return;
	}
	//get rid of some meanless words.
	string temp;
	while(temp!="v")
	{
		objfile>>temp;
		//cout<<temp;
	}
	double pos_x, pos_y, pos_z;
	//count the number of the vertex
	int V_counter=0;
	while(true)
	{
		V_counter++;
		objfile>>pos_x>>pos_y>>pos_z;
		//cout<<pos_x<<" "<<pos_y<<" "<<pos_z<<endl;
		//openGlfile<<pos_x<<" "<<pos_y<<" "<<pos_z<<endl;
		objfile>>temp;
		if(temp!="v")
		{
			break;
		}
	}
	cout<<"Total number of vertexs: "<<V_counter<<endl;
	//the counter result for VT should be the same as vertex.
	/*double image_pos_x, image_pos_y;
	int VT_counter=0;
	while(true)
	{
		VT_counter++;
		objfile>>image_pos_x>>image_pos_y;
		//cout<<image_pos_x<<" "<<image_pos_y<<endl;
		objfile>>temp;
		if(temp!="vt")
		{
			break;
		}
	}
	cout<<"Total number of related position in the image: "<<VT_counter<<endl;*/
	while(temp!="f")
	{
		objfile>>temp;
		cout<<temp;
	}
	int Vindex1,Vindex2,Vindex3,VTindex1,VTindex2,VTindex3;
	//count the number of the face
	int F_counter=0;
	char c;
	while(true)
	{
		F_counter++;
		objfile>>Vindex1;
		/*objfile>>c;
		if(c!='/')
		{
			cout<<"Wrong condition in read face information"<<endl;
		}
		objfile>>c;
		if(c!='/')
		{
			cout<<"Wrong condition in read face information"<<endl;
		}*/
		//objfile>>VTindex1;
		objfile>>Vindex2;
		/*objfile>>c;
		if(c!='/')
		{
			cout<<"Wrong condition in read face information"<<endl;
		}
		objfile>>c;
		if(c!='/')
		{
			cout<<"Wrong condition in read face information"<<endl;
		}*/
		//objfile>>VTindex2;
		objfile>>Vindex3;
		/*objfile>>c;
		if(c!='/')
		{
			cout<<"Wrong condition in read face information"<<endl;
		}
		objfile>>c;
		if(c!='/')
		{
			cout<<"Wrong condition in read face information"<<endl;
		}
		//objfile>>VTindex3;*/
		cout<<Vindex1<<" "<<Vindex2<<" "<<Vindex3<<endl;
		//openGlfile<<Vindex1<<" "<<Vindex2<<" "<<Vindex3<<endl;
		objfile>>temp;
		if(temp=="#")
		{
			break;
		}
	}
	cout<<"Total number of face: "<<F_counter<<endl;
	objfile.close();
	ofstream openGLfile;
	openGLfile.open(s+".txt");
	if(openGLfile.is_open())
	{
		cout<<"Opening openGl file succeeds!"<<endl;
	}
	else
	{
		cout<<"Opening openGl file fails"<<endl;
		return;
	}
	//openGlfile<<temp;
	objfile.open(s+".obj");
	if(objfile.is_open())
	{
		cout<<"Opening obj file succeeds!<2>"<<endl;
	}
	else
	{
		cout<<"Opening obj file fails.<2>"<<endl;
		return;
	}
	/*
	ifstream openGLheader;
	openGLheader.open("OpenGLheader.txt");
	if(openGLheader.is_open())
	{
		cout<<"Open obj file succeeds!"<<endl;
	}
	else
	{
		cout<<"Opening obj file fails"<<endl;
	}
	//read the openGL header
	double r,g,b;
	int e,light_num;
	openGLheader>>r>>g>>b;
	openGLfile<<r<<" "<<g<<" "<<b<<endl;
	openGLheader>>r>>g>>b;
	openGLfile<<r<<" "<<g<<" "<<b<<endl;
	openGLheader>>r>>g>>b;
	openGLfile<<r<<" "<<g<<" "<<b<<endl;
	openGLheader>>e;
	openGLfile<<e<<endl;
	openGLheader>>r>>g>>b;
	openGLfile<<r<<" "<<g<<" "<<b<<endl;
	openGLheader>>light_num;
	openGLfile<<light_num<<endl;
	for(int i=0;i<light_num;i++)
	{
		openGLheader>>r>>g>>b;
		openGLfile<<r<<" "<<g<<" "<<b<<endl;
		openGLheader>>r>>g>>b;
		openGLfile<<r<<" "<<g<<" "<<b<<endl;
	}
	openGLheader.close();*/
	while(temp!="v")
	{
		objfile>>temp;
		//cout<<temp;
	}
	openGLfile<<V_counter<<endl;
	for(int i=0; i < V_counter; i++)
	{
		objfile>>pos_x>>pos_y>>pos_z;
		cout<<pos_x/Scalesize<<" "<<pos_y/Scalesize<<" "<<pos_z/Scalesize<<endl;
		openGLfile<<pos_x/Scalesize<<" "<<pos_y/Scalesize<<" "<<pos_z/Scalesize<<endl;
		objfile>>temp;
	}
	/*while(true)
	{
		objfile>>image_pos_x>>image_pos_y;
		objfile>>temp;
		if(temp!="vt")
		{
			break;
		}
	}*/
	while(temp!="f")
	{
		objfile>>temp;
		//cout<<temp;
	}
	openGLfile<<F_counter<<endl;
	for(int i=0;i<F_counter;i++)
	{
		objfile>>Vindex1;
		/*objfile>>c;
		if(c!='/')
		{
			cout<<"Wrong condition in read face information"<<endl;
		}
		objfile>>c;
		if(c!='/')
		{
			cout<<"Wrong condition in read face information"<<endl;
		}*/
		//objfile>>VTindex1;//C++ counts from 0 while Maya connts from 1;
		objfile>>Vindex2;
		/*objfile>>c;
		if(c!='/')
		{
			cout<<"Wrong condition in read face information"<<endl;
		}
		objfile>>c;
		if(c!='/')
		{
			cout<<"Wrong condition in read face information"<<endl;
		}*/
		//objfile>>VTindex2;
		objfile>>Vindex3;
		/*objfile>>c;
		if(c!='/')
		{
			cout<<"Wrong condition in read face information"<<endl;
		}
		objfile>>c;
		if(c!='/')
		{
			cout<<"Wrong condition in read face information"<<endl;
		}*/
		//objfile>>VTindex3;
		cout<<Vindex1<<" "<<Vindex2<<" "<<Vindex3<<endl;
		openGLfile<<Vindex1-1<<" "<<Vindex2-1<<" "<<Vindex3-1<<endl;
		if(objfile.eof())
		{
			break;
		}
		objfile>>temp;
	}
	cout<<"Done!"<<endl;
}



/*
 *  string s defines the name of the input openGL file
 */
void OpenGLtoMatlabfileconvertor(string s)
{
	ifstream openGLfile;
	openGLfile.open(s+".txt");
	ofstream Matlabfile;
	Matlabfile.open(s+"_matlab.txt");
	if(openGLfile.is_open())
	{
		cout<<"Open obj file succeeds!"<<endl;
	}
	else
	{
		cout<<"Opening obj file fails"<<endl;
	}
	if(Matlabfile.is_open())
	{
		cout<<"Open Matlab file succeed!"<<endl;
	}
	else
	{
		cout<<"Open Matlab file fails"<<endl;
	}
	//read the openGL header and that is not included in the Matlab file
	double r,g,b;
	int e,light_num;
	openGLfile>>r>>g>>b;
	openGLfile>>r>>g>>b;
	openGLfile>>r>>g>>b;
	openGLfile>>e;
	openGLfile>>r>>g>>b;
	openGLfile>>light_num;
	for(int i=0;i<light_num;i++)
	{
		openGLfile>>r>>g>>b;
		openGLfile>>r>>g>>b;
	}
	int V_counter, F_counter;
	openGLfile>>V_counter;
	Matlabfile<<V_counter<<endl;
	double pos_x,pos_y,pos_z;
	for(int i=0;i<V_counter;i++)
	{
		openGLfile>>pos_x>>pos_y>>pos_z;
		Matlabfile<<pos_x<<" "<<pos_y<<" "<<pos_z<<endl;
	}
	openGLfile>>F_counter;
	Matlabfile<<F_counter<<endl;
	int point1, point2, point3;
	for(int i=0;i<F_counter;i++)
	{
		openGLfile>>point1>>point2>>point3;
		Matlabfile<<point1+1<<" "<<point2+1<<" "<<point3+1<<endl;
	}
	openGLfile.close();
	Matlabfile.close();
}



void main(int argc, char *argv[])
{
	//MayatoOpenGLfileconvertor("R1_Nose",100);
	//MayatoOpenGLfileconvertor("test_out",100);
	//OpenGLtoMatlabfileconvertor("tetra");
	//OpenGLtoMatlabfileconvertor("sphere");
	//OpenGLtoMatlabfileconvertor("frog");
	//OpenGLtoMatlabfileconvertor("horse");
	//OpenGLtoMatlabfileconvertor("mechpart");
	//OpenGLtoMatlabfileconvertor("face");

	//MayatoOpenGLfileconvertor("Refined_after_reg_8745",100);
	//MayatoOpenGLfileconvertor("Refined_before_reg_8274",100);
	
	//MayatoOpenGLfileconvertor("Patient1_140611093239_Refined_8057",100);
	//MayatoOpenGLfileconvertor("Patient1_140721140018_Refined_8146",100);
	//MayatoOpenGLfileconvertor("Patient2_131106134948_Refined_8048",100);
	//MayatoOpenGLfileconvertor("Patient2_140723101551_Refined_7969",100);
	//MayatoOpenGLfileconvertor("Patient3_140319120242_Refined_8139",100);
	//MayatoOpenGLfileconvertor("Patient4_140402110251_Refined_8221",100);
	//MayatoOpenGLfileconvertor("Patient4_140611102320_Refined_8070",100);
	//MayatoOpenGLfileconvertor("Patient5_140609095416_Refined_8038",100);
	//MayatoOpenGLfileconvertor("Patient5_140716113238_Refined_8171",100);


	//MayatoOpenGLfileconvertor("9016084POST2_Sim_7400",100);
	//MayatoOpenGLfileconvertor("9016084PRE_Sim_7447",100);
	//MayatoOpenGLfileconvertor("9016091POST4_Sim_7252",100);
	//MayatoOpenGLfileconvertor("9016091PRE_Sim_7444",100);
	//MayatoOpenGLfileconvertor("3000415814POST1_Sim_8792",100);
	//MayatoOpenGLfileconvertor("3000415814PRE_Sim_8371",100);
	//MayatoOpenGLfileconvertor("3001425158POST_Sim_6903",100);
	//MayatoOpenGLfileconvertor("3001425158PRE_Sim_6951",100);
	//MayatoOpenGLfileconvertor("3001518168POST_Sim_8251",100);
	MayatoOpenGLfileconvertor("C:/Users/Dell/Documents/OCAF/OCAFComparativeRelease/stage/Update_02",100);
	//MayatoOpenGLfileconvertor(argv[2],100);

}