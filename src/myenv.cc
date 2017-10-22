// myenv.cc
// Copyright 2016 Mac Radigan
// All Rights Reserved

#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string.h>
#include <errno.h>

   // represents an array of C strings
   class pcstr_range_t
   {
    public:
     
     pcstr_range_t(const pcstr_range_t &o)  = default;
     
     pcstr_range_t(int n, const char * const pcstr[])
       : n_(n), pcstr_(const_cast<char **>(pcstr)) { /**/ }
     
     pcstr_range_t(const char * const pcstr[])
       : n_(-1), pcstr_(const_cast<char **>(pcstr))
     {
       while(0 != pcstr_[++n_]);
     }
     
     typedef char * const iterator;
     typedef const char * const *const_iterator;
     
     const_iterator begin() const { return pcstr_; }
     
     const_iterator end() const
     {
       return pcstr_ + n_;
     }
     
     std::size_t size()
     {
       return n_;
     }
     
     pcstr_range_t& operator++() // prefix
     {
       ++pcstr_;
       --n_;
       return *this;
     }
     
     pcstr_range_t operator++(int) // postfix
     {
       pcstr_range_t result(*this);
       ++(*this);
       return result;
     }
     
     pcstr_range_t operator+(int k) const
     {
       pcstr_range_t result(*this);
       result.pcstr_ += k;
       result.n_ -= k;
       return result;
     }
     
    private:
     char **pcstr_  = nullptr;
     std::size_t n_ = 0;
  };

  // represents lower and upper bounds
  template<typename T>
  struct range_t
  {
    T lo_;
    T hi_;
    range_t(T lo, T hi) 
     : lo_(lo), hi_(hi) {};
  };

  // represents a slice of an array
  template<typename T>
  struct slice_t
  {
    T* data_;
    std::size_t n_;
    std::size_t lo_;
    std::size_t hi_;
     
    T* begin() { return data_ + lo_; }
     
    T* end() { return data_ + hi_; }
     
    slice_t(T *data, std::size_t lo, std::size_t hi) 
     : data_(data), n_(hi-lo), lo_(lo), hi_(hi) {};
     
    slice_t(T *data, const range_t<std::size_t> &range)
     : data_(data), n_(range.hi_-range.lo_), lo_(range.lo_), hi_(range.hi_) {};
  };

  template<char delimiter=' '>
  class Tokenizer : public std::string 
  {
    public:
      static std::vector<std::string> split(const std::string &s)
      {
        std::istringstream iss(s);
        std::vector<std::string> tokens((std::istream_iterator<Tokenizer<delimiter> >(iss)), std::istream_iterator<Tokenizer<delimiter> >());
        return tokens;
      }
      static char* const* csplit(const std::string &s)
      {
        std::istringstream iss(s);
        std::vector<std::string> tokens((std::istream_iterator<Tokenizer<delimiter> >(iss)), std::istream_iterator<Tokenizer<delimiter> >());
        const std::size_t N = tokens.size()+1;
        char** ctokens = new char*[N];
        std::size_t k = 0;
        for(auto token : tokens)
        {
          ctokens[k] = strdup(const_cast<char *>(token.c_str()));
          k++;
        }
        ctokens[k] = nullptr;
        return ctokens;              
      }
  };
  template<char delimiter>
  std::istream& operator>>(std::istream &is, Tokenizer<delimiter> &o)
  {
    std::getline(is, o, delimiter);
    return is;
  }

  enum Status : uint32_t
  {
    EXIT_TIMEDOUT      = 124,
    EXIT_CANCELED      = 125,
    EXIT_CANNOT_INVOKE = 126,
    EXIT_ENOENT        = 127
  };

  int main(int argc, char *argv[], char *envp[])
  {
    int status = EXIT_SUCCESS;

    pcstr_range_t envp_range(envp);
    pcstr_range_t argv_range(argc, argv);
    std::ostream_iterator<std::string> os(std::cout,"\n");

#if 0
    std::size_t k = 0;
    for(const auto env : argv_range)
    {
      if(std::string(env).find("=") != std::string::npos)
      {
        putenv(const_cast<char *>(env));
      }
      k++;
    }
#endif

    if(argc < 2)
    {
      std::copy(envp_range.begin(), envp_range.end(), os);
      return status;
    }
    else if(0 == std::string(argv[1]).find("-- "))
    {
      char* const* tokens = Tokenizer<' '>::csplit(argv[1]);
      const char* const &prog = tokens[1];
      char* const* args = &tokens[1];
      execvp(prog, args);
      status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
    }
    else if(0 == std::string(argv[1]).find("--"))
    {
      const auto args = argv + 2;
      execvp(argv[2], args);
      status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
    }

    return status;

  } // main


// *EOF*
