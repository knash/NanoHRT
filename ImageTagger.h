#ifndef ImageTAGGER_H
#define ImageTAGGER_H


#include <memory>
#include <fstream>

// CMS
#include "DataFormats/PatCandidates/interface/Jet.h"

// ROOT
#include "TLorentzVector.h"


// ImageTagger Class
class ImageTagger {

  public:

    ImageTagger( const std::string& dnnFile);
    ~ImageTagger();
    void processimage( const pat::Jet& jet );
    const std::map<std::string,double>& execute( const pat::Jet& jet );

  protected:
    float test;  
    std::map<std::string,double> m_NNresults;      
};

#endif
