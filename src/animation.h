#ifndef ANIMATION_H_
#define ANIMATION_H_

class Animation;

#include "image.h"
#include "imageManager.h"

class Animation
{
private:
    ImageManager::shared_ptr    _img;
    int                         _frameWidth;
    int                         _frameHeight;
    int                         _interval;      //time between switching frames.
    int                         _currentFrame;
    int                         _totalTime;
    int                         _totalHoriFrames;
    int                         _totalVertFrames;
    bool                        _active;

public:
    Animation(ImageManager::shared_ptr img, int frameWidth, int frameHeight,
              int interval, int startFrame = 0, bool active = true);
    ~Animation();

    void update(unsigned msPassed);

    void                        setCurrentFrame(int currentFrame)   { _currentFrame = currentFrame; }
    void                        setActive(bool active)              { _active = active; }

    ImageManager::shared_ptr    getImage()          { return _img; }
    int                         getFrameWidth()     { return _frameWidth; }
    int                         getFrameHeight()    { return _frameHeight; }
    int                         getHoriFrameNo()    { return _totalHoriFrames; }
    int                         getVertFrameNo()    { return _totalVertFrames; }
    int                         getCurrentFrame()   { return _currentFrame; }
	int                         getCurrentFrameX()  { return _currentFrame % getHoriFrameNo() *_frameWidth; }
    int                         getCurrentFrameY()  { return (int(_currentFrame/getHoriFrameNo())) *_frameHeight; }

    bool                        isActive()          { return _active; }
};

#endif
