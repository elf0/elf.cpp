#ifndef WBUFFER_H
#define WBUFFER_H

class WBuffer {
public:
  typedef U Size;
  typedef C* iterator;
  typedef const C* const_iterator;

         // WBuffer() {}
         // ~WBuffer() {}

  void set(Size uSize, C *pBegin, C *pEnd) {
    _uSize = uSize;
    _pBegin = _pDataEnd = pBegin;
    _pEnd = pEnd;
    assert((pBegin + uSize == pEnd) && "Invalid arguments");
  }

  void reset() {
    _pDataEnd = _pBegin;
  }

  Size size() const {
    return _uSize;
  }

  Size data() const {
    return _pDataEnd - _pBegin;
  }

  iterator begin() const {
    return _pBegin;
  }

  const_iterator end() const {
    return _pEnd;
  }

  iterator dataBegin() const {
    return _pBegin;
  }

  iterator dataEnd() const {
    return _pDataEnd;
  }

  Size beginSpace() const {
    return 0;
  }

  Size endSpace() const {
    return _pEnd - _pDataEnd;
  }

         // stack-style allocate space for external use.
  iterator push(Size uSize) {
    iterator pDataEnd = _pDataEnd + uSize;
    if (pDataEnd <= _pEnd) {
        iterator pNew = _pDataEnd;
        _pDataEnd = pDataEnd;
        return pNew;
      }
    return nullptr;
  }

         // stack-style free space
  void pop(Size uSize) {
    iterator pDataEnd = _pDataEnd - uSize;
    assert(pDataEnd >= _pBegin && "Invalid uSize");
    _pDataEnd = pDataEnd;
  }

         // return unwrote size
  Size write(const_iterator &pData, Size uData) {
    Size uSpace = endSpace();
    if (uData <= uSpace) {
        std::memcpy(_pDataEnd, pData, uData);
        _pDataEnd += uData;
        pData += uData;
        return 0;
      }
    std::memcpy(_pDataEnd, pData, uSpace);
    _pDataEnd = _pEnd;
    pData += uSpace;
    return uData - uSpace;
  }

         // return unwrote size, include c
  Size writeDC(const_iterator &pData, Size uData, C c) {
    Size uSpace = endSpace();
    if (uData < uSpace) {
        // if (uData) {
        std::memcpy(_pDataEnd, pData, uData);
        _pDataEnd += uData;
        // }
        *_pDataEnd++ = c;
        pData += uData;
        return 0;
      }
    // if (uSpace) {
    std::memcpy(_pDataEnd, pData, uSpace);
    _pDataEnd = _pEnd;
    pData += uSpace;
    uData -= uSpace;
    // }
    return uData + 1;
  }

  U8 writeC(C c) {
    if (_pDataEnd < _pEnd) {
        *_pDataEnd++ = c;
        return 0;
      }
    return 1;
  }

  U8 writeU8(U8 u8) {
    if (_pDataEnd < _pEnd) {
        *_pDataEnd++ = u8;
        return 0;
      }
    return 1;
  }

  U8 writeU16(U16 u16) {
    iterator pDataEnd = _pDataEnd + 2;
    if (pDataEnd <= _pEnd) {
        *(U16*)_pDataEnd = u16;
        _pDataEnd = pDataEnd;
        return 0;
      }
    return 2;
  }

  U8 writeU32(U32 u32) {
    iterator pDataEnd = _pDataEnd + 4;
    if (pDataEnd <= _pEnd) {
        *(U32*)_pDataEnd = u32;
        _pDataEnd = pDataEnd;
        return 0;
      }
    return 4;
  }

  U8 writeU64(U64 u64) {
    iterator pDataEnd = _pDataEnd + 8;
    if (pDataEnd <= _pEnd) {
        *(U64*)_pDataEnd = u64;
        _pDataEnd = pDataEnd;
        return 0;
      }
    return 8;
  }

protected:
  Size     _uSize = 0;
  iterator _pBegin = nullptr;
  iterator _pEnd = nullptr;
  iterator _pDataEnd = nullptr;
};

#endif