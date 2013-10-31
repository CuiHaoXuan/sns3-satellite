/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Magister Solutions Ltd.
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
#ifndef SATELLITE_MARKOV_MODEL_H
#define SATELLITE_MARKOV_MODEL_H

#include "ns3/object.h"
#include "ns3/log.h"

namespace ns3 {

/**
 * \ingroup satellite
 *
 * \brief Markov model
 */
class SatMarkovModel : public Object
{
public:

  /**
   * \brief NS-3 function for type id
   * \return type id
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Constructor
   */
  SatMarkovModel ();

  /**
   * \brief Constructor
   * \param numOfStates number of states
   */
  SatMarkovModel (uint32_t numOfStates);

  /**
   * \brief Destructor
   */
  ~SatMarkovModel ();

  /**
   * \brief Function for setting the probability values
   * \param from start state
   * \param to end state
   * \param probability change probability
   */
  void SetProbability (uint32_t from,
                       uint32_t to,
                       double probability);

  /**
   * \brief Function for evaluating the state change
   * \return new state
   */
  uint32_t DoTransition ();

  /**
   * \brief Function for returning the current state
   * \return current state
   */
  uint32_t GetState () const;

  /**
   * \brief Function for setting the state
   * \param newState new state
   */
  void SetState (uint32_t newState);

private:

  /**
   * \brief Markov state change probabilities
   */
  double* m_probabilities;

  /**
   * \brief Number of states
   */
  uint32_t m_numOfStates;

  /**
   * \brief Current state
   */
  uint32_t m_currentState;

};

} // namespace ns3

#endif /* SATELLITE_MARKOV_MODEL_H */
