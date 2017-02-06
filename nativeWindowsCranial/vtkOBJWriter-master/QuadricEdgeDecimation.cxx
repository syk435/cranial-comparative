/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkOBJReader.h>
#include "vtkOBJWriter.h"
#include "vtkPolyData.h"
#include <vtkQuadricDecimation.h>
#include "vtkQuadricClustering.h"
#include "vtkSmoothPolyDataFilter.h"

int main (int argc, char *argv[])
{
  vtkSmartPointer<vtkPolyData> input =
    vtkSmartPointer<vtkPolyData>::New();
  std::string outputFilename;

  // Verify command line arguments
  if(argc > 1) // Use the command line arguments
    {
    if(argc != 3)
      {
      std::cout << "Required arguments: InputFilename.vtp OutputFilename.obj" << std::endl;
      return EXIT_FAILURE;
      }
    vtkSmartPointer<vtkOBJReader> reader =
      vtkSmartPointer<vtkOBJReader>::New();
    reader->SetFileName(argv[1]);
    reader->Update();

    input->ShallowCopy(reader->GetOutput());

    outputFilename = argv[2];
    }
  else
    {
		return EXIT_FAILURE;
    }

  // QUADRIC DECIMATION SECTION
  vtkSmartPointer<vtkQuadricDecimation> decimate =
	  vtkSmartPointer<vtkQuadricDecimation>::New();
  decimate->SetInputData(input);
  decimate->SetTargetReduction(.9);
  decimate->Update();

  // QUADRIC CLUSTERING SECTION
  /*
  vtkSmartPointer<vtkQuadricClustering> decimate =
	  vtkSmartPointer<vtkQuadricClustering>::New();
  decimate->SetInputData(input);
  decimate->SetNumberOfXDivisions(32);
  decimate->SetNumberOfYDivisions(32);
  decimate->SetNumberOfZDivisions(32);
  decimate->Update();
  */

  // SMOOTH SECTION
  /*vtkSmartPointer<vtkSmoothPolyDataFilter> smoothFilter =
	  vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
  smoothFilter->SetInputData(input);
  smoothFilter->SetNumberOfIterations(10);
  smoothFilter->SetRelaxationFactor(0.05);
  smoothFilter->FeatureEdgeSmoothingOff();
  smoothFilter->BoundarySmoothingOn();
  smoothFilter->Update();*/
  

  vtkSmartPointer<vtkPolyData> decimated =
	  vtkSmartPointer<vtkPolyData>::New();
  //decimated->ShallowCopy(smoothFilter->GetOutput());
  decimated->ShallowCopy(decimate->GetOutput());

  vtkSmartPointer<vtkOBJWriter> writer = 
    vtkSmartPointer<vtkOBJWriter>::New();
  writer->SetInputData(decimated);
  writer->SetFileName(outputFilename.c_str());
  writer->Update();

  return EXIT_SUCCESS;
}
