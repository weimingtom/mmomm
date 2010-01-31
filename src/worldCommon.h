#ifndef WORLD_COMMON_H_
#define WORLD_COMMON_H_

#include <boost/functional/hash.hpp>

struct CellCoord {
    CellCoord(long x, long y);
    bool operator==(const CellCoord& a) const;

    long x;
    long y;

    class Hash : public boost::hash< CellCoord > {
    public:

        std::size_t operator()(const CellCoord& a) const;

    };
};

#endif
