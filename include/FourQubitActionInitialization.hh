//
/// \file FourQubitActionInitialization.hh
/// \brief Definition of the class 
///   

#ifndef FourQubitActionInitialization_hh
#define FourQubitActionInitialization_hh 1

#include "G4VUserActionInitialization.hh"

class FourQubitActionInitialization : public G4VUserActionInitialization {
public:
  FourQubitActionInitialization() {;}
  virtual ~FourQubitActionInitialization() {;}
  virtual void Build() const;
};

#endif	/* FourQubitActionInitialization_hh */
