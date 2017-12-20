# Script for preparing images and calibration data 
#   for Yasutaka Furukawa's PMVS system

BUNDLER_BIN_PATH= # Edit this line before running
if [ "$BUNDLER_BIN_PATH" == "" ] ; then echo Please edit prep_pmvs.sh to specify the path to the  bundler binaries.; exit; fi
# Apply radial undistortion to the images
$BUNDLER_BIN_PATH/RadialUndistort E:\result\hama\bundler_tmp\imageslist.txt E:\result\hama\bundler_output\bundle.out output

# Create directory structure
mkdir -p output/txt/
mkdir -p output/visualize/
mkdir -p output/models/

# Copy and rename files
mv pmvs/E:\result\hama\kermit001.rd.jpg output/visualize/00000000.jpg
mv output/00000000.txt output/txt/
mv pmvs/E:\result\hama\kermit002.rd.jpg output/visualize/00000001.jpg
mv output/00000001.txt output/txt/
mv pmvs/E:\result\hama\kermit003.rd.jpg output/visualize/00000002.jpg
mv output/00000002.txt output/txt/
mv pmvs/E:\result\hama\kermit004.rd.jpg output/visualize/00000003.jpg
mv output/00000003.txt output/txt/
mv pmvs/E:\result\hama\kermit005.rd.jpg output/visualize/00000004.jpg
mv output/00000004.txt output/txt/
mv pmvs/E:\result\hama\kermit006.rd.jpg output/visualize/00000005.jpg
mv output/00000005.txt output/txt/
mv pmvs/E:\result\hama\kermit007.rd.jpg output/visualize/00000006.jpg
mv output/00000006.txt output/txt/
mv pmvs/E:\result\hama\kermit008.rd.jpg output/visualize/00000007.jpg
mv output/00000007.txt output/txt/
mv pmvs/E:\result\hama\kermit009.rd.jpg output/visualize/00000008.jpg
mv output/00000008.txt output/txt/
mv pmvs/E:\result\hama\kermit010.rd.jpg output/visualize/00000009.jpg
mv output/00000009.txt output/txt/

echo "Running Bundle2Vis to generate vis.dat
"
$BUNDLER_BIN_PATH/Bundle2Vis output/bundle.rd.out output/vis.dat



echo @@ Sample command for running pmvs:
echo "   pmvs2 output/ pmvs_options.txt"
echo "    - or - "
echo "   use Dr. Yasutaka Furukawa's view clustering algorithm to generate a set of options files."
echo "       The clustering software is available at http://grail.cs.washington.edu/software/cmvs"
