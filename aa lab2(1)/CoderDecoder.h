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
    std::string inputFileName;
    std::string encoded;
    std::vector<std::string> comments;
public:
    Base64Encoder();
    void EncodeTriplet(const std::string& triplet);
    void EncodeDuplet(const std::string& duplet);
    void EncodeSymbol(const char& symbol);
    void OpenFile(const std::string& inputFileName);
    void CloseFile();
    void AddComment(const std::string& comment);
    void GetEncodedData(const std::string& outputFileName = "");
    void Encode();
};

class Base64Decoder {
private:
    std::string alphabet;
    std::ifstream inputFile;
    std::string decoded;
public:
    Base64Decoder();
    char GetIndex(char c);
    int DecodeTriplet(const std::string& triplet);
    int DecodeDuplet(const std::string& duplet);
    int DecodeSymbol(const std::string& symbol);
    void OpenFile(const std::string& inputFileName);
    void CloseFile();
    void Decode();
    void GetDecodedData(const std::string& outputFileName = "");
};
