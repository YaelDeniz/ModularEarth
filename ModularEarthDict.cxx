// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME ModularEarthDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "inc/Block.h"
#include "inc/Earth.h"
#include "inc/Layer.h"
#include "inc/Sublayer.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ModularEarth {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *ModularEarth_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("ModularEarth", 0 /*version*/, "Block.h", 35,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &ModularEarth_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *ModularEarth_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static TClass *ModularEarthcLcLBlock_Dictionary();
   static void ModularEarthcLcLBlock_TClassManip(TClass*);
   static void *new_ModularEarthcLcLBlock(void *p = nullptr);
   static void *newArray_ModularEarthcLcLBlock(Long_t size, void *p);
   static void delete_ModularEarthcLcLBlock(void *p);
   static void deleteArray_ModularEarthcLcLBlock(void *p);
   static void destruct_ModularEarthcLcLBlock(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ModularEarth::Block*)
   {
      ::ModularEarth::Block *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ModularEarth::Block));
      static ::ROOT::TGenericClassInfo 
         instance("ModularEarth::Block", "Block.h", 37,
                  typeid(::ModularEarth::Block), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ModularEarthcLcLBlock_Dictionary, isa_proxy, 4,
                  sizeof(::ModularEarth::Block) );
      instance.SetNew(&new_ModularEarthcLcLBlock);
      instance.SetNewArray(&newArray_ModularEarthcLcLBlock);
      instance.SetDelete(&delete_ModularEarthcLcLBlock);
      instance.SetDeleteArray(&deleteArray_ModularEarthcLcLBlock);
      instance.SetDestructor(&destruct_ModularEarthcLcLBlock);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ModularEarth::Block*)
   {
      return GenerateInitInstanceLocal(static_cast<::ModularEarth::Block*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::ModularEarth::Block*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ModularEarthcLcLBlock_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::ModularEarth::Block*>(nullptr))->GetClass();
      ModularEarthcLcLBlock_TClassManip(theClass);
   return theClass;
   }

   static void ModularEarthcLcLBlock_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ModularEarthcLcLSublayer_Dictionary();
   static void ModularEarthcLcLSublayer_TClassManip(TClass*);
   static void *new_ModularEarthcLcLSublayer(void *p = nullptr);
   static void *newArray_ModularEarthcLcLSublayer(Long_t size, void *p);
   static void delete_ModularEarthcLcLSublayer(void *p);
   static void deleteArray_ModularEarthcLcLSublayer(void *p);
   static void destruct_ModularEarthcLcLSublayer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ModularEarth::Sublayer*)
   {
      ::ModularEarth::Sublayer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ModularEarth::Sublayer));
      static ::ROOT::TGenericClassInfo 
         instance("ModularEarth::Sublayer", "Sublayer.h", 37,
                  typeid(::ModularEarth::Sublayer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ModularEarthcLcLSublayer_Dictionary, isa_proxy, 4,
                  sizeof(::ModularEarth::Sublayer) );
      instance.SetNew(&new_ModularEarthcLcLSublayer);
      instance.SetNewArray(&newArray_ModularEarthcLcLSublayer);
      instance.SetDelete(&delete_ModularEarthcLcLSublayer);
      instance.SetDeleteArray(&deleteArray_ModularEarthcLcLSublayer);
      instance.SetDestructor(&destruct_ModularEarthcLcLSublayer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ModularEarth::Sublayer*)
   {
      return GenerateInitInstanceLocal(static_cast<::ModularEarth::Sublayer*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::ModularEarth::Sublayer*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ModularEarthcLcLSublayer_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::ModularEarth::Sublayer*>(nullptr))->GetClass();
      ModularEarthcLcLSublayer_TClassManip(theClass);
   return theClass;
   }

   static void ModularEarthcLcLSublayer_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ModularEarthcLcLSublayerConfig_Dictionary();
   static void ModularEarthcLcLSublayerConfig_TClassManip(TClass*);
   static void *new_ModularEarthcLcLSublayerConfig(void *p = nullptr);
   static void *newArray_ModularEarthcLcLSublayerConfig(Long_t size, void *p);
   static void delete_ModularEarthcLcLSublayerConfig(void *p);
   static void deleteArray_ModularEarthcLcLSublayerConfig(void *p);
   static void destruct_ModularEarthcLcLSublayerConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ModularEarth::SublayerConfig*)
   {
      ::ModularEarth::SublayerConfig *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ModularEarth::SublayerConfig));
      static ::ROOT::TGenericClassInfo 
         instance("ModularEarth::SublayerConfig", "Layer.h", 69,
                  typeid(::ModularEarth::SublayerConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ModularEarthcLcLSublayerConfig_Dictionary, isa_proxy, 4,
                  sizeof(::ModularEarth::SublayerConfig) );
      instance.SetNew(&new_ModularEarthcLcLSublayerConfig);
      instance.SetNewArray(&newArray_ModularEarthcLcLSublayerConfig);
      instance.SetDelete(&delete_ModularEarthcLcLSublayerConfig);
      instance.SetDeleteArray(&deleteArray_ModularEarthcLcLSublayerConfig);
      instance.SetDestructor(&destruct_ModularEarthcLcLSublayerConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ModularEarth::SublayerConfig*)
   {
      return GenerateInitInstanceLocal(static_cast<::ModularEarth::SublayerConfig*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::ModularEarth::SublayerConfig*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ModularEarthcLcLSublayerConfig_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::ModularEarth::SublayerConfig*>(nullptr))->GetClass();
      ModularEarthcLcLSublayerConfig_TClassManip(theClass);
   return theClass;
   }

   static void ModularEarthcLcLSublayerConfig_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ModularEarthcLcLSubLayersUpdates_Dictionary();
   static void ModularEarthcLcLSubLayersUpdates_TClassManip(TClass*);
   static void *new_ModularEarthcLcLSubLayersUpdates(void *p = nullptr);
   static void *newArray_ModularEarthcLcLSubLayersUpdates(Long_t size, void *p);
   static void delete_ModularEarthcLcLSubLayersUpdates(void *p);
   static void deleteArray_ModularEarthcLcLSubLayersUpdates(void *p);
   static void destruct_ModularEarthcLcLSubLayersUpdates(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ModularEarth::SubLayersUpdates*)
   {
      ::ModularEarth::SubLayersUpdates *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ModularEarth::SubLayersUpdates));
      static ::ROOT::TGenericClassInfo 
         instance("ModularEarth::SubLayersUpdates", "Layer.h", 83,
                  typeid(::ModularEarth::SubLayersUpdates), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ModularEarthcLcLSubLayersUpdates_Dictionary, isa_proxy, 4,
                  sizeof(::ModularEarth::SubLayersUpdates) );
      instance.SetNew(&new_ModularEarthcLcLSubLayersUpdates);
      instance.SetNewArray(&newArray_ModularEarthcLcLSubLayersUpdates);
      instance.SetDelete(&delete_ModularEarthcLcLSubLayersUpdates);
      instance.SetDeleteArray(&deleteArray_ModularEarthcLcLSubLayersUpdates);
      instance.SetDestructor(&destruct_ModularEarthcLcLSubLayersUpdates);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ModularEarth::SubLayersUpdates*)
   {
      return GenerateInitInstanceLocal(static_cast<::ModularEarth::SubLayersUpdates*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::ModularEarth::SubLayersUpdates*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ModularEarthcLcLSubLayersUpdates_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::ModularEarth::SubLayersUpdates*>(nullptr))->GetClass();
      ModularEarthcLcLSubLayersUpdates_TClassManip(theClass);
   return theClass;
   }

   static void ModularEarthcLcLSubLayersUpdates_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ModularEarthcLcLLayerConfig_Dictionary();
   static void ModularEarthcLcLLayerConfig_TClassManip(TClass*);
   static void *new_ModularEarthcLcLLayerConfig(void *p = nullptr);
   static void *newArray_ModularEarthcLcLLayerConfig(Long_t size, void *p);
   static void delete_ModularEarthcLcLLayerConfig(void *p);
   static void deleteArray_ModularEarthcLcLLayerConfig(void *p);
   static void destruct_ModularEarthcLcLLayerConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ModularEarth::LayerConfig*)
   {
      ::ModularEarth::LayerConfig *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ModularEarth::LayerConfig));
      static ::ROOT::TGenericClassInfo 
         instance("ModularEarth::LayerConfig", "Layer.h", 106,
                  typeid(::ModularEarth::LayerConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ModularEarthcLcLLayerConfig_Dictionary, isa_proxy, 4,
                  sizeof(::ModularEarth::LayerConfig) );
      instance.SetNew(&new_ModularEarthcLcLLayerConfig);
      instance.SetNewArray(&newArray_ModularEarthcLcLLayerConfig);
      instance.SetDelete(&delete_ModularEarthcLcLLayerConfig);
      instance.SetDeleteArray(&deleteArray_ModularEarthcLcLLayerConfig);
      instance.SetDestructor(&destruct_ModularEarthcLcLLayerConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ModularEarth::LayerConfig*)
   {
      return GenerateInitInstanceLocal(static_cast<::ModularEarth::LayerConfig*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::ModularEarth::LayerConfig*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ModularEarthcLcLLayerConfig_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::ModularEarth::LayerConfig*>(nullptr))->GetClass();
      ModularEarthcLcLLayerConfig_TClassManip(theClass);
   return theClass;
   }

   static void ModularEarthcLcLLayerConfig_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ModularEarthcLcLLayer_Dictionary();
   static void ModularEarthcLcLLayer_TClassManip(TClass*);
   static void *new_ModularEarthcLcLLayer(void *p = nullptr);
   static void *newArray_ModularEarthcLcLLayer(Long_t size, void *p);
   static void delete_ModularEarthcLcLLayer(void *p);
   static void deleteArray_ModularEarthcLcLLayer(void *p);
   static void destruct_ModularEarthcLcLLayer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ModularEarth::Layer*)
   {
      ::ModularEarth::Layer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ModularEarth::Layer));
      static ::ROOT::TGenericClassInfo 
         instance("ModularEarth::Layer", "Layer.h", 120,
                  typeid(::ModularEarth::Layer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ModularEarthcLcLLayer_Dictionary, isa_proxy, 4,
                  sizeof(::ModularEarth::Layer) );
      instance.SetNew(&new_ModularEarthcLcLLayer);
      instance.SetNewArray(&newArray_ModularEarthcLcLLayer);
      instance.SetDelete(&delete_ModularEarthcLcLLayer);
      instance.SetDeleteArray(&deleteArray_ModularEarthcLcLLayer);
      instance.SetDestructor(&destruct_ModularEarthcLcLLayer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ModularEarth::Layer*)
   {
      return GenerateInitInstanceLocal(static_cast<::ModularEarth::Layer*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::ModularEarth::Layer*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ModularEarthcLcLLayer_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::ModularEarth::Layer*>(nullptr))->GetClass();
      ModularEarthcLcLLayer_TClassManip(theClass);
   return theClass;
   }

   static void ModularEarthcLcLLayer_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ModularEarthcLcLEarth_Dictionary();
   static void ModularEarthcLcLEarth_TClassManip(TClass*);
   static void *new_ModularEarthcLcLEarth(void *p = nullptr);
   static void *newArray_ModularEarthcLcLEarth(Long_t size, void *p);
   static void delete_ModularEarthcLcLEarth(void *p);
   static void deleteArray_ModularEarthcLcLEarth(void *p);
   static void destruct_ModularEarthcLcLEarth(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ModularEarth::Earth*)
   {
      ::ModularEarth::Earth *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ModularEarth::Earth));
      static ::ROOT::TGenericClassInfo 
         instance("ModularEarth::Earth", "Earth.h", 31,
                  typeid(::ModularEarth::Earth), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ModularEarthcLcLEarth_Dictionary, isa_proxy, 4,
                  sizeof(::ModularEarth::Earth) );
      instance.SetNew(&new_ModularEarthcLcLEarth);
      instance.SetNewArray(&newArray_ModularEarthcLcLEarth);
      instance.SetDelete(&delete_ModularEarthcLcLEarth);
      instance.SetDeleteArray(&deleteArray_ModularEarthcLcLEarth);
      instance.SetDestructor(&destruct_ModularEarthcLcLEarth);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ModularEarth::Earth*)
   {
      return GenerateInitInstanceLocal(static_cast<::ModularEarth::Earth*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::ModularEarth::Earth*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ModularEarthcLcLEarth_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::ModularEarth::Earth*>(nullptr))->GetClass();
      ModularEarthcLcLEarth_TClassManip(theClass);
   return theClass;
   }

   static void ModularEarthcLcLEarth_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_ModularEarthcLcLBlock(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::ModularEarth::Block : new ::ModularEarth::Block;
   }
   static void *newArray_ModularEarthcLcLBlock(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::ModularEarth::Block[nElements] : new ::ModularEarth::Block[nElements];
   }
   // Wrapper around operator delete
   static void delete_ModularEarthcLcLBlock(void *p) {
      delete (static_cast<::ModularEarth::Block*>(p));
   }
   static void deleteArray_ModularEarthcLcLBlock(void *p) {
      delete [] (static_cast<::ModularEarth::Block*>(p));
   }
   static void destruct_ModularEarthcLcLBlock(void *p) {
      typedef ::ModularEarth::Block current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::ModularEarth::Block

namespace ROOT {
   // Wrappers around operator new
   static void *new_ModularEarthcLcLSublayer(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::ModularEarth::Sublayer : new ::ModularEarth::Sublayer;
   }
   static void *newArray_ModularEarthcLcLSublayer(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::ModularEarth::Sublayer[nElements] : new ::ModularEarth::Sublayer[nElements];
   }
   // Wrapper around operator delete
   static void delete_ModularEarthcLcLSublayer(void *p) {
      delete (static_cast<::ModularEarth::Sublayer*>(p));
   }
   static void deleteArray_ModularEarthcLcLSublayer(void *p) {
      delete [] (static_cast<::ModularEarth::Sublayer*>(p));
   }
   static void destruct_ModularEarthcLcLSublayer(void *p) {
      typedef ::ModularEarth::Sublayer current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::ModularEarth::Sublayer

namespace ROOT {
   // Wrappers around operator new
   static void *new_ModularEarthcLcLSublayerConfig(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::ModularEarth::SublayerConfig : new ::ModularEarth::SublayerConfig;
   }
   static void *newArray_ModularEarthcLcLSublayerConfig(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::ModularEarth::SublayerConfig[nElements] : new ::ModularEarth::SublayerConfig[nElements];
   }
   // Wrapper around operator delete
   static void delete_ModularEarthcLcLSublayerConfig(void *p) {
      delete (static_cast<::ModularEarth::SublayerConfig*>(p));
   }
   static void deleteArray_ModularEarthcLcLSublayerConfig(void *p) {
      delete [] (static_cast<::ModularEarth::SublayerConfig*>(p));
   }
   static void destruct_ModularEarthcLcLSublayerConfig(void *p) {
      typedef ::ModularEarth::SublayerConfig current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::ModularEarth::SublayerConfig

namespace ROOT {
   // Wrappers around operator new
   static void *new_ModularEarthcLcLSubLayersUpdates(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::ModularEarth::SubLayersUpdates : new ::ModularEarth::SubLayersUpdates;
   }
   static void *newArray_ModularEarthcLcLSubLayersUpdates(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::ModularEarth::SubLayersUpdates[nElements] : new ::ModularEarth::SubLayersUpdates[nElements];
   }
   // Wrapper around operator delete
   static void delete_ModularEarthcLcLSubLayersUpdates(void *p) {
      delete (static_cast<::ModularEarth::SubLayersUpdates*>(p));
   }
   static void deleteArray_ModularEarthcLcLSubLayersUpdates(void *p) {
      delete [] (static_cast<::ModularEarth::SubLayersUpdates*>(p));
   }
   static void destruct_ModularEarthcLcLSubLayersUpdates(void *p) {
      typedef ::ModularEarth::SubLayersUpdates current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::ModularEarth::SubLayersUpdates

namespace ROOT {
   // Wrappers around operator new
   static void *new_ModularEarthcLcLLayerConfig(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::ModularEarth::LayerConfig : new ::ModularEarth::LayerConfig;
   }
   static void *newArray_ModularEarthcLcLLayerConfig(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::ModularEarth::LayerConfig[nElements] : new ::ModularEarth::LayerConfig[nElements];
   }
   // Wrapper around operator delete
   static void delete_ModularEarthcLcLLayerConfig(void *p) {
      delete (static_cast<::ModularEarth::LayerConfig*>(p));
   }
   static void deleteArray_ModularEarthcLcLLayerConfig(void *p) {
      delete [] (static_cast<::ModularEarth::LayerConfig*>(p));
   }
   static void destruct_ModularEarthcLcLLayerConfig(void *p) {
      typedef ::ModularEarth::LayerConfig current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::ModularEarth::LayerConfig

namespace ROOT {
   // Wrappers around operator new
   static void *new_ModularEarthcLcLLayer(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::ModularEarth::Layer : new ::ModularEarth::Layer;
   }
   static void *newArray_ModularEarthcLcLLayer(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::ModularEarth::Layer[nElements] : new ::ModularEarth::Layer[nElements];
   }
   // Wrapper around operator delete
   static void delete_ModularEarthcLcLLayer(void *p) {
      delete (static_cast<::ModularEarth::Layer*>(p));
   }
   static void deleteArray_ModularEarthcLcLLayer(void *p) {
      delete [] (static_cast<::ModularEarth::Layer*>(p));
   }
   static void destruct_ModularEarthcLcLLayer(void *p) {
      typedef ::ModularEarth::Layer current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::ModularEarth::Layer

namespace ROOT {
   // Wrappers around operator new
   static void *new_ModularEarthcLcLEarth(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::ModularEarth::Earth : new ::ModularEarth::Earth;
   }
   static void *newArray_ModularEarthcLcLEarth(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::ModularEarth::Earth[nElements] : new ::ModularEarth::Earth[nElements];
   }
   // Wrapper around operator delete
   static void delete_ModularEarthcLcLEarth(void *p) {
      delete (static_cast<::ModularEarth::Earth*>(p));
   }
   static void deleteArray_ModularEarthcLcLEarth(void *p) {
      delete [] (static_cast<::ModularEarth::Earth*>(p));
   }
   static void destruct_ModularEarthcLcLEarth(void *p) {
      typedef ::ModularEarth::Earth current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::ModularEarth::Earth

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = nullptr);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 428,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<int>","std::vector<int, std::allocator<int> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<int>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<int>*>(nullptr))->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete (static_cast<vector<int>*>(p));
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] (static_cast<vector<int>*>(p));
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = nullptr);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 428,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<double>","std::vector<double, std::allocator<double> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<double>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<double>*>(nullptr))->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete (static_cast<vector<double>*>(p));
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] (static_cast<vector<double>*>(p));
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlEModularEarthcLcLSublayerConfiggR_Dictionary();
   static void vectorlEModularEarthcLcLSublayerConfiggR_TClassManip(TClass*);
   static void *new_vectorlEModularEarthcLcLSublayerConfiggR(void *p = nullptr);
   static void *newArray_vectorlEModularEarthcLcLSublayerConfiggR(Long_t size, void *p);
   static void delete_vectorlEModularEarthcLcLSublayerConfiggR(void *p);
   static void deleteArray_vectorlEModularEarthcLcLSublayerConfiggR(void *p);
   static void destruct_vectorlEModularEarthcLcLSublayerConfiggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ModularEarth::SublayerConfig>*)
   {
      vector<ModularEarth::SublayerConfig> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ModularEarth::SublayerConfig>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ModularEarth::SublayerConfig>", -2, "vector", 428,
                  typeid(vector<ModularEarth::SublayerConfig>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEModularEarthcLcLSublayerConfiggR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ModularEarth::SublayerConfig>) );
      instance.SetNew(&new_vectorlEModularEarthcLcLSublayerConfiggR);
      instance.SetNewArray(&newArray_vectorlEModularEarthcLcLSublayerConfiggR);
      instance.SetDelete(&delete_vectorlEModularEarthcLcLSublayerConfiggR);
      instance.SetDeleteArray(&deleteArray_vectorlEModularEarthcLcLSublayerConfiggR);
      instance.SetDestructor(&destruct_vectorlEModularEarthcLcLSublayerConfiggR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ModularEarth::SublayerConfig> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<ModularEarth::SublayerConfig>","std::vector<ModularEarth::SublayerConfig, std::allocator<ModularEarth::SublayerConfig> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<ModularEarth::SublayerConfig>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEModularEarthcLcLSublayerConfiggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<ModularEarth::SublayerConfig>*>(nullptr))->GetClass();
      vectorlEModularEarthcLcLSublayerConfiggR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEModularEarthcLcLSublayerConfiggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEModularEarthcLcLSublayerConfiggR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<ModularEarth::SublayerConfig> : new vector<ModularEarth::SublayerConfig>;
   }
   static void *newArray_vectorlEModularEarthcLcLSublayerConfiggR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<ModularEarth::SublayerConfig>[nElements] : new vector<ModularEarth::SublayerConfig>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEModularEarthcLcLSublayerConfiggR(void *p) {
      delete (static_cast<vector<ModularEarth::SublayerConfig>*>(p));
   }
   static void deleteArray_vectorlEModularEarthcLcLSublayerConfiggR(void *p) {
      delete [] (static_cast<vector<ModularEarth::SublayerConfig>*>(p));
   }
   static void destruct_vectorlEModularEarthcLcLSublayerConfiggR(void *p) {
      typedef vector<ModularEarth::SublayerConfig> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<ModularEarth::SublayerConfig>

namespace ROOT {
   static TClass *vectorlEModularEarthcLcLSublayergR_Dictionary();
   static void vectorlEModularEarthcLcLSublayergR_TClassManip(TClass*);
   static void *new_vectorlEModularEarthcLcLSublayergR(void *p = nullptr);
   static void *newArray_vectorlEModularEarthcLcLSublayergR(Long_t size, void *p);
   static void delete_vectorlEModularEarthcLcLSublayergR(void *p);
   static void deleteArray_vectorlEModularEarthcLcLSublayergR(void *p);
   static void destruct_vectorlEModularEarthcLcLSublayergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ModularEarth::Sublayer>*)
   {
      vector<ModularEarth::Sublayer> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ModularEarth::Sublayer>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ModularEarth::Sublayer>", -2, "vector", 428,
                  typeid(vector<ModularEarth::Sublayer>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEModularEarthcLcLSublayergR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ModularEarth::Sublayer>) );
      instance.SetNew(&new_vectorlEModularEarthcLcLSublayergR);
      instance.SetNewArray(&newArray_vectorlEModularEarthcLcLSublayergR);
      instance.SetDelete(&delete_vectorlEModularEarthcLcLSublayergR);
      instance.SetDeleteArray(&deleteArray_vectorlEModularEarthcLcLSublayergR);
      instance.SetDestructor(&destruct_vectorlEModularEarthcLcLSublayergR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ModularEarth::Sublayer> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<ModularEarth::Sublayer>","std::vector<ModularEarth::Sublayer, std::allocator<ModularEarth::Sublayer> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<ModularEarth::Sublayer>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEModularEarthcLcLSublayergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<ModularEarth::Sublayer>*>(nullptr))->GetClass();
      vectorlEModularEarthcLcLSublayergR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEModularEarthcLcLSublayergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEModularEarthcLcLSublayergR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<ModularEarth::Sublayer> : new vector<ModularEarth::Sublayer>;
   }
   static void *newArray_vectorlEModularEarthcLcLSublayergR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<ModularEarth::Sublayer>[nElements] : new vector<ModularEarth::Sublayer>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEModularEarthcLcLSublayergR(void *p) {
      delete (static_cast<vector<ModularEarth::Sublayer>*>(p));
   }
   static void deleteArray_vectorlEModularEarthcLcLSublayergR(void *p) {
      delete [] (static_cast<vector<ModularEarth::Sublayer>*>(p));
   }
   static void destruct_vectorlEModularEarthcLcLSublayergR(void *p) {
      typedef vector<ModularEarth::Sublayer> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<ModularEarth::Sublayer>

namespace ROOT {
   static TClass *vectorlEModularEarthcLcLSubLayersUpdatesgR_Dictionary();
   static void vectorlEModularEarthcLcLSubLayersUpdatesgR_TClassManip(TClass*);
   static void *new_vectorlEModularEarthcLcLSubLayersUpdatesgR(void *p = nullptr);
   static void *newArray_vectorlEModularEarthcLcLSubLayersUpdatesgR(Long_t size, void *p);
   static void delete_vectorlEModularEarthcLcLSubLayersUpdatesgR(void *p);
   static void deleteArray_vectorlEModularEarthcLcLSubLayersUpdatesgR(void *p);
   static void destruct_vectorlEModularEarthcLcLSubLayersUpdatesgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ModularEarth::SubLayersUpdates>*)
   {
      vector<ModularEarth::SubLayersUpdates> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ModularEarth::SubLayersUpdates>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ModularEarth::SubLayersUpdates>", -2, "vector", 428,
                  typeid(vector<ModularEarth::SubLayersUpdates>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEModularEarthcLcLSubLayersUpdatesgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ModularEarth::SubLayersUpdates>) );
      instance.SetNew(&new_vectorlEModularEarthcLcLSubLayersUpdatesgR);
      instance.SetNewArray(&newArray_vectorlEModularEarthcLcLSubLayersUpdatesgR);
      instance.SetDelete(&delete_vectorlEModularEarthcLcLSubLayersUpdatesgR);
      instance.SetDeleteArray(&deleteArray_vectorlEModularEarthcLcLSubLayersUpdatesgR);
      instance.SetDestructor(&destruct_vectorlEModularEarthcLcLSubLayersUpdatesgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ModularEarth::SubLayersUpdates> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<ModularEarth::SubLayersUpdates>","std::vector<ModularEarth::SubLayersUpdates, std::allocator<ModularEarth::SubLayersUpdates> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<ModularEarth::SubLayersUpdates>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEModularEarthcLcLSubLayersUpdatesgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<ModularEarth::SubLayersUpdates>*>(nullptr))->GetClass();
      vectorlEModularEarthcLcLSubLayersUpdatesgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEModularEarthcLcLSubLayersUpdatesgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEModularEarthcLcLSubLayersUpdatesgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<ModularEarth::SubLayersUpdates> : new vector<ModularEarth::SubLayersUpdates>;
   }
   static void *newArray_vectorlEModularEarthcLcLSubLayersUpdatesgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<ModularEarth::SubLayersUpdates>[nElements] : new vector<ModularEarth::SubLayersUpdates>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEModularEarthcLcLSubLayersUpdatesgR(void *p) {
      delete (static_cast<vector<ModularEarth::SubLayersUpdates>*>(p));
   }
   static void deleteArray_vectorlEModularEarthcLcLSubLayersUpdatesgR(void *p) {
      delete [] (static_cast<vector<ModularEarth::SubLayersUpdates>*>(p));
   }
   static void destruct_vectorlEModularEarthcLcLSubLayersUpdatesgR(void *p) {
      typedef vector<ModularEarth::SubLayersUpdates> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<ModularEarth::SubLayersUpdates>

namespace ROOT {
   static TClass *vectorlEModularEarthcLcLLayerConfiggR_Dictionary();
   static void vectorlEModularEarthcLcLLayerConfiggR_TClassManip(TClass*);
   static void *new_vectorlEModularEarthcLcLLayerConfiggR(void *p = nullptr);
   static void *newArray_vectorlEModularEarthcLcLLayerConfiggR(Long_t size, void *p);
   static void delete_vectorlEModularEarthcLcLLayerConfiggR(void *p);
   static void deleteArray_vectorlEModularEarthcLcLLayerConfiggR(void *p);
   static void destruct_vectorlEModularEarthcLcLLayerConfiggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ModularEarth::LayerConfig>*)
   {
      vector<ModularEarth::LayerConfig> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ModularEarth::LayerConfig>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ModularEarth::LayerConfig>", -2, "vector", 428,
                  typeid(vector<ModularEarth::LayerConfig>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEModularEarthcLcLLayerConfiggR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ModularEarth::LayerConfig>) );
      instance.SetNew(&new_vectorlEModularEarthcLcLLayerConfiggR);
      instance.SetNewArray(&newArray_vectorlEModularEarthcLcLLayerConfiggR);
      instance.SetDelete(&delete_vectorlEModularEarthcLcLLayerConfiggR);
      instance.SetDeleteArray(&deleteArray_vectorlEModularEarthcLcLLayerConfiggR);
      instance.SetDestructor(&destruct_vectorlEModularEarthcLcLLayerConfiggR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ModularEarth::LayerConfig> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<ModularEarth::LayerConfig>","std::vector<ModularEarth::LayerConfig, std::allocator<ModularEarth::LayerConfig> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<ModularEarth::LayerConfig>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEModularEarthcLcLLayerConfiggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<ModularEarth::LayerConfig>*>(nullptr))->GetClass();
      vectorlEModularEarthcLcLLayerConfiggR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEModularEarthcLcLLayerConfiggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEModularEarthcLcLLayerConfiggR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<ModularEarth::LayerConfig> : new vector<ModularEarth::LayerConfig>;
   }
   static void *newArray_vectorlEModularEarthcLcLLayerConfiggR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<ModularEarth::LayerConfig>[nElements] : new vector<ModularEarth::LayerConfig>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEModularEarthcLcLLayerConfiggR(void *p) {
      delete (static_cast<vector<ModularEarth::LayerConfig>*>(p));
   }
   static void deleteArray_vectorlEModularEarthcLcLLayerConfiggR(void *p) {
      delete [] (static_cast<vector<ModularEarth::LayerConfig>*>(p));
   }
   static void destruct_vectorlEModularEarthcLcLLayerConfiggR(void *p) {
      typedef vector<ModularEarth::LayerConfig> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<ModularEarth::LayerConfig>

namespace ROOT {
   static TClass *vectorlEModularEarthcLcLLayergR_Dictionary();
   static void vectorlEModularEarthcLcLLayergR_TClassManip(TClass*);
   static void *new_vectorlEModularEarthcLcLLayergR(void *p = nullptr);
   static void *newArray_vectorlEModularEarthcLcLLayergR(Long_t size, void *p);
   static void delete_vectorlEModularEarthcLcLLayergR(void *p);
   static void deleteArray_vectorlEModularEarthcLcLLayergR(void *p);
   static void destruct_vectorlEModularEarthcLcLLayergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ModularEarth::Layer>*)
   {
      vector<ModularEarth::Layer> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ModularEarth::Layer>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ModularEarth::Layer>", -2, "vector", 428,
                  typeid(vector<ModularEarth::Layer>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEModularEarthcLcLLayergR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ModularEarth::Layer>) );
      instance.SetNew(&new_vectorlEModularEarthcLcLLayergR);
      instance.SetNewArray(&newArray_vectorlEModularEarthcLcLLayergR);
      instance.SetDelete(&delete_vectorlEModularEarthcLcLLayergR);
      instance.SetDeleteArray(&deleteArray_vectorlEModularEarthcLcLLayergR);
      instance.SetDestructor(&destruct_vectorlEModularEarthcLcLLayergR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ModularEarth::Layer> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<ModularEarth::Layer>","std::vector<ModularEarth::Layer, std::allocator<ModularEarth::Layer> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<ModularEarth::Layer>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEModularEarthcLcLLayergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<ModularEarth::Layer>*>(nullptr))->GetClass();
      vectorlEModularEarthcLcLLayergR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEModularEarthcLcLLayergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEModularEarthcLcLLayergR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<ModularEarth::Layer> : new vector<ModularEarth::Layer>;
   }
   static void *newArray_vectorlEModularEarthcLcLLayergR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<ModularEarth::Layer>[nElements] : new vector<ModularEarth::Layer>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEModularEarthcLcLLayergR(void *p) {
      delete (static_cast<vector<ModularEarth::Layer>*>(p));
   }
   static void deleteArray_vectorlEModularEarthcLcLLayergR(void *p) {
      delete [] (static_cast<vector<ModularEarth::Layer>*>(p));
   }
   static void destruct_vectorlEModularEarthcLcLLayergR(void *p) {
      typedef vector<ModularEarth::Layer> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<ModularEarth::Layer>

namespace ROOT {
   static TClass *vectorlEModularEarthcLcLBlockgR_Dictionary();
   static void vectorlEModularEarthcLcLBlockgR_TClassManip(TClass*);
   static void *new_vectorlEModularEarthcLcLBlockgR(void *p = nullptr);
   static void *newArray_vectorlEModularEarthcLcLBlockgR(Long_t size, void *p);
   static void delete_vectorlEModularEarthcLcLBlockgR(void *p);
   static void deleteArray_vectorlEModularEarthcLcLBlockgR(void *p);
   static void destruct_vectorlEModularEarthcLcLBlockgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ModularEarth::Block>*)
   {
      vector<ModularEarth::Block> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ModularEarth::Block>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ModularEarth::Block>", -2, "vector", 428,
                  typeid(vector<ModularEarth::Block>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEModularEarthcLcLBlockgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ModularEarth::Block>) );
      instance.SetNew(&new_vectorlEModularEarthcLcLBlockgR);
      instance.SetNewArray(&newArray_vectorlEModularEarthcLcLBlockgR);
      instance.SetDelete(&delete_vectorlEModularEarthcLcLBlockgR);
      instance.SetDeleteArray(&deleteArray_vectorlEModularEarthcLcLBlockgR);
      instance.SetDestructor(&destruct_vectorlEModularEarthcLcLBlockgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ModularEarth::Block> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<ModularEarth::Block>","std::vector<ModularEarth::Block, std::allocator<ModularEarth::Block> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<ModularEarth::Block>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEModularEarthcLcLBlockgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<ModularEarth::Block>*>(nullptr))->GetClass();
      vectorlEModularEarthcLcLBlockgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEModularEarthcLcLBlockgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEModularEarthcLcLBlockgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<ModularEarth::Block> : new vector<ModularEarth::Block>;
   }
   static void *newArray_vectorlEModularEarthcLcLBlockgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<ModularEarth::Block>[nElements] : new vector<ModularEarth::Block>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEModularEarthcLcLBlockgR(void *p) {
      delete (static_cast<vector<ModularEarth::Block>*>(p));
   }
   static void deleteArray_vectorlEModularEarthcLcLBlockgR(void *p) {
      delete [] (static_cast<vector<ModularEarth::Block>*>(p));
   }
   static void destruct_vectorlEModularEarthcLcLBlockgR(void *p) {
      typedef vector<ModularEarth::Block> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<ModularEarth::Block>

namespace ROOT {
   // Registration Schema evolution read functions
   int RecordReadRules_ModularEarthDict() {
      return 0;
   }
   static int _R__UNIQUE_DICT_(ReadRules_ModularEarthDict) = RecordReadRules_ModularEarthDict();R__UseDummy(_R__UNIQUE_DICT_(ReadRules_ModularEarthDict));
} // namespace ROOT
namespace {
  void TriggerDictionaryInitialization_ModularEarthDict_Impl() {
    static const char* headers[] = {
"inc/Block.h",
"inc/Earth.h",
"inc/Layer.h",
"inc/Sublayer.h",
nullptr
    };
    static const char* includePaths[] = {
"inc",
"/home/ydenizhernandez/root-install/include/",
"/home/ydenizhernandez/ModularEarth/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "ModularEarthDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace ModularEarth{class __attribute__((annotate("$clingAutoload$inc/Block.h")))  Block;}
namespace ModularEarth{class __attribute__((annotate("$clingAutoload$Sublayer.h")))  __attribute__((annotate("$clingAutoload$inc/Earth.h")))  Sublayer;}
namespace ModularEarth{struct __attribute__((annotate("$clingAutoload$Layer.h")))  __attribute__((annotate("$clingAutoload$inc/Earth.h")))  SublayerConfig;}
namespace ModularEarth{struct __attribute__((annotate("$clingAutoload$Layer.h")))  __attribute__((annotate("$clingAutoload$inc/Earth.h")))  SubLayersUpdates;}
namespace ModularEarth{struct __attribute__((annotate("$clingAutoload$Layer.h")))  __attribute__((annotate("$clingAutoload$inc/Earth.h")))  LayerConfig;}
namespace ModularEarth{class __attribute__((annotate("$clingAutoload$Layer.h")))  __attribute__((annotate("$clingAutoload$inc/Earth.h")))  Layer;}
namespace ModularEarth{class __attribute__((annotate("$clingAutoload$inc/Earth.h")))  Earth;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "ModularEarthDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "inc/Block.h"
#include "inc/Earth.h"
#include "inc/Layer.h"
#include "inc/Sublayer.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"ModularEarth::Block", payloadCode, "@",
"ModularEarth::Earth", payloadCode, "@",
"ModularEarth::Layer", payloadCode, "@",
"ModularEarth::LayerConfig", payloadCode, "@",
"ModularEarth::SubLayersUpdates", payloadCode, "@",
"ModularEarth::Sublayer", payloadCode, "@",
"ModularEarth::SublayerConfig", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("ModularEarthDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_ModularEarthDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_ModularEarthDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_ModularEarthDict() {
  TriggerDictionaryInitialization_ModularEarthDict_Impl();
}
