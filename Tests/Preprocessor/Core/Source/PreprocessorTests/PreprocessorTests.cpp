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

#include "PreprocessorTests.h"
#include "TestCallbacks.h"
#include "CodeSmithy/Cpp/Preprocessor/Core/Preprocessor.h"
#include <fstream>

void AddPreprocessorTests(TestHarness& theTestHarness)
{
    TestSequence& preprocessorTestSequence = theTestHarness.appendTestSequence("Preprocessor tests");

    new HeapAllocationErrorsTest("Creation test 1", PreprocessorCreationTest1, preprocessorTestSequence);

    new FileComparisonTest("run test 1", PreprocessorRunTest1, preprocessorTestSequence);
    new FileComparisonTest("run test 2", PreprocessorRunTest2, preprocessorTestSequence);

    new FileComparisonTest("macro test 1", PreprocessorMacroTest1, preprocessorTestSequence);
}

TestResult::EOutcome PreprocessorCreationTest1(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "MinimalMainFunction1.cpp");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::Preprocessor preprocessor(input);

    return TestResult::ePassed;
}

TestResult::EOutcome PreprocessorRunTest1(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "EmptyFile.cpp");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PreprocessorRunTest1.txt");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::Preprocessor preprocessor(input);

    TestCallbacks callbacks;
    preprocessor.run(callbacks);
    callbacks.write(outputPath);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PreprocessorRunTest1.txt");

    if (callbacks.unexpectedCharactersCount() == 0)
    {
        result = TestResult::ePassed;
    }

    return result;
}

TestResult::EOutcome PreprocessorRunTest2(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "MinimalMainFunction1.cpp");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PreprocessorRunTest2.txt");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::Preprocessor preprocessor(input);

    TestCallbacks callbacks;
    preprocessor.run(callbacks);
    callbacks.write(outputPath);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PreprocessorRunTest2.txt");

    if (callbacks.unexpectedCharactersCount() == 0)
    {
        result = TestResult::ePassed;
    }

    return result;
}

TestResult::EOutcome PreprocessorMacroTest1(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "HeaderGuards1.h");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PreprocessorMacroTest1.txt");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::Preprocessor preprocessor(input);

    TestCallbacks callbacks;
    preprocessor.run(callbacks);
    callbacks.write(outputPath);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PreprocessorMacroTest1.txt");

    if (callbacks.unexpectedCharactersCount() == 0)
    {
        result = TestResult::ePassed;
    }

    return result;
}
