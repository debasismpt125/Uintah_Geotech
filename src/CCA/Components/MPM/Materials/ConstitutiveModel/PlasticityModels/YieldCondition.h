/*
 * The MIT License
 *
 * Copyright (c) 1997-2023 The University of Utah
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef __YIELD_CONDITION_H__
#define __YIELD_CONDITION_H__

#include <Core/Math/Matrix3.h>
#include <Core/Math/TangentModulusTensor.h>
#include <Core/ProblemSpec/ProblemSpecP.h>
#include <Core/ProblemSpec/ProblemSpec.h>
#include <Core/Math/TangentModulusTensor.h>
#include <CCA/Components/MPM/Materials/ConstitutiveModel/PlasticityModels/PlasticityState.h>

namespace Uintah {

  /*! \class YieldCondition
   *  \brief A generic wrapper for various yield conditions
   *  \author Biswajit Banerjee, 
   *  \author C-SAFE and Department of Mechanical Engineering,
   *  \author University of Utah.
   *  \warning Mixing and matching yield conditions with damage and plasticity 
   *           models should be done with care.  No checks are provided to stop
   *           the user from using the wrong combination of models.
   *
   * Provides an abstract base class for various yield conditions used
   * in the plasticity and damage models
  */
  class YieldCondition {

  public:
         
    //! Construct a yield condition.  
    /*! This is an abstract base class. */
    YieldCondition();

    //! Destructor of yield condition.  
    /*! Virtual to ensure correct behavior */
    virtual ~YieldCondition();

    virtual void outputProblemSpec(ProblemSpecP& ps) = 0;
         
    /////////////////////////////////////////////////////////////////////////
    /*! 
      \brief Evaluate the yield function \f$(\Phi)\f$.

      If \f$\Phi \le 0\f$ the state is elastic.
      If \f$\Phi > 0\f$ the state is plastic and a normal return 
      mapping algorithm is necessary. 

      Returns the appropriate value of sig(t+delT) that is on
      the flow surface.
    */
    /////////////////////////////////////////////////////////////////////////
    virtual double evalYieldCondition(const double equivStress,
                                      const double flowStress,
                                      const double traceOfCauchyStress,
                                      const double porosity,
                                      double& sig) = 0;

    /////////////////////////////////////////////////////////////////////////
    /*! 
      \brief Evaluate the derivative of the yield function \f$(\Phi)\f$
      with respect to \f$\sigma_{ij}\f$.

      This is for the associated flow rule.
    */
    /////////////////////////////////////////////////////////////////////////
    virtual void evalDerivOfYieldFunction(const Matrix3& stress,
                                          const double flowStress,
                                          const double porosity,
                                          Matrix3& derivative) = 0;

    /////////////////////////////////////////////////////////////////////////
    /*! 
      \brief Evaluate the derivative of the yield function \f$(\Phi)\f$
      with respect to \f$s_{ij}\f$.

      This is for the associated flow rule with \f$s_{ij}\f$ being
      the deviatoric stress.
    */
    /////////////////////////////////////////////////////////////////////////
    virtual void evalDevDerivOfYieldFunction(const Matrix3& stress,
                                             const double flowStress,
                                             const double porosity,
                                             Matrix3& derivative) = 0;

    /////////////////////////////////////////////////////////////////////////
    /*! 
      \brief Compute the elastic-plastic tangent modulus.
    */
    /////////////////////////////////////////////////////////////////////////
    virtual void computeElasPlasTangentModulus(const TangentModulusTensor& Ce,
                                               const Matrix3& sigma, 
                                               double sigY,
                                               double dsigYdV,
                                               double porosity,
                                               double voidNuclFac,
                                               TangentModulusTensor& Cep) = 0;

    /*! Compute continuum elastic-plastic tangent modulus.
       df_dsigma = r */ 
    virtual void computeElasPlasTangentModulus(const Matrix3& r, 
                                               const Matrix3& df_ds, 
                                               const Matrix3& h_beta,
                                               const Matrix3& df_dbeta, 
                                               const double& h_alpha,             
                                               const double& df_dep,
                                               const double& h_phi,             
                                               const double& df_phi,
                                               const double& J,
                                               const double& dp_dJ,
                                               const PlasticityState* state,
                                               TangentModulusTensor& Cep);

  };
} // End namespace Uintah
      
#endif  // __YIELD_CONDITION_H__

