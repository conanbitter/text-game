#include "datafile.hpp"
#include <format>

DataFile::DataFile(const std::string& filename, uint32_t datatype, uint32_t version) :file(filename, std::ios::in | std::ios::binary) {
    uint32_t fileType = readU32();
    if (fileType != datatype) {
        throw std::runtime_error(std::format("Error opening file \"{}\" : wrong file format", filename));
    }
    uint8_t fileVersion = readU8();
    if (fileVersion != version) {
        throw std::runtime_error(std::format("Error opening file \"{}\" : wrong file format version", filename));
    }

    while (!file.eof()) {
        uint32_t sectionId = readU32();
        if (file.eof()) break;
        size_t sectionSize = readU32();
        size_t sectionPos = file.tellg();
        sections[sectionId] = SectionEntry{ .pos = sectionPos,.size = sectionSize };
        file.seekg(sectionSize, std::ios_base::cur);
    }
    file.clear();
}

void DataFile::seek(uint32_t section) {
    size_t pos = sections.at(section).pos;
    file.seekg(pos);
}

uint64_t DataFile::sectionSize(uint32_t section) {
    return sections.at(section).size;
}

uint8_t DataFile::readU8() {
    uint8_t result;
    file.read(reinterpret_cast<char*>(&result), sizeof(uint8_t));
    return result;
}

uint32_t DataFile::readU32() {
    uint32_t result;
    file.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));
    return result;
}

int32_t DataFile::readI32() {
    int32_t result;
    file.read(reinterpret_cast<char*>(&result), sizeof(int32_t));
    return result;
}

float DataFile::readF32() {
    float result;
    file.read(reinterpret_cast<char*>(&result), sizeof(float));
    return result;
}

std::string DataFile::readStr(uint32_t section) {
    SectionEntry se = sections.at(section);
    file.seekg(se.pos);

    std::string result(se.size, '\0');
    file.read(&result[0], se.size);

    return result;
}
