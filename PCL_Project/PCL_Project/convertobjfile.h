
#include <iostream>
#include <fstream>
#include <string>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

pcl::PointCloud<pcl::PointXYZRGB> convertObjFile(std::string filename,bool outputFlag)
{
	pcl::PointCloud<pcl::PointXYZRGB> cloud;
	std::ifstream input;
	input.open(filename+".obj");
	if(!input.is_open()){
		std::cout<<"Can not open input<"<<filename<<".obj> file!"<<std::endl;
		return cloud;
	}
	//get rid of the objfile head 
	std::string s;
	while(s!="v"){
		input>>s;
	}
	while(s=="v"){
		pcl::PointXYZRGB current;
		input>>current.x>>current.y>>current.z; 
		current.r = 128;
		current.g = 128;
		current.b = 128;
		cloud.points.push_back(current);
		input>>s;
	}
	// Fill in the cloud configuration
	cloud.height   = 1;
	cloud.is_dense = false;
	cloud.width = cloud.points.size();
	cloud.points.resize (cloud.width * cloud.height);
	if(outputFlag){
		pcl::io::savePCDFileASCII (filename+".pcd", cloud);
		std::cout << "Saved " << cloud.points.size () << " data points to "<<filename<<".pcd." << std::endl;
	}
	/*for (size_t i = 0; i < cloud.points.size (); ++i){
		std::cerr << "    " << cloud.points[i].x << " " << cloud.points[i].y << " " << cloud.points[i].z << std::endl;
	}*/
	return cloud;
}