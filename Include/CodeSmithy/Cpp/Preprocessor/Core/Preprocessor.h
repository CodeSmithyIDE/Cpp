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

#include "PreprocessorSettings.h"
#include "PreprocessorContext.h"
#include "SourceFile.h"
#include "PreprocessingToken.h"
#include "PreprocessingDirective.h"
#include "PreprocessingIncludeDirectiveResolver.h"
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

    void run(PreprocessingIncludeDirectiveResolver& includeResolver, PreprocessorCallbacks& callbacks);
    void run(PreprocessingIncludeDirectiveResolver& includeResolver, TranslationUnit& translationUnit);

    const PreprocessorContext& context() const;

private:
    static PreprocessingToken readWhiteSpaceCharacters(const char* buffer, size_t& position);
    static PreprocessingToken readIdentifier(const char* buffer, size_t& position);
    static PreprocessingToken readOpOrPunctuator(const char* buffer, size_t& position);
    static PreprocessingToken readNumber(const char* buffer, size_t& position);
    static PreprocessingToken readCharacterLiteral(const char* buffer, size_t& position);
    static PreprocessingToken readStringLiteral(const char* buffer, size_t& position);

    enum EMode
    {
        eNormal,
        eDirective
    };

    class State
    {
    public:
        State(std::istream& input);
        State(std::istream& input, std::shared_ptr<std::istream>& includedInput);
        ~State();

        SourceFile m_source;
        static const int m_bufferCapacity = 1024;
        char m_buffer[m_bufferCapacity];
        size_t m_bufferSize;
        size_t m_position;
        EMode m_mode;
        // When encountering a directive several tokens need to be read 
        // to parse the directive. m_directive is build progressively as
        // the tokens are extracted. So its contents will be incomplete 
        // until the last token of the directive is extracted.
        std::shared_ptr<PreprocessingDirective> m_directive;
        // For included files we need to keep the istream object
        // somewhere in scope so we keep it in the state itself. This
        // member is never access directly, only via the SourceFile
        // object.
        std::shared_ptr<std::istream> m_includedInput;
    };

private:
    PreprocessorSettings m_settings;
    // The context is not part of the state because included files
    // do not have a separate nested context, they use the same
    // context as the top file.
    PreprocessorContext m_context;
    // We need a stack of states as we do not want to use recursion to
    // handle include directives.
    std::vector<State> m_stateStack;
};

}
}

#include "linkoptions.h"

#endif
