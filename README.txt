README FILE

Author:   DI YANG
Contact:  di.yang@anu.edu.au
Date:     13/03/2012
================================================================================

Active Contour Library consists of C/C++ codes developed for the implementation of two advanced active contour algorithms, such as 
1. Snake with Gradient Vector Flow (GVF snake)[1]
2. Distance Regularized Level Set Evolution (DRLSE)[2]

as well as a separate executable for demonstration purposes.

This document explains the contents of this directory, location of manuals and
provides installation instructions.


DIRECTORY CONTENTS
==================

This directory contains the following subdirectories.

#1 images
    This directory contains the sample data and sample image to test the software.
    P.S. output images would be stored in folder as well.

#2 src
    This directory contains the source code.
    -main.cpp (demonstration of active contour)
    -gvfc.cpp (Library contains dependent functions for GVFsnake)
    -drlse_edge.cpp (Library contains dependent functions for DRLSE)
    -common.cpp (Library contains dependent functions for both methods)
	
#3 headers
    This directory contains the corresponding header files

#3 examples
    DRLSE: folder contains some sample results of DRLSE.
    GVFsnake: folder contains some sample results of GVF snake.

#4 Makefile: to compile demonstration.

#5 The demonstration program needs to be compiled and installed in order to run the examples.


USER MANUAL
===========

#1 Colour image segmentation without predefined masks:
  To run GVF snake demonstration:
  ./active_contour_demo -1 test_images/<input image name>.png

  To run DRLSE demonstration:
  ./active_contour_demo -2 test_images/<input image name>.png
where '-1' and '-2' denotes the index of corresponding active contour algorithm.

#2 Colour image segmentation with predefined masks:
  To run GVF snake demonstration:
  ./active_contour_demo -1 test_images/<input image name>.png test_images/<input image name>_mask.png

  To run DRLSE demonstration:
  ./active_contour_demo -2 test_images/<input image name>.png test_images/<input image name>_mask.png

#3 The output names are specified, and stored in directory "images":     
  - "<input image name>_ini.png" the input image with initial-contour  
  - "<input image name>_res.png" the input image with contour of outline of object
  
#4 In the case of manual setting masks, "<input image name>_mask.png" presents initial contour.  


GUI
===

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

A detailed developer manual, which will be useful for developers who want to use
this code library for future work, is provided in doc/developerManual.pdf.
Please refer this document to understand the source code itself.

Functionality of active contour algorithms:

#1 GVF snake:
CvPoint* cvSnakeImageGVF(const CvArr* image, CvPoint* points, int *length, float alpha,
        float beta, float gamma, float kappa, int ITER_ext, int ITER_int, int calcInitail,
        int alg);

conduct GVF snake to evolve initial-contour in order to find object outline

Output: CvPoint type array contains coordinates information of all points on object outline.

Parameters: image  - The source image
	    points - The array contains coordinates of each point on the initial-contour
	    length - Return length of output array
  	    alpha  - GVF snake parameter 1 
	    beta   - GVF snake parameter 2
	    gamma  - GVF snake parameter 3
	    kappa  - GVF snake parameter 4
	    ITER_ext  - external iteration
 	    ITER_int  - internal iteration
	    calcInitail -
		    CV_REINITIAL: Re-initialization of contour will be conducted
		    (significantly increase accuracy, but increase execution time)
		    CV_NREINITIAL: Re-initialization of contour will not be conducted     
	    alg -   
		    3 different ways to generate energy field
		    CV_GVF: Gradient Vector Flow will be adopted as energy field
		    CV_GRD: Gradient Magnitude for every image pixel
			(make GVFsnake be equivalent to conventional Snake)			
		    CV_IMG: The input image itself is considered 

-------

#2 DRLSE
CvPoint* cvDRLSE(const CvArr * mask, const CvArr * image, int *length, double lambda,
        double alfa, double epsilon, int timestep, int ITER_ext, int ITER_int,
        int evolution = CV_LSE_OUT);

conduct DRLSE to evolve initial-contour in order to find object outline

Output: CvPoint type array contains coordinates information of all points on object outline.

Parameters: mask      - The mask image contains initial-contour
	    image     - The source image 
	    length    - Return length of output array
  	    lambda    - Level set parameter 1 
	    alfa      - Level set parameter 2
	    epsilon   - Level set parameter 3
	    time step - parameter to control evolving speed
	    ITER_ext  - external iteration
 	    ITER_int  - internal iteration
	    evolution -
			CV_LSE_OUT: initial contour will shrink
			CV_LSE_IN : initial contour will expand


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
	$ cd <edge_detection path>
	$ make

	
REFERENCE 
=========
[1] C. Xu and J. L. Prince, ``Snakes, Shapes, and Gradient Vector Flow,'' IEEE Transactions on Image Processing, 7(3), pp. 359-369, March 1998.

[2] C. Li, C. Xu, C. Gui, and M. D. Fox, "Distance Regularized Level Set Evolution and its Application to Image Segmentation", IEEE Trans. Image Processing, vol. 19 (12), 2010.
