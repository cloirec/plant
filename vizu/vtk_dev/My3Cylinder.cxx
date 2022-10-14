/*=========================================================================

  Program:   Visualization Toolkit
  Module:    Cylinder.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//
// This simple example shows how to do basic rendering and pipeline
// creation using C++.
//
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCylinderSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
//#include <vtkPropAssembly.h>
#include <vtkSmartPointer.h>


int main()
{
  // This creates a polygonal cylinder model with eight circumferential facets.
  //
  vtkCylinderSource* cylinder1 = vtkCylinderSource::New();
  cylinder1->SetResolution(12);
  cylinder1->SetHeight(4);
  cylinder1->SetRadius(1);
  cylinder1->SetCenter(0,2,0);

  vtkCylinderSource* cylinder2 = vtkCylinderSource::New();
  cylinder2->SetResolution(12);
  cylinder2->SetHeight(2);
  cylinder2->SetRadius(0.5);
  cylinder2->SetCenter(0,5,0);

  vtkCylinderSource* cylinder3 = vtkCylinderSource::New();
  cylinder3->SetResolution(12);
  cylinder3->SetHeight(3);
  cylinder3->SetRadius(0.25);
  cylinder3->SetCenter(0,6.5,-3);

  // The mapper is responsible for pushing the geometry into the graphics
  // library. It may also do color mapping, if scalars or other attributes
  // are defined.
  //
  vtkPolyDataMapper* cylinder1Mapper = vtkPolyDataMapper::New();
  vtkPolyDataMapper* cylinder2Mapper = vtkPolyDataMapper::New();
  vtkPolyDataMapper* cylinder3Mapper = vtkPolyDataMapper::New();
  cylinder1Mapper->SetInputConnection(cylinder1->GetOutputPort());
  cylinder2Mapper->SetInputConnection(cylinder2->GetOutputPort());
  cylinder3Mapper->SetInputConnection(cylinder3->GetOutputPort());

  // The actor is a grouping mechanism: besides the geometry (mapper), it
  // also has a property, transformation matrix, and/or texture map.
  // Here we set its color and rotate it -22.5 degrees.
  vtkActor* cylinder1Actor = vtkActor::New();
  cylinder1Actor->SetMapper(cylinder1Mapper);
  cylinder1Actor->GetProperty()->SetColor(1.0000, 0.3882, 0.2784);
  //  cylinder1Actor->RotateX(30.0);
  //cylinder1Actor->RotateY(-45.0);

  vtkActor* cylinder2Actor = vtkActor::New();
  cylinder2Actor->SetMapper(cylinder2Mapper);
  cylinder2Actor->GetProperty()->SetColor(1.0000, 0.3882, 0.2784);
  //cylinder2Actor->RotateX(30.0);
  //cylinder2Actor->RotateY(-45.0);

  vtkActor* cylinder3Actor = vtkActor::New();
  cylinder3Actor->SetMapper(cylinder3Mapper);
  cylinder3Actor->GetProperty()->SetColor(1.0000, 0.3882, 0.2784);
  cylinder3Actor->RotateX(30.0);
  //cylinder3Actor->RotateY(-45.0);

  // Create the graphics structure. The renderer renders into the
  // render window. The render window interactor captures mouse events
  // and will perform appropriate camera or actor manipulation
  // depending on the nature of the events.
  //
  vtkRenderer* ren1 = vtkRenderer::New();
  vtkRenderWindow* renWin = vtkRenderWindow::New();
  renWin->AddRenderer(ren1);
  vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
  iren->SetRenderWindow(renWin);

  // Add the actors to the renderer, set the background and size
  //
  ren1->AddActor(cylinder1Actor);
  ren1->AddActor(cylinder2Actor);
  ren1->AddActor(cylinder3Actor);
  ren1->SetBackground(0.1, 0.2, 0.4);
  renWin->SetSize(200, 200);

  //add axes
  vtkSmartPointer<vtkAxesActor> axes = 
    vtkSmartPointer<vtkAxesActor>::New();
  
  vtkSmartPointer<vtkOrientationMarkerWidget> widget = 
    vtkSmartPointer<vtkOrientationMarkerWidget>::New();
  widget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
  widget->SetOrientationMarker( axes );
  widget->SetInteractor(iren);
  widget->SetViewport( 0.0, 0.0, 0.4, 0.4 );
  widget->SetEnabled( 1 );
  widget->InteractiveOn();

  // We'll zoom in a little by accessing the camera and invoking a "Zoom"
  // method on it.
  ren1->ResetCamera();
  ren1->GetActiveCamera()->Zoom(1.5);
  renWin->Render();

  // This starts the event loop and as a side effect causes an initial render.
  iren->Start();

  // Exiting from here, we have to delete all the instances that
  // have been created.
  cylinder1->Delete();
  cylinder1Mapper->Delete();
  cylinder1Actor->Delete();
  cylinder2->Delete();
  cylinder2Mapper->Delete();
  cylinder2Actor->Delete();
  cylinder3->Delete();
  cylinder3Mapper->Delete();
  cylinder3Actor->Delete();
  ren1->Delete();
  renWin->Delete();
  iren->Delete();

  return 0;
}
