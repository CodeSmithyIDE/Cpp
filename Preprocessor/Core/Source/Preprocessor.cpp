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
{
    m_stateStack.push_back(State(input));
}

Preprocessor::~Preprocessor()
{
}

void Preprocessor::run(PreprocessingIncludeDirectiveResolver& includeResolver, 
                       PreprocessorCallbacks& callbacks)
{
    State& state = m_stateStack.back();

    while (state.m_source.read(&state.m_buffer[0], (state.m_bufferSize - 1)))
    {
    }
    if (state.m_source.eof())
    {
        state.m_buffer[(unsigned int)state.m_source.gcount()] = '\0';
        char c = state.m_buffer[state.m_position];
        while (c != 0)
        {
            PreprocessingToken token(PreprocessingToken::eInvalid);

            if (((c >= 'a') && (c <= 'z')) ||
                ((c >= 'A') && (c <= 'Z')) ||
                (c == '_'))
            {
                if ((c == 'L') && (state.m_buffer[state.m_position + 1] == '\''))
                {
                    // This is a special case that needs to be checked first
                    // to avoid confusion with an identifier
                    token = readCharacterLiteral(state.m_buffer, state.m_position);
                }
                else if ((c == 'L') && (state.m_buffer[state.m_position + 1] == '\"'))
                {
                    // This is a special case that needs to be checked first
                    // to avoid confusion with an identifier
                    token = readStringLiteral(state.m_buffer, state.m_position);
                }
                else
                {
                    token = readIdentifier(state.m_buffer, state.m_position);
                }
            }
            else if ((c == ' ') || (c == '\r') || (c == '\n') || (c == '\t'))
            {
                token = readWhiteSpaceCharacters(state.m_buffer, state.m_position);
            }
            else if (c == '\'')
            {
                token = readCharacterLiteral(state.m_buffer, state.m_position);
            }
            else if (c == '\"')
            {
                token = readStringLiteral(state.m_buffer, state.m_position);
            }
            else if ((c == ';') || (c == '(') || (c == ')') ||
                (c == '[') || (c == ']') || (c == '{') || (c == '}') ||
                (c == '*') || (c == ',') || (c == '=') || (c == '+') ||
                (c == '-') || (c == '/') || (c == '#'))
            {
                token = readOpOrPunctuator(state.m_buffer, state.m_position);
            }
            else if ((c >= '0') && (c <= '9'))
            {
                token = readNumber(state.m_buffer, state.m_position);
            }

            if (state.m_mode == eNormal)
            {
                if (token.type() == PreprocessingToken::eInvalid)
                {
                    bool skip = callbacks.onUnexpectedCharacter(c);
                    if (skip)
                    {
                        ++state.m_position;
                    }
                    else
                    {
                        break;
                    }
                }
                else if ((token.type() == PreprocessingToken::eOpOrPunctuator) && (token.text() == "#"))
                {
                    state.m_mode = eDirective;
                    state.m_directive = std::make_shared<PreprocessingDirective>(PreprocessingDirective::eInvalid);
                }
                else
                {
                    std::vector<PreprocessingToken> tokens;
                    tokens.push_back(token);
                    callbacks.onTokens(tokens);
                }
            }
            else if (state.m_mode == eDirective)
            {
                if ((token.type() == PreprocessingToken::eWhiteSpaceCharacters) &&
                    (token.text() == "\n"))
                {
                    // We have encountered the end of the directive
                    if (state.m_directive->type() == PreprocessingDirective::eDefine)
                    {
                        m_context[state.m_directive->identifier().text()] = state.m_directive;
                    }
                    else if (state.m_directive->type() == PreprocessingDirective::eInclude)
                    {
                        const std::vector<PreprocessingToken>& tokens = state.m_directive->tokens();
                        if (tokens.size() > 0)
                        {
                            if (tokens[0].type() == PreprocessingToken::eStringLiteral)
                            {
                                // The string literal includes the opening and closing quotes, so
                                // remove them
                                const std::string& text = tokens[0].text();
                                std::shared_ptr<std::istream> includeInput = includeResolver.resolve(text.substr(1, text.size() - 2));
                                if (includeInput)
                                {
                                    Preprocessor includePreprocessor(*includeInput);
                                    includePreprocessor.run(includeResolver, callbacks);
                                }
                                else
                                {
                                    bool skip = callbacks.onError();
                                    if (skip)
                                    {
                                        ++state.m_position;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    state.m_mode = eNormal;
                }
                else if (token.type() == PreprocessingToken::eIdentifier)
                {
                    if (state.m_directive->type() == PreprocessingDirective::eInvalid)
                    {
                        if (token.text() == "define")
                        {
                            state.m_directive->setType(PreprocessingDirective::eDefine);
                        }
                        else if (token.text() == "include")
                        {
                            state.m_directive->setType(PreprocessingDirective::eInclude);
                        }
                    }
                    else if ((state.m_directive->type() == PreprocessingDirective::eDefine) &&
                        (state.m_directive->identifier().type() == PreprocessingToken::eInvalid))
                    {
                        state.m_directive->setIdentifier(token);
                    }
                }
                else if(token.type() == PreprocessingToken::eStringLiteral)
                {
                    if (state.m_directive->type() == PreprocessingDirective::eInclude)
                    {
                        state.m_directive->appendToken(token);
                    }
                }
            }

            c = state.m_buffer[state.m_position];
        }
    }
}

void Preprocessor::run(PreprocessingIncludeDirectiveResolver& includeResolver,
                       TranslationUnit& translationUnit)
{
    TranslationUnitBuilderCallbacks callbacks(translationUnit);
    run(includeResolver, callbacks);
}

const PreprocessorContext& Preprocessor::context() const
{
    return m_context;
}

PreprocessingToken Preprocessor::readWhiteSpaceCharacters(const char* buffer, size_t& position)
{
    PreprocessingToken result(PreprocessingToken::eWhiteSpaceCharacters);

    size_t start = position;

    // As per the standard new-line characters must be retained but
    // sequence of other white-space characters may or may not be 
    // concatenated. So to make things easier we create one token
    // for each new-line but group the sequences of other white-space
    // characters into a single token.
    char c = buffer[position];
    if (c == '\n')
    {
        ++position;
    }
    else
    {
        while ((c == ' ') || (c == '\r') || (c == '\t'))
        {
            c = buffer[++position];
        }
    }

    result.setText(std::string(&buffer[start], position - start));

    return result;
}

PreprocessingToken Preprocessor::readIdentifier(const char* buffer, size_t& position)
{
    PreprocessingToken result(PreprocessingToken::eIdentifier);

    size_t start = position;

    char c = buffer[position];
    while (((c >= 'a') && (c <= 'z')) ||
        ((c >= 'A') && (c <= 'Z')) ||
        (c == '_'))
    {
        c = buffer[++position];
    }

    result.setText(std::string(&buffer[start], position - start));

    return result;
}

PreprocessingToken Preprocessor::readOpOrPunctuator(const char* buffer, size_t& position)
{
    PreprocessingToken result(PreprocessingToken::eOpOrPunctuator);

    result.setText(std::string(&buffer[position], 1));
    ++position;

    return result;
}

PreprocessingToken Preprocessor::readNumber(const char* buffer, size_t& position)
{
    PreprocessingToken result(PreprocessingToken::eNumber);

    size_t start = position;

    char c = buffer[position];
    while ((c >= '0') && (c <= '9'))
    {
        c = buffer[++position];
    }

    result.setText(std::string(&buffer[start], position - start));

    return result;
}

PreprocessingToken Preprocessor::readCharacterLiteral(const char* buffer, size_t& position)
{
    PreprocessingToken result(PreprocessingToken::eCharacterLiteral);

    size_t start = position;

    char c = buffer[position];
    // The readCharacterLiteral function is only called if we
    // encountered "'" or "L'" so we check which one it is
    // and increase the position accordingly.
    if (c == 'L')
    {
        position += 2;
    }
    else
    {
        ++position;
    }

    c = buffer[position];
    while (c != '\'')
    {
        c = buffer[++position];
    }

    // We do want to include the closing '\'' in the token
    ++position;

    result.setText(std::string(&buffer[start], position - start));

    return result;
}

PreprocessingToken Preprocessor::readStringLiteral(const char* buffer, size_t& position)
{
    PreprocessingToken result(PreprocessingToken::eStringLiteral);

    size_t start = position;

    char c = buffer[position];
    // The readStringLiteral function is only called if we
    // encountered "\"" or "L\"" so we check which one it is
    // and increase the position accordingly.
    if (c == 'L')
    {
        position += 2;
    }
    else
    {
        ++position;
    }

    c = buffer[position];
    while (c != '\"')
    {
        c = buffer[++position];
    }

    // We do want to include the closing '\'' in the token
    ++position;

    result.setText(std::string(&buffer[start], position - start));

    return result;
}

Preprocessor::State::State(std::istream& input)
    : m_source(input), m_position(0), m_mode(eNormal)
{
}

Preprocessor::State::~State()
{
}

}
}
