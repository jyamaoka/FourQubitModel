/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id: 539f524339ae53ad098a07cfa3bebd07784d23dd $

#include "FourQubitActionInitialization.hh"
#include "FourQubitPrimaryGeneratorAction.hh"
#include "FourQubitSteppingAction.hh"
#include "G4CMPStackingAction.hh"

void FourQubitActionInitialization::Build() const {
  SetUserAction(new FourQubitPrimaryGeneratorAction);
  SetUserAction(new G4CMPStackingAction);
  SetUserAction(new FourQubitSteppingAction);
} 
