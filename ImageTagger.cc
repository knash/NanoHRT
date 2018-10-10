
#include "../interface/ImageTagger.h"


// constructor
ImageTagger::ImageTagger( const std::string& dnnFile) :
  test(0)
{} // end constructor


ImageTagger::~ImageTagger(){
}


const std::map<std::string,double>& ImageTagger::execute( const pat::Jet& jet ){
    m_NNresults["dnn_qcd"] = 1;
    return m_NNresults;
}


void ImageTagger::processimage( const pat::Jet& jet ){
 
}


