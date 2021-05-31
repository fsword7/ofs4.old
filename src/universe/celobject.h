
#pragma once

#include "engine/object.h"

namespace ofs::universe
{
    using namespace ofs::engine;

    class CelestialObject : public Object
    {
        public:
            enum CelestialType
            {
                celUnknown = 0,
                celStar,
                celPlanet,
                celMoon
            };

            CelestialObject(const std::string &name)
            : Object(name, objCelestial)
            {

            }
            virtual ~CelestialObject() = default;

        private:
            CelestialType celType = celUnknown;

        
    };

    class CelestialStar : public CelestialObject
    {
        public:
            CelestialStar(const std::string &name)
            : CelestialObject(name)
            {

            }
            virtual ~CelestialStar();
    };

    class CelestialBody : public CelestialObject
    {
           CelestialBody(const std::string &name)
            : CelestialObject(name)
            {

            }
            virtual ~CelestialBody();
    };
}