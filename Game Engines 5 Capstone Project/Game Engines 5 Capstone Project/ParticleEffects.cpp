#include "ParticleEffects.h"
#include <algorithm>
#include <assert.h>
#include <string>

inline float Deg2Rad(float a)
{
    return a * 0.01745329252f;
}

inline float Rad2Deg(float a)
{
    return a * 57.29577951f;
}

inline float clampf(float value, float min_inclusive, float max_inclusive)
{
    if (min_inclusive > max_inclusive)
    {
        std::swap(min_inclusive, max_inclusive);
    }
    return value < min_inclusive ? min_inclusive : value < max_inclusive ? value : max_inclusive;
}

inline void normalize_point(float x, float y, Point* out)
{
    float n = x * x + y * y;
    // Already normalized.
    if (n == 1.0f)
    {
        return;
    }

    n = sqrt(n);
    // Too close to zero.
    if (n < 1e-5)
    {
        return;
    }

    n = 1.0f / n;
    out->x = x * n;
    out->y = y * n;
}

/**
A more effect random number getter function, get from ejoy2d.
*/
inline static float RANDOM_M11(unsigned int* seed)
{
    *seed = *seed * 134775813 + 1;
    union
    {
        uint32_t d;
        float f;
    } u;
    u.d = (((uint32_t)(*seed) & 0x7fff) << 8) | 0x40000000;
    return u.f - 3.0f;
}

void ParticleEffects::ParticleSystem()
{
}

// implementation ParticleEffects

bool ParticleEffects::initWithTotalParticles(int numberOfParticles)
{
    _totalParticles = numberOfParticles;
    _isActive = true;
    _emitterMode = Mode::GRAVITY;
    _isAutoRemoveOnFinish = false;
    _transformSystemDirty = false;

    resetTotalParticles(numberOfParticles);

    return true;
}

void ParticleEffects::resetTotalParticles(int numberOfParticles)
{
    if (particle_data_.size() < numberOfParticles)
    {
        particle_data_.resize(numberOfParticles);
    }
}

void ParticleEffects::DestroyParticleSystem()
{
}

void ParticleEffects::addParticles(int count)
{
    if (_paused)
    {
        return;
    }
    uint32_t RANDSEED = rand();

    int start = _particleCount;
    _particleCount += count;

    //life
    for (int i = start; i < _particleCount; ++i)
    {
        float theLife = _life + _lifeVar * RANDOM_M11(&RANDSEED);
        particle_data_[i].timeToLive = (std::max)(0.0f, theLife);
    }

    //position
    for (int i = start; i < _particleCount; ++i)
    {
        particle_data_[i].posx = _sourcePosition.x + _posVar.x * RANDOM_M11(&RANDSEED);
    }

    for (int i = start; i < _particleCount; ++i)
    {
        particle_data_[i].posy = _sourcePosition.y + _posVar.y * RANDOM_M11(&RANDSEED);
    }

    //color
#define SET_COLOR(c, b, v)                                                 \
    for (int i = start; i < _particleCount; ++i)                           \
    {                                                                      \
        particle_data_[i].c = clampf(b + v * RANDOM_M11(&RANDSEED), 0, 1); \
    }

    SET_COLOR(colorR, _startColor.r, _startColorVar.r);
    SET_COLOR(colorG, _startColor.g, _startColorVar.g);
    SET_COLOR(colorB, _startColor.b, _startColorVar.b);
    SET_COLOR(colorA, _startColor.a, _startColorVar.a);

    SET_COLOR(deltaColorR, _endColor.r, _endColorVar.r);
    SET_COLOR(deltaColorG, _endColor.g, _endColorVar.g);
    SET_COLOR(deltaColorB, _endColor.b, _endColorVar.b);
    SET_COLOR(deltaColorA, _endColor.a, _endColorVar.a);

#define SET_DELTA_COLOR(c, dc)                                                                              \
    for (int i = start; i < _particleCount; ++i)                                                            \
    {                                                                                                       \
        particle_data_[i].dc = (particle_data_[i].dc - particle_data_[i].c) / particle_data_[i].timeToLive; \
    }

    SET_DELTA_COLOR(colorR, deltaColorR);
    SET_DELTA_COLOR(colorG, deltaColorG);
    SET_DELTA_COLOR(colorB, deltaColorB);
    SET_DELTA_COLOR(colorA, deltaColorA);

    //size
    for (int i = start; i < _particleCount; ++i)
    {
        particle_data_[i].size = _startSize + _startSizeVar * RANDOM_M11(&RANDSEED);
        particle_data_[i].size = (std::max)(0.0f, particle_data_[i].size);
    }

    if (_endSize != START_SIZE_EQUAL_TO_END_SIZE)
    {
        for (int i = start; i < _particleCount; ++i)
        {
            float endSize = _endSize + _endSizeVar * RANDOM_M11(&RANDSEED);
            endSize = (std::max)(0.0f, endSize);
            particle_data_[i].deltaSize = (endSize - particle_data_[i].size) / particle_data_[i].timeToLive;
        }
    }
    else
    {
        for (int i = start; i < _particleCount; ++i)
        {
            particle_data_[i].deltaSize = 0.0f;
        }
    }

    // rotation
    for (int i = start; i < _particleCount; ++i)
    {
        particle_data_[i].rotation = _startSpin + _startSpinVar * RANDOM_M11(&RANDSEED);
    }
    for (int i = start; i < _particleCount; ++i)
    {
        float endA = _endSpin + _endSpinVar * RANDOM_M11(&RANDSEED);
        particle_data_[i].deltaRotation = (endA - particle_data_[i].rotation) / particle_data_[i].timeToLive;
    }

    // position
    Vector2 pos;
    pos.x = x_;
    pos.y = y_;

    for (int i = start; i < _particleCount; ++i)
    {
        particle_data_[i].startPosX = pos.x;
    }
    for (int i = start; i < _particleCount; ++i)
    {
        particle_data_[i].startPosY = pos.y;
    }

    // Mode Gravity: A
    if (_emitterMode == Mode::GRAVITY)
    {

        // radial accel
        for (int i = start; i < _particleCount; ++i)
        {
            particle_data_[i].gravityMode.radialAccel = gravityMode.radialAccel + gravityMode.radialAccelVar * RANDOM_M11(&RANDSEED);
        }

        // tangential accel
        for (int i = start; i < _particleCount; ++i)
        {
            particle_data_[i].gravityMode.tangentialAccel = gravityMode.tangentialAccel + gravityMode.tangentialAccelVar * RANDOM_M11(&RANDSEED);
        }

        // rotation is dir
        if (gravityMode.rotationIsDir)
        {
            for (int i = start; i < _particleCount; ++i)
            {
                float a = Deg2Rad(_angle + _angleVar * RANDOM_M11(&RANDSEED));
                Vector2 v(cosf(a), sinf(a));
                float s = gravityMode.speed + gravityMode.speedVar * RANDOM_M11(&RANDSEED);
                Vector2 dir = v * s;
                particle_data_[i].gravityMode.dirX = dir.x;    //v * s ;
                particle_data_[i].gravityMode.dirY = dir.y;
                particle_data_[i].rotation = -Rad2Deg(dir.getAngle());
            }
        }
        else
        {
            for (int i = start; i < _particleCount; ++i)
            {
                float a = Deg2Rad(_angle + _angleVar * RANDOM_M11(&RANDSEED));
                Vector2 v(cosf(a), sinf(a));
                float s = gravityMode.speed + gravityMode.speedVar * RANDOM_M11(&RANDSEED);
                Vector2 dir = v * s;
                particle_data_[i].gravityMode.dirX = dir.x;    //v * s ;
                particle_data_[i].gravityMode.dirY = dir.y;
            }
        }
    }

    // Mode Radius: B
    else
    {
        for (int i = start; i < _particleCount; ++i)
        {
            particle_data_[i].radiusMode.radius = radiusMode.startRadius + radiusMode.startRadiusVar * RANDOM_M11(&RANDSEED);
        }

        for (int i = start; i < _particleCount; ++i)
        {
            particle_data_[i].radiusMode.angle = Deg2Rad(_angle + _angleVar * RANDOM_M11(&RANDSEED));
        }

        for (int i = start; i < _particleCount; ++i)
        {
            particle_data_[i].radiusMode.degreesPerSecond = Deg2Rad(radiusMode.rotatePerSecond + radiusMode.rotatePerSecondVar * RANDOM_M11(&RANDSEED));
        }

        if (radiusMode.endRadius == START_RADIUS_EQUAL_TO_END_RADIUS)
        {
            for (int i = start; i < _particleCount; ++i)
            {
                particle_data_[i].radiusMode.deltaRadius = 0.0f;
            }
        }
        else
        {
            for (int i = start; i < _particleCount; ++i)
            {
                float endRadius = radiusMode.endRadius + radiusMode.endRadiusVar * RANDOM_M11(&RANDSEED);
                particle_data_[i].radiusMode.deltaRadius = (endRadius - particle_data_[i].radiusMode.radius) / particle_data_[i].timeToLive;
            }
        }
    }
}

void ParticleEffects::stopSystem()
{
    _isActive = false;
    _elapsed = _duration;
    _emitCounter = 0;
}

void ParticleEffects::resetSystem()
{
    _isActive = true;
    _elapsed = 0;
    for (int i = 0; i < _particleCount; ++i)
    {
        //particle_data_[i].timeToLive = 0.0f;
    }
}

bool ParticleEffects::isFull()
{
    return (_particleCount == _totalParticles);
}

// ParticleEffects - MainLoop
void ParticleEffects::update()
{
    float dt = 1.0 / 25;
    if (_isActive && _emissionRate)
    {
        float rate = 1.0f / _emissionRate;
        int totalParticles = _totalParticles;

        //issue #1201, prevent bursts of particles, due to too high emitCounter
        if (_particleCount < totalParticles)
        {
            _emitCounter += dt;
            if (_emitCounter < 0.f)
            {
                _emitCounter = 0.f;
            }
        }

        int emitCount = (std::min)(1.0f * (totalParticles - _particleCount), _emitCounter / rate);
        addParticles(emitCount);
        _emitCounter -= rate * emitCount;

        _elapsed += dt;
        if (_elapsed < 0.f)
        {
            _elapsed = 0.f;
        }
        if (_duration != DURATION_INFINITY && _duration < _elapsed)
        {
            this->stopSystem();
        }
    }

    for (int i = 0; i < _particleCount; ++i)
    {
        particle_data_[i].timeToLive -= dt;
    }

    // rebirth
    for (int i = 0; i < _particleCount; ++i)
    {
        if (particle_data_[i].timeToLive <= 0.0f)
        {
            int j = _particleCount - 1;
            //while (j > 0 && particle_data_[i].timeToLive <= 0)
            //{
            //    _particleCount--;
            //    j--;
            //}
            particle_data_[i] = particle_data_[_particleCount - 1];
            --_particleCount;
        }
    }

    if (_emitterMode == Mode::GRAVITY)
    {
        for (int i = 0; i < _particleCount; ++i)
        {
            Point tmp, radial = { 0.0f, 0.0f }, tangential;

            // radial acceleration
            if (particle_data_[i].posx || particle_data_[i].posy)
            {
                normalize_point(particle_data_[i].posx, particle_data_[i].posy, &radial);
            }
            tangential = radial;
            radial.x *= particle_data_[i].gravityMode.radialAccel;
            radial.y *= particle_data_[i].gravityMode.radialAccel;

            // tangential acceleration
            std::swap(tangential.x, tangential.y);
            tangential.x *= -particle_data_[i].gravityMode.tangentialAccel;
            tangential.y *= particle_data_[i].gravityMode.tangentialAccel;

            // (gravity + radial + tangential) * dt
            tmp.x = radial.x + tangential.x + gravityMode.gravity.x;
            tmp.y = radial.y + tangential.y + gravityMode.gravity.y;
            tmp.x *= dt;
            tmp.y *= dt;

            particle_data_[i].gravityMode.dirX += tmp.x;
            particle_data_[i].gravityMode.dirY += tmp.y;

            // this is cocos2d-x v3.0
            // if (_configName.length()>0 && _yCoordFlipped != -1)

            // this is cocos2d-x v3.0
            tmp.x = particle_data_[i].gravityMode.dirX * dt * _yCoordFlipped;
            tmp.y = particle_data_[i].gravityMode.dirY * dt * _yCoordFlipped;
            particle_data_[i].posx += tmp.x;
            particle_data_[i].posy += tmp.y;
        }
    }
    else
    {
        for (int i = 0; i < _particleCount; ++i)
        {
            particle_data_[i].radiusMode.angle += particle_data_[i].radiusMode.degreesPerSecond * dt;
            particle_data_[i].radiusMode.radius += particle_data_[i].radiusMode.deltaRadius * dt;
            particle_data_[i].posx = -cosf(particle_data_[i].radiusMode.angle) * particle_data_[i].radiusMode.radius;
            particle_data_[i].posy = -sinf(particle_data_[i].radiusMode.angle) * particle_data_[i].radiusMode.radius * _yCoordFlipped;
        }
    }

    //color, size, rotation
    for (int i = 0; i < _particleCount; ++i)
    {
        particle_data_[i].colorR += particle_data_[i].deltaColorR * dt;
        particle_data_[i].colorG += particle_data_[i].deltaColorG * dt;
        particle_data_[i].colorB += particle_data_[i].deltaColorB * dt;
        particle_data_[i].colorA += particle_data_[i].deltaColorA * dt;
        particle_data_[i].size += (particle_data_[i].deltaSize * dt);
        particle_data_[i].size = (std::max)(0.0f, particle_data_[i].size);
        particle_data_[i].rotation += particle_data_[i].deltaRotation * dt;
    }
}

// ParticleEffects - Texture protocol
void ParticleEffects::setTexture(SDL_Texture* var)
{
    if (_texture != var)
    {
        _texture = var;
    }
}

void ParticleEffects::draw()
{
    if (_texture == nullptr)
    {
        return;
    }
    for (int i = 0; i < _particleCount; i++)
    {
        auto& p = particle_data_[i];
        if (p.size <= 0 || p.colorA <= 0)
        {
            continue;
        }
        SDL_Rect r = { int(p.posx + p.startPosX - p.size / 2), int(p.posy + p.startPosY - p.size / 2), int(p.size), int(p.size) };
        SDL_Color c = { Uint8(p.colorR * 255), Uint8(p.colorG * 255), Uint8(p.colorB * 255), Uint8(p.colorA * 255) };
        SDL_SetTextureColorMod(_texture, c.r, c.g, c.b);
        SDL_SetTextureAlphaMod(_texture, c.a);
        SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopyEx(_renderer, _texture, nullptr, &r, p.rotation, nullptr, SDL_FLIP_NONE);
    }
    update();
}

SDL_Texture* ParticleEffects::getTexture()
{
    return _texture;
}

// ParticleEffects - Properties of Gravity Mode
void ParticleEffects::setTangentialAccel(float t)
{
    gravityMode.tangentialAccel = t;
}

float ParticleEffects::getTangentialAccel() const
{
    return gravityMode.tangentialAccel;
}

void ParticleEffects::setTangentialAccelVar(float t)
{
    gravityMode.tangentialAccelVar = t;
}

float ParticleEffects::getTangentialAccelVar() const
{
    return gravityMode.tangentialAccelVar;
}

void ParticleEffects::setRadialAccel(float t)
{
    gravityMode.radialAccel = t;
}

float ParticleEffects::getRadialAccel() const
{
    return gravityMode.radialAccel;
}

void ParticleEffects::setRadialAccelVar(float t)
{
    gravityMode.radialAccelVar = t;
}

float ParticleEffects::getRadialAccelVar() const
{
    return gravityMode.radialAccelVar;
}

void ParticleEffects::setRotationIsDir(bool t)
{
    gravityMode.rotationIsDir = t;
}

bool ParticleEffects::getRotationIsDir() const
{
    return gravityMode.rotationIsDir;
}

void ParticleEffects::setGravity(const Vector2& g)
{
    gravityMode.gravity = g;
}

const Vector2& ParticleEffects::getGravity()
{
    return gravityMode.gravity;
}

void ParticleEffects::setSpeed(float speed)
{
    gravityMode.speed = speed;
}

float ParticleEffects::getSpeed() const
{
    return gravityMode.speed;
}

void ParticleEffects::setSpeedVar(float speedVar)
{

    gravityMode.speedVar = speedVar;
}

float ParticleEffects::getSpeedVar() const
{

    return gravityMode.speedVar;
}

// ParticleEffects - Properties of Radius Mode
void ParticleEffects::setStartRadius(float startRadius)
{
    radiusMode.startRadius = startRadius;
}

float ParticleEffects::getStartRadius() const
{
    return radiusMode.startRadius;
}

void ParticleEffects::setStartRadiusVar(float startRadiusVar)
{
    radiusMode.startRadiusVar = startRadiusVar;
}

float ParticleEffects::getStartRadiusVar() const
{
    return radiusMode.startRadiusVar;
}

void ParticleEffects::setEndRadius(float endRadius)
{
    radiusMode.endRadius = endRadius;
}

float ParticleEffects::getEndRadius() const
{
    return radiusMode.endRadius;
}

void ParticleEffects::setEndRadiusVar(float endRadiusVar)
{
    radiusMode.endRadiusVar = endRadiusVar;
}

float ParticleEffects::getEndRadiusVar() const
{

    return radiusMode.endRadiusVar;
}

void ParticleEffects::setRotatePerSecond(float degrees)
{
    radiusMode.rotatePerSecond = degrees;
}

float ParticleEffects::getRotatePerSecond() const
{
    return radiusMode.rotatePerSecond;
}

void ParticleEffects::setRotatePerSecondVar(float degrees)
{
    radiusMode.rotatePerSecondVar = degrees;
}

float ParticleEffects::getRotatePerSecondVar() const
{
    return radiusMode.rotatePerSecondVar;
}

bool ParticleEffects::isActive() const
{
    return _isActive;
}

int ParticleEffects::getTotalParticles() const
{
    return _totalParticles;
}

void ParticleEffects::setTotalParticles(int var)
{
    _totalParticles = var;
}

bool ParticleEffects::isAutoRemoveOnFinish() const
{
    return _isAutoRemoveOnFinish;
}

void ParticleEffects::setAutoRemoveOnFinish(bool var)
{
    _isAutoRemoveOnFinish = var;
}

////don't use a transform matrix, this is faster
//void ParticleEffects::setScale(float s)
//{
//    _transformSystemDirty = true;
//    Node::setScale(s);
//}
//
//void ParticleEffects::setRotation(float newRotation)
//{
//    _transformSystemDirty = true;
//    Node::setRotation(newRotation);
//}
//
//void ParticleEffects::setScaleX(float newScaleX)
//{
//    _transformSystemDirty = true;
//    Node::setScaleX(newScaleX);
//}
//
//void ParticleEffects::setScaleY(float newScaleY)
//{
//    _transformSystemDirty = true;
//    Node::setScaleY(newScaleY);
//}

bool ParticleEffects::isPaused() const
{
    return _paused;
}

void ParticleEffects::pauseEmissions()
{
    _paused = true;
}

void ParticleEffects::resumeEmissions()
{
    _paused = false;
}

void ParticleEffects::setStyle(ParticleStyle style)
{
    if (style_ == style)
    {
        return;
    }
    style_ = style;
    if (style == NONE)
    {
        stopSystem();
    }
    if (_texture == nullptr)
    {
        setTexture(getDefaultTexture());
    }
    switch (style)
    {
    case ParticleEffects::FIRE:
    {
        initWithTotalParticles(250);

        // duration
        _duration = DURATION_INFINITY;

        // Gravity Mode
        this->_emitterMode = Mode::GRAVITY;

        // Gravity Mode: gravity
        this->gravityMode.gravity = { 0, 0 };

        // Gravity Mode: radial acceleration
        this->gravityMode.radialAccel = 0;
        this->gravityMode.radialAccelVar = 0;

        // Gravity Mode: speed of particles
        this->gravityMode.speed = -60;
        this->gravityMode.speedVar = 20;

        // starting angle
        _angle = 90;
        _angleVar = 10;

        // life of particles
        _life = 3;
        _lifeVar = 0.25f;

        // size, in pixels
        _startSize = 54.0f;
        _startSizeVar = 10.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per frame
        _emissionRate = _totalParticles / _life;

        // color of particles
        _startColor.r = 0.76f;
        _startColor.g = 0.25f;
        _startColor.b = 0.12f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.0f;
        _startColorVar.g = 0.0f;
        _startColorVar.b = 0.0f;
        _startColorVar.a = 0.0f;
        _endColor.r = 0.0f;
        _endColor.g = 0.0f;
        _endColor.b = 0.0f;
        _endColor.a = 0.0f;
        _endColorVar.r = 0.0f;
        _endColorVar.g = 0.0f;
        _endColorVar.b = 0.0f;
        _endColorVar.a = 0.0f;

        _posVar = { 40.0f, 20.0f };
        break;
    }
    case ParticleEffects::FIRE_WORK:
    {
        initWithTotalParticles(1500);

        // duration
        _duration = DURATION_INFINITY;

        // Gravity Mode
        this->_emitterMode = Mode::GRAVITY;

        // Gravity Mode: gravity
        this->gravityMode.gravity = { 0.0f, 90.0f };

        // Gravity Mode:  radial
        this->gravityMode.radialAccel = 0.0f;
        this->gravityMode.radialAccelVar = 0.0f;

        //  Gravity Mode: speed of particles
        this->gravityMode.speed = -180.0f;
        this->gravityMode.speedVar = 50.0f;

        // angle
        this->_angle = 90.0f;
        this->_angleVar = 20.0f;

        // life of particles
        this->_life = 3.5f;
        this->_lifeVar = 1.0f;

        // emits per frame
        this->_emissionRate = _totalParticles / _life;

        // color of particles
        _startColor.r = 0.5f;
        _startColor.g = 0.5f;
        _startColor.b = 0.5f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.5f;
        _startColorVar.g = 0.5f;
        _startColorVar.b = 0.5f;
        _startColorVar.a = 0.1f;
        _endColor.r = 0.1f;
        _endColor.g = 0.1f;
        _endColor.b = 0.1f;
        _endColor.a = 0.2f;
        _endColorVar.r = 0.1f;
        _endColorVar.g = 0.1f;
        _endColorVar.b = 0.1f;
        _endColorVar.a = 0.2f;

        // size, in pixels
        _startSize = 8.0f;
        _startSizeVar = 2.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;

        _posVar = { 0, 0 };
        break;
    }
    case ParticleEffects::SUN:
    {
        initWithTotalParticles(350);

        // additive
        //this->setBlendAdditive(true);

        // duration
        _duration = DURATION_INFINITY;

        // Gravity Mode
        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vector2(0, 0));

        // Gravity mode: radial acceleration
        setRadialAccel(0);
        setRadialAccelVar(0);

        // Gravity mode: speed of particles
        setSpeed(-20);
        setSpeedVar(5);

        // angle
        _angle = 90;
        _angleVar = 360;

        // life of particles
        _life = 1;
        _lifeVar = 0.5f;

        // size, in pixels
        _startSize = 30.0f;
        _startSizeVar = 10.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per seconds
        _emissionRate = _totalParticles / _life;

        // color of particles
        _startColor.r = 0.76f;
        _startColor.g = 0.25f;
        _startColor.b = 0.12f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.0f;
        _startColorVar.g = 0.0f;
        _startColorVar.b = 0.0f;
        _startColorVar.a = 0.0f;
        _endColor.r = 0.0f;
        _endColor.g = 0.0f;
        _endColor.b = 0.0f;
        _endColor.a = 1.0f;
        _endColorVar.r = 0.0f;
        _endColorVar.g = 0.0f;
        _endColorVar.b = 0.0f;
        _endColorVar.a = 0.0f;

        _posVar = { 0, 0 };
        break;
    }
    case ParticleEffects::GALAXY:
    {
        initWithTotalParticles(200);
        // duration
        _duration = DURATION_INFINITY;

        // Gravity Mode
        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vector2(0, 0));

        // Gravity Mode: speed of particles
        setSpeed(-60);
        setSpeedVar(10);

        // Gravity Mode: radial
        setRadialAccel(-80);
        setRadialAccelVar(0);

        // Gravity Mode: tangential
        setTangentialAccel(80);
        setTangentialAccelVar(0);

        // angle
        _angle = 90;
        _angleVar = 360;

        // life of particles
        _life = 4;
        _lifeVar = 1;

        // size, in pixels
        _startSize = 37.0f;
        _startSizeVar = 10.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        _emissionRate = _totalParticles / _life;

        // color of particles
        _startColor.r = 0.12f;
        _startColor.g = 0.25f;
        _startColor.b = 0.76f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.0f;
        _startColorVar.g = 0.0f;
        _startColorVar.b = 0.0f;
        _startColorVar.a = 0.0f;
        _endColor.r = 0.0f;
        _endColor.g = 0.0f;
        _endColor.b = 0.0f;
        _endColor.a = 1.0f;
        _endColorVar.r = 0.0f;
        _endColorVar.g = 0.0f;
        _endColorVar.b = 0.0f;
        _endColorVar.a = 0.0f;

        _posVar = { 0, 0 };
        break;
    }
    case ParticleEffects::FLOWER:
    {
        initWithTotalParticles(250);

        // duration
        _duration = DURATION_INFINITY;

        // Gravity Mode
        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vector2(0, 0));

        // Gravity Mode: speed of particles
        setSpeed(-80);
        setSpeedVar(10);

        // Gravity Mode: radial
        setRadialAccel(-60);
        setRadialAccelVar(0);

        // Gravity Mode: tangential
        setTangentialAccel(15);
        setTangentialAccelVar(0);

        // angle
        _angle = 90;
        _angleVar = 360;

        // life of particles
        _life = 4;
        _lifeVar = 1;

        // size, in pixels
        _startSize = 30.0f;
        _startSizeVar = 10.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        _emissionRate = _totalParticles / _life;

        // color of particles
        _startColor.r = 0.50f;
        _startColor.g = 0.50f;
        _startColor.b = 0.50f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.5f;
        _startColorVar.g = 0.5f;
        _startColorVar.b = 0.5f;
        _startColorVar.a = 0.5f;
        _endColor.r = 0.0f;
        _endColor.g = 0.0f;
        _endColor.b = 0.0f;
        _endColor.a = 1.0f;
        _endColorVar.r = 0.0f;
        _endColorVar.g = 0.0f;
        _endColorVar.b = 0.0f;
        _endColorVar.a = 0.0f;

        _posVar = { 0, 0 };
        break;
    }
    case ParticleEffects::METEOR:
    {
        initWithTotalParticles(150);

        // duration
        _duration = DURATION_INFINITY;

        // Gravity Mode
        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vector2(-200, -200));

        // Gravity Mode: speed of particles
        setSpeed(-15);
        setSpeedVar(5);

        // Gravity Mode: radial
        setRadialAccel(0);
        setRadialAccelVar(0);

        // Gravity Mode: tangential
        setTangentialAccel(0);
        setTangentialAccelVar(0);

        // angle
        _angle = 90;
        _angleVar = 360;

        // life of particles
        _life = 2;
        _lifeVar = 1;

        // size, in pixels
        _startSize = 60.0f;
        _startSizeVar = 10.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        _emissionRate = _totalParticles / _life;

        // color of particles
        _startColor.r = 0.2f;
        _startColor.g = 0.4f;
        _startColor.b = 0.7f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.0f;
        _startColorVar.g = 0.0f;
        _startColorVar.b = 0.2f;
        _startColorVar.a = 0.1f;
        _endColor.r = 0.0f;
        _endColor.g = 0.0f;
        _endColor.b = 0.0f;
        _endColor.a = 1.0f;
        _endColorVar.r = 0.0f;
        _endColorVar.g = 0.0f;
        _endColorVar.b = 0.0f;
        _endColorVar.a = 0.0f;

        _posVar = { 0, 0 };
        break;
    }
    case ParticleEffects::SPIRAL:
    {
        initWithTotalParticles(500);

        // duration
        _duration = DURATION_INFINITY;

        // Gravity Mode
        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vector2(0, 0));

        // Gravity Mode: speed of particles
        setSpeed(-150);
        setSpeedVar(0);

        // Gravity Mode: radial
        setRadialAccel(-380);
        setRadialAccelVar(0);

        // Gravity Mode: tangential
        setTangentialAccel(45);
        setTangentialAccelVar(0);

        // angle
        _angle = 90;
        _angleVar = 0;

        // life of particles
        _life = 12;
        _lifeVar = 0;

        // size, in pixels
        _startSize = 20.0f;
        _startSizeVar = 0.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        _emissionRate = _totalParticles / _life;

        // color of particles
        _startColor.r = 0.5f;
        _startColor.g = 0.5f;
        _startColor.b = 0.5f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.5f;
        _startColorVar.g = 0.5f;
        _startColorVar.b = 0.5f;
        _startColorVar.a = 0.0f;
        _endColor.r = 0.5f;
        _endColor.g = 0.5f;
        _endColor.b = 0.5f;
        _endColor.a = 1.0f;
        _endColorVar.r = 0.5f;
        _endColorVar.g = 0.5f;
        _endColorVar.b = 0.5f;
        _endColorVar.a = 0.0f;

        _posVar = { 0, 0 };
        break;
    }
    case ParticleEffects::EXPLOSION:
    {
        initWithTotalParticles(700);

        // duration
        _duration = 0.1f;

        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vector2(0, 0));

        // Gravity Mode: speed of particles
        setSpeed(-70);
        setSpeedVar(40);

        // Gravity Mode: radial
        setRadialAccel(0);
        setRadialAccelVar(0);

        // Gravity Mode: tangential
        setTangentialAccel(0);
        setTangentialAccelVar(0);

        // angle
        _angle = 90;
        _angleVar = 360;

        // life of particles
        _life = 5.0f;
        _lifeVar = 2;

        // size, in pixels
        _startSize = 15.0f;
        _startSizeVar = 10.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        _emissionRate = _totalParticles / _duration;

        // color of particles
        _startColor.r = 0.7f;
        _startColor.g = 0.1f;
        _startColor.b = 0.2f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.5f;
        _startColorVar.g = 0.5f;
        _startColorVar.b = 0.5f;
        _startColorVar.a = 0.0f;
        _endColor.r = 0.5f;
        _endColor.g = 0.5f;
        _endColor.b = 0.5f;
        _endColor.a = 0.0f;
        _endColorVar.r = 0.5f;
        _endColorVar.g = 0.5f;
        _endColorVar.b = 0.5f;
        _endColorVar.a = 0.0f;

        _posVar = { 0, 0 };
        break;
    }
    case ParticleEffects::SMOKE:
    {
        initWithTotalParticles(200);

        // duration
        _duration = DURATION_INFINITY;

        // Emitter mode: Gravity Mode
        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vector2(0, 0));

        // Gravity Mode: radial acceleration
        setRadialAccel(0);
        setRadialAccelVar(0);

        // Gravity Mode: speed of particles
        setSpeed(-25);
        setSpeedVar(10);

        // angle
        _angle = 90;
        _angleVar = 5;

        // life of particles
        _life = 4;
        _lifeVar = 1;

        // size, in pixels
        _startSize = 60.0f;
        _startSizeVar = 10.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per frame
        _emissionRate = _totalParticles / _life;

        // color of particles
        _startColor.r = 0.8f;
        _startColor.g = 0.8f;
        _startColor.b = 0.8f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.02f;
        _startColorVar.g = 0.02f;
        _startColorVar.b = 0.02f;
        _startColorVar.a = 0.0f;
        _endColor.r = 0.0f;
        _endColor.g = 0.0f;
        _endColor.b = 0.0f;
        _endColor.a = 1.0f;
        _endColorVar.r = 0.0f;
        _endColorVar.g = 0.0f;
        _endColorVar.b = 0.0f;
        _endColorVar.a = 0.0f;

        _posVar = { 20.0f, 0.0f };
        break;
    }
    case ParticleEffects::SNOW:
    {
        initWithTotalParticles(700);

        // duration
        _duration = DURATION_INFINITY;

        // set gravity mode.
        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vector2(0, 1));

        // Gravity Mode: speed of particles
        setSpeed(-5);
        setSpeedVar(1);

        // Gravity Mode: radial
        setRadialAccel(0);
        setRadialAccelVar(1);

        // Gravity mode: tangential
        setTangentialAccel(0);
        setTangentialAccelVar(1);

        // angle
        _angle = -90;
        _angleVar = 5;

        // life of particles
        _life = 45;
        _lifeVar = 15;

        // size, in pixels
        _startSize = 10.0f;
        _startSizeVar = 5.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        _emissionRate = 10;

        // color of particles
        _startColor.r = 1.0f;
        _startColor.g = 1.0f;
        _startColor.b = 1.0f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.0f;
        _startColorVar.g = 0.0f;
        _startColorVar.b = 0.0f;
        _startColorVar.a = 0.0f;
        _endColor.r = 1.0f;
        _endColor.g = 1.0f;
        _endColor.b = 1.0f;
        _endColor.a = 0.0f;
        _endColorVar.r = 0.0f;
        _endColorVar.g = 0.0f;
        _endColorVar.b = 0.0f;
        _endColorVar.a = 0.0f;

        _posVar = { 1.0f * x_, 0.0f };
        break;
    }
    case ParticleEffects::RAIN:
    {
        initWithTotalParticles(1000);

        // duration
        _duration = DURATION_INFINITY;

        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vector2(10, 10));

        // Gravity Mode: radial
        setRadialAccel(0);
        setRadialAccelVar(1);

        // Gravity Mode: tangential
        setTangentialAccel(0);
        setTangentialAccelVar(1);

        // Gravity Mode: speed of particles
        setSpeed(-130);
        setSpeedVar(30);

        // angle
        _angle = -90;
        _angleVar = 5;

        // life of particles
        _life = 4.5f;
        _lifeVar = 0;

        // size, in pixels
        _startSize = 4.0f;
        _startSizeVar = 2.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        _emissionRate = 20;

        // color of particles
        _startColor.r = 0.7f;
        _startColor.g = 0.8f;
        _startColor.b = 1.0f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.0f;
        _startColorVar.g = 0.0f;
        _startColorVar.b = 0.0f;
        _startColorVar.a = 0.0f;
        _endColor.r = 0.7f;
        _endColor.g = 0.8f;
        _endColor.b = 1.0f;
        _endColor.a = 0.5f;
        _endColorVar.r = 0.0f;
        _endColorVar.g = 0.0f;
        _endColorVar.b = 0.0f;
        _endColorVar.a = 0.0f;

        _posVar = { 1.0f * x_, 0.0f };
        break;
    }
    default:
        break;
    }
}