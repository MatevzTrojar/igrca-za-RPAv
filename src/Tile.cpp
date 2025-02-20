#include "Tile.hpp"
bool Tile::IsOffScreen(){
    if(tile.x + tile.w < 0 || tile.x > 1920 || tile.y + tile.h < 0 || tile.y > 1080){
        return true;
    }
    return false;
}
