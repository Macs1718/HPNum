// ========================================================================
// ==                                                                    ==
// ==      Mise en oeuvre des méthodes de TreeNode et de Tree            ==
// ==                                                                    ==
// ========================================================================
# include "geometry/kdtree"

template class Geometry::KdTree<float,PerComponent>;
template class Geometry::KdTree<float,PerNode>;
// ................................................................................................
template class Geometry::KdTree<double,PerComponent>;
template class Geometry::KdTree<double,PerNode>;
