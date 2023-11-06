#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class Base64Encoder {
private: 
    std::string alphabet;
    std::ifstream inputFile;
    std::string inputData;
    std::string encoded;
    std::string comments;
public:
    Base64Encoder();
    void EncodeTriplet(const std::string& triplet);
    void EncodeDuplet(const std::string& duplet);
    void EncodeSymbol(const char& symbol);
    void OpenFile(const std::string& inputFileName);
    void CloseFile();
    std::string GetEncodedData();
    void Encode();
    std::string GetOutputFileName(const std::string& inputFileName);
};

class Base64Decoder {
private:
    std::string alphabet;
    std::string decoded;
public:
    Base64Decoder();
    char GetIndex(char c);
    void DecodeTriplet(const std::string& triplet);
    void DecodeDuplet(const std::string& duplet);
    void DecodeSymbol(const std::string& symbol);
    void Decode(const std::string& input);
    std::string GetDecodedData();
};