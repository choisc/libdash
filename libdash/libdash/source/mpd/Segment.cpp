/*
 * Segment.cpp
 *****************************************************************************
 * Copyright (C) 2013, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "Segment.h"
#include "../helpers/Uri.h"

using namespace dash::mpd;
using namespace dash::helpers;
using namespace dash::metrics;

Segment::Segment    ()  :
            host(""),
            port(0),
            path(""),
            startByte(0),
            endByte(0),
            hasByteRange(false)
{
}
Segment::~Segment   ()
{
}

bool                Segment::Init               (const std::vector<IBaseUrl *>& baseurls, const std::string &uri, const std::string &arange, HTTPTransactionType atype)
{
    std::string thost        = "";
    size_t      tport        = 80;
    std::string tpath        = "";
    size_t      tstartByte   = 0;
    size_t      tendByte     = 0;

    this->absoluteuri = "";

    Uri base;
    for (size_t i = 0; i < baseurls.size(); i++)
    {
        base.resolve(baseurls.at(i)->GetUrl());
    }
    base.resolve(uri);

    this->absoluteuri = base.toString();

    if (uri != "" && dash::helpers::Path::GetHostPortAndPath(this->absoluteuri, thost, tport, tpath))
    {
        this->host = thost;
        this->port = tport;
        this->path = tpath;

        if (arange != "" && dash::helpers::Path::GetStartAndEndBytes(arange, tstartByte, tendByte))
        {
            this->range         = arange;
            this->hasByteRange  = true;
            this->startByte     = tstartByte;
            this->endByte       = tendByte;
        }

        this->type = atype;

        return true;
    }

    return false;
}
std::string&        Segment::AbsoluteURI        ()
{
    return this->absoluteuri;
}
std::string&        Segment::Host               ()
{
    return this->host;
}
size_t              Segment::Port               ()
{
    return this->port;
}
std::string&        Segment::Path               ()
{
    return this->path;
}
std::string&        Segment::Range              ()
{
    return this->range;
}
size_t              Segment::StartByte          ()
{
    return this->startByte;
}
size_t              Segment::EndByte            ()
{
    return this->endByte;
}
bool                Segment::HasByteRange       ()
{
    return this->hasByteRange;
}
void                Segment::SetAbsoluteURI        (const std::string& uri)
{
    this->absoluteuri = uri;
}
void                Segment::SetHost               (const std::string& ahost)
{
    this->host = ahost;
}
void                Segment::SetPort               (size_t aport)
{
    this->port = aport;
}
void                Segment::SetPath               (const std::string& apath)
{
    this->path = apath;
}
void                Segment::SetRange              (const std::string& arange)
{
    this->range = arange;
}
void                Segment::SetStartByte          (size_t astartByte)
{
    this->startByte = astartByte;
}
void                Segment::SetEndByte            (size_t aendByte)
{
    this->endByte = aendByte;
}
void                Segment::SetHasByteRange       (bool ahasByteRange)
{
    this->hasByteRange = ahasByteRange;
}
HTTPTransactionType Segment::GetType            ()
{
    return this->type;
}
void                Segment::SetType            (HTTPTransactionType atype)
{
    this->type = atype;
}
