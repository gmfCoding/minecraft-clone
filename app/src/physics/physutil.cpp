#include "physics/physutil.hpp"

namespace physutil
{
	glm::vec3 antivector(glm::vec3 vec)
	{
		glm::vec3 an = vec;
		if(an.x != -1)
			an.x = -1;
		else
			an.x = 0;
			
		if(an.y != -1)
			an.y = -1;
		else
			an.y = 0;

		if(an.z != -1)
			an.z = -1;
		else
			an.z = 0;
		return an;
	}
}
