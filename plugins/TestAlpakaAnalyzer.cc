 #include "DataFormats/PortableTestObjects/interface/TestHostCollection.h"
 #include "FWCore/Framework/interface/Event.h"
 #include "FWCore/Framework/interface/EventSetup.h"
 #include "FWCore/Framework/interface/Frameworkfwd.h"
 #include "FWCore/Framework/interface/stream/EDAnalyzer.h"
 #include "FWCore/MessageLogger/interface/MessageLogger.h"
 #include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
 #include "FWCore/ParameterSet/interface/ParameterSet.h"
 #include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
 #include "FWCore/Utilities/interface/EDGetToken.h"
 #include "FWCore/Utilities/interface/InputTag.h"

class TestAlpakaAnalyzer : public edm::stream::EDAnalyzer {
public:
  TestAlpakaAnalyzer(edm::ParameterSet const& config)
      : source_{config.getParameter<edm::InputTag>("source")},
        token_{consumes(source_)},
        expectSize_(config.getParameter<int>("expectSize")) {}

  void analyze(edm::Event const& event, edm::EventSetup const&) override {
    printf(">>>>> [INFO] expectSize_ = %d\n", expectSize_);
    portabletest::TestHostCollection const& product = event.get(token_);
    auto const& view = product.const_view();
    if (expectSize_ >= 0 and expectSize_ != view.metadata().size()) {
      throw cms::Exception("Assert") << "Expected input collection size " << expectSize_ << ", got " << view.metadata().size();
    }

    //...
  }

private:
  const edm::InputTag source_;
  const edm::EDGetTokenT<portabletest::TestHostCollection> token_;
  const int expectSize_;
};

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TestAlpakaAnalyzer);
