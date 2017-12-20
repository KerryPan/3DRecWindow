INTRODUCTION
===============================================================================
TextureMesh is a program to generate textured mesh from a point cloud created by Bundler/PMVS.
It is designed to be used inconjunction with my RunSFM package.


COMPILING
===============================================================================
To compile you'll need the following libraries 
- OpenCV 2.x
- libANN (http://www.cs.umd.edu/~mount/ANN/)

You can compile by typing 'make' or by opening the project in CodeBlocks.

You'll also need Meshlab (http://meshlab.sourceforge.net) installed to manually
generate the mesh.


USAGE
===============================================================================
This section assumes the reader is already familiar with RunSFM and Meshlab.
If you've never used Meshlab that's okay, it's easy enough to use. Just follow 
my instructions.

RunSFM creates a 'pmvs' sub-directory. This directory contains all the information we need
to work with except for a 3D mesh. To create one we'll use Meshlab. 
Open pmvs/model/option-0000.ply in Meshlab. My general workflow for creating a mesh is as follows:

    1. [Filters] --> [Remeshing, Simplification, Reconstruction] --> [Surface reconstruction: Poisson]
    2. Set Octree Depth to 8, click Apply, then Close
    3. [Filters] --> [Selection] --> [Select Faces with edges longer than]
    4. Set the Edge Threshold to around 0.4. You should see the large errooneous faces highlighted in red. Click Close.
    5. Hit Shift + Delete, to delete the faces and vertices
    6. [Filters] --> [Remeshing, Simplification, Reconstruction] --> [Quadric Edge Collapse Decimation]
    7. Choose the desired 'Target number of faces'. I typically experiment with 1%-10% of the total faces.
    8. Tick 'Preserve Topology', tick 'Planar Simplification', click Apply, click Close
    9. [File] --> [Export Mesh As], save as 'mesh.ply'
    10. Untick 'Binary encoding', click OK

Feel free to experiment. I sometimes spend more time to clean out the stray floating faces in mid air.

We can now use TextureMesh to assign each face a texture mapping. Run TextureMesh by calling

$ ./TextureMesh [pmvs_dir]

where [pmvs_dir] points to the pmvs directory, which must contain the following files

- pmvs/visualize/00000000.jpg
- pmvs/bundle.rd.out
- pmvs/visualize/option-0000.patch
- pmvs/visualize/mesh.ply

TextureMesh will generate the following files:

- pmvs/models/output.mesh
- pmvs/models/output.point
- pmvs/models/output.obj <-- Wavefront OBJ
- pmvs/models/output.mtl <-- Wavefront OBJ

You can now view the mesh by calling ViewMesh as follows

$ ./ViewMesh [pmvs_dir]

Or use Meshlab to view output.obj.

LIMITATION
===============================================================================
Currently, TextureMesh doesn't support models that have been split by CMVS eg. option-0000.ply, option-0001.ply, option-0002.ply.


LICENSE
===============================================================================
Please see the LICENSE file. In summary, it's a BSD license, which means you can
pretty do much whatever you want with the code. For educational or commercial.


