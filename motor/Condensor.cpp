// Copyright [2015] Ruud Cools

#include <macroHeader.h>
#include <StateSequence.h>
#include <SequenceVector.h>
#include <Condensor.h>

void Condensor::prepareSequences(
    CompileSet* i_compileSet) {
  int averageSpeed(0);

  // iterator represeting where we left off
  PinStateSequenceVector::iterator currentSequence =
      i_compileSet->currentSequence;
  int numberOfRecompiledSequences = 0;

  // do this at least untill the end of the vector
  while (currentSequence != i_compileSet->sequenceVector->end()) {
    LOG_DEBUG("Start prepareSequence while loop");
    // check if current sequence has only 1 repitition
    if (currentSequence->getNumberOfRepetitions() != 1) {
      ++i_compileSet->currentSequence, ++currentSequence;
      LOG_DEBUG("Continue to the next sequence" <<
                " this one has multiple repititions");
      continue;
    }

    // if it is the first one
    // if possible set the m_preIntegerSequence
    if (currentSequence != i_compileSet->sequenceVector->begin()) {
      if ((currentSequence-1)->getNumberOfRepetitions() != 0) {
        LOG_DEBUG("Setting preIntegerSequence, its size is: " <<
                  i_compileSet->preIntegerSequence.size());
        i_compileSet->preIntegerSequence =
            (currentSequence-1)->getIntegerSequence();
      } else {
        LOG_DEBUG("Cannot add a preIntegerSequence, due to rep = 0");
      }
    }

    // creation of a pin state vector which will be used
    PinStateVector pinStateVector;
    LOG_DEBUG("Start gather sequences");
    while (currentSequence != i_compileSet->sequenceVector->end() &&
           currentSequence->getNumberOfRepetitions() == 1) {
      LOG_DEBUG("Add current integer sequence to the rest.");
      std::vector<int> thisIntegerSequence =
          currentSequence->getIntegerSequence();

      i_compileSet->integerSequence.insert(
          i_compileSet->integerSequence.end(),
          thisIntegerSequence.begin(),
          thisIntegerSequence.end());

      LOG_DEBUG("Add current state sequences to the rest.");
      // add its pin state to the whole list
      pinStateVector.insert(pinStateVector.end(),
                            currentSequence->getPinStateVector().begin(),
                            currentSequence->getPinStateVector().end());
      LOG_DEBUG("This speed is: " << currentSequence->getSpeed());
      // calculating the average speed in a sec
      averageSpeed += currentSequence->getSpeed();

      LOG_DEBUG("cleaning up the current sequence.");
      currentSequence->setNumberOfRepetitions(0);
      currentSequence->setPinStateVector(PinStateVector());
      ++currentSequence;
      ++numberOfRecompiledSequences;
    }

    // the new representing sequence is set now
    i_compileSet->stateSequence.setPinStateVector(pinStateVector);
    i_compileSet->stateSequence.setNumberOfRepetitions(1);

    // Setting the average speed now
    i_compileSet->stateSequence.setSpeed(
        averageSpeed/numberOfRecompiledSequences);

    // If it is possible fetch the following sequence
    if (currentSequence != i_compileSet->sequenceVector->end()) {
      if (currentSequence->getNumberOfRepetitions() != 0) {
        i_compileSet->postIntegerSequence =
            currentSequence->getIntegerSequence();
      } else {
        LOG_DEBUG("Cannot add a postIntegerSequence, due to rep = 0");
      }
    }

    // break the while loop because we set everything allright for 1 shot
    LOG_DEBUG("This is break!");  // Break out the first while loop
    break;
    }
  // Setting the end sequence indicating the ++sequence we will be optimising
  i_compileSet->endSequence = currentSequence;
}


bool Condensor::handleForwardCorrispondingSequences(CompileSet* i_compileSet) {
  // checking that there is a pre intger sequence
  if (i_compileSet->preIntegerSequence.size() == 0) {
    LOG_DEBUG("pre integer sequence size is 0");
    return false;
  }

  // checking if the current integer sequence list can hold a pre sequence list
  if (i_compileSet->preIntegerSequence.size() >
      i_compileSet->integerSequence.size()) {
    LOG_DEBUG("pre integer sequence size" <<
              "is bigger then current integer sequence");
    return false;
  }

  // setting variables
  int numberOfFinds = 0;
  std::vector<int>::size_type sizeOfPre =
      i_compileSet->preIntegerSequence.size();

  auto begin = i_compileSet->integerSequence.begin() - sizeOfPre;
  auto end = i_compileSet->integerSequence.begin();

  // while we are finding presequence in the integer sequence
  while (std::search(begin + sizeOfPre,
                     end + sizeOfPre,
                     i_compileSet->preIntegerSequence.begin(),
                     i_compileSet->preIntegerSequence.end()) ==
         begin + sizeOfPre) {
    // set the begin 1 presequence further
    begin += sizeOfPre;
    // set the end 1 presequence further
    end += sizeOfPre;
    // indicate that we have found 1
    ++numberOfFinds;
    // check if we are still within bounds in our next iteration
    if (i_compileSet->integerSequence.size() <
        (numberOfFinds + 1) * i_compileSet->preIntegerSequence.size()) {
      break;
    }
  }

  // previous state is equal to the first bit of the current state.
  // remove the first bit, and add it to the prev sequence
  if (numberOfFinds == 0) {
    LOG_DEBUG("The begin of the interger list is not equial" <<
              " to that of list before it.");
    return false;
  }

  if (end == i_compileSet->integerSequence.end()) {
    // The whole current sequence is equal
    LOG_ERROR("This should be optimised somewhere else!");
  }

  // Increase the previous sequence numberOfRepititions with numberOfFinds
  auto previousSequence = (i_compileSet->currentSequence-1);
  previousSequence->setNumberOfRepetitions(
      previousSequence->getNumberOfRepetitions() + numberOfFinds);

  // remove the integer states fromt he current integer sequence list
  i_compileSet->integerSequence.erase(
      i_compileSet->integerSequence.begin(),
      end);

  PinStateVector pinStateVector =
      i_compileSet->stateSequence.getPinStateVector();
  // remove the states from the stateVector
  pinStateVector.erase(
      pinStateVector.begin(),
      pinStateVector.begin() + (end - begin) * numberOfFinds);

  // set this pin vector to the current state sequence
  i_compileSet->stateSequence.setPinStateVector(pinStateVector);

  return true;
}


bool Condensor::handleBackwardCorrispondingSequences(
    CompileSet* i_compileSet) {
  // checking precondition
  if (i_compileSet->postIntegerSequence.size() == 0) {
    LOG_DEBUG("post interger Sequence size is 0");
    return false;
  }

  if (i_compileSet->endSequence ==
      i_compileSet->sequenceVector->end()) {
    LOG_DEBUG("End sequence is equal to the end of the state sequence vector");
    return false;
  }

  // checking if the current integer sequence list can hold a pre sequence list
  if (i_compileSet->postIntegerSequence.size() >
      i_compileSet->integerSequence.size()) {
    LOG_DEBUG("post integer sequence size" <<
              "is bigger then current integer sequence");
    return false;
  }

  // setting variables
  int numberOfFinds = 0;
  std::vector<int>::size_type postIntSize =
      i_compileSet->postIntegerSequence.size();

  auto begin = i_compileSet->integerSequence.end();
  auto end = i_compileSet->integerSequence.end() +
      postIntSize;

  // while we are finding presequence in the integer sequence
  while (std::search(begin - postIntSize,
                     end - postIntSize,
                     i_compileSet->postIntegerSequence.begin(),
                     i_compileSet->postIntegerSequence.end()) ==
         begin - postIntSize) {
    // set the begin 1 presequence further
    begin -= postIntSize;
    // set the end 1 presequence further
    end -= postIntSize;
    // indicate that we have found 1
    ++numberOfFinds;
    // check if we are still within bounds in our next iteration
    if (i_compileSet->integerSequence.size() <
        (numberOfFinds + 1) * i_compileSet->postIntegerSequence.size()) {
      break;
    }
  }

  // previous state is equal to the first bit of the current state.
  // remove the first bit, and add it to the prev sequence
  if (numberOfFinds == 0) {
    LOG_DEBUG("Did not find any post sequences");
    return false;
  } else {
    LOG_DEBUG("Found " << numberOfFinds << " corrisponding sequences!");
  }

  if (begin == i_compileSet->integerSequence.begin()) {
    // The whole current sequence is equal
    LOG_ERROR("This should be optimised somewhere else!");
  }

  LOG_DEBUG("Old number of reps: " <<
            i_compileSet->endSequence->getNumberOfRepetitions());

  // Increase the previous sequence numberOfRepititions with numberOfFinds
  i_compileSet->endSequence->setNumberOfRepetitions(
      i_compileSet->endSequence->getNumberOfRepetitions() + numberOfFinds);
  LOG_DEBUG("New number of reps: " <<
            i_compileSet->endSequence->getNumberOfRepetitions());

  // remove the integer states fromt he current integer sequence list
  i_compileSet->integerSequence.erase(begin,
                          end);

  PinStateVector pinStateVector =
      i_compileSet->stateSequence.getPinStateVector();

  // remove the states from the stateVector
  pinStateVector.erase(pinStateVector.end() - (end - begin)*numberOfFinds,
                       pinStateVector.end());

  // set this pin vector to the current state sequence
  i_compileSet->stateSequence.setPinStateVector(pinStateVector);

  return true;
}


bool Condensor::recompile(
    CompileSet* i_compileSet) {
  // if the integerSequence does not have any values, return
  if (i_compileSet->integerSequence.size() == 0)
    return false;

  // finding recurences in the vector
  Condensor::RecurrenceVector recurenceVector =
      findRecurrence(i_compileSet->integerSequence);

  // finding the recurence that has the most effect
  Condensor::RecurrenceVector::const_iterator maximumEffectRecurence =
      findMaximumEffectRecurence(recurenceVector,
                                 i_compileSet->integerSequence);
  bool hasAddedEndSequence = false;
  // If the max effect is pointing to the end, no luck
  if (maximumEffectRecurence != recurenceVector.end()) {
    /*
      finaly recompile the state sequence to something nice
      if it makes sense
    */
    hasAddedEndSequence = handleRecurence(maximumEffectRecurence,
                                          i_compileSet);
  }

  LOG_DEBUG("Yay done");
  return hasAddedEndSequence;
}


Condensor::RecurrenceVector Condensor::findRecurrence(
    const std::vector<int>& i_vector) {
  // Vector which stores the bestRecurrence of all the found recurrences
  Condensor::RecurrenceVector resultVector;

  // position which is currently investigated
  std::vector<int>::const_iterator currentPosition;

  // number of recurrence found of the current sequence
  int finds(1);

  // Maximum size of sequences
  unsigned int maxSequenceSize(0);

  // for each possible offset
  for (unsigned int offset = 0;
       offset <= i_vector.size() - 1;
       ++offset) {
    // in the remainer at least 2 sequence should fit
    maxSequenceSize = (i_vector.size() - offset) / 2;
    LOG_DEBUG("Max recurence size : " << maxSequenceSize);
    // for each possible sequences size that will fit
    for (unsigned int sequenceSize = 1;
         sequenceSize <= maxSequenceSize;
         ++sequenceSize) {
      LOG_DEBUG("Testing offset: " << offset <<
                " and sequence size: " << sequenceSize);

      currentPosition = i_vector.begin() + offset;
      finds = 1;
      // if the next block is equal to the first
      while (std::search(currentPosition + sequenceSize,
                         currentPosition + 2 * sequenceSize,
                         currentPosition,
                         currentPosition + sequenceSize) ==
             currentPosition + sequenceSize) {
        // add a find and update the current position
        ++finds;
        currentPosition += sequenceSize;
        LOG_DEBUG("Found a re-occurence");

        for (auto i = currentPosition;
             i != currentPosition + sequenceSize;
             ++i) {
          LOG_DEBUG(*i << ", ");
        }

        // check if we are still within bounds
        if (std::distance(i_vector.begin(),
                          currentPosition + 2 * sequenceSize) >
            static_cast<int>(i_vector.size())) {
          break;
        } else {
          LOG_DEBUG("Vector size: " << i_vector.size());
          LOG_DEBUG("End position " <<
                    std::distance(i_vector.begin(),
                                  currentPosition + sequenceSize));
        }
      }

      if (finds > 1) {
      LOG_DEBUG("Adding re to the staple");
      resultVector.push_back(std::make_tuple(
          // begin of recurence sequence
          i_vector.begin() + offset,
           // end of recuring sequence
          i_vector.begin() + offset + sequenceSize,
          // end of last seen sequence
          currentPosition + sequenceSize));
      }
    }
  }
  return resultVector;
}


Condensor::RecurrenceVector::const_iterator
    Condensor::findMaximumEffectRecurence(
        const Condensor::RecurrenceVector& i_recurrenceVector,
        const std::vector<int>& i_integerSequence) {
  Condensor::RecurrenceVector::const_iterator bestRecurrence = i_recurrenceVector.end();

  int currentScore(0),
      maximumScore(0),
      lengthOfSequence(0),
      totalLength(0);

  for (auto recurrence = i_recurrenceVector.begin();
       recurrence != i_recurrenceVector.end();
       ++recurrence
       ) {
    currentScore = 0;

    if (std::get<0>(*recurrence) != i_integerSequence.begin()) {
      LOG_DEBUG("minus " << Condensor::statePenalty <<
                " because we need to make a sequence at the begin");
      currentScore -= Condensor::statePenalty;
    }

    if (std::get<2>(*recurrence) != i_integerSequence.end()) {
      LOG_DEBUG("minus " << Condensor::statePenalty <<
                " because we need to make a sequence at the end");
      currentScore -= Condensor::statePenalty;
    }

    lengthOfSequence = std::distance(std::get<0>(*recurrence),
                                     std::get<1>(*recurrence));

    LOG_DEBUG("Length of the sequence is: " << lengthOfSequence);
    totalLength = std::distance(std::get<0>(*recurrence),
                                std::get<2>(*recurrence));
    
    LOG_DEBUG("Total length it will replace is: " << totalLength);
    currentScore += totalLength - lengthOfSequence;

    LOG_DEBUG("A sequence has a score of: " << currentScore);
    if (currentScore >= maximumScore) {
      LOG_DEBUG("Setting current recurence as the best recurrence!");
      maximumScore = currentScore;
      bestRecurrence = recurrence;
    }
  }
  return bestRecurrence;
}


bool Condensor::handleRecurence(
    const Condensor::RecurrenceVector::const_iterator& maximumEffectRecurence,
    CompileSet* i_compileSet) {
  // if the recuring sequence does not start at the beginning
  bool hasAddedEndSequence = false;
  if (std::get<0>(*maximumEffectRecurence) !=
      i_compileSet->integerSequence.begin()) {
    LOG_DEBUG("Creating a start sequence.");
    StateSequence newSequence;
    splitSequence(i_compileSet,
                  &newSequence,
                  std::get<0>(*maximumEffectRecurence),
                  /*erase begin = */ false);
    // adding sequence to the vector
    addSequenceToVector(i_compileSet,
                        std::distance(i_compileSet->currentSequence,
                                      i_compileSet->endSequence),
                        &newSequence);
  }

  if (std::get<2>(*maximumEffectRecurence) !=
      i_compileSet->integerSequence.end()) {
    LOG_DEBUG("Creating a end sequence.");
    StateSequence newSequence;
    splitSequence(i_compileSet,
                  &newSequence,
                  std::get<2>(*maximumEffectRecurence),
                  /*erase_begin = */ true);
    // adding sequence to the vector
    addSequenceToVector(i_compileSet,
                        std::distance(i_compileSet->currentSequence,
                                      i_compileSet->endSequence),
                        &newSequence);
    hasAddedEndSequence = true;
  }
  
  // Update current state after adding pre and post state if necessary
  updateCurrentState(i_compileSet,
                     *maximumEffectRecurence);
  return hasAddedEndSequence;
}


void Condensor::splitSequence(CompileSet* i_compileSet,
                              StateSequence* i_newSequence,
                              const std::vector<int>::const_iterator& i_split,
                              bool i_eraseStart) {
  PinStateVector pinStateVector =
      i_compileSet->stateSequence.getPinStateVector();

  LOG_DEBUG("Erase bit of the state vector");
  if (i_eraseStart) {
    // remove the states from the stateVector
    pinStateVector.erase(pinStateVector.begin(),
                         pinStateVector.end() -
                         (i_compileSet->integerSequence.end() - i_split));
  } else {
    pinStateVector.erase(
        pinStateVector.begin() +
        (i_split - i_compileSet->integerSequence.begin()),
        pinStateVector.end());
  }
  // set this pin vector to the current state sequence
  i_newSequence->setPinStateVector(pinStateVector);
  i_newSequence->setNumberOfRepetitions(1);
}


void Condensor::addSequenceToVector(CompileSet* i_compileSet,
                                    const int& insertionPosition,
                                    StateSequence* newSequence) {
    int distance = std::distance(i_compileSet->currentSequence,
                                 i_compileSet->endSequence);

    LOG_DEBUG("Distance between iterators: " <<
              distance);
#if defined(DEBUG) || defined(DEBUG_FILE)
    int positionInVector = std::distance(
        i_compileSet->sequenceVector->begin(),
        i_compileSet->currentSequence + insertionPosition);
#endif
    LOG_DEBUG("Insert a sequence to the vector @ " << positionInVector);
    i_compileSet->currentSequence =
        i_compileSet->sequenceVector->insert(
            i_compileSet->currentSequence + insertionPosition, *newSequence)
        - insertionPosition;

    // relocation make iterators invalid, reset them
    i_compileSet->endSequence = i_compileSet->currentSequence + distance + 1;
}


void Condensor::updateCurrentState(
    CompileSet* i_compileSet,
    const RecurrenceResult& maximumEffectRecurence) {
  // reload the pin state vector
  PinStateVector pinStateVector =
      i_compileSet->stateSequence.getPinStateVector();

  auto begin =
      pinStateVector.begin() +
      (std::get<0>(maximumEffectRecurence) -
       i_compileSet->integerSequence.begin());

  auto end = pinStateVector.begin() +
      (std::get<1>(maximumEffectRecurence) -
       i_compileSet->integerSequence.begin());

  PinStateVector newPinStateVector(begin, end);
  i_compileSet->stateSequence.setPinStateVector(newPinStateVector);
  int totalDistance = std::distance(std::get<2>(maximumEffectRecurence),
                                    std::get<0>(maximumEffectRecurence));

  int sequenceDistance = std::distance(end, begin);
  i_compileSet->stateSequence.setNumberOfRepetitions(
      totalDistance / sequenceDistance);
}


void Condensor::cleanCompileSet(CompileSet* i_compileSet) {
  /*
    this functions cleans the compile set,
    so it will give good results for the next session!
  */
  std::vector<int>().swap(i_compileSet->integerSequence);
  std::vector<int>().swap(i_compileSet->postIntegerSequence);
  std::vector<int>().swap(i_compileSet->preIntegerSequence);
  i_compileSet->stateSequence = StateSequence();
}


bool Condensor::recompileSequenceVector(
    PinStateSequenceVector* i_sequenceVector) {
  LOG_DEBUG("Recompile condense!");
  CompileSet  compileSet;
  compileSet.currentSequence = i_sequenceVector->begin();
  compileSet.endSequence = i_sequenceVector->end();
  compileSet.sequenceVector = i_sequenceVector;
  bool hasCondensed = false;
  PinStateSequenceVector::iterator endOfCurrentSequence;
  while (compileSet.currentSequence != i_sequenceVector->end()) {
    // Look for sequence with 1 repitition
    prepareSequences(&compileSet);

    // if current sequence is end, did not find
    if (compileSet.currentSequence == i_sequenceVector->end()) {
      continue;
    } else {
      LOG_DEBUG("Start internal recompile");
    }
    // Notify that we recompiled the sequence vector
    hasCondensed = true;
    handleForwardCorrispondingSequences(&compileSet);
    handleBackwardCorrispondingSequences(&compileSet);

    bool hasInsertedAtEnd = recompile(&compileSet);

    addSequenceToVector(
        &compileSet,
        std::distance(compileSet.currentSequence,
            compileSet.endSequence) - hasInsertedAtEnd,
        &compileSet.stateSequence);
    // clean up
    cleanCompileSet(&compileSet);
    // set the current to the end, and start over again
    compileSet.currentSequence = compileSet.endSequence;
  }
  LOG_DEBUG("Finished recompile sequence");
  return hasCondensed;
}


bool Condensor::internalCondense(PinStateSequenceVector* i_sequenceVector) {
  LOG_DEBUG("Internal condensing!");
  bool internalyCondesed = false;
  for (auto sequenceIterator = i_sequenceVector->begin();
       sequenceIterator != i_sequenceVector->end();
       sequenceIterator++) {
    internalyCondesed |= sequenceIterator->condenseSequence();
  }
  LOG_DEBUG("End of internal condensing");
  return internalyCondesed;
}


bool Condensor::mergeCondense(PinStateSequenceVector* i_sequenceVector,
                              const bool& i_removeFromVector /*= false*/) {
  bool hasCondensed = false;
  LOG_DEBUG("Condensing vector by merging state sequences!");
  // merge all sequential sequence with 1 repetition.

  auto sequence = i_sequenceVector->begin();
  auto nextSequence = sequence+1;

  while ((nextSequence != i_sequenceVector->end() &&
          sequence != i_sequenceVector->end())) {
    LOG_DEBUG("attempting merge condense!");
    // add the previous sequence to the next
    if (nextSequence->addToSequence(*sequence)) {
      // it has worked at least once
      hasCondensed |= true;
      // if it worked determine how to handle the old one
      if (i_removeFromVector) {
        // remove it from the vector, slow but lowers memory
        // the returned sequence is next sequence
        sequence = i_sequenceVector->erase(sequence);
      } else {
        // make it emtpy, slightly faster
        // bit tho
        // due to the fact that we need to clear the pinStateSequenceVector
        sequence->setNumberOfRepetitions(0);
        sequence->setPinStateVector(PinStateVector());
        // update sequence
        ++sequence;
      }
    } else {
      ++sequence;
    }
    // get the next sequence of the vector
    nextSequence = sequence+1;
  }
  LOG_DEBUG("End of merge condensing");
  return hasCondensed;
}
