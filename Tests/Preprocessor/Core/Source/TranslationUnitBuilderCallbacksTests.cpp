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

#include "TranslationUnitBuilderCallbacksTests.h"
#include "CodeSmithy/Cpp/Preprocessor/Core/TranslationUnitBuilderCallbacks.h"
#include "CodeSmithy/Cpp/Preprocessor/Core/Preprocessor.h"
#include <fstream>

void AddTranslationUnitBuilderCallbacksTests(TestHarness& theTestHarness)
{
    TestSequence& builderCallbacksTestSequence = theTestHarness.appendTestSequence("TranslationUnitBuilderCallbacks tests");

    new HeapAllocationErrorsTest("Creation test 1", TranslationUnitBuilderCallbacksCreationTest1, builderCallbacksTestSequence);

    new FileComparisonTest("run test 1", TranslationUnitBuilderCallbacksRunTest1, builderCallbacksTestSequence);
    new FileComparisonTest("run test 2", TranslationUnitBuilderCallbacksRunTest2, builderCallbacksTestSequence);
}

TestResult::EOutcome TranslationUnitBuilderCallbacksCreationTest1()
{
    CodeSmithy::Cpp::TranslationUnit translationUnit;
    CodeSmithy::Cpp::TranslationUnitBuilderCallbacks callbacks(translationUnit);
    return TestResult::ePassed;
}

TestResult::EOutcome TranslationUnitBuilderCallbacksRunTest1(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "EmptyFile.cpp");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "TranslationUnitBuilderCallbacksRunTest1.cpp");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::Preprocessor preprocessor(input);

    CodeSmithy::Cpp::TranslationUnit translationUnit;
    CodeSmithy::Cpp::TranslationUnitBuilderCallbacks callbacks(translationUnit);
    preprocessor.run(callbacks);

    std::ofstream output(outputPath.c_str());
    translationUnit.write(output);
    
    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "TranslationUnitBuilderCallbacksRunTest1.cpp");

    return result;
}

TestResult::EOutcome TranslationUnitBuilderCallbacksRunTest2(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "MinimalMainFunction1.cpp");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "TranslationUnitBuilderCallbacksRunTest2.cpp");

    std::ifstream input(inputPath.c_str());
    CodeSmithy::Cpp::Preprocessor preprocessor(input);

    CodeSmithy::Cpp::TranslationUnit translationUnit;
    CodeSmithy::Cpp::TranslationUnitBuilderCallbacks callbacks(translationUnit);
    preprocessor.run(callbacks);

    std::ofstream output(outputPath.c_str());
    translationUnit.write(output);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "TranslationUnitBuilderCallbacksRunTest2.cpp");

    return result;
}
