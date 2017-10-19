// myenv.cc
// Copyright 2016 Mac Radigan
// All Rights Reserved

#include <cstdlib>
#include <iostream>
#include <iterator>
#include <gflags/gflags.h>

  DEFINE_bool(debug,   false, "debug");
  DEFINE_bool(verbose, false, "verbose");

   class envp_range 
   {
    public:
     envp_range(const char * const envp[])
       : envp_(envp)
     {
     }
     const char * const *begin() const { return envp_; }
     const char * const *end() const 
     {
       std::size_t k = 0;
       while(0!=envp_[++k]);
       return envp_ + k;
     }
    private:
     const char * const *envp_;
  };

  int main(int argc, char *argv[], char *envp[])
  {

    int status = EXIT_SUCCESS;

    std::string usage("myenv - Sample usage:\n");
    usage += argv[0];
    usage += " ";
    gflags::SetUsageMessage(usage);
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    envp_range range(envp);
    std::ostream_iterator<std::string> os(std::cout,"\n");
    std::copy(range.begin(), range.end(), os);

    return status;
  } // main

// *EOF*
