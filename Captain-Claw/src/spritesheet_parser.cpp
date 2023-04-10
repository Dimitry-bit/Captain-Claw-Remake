#include <iostream>
#include <fstream>
#include <string>
#include "SFML/Graphics.hpp"

#include "spritesheet_parser.h"

bool SpriteSheetLoadFromFile(const char* identifier, spriteSheet_t& spriteSheet)
{
    spriteSheet.frameCount = 0;
    spriteSheet.frames.clear();

    if (!spriteSheet.texture.loadFromFile(identifier)) {
        printf("[ERROR][SpriteSheet]: Unable to load %s.\n", identifier);
        return false;
    }

    string metaPath(identifier);
    metaPath = metaPath.substr(0, metaPath.find_last_of('.'));
    metaPath.append(".meta");

    FILE* fp = fopen(metaPath.c_str(), "r");
    if (!fp) {
        printf("[ERROR][SpriteSheet]: Unable to load %s.\n", metaPath.c_str());
        return false;
    }

    char frameID[256];
    sf::IntRect area;
    sf::Vector2f pivot;

    // TODO(Dimitry): This will cause a crash if parsed wrong. TOO BAD!
    while (!feof(fp)) {
        fscanf(fp, " n=%255s x=%d y=%d w=%d h=%d pX=%f pY=%f",
               frameID, &area.left, &area.top, &area.width, &area.height, &pivot.x, &pivot.y);
        spriteSheet.frameCount++;
        spriteSheet.frames.push_back(
            frameMetadata_t{
                .id = frameID,
                .area = area,
                .pivot = pivot,
            }
        );
    }
    spriteSheet.frameCount--;

    fclose(fp);
    printf("[INFO][SpriteSheet]: %s loaded successfully.\n", identifier);
    return true;
}