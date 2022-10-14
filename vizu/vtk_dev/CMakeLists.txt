cmake_minimum_required(VERSION 3.8...3.12 FATAL_ERROR)


PROJECT (MyFiles)

find_package(VTK REQUIRED)
vtk_module_config(VTK
  vtkCommonCore
  vtkFiltersSources
  vtkInteractionStyle
  vtkRenderingOpenGL2
  vtkInteractionWidgets
  vtkIOImage
  vtkInteractionImage
  vtkInteractionStyle
  vtkRenderingContextOpenGL2
  vtkRenderingCore
  vtkRenderingFreeType
  vtkCommonDataModel
  vtkFiltersGeometry
  vtkIOXML
  vtkImagingMorphological
  vtkFiltersTexture
)
include(${VTK_USE_FILE})

set(RENDERING_EXAMPLES_SRCS
  My3Cylinder
  Branche
  ReadJpegImage
  ResizeImage
  ConvertJPegToVtkPolyData
  TransformJpeg
  ConvertJPegToVtkPolyDataMesh
  )

foreach(name ${RENDERING_EXAMPLES_SRCS})
  add_executable(${name} MACOSX_BUNDLE ${name}.cxx)
  target_link_libraries(${name} ${VTK_LIBRARIES} )
endforeach()
