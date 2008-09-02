//-*- C++ -*-
//
// Package:    TrkJetMaker
// Class:      TrkJetMaker.cc
//
/**\class TrkJetMaker TrkJetMaker.cc CMS2/NtupleMaker/src/TrkJetMaker.cc

Description: Dumps the TrkJet contents into the ntuple
Implementation:
*/
//
// Original Author:  Sanjay Padhi
//         Created:  Mon Jun 23 03:57:47 CEST 2008
// $Id: TrkJetMaker.cc,v 1.1 2008/08/19 14:23:33 spadhi Exp $
//


// system include files
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CMS2/NtupleMaker/interface/TrkJetMaker.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

typedef math::XYZTLorentzVector LorentzVector;


//
// constructors and destructor
//
TrkJetMaker::TrkJetMaker(const edm::ParameterSet& iConfig)
{
  // product of this EDProducer
  produces<unsigned int> ("evtntrkjets").setBranchAlias("evt_ntrkjets"); // number of jets
  produces<std::vector<LorentzVector> > ("trkjetsp4").setBranchAlias("trkjets_p4"); // p4 of the jet

  // parameters from configuration
  trkJetsInputTag = iConfig.getParameter<edm::InputTag>("trkJetsInputTag");

}


TrkJetMaker::~TrkJetMaker()
{

}

void
TrkJetMaker::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // get TrkJet collection
  edm::Handle<edm::View<reco::BasicJet> > trkJets;
  iEvent.getByLabel(trkJetsInputTag, trkJets);
  
  // create containers
  std::auto_ptr<unsigned int> evt_ntrkjets(new unsigned int(trkJets->size()));
  std::auto_ptr<std::vector<LorentzVector> > vector_trkjets_p4(new std::vector<LorentzVector>);
  
  // loop over jets and fill containers
  edm::View<reco::BasicJet>::const_iterator jetsEnd = trkJets->end();
  for ( edm::View<reco::BasicJet>::const_iterator jet = trkJets->begin();
        jet != jetsEnd;
        ++jet) {
    vector_trkjets_p4->push_back(jet->p4());
  }

  // put containers into event
  iEvent.put(evt_ntrkjets, "evtntrkjets");
  iEvent.put(vector_trkjets_p4, "trkjetsp4");

}


void
TrkJetMaker::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
TrkJetMaker::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrkJetMaker);