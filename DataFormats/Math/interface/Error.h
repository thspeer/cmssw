#ifndef TrackReco_Error_h
#define TrackReco_Error_h
//
// Symmetric matrix
//
#include "Rtypes.h"

#include "FWCore/Utilities/interface/GCC11Compatibility.h"

#ifdef CMS_NOCXX11
#define SMATRIX_USE_COMPUTATION
#else
#define SMATRIX_USE_CONSTEXPR
#endif


#include "Math/SMatrix.h"
#include "Math/BinaryOperators.h"

#include <vector>

namespace math {

  /// fixed size error matrix with double components
  template<unsigned int N>
  struct ErrorD {
    typedef ROOT::Math::SMatrix<double, N, N, ROOT::Math::MatRepSym<double, N> > type;
  };

  /// fixed size error matrix with double components
  template<unsigned int N>
  struct ErrorF {
    typedef ROOT::Math::SMatrix<float, N, N, ROOT::Math::MatRepSym<float, N> > type;
  };

  /// fixed size error matrix
  template<unsigned int N>
  struct Error {
    typedef typename ErrorD<N>::type type;
  };

}

#endif
