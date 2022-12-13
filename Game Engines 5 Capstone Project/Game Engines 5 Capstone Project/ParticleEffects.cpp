#include "ParticleEffects.h"
#include <algorithm>
#include <assert.h>
#include <string>

inline float DegreesToRadians(float a)
{
    return a * 0.01745329252f;
}

inline float RadiansToDegrees(float a)
{
    return a * 57.29577951f;
}

/* Clamped floating point types (found in https://docs.rs/clampf/latest/clampf/)
Provides wrappers around floating point values clamped to the range[0, 1] */

inline float ClampFloat(float value, float min_inclusive, float max_inclusive)
{
    if (min_inclusive > max_inclusive)
    {
        std::swap(min_inclusive, max_inclusive);
    }
    return value < min_inclusive ? min_inclusive : value < max_inclusive ? value : max_inclusive;
}

inline void NormalizePoint(float x, float y, Point* out)
{
    float n = x * x + y * y;

    // If it's already normalized, return nothing
    if (n == 1.0f)
    {
        return;
    }

    n = sqrt(n);

    // If it's too close to zero (1e-5 = 0.00001), return nothing
    if (n < 1e-5)
    {
        return;
    }

    // Normalized point
    n = 1.0f / n;
    out->x = x * n;
    out->y = y * n;
}

// Random number getter function
inline static float GetRandomNumber(unsigned int* seed)
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

bool ParticleEffects::initWithTotalParticles(int numberOfParticles)
{
    totalParticles = numberOfParticles;
    isParticleActive = true;
    emitterMode = Mode::GRAVITY;
    isAutoRemovedOnFinish = false;
    isTransformSystemDirty = false;

    resetTotalParticles(numberOfParticles);

    return true;
}

void ParticleEffects::resetTotalParticles(int numberOfParticles)
{
    if (particleInfo.size() < numberOfParticles)
    {
        particleInfo.resize(numberOfParticles);
    }
}

void ParticleEffects::DestroyParticleSystem()
{
}

void ParticleEffects::addParticles(int count)
{
    if (isParticlePaused)
    {
        return;
    }
    uint32_t randomNum = rand();

    int start = particleCount;
    particleCount += count;

    // Life
    for (int i = start; i < particleCount; ++i)
    {
        float theLife = life + lifeVar * GetRandomNumber(&randomNum);
        particleInfo[i].timeToLive = (std::max)(0.0f, theLife);
    }

    // Position
    for (int i = start; i < particleCount; ++i)
    {
        particleInfo[i].posx = sourcePos.x + posVariance.x * GetRandomNumber(&randomNum);
    }

    for (int i = start; i < particleCount; ++i)
    {
        particleInfo[i].posy = sourcePos.y + posVariance.y * GetRandomNumber(&randomNum);
    }

    /* Set the color
      \ slash allows you to #define a variable inside a function and creates a variable
      for the ParticleEffects class (only in .cpp file)
      So without the / slash, there will be errors with ParticleEffects class not having a variable
      inside of the #define variable (this is a macro call, by the way)
    */ 
#define SET_COLOR(c, b, v)                                                 \
    for (int i = start; i < particleCount; ++i)                           \
    {                                                                      \
        particleInfo[i].c = ClampFloat(b + v * GetRandomNumber(&randomNum), 0, 1); \
    }

    SET_COLOR(colorR, startColor.r, startColorVar.r);
    SET_COLOR(colorG, startColor.g, startColorVar.g);
    SET_COLOR(colorB, startColor.b, startColorVar.b);
    SET_COLOR(colorA, startColor.a, startColorVar.a);

    SET_COLOR(deltaColorR, endColor.r, endColorVar.r);
    SET_COLOR(deltaColorG, endColor.g, endColorVar.g);
    SET_COLOR(deltaColorB, endColor.b, endColorVar.b);
    SET_COLOR(deltaColorA, endColor.a, endColorVar.a);

    // Set the delta color
#define SET_DELTA_COLOR(c, dc)                                                                              \
    for (int i = start; i < particleCount; ++i)                                                            \
    {                                                                                                       \
        particleInfo[i].dc = (particleInfo[i].dc - particleInfo[i].c) / particleInfo[i].timeToLive; \
    }

    SET_DELTA_COLOR(colorR, deltaColorR);
    SET_DELTA_COLOR(colorG, deltaColorG);
    SET_DELTA_COLOR(colorB, deltaColorB);
    SET_DELTA_COLOR(colorA, deltaColorA);

    // Size
    for (int i = start; i < particleCount; ++i)
    {
        particleInfo[i].size = startSize + startSizeVar * GetRandomNumber(&randomNum);
        particleInfo[i].size = (std::max)(0.0f, particleInfo[i].size);
    }

    if (endSize != START_SIZE_EQUAL_TO_END_SIZE)
    {
        for (int i = start; i < particleCount; ++i)
        {
            float localEndSize = endSize + endSizeVar * GetRandomNumber(&randomNum);
            localEndSize = (std::max)(0.0f, localEndSize);
            particleInfo[i].deltaSize = (localEndSize - particleInfo[i].size) / particleInfo[i].timeToLive;
        }
    }
    else
    {
        for (int i = start; i < particleCount; ++i)
        {
            particleInfo[i].deltaSize = 0.0f;
        }
    }

    // Rotation
    for (int i = start; i < particleCount; ++i)
    {
        particleInfo[i].rotation = startSpin + startSpinVar * GetRandomNumber(&randomNum);
    }
    for (int i = start; i < particleCount; ++i)
    {
        float endA = endSpin + endSpinVar * GetRandomNumber(&randomNum);
        particleInfo[i].deltaRotation = (endA - particleInfo[i].rotation) / particleInfo[i].timeToLive;
    }

    // Local position variable
    Vector2 pos;
    pos.x = x;
    pos.y = y;

    for (int i = start; i < particleCount; ++i)
    {
        particleInfo[i].startPosX = pos.x;
    }
    for (int i = start; i < particleCount; ++i)
    {
        particleInfo[i].startPosY = pos.y;
    }

    // Gravity mode
    if (emitterMode == Mode::GRAVITY)
    {

        // radial accel
        for (int i = start; i < particleCount; ++i)
        {
            particleInfo[i].gravityMode.radialAccel = gravityMode.radialAccel + gravityMode.radialAccelVar * GetRandomNumber(&randomNum);
        }

        // tangential accel
        for (int i = start; i < particleCount; ++i)
        {
            particleInfo[i].gravityMode.tangentialAccel = gravityMode.tangentialAccel + gravityMode.tangentialAccelVar * GetRandomNumber(&randomNum);
        }

        // rotation is dir
        if (gravityMode.rotationIsDir)
        {
            for (int i = start; i < particleCount; ++i)
            {
                float a = DegreesToRadians(angle + angleVar * GetRandomNumber(&randomNum));
                Vector2 v(cosf(a), sinf(a));
                float s = gravityMode.speed + gravityMode.speedVar * GetRandomNumber(&randomNum);
                Vector2 dir = v * s;
                particleInfo[i].gravityMode.dirX = dir.x;    //v * s ;
                particleInfo[i].gravityMode.dirY = dir.y;
                particleInfo[i].rotation = -RadiansToDegrees(dir.getAngle());
            }
        }
        else
        {
            for (int i = start; i < particleCount; ++i)
            {
                float a = DegreesToRadians(angle + angleVar * GetRandomNumber(&randomNum));
                Vector2 v(cosf(a), sinf(a));
                float s = gravityMode.speed + gravityMode.speedVar * GetRandomNumber(&randomNum);
                Vector2 dir = v * s;
                particleInfo[i].gravityMode.dirX = dir.x;
                particleInfo[i].gravityMode.dirY = dir.y;
            }
        }
    }

    // Radius mode
    else
    {
        for (int i = start; i < particleCount; ++i)
        {
            particleInfo[i].radiusMode.radius = radiusMode.startRadius + radiusMode.startRadiusVar * GetRandomNumber(&randomNum);
        }

        for (int i = start; i < particleCount; ++i)
        {
            particleInfo[i].radiusMode.angle = DegreesToRadians(angle + angleVar * GetRandomNumber(&randomNum));
        }

        for (int i = start; i < particleCount; ++i)
        {
            particleInfo[i].radiusMode.degreesPerSecond = DegreesToRadians(radiusMode.rotatePerSecond + radiusMode.rotatePerSecondVar * GetRandomNumber(&randomNum));
        }

        if (radiusMode.endRadius == START_RADIUS_EQUAL_TO_END_RADIUS)
        {
            for (int i = start; i < particleCount; ++i)
            {
                particleInfo[i].radiusMode.deltaRadius = 0.0f;
            }
        }
        else
        {
            for (int i = start; i < particleCount; ++i)
            {
                float endRadius = radiusMode.endRadius + radiusMode.endRadiusVar * GetRandomNumber(&randomNum);
                particleInfo[i].radiusMode.deltaRadius = (endRadius - particleInfo[i].radiusMode.radius) / particleInfo[i].timeToLive;
            }
        }
    }
}

void ParticleEffects::stopSystem()
{
    isParticleActive = false;
    elapsedTIme = duration;
    emitterCounter = 0;
}

void ParticleEffects::resetSystem()
{
    isParticleActive = true;
    elapsedTIme = 0;
    for (int i = 0; i < particleCount; ++i)
    {
        //particleInfo[i].timeToLive = 0.0f;
    }
}

bool ParticleEffects::isFull()
{
    return (particleCount == totalParticles);
}

void ParticleEffects::update()
{
    float dt = 1.0 / 25;
    if (isParticleActive && emissionRate)
    {
        float rate = 1.0f / emissionRate;
        int totalNumOfParticles = totalParticles;

        // Prevents bursts of particles, due to too high emitCounter
        if (particleCount < totalNumOfParticles)
        {
            emitterCounter += dt;
            if (emitterCounter < 0.f)
            {
                emitterCounter = 0.f;
            }
        }

        int emitCount = (std::min)(1.0f * (totalNumOfParticles - particleCount), emitterCounter / rate);
        addParticles(emitCount);
        emitterCounter -= rate * emitCount;

        elapsedTIme += dt;
        if (elapsedTIme < 0.f)
        {
            elapsedTIme = 0.f;
        }
        if (duration != DURATION_INFINITY && duration < elapsedTIme)
        {
            this->stopSystem();
        }
    }

    for (int i = 0; i < particleCount; ++i)
    {
        particleInfo[i].timeToLive -= dt;
    }

    // rebirth
    for (int i = 0; i < particleCount; ++i)
    {
        if (particleInfo[i].timeToLive <= 0.0f)
        {
            int j = particleCount - 1;
            //while (j > 0 && particleInfo[i].timeToLive <= 0)
            //{
            //    particleCount--;
            //    j--;
            //}
            particleInfo[i] = particleInfo[particleCount - 1];
            --particleCount;
        }
    }

    if (emitterMode == Mode::GRAVITY)
    {
        for (int i = 0; i < particleCount; ++i)
        {
            Point tmp, radial = { 0.0f, 0.0f }, tangential;

            // radial acceleration
            if (particleInfo[i].posx || particleInfo[i].posy)
            {
                NormalizePoint(particleInfo[i].posx, particleInfo[i].posy, &radial);
            }
            tangential = radial;
            radial.x *= particleInfo[i].gravityMode.radialAccel;
            radial.y *= particleInfo[i].gravityMode.radialAccel;

            // tangential acceleration
            std::swap(tangential.x, tangential.y);
            tangential.x *= -particleInfo[i].gravityMode.tangentialAccel;
            tangential.y *= particleInfo[i].gravityMode.tangentialAccel;

            // (gravity + radial + tangential) * dt
            tmp.x = radial.x + tangential.x + gravityMode.gravity.x;
            tmp.y = radial.y + tangential.y + gravityMode.gravity.y;
            tmp.x *= dt;
            tmp.y *= dt;

            particleInfo[i].gravityMode.dirX += tmp.x;
            particleInfo[i].gravityMode.dirY += tmp.y;

            tmp.x = particleInfo[i].gravityMode.dirX * dt * flippedY;
            tmp.y = particleInfo[i].gravityMode.dirY * dt * flippedY;
            particleInfo[i].posx += tmp.x;
            particleInfo[i].posy += tmp.y;
        }
    }
    else
    {
        for (int i = 0; i < particleCount; ++i)
        {
            particleInfo[i].radiusMode.angle += particleInfo[i].radiusMode.degreesPerSecond * dt;
            particleInfo[i].radiusMode.radius += particleInfo[i].radiusMode.deltaRadius * dt;
            particleInfo[i].posx = -cosf(particleInfo[i].radiusMode.angle) * particleInfo[i].radiusMode.radius;
            particleInfo[i].posy = -sinf(particleInfo[i].radiusMode.angle) * particleInfo[i].radiusMode.radius * flippedY;
        }
    }

    //color, size, rotation
    for (int i = 0; i < particleCount; ++i)
    {
        particleInfo[i].colorR += particleInfo[i].deltaColorR * dt;
        particleInfo[i].colorG += particleInfo[i].deltaColorG * dt;
        particleInfo[i].colorB += particleInfo[i].deltaColorB * dt;
        particleInfo[i].colorA += particleInfo[i].deltaColorA * dt;
        particleInfo[i].size += (particleInfo[i].deltaSize * dt);
        particleInfo[i].size = (std::max)(0.0f, particleInfo[i].size);
        particleInfo[i].rotation += particleInfo[i].deltaRotation * dt;
    }
}

void ParticleEffects::setTexture(SDL_Texture* var)
{
    if (particleTex != var)
    {
        particleTex = var;
    }
}

void ParticleEffects::draw()
{
    if (particleTex == nullptr)
    {
        return;
    }
    for (int i = 0; i < particleCount; i++)
    {
        auto& p = particleInfo[i];
        if (p.size <= 0 || p.colorA <= 0)
        {
            continue;
        }
        SDL_Rect r = { int(p.posx + p.startPosX - p.size / 2), int(p.posy + p.startPosY - p.size / 2), int(p.size), int(p.size) };
        SDL_Color c = { Uint8(p.colorR * 255), Uint8(p.colorG * 255), Uint8(p.colorB * 255), Uint8(p.colorA * 255) };
        SDL_SetTextureColorMod(particleTex, c.r, c.g, c.b);
        SDL_SetTextureAlphaMod(particleTex, c.a);
        SDL_SetTextureBlendMode(particleTex, SDL_BLENDMODE_BLEND);
        SDL_RenderCopyEx(renderer, particleTex, nullptr, &r, p.rotation, nullptr, SDL_FLIP_NONE);
    }
    update();
}

SDL_Texture* ParticleEffects::getTexture()
{
    return particleTex;
}

// Set the properties of Gravity Mode variables
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

// Set the properties of Radius Mode variables
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
    return isParticleActive;
}

int ParticleEffects::getTotalParticles() const
{
    return totalParticles;
}

void ParticleEffects::setTotalParticles(int var)
{
    totalParticles = var;
}

bool ParticleEffects::isAutoRemoveOnFinish() const
{
    return isAutoRemovedOnFinish;
}

void ParticleEffects::setAutoRemoveOnFinish(bool var)
{
    isAutoRemovedOnFinish = var;
}

bool ParticleEffects::isPaused() const
{
    return isParticlePaused;
}

void ParticleEffects::pauseEmissions()
{
    isParticlePaused = true;
}

void ParticleEffects::resumeEmissions()
{
    isParticlePaused = false;
}

void ParticleEffects::setParticle(Particles type_)
{
    if (particleType == type_)
    {
        return;
    }
    particleType = type_;
    if (type_ == NONE)
    {
        stopSystem();
    }
    if (particleTex == nullptr)
    {
        setTexture(getDefaultTexture());
    }
    switch (type_)
    {
    case ParticleEffects::FIRE:
    {
        initWithTotalParticles(250);

        duration = DURATION_INFINITY;

        // Gravity mode
        this->emitterMode = Mode::GRAVITY;
        this->gravityMode.gravity = { 0, 0 };

        // Radial acceleration
        this->gravityMode.radialAccel = 0;
        this->gravityMode.radialAccelVar = 0;

        // Speed of particles
        this->gravityMode.speed = -60;
        this->gravityMode.speedVar = 20;

        // Starting angle
        angle = 90;
        angleVar = 10;

        // Life of particles
        life = 3;
        lifeVar = 0.25f;

        // Size, in pixels
        startSize = 54.0f;
        startSizeVar = 10.0f;
        endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // Emits per frame
        emissionRate = totalParticles / life;

        // Color of particles
        startColor.r = 0.76f;
        startColor.g = 0.25f;
        startColor.b = 0.12f;
        startColor.a = 1.0f;
        startColorVar.r = 0.0f;
        startColorVar.g = 0.0f;
        startColorVar.b = 0.0f;
        startColorVar.a = 0.0f;
        endColor.r = 0.0f;
        endColor.g = 0.0f;
        endColor.b = 0.0f;
        endColor.a = 0.0f;
        endColorVar.r = 0.0f;
        endColorVar.g = 0.0f;
        endColorVar.b = 0.0f;
        endColorVar.a = 0.0f;

        posVariance = { 40.0f, 20.0f };
        break;
    }
    case ParticleEffects::FIRE_WORK:
    {
        initWithTotalParticles(1500);

        duration = DURATION_INFINITY;

        // Gravity mode
        this->emitterMode = Mode::GRAVITY;
        this->gravityMode.gravity = { 0.0f, 90.0f };

        // Radial acceleration
        this->gravityMode.radialAccel = 0.0f;
        this->gravityMode.radialAccelVar = 0.0f;

        // Speed of particles
        this->gravityMode.speed = -180.0f;
        this->gravityMode.speedVar = 50.0f;

        // Angle
        this->angle = 90.0f;
        this->angleVar = 20.0f;

        // Life of particles
        this->life = 3.5f;
        this->lifeVar = 1.0f;

        // Emits per frame
        this->emissionRate = totalParticles / life;

        // Color of particles
        startColor.r = 0.5f;
        startColor.g = 0.5f;
        startColor.b = 0.5f;
        startColor.a = 1.0f;
        startColorVar.r = 0.5f;
        startColorVar.g = 0.5f;
        startColorVar.b = 0.5f;
        startColorVar.a = 0.1f;
        endColor.r = 0.1f;
        endColor.g = 0.1f;
        endColor.b = 0.1f;
        endColor.a = 0.2f;
        endColorVar.r = 0.1f;
        endColorVar.g = 0.1f;
        endColorVar.b = 0.1f;
        endColorVar.a = 0.2f;

        // Size, in pixels
        startSize = 8.0f;
        startSizeVar = 2.0f;
        endSize = START_SIZE_EQUAL_TO_END_SIZE;

        posVariance = { 0, 0 };
        break;
    }
    case ParticleEffects::SUN:
    {
        initWithTotalParticles(350);

        duration = DURATION_INFINITY;

        // Gravity mode
        setEmitterMode(Mode::GRAVITY);
        setGravity(Vector2(0, 0));

        // Radial acceleration
        setRadialAccel(0);
        setRadialAccelVar(0);

        // Speed of particles
        setSpeed(-20);
        setSpeedVar(5);

        // Angle
        angle = 90;
        angleVar = 360;

        // Life of particles
        life = 1;
        lifeVar = 0.5f;

        // Size, in pixels
        startSize = 30.0f;
        startSizeVar = 10.0f;
        endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // Emits per seconds
        emissionRate = totalParticles / life;

        // Color of particles
        startColor.r = 0.76f;
        startColor.g = 0.25f;
        startColor.b = 0.12f;
        startColor.a = 1.0f;
        startColorVar.r = 0.0f;
        startColorVar.g = 0.0f;
        startColorVar.b = 0.0f;
        startColorVar.a = 0.0f;
        endColor.r = 0.0f;
        endColor.g = 0.0f;
        endColor.b = 0.0f;
        endColor.a = 1.0f;
        endColorVar.r = 0.0f;
        endColorVar.g = 0.0f;
        endColorVar.b = 0.0f;
        endColorVar.a = 0.0f;

        posVariance = { 0, 0 };
        break;
    }
    case ParticleEffects::GALAXY:
    {
        initWithTotalParticles(200);

        duration = DURATION_INFINITY;

        // Gravity mode
        setEmitterMode(Mode::GRAVITY);
        setGravity(Vector2(0, 0));

        // Speed of particles
        setSpeed(-60);
        setSpeedVar(10);

        // Radial acceleration
        setRadialAccel(-80);
        setRadialAccelVar(0);

        // Tangential acceleration
        setTangentialAccel(80);
        setTangentialAccelVar(0);

        // Angle
        angle = 90;
        angleVar = 360;

        // Life of particles
        life = 4;
        lifeVar = 1;

        // Size, in pixels
        startSize = 37.0f;
        startSizeVar = 10.0f;
        endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        emissionRate = totalParticles / life;

        // Color of particles
        startColor.r = 0.12f;
        startColor.g = 0.25f;
        startColor.b = 0.76f;
        startColor.a = 1.0f;
        startColorVar.r = 0.0f;
        startColorVar.g = 0.0f;
        startColorVar.b = 0.0f;
        startColorVar.a = 0.0f;
        endColor.r = 0.0f;
        endColor.g = 0.0f;
        endColor.b = 0.0f;
        endColor.a = 1.0f;
        endColorVar.r = 0.0f;
        endColorVar.g = 0.0f;
        endColorVar.b = 0.0f;
        endColorVar.a = 0.0f;

        posVariance = { 0, 0 };
        break;
    }
    case ParticleEffects::FLOWER:
    {
        initWithTotalParticles(250);

        duration = DURATION_INFINITY;

        // Gravity mode
        setEmitterMode(Mode::GRAVITY);
        setGravity(Vector2(0, 0));

        // Speed of particles
        setSpeed(-80);
        setSpeedVar(10);

        // Radial acceleration
        setRadialAccel(-60);
        setRadialAccelVar(0);

        // Tangential acceleration
        setTangentialAccel(15);
        setTangentialAccelVar(0);

        // Angle
        angle = 90;
        angleVar = 360;

        // Life of particles
        life = 4;
        lifeVar = 1;

        // Size, in pixels
        startSize = 30.0f;
        startSizeVar = 10.0f;
        endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        emissionRate = totalParticles / life;

        // Color of particles
        startColor.r = 0.50f;
        startColor.g = 0.50f;
        startColor.b = 0.50f;
        startColor.a = 1.0f;
        startColorVar.r = 0.5f;
        startColorVar.g = 0.5f;
        startColorVar.b = 0.5f;
        startColorVar.a = 0.5f;
        endColor.r = 0.0f;
        endColor.g = 0.0f;
        endColor.b = 0.0f;
        endColor.a = 1.0f;
        endColorVar.r = 0.0f;
        endColorVar.g = 0.0f;
        endColorVar.b = 0.0f;
        endColorVar.a = 0.0f;

        posVariance = { 0, 0 };
        break;
    }
    case ParticleEffects::METEOR:
    {
        initWithTotalParticles(150);

        duration = DURATION_INFINITY;

        // Gravity mode
        setEmitterMode(Mode::GRAVITY);
        setGravity(Vector2(-200, -200));

        // Speed of particles
        setSpeed(-15);
        setSpeedVar(5);

        // Radial acceleration
        setRadialAccel(0);
        setRadialAccelVar(0);

        // Tangential acceleration
        setTangentialAccel(0);
        setTangentialAccelVar(0);

        // Angle
        angle = 90;
        angleVar = 360;

        // Life of particles
        life = 2;
        lifeVar = 1;

        // Size, in pixels
        startSize = 60.0f;
        startSizeVar = 10.0f;
        endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        emissionRate = totalParticles / life;

        // Color of particles
        startColor.r = 0.2f;
        startColor.g = 0.4f;
        startColor.b = 0.7f;
        startColor.a = 1.0f;
        startColorVar.r = 0.0f;
        startColorVar.g = 0.0f;
        startColorVar.b = 0.2f;
        startColorVar.a = 0.1f;
        endColor.r = 0.0f;
        endColor.g = 0.0f;
        endColor.b = 0.0f;
        endColor.a = 1.0f;
        endColorVar.r = 0.0f;
        endColorVar.g = 0.0f;
        endColorVar.b = 0.0f;
        endColorVar.a = 0.0f;

        posVariance = { 0, 0 };
        break;
    }
    case ParticleEffects::SPIRAL:
    {
        initWithTotalParticles(500);

        duration = DURATION_INFINITY;

        // Gravity mode
        setEmitterMode(Mode::GRAVITY);
        setGravity(Vector2(0, 0));

        // Speed of particles
        setSpeed(-150);
        setSpeedVar(0);

        // Radial acceleration
        setRadialAccel(-380);
        setRadialAccelVar(0);

        // Tangential acceleration
        setTangentialAccel(45);
        setTangentialAccelVar(0);

        // Angle
        angle = 90;
        angleVar = 0;

        // Life of particles
        life = 12;
        lifeVar = 0;

        // Size, in pixels
        startSize = 20.0f;
        startSizeVar = 0.0f;
        endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        emissionRate = totalParticles / life;

        // Color of particles
        startColor.r = 0.5f;
        startColor.g = 0.5f;
        startColor.b = 0.5f;
        startColor.a = 1.0f;
        startColorVar.r = 0.5f;
        startColorVar.g = 0.5f;
        startColorVar.b = 0.5f;
        startColorVar.a = 0.0f;
        endColor.r = 0.5f;
        endColor.g = 0.5f;
        endColor.b = 0.5f;
        endColor.a = 1.0f;
        endColorVar.r = 0.5f;
        endColorVar.g = 0.5f;
        endColorVar.b = 0.5f;
        endColorVar.a = 0.0f;

        posVariance = { 0, 0 };
        break;
    }
    case ParticleEffects::EXPLOSION:
    {
        initWithTotalParticles(700);

        duration = 0.1f;

        setEmitterMode(Mode::GRAVITY);
        setGravity(Vector2(0, 0));

        // Speed of particles
        setSpeed(-70);
        setSpeedVar(40);

        // Radial acceleration
        setRadialAccel(0);
        setRadialAccelVar(0);

        // Tangential acceleration
        setTangentialAccel(0);
        setTangentialAccelVar(0);

        // Angle
        angle = 90;
        angleVar = 360;

        // Life of particles
        life = 5.0f;
        lifeVar = 2;

        // Size, in pixels
        startSize = 15.0f;
        startSizeVar = 10.0f;
        endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        emissionRate = totalParticles / duration;

        // Color of particles
        startColor.r = 0.7f;
        startColor.g = 0.1f;
        startColor.b = 0.2f;
        startColor.a = 1.0f;
        startColorVar.r = 0.5f;
        startColorVar.g = 0.5f;
        startColorVar.b = 0.5f;
        startColorVar.a = 0.0f;
        endColor.r = 0.5f;
        endColor.g = 0.5f;
        endColor.b = 0.5f;
        endColor.a = 0.0f;
        endColorVar.r = 0.5f;
        endColorVar.g = 0.5f;
        endColorVar.b = 0.5f;
        endColorVar.a = 0.0f;

        posVariance = { 0, 0 };
        break;
    }
    case ParticleEffects::SMOKE:
    {
        initWithTotalParticles(200);

        // duration
        duration = DURATION_INFINITY;

        // Emitter mode: Gravity Mode
        setEmitterMode(Mode::GRAVITY);
        setGravity(Vector2(0, 0));

        // Radial acceleration acceleration
        setRadialAccel(0);
        setRadialAccelVar(0);

        // Speed of particles
        setSpeed(-25);
        setSpeedVar(10);

        // Angle
        angle = 90;
        angleVar = 5;

        // Life of particles
        life = 4;
        lifeVar = 1;

        // Size, in pixels
        startSize = 60.0f;
        startSizeVar = 10.0f;
        endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // Emits per frame
        emissionRate = totalParticles / life;

        // Color of particles
        startColor.r = 0.8f;
        startColor.g = 0.8f;
        startColor.b = 0.8f;
        startColor.a = 1.0f;
        startColorVar.r = 0.02f;
        startColorVar.g = 0.02f;
        startColorVar.b = 0.02f;
        startColorVar.a = 0.0f;
        endColor.r = 0.0f;
        endColor.g = 0.0f;
        endColor.b = 0.0f;
        endColor.a = 1.0f;
        endColorVar.r = 0.0f;
        endColorVar.g = 0.0f;
        endColorVar.b = 0.0f;
        endColorVar.a = 0.0f;

        posVariance = { 20.0f, 0.0f };
        break;
    }
    case ParticleEffects::SNOW:
    {
        initWithTotalParticles(700);

        duration = DURATION_INFINITY;

        // Gravity mode
        setEmitterMode(Mode::GRAVITY);
        setGravity(Vector2(0, 1));

        // Speed of particles
        setSpeed(-5);
        setSpeedVar(1);

        // Radial acceleration
        setRadialAccel(0);
        setRadialAccelVar(1);

        // Tangential acceleration
        setTangentialAccel(0);
        setTangentialAccelVar(1);

        // Angle
        angle = -90;
        angleVar = 5;

        // Life of particles
        life = 45;
        lifeVar = 15;

        // Size, in pixels
        startSize = 10.0f;
        startSizeVar = 5.0f;
        endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        emissionRate = 10;

        // Color of particles
        startColor.r = 1.0f;
        startColor.g = 1.0f;
        startColor.b = 1.0f;
        startColor.a = 1.0f;
        startColorVar.r = 0.0f;
        startColorVar.g = 0.0f;
        startColorVar.b = 0.0f;
        startColorVar.a = 0.0f;
        endColor.r = 1.0f;
        endColor.g = 1.0f;
        endColor.b = 1.0f;
        endColor.a = 0.0f;
        endColorVar.r = 0.0f;
        endColorVar.g = 0.0f;
        endColorVar.b = 0.0f;
        endColorVar.a = 0.0f;

        posVariance = { 1.0f * x, 0.0f };
        break;
    }
    default:
        break;
    }
}