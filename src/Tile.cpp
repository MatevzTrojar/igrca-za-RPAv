#include "Tile.hpp"
bool Tile::IsOffScreen(){
    if(dest.x + dest.w < 0 || dest.x > 1920 || dest.y + dest.h < 0 || dest.y > 1080){
        return true;
    }
    return false;
}

