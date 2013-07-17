README FILE

Author:   DI YANG
Contact:  di.yang@anu.edu.au
Date:     13/03/2012
================================================================================

Active Contour Library consists of C/C++ codes developed implementation and demonstration of two advanced active contour algorithms: 
1. Snake with Gradient Vector Flow (GVF snake)[1]
2. Distance Regularised Level Set Evolution (DRLSE)[2]

This document explains the contents of this directory and provides installation instructions as well as user and developer manuals.


DIRECTORY CONTENTS
==================

This directory contains the following subdirectories.

#1 images
    This directory contains the sample image to test this program.
    P.S. output images would be stored in this directory as well.

#2 src
    This directory contains the source code.
    - main.cpp (Demonstration of active contour)
    - gvfc.cpp (Library contains dependent functions for GVFsnake)
    - drlse_edge.cpp (Library contains dependent functions for DRLSE)
    - common.cpp (Library contains dependent functions for both methods)
	
#3 headers
    This directory contains the corresponding header files

#3 examples
    This directory contains sample results for both active contour methods.
    DRLSE: this folder contains some sample results of DRLSE.
    GVFsnake: this folder contains some sample results of GVF snake.

#4 Makefile: to compile demonstration.

#5 The demonstration program needs to be compiled and installed in order to run the examples.


INSTALLATION INSTRUCTIONS FOR UBUNTU
====================================

This program was tested on Linux using Ubuntu 12.04 (the latest release as of writing
this document) after installing all updates on 20/11/2012. Ubuntu was obtained
from www.ubuntu.com which provides a free download.

This program was also tested on MAC OS X 10.8 Mountain Lion.

To install, the Active Contour directory needs to be copied over to your hard drive. The rest of this document assumes that the directory along with the subdirectories inside it have been copied over to your hard drive. We refer to the location on your hard drive as
<active contour path> throughout the remainder of this document. 

A working internet connection is required to install some of the dependent
libraries mentioned below. 

Lines beginning with the $ sign indicate commands that need to be typed and
entered in the command line in a terminal. The $ sign should not be typed. These
commands can be directly copy pasted. A terminal can be opened in Ubuntu by
pressing 'CTRL+ALT+t'.

Proceed as follows to install the dependent libraries and to install active contour library.

  1) Install the C/C++ build system.
	$ sudo apt-get install build-essential
  
  2) Install the OpenCV library.
	$ sudo apt-get install libcv2.3 libcv-dev libcvaux-dev libcvaux2.3	
	$ sudo apt-get install libhighgui-dev libhighgui2.3 opencv-doc

  3) Now that all dependant libraries have been installed we can proceed to
     build the active contour source code as follows.
	$ cd <active contour path>
	$ make


USER MANUAL
===========
Quick run:

#1 Contour detection in colour image without predefined masks:
  To run GVF snake demonstration:
  ./active_contour_demo -1 images/<input image name>.png

  To run DRLSE demonstration (Expand initial curve from inside of object):
  ./active_contour_demo -2 images/<input image name>.png

  To run DRLSE demonstration (Shrink initial curve from outside of object):
  ./active_contour_demo -3 images/<input image name>.png
where '-1', '-2' and '-3' denotes the index of corresponding active contour algorithm.

#2 Contour detection in colour image with predefined masks:
  To run GVF snake demonstration:
  ./active_contour_demo -1 test_images/<input image name>.png test_images/<input image name>_mask.png

  To run DRLSE demonstration (Expand initial curve from inside of object):
  ./active_contour_demo -2 test_images/<input image name>.png test_images/<input image name>_mask_expand.png
 
  To run DRLSE demonstration (Shrink initial curve from outside of object):
  ./active_contour_demo -3 test_images/<input image name>.png test_images/<input image name>_mask_shrink.png

#3 The output names are specified, and stored in directory "images":     
  - "<input image name>_ini.png" displays the initial-contour in the input image 
  - "<input image name>_res.png" displays detected contour of object in the input image
  
#4 In the case of manual setting masks, "<input image name>_mask.png" presents initial contour.  


GUIDE USER INTERFACE
====================

#1 GUI for GVFsnake without pre-defined mask.
     *------------------------------------------------------------------*
     | Hot keys:                                                        |
     | Press 'ESC' - exit the program                                   |
     | No mask is found                                                 |
     | Press 'w' or 'ENTER' - run GVFsnake (with default intial-contour)|
     *------------------------------------------------------------------*
     | (Otherwise, before running, roughly mark the areas on the image) |
     | Press 'r' - resort the original image                            |
     *------------------------------------------------------------------*

#2 GUI for GVF with pre-defined mask.
     *------------------------------------------------------------------*
     | Hot keys:                                                        |
     | Press 'ESC' - exit the program                                   |
     | Press 'w' or 'ENTER' - run GVFsnake (with loaded intial-contour) |
     *------------------------------------------------------------------*

#3 GUI for DRLSE without pre-defined mask
     *------------------------------------------------------------------*
     | Hot keys:                                                        |
     | Press 'ESC' - exit the program                                   |
     | No mask is found                                                 |
     | Press 'w' or 'ENTER' - run DRLSE (intial-contour required)       |
     *------------------------------------------------------------------*
     | (Before running, roughly mark the areas on the image)            |
     | Press 'r' - resort the original image                            |
     *------------------------------------------------------------------*

#4 GUI for DRLSE with pre-defined mask
     *------------------------------------------------------------------*
     | Hot keys:                                                        |
     | Press 'ESC' - exit the program                                   |
     | Press 'w' or 'ENTER' - run DRLSE (with loaded intial-contour)    |
     *------------------------------------------------------------------*


DEVELOPER MANUAL
================

Functionality of active contour library is addressed as follows:

#1 GVF snake:
conduct GVF snake to evolve initial-contour in order to detect object outline

CvPoint* cvSnakeImageGVF(const CvArr* image, 
			 CvPoint* points, 
                         int *length, 
                         float alpha,
                         float beta, 
                         float gamma, 
                         float kappa, 
                         int ITER_ext, 
                         int ITER_int, 
                         int calcInitail,
                         int alg);

Output: a CvPoint type array contains coordinates of all points on detected contour.

Parameters:
	image  	- The source image
	points 	- The array contains coordinates of all points on initial-contour
	length 	- Return length of the output CvPoint array
	alpha  	- GVF snake parameter 1 
	beta   	- GVF snake parameter 2
	gamma  	- GVF snake parameter 3
	kappa  	- GVF snake parameter 4
	ITER_ext- external iteration
	ITER_int- internal iteration
	calcInitail -
		CV_REINITIAL: Re-initialisation of contour will be conducted
		Note: Re-initialisation may significantly increase accuracy, but would increase execution time simultaneously.
		CV_NREINITIAL: Re-initialisation of contour will not be conducted     
	alg -   
		three different ways to generate energy field
		 CV_GVF: Gradient Vector Flow will be adopted as energy field
		 CV_GRD: Gradient Magnitude for every image pixel
		 Note: CV_GRD will make GVFsnake be equivalent to conventional Snake
		 CV_IMG: The input image itself is considered 

-------

#2 DRLSE
conduct DRLSE to evolve initial-contour in order to detect object outline

CvPoint* cvDRLSE(const CvArr * mask, 
                 const CvArr * image, 
                 int *length, 
                 double lambda,
        	  double alfa, 
                 double epsilon, 
                 int timestep, 
                 int ITER_ext, 
                 int ITER_int,
                 int evolution);

Output: a CvPoint type array contains coordinates information of all points on detected contour.

Parameters: 
	mask 	- The mask image contains initial-contour
	image	- The source image 
	length  - Return length of output array
	lambda  - Level set parameter 1 
	alfa    - Level set parameter 2
	epsilon - Level set parameter 3
	time step - parameter to control evolving speed
	ITER_ext  - external iteration
	ITER_int  - internal iteration
	evolution-
		CV_LSE_OUT: initial contour will shrink
		CV_LSE_IN : initial contour will expand

	
REFERENCE 
=========
[1] C. Xu and J. L. Prince, "Snakes, Shapes, and Gradient Vector Flow,'' IEEE Transactions on Image Processing, 7(3), pp. 359-369, March 1998.

[2] C. Li, C. Xu, C. Gui, and M. D. Fox, "Distance Regularized Level Set Evolution and its Application to Image Segmentation", IEEE Trans. Image Processing, vol. 19 (12), 2010.
