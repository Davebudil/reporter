#ifndef EXPORT_H
#define EXPORT_H
#include <exportxls.h>
#include <exportcsv.h>
#include <exporthtml.h>
class Export{
   public:
      Export();
      ~Export();
   private:
      ExportXLS m_XLS;
      ExportCSV m_CSV;
      ExportHTML m_HTML;
};

#endif // EXPORT_H
