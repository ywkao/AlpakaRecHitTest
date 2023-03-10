#ifndef __TESTALGO_H__
#define __TESTALGO_H__

#include "DataFormats/PortableTestObjects/interface/alpaka/TestDeviceCollection.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE {

  class TestAlgo {
  public:
    void fill(Queue& queue, portabletest::TestDeviceCollection& collection, double xvalue = 0.) const;
  };

} // namespace ALPAKA_ACCELERATOR_NAMESPACE

#endif // __TESTALGO_H__
