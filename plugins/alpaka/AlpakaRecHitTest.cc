#include "DataFormats/Portable/interface/Product.h"
#include "DataFormats/PortableTestObjects/interface/alpaka/TestDeviceCollection.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/Event.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/EventSetup.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/global/EDProducer.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"

#include "TestAlgo.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE {

  class AlpakaRecHitTest : public global::EDProducer<> {
  public:
    AlpakaRecHitTest(edm::ParameterSet const& config)
        : deviceToken_{produces()}, size_{config.getParameter<int32_t>("size")} {}

    void produce(edm::StreamID sid, device::Event& event, device::EventSetup const& setup) const override {
      // run the algorithm, potentially asynchronously
      portabletest::TestDeviceCollection deviceProduct{size_, event.queue()};

      // put the asynchronous product into the event without waiting
      algo_.fill(event.queue(), deviceProduct);
    }

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
      edm::ParameterSetDescription desc;
      desc.add<int32_t>("size");
      descriptions.addWithDefaultLabel(desc);
    }
  
  private:
    const device::EDPutToken<portabletest::TestDeviceCollection> deviceToken_;
    const int32_t size_;

    // implementation of the algorithm
    TestAlgo algo_;
  };

} // namespace ALPAKA_ACCELERATOR_NAMESPACE

//define this as a plug-in
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/MakerMacros.h"
DEFINE_FWK_ALPAKA_MODULE(AlpakaRecHitTest);
