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

#include "SourceFileTests.h"
#include "CodeSmithy/Cpp/Preprocessor/Core/SourceFile.h"
#include <fstream>

void AddSourceFileTests(TestHarness& theTestHarness)
{
    TestSequence& sourceFileTestSequence = theTestHarness.appendTestSequence("SourceFile tests");

    new HeapAllocationErrorsTest("Creation test 1", SourceFileCreationTest1, sourceFileTestSequence);

    new HeapAllocationErrorsTest("read test 1", SourceFileReadTest1, sourceFileTestSequence);
    new HeapAllocationErrorsTest("read test 2", SourceFileReadTest2, sourceFileTestSequence);
}

TestResult::EOutcome SourceFileCreationTest1(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "MinimalMainFunction1.cpp");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::SourceFile source(input);

    return TestResult::ePassed;
}

TestResult::EOutcome SourceFileReadTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "MinimalMainFunction1.cpp");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::SourceFile source(input);

    char buffer[3];
    if (source.read(buffer, 3))
    {
        if ((source.gcount() == 3) && (strncmp(buffer, "int", 3) == 0))
        {
            result = TestResult::ePassed;
        }
    }

    return result;
}

TestResult::EOutcome SourceFileReadTest2(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "EmptyFile.cpp");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::SourceFile source(input);

    char buffer[3];
    if ((!source.read(buffer, 3)) && (input.gcount() == 0))
    {
        result = TestResult::ePassed;
    }

    return result;
}
