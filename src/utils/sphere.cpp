
#include "main/core.h"
#include "utils/sphere.h"
            
using namespace utils;            
      
void Sphere::createTetrahedron()
{
    double a = 1.0 / 3.0;
    double b = sqrt(8.0 / 9.0);
    double c = sqrt(2.0 / 9.0);
    double d = sqrt(2.0 / 3.0);

    // auto v0 = addVertex(vec3d_t(  0,  0,  1 ));
    // auto v1 = addVertex(vec3d_t( -c,  d, -a ));
    // auto v2 = addVertex(vec3d_t( -c, -d, -a ));
    // auto v3 = addVertex(vec3d_t(  b,  0, -a ));

    // addTriangle(v0, v1, v2);
    // addTriangle(v0, v2, v3);
    // addTriangle(v0, v3, v1);
    // addTriangle(v3, v2, v1);
}

void Sphere::createHexahedron()
{
    double a = 1.0 / sqrt(3.0);

    // auto v0 = addVertex(vec3d_t( -a, -a, -a ));
    // auto v1 = addVertex(vec3d_t(  a, -a, -a ));
    // auto v2 = addVertex(vec3d_t(  a,  a, -a ));
    // auto v3 = addVertex(vec3d_t( -a,  a, -a ));
    // auto v4 = addVertex(vec3d_t( -a, -a,  a ));
    // auto v5 = addVertex(vec3d_t(  a, -a,  a ));
    // auto v6 = addVertex(vec3d_t(  a,  a,  a ));
    // auto v7 = addVertex(vec3d_t( -a,  a,  a ));

    // addQuad(v3, v2, v1, v0);
    // addQuad(v2, v6, v5, v1);
    // addQuad(v5, v6, v7, v4);
    // addQuad(v0, v4, v7, v3);
    // addQuad(v3, v7, v6, v2);
    // addQuad(v1, v5, v4, v0);
}

void Sphere::createIcosahedron()
{
    double phi = (1.0 + sqrt(5.0)) * 0.5;
    double a = 1.0;
    double b = 1.0 / phi;

    // auto v0  = addVertex(vec3d_t(  0,  b, -a ));
    // auto v1  = addVertex(vec3d_t(  b,  a,  0 ));
    // auto v2  = addVertex(vec3d_t( -b,  a,  0 ));
    // auto v3  = addVertex(vec3d_t(  0,  b,  a ));
    // auto v4  = addVertex(vec3d_t(  0, -b,  a ));
    // auto v5  = addVertex(vec3d_t( -a,  0,  b ));
    // auto v6  = addVertex(vec3d_t(  0, -b, -a ));
    // auto v7  = addVertex(vec3d_t(  a,  0, -b ));
    // auto v8  = addVertex(vec3d_t(  a,  0,  b ));
    // auto v9  = addVertex(vec3d_t( -a,  0, -b ));
    // auto v10 = addVertex(vec3d_t(  b, -a,  0 ));
    // auto v11 = addVertex(vec3d_t( -b, -a,  0 ));

    // addTriangle(v2, v1, v0);
    // addTriangle(v1, v2, v3);
    // addTriangle(v5, v2, v3);
    // addTriangle(v4, v8, v3);
    // addTriangle(v7, v6, v0);
    // addTriangle(v6, v9, v0);
    // addTriangle(v11, v10, v4);
    // addTriangle(v10, v11, v6);
    // addTriangle(v9, v5, v2);
    // addTriangle(v5, v9, v11);
    // addTriangle(v8, v7, v1);
    // addTriangle(v7, v8, v10);
    // addTriangle(v2, v5, v3);
    // addTriangle(v8, v1, v3);
    // addTriangle(v9, v2, v0);
    // addTriangle(v1, v7, v0);
    // addTriangle(v11, v9, v6);
    // addTriangle(v7, v10, v6);
    // addTriangle(v5, v11, v4);
    // addTriangle(v10, v8, v4);
}
