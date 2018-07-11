/*
 * ProgramInformation.cpp
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "ProgramInformation.h"

using namespace dash::mpd;

ProgramInformation::ProgramInformation  () :
                    title(""),
                    source(""),
                    copyright(""),
                    lang(""),
                    moreInformationURL("")
{
}
ProgramInformation::~ProgramInformation ()
{
}

const std::string&  ProgramInformation::GetTitle                ()  const
{
    return this->title;
}
void                ProgramInformation::SetTitle                (const std::string& aTitle)
{
    this->title = aTitle;
}
const std::string&  ProgramInformation::GetSource               ()  const
{
    return this->source;
}
void                ProgramInformation::SetSource               (const std::string& aSource)
{
    this->source = aSource;
}
const std::string&  ProgramInformation::GetCopyright            ()  const
{
    return this->copyright;
}
void                ProgramInformation::SetCopyright            (const std::string& aCopyright)
{
    this->copyright = aCopyright;
}
const std::string&  ProgramInformation::GetLang                 ()  const
{
    return this->lang;
}
void                ProgramInformation::SetLang                 (const std::string& aLang)
{
    this->lang = aLang;
}
const std::string&  ProgramInformation::GetMoreInformationURL   ()  const
{
    return this->moreInformationURL;
}
void                ProgramInformation::SetMoreInformationURL   (const std::string& aMoreInfoURL)
{
    this->moreInformationURL = aMoreInfoURL;
}
