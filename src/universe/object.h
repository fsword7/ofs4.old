
#pragma once

#include "engine/object.h"
#include "universe/surface.h"

namespace ofs::universe
{
    using namespace ofs::engine;

    class CelestialBody : public Object
    {
        public:
            enum CelestialType
            {
                cbUnknown = 0,
                cbStar,
                cbPlanet,
                cbDwarfPlanet,
                cbMoon,
                cbComet
            };

            enum {
                cbKnownRadius   = 1,
                cbKnownRotation = 2,
                cbKnownSurface  = 4,
                cbKnownObject   = (cbKnownRadius|cbKnownRotation|cbKnownSurface)
            };

            CelestialBody(const std::string &name, CelestialType type)
            : Object(name, objCelestial), cbType(type)
            {

            }
            virtual ~CelestialBody() = default;

        private:
            CelestialType cbType = cbUnknown;

        protected:
            mutable Surface surface;

            uint32_t knownFlags = 0;        
    };

    class CelestialStar : public CelestialBody
    {
        public:
            CelestialStar(const std::string &name)
            : CelestialBody(name, cbStar)
            {

            }
            virtual ~CelestialStar();
    };
}