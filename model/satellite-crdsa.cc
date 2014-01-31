/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Magister Solutions Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Frans Laakso <frans.laakso@magister.fi>
 */
#include "satellite-crdsa.h"

NS_LOG_COMPONENT_DEFINE ("SatCrdsa");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (SatCrdsa);

TypeId 
SatCrdsa::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SatCrdsa")
    .SetParent<Object> ();
  return tid;
}

SatCrdsa::SatCrdsa () :
  m_randomAccessConf (),
  m_uniformVariable (),
  m_min (0.0),
  m_max (0.0),
  m_setSize (0),
  m_newData (true),
  m_backoffReleaseTime (0.0),
  m_backoffPeriodLength (0.0),
  m_backoffProbability (0.0)
{
  NS_LOG_FUNCTION (this);

  NS_FATAL_ERROR ("SatCrdsa::SatCrdsa - Constructor not in use");
}

SatCrdsa::SatCrdsa (Ptr<SatRandomAccessConf> randomAccessConf) :
  m_randomAccessConf (randomAccessConf),
  m_uniformVariable (),
  m_min (randomAccessConf->GetCrdsaDefaultMin ()),
  m_max (randomAccessConf->GetCrdsaDefaultMax ()),
  m_setSize (randomAccessConf->GetCrdsaDefaultSetSize ()),
  m_newData (true),
  m_backoffReleaseTime (Now ().GetSeconds ()),
  m_backoffPeriodLength (randomAccessConf->GetCrdsaDefaultBackoffPeriodLength ()),
  m_backoffProbability (randomAccessConf->GetCrdsaDefaultBackoffPeriodProbability ())
{
  NS_LOG_FUNCTION (this);

  m_uniformVariable = CreateObject<UniformRandomVariable> ();

  DoVariableSanityCheck ();

  NS_LOG_INFO ("SatCrdsa::SatCrdsa - Module created");
}

SatCrdsa::~SatCrdsa ()
{
  NS_LOG_FUNCTION (this);
}

void
SatCrdsa::DoVariableSanityCheck ()
{
  NS_LOG_FUNCTION (this);

  if (m_min < 0 || m_max < 0)
    {
      NS_FATAL_ERROR ("SatCrdsa::DoVariableSanityCheck - min < 0 || max < 0");
    }

  if (m_min > m_max)
    {
      NS_FATAL_ERROR ("SatCrdsa::DoVariableSanityCheck - min > max");
    }

  if ( m_setSize < 1)
    {
      NS_FATAL_ERROR ("SatCrdsa::DoVariableSanityCheck - set size < 1");
    }

  if ( (m_max - m_min) < m_setSize)
    {
      NS_FATAL_ERROR ("SatCrdsa::DoVariableSanityCheck - (max - min) < set size");
    }

  NS_LOG_INFO ("SatCrdsa::DoVariableSanityCheck - Variable sanity check done");
}

void
SatCrdsa::UpdateRandomizationVariables (uint32_t min, uint32_t max, uint32_t setSize)
{
  NS_LOG_FUNCTION (this << " new min: " << min << " new max: " << max << " new set size: " << setSize);

  m_min = min;
  m_max = max;
  m_setSize = setSize;

  DoVariableSanityCheck ();

  NS_LOG_INFO ("SatCrdsa::UpdateVariables - new min: " << min << " new max: " << max << " new set size: " << setSize);
}

void
SatCrdsa::SetBackoffProbability (double backoffProbability)
{
  m_backoffProbability = backoffProbability;
}

void
SatCrdsa::SetBackoffPeriodLength (double backoffPeriodLength)
{
  m_backoffPeriodLength = backoffPeriodLength;
}

bool
SatCrdsa::IsBackoffPeriodOver ()
{
  NS_LOG_FUNCTION (this);

  bool isBackoffPeriodOver = false;

  if (Now ().GetSeconds () >= m_backoffReleaseTime)
    {
      isBackoffPeriodOver = true;
    }

  NS_LOG_INFO ("SatCrdsa::IsBackoffPeriodOver: " << isBackoffPeriodOver);

  return isBackoffPeriodOver;
}

bool
SatCrdsa::DoBackoff ()
{
  NS_LOG_FUNCTION (this);

  bool doBackoff = false;

  if (m_uniformVariable->GetValue (0.0,1.0) < m_backoffProbability)
    {
      doBackoff = true;
    }

  NS_LOG_INFO ("SatCrdsa::DoBackoff: " << doBackoff);

  return doBackoff;
}

/// TODO: implement this
bool
SatCrdsa::IsDamaAvailable ()
{
  NS_LOG_FUNCTION (this);

  bool isDamaAvailable = false;

  NS_LOG_INFO ("SatCrdsa::IsDamaAvailable: " << isDamaAvailable);

  return isDamaAvailable;
}

/// TODO: implement this
bool
SatCrdsa::AreBuffersEmpty ()
{
  NS_LOG_FUNCTION (this);

  bool areBuffersEmpty = true;

  NS_LOG_INFO ("SatCrdsa::AreBuffersEmpty: " << areBuffersEmpty);

  return areBuffersEmpty;
}

/// TODO: implement this
void
SatCrdsa::UpdateMaximumRateLimitationParameters ()
{
  NS_LOG_FUNCTION (this);

  NS_LOG_INFO ("SatCrdsa::UpdateMaximumRateLimitationParameters - Updating parameters");
}

/// TODO: implement this
void
SatCrdsa::CheckMaximumRateLimitations ()
{
  NS_LOG_FUNCTION (this);

  NS_LOG_INFO ("SatCrdsa::CheckMaximumRateLimitations - Checking maximum rate limitations");
}

void
SatCrdsa::SetBackoffTimer ()
{
  NS_LOG_FUNCTION (this);

  m_backoffReleaseTime = Now ().GetSeconds () + m_backoffPeriodLength;

  NS_LOG_INFO ("SatCrdsa::SetBackoffTimer - Setting backoff timer");
}

std::set<uint32_t>
SatCrdsa::PrepareToTransmit ()
{
  std::set<uint32_t> txOpportunities;

  NS_LOG_INFO ("SatCrdsa::PrepareToTransmit - Preparing for transmission...");

  CheckMaximumRateLimitations ();

  txOpportunities = RandomizeTxOpportunities ();

  if (AreBuffersEmpty ())
    {
      m_newData = true;
    }
  return txOpportunities;
}

std::set<uint32_t>
SatCrdsa::DoCrdsa ()
{
  NS_LOG_FUNCTION (this);

  /// TODO: what to return in the case CRDSA is not used?
  std::set<uint32_t> txOpportunities;

  NS_LOG_INFO ("-------------------------------------");
  NS_LOG_INFO ("------ Running CRDSA algorithm ------");
  NS_LOG_INFO ("-------------------------------------");

  PrintVariables ();

  NS_LOG_INFO ("-------------------------------------");

  NS_LOG_INFO ("SatCrdsa::DoCrdsa - Checking backoff period status...");

  if (IsBackoffPeriodOver ())
    {
      NS_LOG_INFO ("SatCrdsa::DoCrdsa - Backoff period over, checking DAMA status...");

      if (!IsDamaAvailable ())
        {
          NS_LOG_INFO ("SatCrdsa::DoCrdsa - No DAMA, checking buffer status...");

          if (m_newData)
            {
              m_newData = false;

              NS_LOG_INFO ("SatCrdsa::DoCrdsa - Evaluating backoff...");

              if (DoBackoff ())
                {
                  SetBackoffTimer ();
                }
              else
                {
                  txOpportunities = PrepareToTransmit ();
                }
            }
          else
            {
              txOpportunities = PrepareToTransmit ();
            }
        }
    }

  UpdateMaximumRateLimitationParameters ();

  return txOpportunities;
}


std::set<uint32_t>
SatCrdsa::RandomizeTxOpportunities ()
{
  NS_LOG_FUNCTION (this);

  std::set<uint32_t> txOpportunities;
  std::pair<std::set<uint32_t>::iterator,bool> result;

  NS_LOG_INFO ("SatCrdsa::DoCrdsa - Randomizing TX opportunities");

  while (txOpportunities.size () < m_setSize)
    {
      uint32_t slot = m_uniformVariable->GetInteger (m_min, m_max);

      result = txOpportunities.insert (slot);

      NS_LOG_INFO ("SatCrdsa::DoCrdsa - Insert successful: " << result.second << " for TX opportunity slot: " << (*result.first));
    }

  NS_LOG_INFO ("SatCrdsa::DoCrdsa - Randomizing done");

  return txOpportunities;
}

void
SatCrdsa::PrintVariables ()
{
  NS_LOG_INFO ("Simulation time: " << Now ().GetSeconds () << " seconds");
  NS_LOG_INFO ("Backoff period release time: " << m_backoffReleaseTime << " seconds");
  NS_LOG_INFO ("Backoff period length: " << m_backoffPeriodLength << " seconds");
  NS_LOG_INFO ("Backoff probability: " << m_backoffProbability * 100 << " %");
  NS_LOG_INFO ("New data status: " << m_newData);
  NS_LOG_INFO ("Slot randomization range: " << m_min << " to " << m_max);
  NS_LOG_INFO ("Number of randomized TX opportunities: " << m_setSize);
}

} // namespace ns3