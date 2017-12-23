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

#include "PreprocessingIncludeDirectiveDefaultResolver.h"
#include <fstream>

namespace CodeSmithy
{
namespace Cpp
{

PreprocessingIncludeDirectiveDefaultResolver::PreprocessingIncludeDirectiveDefaultResolver()
{
}

PreprocessingIncludeDirectiveDefaultResolver::~PreprocessingIncludeDirectiveDefaultResolver()
{
}

void PreprocessingIncludeDirectiveDefaultResolver::appendSearchPath(const std::string& path)
{
    m_searchPaths.push_back(path);
}

std::shared_ptr<std::istream> PreprocessingIncludeDirectiveDefaultResolver::resolve(const std::string& path) const
{
    std::shared_ptr<std::istream> result;

    for (const std::string& dirPath : m_searchPaths)
    {
        result = std::make_shared<std::ifstream>(dirPath + path);
        if (!result->fail())
        {
            break;
        }
    }

    return result;
}

}
}
