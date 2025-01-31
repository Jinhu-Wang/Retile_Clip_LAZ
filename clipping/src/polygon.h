#ifndef MM_BASIC_POLYGON_H
#define MM_BASIC_POLYGON_H

// System
#include <algorithm>
#include <vector>

// Local - [vec]
#include "box.h"
#include "vec.h"

namespace mm
{

    /// \brief A 2D polygon representation.
    /// \class GenericPolygon miemie/core/polygon.h
    template <typename FT>
    class GenericPolygon : public std::vector<Vec<2, FT>>
    {
        typedef std::vector<Vec<2, FT>> BaseClass;

    public:
        /// \brief Default constructor.
        GenericPolygon() = default;

        /// \brief Initializes with a know size.
        explicit GenericPolygon(std::size_t size)
        {
            BaseClass::resize(size);
        }

        /// \brief Initializes from a known range.
        template <class InputIterator>
        GenericPolygon(InputIterator first, InputIterator last);

        /**
         * @brief Checks if the polygon has a clockwise orientation (right-hand rule).
         * @details This method determines the orientation of a polygon based on the sign of its signed area:
         *              - negative: clockwise
         *              - positive: counterclockwise
         */
        bool is_clockwise() const;

        /// \brief Reverses the orientation of the polygon. The first vertex (pointed to by `p.begin()`, or [0])
        /// remains the same.
        void reverse_orientation();

        /// \brief Tests if a \p point lies inside the polygon.
        bool contains(const Vec<2, FT> &point) const;

        /// \brief Tests if a polygon \p plg lies inside this polygon (i.e., all its vertices are inside).
        bool contains(const GenericPolygon<FT> &plg) const;

        /// @brief Tests if a polygon \p plg intersects this polygon (i.e., one of its points lies in this polygon).
        bool intersects(const GenericPolygon<FT> &plg) const;
        /// \brief Returns the signed area of the polygon. The signed area is positive for counter clockwise polygons
        /// and negative for clockwise polygons.
        FT signed_area() const;

        /// \brief Returns the area of this polygon. This is the absolute value of the signed area, so it is always
        /// positive.
        FT area() const;

        /// \brief Returns the smallest axis-aligned bounding box containing this polygon.
        GenericBox<2, FT> bbox() const;

    }; // !- template class GenericPolygon;

    template <typename FT>
    class ShpPolygon : public GenericPolygon<FT>
    {
    public:
        /// @brief Indices of the negative that this positive polygon contains.
        std::vector<int> indices;
        bool parsed;
        ShpPolygon()
            : parsed(false)
        {
        }
    };

    //////////////////////////////////////////////////////////////////////////

    template <typename FT>
    template <class InputIterator>
    inline GenericPolygon<FT>::GenericPolygon(InputIterator first, InputIterator last)
    {
        std::copy(first, last, BaseClass::end());
    }

    template <typename FT>
    inline bool GenericPolygon<FT>::is_clockwise() const
    {
#if 0
        return signed_area() < 0;
#else
        double sum = 0;
        for (std::size_t i = 0; i + 1 < BaseClass::size(); ++i)
        {
            const auto &p1 = BaseClass::at(i);
            const auto &p2 = BaseClass::at((i + 1) % BaseClass::size());
            sum += (p2.x - p1.x) * (p2.y + p1.y);
        }
        return sum < 0.0;
#endif
    }

    template <typename FT>
    inline void GenericPolygon<FT>::reverse_orientation()
    {
        if (BaseClass::size() <= 1)
            return;
        auto it = BaseClass::begin();
        std::reverse(++it, BaseClass::end());
    }

    template <typename FT>
    inline bool GenericPolygon<FT>::contains(const Vec<2, FT> &p) const
    {
        bool inside = false;
        std::size_t n = BaseClass::size();
        for (std::size_t i = 0, j = n - 1; i < n; j = i, ++i)
        {
            const Vec<2, FT> &u0 = BaseClass::at(i);
            const Vec<2, FT> &u1 = BaseClass::at(j); // current edge

            if (((u0.y <= p.y) && (p.y < u1.y)) || // U1 is above the ray, U0 is on or below the ray
                ((u1.y <= p.y) && (p.y < u0.y)))   // U0 is above the ray, U1 is on or below the ray
            {
                // find x-intersection of current edge with the ray.
                // Only consider edge crossings on the ray to the right of P.
                double x = u0.x + (p.y - u0.y) * (u1.x - u0.x) / (u1.y - u0.y);
                if (x > p.x)
                    inside = !inside;
            }
        }

        return inside;
    }

    template <typename FT>
    inline bool GenericPolygon<FT>::contains(const GenericPolygon<FT> &plg) const
    {
        for (const auto &p : plg)
        {
            if (!contains(p))
                return false;
        }
        return true;
    }

    /// @brief
    /// @tparam FT
    /// @param plg
    /// @return
    template <typename FT>
    inline bool GenericPolygon<FT>::intersects(const GenericPolygon<FT> &plg) const
    {
        for (const auto &p : plg)
        {
            if (this->contains(p))
            {
                return true;
            }
        }
        

        return false;
    }

    template <typename FT>
    inline FT GenericPolygon<FT>::area() const
    {
        return std::fabs(signed_area());
    }

    // http://astronomy.swin.edu.au/~pbourke/geometry/polyarea/
    template <typename FT>
    inline FT GenericPolygon<FT>::signed_area() const
    {
        FT result = 0;
        for (unsigned int i = 0; i < BaseClass::size(); i++)
        {
            unsigned int j = (i + 1) % BaseClass::size();
            const Vec<2, FT> &t1 = BaseClass::at(i);
            const Vec<2, FT> &t2 = BaseClass::at(j);
            result += t1.x * t2.y - t2.x * t1.y;
        }
        result /= 2.0;
        return result;
    }

    template <typename FT>
    inline GenericBox<2, FT> GenericPolygon<FT>::bbox() const
    {
        GenericBox<2, FT> box;
        for (auto p : *this)
            box.grow(p);
        return box;
    }

} // !- namespace mm;

#endif // !- MM_BASIC_POLYGON_h