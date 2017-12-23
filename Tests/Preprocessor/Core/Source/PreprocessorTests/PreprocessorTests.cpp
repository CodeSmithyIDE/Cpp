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
#include "CodeSmithy/Cpp/Preprocessor/Core/PreprocessingIncludeDirectiveDefaultResolver.h"
#include <fstream>

void AddPreprocessorTests(TestHarness& theTestHarness)
{
    TestSequence& preprocessorTestSequence = theTestHarness.appendTestSequence("Preprocessor tests");

    new HeapAllocationErrorsTest("Creation test 1", PreprocessorCreationTest1, preprocessorTestSequence);

    new FileComparisonTest("run test 1", PreprocessorRunTest1, preprocessorTestSequence);
    new FileComparisonTest("run test 2", PreprocessorRunTest2, preprocessorTestSequence);

    new FileComparisonTest("character literals test 1", PreprocessorCharacterLiteralsTest1, preprocessorTestSequence);

    new FileComparisonTest("string literals test 1", PreprocessorStringLiteralsTest1, preprocessorTestSequence);

    new FileComparisonTest("mathematical expressions test 1", PreprocessorMathematicalExpressionsTest1, preprocessorTestSequence);

    new FileComparisonTest("directive test 1", PreprocessorDirectiveTest1, preprocessorTestSequence);
    new FileComparisonTest("directive test 2", PreprocessorDirectiveTest2, preprocessorTestSequence);
    new FileComparisonTest("directive test 3", PreprocessorDirectiveTest3, preprocessorTestSequence);
    new FileComparisonTest("directive test 4", PreprocessorDirectiveTest4, preprocessorTestSequence);
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

    CodeSmithy::Cpp::PreprocessingIncludeDirectiveDefaultResolver includeResolver;
    TestCallbacks callbacks;
    preprocessor.run(includeResolver, callbacks);
    callbacks.write(outputPath);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PreprocessorRunTest1.txt");

    if ((callbacks.unexpectedCharactersCount() == 0) &&
        (preprocessor.context().size() == 0))
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

    CodeSmithy::Cpp::PreprocessingIncludeDirectiveDefaultResolver includeResolver;
    TestCallbacks callbacks;
    preprocessor.run(includeResolver, callbacks);
    callbacks.write(outputPath);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PreprocessorRunTest2.txt");

    if ((callbacks.unexpectedCharactersCount() == 0) &&
        (preprocessor.context().size() == 0))
    {
        result = TestResult::ePassed;
    }

    return result;
}

TestResult::EOutcome PreprocessorCharacterLiteralsTest1(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "CharacterLiterals1.cpp");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PreprocessorCharacterLiteralsTest1.txt");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::Preprocessor preprocessor(input);

    CodeSmithy::Cpp::PreprocessingIncludeDirectiveDefaultResolver includeResolver;
    TestCallbacks callbacks;
    preprocessor.run(includeResolver, callbacks);
    callbacks.write(outputPath);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PreprocessorCharacterLiteralsTest1.txt");

    if ((callbacks.unexpectedCharactersCount() == 0) &&
        (preprocessor.context().size() == 0))
    {
        result = TestResult::ePassed;
    }

    return result;
}

TestResult::EOutcome PreprocessorStringLiteralsTest1(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "StringLiterals1.cpp");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PreprocessorStringLiteralsTest1.txt");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::Preprocessor preprocessor(input);

    CodeSmithy::Cpp::PreprocessingIncludeDirectiveDefaultResolver includeResolver;
    TestCallbacks callbacks;
    preprocessor.run(includeResolver, callbacks);
    callbacks.write(outputPath);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PreprocessorStringLiteralsTest1.txt");

    if ((callbacks.unexpectedCharactersCount() == 0) &&
        (preprocessor.context().size() == 0))
    {
        result = TestResult::ePassed;
    }

    return result;
}

TestResult::EOutcome PreprocessorMathematicalExpressionsTest1(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "MathematicalExpressions1.cpp");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PreprocessorMathematicalExpressionsTest1.txt");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::Preprocessor preprocessor(input);

    CodeSmithy::Cpp::PreprocessingIncludeDirectiveDefaultResolver includeResolver;
    TestCallbacks callbacks;
    preprocessor.run(includeResolver, callbacks);
    callbacks.write(outputPath);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PreprocessorMathematicalExpressionsTest1.txt");

    if ((callbacks.unexpectedCharactersCount() == 0) &&
        (preprocessor.context().size() == 0))
    {
        result = TestResult::ePassed;
    }

    return result;
}

TestResult::EOutcome PreprocessorDirectiveTest1(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "HeaderGuards1.h");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PreprocessorDirectiveTest1.txt");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::Preprocessor preprocessor(input);

    CodeSmithy::Cpp::PreprocessingIncludeDirectiveDefaultResolver includeResolver;
    TestCallbacks callbacks;
    preprocessor.run(includeResolver, callbacks);
    callbacks.write(outputPath);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PreprocessorDirectiveTest1.txt");

    if ((callbacks.unexpectedCharactersCount() == 0) &&
        (preprocessor.context().size() == 1))
    {
        auto it = preprocessor.context().find("_SOMETHING_");
        if (it != preprocessor.context().end())
        {
            result = TestResult::ePassed;
        }
    }

    return result;
}

TestResult::EOutcome PreprocessorDirectiveTest2(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "IncludeDirective1.cpp");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PreprocessorDirectiveTest2.txt");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::Preprocessor preprocessor(input);

    CodeSmithy::Cpp::PreprocessingIncludeDirectiveDefaultResolver includeResolver;
    TestCallbacks callbacks;
    preprocessor.run(includeResolver, callbacks);
    callbacks.write(outputPath);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PreprocessorDirectiveTest2.txt");

    if ((callbacks.unexpectedCharactersCount() == 0) &&
        (preprocessor.context().size() == 0))
    {
        result = TestResult::ePassed;
    }

    return result;
}

TestResult::EOutcome PreprocessorDirectiveTest3(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "IncludeDirective2.cpp");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PreprocessorDirectiveTest3.txt");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::Preprocessor preprocessor(input);

    CodeSmithy::Cpp::PreprocessingIncludeDirectiveDefaultResolver includeResolver;
    includeResolver.appendSearchPath(test.environment().getTestDataDirectory().string() + '/');
    TestCallbacks callbacks;
    preprocessor.run(includeResolver, callbacks);
    callbacks.write(outputPath);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PreprocessorDirectiveTest3.txt");

    if ((callbacks.unexpectedCharactersCount() == 0) &&
        (preprocessor.context().size() == 0))
    {
        result = TestResult::ePassed;
    }

    return result;
}

TestResult::EOutcome PreprocessorDirectiveTest4(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "IncludeDirective3.cpp");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PreprocessorDirectiveTest4.txt");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::Preprocessor preprocessor(input);

    CodeSmithy::Cpp::PreprocessingIncludeDirectiveDefaultResolver includeResolver;
    includeResolver.appendSearchPath(test.environment().getTestDataDirectory().string() + '/');
    TestCallbacks callbacks;
    preprocessor.run(includeResolver, callbacks);
    callbacks.write(outputPath);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PreprocessorDirectiveTest4.txt");

    if ((callbacks.unexpectedCharactersCount() == 0) &&
        (preprocessor.context().size() == 0))
    {
        result = TestResult::ePassed;
    }

    return result;
}
