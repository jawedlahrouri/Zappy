/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DCamera
*/

/**
 * @file Graphic3DCamera.hpp
 * @brief Declares the orbital camera used by the Raylib renderer.
 */

#ifndef GRAPHIC3DCAMERA_HPP_
    #define GRAPHIC3DCAMERA_HPP_

    #include "GameWorld.hpp"
    #include <raylib.h>

    /**
     * @brief Maintains a bounded orbit camera around the 3D map.
     */
    class Graphic3DCamera {
        public:
            /**
             * @brief Creates an uninitialized camera.
             */
            Graphic3DCamera();

            /**
             * @brief Updates camera input and resets when map dimensions change.
             */
            void update(const GameWorld& world);

            /**
             * @brief Gets the Raylib camera instance.
             * @return Camera used by BeginMode3D().
             */
            const Camera3D& getCamera() const;

        private:
            /** Minimum allowed camera distance from the target. */
            static constexpr float MIN_CAMERA_DISTANCE = 4.0f;
            /** Maximum allowed camera distance from the target. */
            static constexpr float MAX_CAMERA_DISTANCE = 80.0f;
            /** Conversion factor from degrees to radians. */
            static constexpr float DEG_TO_RAD = 0.017453292519943295769f;

            /**
             * @brief Clamps a floating-point value between two bounds.
             */
            static float clampFloat(float value, float min, float max);

            /**
             * @brief Adds two Raylib Vector3 values.
             */
            static Vector3 addVector(Vector3 a, Vector3 b);

            /**
             * @brief Scales a Raylib Vector3 by a scalar.
             */
            static Vector3 scaleVector(Vector3 vector, float scale);

            /**
             * @brief Re-centers the camera for a world snapshot.
             */
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
