//
//  drlse_edge.h
//  DRLSE
//
//  Created by Di Yang on 13/02/12.
//  Copyright (c) 2012 The Australian National University. All rights reserved.
//

#ifndef DRLSE_drlse_edge_h
#define DRLSE_drlse_edge_h
#include <string.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <fstream> 
#endif

#define CV_LSE_SHR 1
#define CV_LSE_EXP 0

CV_IMPL void cvDirac(const CvArr* srcarr,
                     CvArr* dstarr,
                     double sigma);

CV_IMPL void cvCalS(const CvArr* srcarr,
                    CvArr* dstarr);

CV_IMPL void cvCurvature(const CvArr* srcarr_x, 
                         const CvArr* srcarr_y,
                         CvArr* dstarr);

CV_IMPL void cvDistReg(const CvArr* srcarr,
                       CvArr* dstarr);

CV_IMPL void cvDrlse_edge(CvArr * srcphi, 
                          CvArr * srcgrad,
                          CvArr * dstarr,
                          double lambda, 
                          double mu, 
                          double alfa, 
                          double epsilon, 
                          int timestep,
                          int iter);

CvPoint*
cvDRLSE(const CvArr * mask,
        const CvArr * image,
        int *length,
        double lambda,
        double alfa,
        double epsilon,
        int timestep,
        int ITER_ext,
        int ITER_int,
        int flag);
