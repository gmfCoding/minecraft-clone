#ifndef COMMONDATA_H_INCLUDE
#define COMMONDATA_H_INCLUDE
#include <glm.hpp>

class RectUV
{
    public:
    glm::vec2 topRight;
    glm::vec2 topLeft;
    glm::vec2 bottomRight;
    glm::vec2 bottomLeft;
    
    RectUV();
    RectUV (glm::vec2 p_topRight, glm::vec2 p_topLeft, glm::vec2 p_bottomRight, glm::vec2 p_bottomLeft, float size);
};

struct VoxelData
{
    int x;
    int y;
    int z;
    int pallet;
};

struct VoxelVertexData {
    glm::vec3 position;
    glm::vec2 uv;
};

namespace block
{
    enum FACEFlag
    {
        NONE=0, 		// 0xb000000
        PYT=1,  		// 0xb100000
        NYB=2,  		// 0xb010000
        NZF=4,			// 0xb001000
        PZB=8,			// 0xb000100
        NXL=16,			// 0xb000010
        PXR=32,			// 0xb000001
        SIDES=120,		// 0xb001111
        TOPBOTTOM=3,	// 0xb110000
        ALL=63			// 0xb111111       
    };

    // Ordering is based on XYZ Positive then XYZ Negative, mostly for mathmatical conversion purposes
    enum FACE
    {
        RPX,
        TPY,
        BPZ,
        LNX,
        BNY,
        FNZ
    };

    FACE GLMVecToFACE(glm::vec3 direction);
}

#endif