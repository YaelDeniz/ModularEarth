// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dictdIModularEarthDict
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
         instance("ModularEarth::Sublayer", "Sublayer.h", 32,
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
         instance("ModularEarth::Layer", "Layer.h", 83,
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
namespace ModularEarth{class __attribute__((annotate("$clingAutoload$Sublayer.h")))  __attribute__((annotate("$clingAutoload$inc/Layer.h")))  Sublayer;}
namespace ModularEarth{struct __attribute__((annotate("$clingAutoload$inc/Layer.h")))  SublayerConfig;}
namespace ModularEarth{class __attribute__((annotate("$clingAutoload$inc/Layer.h")))  Layer;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "ModularEarthDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "inc/Block.h"
#include "inc/Layer.h"
#include "inc/Sublayer.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"ModularEarth::Block", payloadCode, "@",
"ModularEarth::Layer", payloadCode, "@",
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
