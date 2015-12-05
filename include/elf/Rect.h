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

  const Point<T> &GetLeftTop()const{
    return _ptLeftTop;
  }

  void SetLeftTop(const Point<T> &ptLeftTop){
    SetLeftTop(ptLeftTop.GetX(), ptLeftTop.GetY());
  }

  void SetLeftTop(const T &left, const T &top){
    SetLeft(left);
    SetTop(top);
  }

  //Note: Performance
  Point<T> GetRightBottom()const{
    return Point<T>(GetRight(), GetBottom());
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

  //Note: Performance
  T GetRight()const{
    return _ptLeftTop.GetX() + _size.GetWidth();
  }

  void SetRight(const T &right){
    if(right < _ptLeftTop.GetX()){
      _size.SetWidth(_ptLeftTop.GetX() - right);
      _ptLeftTop.SetX(right);
    }
    else
      _size.SetWidth(right - _ptLeftTop.GetX());
  }

  //Note: Performance
  T GetBottom()const{
    return _ptLeftTop.GetY() + _size.GetHeight();
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

  //Note: Performance
  bool Contains(const Point<T> &point)const{
    return Contains(point.GetX(), point.GetY());
  }

  //Note: Performance
  bool Contains(const T &tX, const T &tY)const{
    return ContainsX(tX, GetRight()) && ContainsY(tY, GetBottom());
  }

  bool Contains(const Point<T> &point, const Point<T> &ptRightBottom)const{
    return Contains(point.GetX(), point.GetY(),
                    ptRightBottom.GetX(), ptRightBottom.GetY());
  }

  bool Contains(const T &tX, const T &tY, const T &tRight, const T &tBottom)const{
    return ContainsX(tX, tRight) && ContainsY(tY, tBottom);
  }

  //Note: Performance
  bool ContainsX(const T &tX)const{
    return ContainsX(tX, GetRight());
  }

  bool ContainsX(const T &tX, const T &tRight)const{
    return tX >= GetLeft() && tX < tRight;
  }

  //Note: Performance
  bool ContainsY(const T &tY)const{
    return ContainsY(tY, GetBottom());
  }

  bool ContainsY(const T &tY, const T &tBottom)const{
    return tY >= GetTop() && tY < tBottom;
  }
private:
  Size<T> _size;
  Point<T> _ptLeftTop;
};

} //namespace elf

#endif //RECT_H

