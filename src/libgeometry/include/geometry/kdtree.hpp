#ifndef _GEOMETRY_KDTREE_HPP_
#define _GEOMETRY_KDTREE_HPP_
#include "geometry/boundingbox.hpp"
#include "geometry/box.hpp"
#include "geometry/tolerance.hpp"
#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

using ptNbDegOfFreedomFct = std::function<int( const std::size_t &indFreedom )>;
using node2FreedomFct =
    std::function<void( const std::size_t &nbNodes, std::size_t *indNodes,
                        std::size_t &nbFreedoms, std::size_t *indFreedoms )>;

namespace Geometry {
    /**@brief Node class of the Kd-Tree

       Each node for the tree contains a box which bounds a part of the nodes.

       Chaque noeud de l'arbre contient une boîte englobant une partie des
       points appartenant à l'espace qu'on découpe ainsi qu'un pointeur
       sur un tableau partagé l2g contenant la renumérotation des points
       au fur et à mesure des itérations de la bissection.
    */
    template <typename K, int stride>
    class KdTreeNode {
    public:
        /** @name Constructors and destructor
       */
        //@{
        /** @brief Construit le noeud et ses fils éventuels

          Construit le noeud et ses fils par bissection itérative de la boîte
         contenant
          le nuage de nbNodes points décrits par nodes. Si le nombre de noeud
         est
         inférieur
          à un nombre minimal
          de noeuds donné par minNbNodesInALeaf, le noeud est une feuille de
         l'arbre.
      */
        KdTreeNode( const std::size_t &nbNodes, const K *ndsX, const K *ndsY,
                    const K *ndsZ, int minNbNodesInALeaf, std::size_t *l2g,
                    const K &tol = ::Geometry::Tolerance<K>::value( ) );
        /** @brief Construit le noeud et ses fils éventuels

          Construit le noeud et ses fils par bissection itérative de la boîte
         contenant
          le nuage de nbNodes points décrits par nodes. Si le nombre de noeud
         est
         inférieur
          à un nombre minimal
          de noeuds donné par minNbNodesInALeaf, le noeud est une feuille de
         l'arbre.
      */
        KdTreeNode( const std::size_t &nbNodes, const K *ndsX, const K *ndsY,
                    const K *ndsZ, int minNbNodesInALeaf, std::size_t *l2g,
                    const ::Geometry::Box<K> &bbox,
                    K tol = ::Geometry::Tolerance<K>::value( ) );

        /** @brief Construit le noeud et ses fils éventuels

          Construit le noeud et ses fils par bissection itérative de la boîte
         contenant
          le nuage de nbNodes points décrits par nodes. Si le nombre de noeud
         est
         inférieur
          à un nombre minimal
          de noeuds donné par minNbNodesInALeaf, le noeud est une feuille de
         l'arbre.
      */
        KdTreeNode( const std::size_t &nbNodes1, const std::size_t &nbNodes2,
                    const K *ndsX, const K *ndsY, const K *ndsZ,
                    int minNbNodesInALeaf, std::size_t *l2g,
                    K tol = ::Geometry::Tolerance<K>::value( ) );

        /** @brief Destructeur

          Détruit le noeud et ses fils éventuels
      */
        ~KdTreeNode( );
        //@}

        /** @name Accesseurs et modifieurs */
        // @{
        // /**
        // @brief      Retourne le nombre de sommets contenus dans ce noeud de
        // l'arbre
        // */
        //
        // @return     { description_of_the_return_value }
        //
        std::size_t nbVertices( ) const { return m_nbNds; }
        /** @brief Retourne les indices des sommets contenus dans ce noeud de
         * l'arbre
       */
        const std::size_t *indicesOfVertices( ) const { return m_pt_l2g; }
        /** @brief Retourne un pointeur sur le fils gauche (NULL si pas de fils
       * gauche
       * ) */
        const KdTreeNode<K, stride> *leftChild( ) const { return _leftChild; }
        /** @brief Retourne un pointeur sur le fils droit (NULL si pas de fils
         * droit
         * )
       */
        const KdTreeNode<K, stride> *rightChild( ) const { return _rightChild; }

        /** @brief Retourne un pointeur sur le fils gauche (NULL si pas de fils
       * gauche
       * ) */
        KdTreeNode<K, stride> *leftChild( ) { return _leftChild; }
        /** @brief Retourne un pointeur sur le fils droit (NULL si pas de fils
         * droit
         * )
       */
        KdTreeNode<K, stride> *rightChild( ) { return _rightChild; }

        /** @brief Retourne la boîte minimal contenant tous les sommets de ce
         * noeud
         */
        const ::Geometry::Box<K> &boundingBox( ) const { return m_box; }
        /** @brief Number of freedoms degree for the tree. */
        std::size_t compute_nbDegOfFreedoms( ptNbDegOfFreedomFct degOfFree );
        /** @brief Indices of degree of freedoms  corresponding to the indices
         of
         nodes
          contained by  the tree.
      */
        void compute_indDegOfFreedoms( node2FreedomFct indFreedoms,
                                       std::size_t *indicesFreedoms = nullptr );
        /** @brief Return number of degree of freedoms for this node */
        std::size_t getNumberOfFreedoms( ) const { return m_nbDegOfFreedoms; }
        /** @brief Return the indices of degree of freedoms  */
        const std::size_t *getIndiceOfFreedoms( ) const {
            return m_arr_indiceOfFreedoms;
        }
        std::size_t *getIndiceOfFreedoms( ) { return m_arr_indiceOfFreedoms; }
        //@}

        /** @name Sorties
       */
        //@{
        /** @brief Sort une représentation xml du noeud (fils compris)
       */
        std::ostream &dumpXML( std::ostream &out ) const;
        //@}
        std::size_t *stealIndiceOfFreedoms( ) {
            m_mustDestroyIndiceOfFreedoms = false;
            return m_arr_indiceOfFreedoms;
        }

    private:
        std::size_t  m_nbNds;  // Nombre de points contenus dans ce noeud.
        std::size_t *m_pt_l2g; /* Partie de la renumérotation qui
                                nous intéresse */
        /* Fils gauche et droite. L'un ou l'autre (ou les deux) peuvent être
         * nuls.
       */
        KdTreeNode<K, stride> *_leftChild, *_rightChild;
        /* Boîte englobant une portion de l'espace. */
        ::Geometry::Box<K> m_box;
        std::size_t        m_nbDegOfFreedoms;
        std::size_t *      m_arr_indiceOfFreedoms;
        bool               m_mustDestroyIndiceOfFreedoms;
    };

    /** @brief Arbre représentant la bissection récursive d'un nuage de points

        Chaque noeud de l'arbre contient une boîte décrivant la partie de
       l'espace
        contenu dans ce noeud.
    */
    template <typename K, int stride = PerComponent>
    class KdTree {
    public:
        /** @name Constructeurs et destructeur. */
        //@{
        /** @brief Construit un arbre de découpage de l'espace pris par les
         * sommets
         */
        KdTree( const std::size_t &nbNodes, const K *ndsX, const K *ndsY,
                const K *ndsZ,
                // ptNbDegOfFreedomFct degOfFree, node2FreedomFct indFreedoms,
                int minNbNodesInALeaf, std::size_t *l2g,
                K tol = ::Geometry::Tolerance<K>::value( ) );
        /** @brief Construit un arbre de découpage de l'espace pris par les
         * sommets
         */
        KdTree( const std::size_t &nbNodes1, const std::size_t &nbNodes2,
                const K *nodesX, const K *nodesY, const K *nodesZ,
                // ptNbDegOfFreedomFct degOfFree, node2FreedomFct indFreedoms,
                int minNbNodesInALeaf, std::size_t *l2g,
                K tol = ::Geometry::Tolerance<K>::value( ) );

        /** @brief Destruction de l'arbre. Détruit tous les fils et la
          renumérotation des sommets.
      */
        ~KdTree( );
        //@}

        /** @name Accesseurs */
        //@{
        /** @brief Retourne la racine de l'arbre */
        KdTreeNode<K, stride> *root( ) { return _root; }
        /** @brief Retourne la racine de l'arbre */
        const KdTreeNode<K, stride> *root( ) const { return _root; }
        /* @brief Compute number of degree of freedom */
        std::size_t compute_nbDegOfFreedoms( ptNbDegOfFreedomFct degOfFree ) {
            return _root->compute_nbDegOfFreedoms( degOfFree );
        }
        /** @brief compute the indices of degree of freedom */
        void compute_indDegOfFreedoms( node2FreedomFct indFreedoms ) {
            _root->compute_indDegOfFreedoms( indFreedoms );
        }
        //@}
    private:
        KdTreeNode<K, stride> *_root;
    };
}

#endif
