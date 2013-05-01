//
//  gvfc.cpp
//  GVFsnake
//
//  Created by Di Yang on 9/03/12.
//  Copyright (c) 2012 The Australian National University. All rights reserved.
//

#include <iostream>
#include "gvfc.h"
#include "common.h"

static CvPoint*  
cvSnakeInterp2(CvPoint* points,
               int * _length,
               int dmax,
               int * _dist,
               int flag)
{
    int i, distance=0, cont=0;
    int length = *_length;
    int length_out = 2*length;
    int Max_d = 0;
    
    CvPoint* pt_temp = new CvPoint[length];
    int* index_z = new int[2*length];
    
    for (i=0; i<length; i++){
        if (flag == 1){
            pt_temp[i].x = points[i].x;
            pt_temp[i].y = points[i].y;
        }
        else{
            pt_temp[i].x = points[i].x*100;
            pt_temp[i].y = points[i].y*100;
        }
    }
    
    for (i=0; i<2*length; i++){
        index_z[i] = i;
    }
    
    for (i=0; i<length; i++){
        if (i == 0)
            distance = (abs(pt_temp[i].x-pt_temp[length-1].x)+abs(pt_temp[i].y-pt_temp[length-1].y))/100;
        else
            distance = (abs(pt_temp[i].x-pt_temp[i-1].x)+abs(pt_temp[i].y-pt_temp[i-1].y))/100;
        
        if (distance < dmax){
            index_z[2*i] = CV_PT_RM;
            length_out--;
        }
    }
    
    if(points != NULL){
        delete []points;
    }
    points = new CvPoint[length_out];
    int ind_prev=0;
    float a=0.0f;
    
    for (i=0; i<2*length; i++) {
        if (index_z[i] != CV_PT_RM){
            ind_prev = int(float(index_z[i])/2.0f);
            a = float(index_z[i])/2.0f - float(ind_prev);
            
            if(ind_prev == length-1){
                points[cont].x = int((1.0f-a)*float(pt_temp[ind_prev].x) + a*float(pt_temp[0].x));
                points[cont].y = int((1.0f-a)*float(pt_temp[ind_prev].y) + a*float(pt_temp[0].y));
            }
            else{
                points[cont].x = int((1.0f-a)*float(pt_temp[ind_prev].x) + a*float(pt_temp[ind_prev+1].x));
                points[cont].y = int((1.0f-a)*float(pt_temp[ind_prev].y) + a*float(pt_temp[ind_prev+1].y));
            }
            cont++;
        }
    }
    if (cont < length_out)
        printf("Error\n");
    
    for (i=0; i<length_out; i++){
        if (i == 0)
            distance = (abs(points[i].x-points[length_out-1].x)+abs(points[i].y-points[length_out-1].y))/100;
        else
            distance = (abs(points[i].x-points[i-1].x)+abs(points[i].y-points[i-1].y))/100;
        Max_d = MAX(Max_d, distance);
    }
    
    *_dist = Max_d;
    *_length = length_out;
    return points;
}

static CvPoint*  
cvSnakeInterp(CvPoint* points,
              int * _length,
              int dmin,
              int dmax,
              int flag)
{
    CV_FUNCNAME("cvSnakeInterp");
    
    __BEGIN__;
    int distance=0, cont=0, i;
    int length = * _length;
    int length_out = * _length;
    int Max_d = 0;
    
    for (i=0; i<length; i++){
        if (flag == 1){
            if (i == 0)
                distance = (abs(points[i].x-points[length-1].x)+abs(points[i].y-points[length-1].y))/100;
            else
                distance = (abs(points[i].x-points[i-1].x)+abs(points[i].y-points[i-1].y))/100;
        }    
        else{    
            if (i == 0)
                distance = abs(points[i].x-points[length-1].x)+abs(points[i].y-points[length-1].y);
            else
                distance = abs(points[i].x-points[i-1].x)+abs(points[i].y-points[i-1].y);
        }
        
        if (distance < dmin){
            points[i].x = CV_PT_RM;
            length_out--;
        }
    }
    assert( length_out > 0 );
    CvPoint* pt_temp = new CvPoint[length_out];
    for (i=0; i<length; i++){
        if (points[i].x != CV_PT_RM){
            pt_temp[cont] = points[i];
            cont++;
        }
    }
    if(points != NULL)
        delete []points;
    points = pt_temp;
    *_length = length_out;
    
    points = cvSnakeInterp2(points, _length, dmax, &Max_d, flag);
    
    do{
        points = cvSnakeInterp2(points, _length, dmax, &Max_d, 1);
    }while(Max_d > dmax);
    
    return points;
    
    __END__;
}

CV_IMPL float cvFindOpElem(const CvArr* srcarr,
                           int flag)
{
    CV_FUNCNAME("cvFindOpElem");
    
    __BEGIN__;
    CvMat sstub, *src;
    CvSize size;
    float dstElem, *ptr_src;
    int iStep_src;
    
    CV_CALL( src = cvGetMat(srcarr, &sstub ));
    size = cvGetMatSize( src );
    if( CV_MAT_TYPE(src->type) != CV_32FC1)
        CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel input images are supported" );
    ptr_src = src->data.fl;
    iStep_src = src->step / sizeof(ptr_src[0]);
    
    dstElem = ptr_src[0];
    for (int i=0; i<size.height; i++)
        for (int j=0; j<size.width; j++)
        {
            if (flag == CV_MAX_ELEM) {
                if (dstElem < ptr_src[j+iStep_src*i])
                    dstElem = ptr_src[j+iStep_src*i];
            } else {
                if (dstElem > ptr_src[j+iStep_src*i])
                    dstElem = ptr_src[j+iStep_src*i];
            }
        }
    return dstElem;
    __END__;
    
}

CV_IMPL void cvGVF(const CvArr* srcarr,
                   CvArr* dstarr_u,
                   CvArr* dstarr_v,
                   double mu,
                   int ITER,
                   int flag)
{
    CV_FUNCNAME("cvGVF");
    
    __BEGIN__;
    CvMat sstub, *src;
    CvMat dstubu, *dst_u;
    CvMat dstubv, *dst_v;
    CvMat* ones, * SqrMagf;
    CvMat* fx, * fy, *temp1, *temp2;
    CvMat* del_u, *del_v;
    CvSize size;
    float fmax, fmin;
    float* fPtr_fx, *fPtr_fy, *fPtr_src;
    int iStep_fx, iStep_fy, iStep_src;
    
    CV_CALL( src = cvGetMat(srcarr, &sstub ));
    CV_CALL( dst_u = cvGetMat(dstarr_u, &dstubu ));
    CV_CALL( dst_v = cvGetMat(dstarr_v, &dstubv ));
    size = cvGetSize(src);
    
    if( CV_MAT_TYPE(src->type) != CV_32FC1)
        CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel input images are supported" );
    if( CV_MAT_TYPE(dst_u->type) != CV_32FC1)
        CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel input images are supported" );
    if( CV_MAT_TYPE(dst_v->type) != CV_32FC1)
        CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel input images are supported" );
    if( !CV_ARE_SIZES_EQ(src, dst_u))
        CV_ERROR( CV_StsUnmatchedSizes, "The input and output matrixes must have the same size" );
    if( !CV_ARE_SIZES_EQ(src, dst_v))
        CV_ERROR( CV_StsUnmatchedSizes, "The input and output matrixes must have the same size" );
    temp1 = cvCreateMat(size.height, size.width, CV_32FC1);
    temp2 = cvCreateMat(size.height, size.width, CV_32FC1);
    ones  = cvCreateMat(size.height, size.width, CV_32FC1);
    fx    = cvCreateMat(size.height, size.width, CV_32FC1);
    fy    = cvCreateMat(size.height, size.width, CV_32FC1);
    del_u = cvCreateMat(size.height, size.width, CV_32FC1);
    del_v = cvCreateMat(size.height, size.width, CV_32FC1);
    SqrMagf = cvCreateMat(size.height, size.width, CV_32FC1);
    cvSet(ones, cvRealScalar(1.0f));
    cvSetZero(del_u);
    cvSetZero(del_v);
    
    fPtr_fx = fx->data.fl; 
    fPtr_fy = fy->data.fl; 
    fPtr_src = src->data.fl;
    iStep_fx = fx->step / sizeof(fPtr_fx[0]);
    iStep_fy = fy->step / sizeof(fPtr_fy[0]);
    iStep_src = src->step / sizeof(fPtr_src[0]);
    
    size = cvGetMatSize( src );
    cvNeumannBoundCond(src, src);
    
    if (flag == 1){
        fmax = cvFindOpElem(src, CV_MAX_ELEM);
        fmin = cvFindOpElem(src, CV_MIN_ELEM);
        cvSubS(src, cvRealScalar(fmin), src);
        cvMul(src, ones, src, 1.0f/(fmax-fmin));
        cvSobel(src, fx, 1, 0, 1);
        cvSobel(src, fy, 0, 1, 1);
        cvMul(fx, ones, fx, 0.5f);
        cvMul(fy, ones, fy, 0.5f);
    }
    else if (flag == 2){
        fmax = cvFindOpElem(src, CV_MAX_ELEM);
        fmin = cvFindOpElem(src, CV_MIN_ELEM);
        cvSubS(src, cvRealScalar(fmin), src);
        cvMul(src, ones, src, 1.0f/(fmax-fmin));
        cvSobel(src, fx, 1, 0, 1);
        cvSobel(src, fy, 0, 1, 1);
        cvMul(fx, ones, dst_u, 0.5f);
        cvMul(fy, ones, dst_v, 0.5f);
    }
    else if(flag == 3){
        cvSobel(src, fx, 1, 0, 1);
        cvSobel(src, fy, 0, 1, 1);
        for(int j=0; j<size.height; j++)
        {
            for(int i=0; i<size.width; i++)
            {
                fPtr_src[i+iStep_src*j]=sqrtf(fPtr_fx[i+iStep_fx*j]*fPtr_fx[i+iStep_fx*j]+fPtr_fy[i+iStep_fy*j]*fPtr_fy[i+iStep_fy*j]);
            }
        }
        fmax = cvFindOpElem(src, CV_MAX_ELEM);
        fmin = cvFindOpElem(src, CV_MIN_ELEM);
        cvSubS(src, cvRealScalar(fmin), src);
        cvMul(src, ones, src, 1.0f/(fmax-fmin));
        cvSobel(src, fx, 1, 0, 1);
        cvSobel(src, fy, 0, 1, 1);
        cvMul(fx, ones, fx, 0.5f);
        cvMul(fy, ones, fy, 0.5f);
        cvCopy(fx, dst_u);
        cvCopy(fy, dst_v);
        cvMul(fx, fx, temp1);
        cvMul(fy, fy, SqrMagf);
        cvAdd(SqrMagf, temp1, SqrMagf);
        for (int i=0; i<ITER; i++) {
            cvNeumannBoundCond(dst_u, dst_u);
            cvNeumannBoundCond(dst_v, dst_v);
            cvLaplace(dst_u, del_u, 1);
            cvLaplace(dst_v, del_v, 1);
            cvMul(del_u, ones, del_u, mu);
            cvMul(del_v, ones, del_v, mu);
            cvSub(dst_u, fx, temp1);
            cvSub(dst_v, fy, temp2);
            cvMul(temp1, SqrMagf, temp1);
            cvMul(temp2, SqrMagf, temp2);
            cvAdd(del_u, dst_u, dst_u);
            cvAdd(del_v, dst_v, dst_v);
            cvSub(dst_u, temp1, dst_u);
            cvSub(dst_v, temp2, dst_v);
        }
    }
    
    cvReleaseMat(&temp1);
    cvReleaseMat(&temp2);
    cvReleaseMat(&ones);
    cvReleaseMat(&fx);
    cvReleaseMat(&fy);
    cvReleaseMat(&del_u);
    cvReleaseMat(&del_v);
    cvReleaseMat(&SqrMagf);
    
    __END__;
}

static CvStatus
icvSnake32FC1_GVF( const CvArr *src_u,
                  const CvArr *src_v,
                  CvPoint * pt,
                  int *length,
                  float alpha,
                  float beta,
                  float gamma,
                  float kappa,
                  int   ITER_num,
                  int calcInitial)
{
    int i, j, n=*length;
    int iStep_A, iStep_v, iStep_u, iStep_VX, iStep_VY;
    float *ptr_A, *ptr_u, *ptr_v, *ptr_VX, *ptr_VY;
    int flag = calcInitial ? CV_REINITIAL : CV_NREINITIAL;
    CvMat* A, *VX, *VY;
    CvMat* u , sstub_u;
    CvMat* v , sstub_v; 
    CvSize roi;
    
    CV_FUNCNAME("icvSnake32FC1_GVF");
    __BEGIN__;
    
    CV_CALL( u = cvGetMat(src_u, &sstub_u ));
    CV_CALL( v = cvGetMat(src_v, &sstub_v ));
    roi = cvGetSize( u );
    A = cvCreateMat( n, n, CV_32FC1 );
    VX= cvCreateMat( n, 1, CV_32FC1);
    VY= cvCreateMat( n, 1, CV_32FC1);
    cvSetZero( A );
    cvSetZero( VX );
    cvSetZero( VY );
    
    ptr_VX= VX->data.fl;
    iStep_VX= VX->step / sizeof(ptr_VX[0]);
    ptr_VY= VY->data.fl;
    iStep_VY= VY->step / sizeof(ptr_VY[0]);
    ptr_u = u->data.fl;
    iStep_u = u->step / sizeof(ptr_u[0]);
    ptr_v = v->data.fl;
    iStep_v = v->step / sizeof(ptr_v[0]);
    ptr_A = A->data.fl;
    iStep_A = A->step / sizeof(ptr_A[0]);
    
    for (i=0; i<n; i++)
    {
        ptr_A[i+i*iStep_A] = 2*alpha + 6*beta + gamma;
        if ( i>0 )
            ptr_A[i-1+i*iStep_A] = ptr_A[i+(i-1)*iStep_A] = - ( alpha + 4*beta );
        if ( i>1 )
            ptr_A[i-2+i*iStep_A] = ptr_A[i+(i-2)*iStep_A] = beta;
        if ( i>n-3 )
            ptr_A[i-(n-2)+i*iStep_A] = ptr_A[i+(i-(n-2))*iStep_A] = beta;
        if ( i>n-2)
            ptr_A[i-(n-1)+i*iStep_A] = ptr_A[i+(i-(n-1))*iStep_A] = - ( alpha + 4*beta );
    }
    
    cvInvert( A, A, CV_LU );
    for( i = 0; i < n; i++ )
    {
        if(flag == CV_REINITIAL){
            ptr_VX[i*iStep_VX] = float( pt[i].x )/100.0f;
            ptr_VY[i*iStep_VY] = float( pt[i].y )/100.0f;
        }
        else{
            ptr_VX[i*iStep_VX] = float( pt[i].x );
            ptr_VY[i*iStep_VY] = float( pt[i].y );
        }
    }
    float interp_u, interp_v, a, b;
    int index_x, index_y;
    
    for (j = 0; j<ITER_num; j++)
    {
        for( i = 0; i < n; i++ )
        {   
            index_x = int(ptr_VX[i*iStep_VX]);
            index_y = int(ptr_VY[i*iStep_VY]);
            b = ptr_VX[i*iStep_VX]-float(index_x);
            a = ptr_VY[i*iStep_VY]-float(index_y);
            
            if (index_x < roi.width-1 && index_y < roi.height-1){
                interp_u = (1-b)*((1-a)*ptr_u[index_x+iStep_u*index_y]+a*ptr_u[index_x+iStep_u*(index_y+1)])+b*((1-a)*ptr_u[index_x+1+iStep_u*index_y]+a*ptr_u[index_x+1+iStep_u*(index_y+1)]);
                
                interp_v = (1-b)*((1-a)*ptr_v[index_x+iStep_v*index_y]+a*ptr_v[index_x+iStep_v*(index_y+1)])+b*((1-a)*ptr_v[index_x+1+iStep_v*index_y]+a*ptr_v[index_x+1+iStep_v*(index_y+1)]);
            }
            else{
                interp_u = ptr_u[index_x+1+iStep_u*index_y];
                interp_v = ptr_v[index_x+1+iStep_v*index_y];
            }
            
            ptr_VX[i*iStep_VX] = kappa * interp_u + gamma * ptr_VX[i*iStep_VX];
            ptr_VY[i*iStep_VY] = kappa * interp_v + gamma * ptr_VY[i*iStep_VY];
            
            if(ptr_VX[i*iStep_VX] < 0)
                ptr_VX[i*iStep_VX]=0;
            if(ptr_VX[i*iStep_VX] > roi.width-1)
                ptr_VX[i*iStep_VX] = roi.width-1;
            
            if(ptr_VY[i*iStep_VY] < 0)
                ptr_VY[i*iStep_VY]=0;
            if(ptr_VY[i*iStep_VY] > roi.height-1)
                ptr_VY[i*iStep_VY] = roi.height-1;
        }
        cvMatMul(A, VX, VX);
        cvMatMul(A, VY, VY);
    }
    for( i = 0; i < n; i++ )
    {
        if(flag == CV_REINITIAL){
            pt[i].x = int(ptr_VX[i*iStep_VX]*100.0f);
            pt[i].y = int(ptr_VY[i*iStep_VY]*100.0f);;
        }
        else{
            pt[i].x = int(ptr_VX[i*iStep_VX]);
            pt[i].y = int(ptr_VY[i*iStep_VY]);
        }
    }
    cvReleaseMat(&A);
    cvReleaseMat(&VX);
    cvReleaseMat(&VY);
    return CV_OK;
    
    __END__;
}

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
                int calcInitial,
                int alg)
{
    CvMat* src, *data, * u, *v, sstub;
    CvSize size;
    float mu=0.2f;
    int flag = calcInitial ? CV_REINITIAL : CV_NREINITIAL;
    CV_FUNCNAME("cvSnakeImageGVF");    
    
    __BEGIN__;
    
    CV_CALL( src = cvGetMat(srcarr, &sstub ));
    size = cvGetSize( src );
    
    // convert Iplimage format to CvMat with CV_32FC1 
    data = cvCreateMat(size.height, size.width, CV_32FC1);
    cvConvertScale(src, data, 1, 0 );
    u = cvCreateMat( size.height, size.width, CV_32FC1);
    v = cvCreateMat( size.height, size.width, CV_32FC1);
    
    // Compute Gradient Vector Flow
    cvGVF(data, u, v, mu, 80, alg);
    
    // Apply interpolation to initial curve points, in order to make them dense
    if(flag == CV_REINITIAL){
        points = cvSnakeInterp(points, length, 1, 100, CV_WITH_HUN);
    }
    
    for(int i=0; i<ITER_ext; i++)
    {
        icvSnake32FC1_GVF( u, v, points, length, alpha, beta, gamma, kappa, ITER_int, flag);
	if(flag == CV_REINITIAL){
	  points = cvSnakeInterp(points, length, 1, 1, CV_WITHOUT_HUN);
        }
	loadBar(i+1, ITER_ext, 50);
    }
    
    for (int i=0; i<*length; i++) 
    {
        if(flag == CV_REINITIAL){
            points[i].x = points[i].x/100;
            points[i].y = points[i].y/100;
        }
        else{
            points[i].x = points[i].x;
            points[i].y = points[i].y;
        }
    }
    return points;
    __END__;
}

/* end of file */
