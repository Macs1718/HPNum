// ------------------------------------------------------------------------
template<typename K>
Box<K>::Box() : m_minCoords(3), m_maxCoords(3)
{}
// ------------------------------------------------------------------------
template<typename K>
Box<K>::Box( const std::initializer_list<K>& min_crds, 
             const std::initializer_list<K>& max_crds) :
    m_minCoords(min_crds), m_maxCoords(max_crds)
{
}
// ------------------------------------------------------------------------
template<typename K>
Box<K>::Box( const std::initializer_list<K>& origin,
             const K& width, const K& height, const K& depth) :
  m_minCoords(origin),
  m_maxCoords(3)
{
  m_maxCoords[0] = m_minCoords[0] + width ;
  m_maxCoords[1] = m_minCoords[1] + height;
  m_maxCoords[2] = m_minCoords[2] + depth ;
}
// ------------------------------------------------------------------------
template<typename K>
Box<K>::Box(const K minCrds[3], const K maxCrds[3]) :
  m_minCoords(3), m_maxCoords(3)
{
  m_minCoords[0] = minCrds[0]; m_minCoords[1] = minCrds[1]; m_minCoords[2] = minCrds[2];
  m_maxCoords[0] = maxCrds[0]; m_maxCoords[1] = maxCrds[1]; m_maxCoords[2] = maxCrds[2];
}
// ------------------------------------------------------------------------
template<typename K>
Box<K>::~Box()
{}
// ========================================================================
template<typename K> std::vector<K>& 
Box<K>::minCoords() {
  return m_minCoords;
}
// ------------------------------------------------------------------------
template<typename K> const std::vector<K>&  
Box<K>::minCoords() const
{
  return m_minCoords;
}
// ------------------------------------------------------------------------
template<typename K> std::vector<K>& 
Box<K>::maxCoords()
{
  return m_maxCoords;
}
// ------------------------------------------------------------------------
template<typename K> const std::vector<K>&  
Box<K>::maxCoords() const
{
  return m_maxCoords;
}
// ------------------------------------------------------------------------
template<typename K> K
Box<K>::length() const
{
  return m_maxCoords[0]-m_minCoords[0];
}
// ------------------------------------------------------------------------
template<typename K> K
Box<K>::width() const
{
  return m_maxCoords[1]-m_minCoords[1];
}
// ------------------------------------------------------------------------
template<typename K> K
Box<K>::depth() const
{
  return m_maxCoords[2]-m_minCoords[2];
}
// ------------------------------------------------------------------------
template<typename K> K
Box<K>::diameterSqr() const
{
  K l = length();
  K w = width();
  K d = depth();
  return l*l+w*w+d*d;
}
// ------------------------------------------------------------------------
template<typename K> std::tuple<K,K,K>
Box<K>::center() const
{
  return std::make_tuple( 0.5*(m_minCoords[0]+m_maxCoords[0]),
                          0.5*(m_minCoords[1]+m_maxCoords[1]),
                          0.5*(m_minCoords[2]+m_maxCoords[2]) );
}
// ========================================================================
template<typename K> void
Box<K>::reset()
{
  m_minCoords[0] = m_minCoords[1] = m_minCoords[2] = K(0);
  m_maxCoords[0] = m_maxCoords[1] = m_maxCoords[2] = K(0);
}
