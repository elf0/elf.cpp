#ifndef BUFFER_H
#define BUFFER_H

#include "WBuffer.h"

class Buffer : public WBuffer {
public:
        // Buffer() {}
        // ~Buffer() {}

  void set(C *pBegin, C *pEnd) {
    WBuffer::set(pBegin, pEnd);
    _pData = pBegin;
  }

  void reset() {
    WBuffer::reset();
    _pData = _pBegin;
  }

  Size data() const {
    return _pDataEnd - _pData;
  }

  iterator dataBegin() const {
    return _pData;
  }

  Size beginSpace() const {
    return _pData - _pBegin;
  }

  iterator move() {
    Size uData = data();
    if (uData && _pData > _pBegin) {
        std::memmove(_pBegin, _pData, uData);
        _pData = _pBegin;
        return _pDataEnd = _pData + uData;
      }
    return nullptr;
  }

  iterator move(const_iterator pData, const_iterator pDataEnd) {
    Size uData = pDataEnd - pData;
    assert(uData <= size() && "Invalid arguments");
    std::memmove(_pBegin, pData, uData);
    _pData = _pBegin;
    return _pDataEnd = _pData + uData;
  }
    // return read size
  Size read(iterator pBuffer, Size uBuffer) {
    Size uData = data();
    if (uData > uBuffer)
      uData = uBuffer;

    std::memcpy(pBuffer, _pData, uData);
    _pData += uData;
    return uData;
  }

  U8 readC(C &c) {
    if (_pData < _pDataEnd) {
        c = *_pData++;
        return 1;
      }
    return 0;
  }

  U8 readU8(U8 &u8) {
    if (_pData < _pDataEnd) {
        u8 = *_pData++;
        return 1;
      }
    return 0;
  }

  U8 readU16(U16 &u16) {
    iterator pData = _pData + 2;
    if (pData <= _pDataEnd) {
        u16 = *(U16*)_pData;
        _pData = pData;
        return 2;
      }
    return 0;
  }

  U8 readU32(U32 &u32) {
    iterator pData = _pData + 4;
    if (pData <= _pDataEnd) {
        u32 = *(U32*)_pData;
        _pData = pData;
        return 4;
      }
    return 0;
  }

  U8 readU64(U64 &u64) {
    iterator pData = _pData + 8;
    if (pData <= _pDataEnd) {
        u64 = *(U64*)_pData;
        _pData = pData;
        return 8;
      }
    return 0;
  }

private:
  iterator _pData = nullptr;
};

#endif