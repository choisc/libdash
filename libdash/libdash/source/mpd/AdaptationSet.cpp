/*
 * AdaptationSet.cpp
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "AdaptationSet.h"
#include <cstdlib>

using namespace dash::mpd;

AdaptationSet::AdaptationSet    () :
                segmentBase(NULL),
                segmentList(NULL),
                segmentTemplate(NULL),
                xlinkHref(""),
                xlinkActuate("onRequest"),
                id(0),
                lang(""),
                contentType(""),
                par(""),
                minBandwidth(0),
                maxBandwidth(0),
                minWidth(0),
                maxWidth(0),
                minHeight(0),
                maxHeight(0),
                minFramerate(""),
                maxFramerate(""),
                segmentAlignmentIsBool(true),
                subsegmentAlignmentIsBool(true),
                usesSegmentAlignment(false),
                usesSubsegmentAlignment(false),
                segmentAlignment(0),
                subsegmentAlignment(0),
                isBitstreamSwitching(false)
{
}
AdaptationSet::~AdaptationSet   ()
{
    for(size_t i = 0; i < this->accessibility.size(); i++)
        delete(this->accessibility.at(i));
    for(size_t i = 0; i < this->role.size(); i++)
        delete(this->role.at(i));
    for(size_t i = 0; i < this->rating.size(); i++)
        delete(this->rating.at(i));
    for(size_t i = 0; i < this->viewpoint.size(); i++)
        delete(this->viewpoint.at(i));
    for(size_t i = 0; i < this->contentComponent.size(); i++)
        delete(this->contentComponent.at(i));
    for(size_t i = 0; i < this->baseURLs.size(); i++)
        delete(this->baseURLs.at(i));
    for(size_t i = 0; i < this->representation.size(); i++)
        delete(this->representation.at(i));

    delete(segmentBase);
    delete(segmentList);
    delete(segmentTemplate);
}

const std::vector<IDescriptor *>&       AdaptationSet::GetAccessibility                 ()  const
{
    return (std::vector<IDescriptor *> &) this->accessibility;
}
void                                    AdaptationSet::AddAccessibity                   (Descriptor *aAccessibility)
{
    this->accessibility.push_back(aAccessibility);
}
const std::vector<IDescriptor *>&       AdaptationSet::GetRole                          ()  const
{
    return (std::vector<IDescriptor *> &) this->role;
}
void                                    AdaptationSet::AddRole                          (Descriptor *aRole)
{
    this->role.push_back(aRole);
}
const std::vector<IDescriptor *>&       AdaptationSet::GetRating                        ()  const
{
    return (std::vector<IDescriptor *> &) this->rating;
}
void                                    AdaptationSet::AddRating                        (Descriptor *aRating)
{
    this->rating.push_back(aRating);
}
const std::vector<IDescriptor *>&       AdaptationSet::GetViewpoint                     ()  const
{
    return (std::vector<IDescriptor *> &) this->viewpoint;
}
void                                    AdaptationSet::AddViewpoint                     (Descriptor *aViewpoint)
{
    this->viewpoint.push_back(aViewpoint);
}
const std::vector<IContentComponent *>& AdaptationSet::GetContentComponent              ()  const
{
    return (std::vector<IContentComponent *> &) this->contentComponent;
}
void                                    AdaptationSet::AddContentComponent              (ContentComponent *aContentComponent)
{
    this->contentComponent.push_back(aContentComponent);
}
const std::vector<IBaseUrl *>&          AdaptationSet::GetBaseURLs                      ()  const
{
    return (std::vector<IBaseUrl *> &) this->baseURLs;
}
void                                    AdaptationSet::AddBaseURL                       (BaseUrl *aBaseUrl)
{
    this->baseURLs.push_back(aBaseUrl);
}
ISegmentBase*                           AdaptationSet::GetSegmentBase                   ()  const
{
    return this->segmentBase;
}
void                                    AdaptationSet::SetSegmentBase                   (SegmentBase *aSegmentBase)
{
    this->segmentBase = aSegmentBase;
}
ISegmentList*                           AdaptationSet::GetSegmentList                   ()  const
{
    return this->segmentList;
}
void                                    AdaptationSet::SetSegmentList                   (SegmentList *aSegmentList)
{
    this->segmentList = aSegmentList;
}
ISegmentTemplate*                       AdaptationSet::GetSegmentTemplate               ()  const
{
    return this->segmentTemplate;
}
void                                    AdaptationSet::SetSegmentTemplate               (SegmentTemplate *aSegmentTemplate) 
{
    this->segmentTemplate = aSegmentTemplate;
}
const std::vector<IRepresentation *>&   AdaptationSet::GetRepresentation                ()  const
{
    return (std::vector<IRepresentation *> &) this->representation;
}
void                                    AdaptationSet::AddRepresentation                (Representation *aRepresentation)
{
    this->representation.push_back(aRepresentation);
}
const std::string&                      AdaptationSet::GetXlinkHref                     ()  const
{
    return this->xlinkHref;
}
void                                    AdaptationSet::SetXlinkHref                     (const std::string& aXlinkHref)
{
    this->xlinkHref = aXlinkHref;
}
const std::string&                      AdaptationSet::GetXlinkActuate                  ()  const
{
    return this->xlinkActuate;
}
void                                    AdaptationSet::SetXlinkActuate                  (const std::string& aXlinkActuate)
{
    this->xlinkActuate = aXlinkActuate;
}
uint32_t                                AdaptationSet::GetId                            ()  const
{
    return this->id;
}
void                                    AdaptationSet::SetId                            (uint32_t aId)
{
    this->id = aId;
}
uint32_t                                AdaptationSet::GetGroup                         ()  const
{
    return this->group;
}
void                                    AdaptationSet::SetGroup                         (uint32_t aGroup)
{
    this->group = aGroup;
}
const std::string&                      AdaptationSet::GetLang                          ()  const
{
    return this->lang;
}
void                                    AdaptationSet::SetLang                          (const std::string& aLang)
{
    this->lang = aLang;
}
const std::string&                      AdaptationSet::GetContentType                   ()  const
{
    return this->contentType;
}
void                                    AdaptationSet::SetContentType                   (const std::string& aContentType)
{
    this->contentType = aContentType;
}
const std::string&                      AdaptationSet::GetPar                           ()  const
{
    return this->par;
}
void                                    AdaptationSet::SetPar                           (const std::string& aPar)
{
    this->par = aPar;
}
uint32_t                                AdaptationSet::GetMinBandwidth                  ()  const
{
    return this->minBandwidth;
}
void                                    AdaptationSet::SetMinBandwidth                  (uint32_t aMinBandwidth)
{
    this->minBandwidth = aMinBandwidth;
}
uint32_t                                AdaptationSet::GetMaxBandwidth                  ()  const
{
    return this->maxBandwidth;
}
void                                    AdaptationSet::SetMaxBandwidth                  (uint32_t aMaxBandwidth)
{
    this->maxBandwidth = aMaxBandwidth;
}
uint32_t                                AdaptationSet::GetMinWidth                      ()  const
{
    return this->minWidth;
}
void                                    AdaptationSet::SetMinWidth                      (uint32_t aMinWidth)
{
    this->minWidth = aMinWidth;
}
uint32_t                                AdaptationSet::GetMaxWidth                      ()  const
{
    return this->maxWidth;
}
void                                    AdaptationSet::SetMaxWidth                      (uint32_t aMaxWidth)
{
    this->maxWidth = aMaxWidth;
}
uint32_t                                AdaptationSet::GetMinHeight                     ()  const
{
    return this->minHeight;
}
void                                    AdaptationSet::SetMinHeight                     (uint32_t aMinHeight)
{
    this->minHeight = aMinHeight;
}
uint32_t                                AdaptationSet::GetMaxHeight                     ()  const
{
    return this->maxHeight;
}
void                                    AdaptationSet::SetMaxHeight                     (uint32_t aMaxHeight)
{
    this->maxHeight = aMaxHeight;
}
const std::string&                      AdaptationSet::GetMinFramerate                  ()  const
{
    return this->minFramerate;
}
void                                    AdaptationSet::SetMinFramerate                  (const std::string& aMinFramerate)
{
    this->minFramerate = aMinFramerate;
}
const std::string&                      AdaptationSet::GetMaxFramerate                  ()  const
{
    return this->maxFramerate;
}
void                                    AdaptationSet::SetMaxFramerate                  (const std::string& aMaxFramerate)
{
    this->maxFramerate = aMaxFramerate;
}
bool                                    AdaptationSet::SegmentAlignmentIsBoolValue      () const
{
    return this->segmentAlignmentIsBool;
}
bool                                    AdaptationSet::SubsegmentAlignmentIsBoolValue   () const
{
    return this->subsegmentAlignmentIsBool;
}
bool                                    AdaptationSet::HasSegmentAlignment              () const
{
    return this->usesSegmentAlignment;
}
bool                                    AdaptationSet::HasSubsegmentAlignment           () const
{
    return this->usesSubsegmentAlignment;
}
uint32_t                                AdaptationSet::GetSegmentAligment               ()  const
{
    return this->segmentAlignment;
}
void                                    AdaptationSet::SetSegmentAlignment              (const std::string& aSegmentAlignment)
{
    if (aSegmentAlignment == "true" || aSegmentAlignment == "True" || aSegmentAlignment == "TRUE")
    {
        this->segmentAlignmentIsBool = true;
        this->usesSegmentAlignment = true;
        return;
    }
    
    if (aSegmentAlignment == "false" || aSegmentAlignment == "False" || aSegmentAlignment == "FALSE")
    {
        this->segmentAlignmentIsBool = true;
        this->usesSegmentAlignment = false;
        return;
    }

    this->segmentAlignmentIsBool = false;
    this->segmentAlignment = strtoul(aSegmentAlignment.c_str(), NULL, 10);
}
void                                    AdaptationSet::SetSubsegmentAlignment           (const std::string& aSubsegmentAlignment)
{
    if (aSubsegmentAlignment == "true" || aSubsegmentAlignment == "True" || aSubsegmentAlignment == "TRUE")
    {
        this->subsegmentAlignmentIsBool = true;
        this->usesSubsegmentAlignment = true;
        return;
    }
    
    if (aSubsegmentAlignment == "false" || aSubsegmentAlignment == "False" || aSubsegmentAlignment == "FALSE")
    {
        this->subsegmentAlignmentIsBool = true;
        this->usesSubsegmentAlignment = false;
        return;
    }

    this->subsegmentAlignmentIsBool = false;
    this->subsegmentAlignment = strtoul(aSubsegmentAlignment.c_str(), NULL, 10);
}
uint32_t                                AdaptationSet::GetSubsegmentAlignment           ()  const
{
    return this->subsegmentAlignment;
}
uint8_t                                 AdaptationSet::GetSubsegmentStartsWithSAP       ()  const
{
    return this->subsegmentStartsWithSAP;
}
void                                    AdaptationSet::SetSubsegmentStartsWithSAP       (uint8_t aSubsegmentStartsWithSAP)
{
    this->subsegmentStartsWithSAP = aSubsegmentStartsWithSAP;
}
bool                                    AdaptationSet::GetBitstreamSwitching            () const
{
    return this->isBitstreamSwitching;
}
void                                    AdaptationSet::SetBitstreamSwitching            (bool value)
{
    this->isBitstreamSwitching = value;
}
