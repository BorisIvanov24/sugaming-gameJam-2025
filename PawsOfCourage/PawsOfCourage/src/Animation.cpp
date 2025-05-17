#include "Animation.h"


Animation::Animation(int numFramesPerRow, int tilePixelSize,
                     float speed, AnimationType type)
{
    texture = nullptr;
    step = 1;
    first = 0;
    cur = 0;
    last = numFramesPerRow - 1;

    this->numFramesPerRow = numFramesPerRow;
    this->tilePixelSize = tilePixelSize;
    this->speed = speed;
    this->type = type;
    durationLeft = speed;
}

Rectangle Animation::animationFrame() const
{
	int x = (cur % numFramesPerRow) * tilePixelSize;
	int y = (cur / numFramesPerRow) * tilePixelSize;

	return {(float)x, (float)y, (float)tilePixelSize, (float)tilePixelSize};
}

void Animation::animationUpdate()
{
    float dt = GetFrameTime();
    durationLeft -= dt;

    if (durationLeft <= 0.0) 
    {
        durationLeft = speed;
        cur += step;

        if (cur > last) 
        {
            switch (type) 
            {
            case REPEATING:
                cur = first;
                break;
            case ONESHOT:
                cur = last;
                break;
            }
        }
        else if (cur < first) 
        {
            switch (type) 
            {
            case REPEATING:
                cur = last;
                break;
            case ONESHOT:
                cur = first;
                break;
            }
        }
    }
}

void Animation::setTexture(const Texture2D& texture)
{
    this->texture = &texture;
}

const Texture2D& Animation::getTexture() const
{
    return *texture;
}
