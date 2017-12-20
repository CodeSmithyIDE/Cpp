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

#include "CppSourceFileTests.h"
#include "CodeSmithy/Cpp/Preprocessor/Core/CppSourceFile.h"
#include <fstream>

void AddCppSourceFileTests(TestHarness& theTestHarness)
{
    TestSequence& cppSourceFileTestSequence = theTestHarness.appendTestSequence("CppSourceFile tests");

    new HeapAllocationErrorsTest("Creation test 1", CppSourceFileCreationTest1, cppSourceFileTestSequence);

    new HeapAllocationErrorsTest("read test 1", CppSourceFileReadTest1, cppSourceFileTestSequence);
    new HeapAllocationErrorsTest("read test 2", CppSourceFileReadTest2, cppSourceFileTestSequence);
}

TestResult::EOutcome CppSourceFileCreationTest1(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "MinimalMainFunction1.cpp");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::CppSourceFile source(input);

    return TestResult::ePassed;
}

TestResult::EOutcome CppSourceFileReadTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "MinimalMainFunction1.cpp");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::CppSourceFile source(input);

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

TestResult::EOutcome CppSourceFileReadTest2(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "EmptyFile.cpp");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::CppSourceFile source(input);

    char buffer[3];
    if ((!source.read(buffer, 3)) && (input.gcount() == 0))
    {
        result = TestResult::ePassed;
    }

    return result;
}
