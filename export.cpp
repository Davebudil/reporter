#include "export.h"
#include "log.h"

Export::Export(){

}

Export::~Export(){

}

ExportXLS Export::getXLS() const{
   return m_XLS;
}
ExportCSV Export::getCSV() const{
   return m_CSV;
}
ExportHTML Export::getHTML() const{
   return m_HTML;
}
