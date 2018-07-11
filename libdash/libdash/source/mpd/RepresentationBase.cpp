/*
 * RepresentationBase.cpp
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "RepresentationBase.h"

using namespace dash::mpd;

RepresentationBase::RepresentationBase  () :
                    width(0),
                    height(0),
                    sar(""),
                    frameRate(""),
                    audioSamplingRate(""),
                    mimeType(""),
                    maximumSAPPeriod(0.0),
                    startWithSAP(0),
                    maxPlayoutRate(0.0),
                    codingDependency(false),
                    scanType("")
{
}
RepresentationBase::~RepresentationBase ()
{
    for(size_t i = 0; i < this->framePacking.size(); i++)
        delete(this->framePacking.at(i));
    for(size_t i = 0; i < this->audioChannelConfiguration.size(); i++)
        delete(this->audioChannelConfiguration.at(i));
    for(size_t i = 0; i < this->contentProtection.size(); i++)
        delete(this->contentProtection.at(i));
}

const std::vector<IDescriptor*>&    RepresentationBase::GetFramePacking                 () const 
{
    return (std::vector<IDescriptor*> &) this->framePacking;
}
void                                RepresentationBase::AddFramePacking                 (Descriptor *aframePacking) 
{
    this->framePacking.push_back(aframePacking);
}
const std::vector<IDescriptor*>&    RepresentationBase::GetAudioChannelConfiguration    () const
{
    return (std::vector<IDescriptor*> &) this->audioChannelConfiguration;
}
void                                RepresentationBase::AddAudioChannelConfiguration    (Descriptor *aaudioChannelConfiguration)
{
    this->audioChannelConfiguration.push_back(aaudioChannelConfiguration);
}
const std::vector<IDescriptor*>&    RepresentationBase::GetContentProtection            () const
{
    return (std::vector<IDescriptor*> &) this->contentProtection;
}
void                                RepresentationBase::AddContentProtection            (Descriptor *acontentProtection)
{
    this->contentProtection.push_back(acontentProtection);
}
const std::vector<std::string>&     RepresentationBase::GetProfiles                     () const
{
    return this->profiles;
}
void                                RepresentationBase::SetProfiles                     (const std::string& aprofiles)
{
    dash::helpers::String::Split(aprofiles, ',', this->profiles);
}
uint32_t                            RepresentationBase::GetWidth                        () const
{
    return this->width;
}
void                                RepresentationBase::SetWidth                        (uint32_t awidth)
{
    this->width = awidth;
}
uint32_t                            RepresentationBase::GetHeight                       () const
{
    return this->height;
}
void                                RepresentationBase::SetHeight                       (uint32_t aheight)
{
    this->height = aheight;
}
const std::string&                  RepresentationBase::GetSar                          () const
{
    return this->sar;
}
void                                RepresentationBase::SetSar                          (const std::string& asar)
{
    this->sar = asar;
}
const std::string&                  RepresentationBase::GetFrameRate                    () const
{
    return this->frameRate;
}
void                                RepresentationBase::SetFrameRate                    (const std::string& aframeRate)
{
    this->frameRate = aframeRate;
}
const std::string&                  RepresentationBase::GetAudioSamplingRate            () const
{
    return this->audioSamplingRate;
}
void                                RepresentationBase::SetAudioSamplingRate            (const std::string& aaudioSamplingRate)
{
    this->audioSamplingRate = aaudioSamplingRate;
}
const std::string&                  RepresentationBase::GetMimeType                     () const
{
    return this->mimeType;
}
void                                RepresentationBase::SetMimeType                     (const std::string& amimeType)
{
    this->mimeType = amimeType;
}
const std::vector<std::string>&     RepresentationBase::GetSegmentProfiles              () const
{
    return this->segmentProfiles;
}
void                                RepresentationBase::SetSegmentProfiles              (const std::string& asegmentProfiles)
{
    dash::helpers::String::Split(asegmentProfiles, ',', this->segmentProfiles);
}
const std::vector<std::string>&     RepresentationBase::GetCodecs                       () const
{
    return this->codecs;
}
void                                RepresentationBase::SetCodecs                       (const std::string& acodecs)
{
    dash::helpers::String::Split(acodecs, ',', this->codecs);
}
double                              RepresentationBase::GetMaximumSAPPeriod             () const
{
    return this->maximumSAPPeriod;
}
void                                RepresentationBase::SetMaximumSAPPeriod             (double amaximumSAPPeriod)
{
    this->maximumSAPPeriod = amaximumSAPPeriod;
}
uint8_t                             RepresentationBase::GetStartWithSAP                 () const
{
    return this->startWithSAP;
}
void                                RepresentationBase::SetStartWithSAP                 (uint8_t astartWithSAP)
{
    this->startWithSAP = astartWithSAP;
}
double                              RepresentationBase::GetMaxPlayoutRate               () const
{
    return this->maxPlayoutRate;
}
void                                RepresentationBase::SetMaxPlayoutRate               (double amaxPlayoutRate)
{
    this->maxPlayoutRate = amaxPlayoutRate;
}
bool                                RepresentationBase::HasCodingDependency             () const
{
    return this->codingDependency;
}
void                                RepresentationBase::SetCodingDependency             (bool acodingDependency)
{
    this->codingDependency = acodingDependency;
}
const std::string&                  RepresentationBase::GetScanType                     () const
{
    return this->scanType;
}
void                                RepresentationBase::SetScanType                     (const std::string& ascanType)
{
    this->scanType = ascanType;
}
