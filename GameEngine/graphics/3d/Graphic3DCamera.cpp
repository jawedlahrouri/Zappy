/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DCamera
*/

#include "Graphic3DCamera.hpp"
#include "Graphic3DConfig.hpp"
#include <algorithm>
#include <cmath>

float Graphic3DCamera::clampFloat(float value, float min, float max)
{
    return std::max(min, std::min(value, max));
}

Vector3 Graphic3DCamera::addVector(Vector3 a, Vector3 b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

Vector3 Graphic3DCamera::scaleVector(Vector3 vector, float scale)
{
    return {vector.x * scale, vector.y * scale, vector.z * scale};
}

Graphic3DCamera::Graphic3DCamera()
    : _camera{},
    _target{0.0f, 0.0f, 0.0f},
    _distance(18.0f),
    _yaw(45.0f),
    _pitch(48.0f),
    _worldWidth(0),
    _worldHeight(0),
    _initialized(false)
{
    _camera.up = {0.0f, 1.0f, 0.0f};
    _camera.fovy = 45.0f;
    _camera.projection = CAMERA_PERSPECTIVE;
}

const Camera3D& Graphic3DCamera::getCamera() const
{
    return _camera;
}

void Graphic3DCamera::reset(const GameWorld& world)
{
    const int width = std::max(8, world.getWidth());
    const int height = std::max(8, world.getHeight());
    const float step = Graphic3DConfig::TILE_SIZE + Graphic3DConfig::TILE_GAP;
    const float mapSize = std::max(width, height) * step;

    _target = {0.0f, 0.0f, 0.0f};
    _distance = clampFloat(mapSize * 1.35f, 10.0f, MAX_CAMERA_DISTANCE);
    _yaw = 45.0f;
    _pitch = 48.0f;
    _worldWidth = world.getWidth();
    _worldHeight = world.getHeight();
    _initialized = true;
}

void Graphic3DCamera::update(const GameWorld& world)
{
    const float wheel = GetMouseWheelMove();
    const float panSpeed = std::max(0.08f, _distance * 0.012f);
    const float rotationSpeed = 90.0f * GetFrameTime();
    const Vector2 mouseDelta = GetMouseDelta();

    if (!_initialized || (world.getWidth() > 0 && world.getHeight() > 0 &&
        (world.getWidth() != _worldWidth || world.getHeight() != _worldHeight)))
        reset(world);
    if (wheel != 0.0f) {
        _distance -= wheel * std::max(0.8f, _distance * 0.08f);
        _distance = clampFloat(_distance, MIN_CAMERA_DISTANCE,
            MAX_CAMERA_DISTANCE);
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        _yaw -= mouseDelta.x * 0.22f;
        _pitch += mouseDelta.y * 0.18f;
        _pitch = clampFloat(_pitch, 18.0f, 78.0f);
    }
    if (IsKeyDown(KEY_D))
        _yaw += rotationSpeed;
    if (IsKeyDown(KEY_A))
        _yaw -= rotationSpeed;
    if (IsKeyDown(KEY_W))
        _pitch = clampFloat(_pitch + rotationSpeed, 18.0f, 78.0f);
    if (IsKeyDown(KEY_S))
        _pitch = clampFloat(_pitch - rotationSpeed, 18.0f, 78.0f);

    const float yaw = _yaw * DEG_TO_RAD;
    const Vector3 forward = {std::sin(yaw), 0.0f, std::cos(yaw)};
    const Vector3 right = {std::cos(yaw), 0.0f, -std::sin(yaw)};

    if (IsKeyDown(KEY_DOWN))
        _target = addVector(_target, scaleVector(forward, panSpeed));
    if (IsKeyDown(KEY_UP))
        _target = addVector(_target, scaleVector(forward, -panSpeed));
    if (IsKeyDown(KEY_RIGHT))
        _target = addVector(_target, scaleVector(right, panSpeed));
    if (IsKeyDown(KEY_LEFT))
        _target = addVector(_target, scaleVector(right, -panSpeed));
    if (IsKeyPressed(KEY_R))
        reset(world);

    const float pitch = _pitch * DEG_TO_RAD;
    _camera.position = {
        _target.x + _distance * std::cos(pitch) * std::sin(yaw),
        _target.y + _distance * std::sin(pitch),
        _target.z + _distance * std::cos(pitch) * std::cos(yaw)
    };
    _camera.target = _target;
}
