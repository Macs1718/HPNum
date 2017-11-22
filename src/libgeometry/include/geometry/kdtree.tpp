// =================== Mise en oeuvre des méthodes de TreeNode =============
namespace Geometry
{
    template<typename K, int stride>
    KdTreeNode<K,stride>::KdTreeNode( const std::size_t& nbNodes, const K* X, const K* Y, const K* Z, 
				      int minNbNodesInALeaf, std::size_t* l2g, const K& tol) :
	m_nbNds(nbNodes), m_pt_l2g(l2g), 
	_leftChild(NULL), _rightChild(NULL),
	m_arr_indiceOfFreedoms(NULL), m_mustDestroyIndiceOfFreedoms(false),
	m_box()
    {
	m_box = compute_boundingBox<K,stride>(nbNodes, X, Y, Z, l2g, tol);
	if (nbNodes > minNbNodesInALeaf) {
	    Box<K> box1, box2;
	    std::size_t nbNodesInBox1, nbNodesInBox2;
	    std::tie( nbNodesInBox1, box1, nbNodesInBox2, box2 ) =
		subdivideAdjustAndSort(m_box, nbNodes, X, Y, Z, l2g, tol);
	    if (nbNodesInBox1>0) // Si des noeuds dans la boîte 1 :
		_leftChild  = new KdTreeNode<K,stride>(nbNodesInBox1, X, Y, Z, minNbNodesInALeaf, l2g, 
						       box1, tol);
	    if (nbNodesInBox2>0) // Si des noeuds dans la boîte 2 :
		_rightChild = new KdTreeNode<K,stride>(nbNodesInBox2, X, Y, Z, minNbNodesInALeaf, 
						       l2g+nbNodesInBox1, box2, tol);
	}
    }
    // ........................................................................
    template<typename K,int stride>
    KdTreeNode<K,stride>::KdTreeNode( const std::size_t& nbNodes, const K* X, const K* Y, const K* Z, 
				      int minNbNodesInALeaf, std::size_t* l2g,
				      const Box<K>& bbox, K tol) :
	m_nbNds(nbNodes), m_pt_l2g(l2g), 
	_leftChild(NULL), _rightChild(NULL),
	m_arr_indiceOfFreedoms(NULL), m_mustDestroyIndiceOfFreedoms(false),
	m_box(bbox)
    {
	if (nbNodes > minNbNodesInALeaf) {
	    Box<K> box1, box2;
	    std::size_t nbNodesInBox1, nbNodesInBox2;
	    std::tie( nbNodesInBox1, box1, nbNodesInBox2, box2 ) =
		subdivideAdjustAndSort(m_box, nbNodes, X, Y, Z, l2g, tol);
	    if (nbNodesInBox1>0) // Si des noeuds dans la boîte 1 :
		_leftChild  = new KdTreeNode<K,stride>(nbNodesInBox1, X, Y, Z, minNbNodesInALeaf, l2g, 
						       box1, tol);
	    if (nbNodesInBox2>0) // Si des noeuds dans la boîte 2 :
		_rightChild = new KdTreeNode<K,stride>(nbNodesInBox2, X, Y, Z, minNbNodesInALeaf, 
						       l2g+nbNodesInBox1, box2, tol);
	}
    }
    // ........................................................................
    template<typename K,int stride>
    KdTreeNode<K,stride>::KdTreeNode( const std::size_t& nbNodes1, const std::size_t& nbNodes2,
				      const K* X, const K* Y, const K* Z, 
				      int minNbNodesInALeaf, std::size_t* l2g, K tol) :
	m_nbNds(nbNodes1+nbNodes2), m_pt_l2g(l2g), 
	_leftChild(NULL), _rightChild(NULL),
	m_arr_indiceOfFreedoms(NULL), m_mustDestroyIndiceOfFreedoms(false),
	m_box()
    {
	m_box = compute_boundingBox<K,stride>(nbNodes1+nbNodes2, X, Y, Z, l2g, tol);
	Box<K> box1, box2;
	box1 = compute_boundingBox<K,stride>(nbNodes1, X, Y, Z, l2g, tol );
	box2 = compute_boundingBox<K,stride>(nbNodes2, X, Y, Z, l2g+nbNodes1, tol );
	if (nbNodes1>0) { // Si des noeuds dans la boîte 1 : 
	    _leftChild  = new KdTreeNode<K,stride>(nbNodes1, X,Y,Z, minNbNodesInALeaf, l2g, 
						   box1, tol);
	}
	if (nbNodes2>0) { // Si des noeuds dans la boîte 2 :
	    _rightChild = new KdTreeNode<K,stride>(nbNodes2, X, Y, Z, minNbNodesInALeaf, 
						   l2g+nbNodes1, box2, tol);
	}
    }
    // ........................................................................
    template<typename K, int stride>
    KdTreeNode<K,stride>::~KdTreeNode()
    {
	if (_leftChild ) delete _leftChild;
	if (_rightChild) delete _rightChild;
	if (m_mustDestroyIndiceOfFreedoms) delete [] m_arr_indiceOfFreedoms;
    }
// ------------------------------------------------------------------------
    template<typename K, int stride> std::ostream&
    KdTreeNode<K,stride>::dumpXML(std::ostream& out) const
    {
	int i;
	auto minCrds = m_box.minCoords();
	auto maxCrds = m_box.maxCoords();
	out << "<treeNode>\n";
	out << "<bounding class = \"box\">\n"
	    << "  <minCoords> " << minCrds[0] << ", " << minCrds[1] << ", " 
	    << minCrds[2] << "</minCoords>\n"
	    << "  <maxCoords> " << maxCrds[0] << ", " << maxCrds[1] << ", " 
	    << maxCrds[2] << "</maxCoords>\n</bounding>\n";
	out << "<indices> ";
	for (i=0; i < m_nbNds-1; i++) out << m_pt_l2g[i] << ", ";
	if (m_nbNds > 0) out << m_pt_l2g[m_nbNds-1];
	out << "</indices>\n";
	out << "<children>\n  <left>\n";
	if (_leftChild) _leftChild->dumpXML(out);
	out << "</left>\n  <right>\n";
	if (_rightChild) _rightChild->dumpXML(out);
	out << "</right>\n</children>\n</treeNode>\n";
	return out;
    }
    // ------------------------------------------------------------------------
    template<typename K, int stride> std::size_t
    KdTreeNode<K,stride>::compute_nbDegOfFreedoms(ptNbDegOfFreedomFct degOfFree)
    {
	m_nbDegOfFreedoms = 0;
	if (_leftChild)  m_nbDegOfFreedoms += leftChild ()->compute_nbDegOfFreedoms(degOfFree);
	if (_rightChild) m_nbDegOfFreedoms += rightChild()->compute_nbDegOfFreedoms(degOfFree);
	if ((_leftChild == NULL) && (_rightChild == nullptr )) {
	    for (size_t i = 0; i < m_nbNds; ++i)
		m_nbDegOfFreedoms += degOfFree(m_pt_l2g[i]);
	}
	return m_nbDegOfFreedoms;
    }
    // ------------------------------------------------------------------------
    template<typename K, int stride> void
    KdTreeNode<K,stride>::compute_indDegOfFreedoms(node2FreedomFct indFreedoms, 
						   std::size_t* indicesOfFreedoms)
    {
	assert(m_nbDegOfFreedoms>0);
	if ( (m_mustDestroyIndiceOfFreedoms == true) && (m_arr_indiceOfFreedoms != nullptr ) )
	    delete [] m_arr_indiceOfFreedoms;
	if (indicesOfFreedoms == nullptr) {
	    m_arr_indiceOfFreedoms = new std::size_t[m_nbDegOfFreedoms];
	    m_mustDestroyIndiceOfFreedoms = true;
	}
	else {
	    m_arr_indiceOfFreedoms = indicesOfFreedoms;
	    m_mustDestroyIndiceOfFreedoms = false;
	}

	if ((leftChild()==nullptr) && (rightChild()==nullptr))
	    indFreedoms(m_nbNds, m_pt_l2g, m_nbDegOfFreedoms, m_arr_indiceOfFreedoms);
	else {
	    if (leftChild() != nullptr) 
		leftChild()->compute_indDegOfFreedoms(indFreedoms, m_arr_indiceOfFreedoms);
	    if (rightChild() != nullptr) {
		std::size_t* ptIndices = m_arr_indiceOfFreedoms;
		if (leftChild() != nullptr) ptIndices += _leftChild->m_nbDegOfFreedoms;
		rightChild()->compute_indDegOfFreedoms(indFreedoms, ptIndices);
	    }
	}
    }
    // ========================================================================
    template<typename K, int stride>
    KdTree<K,stride>::KdTree( const std::size_t& nbNodes, const K* X, const K* Y, const K* Z,
			      int minNbNodesInALeaf, std::size_t* l2g, K tol) :
	_root(NULL)
    {
	for ( std::size_t i = 0; i < nbNodes; i++ ) l2g[i] = i;
	if (nbNodes > 0)
	    _root = new KdTreeNode<K,stride>(nbNodes, X, Y, Z, minNbNodesInALeaf, l2g,tol);
    }
    // ........................................................................
    template<typename K, int stride>
    KdTree<K,stride>::KdTree( const std::size_t& nbNodes1, const std::size_t& nbNodes2,
			      const K* X, const K* Y, const K* Z,
			      int minNbNodesInALeaf, std::size_t* l2g, K tol) :
	_root(NULL)
    {
	// Here, l2g is filled before call of this constructor
	if (nbNodes1+nbNodes2 > 0)
	    _root = new KdTreeNode<K,stride>(nbNodes1, nbNodes2, X, Y, Z, minNbNodesInALeaf, l2g,tol);
    }
    // ........................................................................
    template<typename K, int stride>
    KdTree<K,stride>::~KdTree()
    {
	if (_root) delete _root;
    }
    // ================================================================================
    
}
