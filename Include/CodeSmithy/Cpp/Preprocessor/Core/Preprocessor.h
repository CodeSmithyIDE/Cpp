/*
    Copyright (c) 2008-2017 Xavier Leclercq

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

#ifndef _CODESMITHY_CPP_PREPROCESSOR_CORE_PREPROCESSOR_H_
#define _CODESMITHY_CPP_PREPROCESSOR_CORE_PREPROCESSOR_H_

#include "PreprocessorContext.h"
#include "SourceFile.h"
#include "PreprocessingToken.h"
#include "PreprocessingDirective.h"
#include "PreprocessorCallbacks.h"
#include "TranslationUnit.h"
#include <istream>
#include <memory>

namespace CodeSmithy
{
namespace Cpp
{

class Preprocessor
{
public:
    Preprocessor(std::istream& input);
    ~Preprocessor();

    void run(PreprocessorCallbacks& callbacks);
    void run(TranslationUnit& translationUnit);

    const PreprocessorContext& context() const;

private:
    enum EState
    {
        eNormal,
        eDirective
    };

    PreprocessingToken readWhiteSpaceCharacters();
    PreprocessingToken readIdentifier();
    PreprocessingToken readOpOrPunctuator();
    PreprocessingToken readNumber();
    PreprocessingToken readCharacterLiteral();
    PreprocessingToken readStringLiteral();

private:
    PreprocessorContext m_context;
    SourceFile m_source;
    static const int m_bufferSize = 1024;
    char m_buffer[m_bufferSize];
    size_t m_position;
    EState m_state;
    // When encountering a directive several tokens need to be read 
    // to parse the directive. m_directive is build progressively as
    // the tokens are extracted. So its contents will be incomplete 
    // until the last token of the directive is extracted.
    std::shared_ptr<PreprocessingDirective> m_directive;
};

}
}

#include "linkoptions.h"

#endif
