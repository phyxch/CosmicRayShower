// 10/6/2014: Hexc, Olesya - This is a new file which is copied from ECRS2014
//   used for writing out the track info when it is created.
#ifndef MuELossTrackingAction_h
#define MuELossTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"


class MuELossTrackingAction : public G4UserTrackingAction
{
  public:
    MuELossTrackingAction();
    ~MuELossTrackingAction();

    void PreUserTrackingAction(const G4Track*);
    void PostUserTrackingAction(const G4Track*);

};

#endif
