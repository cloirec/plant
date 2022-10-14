#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkJPEGReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkXMLImageDataWriter.h>
#include <vtkImageData.h>
#include <vtkXMLImageDataReader.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkImageRGBToHSV.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageThreshold.h>
#include <vtkVersion.h>
#include <vtkInformation.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkImageSeedConnectivity.h>
#include <vtkImageShrink3D.h>
#include <vtkClipPolyData.h>

int main(int argc, char* argv[])
{
  //Verify input arguments
  if ( argc != 3 )
    {
      std::cout << "Usage: " << argv[0] 
		<< " Input Filename(.jpeg/jpg) Output Filename (.vtp) " << std::endl;
      return EXIT_FAILURE;
    }
  
  //Read the image
  vtkSmartPointer<vtkJPEGReader> JpegImage =
    vtkSmartPointer<vtkJPEGReader>::New();
  JpegImage->SetFileName(argv[1]);
  
  // Convert the image to hsv so that we can threshold on value
  vtkSmartPointer<vtkImageRGBToHSV> toHSV =
    vtkSmartPointer<vtkImageRGBToHSV>::New();
  toHSV->SetInputConnection(JpegImage->GetOutputPort());
  
  // Just work with the value
  vtkSmartPointer<vtkImageExtractComponents> extractImage =
    vtkSmartPointer<vtkImageExtractComponents>::New();
  extractImage->SetInputConnection(toHSV->GetOutputPort());
  extractImage->SetComponents(2);
  
  // Threshold to a image
  vtkSmartPointer<vtkImageThreshold> threshold_image =
    vtkSmartPointer<vtkImageThreshold>::New();
  threshold_image->SetInputConnection(extractImage->GetOutputPort());
  threshold_image->ThresholdByUpper(230);
  threshold_image->SetInValue(255);
  threshold_image->SetOutValue(0);
  threshold_image->Update();  
  threshold_image->UpdateInformation();

  // Place a seed in each corner and label connected pixels with 255
  int *extent = threshold_image->GetOutputInformation(0)->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT());
  vtkSmartPointer<vtkImageSeedConnectivity> connect =
    vtkSmartPointer<vtkImageSeedConnectivity>::New();
  connect->SetInputConnection(threshold_image->GetOutputPort());
  connect->SetInputConnectValue(255);
  connect->SetOutputConnectedValue(255);
  connect->SetOutputUnconnectedValue(0);
  connect->AddSeed(extent[0], extent[2]);
  connect->AddSeed(extent[1], extent[2]);
  connect->AddSeed(extent[1], extent[3]);
  connect->AddSeed(extent[0], extent[3]);
  connect->Update();
  

  //shrink the image
  vtkSmartPointer<vtkImageShrink3D> shrink =
    vtkSmartPointer<vtkImageShrink3D>::New();
  shrink->SetInputConnection(connect->GetOutputPort());
  shrink->SetShrinkFactors(1, 1, 1);
  shrink->AveragingOn();

  // Convert the image to polydata
  vtkSmartPointer<vtkImageDataGeometryFilter> geometry =
    vtkSmartPointer<vtkImageDataGeometryFilter>::New();
  geometry->SetInputConnection(shrink->GetOutputPort());

  // Clip the geometry : separate the object from the background
  vtkSmartPointer<vtkClipPolyData> clip =
    vtkSmartPointer<vtkClipPolyData>::New();
  clip->SetInputConnection(geometry->GetOutputPort());
  clip->SetValue(5.5);
  clip->GenerateClipScalarsOff();
  clip->InsideOutOff();
  clip->InsideOutOn();
  clip->Update();

  //Create final 2D poyldata mesh
   vtkSmartPointer<vtkPolyData> polydata;
   polydata = clip->GetOutput();

   //Write the polydata mesh as a .vtp file
  vtkSmartPointer<vtkXMLPolyDataWriter> xml_polydata_writer =
    vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  xml_polydata_writer->SetInputData(polydata);
  xml_polydata_writer->SetFileName(argv[2]);
  xml_polydata_writer->Write();

  return EXIT_SUCCESS;
}
