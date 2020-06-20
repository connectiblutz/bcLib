#include "bcl/singleton.h"

namespace bcl {
  
std::unordered_map<std::type_index,std::weak_ptr<void>> Singleton::weakSingleton;
std::unordered_map<std::type_index,std::shared_ptr<void>> Singleton::strongSingleton;

}