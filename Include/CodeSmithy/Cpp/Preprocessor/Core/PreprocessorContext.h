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

#ifndef _CODESMITHY_CPP_PREPROCESSOR_CORE_PREPROCESSORCONTEXT_H_
#define _CODESMITHY_CPP_PREPROCESSOR_CORE_PREPROCESSORCONTEXT_H_

#include "PreprocessingDirective.h"
#include <map>
#include <string>
#include <memory>

namespace CodeSmithy
{
namespace Cpp
{

// The preprocessor allows the definitioon of macros.
// The PreprocessorContext class is used to stored the
// list of defined macros.
class PreprocessorContext
{
public:
    PreprocessorContext();
    ~PreprocessorContext();

    std::shared_ptr<PreprocessingDirective>& operator[](const std::string& key);
    std::map<std::string, std::shared_ptr<PreprocessingDirective> >::const_iterator end() const;
    std::map<std::string, std::shared_ptr<PreprocessingDirective> >::size_type size() const;
    std::map<std::string, std::shared_ptr<PreprocessingDirective> >::const_iterator find(const std::string& key) const;

private:
    // Only #define directive should end up in the context
    std::map<std::string, std::shared_ptr<PreprocessingDirective> > m_directives;
};

}
}

#include "linkoptions.h"

#endif
