/*
 * Timeline.cpp
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "Timeline.h"

using namespace dash::mpd;

Timeline::Timeline    ()  :
                startTime(0),
                duration(0),
                repeatCount(0)
{
}
Timeline::~Timeline   ()
{
}

uint64_t    Timeline::GetStartTime     ()  const
{
    return this->startTime;
}
void        Timeline::SetStartTime     (uint64_t astartTime)
{
    this->startTime = astartTime;
}
uint64_t    Timeline::GetDuration      ()  const
{
    return this->duration;
}
void        Timeline::SetDuration      (uint64_t aduration)
{
    this->duration = aduration;
}
uint64_t    Timeline::GetRepeatCount   ()  const
{
    return this->repeatCount;
}
void        Timeline::SetRepeatCount   (uint64_t arepeatCount)
{
    this->repeatCount = arepeatCount;
}
