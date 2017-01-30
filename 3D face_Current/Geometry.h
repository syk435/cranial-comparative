



typedef struct Edge{
    int start;
    int end;

    Edge(int a,int b){
        assert(a!=b);
		if(a<b){
			start = a;
            end = b;
		}
		else{
			start = b;
			end = a;
		}	
    }
    friend bool operator < (const Edge & e1,const Edge & e2){
		if(e1.start<e2.start){
            return true;
        }
        else if(e1.start>e2.start){
            return false;
        }
        else{
            if(e1.end<e2.end){
                return true;
            }
            else{
                return false;
            }
        }
    }
}Edge;



typedef struct LineSeg{
	int startID;
	int endID;
}LineSeg;


// To store the curvature values
typedef struct Curvature
{
	double* buffer;
	int size;
	double MaxValue;
	double MinValue;
	Curvature(){
		buffer=NULL;
		size =0;
		MaxValue = 0;
		MinValue = 0;
	}
	Curvature(int s){
		buffer= new double[s];
		size = s;
		MaxValue = 0;
		MinValue = 0;
	}
}Curvature;

