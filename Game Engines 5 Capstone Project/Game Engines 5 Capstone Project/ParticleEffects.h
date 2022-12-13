#pragma once

#include <SDL.h>
#include <vector>
#include <string>
#include <SDL_image.h>

struct Point
{
public:
    Point() {}
    Point(float _x, float _y)
        : x(_x)
        , y(_y)
    {
    }
    ~Point() {}
    float x = 0, y = 0;
    Point operator*(float f)
    {
        Point p{ x * f, y * f };
        return p;
    }
    float getAngle()
    {
        return atan2f(y, x);
    }
};

typedef Point Vector2;

struct ParticleColor
{
    float r = 0, g = 0, b = 0, a = 0;
};

class ParticleEffects
{
public:
    float posx = 0;
    float posy = 0;
    float startPosX = 0;
    float startPosY = 0;

    float colorR = 0;
    float colorG = 0;
    float colorB = 0;
    float colorA = 0;

    float deltaColorR = 0;
    float deltaColorG = 0;
    float deltaColorB = 0;
    float deltaColorA = 0;

    float size = 0;
    float deltaSize = 0;
    float rotation = 0;
    float deltaRotation = 0;
    float timeToLive = 0;
    unsigned int atlasIndex = 0;

    struct
    {
        float dirX = 0;
        float dirY = 0;
        float radialAccel = 0;
        float tangentialAccel = 0;

        Vector2 gravity;
        float speed = 0;
        float speedVar = 0;
        float tangentialAccelVar = 0;
        float radialAccelVar = 0;
        bool rotationIsDir = 0;
    } gravityMode;

    struct
    {
        float angle = 0;
        float degreesPerSecond = 0;
        float radius = 0;
        float deltaRadius = 0;

        float startRadius = 0;
        float startRadiusVar = 0;
        float endRadius = 0;
        float endRadiusVar = 0;
        float rotatePerSecond = 0;
        float rotatePerSecondVar = 0;
    } radiusMode;

public:
    enum class Mode
    {
        GRAVITY,
        RADIUS,
    };

    enum
    {
        DURATION_INFINITY = -1,
        START_SIZE_EQUAL_TO_END_SIZE = -1,
        START_RADIUS_EQUAL_TO_END_RADIUS = -1,
    };

public:
    void addParticles(int count);

    void stopSystem();
    void resetSystem();

    bool isFull();

    virtual bool isAutoRemoveOnFinish() const;
    virtual void setAutoRemoveOnFinish(bool var);

    virtual const Vector2& getGravity();
    virtual void setGravity(const Vector2& g);

    virtual float getSpeed() const;
    virtual void setSpeed(float speed);

    virtual float getSpeedVar() const;
    virtual void setSpeedVar(float speed);

    virtual float getTangentialAccel() const;
    virtual void setTangentialAccel(float t);

    virtual float getTangentialAccelVar() const;
    virtual void setTangentialAccelVar(float t);

    virtual float getRadialAccel() const;
    virtual void setRadialAccel(float t);

    virtual float getRadialAccelVar() const;
    virtual void setRadialAccelVar(float t);

    virtual bool getRotationIsDir() const;
    virtual void setRotationIsDir(bool t);

    virtual float getStartRadius() const;
    virtual void setStartRadius(float startRadius);

    virtual float getStartRadiusVar() const;
    virtual void setStartRadiusVar(float startRadiusVar);

    virtual float getEndRadius() const;
    virtual void setEndRadius(float endRadius);

    virtual float getEndRadiusVar() const;
    virtual void setEndRadiusVar(float endRadiusVar);

    virtual float getRotatePerSecond() const;
    virtual void setRotatePerSecond(float degrees);

    virtual float getRotatePerSecondVar() const;
    virtual void setRotatePerSecondVar(float degrees);

    virtual bool isActive() const;

    int getAtlasIndex() const { return atlasIndexSignedInt; }
    void setAtlasIndex(int index) { atlasIndexSignedInt = index; }

    unsigned int getParticleCount() const { return particleCount; }

    float getDuration() const { return duration; }
    void setDuration(float duration) { duration = duration; }

    const Vector2& getSourcePosition() const { return sourcePos; }
    void setSourcePosition(const Vector2& pos) { sourcePos = pos; }

    const Vector2& getPosVar() const { return posVariance; }
    void setPosVar(const Vector2& pos) { posVariance = pos; }

    float getLife() const { return life; }
    void setLife(float life) { life = life; }

    float getLifeVar() const { return lifeVar; }
    void setLifeVar(float lifeVar) { lifeVar = lifeVar; }

    float getAngle() const { return angle; }
    void setAngle(float angle) { angle = angle; }

    float getAngleVar() const { return angleVar; }
    void setAngleVar(float angleVar) { angleVar = angleVar; }
    
    Mode getEmitterMode() const { return emitterMode; }
    void setEmitterMode(Mode mode) { emitterMode = mode; }

    float getStartSize() const { return startSize; }
    void setStartSize(float startSize) { startSize = startSize; }

    float getStartSizeVar() const { return startSizeVar; }
    void setStartSizeVar(float sizeVar) { startSizeVar = sizeVar; }

    float getEndSize() const { return endSize; }
    void setEndSize(float endSize) { endSize = endSize; }

    float getEndSizeVar() const { return endSizeVar; }
    void setEndSizeVar(float sizeVar) { endSizeVar = sizeVar; }

    const ParticleColor& getStartColor() const { return startColor; }
    void setStartColor(const ParticleColor& color) { startColor = color; }

    const ParticleColor& getStartColorVar() const { return startColorVar; }
    void setStartColorVar(const ParticleColor& color) { startColorVar = color; }

    const ParticleColor& getEndColor() const { return endColor; }
    void setEndColor(const ParticleColor& color) { endColor = color; }

    const ParticleColor& getEndColorVar() const { return endColorVar; }
    void setEndColorVar(const ParticleColor& color) { endColorVar = color; }

    float getStartSpin() const { return startSpin; }
    void setStartSpin(float spin) { startSpin = spin; }

    float getStartSpinVar() const { return startSpinVar; }
    void setStartSpinVar(float pinVar) { startSpinVar = pinVar; }

    float getEndSpin() const { return endSpin; }
    void setEndSpin(float endSpin) { endSpin = endSpin; }

    float getEndSpinVar() const { return endSpinVar; }
    void setEndSpinVar(float endSpinVar) { endSpinVar = endSpinVar; }

    float getEmissionRate() const { return emissionRate; }
    void setEmissionRate(float rate) { emissionRate = rate; }

    virtual int getTotalParticles() const;
    virtual void setTotalParticles(int totalParticles);

    void setOpacityModifyRGB(bool opacityModifyRGB) { opacityModiftRGB = opacityModifyRGB; }
    bool isOpacityModifyRGB() const { return opacityModiftRGB; }

    SDL_Texture* getTexture();
    void setTexture(SDL_Texture* texture);
    void draw();
    void update();

    void ParticleSystem();
    virtual void DestroyParticleSystem();

    virtual bool initWithTotalParticles(int numberOfParticles);
    virtual void resetTotalParticles(int numberOfParticles);
    virtual bool isPaused() const;
    virtual void pauseEmissions();
    virtual void resumeEmissions();

protected:
    bool isAutoRemovedOnFinish = false;

    std::string particleListFile;

    float elapsedTIme = 0;

    std::vector<ParticleEffects> particleInfo;

    std::string emitterName;

    float emitterCounter = 0;

    int atlasIndexSignedInt = 0;

    bool isTransformSystemDirty = false;

    int allocatedParticles = 0;
    bool isParticleActive = true;
    int particleCount = 0;
    float duration = 0;

    Vector2 sourcePos;
    Vector2 posVariance;
    float life = 0;
    float lifeVar = 0;
    float angle = 0;
    float angleVar = 0;

    Mode emitterMode = Mode::GRAVITY;

    float startSize = 0;
    float startSizeVar = 0;
    float endSize = 0;
    float endSizeVar = 0;

    ParticleColor startColor;
    ParticleColor startColorVar;
    ParticleColor endColor;
    ParticleColor endColorVar;

    float startSpin = 0;
    float startSpinVar = 0;
    float endSpin = 0;
    float endSpinVar = 0;
    float emissionRate = 0;
    int totalParticles = 0;

    SDL_Texture* particleTex = nullptr;

    bool opacityModiftRGB = false;

    int flippedY = 1;

    bool isParticlePaused = false;

    bool sourcePosCompatible = false;

    SDL_Renderer* renderer = nullptr;
    int x = 0, y = 0;
public:
    void setRenderer(SDL_Renderer* ren) { renderer = ren; }
    void setPosition(int x_, int y_) { x = x_; y = y_; }

    void ParticleExample() {}
    virtual void DestroyParticleExample() {}

    enum Particles
    {
        NONE,
        FIRE,
        FIRE_WORK,
        SUN,
        GALAXY,
        FLOWER,
        METEOR,
        SPIRAL,
        EXPLOSION,
        SMOKE,
        SNOW,
    };

    Particles particleType = NONE;
    void setParticle(Particles type_);
    SDL_Texture* getDefaultTexture()
    {
        static SDL_Texture* tex = IMG_LoadTexture(renderer, "Default.png");
        //printf(SDL_GetError());
        return tex;
    }
};

