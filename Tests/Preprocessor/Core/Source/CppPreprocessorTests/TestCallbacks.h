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

#ifndef _CODESMITHY_TEST_CPP_PREPROCESSOR_CORE_CPPPREPROCESSORTESTS_TESTCALLBACKS_H_
#define _CODESMITHY_TEST_CPP_PREPROCESSOR_CORE_CPPPREPROCESSORTESTS_TESTCALLBACKS_H_

#include "CodeSmithy/Cpp/Preprocessor/Core/CppPreprocessorCallbacks.h"
#include <boost/filesystem/path.hpp>

class TestCallbacks : public CodeSmithy::Cpp::CppPreprocessorCallbacks
{
public:
    TestCallbacks();
    ~TestCallbacks() override;

    size_t unexpectedCharactersCount() const;

    void onToken(const CodeSmithy::Cpp::CppPreprocessorToken& token) override;
    bool onUnexpectedCharacter(char c) override;

    void write(const boost::filesystem::path& path);

private:
    static std::string typeToString(CodeSmithy::Cpp::CppPreprocessorToken::EType type);

private:
    std::vector<CodeSmithy::Cpp::CppPreprocessorToken> m_tokens;
    size_t m_unexpectedCharactersCount;
};

#endif
