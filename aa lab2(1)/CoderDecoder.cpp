#include "CoderDecoder.h"   
#include <iostream>

Base64Encoder::Base64Encoder() {
    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
}

void Base64Encoder::EncodeTriplet(const std::string& triplet) {
    std::string result = "====";
    int k = triplet.at(0) >> 2;
    result.at(0) = alphabet.at(k);

    k = ((triplet.at(0) & 3) << 4) | (triplet.at(1) >> 4);
    result.at(1) = alphabet.at(k);

    k = ((triplet.at(1) & 0xF) << 2) | (triplet.at(2) >> 6);
    result.at(2) = alphabet.at(k);

    k = triplet.at(2) & 0x3F;
    result.at(3) = alphabet.at(k);

    encoded += result;

}

void Base64Encoder::EncodeDuplet(const std::string& duplet) {
    std::string result = "====";
    int k = duplet.at(0) >> 2;
    result.at(0) = alphabet.at(k);

    k = ((duplet.at(0) & 3) << 4) | (duplet.at(1) >> 4);
    result.at(1) = alphabet.at(k);

    k = ((duplet.at(1) & 0xF) << 2);
    result.at(2) = alphabet.at(k);

    encoded += result;
}

void Base64Encoder::EncodeSymbol(const char& symbol) {
    std::string result = "====";
    int k = symbol >> 2;
    result.at(0) = alphabet.at(k);

    k = ((symbol & 3) << 4);
    result.at(1) = alphabet.at(k);

    encoded += result;
}

void Base64Encoder::OpenFile(const std::string& inputFileName) {
    this->inputFileName = inputFileName; // Store input file name
    inputFile.open(this->inputFileName, std::ios::binary);

    if (!inputFile) {
        std::cerr << "Error: Unable to open the file '" << this->inputFileName << "'\n";
    }
    else {
        std::cout << "File " << this->inputFileName << " opened successfully!\n";
    }
}

void Base64Encoder::CloseFile() {
    inputFile.close();
    std::cout << "File closed successfully!\n";
}

void Base64Encoder::AddComment(const std::string& comment) {
    // Ensure the comment doesn't exceed the maximum length, including the leading character
    std::string truncatedComment = comment.substr(0, 75);
    comments.push_back("-" + truncatedComment);
}

void Base64Encoder::GetEncodedData(const std::string& outputFileName) {
    std::string outputFile;

    if (outputFileName.empty()) {
        // If outputFileName is not provided, create a filename with ".base64" extension
        size_t extensionPos = inputFileName.find_last_of(".");
        if (extensionPos != std::string::npos) {
            outputFile = inputFileName.substr(0, extensionPos) + ".base64";
        }
        else {
            outputFile = inputFileName + ".base64";
        }
    }
    else {
        outputFile = outputFileName;
    }

    for (const auto& comment : comments) {
        encoded += '\n' + comment;
    }

    std::ofstream outputFileStream(outputFile);

    if (!outputFileStream) {
        std::cerr << "Error: Unable to open the output file '" << outputFile << "'\n";
        return;
    }

    outputFileStream << encoded;
    outputFileStream.close();

    std::cout << "Encoded data written to '" << outputFile << "' successfully!\n";
    std::cout << "Encoded Result: " << encoded << std::endl;
}


void Base64Encoder::Encode() {
    char buffer[3];
    int symbolsInLine = 0;
    while (!inputFile.eof()) {
        inputFile.read(buffer, 3);
        int bytesRead = inputFile.gcount();

        if (bytesRead == 3) {
            EncodeTriplet(std::string(buffer, 3));
            symbolsInLine += 4;  // Each triplet adds 4 symbols
        }
        else if (bytesRead == 2) {
            EncodeDuplet(std::string(buffer, 2));
            symbolsInLine += 4;  // Each duplet adds 4 symbols
        }
        else if (bytesRead == 1) {
            EncodeSymbol(buffer[0]);
            symbolsInLine += 4;  // Each symbol adds 4 symbols
        }

        if (symbolsInLine >= 76) {
            encoded += '\n';
            symbolsInLine = 0;
        }
    }

    CloseFile();
}


Base64Decoder::Base64Decoder() {
    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
}

char Base64Decoder::GetIndex(char c) {
    size_t index = alphabet.find(c);
    if (index != std::string::npos) {
        return static_cast<char>(index);
    }
    return -1;
}

int Base64Decoder::DecodeSymbol(const std::string& symbol) {
    if (symbol.size() != 2 || symbol[0] == '-') {
        return symbol.size();
    }

    char index1 = GetIndex(symbol[0]);
    char index2 = GetIndex(symbol[1]);
    if (index1 == std::string::npos) {
        return 1;
    }
    if (index2 == std::string::npos) {
        return 2;
    }


    char decodedByte = (index1 << 2) | (index2 >> 4);
    decoded += decodedByte;
    return 0;
}

int Base64Decoder::DecodeDuplet(const std::string& duplet) {
    if (duplet.size() != 3 || duplet[0] == '-') {
        return duplet.size();
    }

    char index1 = GetIndex(duplet[0]);
    char index2 = GetIndex(duplet[1]);
    char index3 = GetIndex(duplet[2]);
    if (index1 == std::string::npos) {
        return 1;
    }
    if (index2 == std::string::npos) {
        return 2;
    }
    if (index3 == std::string::npos) {
        return 3;
    }


    char decodedByte1 = (index1 << 2) | (index2 >> 4);
    char decodedByte2 = ((index2 & 0x0F) << 4) | (index3 >> 2);

    decoded += decodedByte1;
    decoded += decodedByte2;
    return 0;
}

int Base64Decoder::DecodeTriplet(const std::string& triplet) {
    if (triplet.size() != 4 || triplet[0] == '-') {
        return triplet.size();
    }
    char index1 = GetIndex(triplet[0]);
    char index2 = GetIndex(triplet[1]);
    char index3 = GetIndex(triplet[2]);
    char index4 = GetIndex(triplet[3]);


    if (index1 == std::string::npos) {
        return 1;
    }
    if (index2 == std::string::npos) {
        return 2;
    }
    if (index3 == std::string::npos) {
        return 3;
    }
    if (index4 == std::string::npos) {
        return 4;
    }
    char decodedByte1 = (index1 << 2) | (index2 >> 4);

    char decodedByte2 = ((index2 & 0x0F) << 4) | (index3 >> 2);

    char decodedByte3 = ((index3 & 0x03) << 6) | index4;

    decoded += decodedByte1;
    decoded += decodedByte2;
    decoded += decodedByte3;
    return 0;
}

void Base64Decoder::Decode() {
    char buffer[4];
    std::string symbolsInLine = "";
    int numberOfLine = 0;
    std::string line = "";

    while (std::getline(inputFile, line)) {
        numberOfLine++;
        if (line[line.length() - 1] == '\r') {
            line.erase(line.find_last_of('\r'));
        }
        if (line.length() > 76) {
            std::cerr << "Error: line " << numberOfLine << ": incorrect length: expected 76 but " << line.length() << std::endl;
            return;
        }
        if (line[0] == '-') {
            continue;
        }

        for (int i = 0; i < line.length() / 4; i++)
        {
            std::string subString = line.substr(i * 4, 4);
            int bytesRead = subString.length();
            for (int j = 0; j < subString.length(); j++) {
                buffer[j] = subString[j];

            }
            for (int j = subString.size() - 1; j > -1; j--) {

                if (buffer[j] == '=') {
                    bytesRead--;
                }
            }
            int result = 0;
            if (bytesRead >= 2) {

                if (bytesRead == 4) {
                    result = DecodeTriplet(std::string(buffer, 4));
                }
                else if (bytesRead == 3) {
                    result = DecodeDuplet(std::string(buffer, 3));
                }
                else if (bytesRead == 2) {
                    result = DecodeSymbol(std::string(buffer, 2));
                }
            }

            if (result != 0) {
                std::cerr << "Line " << numberOfLine << ", pos " << i * 4 + result << " - inccorect symbol : " << buffer[result - 1] << std::endl;
                return;
            }
            symbolsInLine += static_cast <char>(bytesRead);
        }
    }

    CloseFile();
}

void Base64Decoder::GetDecodedData(const std::string& outputFileName) {

    std::ofstream outputFile(outputFileName);  // Open the output file

    if (!outputFile) {
        std::cerr << "Error: Unable to open the output file '" << outputFileName << "'\n";
        return;
    }
    // Write the decoded data to the output file
    outputFile << decoded;

    // Close the output file
    outputFile.close();

    std::cout << "Decoded data written to '" << outputFileName << "' successfully!\n";

    // Print the decoded data to the console
    std::cout << "Decoded Result: " << decoded << std::endl;
}

void Base64Decoder::OpenFile(const std::string& inputFileName) {
    if (inputFileName.find(".base64") != std::string::npos) {
        inputFile.open(inputFileName, std::ios::binary);
    }
    else {
        std::cerr << "Error: Only files with the .base64 extension are supported for decoding.\n";
        return;
    }

    if (!inputFile) {
        std::cerr << "Error: Unable to open the file '" << inputFileName << "'\n";
    }
    else {
        std::cout << "File " << inputFileName << " opened successfully!\n";
    }
}

void Base64Decoder::CloseFile() {
    inputFile.close();
    std::cout << "File closed successfully!\n";
}