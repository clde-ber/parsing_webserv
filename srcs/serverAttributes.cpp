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

int main(void)
{
    std::string file = "conf_example.conf";
    std::string output = "";
    output = getContent(file);
    std::cout << output << std::endl;
}