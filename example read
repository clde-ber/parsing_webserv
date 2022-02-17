#include "replace.hpp"

replace::replace( void ) : _s1(""), _s2(""), _c('\0'), _buf(""), _name("")
{
  std::cout << "constructor called" << std::endl;
}

replace::~replace( void )
{
  std::cout << "destructor called" << std::endl;
}

void replace::writeContentToFile(std::string argZero, std::string argOne, std::string argTwo, std::string outputFile)
{
    unsigned long i(0);

    _s1 += argOne;
    _s2 += argTwo;
    std::ifstream ifs(argZero.c_str(), std::ifstream::in);
    if (!ifs.is_open())
    {
        std::cout << "Error : file not found" << std::endl;
        return;
    }
    std::ofstream ofs(outputFile.c_str(), std::ofstream::out);
    while (ifs.good() and !ifs.eof())
    {
      _c = ifs.get();
      if (!ifs.eof())
        _buf += _c;
    }
    while (i < _buf.length())
    {
        if (_buf.at(i) == _s1.at(0) and !_buf.compare(i, _s1.length(), _s1))
        {
            ofs.write(_s2.c_str(), _s2.length());
            i += _s1.length();
        }
        else
        {
            ofs.put(_buf.at(i));
            i++;
        }
    }
    ifs.close();
    ofs.close();
}