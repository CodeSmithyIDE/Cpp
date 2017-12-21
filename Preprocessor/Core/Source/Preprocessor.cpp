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

#include "Preprocessor.h"
#include "TranslationUnitBuilderCallbacks.h"

namespace CodeSmithy
{
namespace Cpp
{

Preprocessor::Preprocessor(std::istream& input)
    : m_source(input), m_position(0), m_state(eNormal)
{
}

Preprocessor::~Preprocessor()
{
}

void Preprocessor::run(PreprocessorCallbacks& callbacks)
{
    m_position = 0;
    m_state = eNormal;
    while (m_source.read(&m_buffer[0], (m_bufferSize - 1)))
    {
    }
    if (m_source.eof())
    {
        m_buffer[(unsigned int)m_source.gcount()] = '\0';
        char c = m_buffer[m_position];
        while (c != 0)
        {
            PreprocessorToken token(PreprocessorToken::eInvalid);

            if (((c >= 'a') && (c <= 'z')) ||
                ((c >= 'A') && (c <= 'Z')) ||
                (c == '_'))
            {
                if ((c == 'L') && (m_buffer[m_position + 1] == '\''))
                {
                    // This is a special case that needs to be checked first
                    // to avoid confusion with an identifier
                    token = readCharacterLiteral();
                }
                else if ((c == 'L') && (m_buffer[m_position + 1] == '\"'))
                {
                    // This is a special case that needs to be checked first
                    // to avoid confusion with an identifier
                    token = readStringLiteral();
                }
                else
                {
                    token = readIdentifier();
                }
            }
            else if ((c == ' ') || (c == '\r') || (c == '\n') || (c == '\t'))
            {
                token = readWhiteSpaceCharacters();
            }
            else if (c == '\'')
            {
                token = readCharacterLiteral();
            }
            else if (c == '\"')
            {
                token = readStringLiteral();
            }
            else if ((c == ';') || (c == '(') || (c == ')') ||
                (c == '[') || (c == ']') || (c == '{') || (c == '}') ||
                (c == '*') || (c == ',') || (c == '=') || (c == '+') ||
                (c == '-') || (c == '/') || (c == '#'))
            {
                token = readOpOrPunctuator();
            }
            else if ((c >= '0') && (c <= '9'))
            {
                token = readNumber();
            }

            if (m_state == eNormal)
            {
                if (token.type() == PreprocessorToken::eInvalid)
                {
                    bool skip = callbacks.onUnexpectedCharacter(c);
                    if (skip)
                    {
                        ++m_position;
                    }
                    else
                    {
                        break;
                    }
                }
                else if ((token.type() == PreprocessorToken::eOpOrPunctuator) && (token.text() == "#"))
                {
                    m_state = eDirective;
                    m_directive = std::make_shared<PreprocessingDirective>();
                }
                else
                {
                    callbacks.onToken(token);
                }
            }
            else if (m_state == eDirective)
            {
                if ((token.type() == PreprocessorToken::eWhiteSpaceCharacters) &&
                    (token.text() == "\n"))
                {
                    // We have encountered the end of the directive
                    m_state = eNormal;
                }
                else
                {
                }
            }

            c = m_buffer[m_position];
        }
    }
}

void Preprocessor::run(TranslationUnit& translationUnit)
{
    TranslationUnitBuilderCallbacks callbacks(translationUnit);
    run(callbacks);
}

const PreprocessorContext& Preprocessor::context() const
{
    return m_context;
}

PreprocessorToken Preprocessor::readWhiteSpaceCharacters()
{
    PreprocessorToken result(PreprocessorToken::eWhiteSpaceCharacters);

    size_t start = m_position;

    // As per the standard new-line characters must be retained but
    // sequence of other white-space characters may or may not be 
    // concatenated. So to make things easier we create one token
    // for each new-line but group the sequences of other white-space
    // characters into a single token.
    char c = m_buffer[m_position];
    if (c == '\n')
    {
        ++m_position;
    }
    else
    {
        while ((c == ' ') || (c == '\r') || (c == '\t'))
        {
            c = m_buffer[++m_position];
        }
    }

    result.setText(std::string(&m_buffer[start], m_position - start));

    return result;
}

PreprocessorToken Preprocessor::readIdentifier()
{
    PreprocessorToken result(PreprocessorToken::eIdentifier);

    size_t start = m_position;

    char c = m_buffer[m_position];
    while (((c >= 'a') && (c <= 'z')) ||
        ((c >= 'A') && (c <= 'Z')) ||
        (c == '_'))
    {
        c = m_buffer[++m_position];
    }

    result.setText(std::string(&m_buffer[start], m_position - start));

    return result;
}

PreprocessorToken Preprocessor::readOpOrPunctuator()
{
    PreprocessorToken result(PreprocessorToken::eOpOrPunctuator);

    result.setText(std::string(&m_buffer[m_position], 1));
    ++m_position;

    return result;
}

PreprocessorToken Preprocessor::readNumber()
{
    PreprocessorToken result(PreprocessorToken::eNumber);

    size_t start = m_position;

    char c = m_buffer[m_position];
    while ((c >= '0') && (c <= '9'))
    {
        c = m_buffer[++m_position];
    }

    result.setText(std::string(&m_buffer[start], m_position - start));

    return result;
}

PreprocessorToken Preprocessor::readCharacterLiteral()
{
    PreprocessorToken result(PreprocessorToken::eCharacterLiteral);

    size_t start = m_position;

    char c = m_buffer[m_position];
    // The readCharacterLiteral function is only called if we
    // encountered "'" or "L'" so we check which one it is
    // and increase the position accordingly.
    if (c == 'L')
    {
        m_position += 2;
    }
    else
    {
        ++m_position;
    }

    c = m_buffer[m_position];
    while (c != '\'')
    {
        c = m_buffer[++m_position];
    }

    // We do want to include the closing '\'' in the token
    ++m_position;

    result.setText(std::string(&m_buffer[start], m_position - start));

    return result;
}

PreprocessorToken Preprocessor::readStringLiteral()
{
    PreprocessorToken result(PreprocessorToken::eStringLiteral);

    size_t start = m_position;

    char c = m_buffer[m_position];
    // The readStringLiteral function is only called if we
    // encountered "\"" or "L\"" so we check which one it is
    // and increase the position accordingly.
    if (c == 'L')
    {
        m_position += 2;
    }
    else
    {
        ++m_position;
    }

    c = m_buffer[m_position];
    while (c != '\"')
    {
        c = m_buffer[++m_position];
    }

    // We do want to include the closing '\'' in the token
    ++m_position;

    result.setText(std::string(&m_buffer[start], m_position - start));

    return result;
}

}
}
