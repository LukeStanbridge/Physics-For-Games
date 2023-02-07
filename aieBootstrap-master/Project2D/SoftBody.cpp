#include "SoftBody.h"
#include "Sphere.h"
#include "Spring.h"

void SoftBody::Build(PhysicsScene* scene, glm::vec2 position, float spacing, float springForce, float damping, std::vector<std::string>& strings)
{
    int numColumns = strings.size();
    int numRows = strings[0].length();

    // traverse across the array and add balls where the ascii art says they be
    Sphere** spheres = new Sphere * [numRows * numColumns];

    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numColumns; j++)
        {
            if (strings[j][i] == '0')
            {
                spheres[i * numColumns + j] = new Sphere(position + glm::vec2((i*0.6f), (j * 0.6f)) * spacing, glm::vec2(0, 0), 1.0f, 1.0f, glm::vec4(1, 0, 0, 1), 4.0f, 0.3f, 0.3f, false);
                scene->addActor(spheres[i * numColumns + j]);
            }
            else
            {
                spheres[i * numColumns + j] = nullptr;
            }
        }
    }

    // second pass - add springs in 
    for (int i = 1; i < numRows; i++)
    {
        for (int j = 1; j < numColumns; j++)
        {
            Sphere* s11 = spheres[i * numColumns + j];
            Sphere* s01 = spheres[(i - 1) * numColumns + j];
            Sphere* s10 = spheres[i * numColumns + j - 1];
            Sphere* s00 = spheres[(i - 1) * numColumns + j - 1];

            // make springs to cardinal neighbours 
            if (s11 && s01)
                scene->addActor(new Spring(s11, s01, springForce, damping, spacing));
            if (s11 && s10)
                scene->addActor(new Spring(s11, s10, springForce, damping, spacing));
            if (s10 && s00)
                scene->addActor(new Spring(s10, s00, springForce, damping, spacing));
            if (s01 && s00)
                scene->addActor(new Spring(s01, s00, springForce, damping, spacing));
            if (s11 && s00)
                scene->addActor(new Spring(s11, s00, springForce, damping, spacing));
            if (s01 && s10)
                scene->addActor(new Spring(s01, s10, springForce, damping, spacing));

            bool endOfJ = j == numColumns - 1;
            bool endOfI = i == numRows - 1;

            Sphere* s22 = (!endOfI && !endOfJ) ? spheres[(i + 1) * numColumns + (j + 1)] : nullptr;
            Sphere* s02 = !endOfJ ? spheres[(i - 1) * numColumns + (j + 1)] : nullptr;
            Sphere* s20 = !endOfI ? spheres[(i + 1) * numColumns + j - 1] : nullptr;

            if (s22 && s02)
                scene->addActor(new Spring(s22, s02, springForce, damping, spacing));
            if (s22 && s20)
                scene->addActor(new Spring(s22, s20, springForce, damping, spacing));
            if (s20 && s00)
                scene->addActor(new Spring(s20, s00, springForce, damping, spacing));
            if (s02 && s00)
                scene->addActor(new Spring(s02, s00, springForce, damping, spacing));
        }
    }
}