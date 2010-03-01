#ifndef ANIMATION_H_
#define ANIMATION_H_

class Animation;
class Sprite;

#include "image.h"
#include "imageManager.h"

class Animation
{
private:
    int                         _id;
    ImageManager::shared_ptr    _img;
    int                         _frameWidth;
    int                         _frameHeight;
    double                      _interval;      //time between switching frames.
    double                      _totalTime;
    int                         _totalHoriFrames;
    int                         _totalVertFrames;
    bool                        _active;
    int                         _startFrame;
    int                         _currentFrame;
    int                         _endFrame;

    Sprite*                     _callbackSprite;

    void                        setActive(bool active)              { _active = active; _currentFrame = _startFrame; }
    friend class AnimationManager;

public:
    Animation(int id, ImageManager::shared_ptr img, int frameWidth, int frameHeight,
              double interval, int startFrame = 0, int endFrame = -1, bool active = true);
    ~Animation();

    void update(double elapsed);

    void                        setCallbackSprite(Sprite* sprite);

    int                         getId()               const { return _id; }
    ImageManager::shared_ptr    getImage()            const { return _img; }
    int                         getFrameWidth()       const { return _frameWidth; }
    int                         getFrameHeight()      const { return _frameHeight; }
    int                         getHoriFrameNo()      const { return _totalHoriFrames; }
    int                         getVertFrameNo()      const { return _totalVertFrames; }
    int                         getCurrentFrame()     const { return _currentFrame; }
	int                         getCurrentFrameX()    const { return _currentFrame % getHoriFrameNo() *_frameWidth; }
    int                         getCurrentFrameY()    const { return (int(_currentFrame/getHoriFrameNo())) *_frameHeight; }
	SDL_Rect                    getCurrentFrameRect() const;
    int                         getStartFrame()       const { return _startFrame; }
    int                         getEndFrame()         const { return _endFrame; }
    double                      getInterval()         const { return _interval; }

    bool                        isActive()            const { return _active; }

    void                        setCurrentFrame(int frame) { _currentFrame = frame; }
};

#endif
