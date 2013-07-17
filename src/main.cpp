//
//  main.cpp
//  GVFSNAKE
//
//  Created by Di Yang on 13/03/12.
//  Copyright (c) 2012 The Australian National University. All rights reserved.
//

#include <iostream>
#include <string.h>
#include <fstream>
#include "gvfc.h"
#include "common.h"
#include "drlse_edge.h"

IplImage *img0 = 0 ;       //color image
IplImage *img0_c = 0 ;     //copied color image
IplImage *res = 0;         //result

IplImage *img1 = 0 ;       //gray-scale image
IplImage *mask = 0;
CvPoint prev_pt = {-1,-1};

using namespace std;
int Thresholdness = 141;

void on_mouse( int event, int x, int y, int flags, void* param )
{
    if( !img0 )
        return;
    
    if( event == CV_EVENT_LBUTTONUP || !(flags & CV_EVENT_FLAG_LBUTTON) )
        prev_pt = cvPoint(-1,-1);
    else if( event == CV_EVENT_LBUTTONDOWN )
        prev_pt = cvPoint(x,y);
    else if( event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) )
    {
        CvPoint pt = cvPoint(x,y);
        if( prev_pt.x < 0 )
            prev_pt = pt;
        cvLine( mask, prev_pt, pt, cvScalarAll(255), 2, 8, 0 );
        cvLine( img0, prev_pt, pt, cvScalar(0, 0, 255), 2, 8, 0 );
        prev_pt = pt;
        cvShowImage( "Input Image", img0 );
    }
}
    
void bind_line(){
  cout<<"     *------------------------------------------------------------------*"<<endl;
}

void info_key0(int option){
  bind_line();
  cout<<"     | Hot keys:                                                        |"<<endl;
  cout<<"     | Press 'ESC' - exit the program                                   |"<<endl;
  switch (option){
  case 1:
    cout<<"     | Press 'w' or 'ENTER' - run GVFsnake (with loaded intial-contour) |"<<endl;
    break;
  default:
    cout<<"     | Press 'w' or 'ENTER' - run DRLSE (with loaded intial-contour)    |"<<endl;
    break;

  }
  bind_line();
}
void info_key1(int option){
  bind_line();
  cout<<"     | Hot keys:                                                        |"<<endl;
  cout<<"     | Press 'ESC' - exit the program                                   |"<<endl;
  cout<<"     | No mask is found                                                 |"<<endl;
  switch (option){
  case 1:
    cout<<"     | Press 'w' or 'ENTER' - run GVFsnake (with default intial-contour)|"<<endl;
    break;
  default:
    cout<<"     | Press 'w' or 'ENTER' - run DRLSE (intial-contour required)       |"<<endl;
    break;
  }
  bind_line();
}  
void info_key2(int option){
  switch (option){
  case 1:
    cout<<"     | (Otherwise, before running, roughly mark the areas on the image) |"<<endl;
    break;
  default:
    cout<<"     | (Before running, roughly mark the areas on the image)            |"<<endl;
    break;
  }
  cout<<"     | Press 'r' - resort the original image                            |"<<endl;
  bind_line();       
}

int main(int argc, char* argv[])
{
    CvMat* mask2;
    CvSize size;
    CvPoint* point = NULL;
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours = 0;
    int length = 14, alg_option = 1, timestep=5;
    float alpha=0.05f, beta=0.1f, gamma=1.0f, kappa=2.0f, flag = 0.0f, t;
    double lambda=5.0f, epsilon=1.5f, alfa=1.5f;
    bool IS_MASK = false;
    
    if(argc < 3){
      cout<<"WARNING: Please locate the input image"<<endl;
      return 0;
    }
    if (strcmp(argv[1], "-1") == 0)
      alg_option = 1;
    else if(strcmp(argv[1], "-2") == 0)
      alg_option = 2;
    else if(strcmp(argv[1], "-3")==0)
      alg_option = 3;
    else{
      cout<<"WARNING: Please choose one of following two active contour methods: "<<endl;
      cout<<"-1: Gradient Vector Field Snake"<<endl;
      cout<<"-2: Distance Regularized Level Set Evolution (Expand curve)"<<endl;
      cout<<"-3: Distance Regularized Level Set Evolution (Shrink curve)"<<endl;
    }

    if(!(img0 = cvLoadImage(argv[2],1))){
      cout<<"ERROR: No Input Image Found"<<endl;
      return 0;
    }  
    
    size = cvGetSize(img0);
    img0_c = cvCreateImage(size, IPL_DEPTH_8U, 3);
    res = cvCreateImage(size, IPL_DEPTH_8U, 3);
    img1 = cvCreateImage(size, IPL_DEPTH_8U, 1);
    mask  = cvCreateImage(size, IPL_DEPTH_8U, 1);
    mask2 = cvCreateMat(size.height, size.width, CV_32FC1);
    
    cvCvtColor( img0, img1, CV_RGB2GRAY );
    cvCopy(img0, img0_c);
    cvCopy(img0, res);
    cvSetZero( mask );
    
    cvNamedWindow("Input Image",0); cvMoveWindow("Input Image", 0, 0);
    cvNamedWindow("Result",0); cvMoveWindow("Result", size.width*1.5, 0);
    cvShowImage("Input Image", img0);
    cvShowImage("Result", res);
    
    if(argc == 4){
      if(mask = cvLoadImage(argv[3],0)){
        IS_MASK = true;
        system("clear");
        info_key0(alg_option);
      }
    }
    else{
      system("clear");
      info_key1(alg_option);
      info_key2(alg_option);
    }
    
    cvSetMouseCallback( "Input Image", on_mouse, 0 );
    while(true)
    {
        char c = cvWaitKey(0);
        if( c == 27 ) break;
        if( c == 'r' )
        {
            cvZero( mask );
            cvCopy( img0_c, img0 );
            cvCopy( img0_c, res );
            cvShowImage( "Input Image", img0 );
        }
        
        if( c == 'w' || c == '\n' )
        {
            cvConvertScale(mask, mask2, 1, 0);
            /*
             Three different ways to initalize active contour:
             1. roughly mark initial-contour in the image;
             2. load pre-define initial-contour from a txt-file;
             3. system default initial-contour 
             */

            if((flag = cvFindOpElem(mask2, CV_MAX_ELEM))>0.0f){	
                CvMemStorage* storage = cvCreateMemStorage(0);
                CvSeq* contours = 0;
                cvFindContours( mask, storage, &contours, sizeof(CvContour), 
                       CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE );
                if(!contours) return 0; 
                length = contours->total;	
                if(length<10) return 0; 
                point = new CvPoint[length]; 
                CvSeqReader reader;
                CvPoint pt= cvPoint(0,0);;	
                CvSeq *contour2=contours;	
                cvStartReadSeq(contour2, &reader);
                for (int i = 0; i < length; i++)
                {
                    CV_READ_SEQ_ELEM(pt, reader);
                    point[i]=pt;
                }
                cvReleaseMemStorage(&storage);
                
                if (IS_MASK) {
                    for (int i=0; i<length; i++){
                        int j = (i+1)%length;
                        cvLine(img0, point[i], point[j], CV_RGB(255, 0, 0), 2, 8, 0);
                    }
                    cvShowImage("Input Image", img0);
                }
            }
            else {
                if (alg_option == 1) {
                    float t = 0.0f;
                    point = new CvPoint[length];
                    for (int i = 0; i < length; i++) {
                        point[i].x = int(float(size.width>>1) 
                        +float(MIN(size.width, size.height)>>2)*sinf(t));
                        point[i].y = int(float(size.height>>1) 
                        +float(MIN(size.width, size.height)>>2)*cosf(t));
                        if (i == length-1){
                          point[i].x = point[0].x;
                          point[i].y = point[0].y;
                        }
                        t +=0.5f;
                    }
                    for(int i=0;i<length;i++){
                        int j = (i+1)%length;
                        cvLine( img0, point[i],point[j],CV_RGB( 255, 0, 0 ),2,8,0 );
                    }
                    cvShowImage("Input Image", img0);
                }
                else{
                  cout<<"WARNING: before running, roughly mark the areas on the image"<<endl;
                  continue;
                }
            }

            t = (float)cvGetTickCount();
            if (alg_option == 1)
                point=cvSnakeImageGVF( img1, point, &length, alpha, beta, gamma, kappa, 50, 10, CV_REINITIAL, CV_GVF);
	    /* 
	    #1 Reinitialization:
	     CV_REINITIAL:  Reinitialization of contour will be conducted, significantly increase accuracy.
	     CV_NREINITIAL: Reinitialization of contour will not be conducted
	    
	    #2 Energy Field:
	     CV_GVF: The function will calculate Gradient Vector Flow 
	     CV_GRD: Gradient Magnitude for every image pixel
	     CV_IMG: The input image itself is considered 
	     */
            else if(alg_option == 2)
                point= cvDRLSE(img1, mask, &length, lambda, alfa, epsilon, timestep, 200, 5, CV_LSE_EXP);
	    /*
	      Evolution direction:
	       CV_LSE_SHR: Contour will shrink
	       CV_LSE_EXP: Contour will expand
	     */
            else
                point= cvDRLSE(img1, mask, &length, lambda, alfa, epsilon, timestep, 200, 5, CV_LSE_SHR);
    
            t = (float)cvGetTickCount() - t;
            if (!point){
                cout<<"Warning: Make sure initial-contour is closed"<<endl;
                cout<<"Press 'r' to resort the original image, then try again"<<endl;
                continue;
            }
            cout<<"exec time = "<<t/(cvGetTickFrequency()*1e6)<<endl;
            for(int i=0;i<length;i++)
            {
              int j = (i+1)%length;
              cvLine( res, point[i],point[j],CV_RGB( 0, 0, 255 ),2,8,0 );
            }
            cvShowImage("Result",res);

	    string str = argv[2];
	    string _str0 = "_ini";
	    string _str1 = "_res";
	    string _ext = ".png";
	 
	    _str0.insert(0, str, 0, str.length()-4);
	    _str0.insert(_str0.length(), _ext);
	 
	    _str1.insert(0, str, 0, str.length()-4);
	    _str1.insert(_str1.length(), _ext);

            cvSaveImage((char*)_str0.c_str(), img0);
            cvSaveImage((char*)_str1.c_str(), res);
            delete []point;
        }
    }
    cvReleaseImage(&img0_c);
    cvReleaseImage(&img0);
    cvReleaseImage(&img1);
    cvReleaseImage(&mask);
    cvReleaseImage(&res);
    cvReleaseMat(&mask2);
}
