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
        : _width(size.GetWidth()), _height(size.GetHeight()){}

    Size(const T &width, const T &height)
        : _width(width), _height(height){}

    void Set(const Size &size){
        Set(size.GetWidth(), size.GetHeight());
    }

    void Set(const T &width, const T &height){
        SetWidth(width);
        SetHeight(height);
    }

    const T&GetWidth()const{
        return _width;
    }

    void SetWidth(const T &width){
        _width = width;
    }

    const T&GetHeight()const{
        return _height;
    }

    void SetHeight(const T &height){
        _height = height;
    }
private:
    T _width;
    T _height;
};

} //namespace elf

#endif //SIZE_H

