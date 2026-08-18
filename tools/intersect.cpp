//License: Public Domain
//Author: elf

extern "C" {
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
}

typedef unsigned char E8;
typedef char C;
typedef signed char I8;
typedef signed short I16;
typedef signed int I32;
typedef signed long I64;
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long U64;

#if defined(__x86_64__) || defined(_WIN64)
typedef U64 U;
typedef I64 I;
#else
typedef U32 U;
typedef I32 I;
#endif

typedef enum {
  eOk,
  eArgument,
  eMemory,
  eIoOpen,
  eIoRead,
  eIoWrite,
  eIoState,
  eIoMap,
  eIoSync,
  eFormat,
  eErrors
} Error;

#include <cassert>
#include <iostream>
#include <cstring>
#include <string_view>
typedef std::basic_string_view<C> SV;

#include <unordered_set>
typedef std::unordered_set<SV> SSet;

#include "Buffer.h"
#include "WFile.h"

class Intersector {
  C *_pBuffer = nullptr;
  Buffer _readBuffer;
  WBuffer _bTokens;
  SSet _lefts;
  SSet _commons;
  SSet _rights;
  WFile _fOut;

public:
  Intersector () {
  }

  ~Intersector() {
    delete []_pBuffer;
  }

  E8 createBuffers(U uReadBuffer, U uWriteBuffer, U uTokenBuffer) {
    assert(!_pBuffer && "Buffer not null");
    try {
        U uBytes = uReadBuffer + uWriteBuffer + uTokenBuffer;
        C *pBuffer = new C[uBytes];
        C *pBegin = pBuffer;
        C *pEnd = pBegin + uReadBuffer;
        _readBuffer.set(pBegin, pEnd);
        pBegin = pEnd;
        pEnd = pBegin + uWriteBuffer;
        _fOut.setBuffer(pBegin, pEnd);
        pBegin = pEnd;
        pEnd = pBegin + uTokenBuffer;
        _bTokens.set(pBegin, pEnd);
        _pBuffer = pBuffer;
        return eOk;
      }
    catch(...) {
        std::cerr << "Error: Memory insufficient!" << std::endl;
        return eMemory;
      }
  }

  E8 excludeLine(const C *pBegin, const C *pEnd) {
    E8 e = eOk;
    SV svLine(pBegin, pEnd);
    auto nh = _lefts.extract(svLine);
    if (nh) {
        try {
            _commons.insert(std::move(nh));
          }
        catch(...) {
            std::cerr << "Error: Memory insufficient!" << std::endl;
            return eMemory;
          }
      }
    else {
        if (_commons.find(svLine) == _commons.cend())
          e = addRight(svLine);
      }
    return e;
  }

  E8 addLeft(const SV &svToken) {
    try {
        auto pr = _lefts.emplace(svToken);
        if (!pr.second)
          return eOk;

        U uToken = svToken.size();
        // RETRY:
        U uSpace = uToken + 1;
        if (uSpace <= _bTokens.endSpace()) {
            C *pToken = _bTokens.push(uSpace);
            std::memcpy(pToken, svToken.begin(), uToken);
            pToken[uToken] = 0;
            const_cast<SV&>(*pr.first) = SV(pToken, uToken);
            return eOk;
          }
      } catch (...) {
      }
    std::cerr << "Error: Memory insufficient!" << std::endl;
    return eMemory;
  }

  E8 addRight(const SV &svToken) {
    try {
        auto pr = _rights.emplace(svToken);
        if (!pr.second)
          return eOk;

        U uToken = svToken.size();
        // RETRY:
        U uSpace = uToken + 1;
        if (uSpace <= _bTokens.endSpace()) {
            C *pToken = _bTokens.push(uSpace);
            std::memcpy(pToken, svToken.begin(), uToken);
            pToken[uToken] = 0;
            const_cast<SV&>(*pr.first) = SV(pToken, uToken);
            return eOk;
          }
        // E8 e = dump();
        // if (!e)
        //   goto RETRY;
        // return e;
      } catch (...) {
      }
    std::cerr << "Error: Memory insufficient!" << std::endl;
    return eMemory;
  }

  E8 loadLeft(const char *pFileName) {
    int fd = open(pFileName, O_RDONLY);
    if (fd >= 0) {
        E8 e = eOk;
        struct stat meta;
        if (fstat(fd, &meta) == 0) {
            const C *pBegin = (C*)mmap(0, meta.st_size, PROT_READ, MAP_SHARED, fd, 0);
            if(pBegin != MAP_FAILED) {
                e = loadLeft(pBegin, pBegin + meta.st_size);
                munmap((void*)pBegin, meta.st_size);
              }
            else {
                if (!meta.st_size) {
                    std::cerr << "Warning: File is empty" << std::endl;
                    e = eOk;
                  }
                else {
                    std::cerr << "Error: Map file failed" << std::endl;
                    e = eIoMap;
                  }
                std::cerr << "File: " << pFileName << std::endl;
              }
          }
        else {
            std::cerr << "Error: Read file meta information failed" << std::endl
                      << "File: " << pFileName << std::endl;
            e = eIoState;
          }
        close(fd);
        return e;
      }
    std::cerr << "Error: Open input file failed" << std::endl
              << "File: " << pFileName << std::endl;
    return eIoOpen;
  }

  E8 excludeFile(const char *pFileName) {
    int fd = open(pFileName, O_RDONLY);
    if (fd >= 0) {
        // E8 e = excludeFD(fd);
        E8 e = eOk;
        struct stat meta;
        if (fstat(fd, &meta) == 0) {
            const C *pBegin = (C*)mmap(0, meta.st_size, PROT_READ, MAP_SHARED, fd, 0);
            if(pBegin != MAP_FAILED) {
                e = exclude(pBegin, pBegin + meta.st_size);
                munmap((void*)pBegin, meta.st_size);
              }
            else {
                if (!meta.st_size) {
                    std::cerr << "Warning: File is empty" << std::endl;
                    e = eOk;
                  }
                else {
                    std::cerr << "Error: Map file failed" << std::endl;
                    e = eIoMap;
                  }
                std::cerr << "File: " << pFileName << std::endl;
              }
          }
        else {
            std::cerr << "Error: Read file meta information failed" << std::endl
                      << "File: " << pFileName << std::endl;
            e = eIoState;
          }
        close(fd);
        return e;
      }
    std::cerr << "Error: Open input file failed" << std::endl
              << "File: " << pFileName << std::endl;
    return eIoOpen;
  }

  E8 loadLeft(const C *pBegin, const C *pEnd) {
    if (*pBegin == 0xEF && pBegin[1] == 0xBB && pBegin[2] == 0xBF)
      pBegin += 3;

    if (pEnd[-1] != '\n') {
        std::cerr << "Error: Expected line-feed(0x0A) at the end of file" << std::endl;
        return eFormat;
      }
    E8 e = eOk;
    const C *p = pBegin;
    do {
        const C *pLine = p;
        while (*p != '\n')
          ++p;

        U64 uLine = p - pLine;
        if (uLine) {
            e = addLeft(SV{pLine, uLine});
            if (e)
              break;
          }
      } while (++p != pEnd);
    return e;
  }

  static I read(int fIn, C *pBuffer, U uBuffer) {
    I iResult = 0;
    while (uBuffer) {
        I iRead = ::read(fIn, pBuffer, uBuffer);
        if (iRead > 0) {
            iResult += iRead;
            pBuffer += iRead;
            uBuffer -= iRead;
          }
        else if (!iRead)
          break;
        else {
            if (errno != EINTR) {
#if defined(__x86_64__) || defined(_WIN64)
                iResult |= 0x8000000000000000;
#else
                iResult |= 0x80000000;
#endif
                break;
              }
          }
      }
    return iResult;
  }

  E8 excludeFD(int fIn) {
    E8 e = eOk;
    const C *pEnd = _readBuffer.end();
    const C *pBegin = _readBuffer.begin();
    const C *p = _readBuffer.dataEnd();
    while (1) {
      READ:
        I iRead = read(fIn, const_cast<C*>(p), pEnd - p);
        if (iRead > 0) {
            const C *pDataEnd = p + iRead;
            p = pBegin;
            e = excludeLines(p, pDataEnd);
            if (!e) {
                if (pBegin < p) {
                    if (p < pDataEnd) {
                        U uData = pDataEnd - p;
                        std::memmove(const_cast<C*>(pBegin), p, uData);
                        p = pBegin + uData;
                        goto READ;
                      }
                    p = pBegin;
                  }
              }
            else
              return e;
          }
        else {
            if (!iRead) {
                if (pBegin < p)
                  e = excludeLine(pBegin, p);
              }
            else {
                std::cerr << "Error(" << errno << "): Read stdin failed" << std::endl;
                e = eIoRead;
              }
            break;
          }
      }
    return e;
  }

  E8 exclude(const C *pBegin, const C *pEnd) {
    const C *p = pBegin;
    if (*p == 0xEF && p[1] == 0xBB && p[2] == 0xBF)
      p += 3;

    E8 e = excludeLines(p, pEnd);
    if (!e) {
        if (p < pEnd)
          e = excludeLine(p, pEnd);
      }
    return e;
  }

         //Exclude lines until reach last line end
         // p will pass last line end
  E8 excludeLines(const C *&p, const C *pEnd) {
    const C *pREnd = p - 1;

    const C *pLineEnd = pEnd;
    do {
        if (--pLineEnd == pREnd)
          return eOk;
      } while (*pLineEnd != '\n');

    do {
        const C *pLine = p;
        while (*p != '\n')
          ++p;

        if (p > pLine) {
            E8 e = excludeLine(pLine, p);
            if (e)
              return e;
          }
      } while (p++ != pLineEnd);
    return eOk;
  }

  E8 printLine(const SV &svLine) {
    return _fOut.writeLine(svLine.data(), svLine.size());
  }

  E8 dump(const char *pFileName, SSet &tokens) {
    E8 e = _fOut.create(pFileName);
    if (!e) {
        if (tokens.size()) {
            for (const auto &svLine : tokens) {
                e = printLine(svLine);
                if (e)
                  break;
              }
            if (!e) {
                e = _fOut.flush();
                if (!e)
                  tokens.clear();
              }
          }
        _fOut.close();
      }
    return e;
  }

  E8 dump(const char *pCommonFile, const char *pLeftFile, const char *pRightFile) {
    E8 e = eOk;
    std::cerr << "Commons(" << _commons.size() << ")" << std::endl;
    e = dump(pCommonFile, _commons);
    if (!e) {
        std::cerr << "Lefts(" << _lefts.size() << ") ........................" << std::endl;
        e = dump(pLeftFile, _lefts);
        if (!e) {
            std::cerr << "Rights(" << _rights.size() << ") ........................" << std::endl;
            e = dump(pRightFile, _rights);
          }
      }
    _bTokens.reset();
    std::cerr << "Done" << std::endl;
    return e;
  }
};

static Intersector _intersector;
#define READ_BUFFER_BYTES (16 * 0x100000)
#define WRITE_BUFFER_BYTES (16 * 0x100000)
#define TOKEN_BUFFER_BYTES (8UL * 0x40000000UL)

int main(int argc, char *argv[]) {
  if (argc < 6) {
      dprintf(STDERR_FILENO, "Usage:\n  %s out_common_file/- out_left_file/- out_right_file/- in_left_file in_right_file0/-[, in_right_file1/- ...]\n", argv[0]);
      return eArgument;
    }
  E8 e = _intersector.createBuffers(READ_BUFFER_BYTES, WRITE_BUFFER_BYTES, TOKEN_BUFFER_BYTES);
  if (!e) {
      char **ppFileName = argv;
      char **ppFileNameEnd = argv + argc;
      ppFileName += 4;
      e = _intersector.loadLeft(*ppFileName++);
      if (!e) {
          do {
              const char *pFileName = *ppFileName;
              if (pFileName[0] != '-' || pFileName[1])
                e = _intersector.excludeFile(*ppFileName);
              else
                e = _intersector.excludeFD(STDIN_FILENO);

              if (e)
                break;
            } while (++ppFileName != ppFileNameEnd);

          if (!e)
            e = _intersector.dump(argv[1], argv[2], argv[3]);
        }
    }
  return e;
}
