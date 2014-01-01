#include "Metazion/Share/Utility/Utility.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

char* NormalizeFilePath(char *filePath)   {
  for (int index = 0; filePath[index] != '\0'; ++index) { 
    if (filePath[index] == '/') {
      filePath[index] = '\\';
    }
  }
  return filePath;
}

DECL_NAMESPACE_MZ_SHARE_END
