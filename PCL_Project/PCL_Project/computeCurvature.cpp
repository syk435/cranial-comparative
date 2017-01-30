#define  EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET

#include <iostream>
#include <vector>
#include <limits>

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/features/normal_3d.h>
#include <pcl/features/principal_curvatures.h>
#include <pcl/visualization/cloud_viewer.h>

#include <pcl/PolygonMesh.h>
#include <pcl/io/ply_io.h>

#include "convertobjfile.h"



void colorByCuravture(pcl::PointCloud<pcl::PrincipalCurvatures>::Ptr principal_curvatures,pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud,std::string option){
	// use the curvature to color the point set.
	vector<float> curvature;
	float  max = std::numeric_limits<float>::min();
	float  min = std::numeric_limits<float>::max();
	int size = principal_curvatures->points.size();
	if(option=="max"){
		for(int i =0;i<size;i++){
			float temp = principal_curvatures->points[i].pc1;
			if(temp > max){
				max = temp;
			}
			if(temp < min){
				min = temp;
			}
			curvature.push_back(temp);
		}
	}
	else if(option == "min"){
		for(int i =0;i<size;i++){
			float temp = principal_curvatures->points[i].pc2;
			if(temp > max){
				max = temp;
			}
			if(temp < min){
				min = temp;
			}
			curvature.push_back(temp);
		}
	}
	else if(option == "Gaussian"){
		for(int i =0;i<size;i++){
			float temp = principal_curvatures->points[i].pc1*principal_curvatures->points[i].pc2;
			if(temp > max){
				max = temp;
			}
			if(temp < min){
				min = temp;
			}
			curvature.push_back(temp);
		}
	}
	else if(option == "mean"){
		for(int i =0;i<size;i++){
			float temp = (principal_curvatures->points[i].pc1+principal_curvatures->points[i].pc2)/2;
			if(temp > max){
				max = temp;
			}
			if(temp < min){
				min = temp;
			}
			curvature.push_back(temp);
		}
	}
	else{
		max = 1;
		min = 0;
		for(int i =0;i<principal_curvatures->points.size();i++){
			curvature.push_back(0.5);
		}
	}
	std::cout<<"Curvature range ("<<min<<", "<<max<<")"<<std::endl;
	float range = max - min;
	for(int i = 0; i<size;i++){
		curvature[i] = (curvature[i] - min)/range;
	}
	for(int i = 0 ; i<size;i++){
		if(curvature[i]>0.5){
			cloud->points[i].r = (int)(128*(curvature[i] - 0.5)/0.5);
			cloud->points[i].g = (int)(128*(1 - curvature[i]) / 0.5);
			cloud->points[i].b = 0;
		}
		else{
			cloud->points[i].r = 0;
			cloud->points[i].g =  (int)(128*curvature[i] / 0.5);
			cloud->points[i].b = (int)(128*(0.5- curvature[i] ))/0.5;
		}
	}
}




void colorByEuclidDistance(
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr sourceCloud,
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr targetCloud,
	int cpdMap[]
	)
{
	vector<float> curvature_diff;
	float  max = std::numeric_limits<float>::min();
	float  min = std::numeric_limits<float>::max();
	int size = sourceCloud->points.size();

	
	for(int i =0;i<size;i++){
		float s_X = sourceCloud->points[i].x;
		float s_Y = sourceCloud->points[i].y;
		float s_Z = sourceCloud->points[i].z;
		float t_X = targetCloud->points[cpdMap[i]].x;
		float t_Y = targetCloud->points[cpdMap[i]].y;
		float t_Z = targetCloud->points[cpdMap[i]].z;
		float temp = sqrt((s_X-t_X)*(s_X-t_X)+(s_Y-t_Y)*(s_Y-t_Y)+(s_Z-t_Z)*(s_Z-t_Z));
		if(temp > max){
			max = temp;
		}
		if(temp < min){
			min = temp;
		}
		curvature_diff.push_back(temp);
	}

	std::cout<<"Curvature range ("<<min<<", "<<max<<")"<<std::endl;
	float range = max - min;
	for(int i = 0; i<size;i++){
		curvature_diff[i] = (curvature_diff[i] - min)/range;
	}
	for(int i = 0 ; i<size;i++){
		if(curvature_diff[i]>0.5){
			sourceCloud->points[i].r = (int)(128*(curvature_diff[i] - 0.5)/0.5);
			sourceCloud->points[i].g = (int)(128*(1 - curvature_diff[i]) / 0.5);
			sourceCloud->points[i].b = 0;
		}
		else{
			sourceCloud->points[i].r = 0;
			sourceCloud->points[i].g =  (int)(128*curvature_diff[i] / 0.5);
			sourceCloud->points[i].b = (int)(128*(0.5- curvature_diff[i] ))/0.5;
		}
	}
}





void computeCurvature(string name)
//int main()
{
  /*
  pcl::PCLPointCloud2 cloud1;
  pcl::io::loadPLYFile("L10.ply",cloud1);
  pcl::io::savePCDFile("L10.pcd",cloud1);
  */

  pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);

  /*if (pcl::io::loadPCDFile<pcl::PointXYZRGB> ("torus.txt", *cloud) == -1) //* load the file
  {
	  
    PCL_ERROR ("Couldn't read file");system("pause");
    return (-1);
  }*/

  //load obj file by my own function
  *cloud = convertObjFile(name,false);

  std::cout << "Loaded " << cloud->points.size () << " points." << std::endl;

  // Compute the normals
  pcl::NormalEstimation<pcl::PointXYZRGB, pcl::Normal> normal_estimation;
  normal_estimation.setInputCloud (cloud);

  pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZRGB>);
  normal_estimation.setSearchMethod (tree);

  pcl::PointCloud<pcl::Normal>::Ptr cloud_with_normals (new pcl::PointCloud<pcl::Normal>);
  normal_estimation.setRadiusSearch (5);//0.03
  normal_estimation.compute (*cloud_with_normals);

  // Setup the principal curvatures computation
  pcl::PrincipalCurvaturesEstimation<pcl::PointXYZRGB, pcl::Normal, pcl::PrincipalCurvatures> principal_curvatures_estimation;

  // Provide the original point cloud (without normals)
  principal_curvatures_estimation.setInputCloud (cloud);

  // Provide the point cloud with normals
  principal_curvatures_estimation.setInputNormals (cloud_with_normals);

  // Use the same KdTree from the normal estimation
  principal_curvatures_estimation.setSearchMethod (tree);
  principal_curvatures_estimation.setRadiusSearch (5);

  // Actually compute the principal curvatures
  pcl::PointCloud<pcl::PrincipalCurvatures>::Ptr principal_curvatures (new pcl::PointCloud<pcl::PrincipalCurvatures> ());
  principal_curvatures_estimation.compute (*principal_curvatures);

  std::cout << "output points.size (): " << principal_curvatures->points.size () << std::endl;

  // Display and retrieve the shape context descriptor vector for the first 100th point.
  for(int i=0;i<100;i++){
	  pcl::PrincipalCurvatures descriptor = principal_curvatures->points[i];
	  std::cout << descriptor << std::endl;
  }

  //colorByCuravture(principal_curvatures,cloud,"max");

  //colorByCuravture(principal_curvatures,cloud,"min");

  //colorByCuravture(principal_curvatures,cloud,"Gaussian");

  colorByCuravture(principal_curvatures,cloud,"mean");
  
  




   // show the cloud
   pcl::visualization::CloudViewer viewer ("Simple Cloud Viewer");
   viewer.showCloud (cloud);
   while (!viewer.wasStopped ())
   {
   }

  cin.get();
}

/*
 * Color the source point set by comparsion of curvature
 */
void colorSourceByTarget(
	pcl::PointCloud<pcl::PrincipalCurvatures>::Ptr source_principal_curvatures,
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr sourceCloud,
	pcl::PointCloud<pcl::PrincipalCurvatures>::Ptr target_principal_curvatures,
	int cpdMap[],
	std::string option
	)
{
	vector<float> curvature_diff;
	float  max = std::numeric_limits<float>::min();
	float  min = std::numeric_limits<float>::max();
	int size = source_principal_curvatures->points.size();

	if(option=="max"){
		for(int i =0;i<size;i++){
			float tempSource = source_principal_curvatures->points[i].pc1;
			float tempTarget = target_principal_curvatures->points[cpdMap[i]-1].pc1;
			float temp = sqrt(abs((tempSource-tempTarget)/(tempSource+tempTarget)));
			if(temp > max){
				max = temp;
			}
			if(temp < min){
				min = temp;
			}
			curvature_diff.push_back(temp);
		}
	}
	else if(option == "min"){
		for(int i =0;i<size;i++){
			float tempSource = source_principal_curvatures->points[i].pc2;
			float tempTarget = target_principal_curvatures->points[cpdMap[i]-1].pc2;
			float temp = sqrt(abs((tempSource-tempTarget)/(tempSource+tempTarget)));
			if(temp > max){
				max = temp;
			}
			if(temp < min){
				min = temp;
			}
			curvature_diff.push_back(temp);
		}
	}
	else if(option == "Gaussian"){
		for(int i =0;i<size;i++){
			float tempSource = source_principal_curvatures->points[i].pc1*source_principal_curvatures->points[i].pc2;
			float tempTarget = target_principal_curvatures->points[cpdMap[i]-1].pc1*target_principal_curvatures->points[cpdMap[i]-1].pc2;
			float temp = sqrt(abs((tempSource-tempTarget)/(tempSource+tempTarget)));
			if(temp > max){
				max = temp;
			}
			if(temp < min){
				min = temp;
			}
			curvature_diff.push_back(temp);
		}
	}
	else if(option == "mean"){
		for(int i =0;i<size;i++){
			float tempSource = (source_principal_curvatures->points[i].pc1+source_principal_curvatures->points[i].pc2)/2;
			float tempTarget = (target_principal_curvatures->points[cpdMap[i]-1].pc1+target_principal_curvatures->points[cpdMap[i]-1].pc2)/2;
			float temp = sqrt(abs((tempSource-tempTarget)/(tempSource+tempTarget)));
			if(temp > max){
				max = temp;
			}
			if(temp < min){
				min = temp;
			}
			curvature_diff.push_back(temp);
		}
	}
	else{
		max = 1;
		min = 0;
		for(int i =0;i<size;i++){
			curvature_diff.push_back(0.5);
		}
	}
	std::cout<<"Curvature range ("<<min<<", "<<max<<")"<<std::endl;
	float range = max - min;
	for(int i = 0; i<size;i++){
		curvature_diff[i] = (curvature_diff[i] - min)/range;
	}
	for(int i = 0 ; i<size;i++){
		if(curvature_diff[i]>0.5){
			sourceCloud->points[i].r = (int)(128*(curvature_diff[i] - 0.5)/0.5);
			sourceCloud->points[i].g = (int)(128*(1 - curvature_diff[i]) / 0.5);
			sourceCloud->points[i].b = 0;
		}
		else{
			sourceCloud->points[i].r = 0;
			sourceCloud->points[i].g =  (int)(128*curvature_diff[i] / 0.5);
			sourceCloud->points[i].b = (int)(128*(0.5- curvature_diff[i] ))/0.5;
		}
	}

}



void outputCurvature(std::ostream& os, pcl::PointCloud<pcl::PrincipalCurvatures>::Ptr source_curvatures,string type){
	int size = source_curvatures->points.size();
	os<<size<<endl;
	if(type == "max"){
		for(int i=0;i<size;i++){
			os<<source_curvatures->points[i].pc1<<endl;
		}
	}
	else if(type == "min"){
		for(int i=0;i<size;i++){
			os<<source_curvatures->points[i].pc2<<endl;
		}
	}
	else if(type == "mean"){
		for(int i=0;i<size;i++){
			float temp = (source_curvatures->points[i].pc1 + source_curvatures->points[i].pc2)/2;
			os<<temp<<endl;
		}
	}
	else if(type == "Gaussian"){
		for(int i=0;i<size;i++){
			float temp = source_curvatures->points[i].pc1 * source_curvatures->points[i].pc2;
			os<<temp<<endl;
		}
	}
	else{
		std::cout<<"No such type! ("<<type<<")"<<std::endl;
	}
}



void compareTwoPointSetPair(string source,string target,string correspondenceMap,string type){
	/*
	*Load and compute the curvature for the source point set
	*/
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr sourceCloud(new pcl::PointCloud<pcl::PointXYZRGB>);
	*sourceCloud = convertObjFile(source,false);

	/*if (pcl::io::loadPCDFile<pcl::PointXYZRGB> ("New_target.txt", *sourceCloud) == -1) //* load the file
	{
		PCL_ERROR ("Couldn't read file");system("pause");
		//return (-1);
	}*/

	std::cout << "Loaded " << sourceCloud->points.size () << " points." << std::endl;
	// Compute the normals
	pcl::NormalEstimation<pcl::PointXYZRGB, pcl::Normal> source_normal_estimation;
	source_normal_estimation.setInputCloud (sourceCloud);
	pcl::search::KdTree<pcl::PointXYZRGB>::Ptr source_tree (new pcl::search::KdTree<pcl::PointXYZRGB>);
	source_normal_estimation.setSearchMethod (source_tree);
	pcl::PointCloud<pcl::Normal>::Ptr source_cloud_with_normals (new pcl::PointCloud<pcl::Normal>);
	source_normal_estimation.setRadiusSearch (5);//0.03
	source_normal_estimation.compute (*source_cloud_with_normals);
	// Setup the principal curvatures computation
	pcl::PrincipalCurvaturesEstimation<pcl::PointXYZRGB, pcl::Normal, pcl::PrincipalCurvatures> source_principal_curvatures_estimation;
	// Provide the original point cloud (without normals)
	source_principal_curvatures_estimation.setInputCloud (sourceCloud);
	// Provide the point cloud with normals
	source_principal_curvatures_estimation.setInputNormals (source_cloud_with_normals);
	// Use the same KdTree from the normal estimation
	source_principal_curvatures_estimation.setSearchMethod (source_tree);
	source_principal_curvatures_estimation.setRadiusSearch (5);
	// Actually compute the principal curvatures
	pcl::PointCloud<pcl::PrincipalCurvatures>::Ptr source_principal_curvatures (new pcl::PointCloud<pcl::PrincipalCurvatures> ());
	source_principal_curvatures_estimation.compute (*source_principal_curvatures);
	std::cout << "output points.size (): " << source_principal_curvatures->points.size () << std::endl;
	// Display and retrieve the shape context descriptor vector for the first 100th point.
	for(int i=0;i<10;i++){
		pcl::PrincipalCurvatures descriptor = source_principal_curvatures->points[i];
		std::cout << descriptor << std::endl;
	}
	// Write the value to file;
	std::ofstream sourceOutput;
	sourceOutput.open(source+"_"+type+"_curvature.txt");
	outputCurvature(sourceOutput,source_principal_curvatures,type);



	/*
	 * Load and compute the curvature for the target point set
	 */
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr targetCloud(new pcl::PointCloud<pcl::PointXYZRGB>);
	*targetCloud = convertObjFile(target,false);

	/*if (pcl::io::loadPCDFile<pcl::PointXYZRGB> ("New_target.txt", *targetCloud) == -1) //* load the file
	{
		PCL_ERROR ("Couldn't read file");system("pause");
		//return (-1);
	}*/



	std::cout << "Loaded " << targetCloud->points.size () << " points." << std::endl;
	// Compute the normals
	pcl::NormalEstimation<pcl::PointXYZRGB, pcl::Normal> target_normal_estimation;
	target_normal_estimation.setInputCloud (targetCloud);
	pcl::search::KdTree<pcl::PointXYZRGB>::Ptr target_tree (new pcl::search::KdTree<pcl::PointXYZRGB>);
	target_normal_estimation.setSearchMethod (target_tree);
	pcl::PointCloud<pcl::Normal>::Ptr target_cloud_with_normals (new pcl::PointCloud<pcl::Normal>);
	target_normal_estimation.setRadiusSearch (5);//0.03
	target_normal_estimation.compute (*target_cloud_with_normals);
	// Setup the principal curvatures computation
	pcl::PrincipalCurvaturesEstimation<pcl::PointXYZRGB, pcl::Normal, pcl::PrincipalCurvatures> target_principal_curvatures_estimation;
	// Provide the original point cloud (without normals)
	target_principal_curvatures_estimation.setInputCloud (targetCloud);
	// Provide the point cloud with normals
	target_principal_curvatures_estimation.setInputNormals (target_cloud_with_normals);
	// Use the same KdTree from the normal estimation
	target_principal_curvatures_estimation.setSearchMethod (target_tree);
	target_principal_curvatures_estimation.setRadiusSearch (5);
	// Actually compute the principal curvatures
	pcl::PointCloud<pcl::PrincipalCurvatures>::Ptr target_principal_curvatures (new pcl::PointCloud<pcl::PrincipalCurvatures> ());
	target_principal_curvatures_estimation.compute (*target_principal_curvatures);
	std::cout << "output points.size (): " << target_principal_curvatures->points.size () << std::endl;
	// Display and retrieve the shape context descriptor vector for the first 100th point.
	for(int i=0;i<100;i++){
		pcl::PrincipalCurvatures descriptor = target_principal_curvatures->points[i];
		std::cout << descriptor << std::endl;
	}
	std::ofstream targetOutput;
	targetOutput.open(target+"_"+type+"_curvature.txt");
	outputCurvature(targetOutput,target_principal_curvatures,type);


	/*
	 * Load the corresponedence map by CPD
	 */
	int mapSize = sourceCloud->points.size();
	int *cpdMap = new int[mapSize];
	std::ifstream correspondenceInput;
	correspondenceInput.open(correspondenceMap+".txt");
	for(int i=0;i<mapSize;i++){
		correspondenceInput>>cpdMap[i];
	}
	correspondenceInput.close();

	// Compute the color by the curvature difference
	
	colorSourceByTarget(source_principal_curvatures,sourceCloud,target_principal_curvatures,cpdMap,type);

	//colorByEuclidDistance(sourceCloud,targetCloud,cpdMap);

	 // show the cloud
   /*pcl::visualization::CloudViewer viewer ("Simple Cloud Viewer");
   viewer.showCloud (sourceCloud);
   while (!viewer.wasStopped ())
   {
   }
	cin.get();*/
}







int main(int argc, char *argv[]){

	//compareTwoPointSetPair("9016084/9016084POST2_Sim_7400","9016084/9016084PRE_Sim_7447","9016084/CPD_9016084COR","mean");
	//compareTwoPointSetPair("9016084/9016084POST2_Sim_7400","9016084/9016084PRE_Sim_7447","9016084/CPD_9016084COR","Gaussian");
	//compareTwoPointSetPair("9016084/9016084POST2_Sim_7400","9016084/9016084PRE_Sim_7447","9016084/CPD_9016084COR","max");
	//compareTwoPointSetPair("9016084/9016084POST2_Sim_7400","9016084/9016084PRE_Sim_7447","9016084/CPD_9016084COR","min");
	
	//compareTwoPointSetPair("9016091/9016091POST4_Sim_7252","9016091/9016091PRE_Sim_7444","9016091/CPD_9016091COR","mean");
	//compareTwoPointSetPair("9016091/9016091POST4_Sim_7252","9016091/9016091PRE_Sim_7444","9016091/CPD_9016091COR","Gaussian");
	//compareTwoPointSetPair("9016091/9016091POST4_Sim_7252","9016091/9016091PRE_Sim_7444","9016091/CPD_9016091COR","max");
	//compareTwoPointSetPair("9016091/9016091POST4_Sim_7252","9016091/9016091PRE_Sim_7444","9016091/CPD_9016091COR","min");

	//compareTwoPointSetPair("3000415814/3000415814POST1_Sim_8792","3000415814/3000415814PRE_Sim_8371","3000415814/CPD_3000415814COR","mean");
	//compareTwoPointSetPair("3000415814/3000415814POST1_Sim_8792","3000415814/3000415814PRE_Sim_8371","3000415814/CPD_3000415814COR","Gaussian");
	//compareTwoPointSetPair("3000415814/3000415814POST1_Sim_8792","3000415814/3000415814PRE_Sim_8371","3000415814/CPD_3000415814COR","max");
	//compareTwoPointSetPair("3000415814/3000415814POST1_Sim_8792","3000415814/3000415814PRE_Sim_8371","3000415814/CPD_3000415814COR","min");
	
	compareTwoPointSetPair(argv[1],argv[2],argv[3],"mean");
	compareTwoPointSetPair(argv[1],argv[2],argv[3],"Gaussian");
	compareTwoPointSetPair(argv[1],argv[2],argv[3],"max");
	compareTwoPointSetPair(argv[1],argv[2],argv[3],"min");
	
	//compareTwoPointSetPair("3001518168/3001518168POST_Sim_8251","3001518168/3001518168PRE_Sim_8971","3001518168/CPD_3001518168COR","mean");
	//compareTwoPointSetPair("3001518168/3001518168POST_Sim_8251","3001518168/3001518168PRE_Sim_8971","3001518168/CPD_3001518168COR","Gaussian");
	//compareTwoPointSetPair("3001518168/3001518168POST_Sim_8251","3001518168/3001518168PRE_Sim_8971","3001518168/CPD_3001518168COR","max");
	//compareTwoPointSetPair("C:/Users/Dell/Documents/OCAF/OCAFComparativeRelease/stage/3001518168POST_Sim_8251","C:/Users/Dell/Documents/OCAF/OCAFComparativeRelease/stage/3001518168PRE_Sim_8971","C:/Users/Dell/Documents/OCAF/OCAFComparativeRelease/stage/CPD_3001518168COR","mean");



	return 0;
}
