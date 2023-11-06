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
    std::string result = "===";
    int k = duplet.at(0) >> 2;
    result.at(0) = alphabet.at(k);

    k = ((duplet.at(0) & 3) << 4) | (duplet.at(1) >> 4);
    result.at(1) = alphabet.at(k);

    k = ((duplet.at(1) & 0xF) << 2);
    result.at(2) = alphabet.at(k);

    encoded += result;
}

void Base64Encoder::EncodeSymbol(const char& symbol) {
    std::string result = "==";
    int k = symbol >> 2;
    result.at(0) = alphabet.at(k);

    k = ((symbol & 3) << 4);
    result.at(1) = alphabet.at(k);

    encoded += result;
}

void Base64Encoder::OpenFile(const std::string& inputFileName) {
    inputFile.open(inputFileName, std::ios::binary);

    if (!inputFile) {
        std::cerr << "Error: Unable to open the file '" << inputFileName << "'\n";
    }
    else {
        std::cout << "File " << inputFileName << " opened successfully!\n";
    }
}

void Base64Encoder::CloseFile() {
    inputFile.close();
    std::cout << "File closed successfully!\n";
}

std::string Base64Encoder::GetEncodedData() {
    return encoded;
}

void Base64Encoder::Encode() {
    char buffer[3];
    while (!inputFile.eof()) {
        inputFile.read(buffer, 3);
        int bytesRead = inputFile.gcount();

        if (bytesRead == 3) {
            EncodeTriplet(std::string(buffer, 3));
        }
        else if (bytesRead == 2) {
            EncodeDuplet(std::string(buffer, 2));
        }
        else if (bytesRead == 1) {
            EncodeSymbol(buffer[0]);
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

void Base64Decoder::DecodeSymbol(const std::string& symbol) {
    if (symbol.size() != 2) {
        std::cerr << "Error: Invalid Base64 symbol length.\n";
        return;
    }

    char index1 = GetIndex(symbol[0]);
    char index2 = GetIndex(symbol[1]);

    if (index1 == -1 || index2 == -1) {
        std::cerr << "Error: Invalid Base64 character(s) in symbol.\n";
        return;
    }

    char decodedByte = (index1 << 2) | (index2 >> 4);
    decoded += decodedByte;
}

void Base64Decoder::DecodeDuplet(const std::string& duplet) {
    if (duplet.size() != 3) {
        std::cerr << "Error: Invalid Base64 duplet length.\n";
        return;
    }

    char index1 = GetIndex(duplet[0]);
    char index2 = GetIndex(duplet[1]);
    char index3 = GetIndex(duplet[2]);

    if (index1 == -1 || index2 == -1 || index3 == -1) {
        std::cerr << "Error: Invalid Base64 character(s) in duplet.\n";
        return;
    }

    char decodedByte1 = (index1 << 2) | (index2 >> 4);
    char decodedByte2 = ((index2 & 0x0F) << 4) | (index3 >> 2);

    decoded += decodedByte1;
    decoded += decodedByte2;
}

void Base64Decoder::DecodeTriplet(const std::string& triplet) {
    if (triplet.size() != 4) {
        std::cerr << "Error: Invalid Base64 triplet length.\n";
        return;
    }

    char index1 = GetIndex(triplet[0]);
    char index2 = GetIndex(triplet[1]);
    char index3 = GetIndex(triplet[2]);
    char index4 = GetIndex(triplet[3]);

    if (index1 == -1 || index2 == -1 || index3 == -1 || index4 == -1) {
        std::cerr << "Error: Invalid Base64 character(s) in triplet.\n";
        return;
    }

    char decodedByte1 = (index1 << 2) | (index2 >> 4);
    char decodedByte2 = ((index2 & 0x0F) << 4) | (index3 >> 2);
    char decodedByte3 = ((index3 & 0x03) << 6) | index4;

    decoded += decodedByte1;
    decoded += decodedByte2;
    decoded += decodedByte3;
}

void Base64Decoder::Decode(const std::string& input) {
    decoded.clear();

    for (size_t i = 0; i < input.size(); i += 4) {
        std::string chunk = input.substr(i, 4);
        if (chunk.size() == 4) {
            DecodeTriplet(chunk);
        }
        else if (chunk.size() == 3) {
            DecodeDuplet(chunk);
        }
        else if (chunk.size() == 2) {
            DecodeSymbol(chunk);
        }
        else {
            std::cerr << "Error: Invalid Base64 chunk size.\n";
            break;
        }
    }
}

std::string Base64Decoder::GetDecodedData() {
    return decoded;
}