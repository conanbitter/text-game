#pragma once

#include <string>
#include <unordered_map>
#include <bit>
#include <fstream>

struct SectionEntry {
    size_t pos;
    size_t size;
};

class DataFile {
public:
    DataFile(const std::string& filename, uint32_t datatype, uint32_t version);
    void seek(uint32_t section);
    uint64_t sectionSize(uint32_t section);
    uint8_t readU8();
    uint32_t readU32();
    int32_t readI32();
    float readF32();
    std::string readStr(uint32_t section);

private:
    std::unordered_map<uint32_t, SectionEntry> sections;
    std::ifstream file;
};

constexpr uint32_t operator""_c(const char* s, std::size_t len) {
    if (len != 4) return 0;

    if constexpr (std::endian::native == std::endian::little) {
        // Little-endian
        return (static_cast<uint32_t>(s[0]) << 0) |
            (static_cast<uint32_t>(s[1]) << 8) |
            (static_cast<uint32_t>(s[2]) << 16) |
            (static_cast<uint32_t>(s[3]) << 24);
    } else {
        // Big-endian
        return (static_cast<uint32_t>(s[0]) << 24) |
            (static_cast<uint32_t>(s[1]) << 16) |
            (static_cast<uint32_t>(s[2]) << 8) |
            (static_cast<uint32_t>(s[3]) << 0);
    }
}