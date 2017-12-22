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

#ifndef _CODESMITHY_CPP_PREPROCESSOR_CORE_PREPROCESSORCALLBACKS_H_
#define _CODESMITHY_CPP_PREPROCESSOR_CORE_PREPROCESSORCALLBACKS_H_

#include "PreprocessingToken.h"
#include <vector>

namespace CodeSmithy
{
namespace Cpp
{

class PreprocessorCallbacks
{
public:
    PreprocessorCallbacks();
    virtual ~PreprocessorCallbacks();

    // In general there will only be one token passed at a time but when
    // the tokens are the result of a macro expansion they will be passed
    // in together.
    virtual void onTokens(const std::vector<PreprocessingToken>& tokens);
    virtual bool onUnexpectedCharacter(char c);
};

}
}

#include "linkoptions.h"

#endif
