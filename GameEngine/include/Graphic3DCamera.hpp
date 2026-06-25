/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DCamera
*/

#ifndef GRAPHIC3DCAMERA_HPP_
    #define GRAPHIC3DCAMERA_HPP_

    #include "GameWorld.hpp"
    #include <raylib.h>

    class Graphic3DCamera {
        public:
            Graphic3DCamera();

            void update(const GameWorld& world);
            const Camera3D& getCamera() const;

        private:
            static constexpr float MIN_CAMERA_DISTANCE = 4.0f;
            static constexpr float MAX_CAMERA_DISTANCE = 80.0f;

            void reset(const GameWorld& world);

            Camera3D _camera;
            Vector3 _target;
            float _distance;
            float _yaw;
            float _pitch;
            int _worldWidth;
            int _worldHeight;
            bool _initialized;
    };

#endif /* !GRAPHIC3DCAMERA_HPP_ */
