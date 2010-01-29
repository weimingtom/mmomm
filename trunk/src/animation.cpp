#include "animation.h"

Animation::Animation(ImageManager::shared_ptr img, int frameWidth, int frameHeight,
                     int interval, int startFrame, bool active)
:   _img(img)
,   _frameWidth(frameWidth)
,   _frameHeight(frameHeight)
,   _interval(interval)
,   _currentFrame(startFrame)
,   _active(active)
{
    _totalHoriFrames = _img.get()->getWidth()  / _frameWidth;
    _totalVertFrames = _img.get()->getHeight() / _frameHeight;
}

Animation::~Animation()
{
}


void Animation::update(int msPassed)
{
    int maxTime = getHoriFrameNo()*getVertFrameNo()*_interval;
    _totalTime += msPassed;

    if(_totalTime > maxTime)
        _totalTime -= maxTime;

    _currentFrame = _totalTime/_interval;
}
