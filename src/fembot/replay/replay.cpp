#include "replay.hpp"

#include <fstream>

#include "../../utilities/utilities.hpp"

/*
 * Fembot file format
 *
 * header "FEM" (3 bytes)
 * fps (4 bytes, float)
 * actions {
 *    frame (4 bytes, unsigned int)
 *    state (1 byte, unsigned char) (first bit = hold, second bit = player 2)
 * }
 */

constexpr const char* HEADER = "FEM";

void FembotReplay::saveToFile(const std::string filename) {
    std::ofstream file;
    file.open(widen(filename), std::ios::binary | std::ios::out);

    file << HEADER;
    writeBinary(file, fps);
    for (const auto& action : actions) {
        writeBinary(file, action.frame);

        uint8_t state =
           static_cast<unsigned char>(action.hold)
           | static_cast<unsigned char>(action.player2) << 1;

        file << state;
    }
    file.close();
}

FembotReplay FembotReplay::loadFromFile(const std::string& filename) {
    FembotReplay replay(0);

    std::ifstream file;
    file.open(widen(filename), std::ios::binary | std::ios::in);

    file.seekg(0, std::ios::end);
    size_t fileSize = static_cast<size_t>(file.tellg());
    file.seekg(0);

    char header[3];
    file.read(header, 3);
    if (memcmp(header, HEADER, 3) != 0) {
        throw std::runtime_error("Invalid header");
    }

    replay.fps = readBinary<float>(file);
    size_t actionsSize = fileSize - static_cast<size_t>(file.tellg());

    for (size_t i = 0; i < actionsSize; i += 5) {
        unsigned int frame = readBinary<unsigned int>(file);
        uint8_t state = readBinary<uint8_t>(file);
        bool hold = state & 0x01;
        bool player2 = state & 0x02;

        replay.addAction(frame, hold, player2);
    }

    file.close();

    return std::move(replay);
}