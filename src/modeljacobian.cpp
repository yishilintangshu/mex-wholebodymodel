/*
 * Copyright (C) 2014 Robotics, Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
 * Authors: Naveen Kuppuswamy
 * email: naveen.kuppuswamy@iit.it
 *
 * The development of this software was supported by the FP7 EU projects
 * CoDyCo (No. 600716 ICT 2011.2.1 Cognitive Systems and Robotics (b))
 * http://www.codyco.eu
 *
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
 */

//global includes

//library includes
#include <wbi/iWholeBodyModel.h>
#include <wbiIcub/icubWholeBodyModel.h>
//local includes

#include "modeljacobian.h"
using namespace mexWBIComponent;

ModelJacobian * ModelJacobian::modelJacobian; 

ModelJacobian::ModelJacobian(wbi::iWholeBodyModel *m): ModelComponent(m,2,1)
{
#ifdef DEBUG
  mexPrintf("ModelJacobian constructed \n");
#endif
}

ModelJacobian::~ModelJacobian()
{

}

bool ModelJacobian::allocateReturnSpace(int nlhs, mxArray* plhs[])
{
#ifdef DEBUG
  mexPrintf("Trying to allocateReturnSpace in ModelMassMatrix\n");
#endif
  
  bool returnVal = false;

  plhs[0]=mxCreateDoubleMatrix(6,numDof+6, mxREAL);
  j = mxGetPr(plhs[0]);
  returnVal = true;
  return(returnVal);
}

ModelJacobian * ModelJacobian::getInstance(wbi::iWholeBodyModel *m) 
{
  if(modelJacobian == NULL)
  {
    modelJacobian = new ModelJacobian(m);
  }
  return(modelJacobian);
}
/*

bool ModelJacobian::display(int nrhs, const mxArray * prhs[])
{
#ifdef DEBUG
  mexPrintf("Trying to display ModelMassMatrix \n");
#endif
  bool processRet = processArguments(nrhs,prhs);
  //processArguments(nrhs,prhs);
  double *mm = new double((6) *(numDof));
  
  //robotModel->computeMassMatrix(modelState->q(),modelState->baseFrame(),mm);
#ifdef DEBUG
  mexPrintf("Trying to display ModelMassMatrix : call from wbi returned\n");
#endif
  double qState[numDof];
  for( int i = 0; i<6; i++)
  {
    for(int k = 0; k<numDof+6; k++)
    {
	mm[i+k*numDof] = j[i+k*numDof];
	//mm[i][j] = 0.5 + 0.1*i*j;
#ifdef DEBUG
	mexPrintf("%f ",mm[i+k*numDof]);
#endif
	
      //mexPrintf("%f ",massMatrix[i+(j*numDof)]);
    }
#ifdef DEBUG
    mexPrintf("\n ");
#endif
    
  }
  
  delete(mm);
    //robotModel->computeMassMatrix()
  return(true);
}*/

bool ModelJacobian::compute(int nrhs, const mxArray * prhs[])
{
#ifdef DEBUG
  mexPrintf("Trying to compute ModelJacobian \n");
#endif
  processArguments(nrhs,prhs);
#ifdef DEBUG
  mexPrintf("ModelJacobian computed\n");
#endif
  return(true);
}



bool ModelJacobian::processArguments(int nrhs, const mxArray * prhs[])
{
//   if(nrhs<3)
//   {
//      mexErrMsgIdAndTxt( "MATLAB:mexatexit:invalidNumInputs","Atleast three input arguments required for ModelJacobian");
//   }
  
  if(mxGetM(prhs[1]) != numDof || mxGetN(prhs[1]) != 1 || !mxIsChar(prhs[2]))
  {
     mexErrMsgIdAndTxt( "MATLAB:mexatexit:invalidNumInputs","Malformed state dimensions/components");
  }
    
  q = mxGetPr(prhs[1]);
  refLink = mxArrayToString(prhs[2]);
#ifdef DEBUG
  mexPrintf("q received \n");

  for(int i = 0; i< numDof;i++)
  {
    mexPrintf(" %f",q[i]);
  }
#endif  
  
  robotModel->computeH(q,wbi::Frame(),ROBOT_BASE_FRAME_LINK, H_base_wrfLink);
  
  H_base_wrfLink.setToInverse().get4x4Matrix (H_w2b.data());
  xB.set4x4Matrix (H_w2b.data());
  
  if(j != NULL)
  {
    int refLinkID;
    robotModel->getLinkId (refLink, refLinkID);
     //robotModel->computeMassMatrix(q,xB,massMatrix);
    if(!(robotModel->computeJacobian(q,xB,refLinkID,j)))
    {
      mexErrMsgIdAndTxt( "MATLAB:mexatexit:invalidInputs","Something failed in the jacobian call");
    }
  }
//   mxFree(q);
  return(true);  
}



















