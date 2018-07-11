/*
 * MultipleSegmentBase.cpp
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "MultipleSegmentBase.h"

using namespace dash::mpd;

MultipleSegmentBase::MultipleSegmentBase    () :
                        bitstreamSwitching(NULL),
                        segmentTimeline(NULL),
                        duration(0),
                        startNumber(1)
{
}
MultipleSegmentBase::~MultipleSegmentBase   ()
{
    delete(this->segmentTimeline);
    delete(this->bitstreamSwitching);
}

const ISegmentTimeline *    MultipleSegmentBase::GetSegmentTimeline     ()  const
{
    return (ISegmentTimeline *) this->segmentTimeline;
}
void                        MultipleSegmentBase::SetSegmentTimeline     (SegmentTimeline *asegmentTimeline)
{
    this->segmentTimeline = asegmentTimeline;
}
const IURLType*             MultipleSegmentBase::GetBitstreamSwitching  ()  const
{
    return this->bitstreamSwitching;
}
void                        MultipleSegmentBase::SetBitstreamSwitching  (URLType *abitstreamSwitching)
{
    this->bitstreamSwitching = abitstreamSwitching;
}
uint64_t                    MultipleSegmentBase::GetDuration            ()  const
{
    return this->duration;
}
void                        MultipleSegmentBase::SetDuration            (uint64_t aduration)
{
    this->duration = aduration;
}
uint64_t                    MultipleSegmentBase::GetStartNumber         ()  const
{
    return this->startNumber;
}
void                        MultipleSegmentBase::SetStartNumber         (uint64_t astartNumber)
{
    this->startNumber = astartNumber;
}
