#include "animation.h"
#include "sprite.h"

Animation::Animation(ImageManager::shared_ptr img, int frameWidth, int frameHeight,
                     double interval, int startFrame, int endFrame, bool active)
:   _img(img)
,   _frameWidth(frameWidth)
,   _frameHeight(frameHeight)
,   _interval(interval)
,   _active(active)
,   _startFrame(startFrame)
,   _currentFrame(_startFrame)
,   _callbackSprite(0)
{
	assert(interval > 0);
	_totalTime = 0;
    _totalHoriFrames = _img.get()->getWidth()  / _frameWidth;
    _totalVertFrames = _img.get()->getHeight() / _frameHeight;
    _endFrame = endFrame >= 0 ? endFrame : _totalHoriFrames * _totalVertFrames;

    assert(_startFrame >= 0 && _startFrame < _totalHoriFrames * _totalVertFrames);
    assert(_endFrame > _startFrame && _endFrame <= _totalHoriFrames * _totalVertFrames);

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

void Animation::setCallbackSprite(Sprite* sprite)
{
    _callbackSprite = sprite;
}

void Animation::update(double elapsed)
{
    //cout << "updating \"" << getImage().get()->getFilename() << "\" with " << msPassed << endl;

    double maxTime = (_endFrame - _startFrame)*_interval;
    _totalTime += elapsed;

	while (_totalTime >= maxTime)
		_totalTime -= maxTime;

    int oldFrame = _currentFrame;
    _currentFrame = _startFrame + int(_totalTime/_interval);
    if ( _currentFrame < oldFrame && _callbackSprite )
        _callbackSprite->RepetitionCallback();
}
