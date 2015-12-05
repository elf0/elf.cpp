#ifndef SIZE_H
#define SIZE_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

namespace elf{

template<class T>
class Size{
public:
    Size()
        : _width(), _height(){}

    Size(const Size &size)
        : _width(size.Width()), _height(size.Height()){}

    Size(const T &width, const T &height)
        : _width(width), _height(height){}

    void set(const Size &size){
        set(size.Width(), size.Height());
    }

    void set(const T &width, const T &height){
        setWidth(width);
        setHeight(height);
    }

    const T &width()const{
        return _width;
    }

    void set_width(const T &width){
        _width = width;
    }

    const T &height()const{
        return _height;
    }

    void set_height(const T &height){
        _height = height;
    }
private:
    T _width;
    T _height;
};

} //namespace elf

#endif //SIZE_H

