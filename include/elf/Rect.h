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

  Rect(const Rect<T> &other)
    : Rect(other._size, other._ptX0Y0){}

  Rect(const Size<T> &size, const Point<T> &ptX0Y0)
    : Rect(size.width(), size.height(), ptX0Y0.x(), ptX0Y0.y()){}

  Rect(const T &width, const T &height, const T &tX0, const T &tY0)
    : _size(width, height), _ptX0Y0(tX0, tY0){}

  void set(const Size<T> &size, const Point<T> &ptX0Y0){
    set(size.width(), size.height(), ptX0Y0.x(), ptX0Y0.y());
  }

  void set(const T &tWidth, const T &tHeight, const T &tX0, const T &tY0){
    set_size(tWidth, tHeight);
    set_x0_y0(tX0, tY0);
  }

  const Size<T> &size()const{
    return _size;
  }

  void set_size(const Size<T> &size){
    set_size(size.width(), size.height());
  }

  void set_size(const T &tWidth, const T &tHeight){
    set_width(tWidth);
    set_height(tHeight);
  }

  const Point<T> &x0_y0()const{
    return _ptX0Y0;
  }

  void set_x0_y0(const Point<T> &ptX0Y0){
    set_x0_y0(ptX0Y0.x(), ptX0Y0.y());
  }

  void set_x0_y0(const T &tX0, const T &tY0){
    set_x0(tX0);
    set_y0(tY0);
  }

  //Note: Performance
  Point<T> x1_y1()const{
    return Point<T>(x1(), y1());
  }

  void set_x1_y1(const Point<T> &ptX1Y1){
    set_x1_y1(ptX1Y1.x(), ptX1Y1.y());
  }

  void set_x1_y1(const T &tX1, const T &tY1){
    set_x1(tX1);
    set_y1(tY1);
  }

  const T &width()const{
    return _size.width();
  }

  const T &half_width()const{
    return _size.width() / (T)2;
  }

  void set_width(const T &width){
    _size.set_width(width);
  }

  const T &height()const{
    return _size.height();
  }

  const T &half_height()const{
    return _size.height() / (T)2;
  }

  void set_height(const T &height){
    _size.set_height(height);
  }

  const T &x0()const{
    return _ptX0Y0.x();
  }

  void set_x0(const T &tX0){
    _ptX0Y0.set_x(tX0);
  }

  const T &y0()const{
    return _ptX0Y0.y();
  }

  void set_y0(const T &tY0){
    _ptX0Y0.set_y(tY0);
  }

  //Note: Performance
  T x1()const{
    return _ptX0Y0.x() + _size.width();
  }

  void set_x1(const T &tX1){
    if(tX1 < _ptX0Y0.x()){
      _size.set_width(_ptX0Y0.x() - tX1);
      _ptX0Y0.set_x(tX1);
    }
    else
      _size.set_width(tX1 - _ptX0Y0.x());
  }

  //Note: Performance
  T y1()const{
    return _ptX0Y0.y() + _size.height();
  }

  void set_y1(const T &tY1){
    if(tY1 < _ptX0Y0.y()){
      _size.set_height(_ptX0Y0.y() - tY1);
      _ptX0Y0.set_y(tY1);
    }
    else
      _size.set_height(tY1 - _ptX0Y0.y());
  }

  Point<T> center()const{
    return Point<T>(x(), y());
  }

  T x()const{
    return x0() + half_width();
  }

  T y()const{
    return y0() + half_height();
  }

  template<class F>
  void Center(const Size<T> &szTarget){
    Center<F>(szTarget.width(), szTarget.height());
  }

  template<class F>
  void Center(const T &tTargetWidth, const T &tTargetHeight){
    F fScale = (F)height() / (F)width();
    T tHeight = tTargetWidth * fScale;
    if(tHeight > tTargetHeight){
      T tWidth = (F)tTargetHeight / fScale;
      set(tWidth, tTargetHeight, (tTargetWidth - tWidth) / (T)2, 0);
    }
    else
      set(tTargetWidth, tHeight, 0, (tTargetHeight - tHeight) / (T)2);
  }

  template<class F>
  void Center(const Rect<T> &rtTarget){
    F fScale = (F)height() / (F)width();
    T tHeight = rtTarget.width() * fScale;
    if(tHeight > rtTarget.height()){
      T tWidth = (F)rtTarget.height() / fScale;
      set_size(tWidth, rtTarget.height());
      set_x0_y0(rtTarget.x0() + ((rtTarget.width() - tWidth) / (T)2), rtTarget.y0());
    }
    else{
      set_size(rtTarget.width(), tHeight);
      set_x0_y0(rtTarget.x0(), rtTarget.y0() + ((rtTarget.height() - tHeight) / (T)2));
    }
  }

  //Note: Performance
  bool Contains(const Point<T> &point)const{
    return Contains(point.x(), point.y());
  }

  //Note: Performance
  bool Contains(const T &tX, const T &tY)const{
    return ContainsX(tX, x1()) && ContainsY(tY, y1());
  }

  bool Contains(const Point<T> &point, const Point<T> &ptThisX1Y1)const{
    return Contains(point.x(), point.y(),
                    ptThisX1Y1.x(), ptThisX1Y1.y());
  }

  bool Contains(const T &tX, const T &tY, const T &tThisX1, const T &tThisY1)const{
    return ContainsX(tX, tThisX1) && ContainsY(tY, tThisY1);
  }

  //Note: Performance
  bool ContainsX(const T &tX)const{
    return ContainsX(tX, x1());
  }

  bool ContainsX(const T &tX, const T &tThisX1)const{
    return tX >= x0() && tX < tThisX1;
  }

  //Note: Performance
  bool ContainsY(const T &tY)const{
    return ContainsY(tY, y1());
  }

  bool ContainsY(const T &tY, const T &tThisY1)const{
    return tY >= y0() && tY < tThisY1;
  }

  //Note: Performance
  bool Contains(const Rect<T> &other)const{
    return Contains(other, x1(), y1());
  }

  bool Contains(const Point<T> &ptOtherX0Y0, const Point<T> &ptOtherX1Y1, const Point<T> &ptThisX1Y1)const{
    return Contains(ptOtherX0Y0.x(), ptOtherX0Y0.y(), ptOtherX1Y1.x(), ptOtherX1Y1.y(), ptThisX1Y1.x(), ptThisX1Y1.y());
  }

  bool Contains(const T &tOtherX0, const T &tOtherY0, const T &tOtherX1, const T &tOtherY1, const T &ptThisX1, const T &ptThisY1)const{
    return x0() <= tOtherX0 && ptThisX1 >= tOtherX1
        && y0() <= tOtherY0 && ptThisY1 >= tOtherY1;
  }

  //Note: Performance
  bool IntersectsWith(const Rect &other)const{
    return IntersectsWith(other.x0(), other.y0(), other.x1(), other.y1, x1(), y1());
  }

  bool IntersectsWith(const Point<T> &ptOtherX0Y0, const Point<T> &ptOtherX1Y1, const Point<T> &ptThisX1Y1)const{
    return IntersectsWith(ptOtherX0Y0.x(), ptOtherX0Y0.y(), ptOtherX1Y1.x(), ptOtherX1Y1.y(), ptThisX1Y1.x(), ptThisX1Y1.y());
  }

  bool IntersectsWith(const T &tOtherX0, const T &tOtherY0, const T &tOtherX1, const T &tOtherY1, const T &tThisX1, const T &tThisY1)const{
    return x0() < tOtherX1 && tOtherX0 < tThisX1 && y0() < tOtherY1 && tOtherY0 < tThisY1;
  }
private:
  Size<T> _size;
  Point<T> _ptX0Y0;
};

} //namespace elf

#endif //RECT_H

