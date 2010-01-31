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
	_totalTime = 0;
    _totalHoriFrames = _img.get()->getWidth()  / _frameWidth;
    _totalVertFrames = _img.get()->getHeight() / _frameHeight;

#ifndef NDEBUG
    cout << "Created animation with " << _totalHoriFrames << "x" << _totalVertFrames << " frames and interval = " << _interval << endl;
#endif
}

Animation::~Animation()
{
#ifndef NDEBUG
    cout << "Animation of " << _img.get()->getFilename() << " destroyed" << endl;
#endif
}


void Animation::update(unsigned msPassed)
{
    //cout << "updating \"" << getImage().get()->getFilename() << "\" with " << msPassed << endl;

    int maxTime = getHoriFrameNo()*getVertFrameNo()*_interval;
    _totalTime += msPassed;

    if(_totalTime > maxTime)
        _totalTime -= maxTime;

    _currentFrame = _totalTime/_interval;
}
