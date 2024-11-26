#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include <iostream> 
#include <string>
#include "../Lab-12.1C/Lab-12.1C.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(UnitTest1)
    {
    public:

        TEST_METHOD(TestCreateBIN)
        {
            const char* fname = "test_input.bin";

            
            std::ofstream tempInput("mock_input.txt");
            tempInput << "1.5\nY\n2.5\nY\n-3.5\nN\n";
            tempInput.close();

          
            FILE* inputFile = nullptr;
            errno_t err = freopen_s(&inputFile, "mock_input.txt", "r", stdin);
            Assert::IsTrue(err == 0, L"Failed to redirect stdin to mock_input.txt");

            
            CreateBIN(fname);

            
            std::ifstream fin(fname, std::ios::binary);
            Assert::IsTrue(fin.is_open(), L"Input file was not created.");

            
            double expectedNumbers[] = { 1.5, 2.5, -3.5 };
            double readNumber;
            for (double expected : expectedNumbers) {
                fin.read(reinterpret_cast<char*>(&readNumber), sizeof(readNumber));
                Assert::AreEqual(expected, readNumber, 0.01, L"Number does not match.");
            }

            
            fin.read(reinterpret_cast<char*>(&readNumber), sizeof(readNumber));
            Assert::IsTrue(fin.eof(), L"File contains unexpected data.");
            fin.close();
        }

        TEST_METHOD(TestProcessBIN)
        {
            const char* inputFile = "test_input.bin";
            const char* outputFile = "test_output.txt";

           
            std::ofstream fout(inputFile, std::ios::binary);
            double numbers[] = { 1.5, 2.5, -3.5, 4.5 };
            for (double num : numbers) {
                fout.write(reinterpret_cast<const char*>(&num), sizeof(num));
            }
            fout.close();

            
            ProcessBIN(inputFile, outputFile);

            
            std::ifstream fin(outputFile);
            Assert::IsTrue(fin.is_open(), L"Output file was not created.");

            
            std::string line;
            std::getline(fin, line);
            Assert::AreEqual(std::string("Average of max and min: 0.50"), line, L"First line does not match.");

            std::getline(fin, line);
            Assert::AreEqual(std::string("Average of all numbers: 1.25"), line, L"Second line does not match.");

            fin.close();
        }

       

    };
}
