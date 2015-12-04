#ifndef POINT_H
#define POINT_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

namespace elf{

template<class T>
class Point{
public:
    Point()
        : _x(), _y(){}

    Point(const Point &point)
        : Point(point.GetX(), point.GetY()){}

    Point(const T &x, const T &y)
        : _x(x), _y(y){}

    void Set(const Point &point){
        Set(point.GetX(), point.GetY());
    }

    void Set(const T &x, const T &y){
        SetX(x);
        SetY(y);
    }

    const T&GetX()const{
        return _x;
    }

    void SetX(const T &x){
        _x= x;
    }

    const T&GetY()const{
        return _y;
    }

    void SetY(const T &y){
        _y = y;
    }
private:
    T _x;
    T _y;
};

} //namespace elf

#endif //POINT_H

