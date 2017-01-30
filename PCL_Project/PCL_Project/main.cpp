#define  EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET
#include "computeCurvature.cpp"
#include "outputCurvature.cpp"

int main(){
	computeCurvature("Refined_before");
	//pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
	//*cloud = convertObjFile("Refined_before",false);
	return 0;
}