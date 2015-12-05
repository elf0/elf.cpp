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
        : Point(point.x(), point.y()){}

    Point(const T &x, const T &y)
        : _x(x), _y(y){}

    void set(const Point &point){
        set(point.x(), point.y());
    }

    void set(const T &x, const T &y){
        set_x(x);
        set_y(y);
    }

    const T &x()const{
        return _x;
    }

    void set_x(const T &x){
        _x = x;
    }

    const T &y()const{
        return _y;
    }

    void set_y(const T &y){
        _y = y;
    }
private:
    T _x;
    T _y;
};

} //namespace elf

#endif //POINT_H

