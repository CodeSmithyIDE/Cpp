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

#include "TestCallbacks.h"
#include <fstream>

TestCallbacks::TestCallbacks()
    : m_unexpectedCharactersCount(0)
{
}

TestCallbacks::~TestCallbacks()
{
}

size_t TestCallbacks::unexpectedCharactersCount() const
{
    return m_unexpectedCharactersCount;
}

void TestCallbacks::onToken(const CodeSmithy::Cpp::CppPreprocessorToken& token)
{
    m_tokens.push_back(token);
}

bool TestCallbacks::onUnexpectedCharacter(char c)
{
    ++m_unexpectedCharactersCount;
    return false;
}

void TestCallbacks::write(const boost::filesystem::path& path)
{
    std::ofstream stream(path.c_str());

    for (CodeSmithy::Cpp::CppPreprocessorToken& token : m_tokens)
    {
        stream << typeToString(token.type()) << ": " << token.text() << std::endl;
    }
}

std::string TestCallbacks::typeToString(CodeSmithy::Cpp::CppPreprocessorToken::EType type)
{
    std::string result = "Invalid type";

    switch (type)
    {
    case CodeSmithy::Cpp::CppPreprocessorToken::eWhiteSpaceCharacters:
        result = "white-space";
        break;

    case CodeSmithy::Cpp::CppPreprocessorToken::eIdentifier:
        result = "identifier";
        break;

    case CodeSmithy::Cpp::CppPreprocessorToken::eOpOrPunctuator:
        result = "op-or-punctuator";
        break;

    case CodeSmithy::Cpp::CppPreprocessorToken::eNumber:
        result = "number";
        break;

    default:
        break;
    }

    return result;
}
