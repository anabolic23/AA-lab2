#include "CoderDecoder.h"
#include <iostream>
#include <fstream>
#include <vector>


int main() {
    Base64Encoder encoder;
    encoder.OpenFile("../sample.txt");
    encoder.Encode();
    std::string encodedResult = encoder.GetEncodedData();
    std::cout << "Encoded Result: " << encodedResult << std::endl;
    encoder.CloseFile();

    Base64Decoder decoder;
    decoder.Decode(encodedResult);
    std::string decodedResult = decoder.GetDecodedData();
    std::cout << "Decoded Result: " << decodedResult << std::endl;

    return 0;
}