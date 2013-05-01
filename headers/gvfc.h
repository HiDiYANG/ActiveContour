//
//  gvfc.h
//  gvfc
//
//  Created by Di Yang on 14/03/12.
//  Copyright (c) 2012 The Australian National University. All rights reserved.
//

#ifndef snaketest2_snaketest2_h
#define snaketest2_snaketest2_h
#include <opencv/cv.h>
#include <opencv/highgui.h>
#endif

#define CV_MAX_ELEM 1
#define CV_MIN_ELEM 0
#define CV_PT_RM -100
#define CV_WITH_HUN 0
#define CV_WITHOUT_HUN 1
#define CV_REINITIAL 1
#define CV_NREINITIAL 0
#define CV_GVF 3
#define CV_IMG 2
#define CV_GRD 1

CV_IMPL float cvFindOpElem(const CvArr* srcarr,
                           int flag);


CV_IMPL void cvGVF(const CvArr* srcarr,
                   CvArr* dstarr_u,
                   CvArr* dstarr_v,
                   double mu,
                   int ITER,
                   int flag);

CvPoint*
cvSnakeImageGVF(const CvArr* srcarr, 
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

static CvPoint*  
cvSnakeInterp(CvPoint* points,
              int * _length,
              int dmin,
              int dmax,
              int flag);
