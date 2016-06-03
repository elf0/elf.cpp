#ifndef SIZE_H
#define SIZE_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

namespace elf{

template<class T>
class Size{
public:
    Size()
        : _width(), _height(){}

    Size(const Size &size)
        : _width(size.width()), _height(size.height()){}

    Size(const T &width, const T &height)
        : _width(width), _height(height){}

    void set(const Size &size){
        set(size.width(), size.height());
    }

    void set(const T &width, const T &height){
        set_width(width);
        set_height(height);
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

