
#pragma once

namespace ofs::engine
{
    class Object
    {
        public:
            enum ObjectType
            {
                objUnknown = 0,
                objPlayer,
                objCelestial
            };

            Object(const std::string &name, ObjectType type)
            : objType(type)
            {
                objNames[0] = name;
            }
            virtual ~Object() = default;

        private:
            ObjectType objType = objUnknown;
            std::vector<std::string> objNames{1};

        protected:

    };
}