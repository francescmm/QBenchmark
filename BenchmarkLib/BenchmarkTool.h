﻿#pragma once

/****************************************************************************
**
** Copyright (C) 2020 Francesc Martinez
** LinkedIn: www.linkedin.com/in/cescmm/
** Web: www.francescmm.com
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of Francesc M. nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************/

#include <TimeProvider.h>
#include <BenchmarkCallback.h>

#include <mutex>
#include <thread>
#include <map>
#include <sstream>
#include <atomic>

namespace Benchmarker
{

class Node;

class BenchmarkTool
{
public:
   enum class OutputFormat
   {
      PlainText
   };

   static BenchmarkTool &getInstance(std::shared_ptr<ITimeProvider> timeProvider = std::make_shared<TimeProvider>());

   ~BenchmarkTool();

   void startBenchmark(const std::string &methodName);
   void startBenchmark(const std::string &methodName, const std::string &comment);
   void endBenchmark(const std::string &methodName);

   void setOutputFormat(OutputFormat outputFormat) { mFileFormat = outputFormat; }

   int addListener(ListenerCallback callback);
   void removeListener(int listenerId);

   friend std::string &operator<<(std::string &out, const BenchmarkTool &node);
   friend std::ostream &operator<<(std::ostream &out, const BenchmarkTool &node);

private:
   BenchmarkTool(std::shared_ptr<ITimeProvider> timeProvider);

   std::shared_ptr<ITimeProvider> mTimeProvider = nullptr;
   std::mutex mMutex;
   std::unique_ptr<Node> mRootNode;
   std::map<std::string, Node *> mActiveNode;
   std::string mThreadId;
   OutputFormat mFileFormat = OutputFormat::PlainText;
   std::mutex mListenersMutex;
   std::atomic<int> mListenerId { 0 };
   std::map<int, ListenerCallback> mListeners;
};

#ifndef PLATFORM_WINDOWS
#   ifndef BenchmarkStart
#      define BenchmarkStart() BenchmarkTool::getInstance().startBenchmark(__PRETTY_FUNCTION__)
#      define BenchmarkStartMsg(msg) BenchmarkTool::getInstance().startBenchmark(__PRETTY_FUNCTION__, msg)
#   endif
#else
#   ifndef BenchmarkStart
#      define BenchmarkStart() BenchmarkTool::getInstance().startBenchmark(__FUNCSIG__)
#      define BenchmarkStartMsg(msg) BenchmarkTool::getInstance().startBenchmark(__FUNCSIG__, msg)
#   endif
#endif

#ifndef PLATFORM_WINDOWS
#   ifndef BenchmarkEnd
#      define BenchmarkEnd() BenchmarkTool::getInstance().endBenchmark(__PRETTY_FUNCTION__)
#   endif
#else
#   ifndef BenchmarkEnd
#      define BenchmarkEnd() BenchmarkTool::getInstance().endBenchmark(__FUNCSIG__)
#   endif
#endif

}
