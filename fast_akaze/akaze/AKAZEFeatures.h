/**
 * @file AKAZE.h
 * @brief Main class for detecting and computing binary descriptors in an
 * accelerated nonlinear scale space
 * @date Mar 27, 2013
 * @author Pablo F. Alcantarilla, Jesus Nuevo
 */

#ifndef __OPENCV_FEATURES_2D_AKAZE_FEATURES_H__
#define __OPENCV_FEATURES_2D_AKAZE_FEATURES_H__

/* ************************************************************************* */
// Includes
#include <vector>

#include <opencv2/core.hpp>

#include "AKAZEConfig.h"
#include "TEvolution.h"

namespace cv
{

/* ************************************************************************* */
// AKAZE Class Declaration
class AKAZEFeaturesV2 {

private:

  AKAZEOptionsV2 options_;                ///< Configuration options for AKAZE
  std::vector<TEvolutionV2> evolution_;        ///< Vector of nonlinear diffusion evolution

  /// FED parameters
  bool reordering_;              ///< Flag for reordering time steps
  std::vector<std::vector<float > > tsteps_;  ///< Vector of FED dynamic time steps
  std::vector<int> nsteps_;      ///< Vector of number of steps per cycle

  /// Matrices for the M-LDB descriptor computation
  cv::Mat descriptorSamples_;  // List of positions in the grids to sample LDB bits from.
  cv::Mat descriptorBits_;
  cv::Mat bitMask_;

  /// Preallocated temporary variables
  cv::Mat lflow_, lstep_;
  std::vector<cv::KeyPoint> kpts_aux_;

public:

  /// Constructor with input arguments
  AKAZEFeaturesV2(const AKAZEOptionsV2& options);

  /// Getters and Setters
  void setThreshold(double threshold_) { options_.dthreshold = (float)threshold_; };
  double getThreshold() const { return options_.dthreshold; }
  void setDiffusivity(int diff_) { options_.diffusivity = diff_; }
  int getDiffusivity() const { return options_.diffusivity; }

  /// Scale Space methods
  void Allocate_Memory_Evolution();
  int Create_Nonlinear_Scale_Space(const cv::Mat& img);
  void Feature_Detection(std::vector<cv::KeyPoint>& kpts);
  void Compute_Determinant_Hessian_Response(void);
  void Compute_Multiscale_Derivatives(void);
  void Find_Scale_Space_Extrema(std::vector<cv::KeyPoint>& kpts);
  void Do_Subpixel_Refinement(std::vector<cv::KeyPoint>& kpts);

  /// Feature description methods
  void Compute_Descriptors(std::vector<cv::KeyPoint>& kpts, cv::Mat& desc);
  static void Compute_Main_Orientation(cv::KeyPoint& kpt, const std::vector<TEvolutionV2>& evolution_);
};

/* ************************************************************************* */
/// Inline functions
void generateDescriptorSubsampleV2(cv::Mat& sampleList, cv::Mat& comparisons,
                                 int nbits, int pattern_size, int nchannels);

}

#endif
