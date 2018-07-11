/* 
 * Descriptor.cpp
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "Descriptor.h"

using namespace dash::mpd;

Descriptor::Descriptor  () :
            schemeIdUri (""),
            value       ("")
{
}
Descriptor::~Descriptor ()
{
}
const std::string&  Descriptor::GetSchemeIdUri  ()  const
{
    return this->schemeIdUri;
}
void                Descriptor::SetSchemeIdUri  (const std::string& aschemeIdUri) 
{
    this->schemeIdUri = aschemeIdUri;
}
const std::string&  Descriptor::GetValue        ()  const
{
    return this->value;
}
void                Descriptor::SetValue        (const std::string& avalue) 
{
    this->value = avalue;
}
