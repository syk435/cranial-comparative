#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class PointSet{
private:
	int count;
	vector<vector<double> > pointArray;
public:
	PointSet();
	void Read(ifstream& input);
	void Write(ofstream& output);
};

PointSet::PointSet(){
	count = 0;
	pointArray.clear();
}

void PointSet::Read(ifstream& input){
	string temp;
	while(temp.compare("v")!=0){
		input>>temp;
	}
	int i = 0;
	double x,y,z;
	while(temp.compare("v")==0){
		input>>x>>y>>z;
		vector<double> point;
		point.push_back(x);
		point.push_back(y);
		point.push_back(z);
		pointArray.push_back(point);
		input>>temp;
		i++;
	}
	count = i;
}

void PointSet::Write(ofstream& output){
	output<<count<<endl;
	for(int i=0;i<count;i++){
		output<<pointArray[i][0]<<" "<<pointArray[i][1]<<" "<<pointArray[i][2]<<endl;
	}
}


void change(string filename, string outputFilename){
	cout<<"Change: "<<filename<<endl;
	ifstream input;
	input.open(filename);
	if(!input.is_open()){
		cout<<"No such file"<<endl;
	}
	ofstream output;
	output.open(outputFilename);
	PointSet current;
	current.Read(input);
	current.Write(output);
}


void main(int argc, char *argv[]){
	// Change the filename here.
	//change("9016084POST2_Sim_7400");
	//change("9016084PRE_Sim_7447");
	//change("9016091POST4_Sim_7252");
	//change("9016091PRE_Sim_7444");
	//change("3000415814POST1_Sim_8792");
	//change("3000415814PRE_Sim_8371");
	//change("3001425158POST_Sim_6903");
	//change("3001425158PRE_Sim_6951");
	//change("3001518168POST_Sim_8251");
	//change("3001518168PRE_Sim_8971");
	change(argv[1],argv[2]);
	//change("C:/Users/Dell/Documents/TestGround/CPD_tests/Data/Patient2_decimated96percent");
}

