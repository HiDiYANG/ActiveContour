#include <iostream>
#include "common.h"

void loadBar(int x, int n, int w)
{
  // Calculuate the ratio of complete-to-incomplete.
  float ratio = x/(float)n;
  int   c     = ratio * w;
 
  // Show the percentage complete.
  printf("Progress: %3d%% [", (int)(ratio*100));
 
  // Show the load bar.
  for (int x=0; x<c; x++)
    printf("=");
 
  for (int x=c; x<w; x++)
    printf(" ");
 
  // ANSI Control codes to go back to the
  // previous line and clear it.
  if (c==w)
    printf("]\n");
  else{
    printf("]\r");
    fflush(stdout);
  }
}

CV_IMPL void cvNeumannBoundCond(const CvArr * srcarr,
                                CvArr * dstarr)
{
  CV_FUNCNAME("cvNeumannBoundCond");
    
  __BEGIN__;
  CvMat sstub, *src;
  CvMat dstub, *dst;
  CvSize size;
  int i, j;
  float * ptr_src, * ptr_dst;
  int iStep_src, iStep_dst;
    
  CV_CALL( src = cvGetMat(srcarr, &sstub ));
  CV_CALL( dst = cvGetMat(dstarr, &dstub ));
    
  if( CV_MAT_TYPE(src->type) != CV_32FC1)
    CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel input images are supported" );
  if( CV_MAT_TYPE(dst->type) != CV_32FC1)
    CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel input images are supported" );
  if( !CV_ARE_SIZES_EQ(src, dst))
    CV_ERROR( CV_StsUnmatchedSizes, "The input images must have the same size" );
    
  size = cvGetMatSize( src );
  cvCopy(src, dst);
    
  ptr_src = src->data.fl;
  iStep_src = src->step / sizeof(ptr_src[0]);
  ptr_dst = dst->data.fl;
  iStep_dst = dst->step / sizeof(ptr_dst[0]);
    
  ptr_dst[0] = ptr_src[2+iStep_src*2];                                       
  //dst(0,0)=src(3,3)
  ptr_dst[size.width-1] = ptr_src[size.width-3+iStep_src*2];                 
  //dst(0,col-1)=src(3,col-3)
  ptr_dst[iStep_dst*(size.height-1)] = ptr_src[2+iStep_src*(size.height-3)];     
  //dst(row-1,0)=src(row-3,3)
  ptr_dst[size.width-1+iStep_dst*(size.height-1)] = ptr_src[size.width-3+iStep_dst*(size.height-3)]; 
  //dst(row-1,col-1)=src(row-3,col-3)
    
  for(i = 1; i < size.width-1; i++){
    ptr_dst[i] = ptr_src[i+iStep_src*2];
    ptr_dst[i+iStep_dst*(size.height-1)]=ptr_src[i+iStep_src*(size.height-3)];
  }
    
  for(j = 1; j < size.height-1; j++){
    ptr_dst[iStep_dst*j] = ptr_src[2+iStep_src*j];
    ptr_dst[size.width-1+iStep_dst*j]=ptr_src[size.width-3+iStep_src*j];
  }
    
  __END__;
}
