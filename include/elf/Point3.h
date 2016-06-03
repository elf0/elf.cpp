#ifndef POINT3_H
#define POINT3_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

namespace elf{

template<class T>
class Point3{
public:
    Point3()
        : _x(), _y(), _z(){}

    Point3(const Point3 &point)
        : Point3(point.x(), point.y(), point.z()){}

    Point3(const T &x, const T &y, const T &z)
        : _x(x), _y(y), _z(z){}

    void set(const Point &point){
        set(point.x(), point.y(), point.z());
    }

    void set(const T &x, const T &y, const T &z){
        set_x(x);
        set_y(y);
        set_z(z);
    }

    const T &x()const{
        return _x;
    }

    void set_x(const T &x){
        _x= x;
    }

    const T &y()const{
        return _y;
    }

    void set_y(const T &y){
        _y = y;
    }

    const T &z()const{
        return _z;
    }

    void set_z(const T &z){
        _z = z;
    }
private:
    T _x;
    T _y;
    T _z;
};

} //namespace elf

#endif // POINT3_H

