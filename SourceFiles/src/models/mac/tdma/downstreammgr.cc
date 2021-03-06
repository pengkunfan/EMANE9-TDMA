/*
 * Copyright (c) Her Majesty the Queen in right of Canada  (2014)
 * Copyright (c) 2013-2014 - Adjacent Link LLC, Bridgewater, New Jersey
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of Adjacent Link LLC nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "downstreammgr.h"

namespace 
{
  const std::uint16_t QUEUE_SIZE_DEFAULT{0xFF};
}

EMANE::Models::TDMA::DownstreamQueueMgr::DownstreamQueueMgr():
  maxQueueSize_{QUEUE_SIZE_DEFAULT},
  numDiscards_{}
{}


EMANE::Models::TDMA::DownstreamQueueMgr::~DownstreamQueueMgr() 
{}

size_t 
EMANE::Models::TDMA::DownstreamQueueMgr::getNumDiscards(bool bClear) 
{ 
   size_t result{numDiscards_};

   if(bClear)
     {
       numDiscards_ = 0;
     }

   return result;
}


size_t 
EMANE::Models::TDMA::DownstreamQueueMgr::getCurrentDepth()
{ 
   return queue_.size();
}


size_t 
EMANE::Models::TDMA::DownstreamQueueMgr::getMaxCapacity()
{ 
   return maxQueueSize_;
}

bool 
EMANE::Models::TDMA::DownstreamQueueMgr::empty()
{ 
   return queue_.empty();
}


std::pair<EMANE::Models::TDMA::DownstreamQueueEntry,bool>
EMANE::Models::TDMA::DownstreamQueueMgr::dequeue()
{ 
  if(queue_.empty())
     {
       return {{},false};
     }

  DownstreamQueueEntry entry{queue_.front()};
  
  queue_.pop_front();
  
  return {entry,true};
}


std::vector<EMANE::Models::TDMA::DownstreamQueueEntry>
EMANE::Models::TDMA::DownstreamQueueMgr::enqueue(DownstreamQueueEntry &entry) 
{ 
   std::vector<DownstreamQueueEntry> result;

   // check for queue overflow
   while(queue_.size() >= maxQueueSize_) 
     {
       ++numDiscards_;

       result.push_back(std::move(queue_.front()));

       queue_.pop_front();
     }

   queue_.push_back(std::move(entry));

   return result;
}

void 
EMANE::Models::TDMA::DownstreamQueueMgr::enqueue_front(DownstreamQueueEntry &entry) 
{ 
   queue_.push_front(std::move(entry));
}



const EMANE::Models::TDMA::DownstreamQueueEntry & 
EMANE::Models::TDMA::DownstreamQueueMgr::peek()
{ 
   return queue_.front();
}

