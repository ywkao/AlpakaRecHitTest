#include <alpaka/alpaka.hpp>

#include "DataFormats/PortableTestObjects/interface/alpaka/TestDeviceCollection.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"
#include "HeterogeneousCore/AlpakaInterface/interface/traits.h"
#include "HeterogeneousCore/AlpakaInterface/interface/workdivision.h"

#include "TestAlgo.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE {

  using namespace cms::alpakatools;
  
  class TestAlgoKernel {
  public:
    template <typename TAcc, typename = std::enable_if_t<alpaka::isAccelerator<TAcc>>>
    ALPAKA_FN_ACC void operator()(TAcc const& acc,
                                  portabletest::TestDeviceRecHitCollection::View view,
                                  int32_t size, double xvalue) const {
      // global index of the thread within the grid
      const int32_t thread = alpaka::getIdx<alpaka::Grid,alpaka::Threads>(acc)[0u];

      //--- placeholder for calibration algorithms ---//
      // .
      // .
      // .
      // .
      // .
      //--- end of calibration algorithms ---//

      // make a strided loop over the kernel grid, covering up to "size" elements
      for (int32_t i : cms::alpakatools::elements_with_stride(acc, size)) {
        // event, detid, adc, adcm, adc_cm, toa, tot, triglatency
        view[i] = {0, 0, 3., 4., 5., 6., 7., 8.};
      }
    }
  };

  void TestAlgo::fill(Queue& queue, portabletest::TestDeviceRecHitCollection& collection, double xvalue) const {
    // use 64 items per group (this value is arbitrary, but it's a reasonable starting point)
    uint32_t items = 64;

    // use as many groups as needed to cover the whole problem
    uint32_t groups = divide_up_by(collection->metadata().size(), items);

    // map items to
    // - threads with a single element per thread on a GPU backend
    // - elements within a single thread on a CPU backend
    auto workDiv = make_workdiv<Acc1D>(groups, items);

    alpaka::exec<Acc1D>(queue, workDiv, TestAlgoKernel{}, collection.view(), collection->metadata().size(), xvalue);
  }

} // namespace ALPAKA_ACCELERATOR_NAMESPACE
