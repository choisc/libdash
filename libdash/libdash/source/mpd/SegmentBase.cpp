/*
 * SegmentBase.cpp
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "SegmentBase.h"

using namespace dash::mpd;

SegmentBase::SegmentBase    ()  :
                initialization(NULL),
                representationIndex(NULL),
                timescale(1),
                presentationTimeOffset(0),
                indexRange(""),
                indexRangeExact(false)
{
}
SegmentBase::~SegmentBase   ()
{
    delete(this->initialization);
    delete(this->representationIndex);
}

const IURLType*     SegmentBase::GetInitialization          ()  const
{
    return this->initialization;
}
void                SegmentBase::SetInitialization          (URLType *ainitialization)
{
    this->initialization = ainitialization;
}
const IURLType*     SegmentBase::GetRepresentationIndex     ()  const
{
    return this->representationIndex;
}
void                SegmentBase::SetRepresentationIndex     (URLType *arepresentationIndex)
{
    this->representationIndex = arepresentationIndex;
}
uint64_t            SegmentBase::GetTimescale               ()  const
{
    return this->timescale;
}
void                SegmentBase::SetTimescale               (uint64_t atimescale)
{
    this->timescale = atimescale;
}
uint64_t            SegmentBase::GetPresentationTimeOffset  ()  const
{
    return this->presentationTimeOffset;
}
void                SegmentBase::SetPresentationTimeOffset  (uint64_t apresentationTimeOffset)
{
    this->presentationTimeOffset = apresentationTimeOffset;
}
const std::string&  SegmentBase::GetIndexRange              ()  const
{
    return this->indexRange;
}
void                SegmentBase::SetIndexRange              (const std::string& aindexRange)
{
    this->indexRange = aindexRange;
}
bool                SegmentBase::HasIndexRangeExact         ()  const
{
    return this->indexRangeExact;
}
void                SegmentBase::SetIndexRangeExact         (bool aindexRangeExact)
{
    this->indexRangeExact = aindexRangeExact;
}
