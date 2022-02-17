#include "serverAttributes.hpp"

std::string getContent(std::string file)
{
    std::ifstream is (file.c_str(), std::ifstream::binary);
    if (is) {
    // get length of file:
    is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);

    char * buffer = new char [length];

    std::cout << "Reading " << length << " characters... ";
    // read data as a block:
    is.read (buffer,length);

    if (is)
      std::cout << "all characters read successfully.";
    else
      std::cout << "error: only " << is.gcount() << " could be read";
    is.close();

    // ...buffer contains the entire file...

    return buffer;
  }
  return 0;
}

int parseContent(std::string content)
{
    size_t posStart = 0;
    size_t posEnd = content.length();
    //check wheter there is an even number of {/}
    while (posStart != posEnd)
    {
        if ((posStart = content.find("{", posStart)) == std::string::npos)
        {
            if ((posEnd = content.rfind("}", posEnd)) == std::string::npos)
                return TRUE;
            else
                return FALSE;
        }
        if ((posEnd = content.rfind("}", posEnd)) == std::string::npos)
            return FALSE;
        posStart++;
        posEnd--;
    }
    return TRUE;
}

int main(void)
{
    std::string file = "conf_example.conf";
    std::string output = "";
    output = getContent(file);
    std::cout << output << std::endl;
    std::cout << "is valid {} : " << parseContent(output) << std::endl;
}