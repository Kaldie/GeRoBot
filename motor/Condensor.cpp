// Copyright [2015] Ruud Cools

#include <macroHeader.h>
#include <StateSequence.h>
#include <SequenceVector.h>
#include <Condensor.h>

PinStateSequenceVector::iterator Condensor::prepareSequences(
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
      if ((currentSequence-1)->getNumberOfRepetitions() == 0) {
        continue;
      }

      LOG_DEBUG("Setting preIntegerSequence, its size is: " <<
                i_compileSet->preIntegerSequence.size());

      i_compileSet->preIntegerSequence =
          (currentSequence-1)->getIntegerSequence();
    }

    // creation of a pin state vector which will be used
    PinStateVector pinStateVector;
    LOG_DEBUG("Start gather sequence while loop");
    while (currentSequence != i_compileSet->sequenceVector->end() &&
           currentSequence->getNumberOfRepetitions() == 1) {
      LOG_DEBUG("insert into integer sequence");
      std::vector<int> thisIntegerSequence = currentSequence->getIntegerSequence();
      i_compileSet->integerSequence.insert(
          i_compileSet->integerSequence.end(),
          thisIntegerSequence.begin(),
          thisIntegerSequence.end());


      LOG_DEBUG("Insert into pin state vector");
      // add its pin state to the whole list
      pinStateVector.insert(pinStateVector.end(),
                            currentSequence->getPinStateVector().begin(),
                            currentSequence->getPinStateVector().end());

      // cleaning up the sequence
      currentSequence->setNumberOfRepetitions(0);
      currentSequence->setPinStateVector(PinStateVector());
      ++currentSequence;
      ++numberOfRecompiledSequences;

      // calculating the average speed in a sec
      averageSpeed+=currentSequence->getSpeed();
    }

    // the new representing sequence is set now
    i_compileSet->stateSequence.setPinStateVector(pinStateVector);
    i_compileSet->stateSequence.setNumberOfRepetitions(1);

    // Setting the average speed now
    i_compileSet->stateSequence.setSpeed(
        averageSpeed/numberOfRecompiledSequences);

    // If it is possible fetch the following sequence
    if (currentSequence != i_compileSet->sequenceVector->end()) {
      i_compileSet->postIntegerSequence =
          currentSequence->getIntegerSequence();
    }

    // break the while loop because we set everything allright for 1 shot
    LOG_DEBUG("This is break!");
    break;  // end while loop
  }
  return currentSequence;
}


bool Condensor::handleForwardCorrispondingSequences(
    CompileSet* i_compileSet) {
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
  if ( i_compileSet->sequenceVector->begin()== (i_compileSet->currentSequence))
    LOG_ERROR("error");
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
    CompileSet* i_compileSet,
    PinStateSequenceVector::iterator i_endSequence) {
  // checking precondition
  if (i_compileSet->postIntegerSequence.size() == 0) {
    LOG_DEBUG("post interger Sequence size is 0");
    return false;
  }

  if (i_endSequence == i_compileSet->sequenceVector->end()) {
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
        (numberOfFinds + 1) * i_compileSet->preIntegerSequence.size()) {
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
            i_endSequence->getNumberOfRepetitions());
  // Increase the previous sequence numberOfRepititions with numberOfFinds
  i_endSequence->setNumberOfRepetitions(
      i_endSequence->getNumberOfRepetitions() + numberOfFinds);
  LOG_DEBUG("New number of reps: " <<
            i_endSequence->getNumberOfRepetitions());

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
    CompileSet* i_compileSet,
    PinStateSequenceVector::iterator* i_endSequence) {
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

  /*
    finaly recompile the state sequence to something nice
   if it makes sense
  */
  bool hasRecompiled = handleRecurence(recurenceVector,
                                       maximumEffectRecurence,
                                       i_compileSet);
                                       
  LOG_DEBUG("Adding a new sequence!");
  *i_endSequence = i_compileSet->sequenceVector->insert(
      *i_endSequence, i_compileSet->stateSequence) + 1;
  return true;
}


Condensor::RecurrenceVector Condensor::findRecurrence(
    const std::vector<int>& i_vector) {
  // Vector which stores the result of all the found recurrences
  Condensor::RecurrenceVector resultVector;

  // position which is currently investigated
  auto currentPosition = i_vector.begin();

  // number of recurrence found of the current sequence
  int finds(1);

  // Maximum size of sequences
  unsigned int maxSequenceSize(0);

  // for each possible offset
  for (unsigned int offset = 0;
       offset <= i_vector.size() - 1;
       ++offset) {
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

        if (std::distance(i_vector.begin(),
                          currentPosition + sequenceSize) >=
            static_cast<int>(i_vector.size())) {
          continue;
        }
      }

      if (finds > 1) {
      LOG_DEBUG("Adding result to the staple");
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
        const Condensor::RecurrenceVector i_recurrenceVector,
        const std::vector<int>& i_integerSequence) {
  Condensor::RecurrenceVector::const_iterator result = i_recurrenceVector.end();

  int currentResult(0),
      maximumResult(0),
      lengthOfSequence(0),
      totalLength(0);

  for (auto recurrence = i_recurrenceVector.begin();
       recurrence != i_recurrenceVector.end();
       ++recurrence
       ) {
    currentResult = 0;

    if (std::get<0>(*recurrence) != i_integerSequence.begin()) {
      LOG_DEBUG("minus " << Condensor::statePenalty <<
                " because we need to make a sequence at the begin");
      currentResult -= Condensor::statePenalty;
    }

    if (std::get<2>(*recurrence) != i_integerSequence.end()) {
            LOG_DEBUG("minus " << Condensor::statePenalty <<
                      " because we need to make a sequence at the end");
      currentResult -= Condensor::statePenalty;
    }

    lengthOfSequence = std::distance(std::get<0>(*recurrence),
                                     std::get<1>(*recurrence));

    LOG_DEBUG("Length of the sequence is: " << lengthOfSequence);
    totalLength = std::distance(std::get<0>(*recurrence),
                                std::get<2>(*recurrence));
    
    LOG_DEBUG("Total length it will replace is: " << totalLength);
    currentResult += totalLength - lengthOfSequence;

    LOG_DEBUG("A sequence has a score of: " << currentResult);
    if (currentResult > maximumResult) {
      maximumResult = currentResult;
      result = recurrence;
    }
  }
  return result;
}


bool Condensor::handleRecurence(
    const Condensor::RecurrenceVector& recurenceVector,
    const Condensor::RecurrenceVector::const_iterator& maximumEffectRecurence,
    CompileSet* i_compileSet) {
  // If the max effect is pointing to the end, no luck
  if (maximumEffectRecurence == recurenceVector.end())
    return false;

  StateSequence newSequence;

  // if the recuring sequence does not start at the beginning
  if (std::get<0>(*maximumEffectRecurence) !=
      i_compileSet->integerSequence.begin()) {
    LOG_DEBUG("Creating a start sequence.");
    newSequence = StateSequence();
    PinStateVector pinStateVector =
        i_compileSet->stateSequence.getPinStateVector();

    // remove the states from the stateVector
    pinStateVector.erase(
        pinStateVector.begin() +
        (std::get<0>(*maximumEffectRecurence) -
         i_compileSet->integerSequence.begin()),
        pinStateVector.end());

    // set this pin vector to the current state sequence
    newSequence.setPinStateVector(pinStateVector);
    newSequence.setNumberOfRepetitions(1);

    // insert the new sequence in the vector
    i_compileSet->currentSequence =
        i_compileSet->sequenceVector->insert(
            i_compileSet->currentSequence,
            newSequence);

    // reload the pin state vector
    pinStateVector =
        i_compileSet->stateSequence.getPinStateVector();

    // updating the replacement state sequence
    pinStateVector.erase(pinStateVector.begin(),
                         pinStateVector.begin() +
                         (std::get<0>(*maximumEffectRecurence) -
                          i_compileSet->integerSequence.begin()));
    i_compileSet->stateSequence.setPinStateVector(pinStateVector);
  }

  if (std::get<2>(*maximumEffectRecurence) !=
      i_compileSet->integerSequence.end()) {
    LOG_DEBUG("Creating a end sequence.");
    newSequence = StateSequence();
    PinStateVector pinStateVector =
        i_compileSet->stateSequence.getPinStateVector();
    // remove the states from the stateVector
    pinStateVector.erase(pinStateVector.begin(),
                         pinStateVector.end() -
                         (i_compileSet->integerSequence.end() -
                          std::get<2>(*maximumEffectRecurence)));
    
    // set this pin vector to the current state sequence
    newSequence.setPinStateVector(pinStateVector);
    newSequence.setNumberOfRepetitions(1);

    // insert the new sequence in the vector
    i_compileSet->currentSequence =
        i_compileSet->sequenceVector->insert(
            i_compileSet->currentSequence,
            newSequence) -1;

    // reload the pin state vector
    pinStateVector =
        i_compileSet->stateSequence.getPinStateVector();

    // updating the replacement state sequence
    pinStateVector.erase(pinStateVector.end() -
                         (i_compileSet->integerSequence.end() -
                          std::get<2>(*maximumEffectRecurence)),
                         pinStateVector.end());
    i_compileSet->stateSequence.setPinStateVector(pinStateVector);
  }

  // clean up
  return true;
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
  compileSet.sequenceVector = i_sequenceVector;
  bool hasCondensed = false;
  PinStateSequenceVector::iterator endOfCurrentSequence;
  while (compileSet.currentSequence != i_sequenceVector->end()) {
    endOfCurrentSequence = prepareSequences(&compileSet);
    hasCondensed |= handleForwardCorrispondingSequences(&compileSet);
    hasCondensed |= handleBackwardCorrispondingSequences(&compileSet,
                                                         endOfCurrentSequence);
    hasCondensed |= recompile(&compileSet,
                              &endOfCurrentSequence);

    cleanCompileSet(&compileSet);

    compileSet.currentSequence = endOfCurrentSequence;
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
