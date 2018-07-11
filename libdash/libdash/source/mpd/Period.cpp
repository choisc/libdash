/*
 * Period.cpp
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "Period.h"

using namespace dash::mpd;

Period::Period  () :
        segmentBase(NULL),
        segmentList(NULL),
        segmentTemplate(NULL),
        xlinkActuate("onRequest"),
        xlinkHref(""),
        id(""),
        start(""),
        duration(""),
        isBitstreamSwitching(false)
{
}
Period::~Period ()
{
    for(size_t i = 0; i < this->baseURLs.size(); i++)
        delete(this->baseURLs.at(i));
    for(size_t i = 0; i < this->adaptationSets.size(); i++)
        delete(this->adaptationSets.at(i));
    for(size_t i = 0; i < this->subsets.size(); i++)
        delete(this->subsets.at(i));
    delete(segmentBase);
    delete(segmentList);
    delete(segmentTemplate);
}

const std::vector<IBaseUrl *>&      Period::GetBaseURLs             ()  const
{
    return (std::vector<IBaseUrl *> &) this->baseURLs;
}
void                                Period::AddBaseURL              (BaseUrl *baseUrl)
{
    this->baseURLs.push_back(baseUrl);
}
ISegmentBase*                       Period::GetSegmentBase          ()  const
{
    return this->segmentBase;
}
void                                Period::SetSegmentBase          (SegmentBase *asegmentBase)
{
    this->segmentBase = asegmentBase;
}
ISegmentList*                       Period::GetSegmentList          ()  const
{
    return this->segmentList;
}
void                                Period::SetSegmentList          (SegmentList *asegmentList)
{
    this->segmentList = asegmentList;
}
ISegmentTemplate*                   Period::GetSegmentTemplate      ()  const
{
    return this->segmentTemplate;
}
void                                Period::SetSegmentTemplate      (SegmentTemplate *asegmentTemplate) 
{
    this->segmentTemplate = asegmentTemplate;
}
const std::vector<IAdaptationSet*>& Period::GetAdaptationSets       () const
{
    return (std::vector<IAdaptationSet*> &) this->adaptationSets;
}
void                                Period::AddAdaptationSet        (AdaptationSet *aadaptationSet)
{
    if(aadaptationSet != NULL)
        this->adaptationSets.push_back(aadaptationSet);
}
const std::vector<ISubset *>&       Period::GetSubsets              () const
{
    return (std::vector<ISubset *> &) this->subsets;
}
void                                Period::AddSubset               (Subset *asubset)
{
    this->subsets.push_back(asubset);
}
const std::string&                  Period::GetXlinkHref            ()  const
{
    return this->xlinkHref;
}
void                                Period::SetXlinkHref            (const std::string& axlinkHref)
{
    this->xlinkHref = axlinkHref;
}
const std::string&                  Period::GetXlinkActuate         ()  const
{
    return this->xlinkActuate;
}
void                                Period::SetXlinkActuate         (const std::string& axlinkActuate)
{
    this->xlinkActuate = axlinkActuate;
}
const std::string&                  Period::GetId                   ()  const
{
    return this->id;
}
void                                Period::SetId                   (const std::string& aid)
{
    this->id = aid;
}
const std::string&                  Period::GetStart                ()  const
{
    return this->start;
}
void                                Period::SetStart                (const std::string& astart)
{
    this->start = astart;
}
const std::string&                  Period::GetDuration             ()  const
{
    return this->duration;
}
void                                Period::SetDuration             (const std::string& aduration)
{
    this->duration = aduration;
}
bool                                Period::GetBitstreamSwitching   ()  const
{
    return this->isBitstreamSwitching;
}
void                                Period::SetBitstreamSwitching   (bool value)
{
    this->isBitstreamSwitching = value;
}
