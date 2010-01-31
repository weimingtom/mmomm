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
    double                      _interval;      //time between switching frames.
    int                         _currentFrame;
    double                      _totalTime;
    int                         _totalHoriFrames;
    int                         _totalVertFrames;
    bool                        _active;

public:
    Animation(ImageManager::shared_ptr img, int frameWidth, int frameHeight,
              double interval, int startFrame = 0, bool active = true);
    ~Animation();

    void update(double elapsed);

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
    double                      getInterval()       { return _interval; }

    bool                        isActive()          { return _active; }
};

#endif
