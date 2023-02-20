import FWCore.ParameterSet.Config as cms

process = cms.Process('Writer')

process.source = cms.Source('EmptySource')

process.load('Configuration.StandardSequences.Accelerators_cff')
process.load('HeterogeneousCore.AlpakaCore.ProcessAcceleratorAlpaka_cfi')

# run the producer explicitly on the cpu
process.testProducerSerial = cms.EDProducer('alpaka_serial_sync::AlpakaRecHitTest',
    size = cms.int32(42)
)

# analyse the product
process.testAnalyzerSerial = cms.EDAnalyzer('TestAlpakaAnalyzer',
    source = cms.InputTag('testProducerSerial')
)

# write the product to a 'test.root' file
process.output = cms.OutputModule('PoolOutputModule',
    fileName = cms.untracked.string('output_test.root'),
    outputCommands = cms.untracked.vstring(
        'drop *',
        #'keep *_testProducer_*_*',
        'keep *_testProducerSerial_*_*',
    )
)

process.serial_path = cms.Path(process.testProducerSerial + process.testAnalyzerSerial)
#process.serial_path = cms.Path(process.testProducerSerial)

process.output_path = cms.EndPath(process.output)

process.maxEvents.input = 10
