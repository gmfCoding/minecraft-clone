#include "CommonData.hpp"

RectUV::RectUV (glm::vec2 p_topRight, glm::vec2 p_topLeft, glm::vec2 p_bottomRight, glm::vec2 p_bottomLeft, float size)
{
    topRight = glm::vec2(p_topRight/size);
    topLeft = glm::vec2(p_topLeft/size);
    bottomRight = glm::vec2(p_bottomRight/size);
    bottomLeft = glm::vec2(p_bottomLeft/size);
}

RectUV::RectUV () : topRight(), topLeft(), bottomRight(), bottomLeft()
{

}


block::FACE block::GLMVecToFACE(glm::vec3 direction)
{
    for (size_t i = 0; i < 3; i++)
    {
        
        if(direction[i] != 0)
        {
            if(direction[i] < 0)
                return (block::FACE) (i+3);
            else
                return (block::FACE) (i);
        }
    }

    return block::FACE::FNZ;
}