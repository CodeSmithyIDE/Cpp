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

#include "CppPreprocessorTests.h"
#include "TestCallbacks.h"
#include "CodeSmithy/Cpp/Preprocessor/Core/CppPreprocessor.h"
#include <fstream>

void AddCppPreprocessorTests(TestHarness& theTestHarness)
{
    TestSequence& cppPreprocessorTestSequence = theTestHarness.appendTestSequence("CppPreprocessor tests");

    new HeapAllocationErrorsTest("Creation test 1", CppPreprocessorCreationTest1, cppPreprocessorTestSequence);

    new FileComparisonTest("run test 1", CppPreprocessorRunTest1, cppPreprocessorTestSequence);
    new FileComparisonTest("run test 2", CppPreprocessorRunTest2, cppPreprocessorTestSequence);
}

TestResult::EOutcome CppPreprocessorCreationTest1(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "MinimalMainFunction1.cpp");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::CppPreprocessor preprocessor(input);

    return TestResult::ePassed;
}

TestResult::EOutcome CppPreprocessorRunTest1(FileComparisonTest& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "EmptyFile.cpp");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "CppPreprocessorRunTest1.txt");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::CppPreprocessor preprocessor(input);

    TestCallbacks callbacks;
    preprocessor.run(callbacks);
    callbacks.write(outputPath);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "CppPreprocessorRunTest1.txt");

    return TestResult::ePassed;
}

TestResult::EOutcome CppPreprocessorRunTest2(FileComparisonTest& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "MinimalMainFunction1.cpp");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "CppPreprocessorRunTest2.txt");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::CppPreprocessor preprocessor(input);

    TestCallbacks callbacks;
    preprocessor.run(callbacks);
    callbacks.write(outputPath);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "CppPreprocessorRunTest2.txt");

    return TestResult::ePassed;
}
