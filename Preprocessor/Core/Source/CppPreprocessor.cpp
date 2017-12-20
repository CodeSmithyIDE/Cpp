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

#include "CppPreprocessor.h"

namespace CodeSmithy
{
namespace Cpp
{

CppPreprocessor::CppPreprocessor(std::istream& input)
    : m_source(input)
{
}

CppPreprocessor::~CppPreprocessor()
{
}

void CppPreprocessor::run(CppPreprocessorCallbacks& callbacks)
{
    m_position = 0;
    while (m_source.read(&m_buffer[0], (m_bufferSize - 1)))
    {
    }
    if (m_source.eof())
    {
        m_buffer[(unsigned int)m_source.gcount()] = '\0';
        char c = m_buffer[m_position];
        while (c != 0)
        {
            if ((c >= 'a') && (c <= 'z'))
            {
                CppPreprocessorToken token = readIdentifier();
                callbacks.onToken(token);
            }
            else if (c == ' ')
            {
                CppPreprocessorToken token = readWhiteSpaceCharacters();
                callbacks.onToken(token);
            }
            else if ((c >= 'a') && (c <= 'z'))
            {
                CppPreprocessorToken token = readIdentifier();
                callbacks.onToken(token);
            }
            else if (c == '(')
            {
                CppPreprocessorToken token = readOpOrPunctuator();
                callbacks.onToken(token);
            }
            else
            {
                ++m_position;
            }
            c = m_buffer[m_position];
        }
    }
}

CppPreprocessorToken CppPreprocessor::readWhiteSpaceCharacters()
{
    CppPreprocessorToken result(CppPreprocessorToken::eWhiteSpaceCharacters);

    size_t start = m_position;

    char c = m_buffer[m_position];
    while (c == ' ')
    {
        c = m_buffer[++m_position];
    }

    result.setText(std::string(&m_buffer[start], m_position - start));

    return result;
}

CppPreprocessorToken CppPreprocessor::readIdentifier()
{
    CppPreprocessorToken result(CppPreprocessorToken::eIdentifier);

    size_t start = m_position;

    char c = m_buffer[m_position];
    while ((c >= 'a') && (c <= 'z'))
    {
        c = m_buffer[++m_position];
    }

    result.setText(std::string(&m_buffer[start], m_position - start));

    return result;
}

CppPreprocessorToken CppPreprocessor::readOpOrPunctuator()
{
    CppPreprocessorToken result(CppPreprocessorToken::eOpOrPunctuator);

    result.setText(std::string(&m_buffer[m_position], 1));
    ++m_position;

    return result;
}

}
}
