// System libraries
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include "CsvParser.h"


namespace Csv
{

//CSV_ROW
CsvRow::CsvRow(std::size_t rowSize ):m_rowSize(rowSize),m_rowData(m_rowSize)
{
}

#ifdef EXCEPTIONS_ALLOWED
const std::string & CsvRow::operator[](std::size_t index) const
{
   if(index > m_rowData.size() - 1)
   throw std::range_error("Index out of range of the CsvRow vector");
   return m_rowData[index];
}

std::string & CsvRow::operator[](std::size_t index)
{
   if(index > m_rowData.size() - 1)
   throw std::range_error("Index out of range of the CsvRow vector");
   return m_rowData[index];
}
#endif // EXCEPTIONS_ALLOWED

bool CsvRow::getField(std::size_t idx, std::string& field) const
{
   if (idx > m_rowData.size() - 1)
   {
      return false;
   }
   field = m_rowData[idx];
   return true;
}

bool CsvRow::addField(const std::string& data)
{
   m_rowData.push_back(data);
   return true;
}

bool CsvRow::removeField(std::size_t elementIdx)
{
   if (elementIdx > (m_rowData.size() - 1))
   {
      return false;
   }
   m_rowData.erase(m_rowData.begin() + elementIdx);
   return true;
}

std::size_t CsvRow::getNumOfFields() const
{
   return m_rowData.size();
}

void CsvRow::clearAll()
{
   m_rowData.clear();
}


//TABLE
CsvTable::CsvTable(std::size_t numOfRows):m_numOfRows(numOfRows),m_rows(numOfRows)
{

}

#ifdef EXCEPTIONS_ALLOWED
const CsvRow & CsvTable::operator[](std::size_t index) const
{
   if(index > m_rows.size() - 1)
   throw std::range_error("Index out of range of the CsvTable vector");

   return m_rows[index];
}

CsvRow & CsvTable::operator[](std::size_t index)
{
   if(index > m_rows.size() - 1)
   throw std::range_error("Index out of range of the CsvTable vector");
   return m_rows[index];
}
#endif // EXCEPTIONS_ALLOWED

bool CsvTable::getRow(std::size_t idx, CsvRow& row) const
{
   if (idx > m_rows.size() - 1)
   {
      return false;
   }
   row = m_rows[idx];
   return true;
}

bool CsvTable::addRow(const CsvRow& row)
{
   m_rows.push_back(row);
   return true;
}

bool CsvTable::removeRow(std::size_t rowIdx)
{
   if (rowIdx > m_rows.size() - 1)
      return false;
   m_rows.erase(m_rows.begin() + rowIdx);
   return true;
}

std::size_t CsvTable::getRowNumbers() const
{
   return m_rows.size();
}

void CsvTable::clearAll()
{
   m_rows.clear();
}


//PARSER
CsvParser::CsvParser(const std::string& fieldDelimiter, const std::string& rowDelimiter):
    m_fieldDelimiter(fieldDelimiter),
    m_rowDelimiter(rowDelimiter),
    m_fileName("default.csv"),
    m_isDebugEnabled(false)
{
}

void CsvParser::setCsvFileDelimeters(const std::string& fieldDelimiter, const std::string& rowDelimiter)
{
   m_fieldDelimiter = fieldDelimiter;
   m_rowDelimiter = rowDelimiter;
}

bool CsvParser::readRow(CsvRow& csvRow)
{
   std::ifstream ifStream(m_fileName, std::ifstream::in);

   if ((ifStream.rdstate() & std::ifstream::failbit) != 0)
   {
      printDebugInfo("Error opening 'test.csv'.");
      return false;
   }

   std::string csvLine;
   // reads a line from the CSV file
   std::getline(ifStream, csvLine);

   std::istreambuf_iterator<char> eos;
   std::string theEntireFile(std::istreambuf_iterator<char>(ifStream), eos);

   printDebugInfo("The entire file:" + theEntireFile);

   printDebugInfo("csvLine = " + csvLine);

   return convertRowStringToRowFields(csvLine, csvRow);

}

bool CsvParser::readCsvFile(const std::string& fileName, CsvTable& csvTable)
{
   std::ifstream ifStream(fileName, std::ifstream::in);

   if ((ifStream.rdstate() & std::ifstream::failbit) != 0)
   {
      //error while opening file
      printDebugInfo("Couldn't open " + fileName);
      return false;
   }

   std::istreambuf_iterator<char> eos;
   std::string entireFile(std::istreambuf_iterator<char>(ifStream), eos);

   ifStream.close();

   printDebugInfo("The entire file:" + entireFile);

   while (1)
   {
      printDebugInfo("Next iteration of processing loop.\r\n");
      std::string csvLine;
      // Read a line from the CSV file.

      // Search for the terminating character(s)
      std::size_t found = entireFile.find(m_rowDelimiter);

      if (found == std::string::npos)
      {
         // Terminating character not found, so read entire string
         // (must be last line)
         printDebugInfo("Line delimiter not found, must be last line.");
         csvLine = entireFile.substr(0, entireFile.size());
         // Removes line from the stream
         entireFile.erase(0, entireFile.size());
      }
      else
      {
         printDebugInfo("Line delimiter found at '" + std::to_string(found) + "'.");
         csvLine = entireFile.substr(0, (unsigned long int) found);
         // Removes line from the stream (including the line delimiters)
         entireFile.erase(0, found + m_rowDelimiter.size());
      }

      printDebugInfo("csvLine = " + csvLine);
      printDebugInfo("csvLine size() = " + std::to_string(csvLine.size()));

      // This stops ExtractElementsToRow from running if the last valid
      // row has a new line character at the end
      if (csvLine.empty())
      {
         printDebugInfo("csvLine empty.");
         return false;
      }

      CsvRow csvRow;
      if (convertRowStringToRowFields(csvLine, csvRow))
      {
         csvTable.addRow(csvRow);
      }
   }
   return true;
}

bool CsvParser::isOStreamCorrect(const std::ostream& ostream)
{
   if (ostream.fail())
   {
      return false;
   }
   else
   {
      return true;
   }
}

bool CsvParser::createCsvFile(const std::string& fileName,const CsvTable& csvTable)
{
   printDebugInfo("Entered '" + std::string(__FUNCTION__));
   printDebugInfo("Number of rows = " + std::to_string(csvTable.getRowNumbers()));
   // Create output stream to file
   std::ofstream outputFile(fileName);
   if (!isOStreamCorrect(outputFile))
   {
      return false;
   }

   std::string result;

   // Iterate through the CSV table
   std::size_t x, y;

   CsvRow row(1);
   std::string field;
   for (x = 0; x < csvTable.getRowNumbers(); x++)
   {
      csvTable.getRow(x, row);
      printDebugInfo("Num of fields = " + std::to_string(row.getNumOfFields()));
      for (y = 0; y < (row.getNumOfFields()); y++)
      {
         row.getField(y, field);
         printDebugInfo("Writing " + field + " to file.");
         outputFile << field;
         if (!isOStreamCorrect(outputFile))
         {
            return false;
         }

         // adds a field delimiter until this is the last field in a row
         if (y != row.getNumOfFields() - 1)
         {
            outputFile << m_fieldDelimiter;
            if (!isOStreamCorrect(outputFile))
            {
               return false;
            }
         }

      }
      // add delimeter to every end of the row
      printDebugInfo("Writing the row's delimiter.");
      outputFile << m_rowDelimiter;
      if (!isOStreamCorrect(outputFile))
      {
         return false;
      }
   }

   // Close the file
   outputFile.close();
   if (!isOStreamCorrect(outputFile))
   {
      return false;
   }
   return true;
}

bool CsvParser::convertRowStringToRowFields(const std::string& csvLine, CsvRow& csvRow)
{
   printDebugInfo("Entered " + std::string(__FUNCTION__));

   int lastFieldDelimiterPosition = 0;
   bool firstTime = true;
   while (true)
   {
      // search for a next occurrence of the field delimiter
      int nextFieldDelimiterPosition;
      if (firstTime)
      {
            nextFieldDelimiterPosition = csvLine.find(m_fieldDelimiter, lastFieldDelimiterPosition);
         firstTime = false;
      }
      else
            nextFieldDelimiterPosition = csvLine.find(m_fieldDelimiter, lastFieldDelimiterPosition + 1);

      int x = 0;
      std::string field;

      // Check to see if delimiter was found
      if (nextFieldDelimiterPosition == static_cast<int>(std::string::npos))
      {
         printDebugInfo("Delimiter not found, must be last field in the row.");
         // field delimiter not found, must be last field in a row,
            for(x = lastFieldDelimiterPosition + 1; x < static_cast<int>(csvLine.length()); x++)
         {
            field += csvLine[x];
         }
      }
      else
      {
         // Next delimiter in row was found
         printDebugInfo("Delimiter found, must not be last field in the row.");
         // are we at the first element or not?
         if (lastFieldDelimiterPosition == 0)
         {
            for (x = lastFieldDelimiterPosition; x < nextFieldDelimiterPosition;
                  x++)
            {
               field += csvLine[x];
            }
         }
         else
         {
                for(x = lastFieldDelimiterPosition + 1; x < nextFieldDelimiterPosition; x++)
            {
               field += csvLine[x];
            }
         }
      }
      csvRow.addField(field);
      printDebugInfo("field = " + field);

      if (nextFieldDelimiterPosition == static_cast<int>(std::string::npos))
      {
         printDebugInfo("End of the row reached.");
         break;
      }
      lastFieldDelimiterPosition = nextFieldDelimiterPosition;
   }

   return true;
}

//CsvParser Debug helpers:
bool CsvParser::shouldShowDebugInfo() const
{
   return true;
}
void CsvParser::printDebugInfo(const std::string& msg)
{
   if(m_isDebugEnabled)
   {
      std::cout<<"CSV_PARSER_DEBUG: "<<msg.c_str();
   }
}


}

