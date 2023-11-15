#include <iostream>
#include "CoderDecoder.h" // Replace with the correct header file name if different

int main() {
    Base64Encoder encoder;
    Base64Decoder decoder;

    std::string inputFilePath;
    std::string outputEncodedFilePath;
    std::string outputDecodedFilePath;

    std::cout << "Base64 Encoder and Decoder\n";

    // Get input file path
    std::cout << "Enter the path of the file to encode: ";
    std::getline(std::cin, inputFilePath);

    // Get output encoded file path
    std::cout << "Enter the path to save the encoded file (or press Enter for default): ";
    std::getline(std::cin, outputEncodedFilePath);
    if (outputEncodedFilePath.empty()) {
        size_t extensionPos = inputFilePath.find_last_of(".");
        if (extensionPos != std::string::npos) {
            outputEncodedFilePath = inputFilePath.substr(0, extensionPos) + ".base64";
        }
        else {
            outputEncodedFilePath = inputFilePath + ".base64";
        }
    }

    // Encoding
    encoder.OpenFile(inputFilePath);
    encoder.Encode();
    encoder.GetEncodedData(outputEncodedFilePath);

    std::cout << "File encoded and saved as: " << outputEncodedFilePath << std::endl;

    // Get input encoded file path
    std::cout << "\nEnter the path of the file to decode: ";
    std::getline(std::cin, inputFilePath);

    // Get output decoded file path
    std::cout << "Enter the path to save the decoded file (or press Enter for default): ";
    std::getline(std::cin, outputDecodedFilePath);
    if (outputDecodedFilePath.empty()) {
        outputDecodedFilePath = "decoded_output.txt"; // Default decoded file name
    }

    // Decoding
    decoder.OpenFile(inputFilePath);
    decoder.Decode();
    decoder.GetDecodedData(outputDecodedFilePath);

    std::cout << "File decoded and saved as: " << outputDecodedFilePath << std::endl;

    return 0;
}