/**
 * \brief   A simple 3D box
 * \author  X. Juvigny
 * \version 0.1
 * \date    June 13th 2012
 *
 * Representation of a 3D box aligned with axis
 *
 */
#ifndef _GEOMETRY_BOX_HPP_
#define _GEOMETRY_BOX_HPP_
# include <initializer_list>
# include <vector>
# include <tuple>

namespace Geometry
{
    /**
     * \brief A basic 3D box
     *
     * This box is aligned with the axis. The length is the
     * distance between left and right side of the box (along
     * the x axis), width the distance between bottom and top
     * (along the y axis) and depth the distance between front
     * and back (along the z axis).
     *
     */
    template<typename K>
    class Box
    {
    public:
      ///@section Creators and destructor
      //@{
      /**
       * \brief Create an uninitialized box
       */
      Box();
      /**
       * \brief Create a box from minimal and maximal coordinates of corner.
       */
      Box(const std::initializer_list<K>& crd_min, 
          const std::initializer_list<K>& crd_max);
      /**
       * \brief Create a box from minimal coordinates and with, height and depth parameters
       */
      Box(const std::initializer_list<K>& origin, 
          const K& width, const K& height, const K& depth );
      /**
       * \brief Create a box from minimal and maximal coordinates of corners.
       */
      Box(const K minCrds[3], const K maxCrds[3]);
      
      Box( const Box& box ) = default;
      
      Box( Box&& box ) = default;
      /**
       * \brief Destructor
       */
      ~Box();
      //@}

	  Box& operator = ( const Box& box ) = default;
	  Box& operator = ( Box&& box ) = default;

      ///@section Getters and setters
      //@{
      /// Return the minimal coords of the box
      std::vector<K>& minCoords();
      const std::vector<K>&  minCoords() const;
      /// Return the maximal coords of the box      
      std::vector<K>& maxCoords();
      const std::vector<K>&  maxCoords() const;
      /// \brief Return the length of the box
      K  length() const;
      /// \brief Return the width of the box
      K  width () const;
      /// \brief Return the heigth of the box
      K  depth() const;
      /// \brief Return the square of the diameter
      K diameterSqr() const;
      /// \brief Return the dimensions of the box (
      std::tuple<K,K,K> dimensions() const {
	     return std::make_tuple(length(), width(), depth());
      }
      /// \brief Return the center of the box
      std::tuple<K,K,K> center() const;
      //@}

      void reset();
      private:
          std::vector<K> m_minCoords, m_maxCoords;
    };
}

#endif
