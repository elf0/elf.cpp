#ifndef RECT_H
#define RECT_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Size.h"
#include "Point.h"

namespace elf{

template<class T>
class Rect{
public:
    Rect(){}

    Rect(const Rect<T> &rect)
        : Rect(rect._size, rect._ptGetLeftTop){}

    Rect(const Size<T> &size, const Point<T> &ptLeftTop)
        : Rect(size.GetWidth(), size.GetHeight(), ptLeftTop.GetX(), ptLeftTop.GetY()){}

    Rect(const T &width, const T &height, const T &left, const T &top)
        : _size(width, height), _ptLeftTop(left, top){}

    void Set(const Size<T> &size, const Point<T> &ptLeftTop){
        Set(size.GetWidth(), size.GetHeight(), ptLeftTop.GetX(), ptLeftTop.GetY());
    }

    void Set(const T &width, const T &height, const T &left, const T &top){
        SetSize(width, height);
        SetLeftTop(left, top);
    }

    void SetSize(const Size<T> &size){
        SetSize(size.GetWidth(), size.GetHeight());
    }

    void SetSize(const T &width, const T &height){
        SetWidth(width);
        SetHeight(height);
    }

    void SetLeftTop(const Point<T> &ptLeftTop){
        SetLeftTop(ptLeftTop.GetX(), ptLeftTop.GetY());
    }

    void SetLeftTop(const T &left, const T &top){
        SetLeft(left);
        SetTop(top);
    }

    void SetRightBottom(const Point<T> &ptRightBottom){
        SetRightBottom(ptRightBottom.GetX(), ptRightBottom.GetY());
    }

    void SetRightBottom(const T &right, const T &bottom){
        SetRight(right);
        SetBottom(bottom);
    }

    const T&GetWidth()const{
        return _size.GetWidth();
    }

    void SetWidth(const T &width){
        _size.SetWidth(width);
    }

    const T&GetHeight()const{
        return _size.GetHeight();
    }

    void SetHeight(const T &height){
        _size.SetHeight(height);
    }

    const T&GetLeft()const{
        return _ptLeftTop.GetX();
    }

    void SetLeft(const T &left){
        _ptLeftTop.SetX(left);
    }

    const T&GetTop()const{
        return _ptLeftTop.GetY();
    }

    void SetTop(const T &top){
        _ptLeftTop.SetY(top);
    }

    void GetRight(T &right){
        right = _ptLeftTop.GetX() + _size.GetWidth();
    }

    void SetRight(const T &right){
        if(right < _ptLeftTop.GetX()){
            _size.SetWidth(_ptLeftTop.GetX() - right);
            _ptLeftTop.SetX(right);
        }
        else
            _size.SetWidth(right - _ptLeftTop.GetX());
    }

    void GetBottom(T &bottom)const{
        bottom = _ptLeftTop.GetY() + _size.getHeight();
    }

    void SetBottom(const T &bottom){
        if(bottom < _ptLeftTop.GetY()){
            _size.SetHeight(_ptLeftTop.GetY() - bottom);
            _ptLeftTop.SetY(bottom);
        }
        else
            _size.SetHeight(bottom - _ptLeftTop.GetY());
    }

    void Center(const Size<T> &szTarget){
        Center(szTarget.GetWidth(), szTarget.GetHeight());
    }

    template<class F>
    void Center(const T &tTargetWidth, const T &tTargetHeight){
        F fScale = (F)GetHeight() / (F)GetWidth();
        T tHeight = tTargetWidth * fScale;
        if(tHeight > tTargetHeight){
            T tWidth = (F)tTargetHeight / fScale;
            Set(tWidth, tTargetHeight, (tTargetWidth - tWidth) >> 1, 0);
        }
        else
            Set(tTargetWidth, tHeight, 0, (tTargetHeight - tHeight) >> 1);
    }

    template<class F>
    void Center(const Rect<T> &rtTarget){
        F fScale = (F)GetHeight() / (F)GetWidth();
        T tHeight = rtTarget.GetWidth() * fScale;
        if(tHeight > rtTarget.GetHeight()){
            T tWidth = (F)rtTarget.GetHeight() / fScale;
            SetSize(tWidth, rtTarget.GetHeight());
            SetLeftTop(rtTarget.GetLeft() + ((rtTarget.GetWidth() - tWidth) >> 1), rtTarget.GetTop());
        }
        else{
            SetSize(rtTarget.width(), tHeight);
            SetLeftTop(rtTarget.GetLeft(), rtTarget.GetTop() + ((rtTarget.height() - tHeight) >> 1));
        }
    }
private:
    Size<T> _size;
    Point<T> _ptLeftTop;
};

} //namespace elf

#endif //RECT_H

