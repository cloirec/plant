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

int main(int argc, char* argv[])
{
  //Verify input arguments
  if ( argc != 3 )
  {
    std::cout << "Usage: " << argv[0] 
              << " Filename(.jpeg/jpg) Filename(.vti) " << std::endl;
    return EXIT_FAILURE;
  }

  //Read the image
  vtkSmartPointer<vtkJPEGReader> jpegReader =
    vtkSmartPointer<vtkJPEGReader>::New();
  jpegReader->SetFileName ( argv[1] );
  jpegReader->Update();

  //define vtkImageData from Jpeg
  vtkSmartPointer<vtkImageData> imageData;
  imageData = jpegReader->GetOutput();

  //write vti file from vtkImageData
  vtkSmartPointer<vtkXMLImageDataWriter> writer =
    vtkSmartPointer<vtkXMLImageDataWriter>::New();
  writer->SetFileName(argv[2]);
  writer->SetInputData(imageData);
  writer->Write();

 

  // Read the file (to test that it was written correctly)
  vtkSmartPointer<vtkXMLImageDataReader> reader =
    vtkSmartPointer<vtkXMLImageDataReader>::New();
  reader->SetFileName(argv[2]);
  reader->Update();

  // Convert the image to a polydata
  vtkSmartPointer<vtkImageDataGeometryFilter> imageDataGeometryFilter =
    vtkSmartPointer<vtkImageDataGeometryFilter>::New();
  imageDataGeometryFilter->SetInputConnection(reader->GetOutputPort());
  imageDataGeometryFilter->Update();

  vtkSmartPointer<vtkPolyData> polydata;
  polydata = imageDataGeometryFilter->GetOutput();

  vtkSmartPointer<vtkXMLPolyDataWriter> xml_polydata_writer =
    vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  xml_polydata_writer->SetInputData(polydata);
  xml_polydata_writer->SetFileName("hetre.vtp");
  xml_polydata_writer->Write();

  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(imageDataGeometryFilter->GetOutputPort());

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  // Setup rendering
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(actor);
  renderer->SetBackground(1,1,1);
  renderer->ResetCamera();

  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();

  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderWindow->Render();
  renderWindowInteractor->Initialize();
  renderWindowInteractor->Start();


  return EXIT_SUCCESS;
}
