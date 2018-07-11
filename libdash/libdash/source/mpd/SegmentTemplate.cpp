/*
 * SegmentTemplate.cpp
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "SegmentTemplate.h"
#include "Segment.h"

using namespace dash::mpd;
using namespace dash::metrics;

SegmentTemplate::SegmentTemplate    () :
                    media(""),
                    index(""),
                    initialization(""),
                    bitstreamSwitching("")
{
}
SegmentTemplate::~SegmentTemplate   ()
{
}

const std::string&  SegmentTemplate::Getmedia                       ()  const
{
    return this->media;
}
void                SegmentTemplate::SetMedia                       (const std::string& amedia)
{
    this->media = amedia;
}
const std::string&  SegmentTemplate::Getindex                       ()  const
{
    return this->index;
}
void                SegmentTemplate::SetIndex                       (const std::string& aindex)
{
    this->index = aindex;
}
const std::string&  SegmentTemplate::Getinitialization              ()  const
{
    return this->initialization;
}
void                SegmentTemplate::SetInitialization              (const std::string& ainitialization)
{
    this->initialization = ainitialization;
}
const std::string&  SegmentTemplate::GetbitstreamSwitching          ()  const
{
    return this->bitstreamSwitching;
}
void                SegmentTemplate::SetBitstreamSwitching          (const std::string& abitstreamSwitching)
{
    this->bitstreamSwitching = abitstreamSwitching;
}
ISegment*           SegmentTemplate::ToInitializationSegment        (const std::vector<IBaseUrl *>& baseurls, const std::string& representationID, uint64_t bandwidth) const
{
    return ToSegment(this->initialization, baseurls, representationID, bandwidth, dash::metrics::InitializationSegment);
}
ISegment*           SegmentTemplate::ToBitstreamSwitchingSegment    (const std::vector<IBaseUrl *>& baseurls, const std::string& representationID, uint64_t bandwidth) const
{
    return ToSegment(this->bitstreamSwitching, baseurls, representationID, bandwidth, dash::metrics::BitstreamSwitchingSegment);
}
ISegment*           SegmentTemplate::GetMediaSegmentFromNumber      (const std::vector<IBaseUrl *>& baseurls, const std::string& representationID, uint64_t bandwidth, uint64_t number) const
{
    return ToSegment(this->media, baseurls, representationID, bandwidth, dash::metrics::MediaSegment, number);
}
ISegment*           SegmentTemplate::GetIndexSegmentFromNumber      (const std::vector<IBaseUrl *>& baseurls, const std::string& representationID, uint64_t bandwidth, uint64_t number) const
{
    return ToSegment(this->index, baseurls, representationID, bandwidth, dash::metrics::IndexSegment, number);
}
ISegment*           SegmentTemplate::GetMediaSegmentFromTime        (const std::vector<IBaseUrl *>& baseurls, const std::string& representationID, uint64_t bandwidth, uint64_t time) const
{
    return ToSegment(this->media, baseurls, representationID, bandwidth, dash::metrics::MediaSegment, 0, time);
}
ISegment*           SegmentTemplate::GetIndexSegmentFromTime        (const std::vector<IBaseUrl *>& baseurls, const std::string& representationID, uint64_t bandwidth, uint64_t time) const
{
    return ToSegment(this->index, baseurls, representationID, bandwidth, dash::metrics::IndexSegment, 0, time);
}
std::string         SegmentTemplate::ReplaceParameters              (const std::string& uri, const std::string& representationID, uint64_t bandwidth, uint64_t number, uint64_t time) const
{
    //see dash spec part 1 @ 5.3.9.4.4 Template-based Segment URL construction 
    std::string replacedUri;
    int cpos = 0;
    for (;;)
    {
        size_t pos = uri.find('$', cpos);
        if (pos == std::string::npos)
        {
            //add the leftovers.
            replacedUri.append(uri.data() + cpos);
            break;
        }
        replacedUri.append(uri.data() + cpos, pos - cpos);
        size_t end = uri.find('$', pos + 1);
        if (end == std::string::npos)
        {
            //no terminating $?
            //just add it as is. 
            //technically the uri is corrupted, and the whole representation should be ignored!
            replacedUri.append(uri.data() + pos);
            break;
        }
        pos++;
        size_t id_len = end - pos;
        if (id_len == 0)
        {
            //id_len==0 means we got $$
            replacedUri += '$';
        }
        else if (uri.compare(pos, id_len, "RepresentationID") == 0) 
        {
            replacedUri.append(representationID);
        }
        else
        {
            //Check for formatter.
            uint64_t value;
            int64_t width = 1; //width = 1 if not defined.
            bool valid = true;
            size_t fmt = uri.find('%', pos);
            if ((fmt != std::string::npos) && (end>fmt))
            {
                //technically format string should be "%0[width]d"
                //not validating hoping for it to be correct.
                id_len = fmt - pos;
                fmt++;
                if (uri[fmt] == '0') fmt++;
                if (uri[end - 1] == 'd')
                {
                    width = 0;
                    //TODO: this could be done better too. (validation and conversion from string to number)
                    for (int t = fmt; t < end - 1; t++)
                    {
                        width *= 10;
                        if ((uri[t] >= '0') && (uri[t] <= '9'))
                        {
                            width += uri[t] - '0';
                        }
                        else
                        {
                            //corrupted formatter.
                            valid = false;
                            break;
                        }
                    }
                }
                else
                {
                    //corrupted formatter.
                    valid = false;
                }
            }
            if (uri.compare(pos, id_len, "Number") == 0)
            {
                value = number;
            }
            else if (uri.compare(pos, id_len, "Bandwidth") == 0)
            {
                value = bandwidth;
            }
            else if (uri.compare(pos, id_len, "Time") == 0)
            {
                value = time;
            }
            else
            {
                //Corrupted!
                valid = false;
            }
            if (valid)
            {
                //TODO: replace this with something better.
                //ie. convert number to string with specified minimum digits with zero left fill..
                char result[64];
                char fmtstring[64];
                sprintf(fmtstring, "%%0%llullu", width);
                sprintf(result, fmtstring, value);
                replacedUri.append(result);
            }
            else
            {
                //technically the uri is corrupted, and the whole representation should be ignored!
                //just add the identifier as is..
                replacedUri.append(uri.data() + pos - 1, end - pos + 2);
            }
        }
        cpos = end + 1;
    }
    return replacedUri;
}
ISegment*           SegmentTemplate::ToSegment                      (const std::string& uri, const std::vector<IBaseUrl *>& baseurls, const std::string& representationID, uint64_t bandwidth, HTTPTransactionType type, uint64_t number, uint64_t time) const
{
    Segment *seg = new Segment();

    if(seg->Init(baseurls, ReplaceParameters(uri, representationID, bandwidth, number, time), "", type))
        return seg;

    delete(seg);

    return NULL;
}
