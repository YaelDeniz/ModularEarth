#pragma once

#ifdef __CLING__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace ModularEarth;
#pragma link C++ class ModularEarth::Block+;
#pragma link C++ class ModularEarth::Sublayer+;
#pragma link C++ struct ModularEarth::SublayerConfig+;
#pragma link C++ struct ModularEarth::SubLayersUpdates+;
#pragma link C++ struct ModularEarth::LayerConfig+;
#pragma link C++ class ModularEarth::Layer+;
#pragma link C++ class ModularEarth::Earth+;

#endif
