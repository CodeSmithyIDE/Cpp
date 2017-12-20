/*
    Copyright (c) 2017 Xavier Leclercq

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#ifndef _CODESMITHY_CPP_PREPROCESSOR_CORE_SOURCEFILE_H_
#define _CODESMITHY_CPP_PREPROCESSOR_CORE_SOURCEFILE_H_

#include <istream>

namespace CodeSmithy
{
namespace Cpp
{

// This is a source file as defined in the standard so it can be a .h or .cpp, .cxx.
// Note that a header in the standard is defined as something else than a header file
// (see note 158 in section 17.4.1.2)
class CppSourceFile
{
public:
    CppSourceFile(std::istream& input);
    ~CppSourceFile();

    std::streamsize gcount() const;
    CppSourceFile& read(char* s, std::streamsize n);
    bool eof() const;
    explicit operator bool() const;

private:
    std::istream& m_input;
};

}
}

#include "linkoptions.h"

#endif
