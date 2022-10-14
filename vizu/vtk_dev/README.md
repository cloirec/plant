Pour créer les fichiers de configuration de compilation
=========================================================

```
module load intel/17.2 intelmpi/2017.2.174 llvm/3.9.1 mesa/17.2.4 qt/4.8.6 VTK cmake/3.16.1
cmake .
```

Compilation
===========
```
module load intel/17.2 intelmpi/2017.2.174 llvm/3.9.1 mesa/17.2.4 qt/4.8.6 VTK cmake/3.16.1
make
```

Exécution 
=========

```
module load intel/17.2 intelmpi/2017.2.174 llvm/3.9.1 mesa/17.2.4 qt/4.8.6 VTK
./Branche
./My3Cylinder
./ReadJpegImage hetre.jpeg
./ResizeImage hetre.jpeg
./ConvertJPegToVtkPolyData hetre.jpeg hetre.vti
./ConvertJPegToVtkPolyDataMesh hetre.jpeg hetre.vtp
```

Visualisation
=============

```
module load intel/17.2 openmpi/intel/2.0.1 llvm mesa qt/4.8.6 python boost/1_63_0 paraview/5.4.0
paraview&
(open vti,vtp, ...etc files)
```
