///////////////////////////////////////////////////////////////////////////////
/// \file   main.cpp
/// \author Benjamin Crist
///
/// \brief  Command-line utility for importing textures to a sandwich

#include <iostream>
#include <string>

#include "be/id.h"
#include "pbj/_pbj.h"



int main(int argc, char** argv)
{
   #ifdef DEBUG
   int verbosity = pbj::VAll;
#else
   int verbosity = pbj::VErrorsAndWarnings;
#endif
   
   // Set the appropriate verbosity level
   be::setVerbosity(verbosity);

    if (argc < 3)
    {
        std::cerr << "Usage:" << std::endl
                  << "   " << (argc >= 1 ? argv[0] : "swtexture") << " <sw_id> <tex_id> <filename> [internal_format = GL_RGBA] [srgb] [
    }

    std::string tex_id_src;

   if (argc < 3)
   {
      for (int i = 1; i < argc; ++i)
      {
         std::cout << be::Id(argv[i]) << std::endl;
      }
   }
   else
   {
      while (std::cin)
      {
         std::string str;
         std::getline(std::cin, str);

         if (str.length() > 0)
            std::cout << be::Id(str) << std::endl;
         else
            std::cout << std::endl;
      }
   }

   return 0;
}
