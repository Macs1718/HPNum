# include "geometry/boundingbox.hpp"
# include "geometry/boundingbox.tpp"

namespace Geometry
{
 
  template Box<float>
  compute_boundingBox<float, PerComponent>(const std::size_t& nbPoints,
                                           const float* X, const float* Y, const float* Z,
                                           const std::size_t* indices = nullptr,
                                           const float& tol = Tolerance<float>::value() );
  template Box<float>
  compute_boundingBox<float, PerNode>(const std::size_t& nbPoints,
                                      const float* X, const float* Y, const float* Z,
                                      const std::size_t* indices = nullptr,
                                      const float& tol = Tolerance<float>::value() );

  template Box<double>
  compute_boundingBox<double, PerComponent>(const std::size_t& nbPoints,
                                           const double* X, const double* Y, const double* Z,
                                           const std::size_t* indices = nullptr,
                                           const double& tol = Tolerance<double>::value() );
  template Box<double>
  compute_boundingBox<double, PerNode>(const std::size_t& nbPoints,
                                       const double* X, const double* Y, const double* Z,
                                       const std::size_t* indices = nullptr,
                                       const double& tol = Tolerance<double>::value() );

  template std::tuple<std::size_t,Box<float>,std::size_t,Box<float> >
  subdivideAdjustAndSort<float, PerComponent>(const Box<float>& box, const std::size_t& nbPoints, 
                                              const float* X, const float* Y, const float* Z, 
                                              std::size_t* indices, const float& tol);

  template std::tuple<std::size_t,Box<float>,std::size_t,Box<float> >
  subdivideAdjustAndSort<float, PerNode>(const Box<float>& box, const std::size_t& nbPoints, 
                                         const float* X, const float* Y, const float* Z, 
                                         std::size_t* indices, const float& tol);

  template std::tuple<std::size_t,Box<double>,std::size_t,Box<double> >
  subdivideAdjustAndSort<double, PerComponent>(const Box<double>& box, const std::size_t& nbPoints, 
                                              const double* X, const double* Y, const double* Z, 
                                              std::size_t* indices, const double& tol);

  template std::tuple<std::size_t,Box<double>,std::size_t,Box<double> >
  subdivideAdjustAndSort<double, PerNode>(const Box<double>& box, const std::size_t& nbPoints, 
                                         const double* X, const double* Y, const double* Z, 
                                         std::size_t* indices, const double& tol);

}
