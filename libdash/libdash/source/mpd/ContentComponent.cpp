/*
 * ContentComponent.cpp
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "ContentComponent.h"

using namespace dash::mpd;

ContentComponent::ContentComponent  ()  :
                    id(0),
                    lang(""),
                    contentType(""),
                    par("")
{
}
ContentComponent::~ContentComponent ()
{
    for(size_t i = 0; i < this->accessibility.size(); i++)
        delete(this->accessibility.at(i));
    for(size_t i = 0; i < this->role.size(); i++)
        delete(this->role.at(i));
    for(size_t i = 0; i < this->rating.size(); i++)
        delete(this->rating.at(i));
    for(size_t i = 0; i < this->viewpoint.size(); i++)
        delete(this->viewpoint.at(i));
}

const std::vector<IDescriptor *>&   ContentComponent::GetAccessibility  ()  const
{
    return (std::vector<IDescriptor *> &)this->accessibility;
}
void                                ContentComponent::AddAccessibity    (Descriptor *aaccessibility)
{
    this->accessibility.push_back(aaccessibility);
}
const std::vector<IDescriptor *>&   ContentComponent::GetRole           ()  const
{
    return (std::vector<IDescriptor *> &)this->role;
}
void                                ContentComponent::AddRole           (Descriptor *arole)
{
    this->role.push_back(arole);
}
const std::vector<IDescriptor *>&   ContentComponent::GetRating         ()  const
{
    return (std::vector<IDescriptor *> &)this->rating;
}
void                                ContentComponent::AddRating         (Descriptor *arating)
{
    this->rating.push_back(arating);
}
const std::vector<IDescriptor *>&   ContentComponent::GetViewpoint      ()  const
{
    return (std::vector<IDescriptor *> &)this->viewpoint;
}
void                                ContentComponent::AddViewpoint      (Descriptor *aviewpoint)
{
    this->viewpoint.push_back(aviewpoint);
}
uint32_t                            ContentComponent::GetId             ()  const
{
    return this->id;
}
void                                ContentComponent::SetId             (uint32_t aid)
{
    this->id = aid;
}
const std::string&                  ContentComponent::GetLang           ()  const
{
    return this->lang;
}
void                                ContentComponent::SetLang           (const std::string& alang)
{
    this->lang = alang;
}
const std::string&                  ContentComponent::GetContentType    ()  const
{
    return this->contentType;
}
void                                ContentComponent::SetContentType    (const std::string& acontentType)
{
    this->contentType = acontentType;
}
const std::string&                  ContentComponent::GetPar            ()  const
{
    return this->par;
}
void                                ContentComponent::SetPar            (const std::string& apar)
{
    this->par = apar;
}
