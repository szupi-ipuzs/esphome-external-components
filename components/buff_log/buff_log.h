#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/logger/logger.h"
#include <string>
#include <cstdint>
#include <queue>
#include <functional>

namespace esphome{

namespace buff_log{

struct LogItem
{
   int level;
   std::string tag;
   std::string payload;
   std::uint32_t combinedLength;
};

static const char* TAG = "bufflog";

enum DumpingTrigger
{
   NONE,
   OVERFLOW,
   TIMEOUT,
   FORCED
};

class BufferingLogger : public Component
{
public:

   BufferingLogger(std::uint32_t bufferingTimeout, std::uint32_t sizeLimit, std::uint32_t dumpLinesPerLoop, std::function<void(const LogItem&)> dumpProcessor, std::function<void(DumpingTrigger)> dumpFinishedHandler = [](DumpingTrigger){}):
   m_bufferingTimeout(bufferingTimeout),
   m_sizeLimit(sizeLimit),
   m_dumpLinesPerLoop(dumpLinesPerLoop),
   m_dumpProcessor(std::move(dumpProcessor)),
   m_dumpFinishedHandler(std::move(dumpFinishedHandler))
   {}

   float get_setup_priority() const override
   {
      if (logger::global_logger != nullptr)
      {
         return logger::global_logger->get_setup_priority() - 100.0;
      }
      else
      {
         return setup_priority::BUS;
      }
   }

   void setup() override
   {
      ESP_LOGI(TAG, "setup");
      if (logger::global_logger != nullptr)
      {
         logger::global_logger->add_on_log_callback([this](int level, const char *tag, const char *message)
         {
            if ((m_state != State::BUFFERING) || (m_paused))
            {
               return;
            }

            uint32_t combinedLength = strlen(tag) + strlen(message);
            m_items.emplace(LogItem{level, std::string(tag), std::string(message), combinedLength});
            m_currentSize += combinedLength;
            if (m_currentSize >= m_sizeLimit)
            {
               m_state = State::DUMPING;
               m_trigger = DumpingTrigger::OVERFLOW;
               ESP_LOGI(TAG, "Starting dump due to overflow");
            }
         });
      }

      m_state = State::BUFFERING;
      m_startTimestamp = esphome::millis();
   }

   void loop() override
   {
      if (m_state == State::BUFFERING)
      {
         loopWhenBuffering();
      }

      if (m_state == State::DUMPING)
      {
         loopWhenDumping();
      }
   }

   void stop()
   {
      m_state = State::IDLE;
      clearQueue();
      ESP_LOGI(TAG, "Stopped");
   }

   void restart()
   {
      ESP_LOGI(TAG, "Restarting");
      clearQueue();
      m_paused = false;
      m_state = State::BUFFERING;
      m_startTimestamp = esphome::millis();
      m_trigger = DumpingTrigger::NONE;
   }

   bool isBuffering() const
   {
      return ((State::BUFFERING == m_state) && (!m_paused));
   }

   std::uint32_t getFillLevel() const
   {
      return m_currentSize;
   }

   void dump()
   {
      m_state = State::DUMPING;
      m_paused = false;
      m_trigger = DumpingTrigger::FORCED;
      ESP_LOGI(TAG, "Forcing dump");
   }

   void pause()
   {
      m_paused = true;
      ESP_LOGI(TAG, "Paused");
   }

   void resume()
   {
      m_paused = false;
      ESP_LOGI(TAG, "Resuming");
   }

   DumpingTrigger getDumpingTrigger() const
   {
      return m_trigger;
   }

private:

   enum State
   {
      IDLE,
      BUFFERING,
      DUMPING
   };

   void clearQueue()
   {
      std::queue<LogItem> queue;
      m_items.swap(queue);
      m_currentSize = 0;
   }

   void loopWhenBuffering()
   {
      if ((m_bufferingTimeout != 0) && (m_startTimestamp - millis() > m_bufferingTimeout))
      {
         m_paused = false;
         m_state = State::DUMPING;
         m_trigger = DumpingTrigger::TIMEOUT;
         ESP_LOGI(TAG, "Starting dump due to timeout");
      }
   }

   void loopWhenDumping()
   {
      uint32_t itemsDumped = 0;
      while ((m_state == State::DUMPING) && (!m_paused) && (itemsDumped < m_dumpLinesPerLoop))
      {
         if (m_items.empty())
         {
            stop();
            m_dumpFinishedHandler(m_trigger);
            break;
         }

         ESP_LOGI(TAG, "dumping, left=%zu", m_items.size());

         auto item = std::move(m_items.front());
         m_items.pop();
         m_currentSize -= item.combinedLength;
         m_dumpProcessor(item);
         ++itemsDumped;
      }
   }

   const std::uint32_t m_bufferingTimeout;
   const std::uint32_t m_sizeLimit;
   const std::uint32_t m_dumpLinesPerLoop;
   std::function<void(const LogItem&)> m_dumpProcessor;
   std::function<void(DumpingTrigger)> m_dumpFinishedHandler;
   State m_state{State::IDLE};
   std::uint32_t m_startTimestamp{0u};
   bool m_paused{false};
   DumpingTrigger m_trigger{DumpingTrigger::NONE};

   std::queue<LogItem> m_items;
   std::uint32_t m_currentSize{0u};
};

}}
