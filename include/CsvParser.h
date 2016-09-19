#pragma once

/***************************************************************************//**
/**
 * @file  CsvParser.h
 * @brief Simpel CSV parser based on https://github.com/mbedded-ninja/CsvCpp
 * @version 1.0.0
 * @author  Lukasz Uszko
 * @date    Tue Sep 13 11:17:57 2016
 *
 ******************************************************************************/


#include <string>
#include <vector>
#include <stdexcept>

#define EXCEPTIONS_ALLOWEDx

namespace Csv
{

/**
 * @brief	Class represents a row of CSV file.
 * @author  Lukasz Uszko
 * @date    05_09_2016
 */
class CsvRow
{
public:

   /**
    * @brief Constructor.
    * @param Row size
    */
   explicit CsvRow(std::size_t rowSize = 0);

#ifdef EXCEPTIONS_ALLOWED
   /**
    * @brief Overloaded [] operator to get a specific field from the CSV row.
    * @param Index of the field in a row
    * @return None
    * @throws {std::range_error} if index is out off a valid range.
    * @note Active when flag EXCEPTIONS_ALLOWED defined, Instead, use {getField} method.
    */
   const std::string & operator[](std::size_t index) const;

   /**
    * @brief Overloaded [] operator to get a specific field from the CSV row.
    * @param Index of the field in a row
    * @return None
    * @throws {std::range_error} if index is out off a valid range.
    * @note Active when flag EXCEPTIONS_ALLOWED defined, Instead, use {getField} method.
    */
   std::string & operator[](std::size_t index);
#endif // EXCEPTIONS_ALLOWED

   /**
    * @brief getter to read a field from a given row
    * @param Index of the field in a row
    * @return true - if a given value found, false - in case of error
    */
   bool getField(std::size_t idx, std::string& field) const;

   /**
    * @brief Adds data to a CSV row.
    * @param Index of the row
    * @return True - added, False - not addeed (index out of range)
    */
   bool addField(const std::string& data);

   /**
    * @brief Removes data from a given index of a row
    * @param Index of an element to be removed from the row
    * @return True - removed, False - not removed
    */
   bool removeField(std::size_t elementIdx);

   /**
    * @brief Returns a number of fields in row
    * @return Number of fields in a row
    */
   std::size_t getNumOfFields() const;

   /**
    * @brief Deletes all fields from the row
    */
   void clearAll();

private:
   /**
    * @brief Stores max number of data fields in a given row.
    * @details Not bigger than 0xFFFF, default value: 20
    */
   std::size_t m_rowSize;

   /**
    * @brief Stores all data fileds of a given row.
    */
   std::vector<std::string> m_rowData;
};

/**
 * @brief	Class represents a CSV file.
 * @author  Lukasz Uszko
 * @date    05_09_2016
 */
class CsvTable
{
public:

   /**
    * @brief Constructor.
    * @param Number of Rows
    */
   explicit CsvTable(std::size_t numOfRows = 0);

#ifdef EXCEPTIONS_ALLOWED
   /**
    * @brief Overloaded [] operator to read a specific row from the CSV table.
    * @param Number of the row
    * @return None
    * @throws {std::range_error} if index is outside valid range.
    */
   const CsvRow & operator[](std::size_t rowIdx) const;

   /**
    * @brief Overloaded [] operator to read a specific row from the CSV table.
    * @param Number of the row
    * @return None
    * @throws {std::range_error} if index is outside valid range.
    */
   CsvRow & operator[](std::size_t rowIdx);
#endif // EXCEPTIONS_ALLOWED
   /**
    * @brief Adds a row to the CSV table.
    * @param Index of the row
    * @return True - added, False - not addeed (index out of range)
    */

   /**
    * @brief getter to read a row from CSV Table
    * @param Index of the row
    * @return true - if a given value found, false - in case of error
    */
   bool getRow(std::size_t idx, CsvRow& row) const;

   bool addRow(const CsvRow& data);

   /**
    * @brief Removes a row from a given index of the table
    * @param Index of an element to be removed from the row
    * @return True - removed, False - not removed
    */
   bool removeRow(std::size_t rowIdx);

   /**
    * @brief Returns number of rows
    * @return Number of rows in the table
    */
   std::size_t getRowNumbers() const;

   /**
    * @brief Deletes all rows from the table
    */
   void clearAll();

private:

   /**
    * @brief Stores max number of rows in the table
    * @details Not bigger than 0xFFFF, default value: 30
    */
   std::size_t m_numOfRows;

   /**
    * @brief Stores all the CsvRows
    */
   std::vector<CsvRow> m_rows;
};

/**
 * @brief	CSV file parser class
 * @author  Lukasz Uszko
 * @date    05_09_2016
 */
class CsvParser
{
public:

   /**
    * @brief Constructor
    * @param fieldDelimiter	The delimiter that is used to separate fields in a row -default [","]
    * @param rowDelimiter	The delimiter that is used to separate rows in a CSV file -default ["\r\n"]
    */
   explicit CsvParser(const std::string& fieldDelimiter = ";",
         const std::string& rowDelimiter = "\n");

   /**
    * @brief Sets the filename for the CSV file
    * @param filename	The filename you want to save in the Parser object.
    */
   void setFilename(const std::string& fileName) = delete;

   /**
    * @brief Read a row from the CSV file
    * @param[out] csvRow - a row from the CSV file
    * @return true - success, false not
    */
   bool readRow(CsvRow& csvRow);

   /**
    * @brief Reads an CSV entire file into CsvTable
    * @param[in] fileName - csv file name
    * @param[out] csvTable - a csvTable that will store a content of the parsed CSV file
    * @return true - success, false not
    */
   bool readCsvFile(const std::string& fileName, CsvTable& csvTable);

   /**
    * @brief Creates CSV file from CsvTable object
    * @param[in] fileName - new created csv file name
    * @param[out] csvTable - a row from the CSV file
    * @return true - success, false not
    */
   bool createCsvFile(const std::string& fileName, const CsvTable& csvTable);

private:

   /**
    * @brief Sets CSV file delimeters
    * @param fieldDelimiter - The delimiter that is used to separate fields in a row.
    * @param rowDelimiter - The delimiter  used to separate rows in a CSV file.
    */
   void setCsvFileDelimeters(const std::string& fieldDelimiter,
         const std::string& rowDelimiter);

   /**
    * @brief Converts a line of CSV file to CsvRow object
    * @param[in] csvLine - line of the CSV file
    * @param[out] csvRow - CsvRow to be filled up with csvLine parsed content
    * @return true - success, false not
    */
   bool convertRowStringToRowFields(const std::string& csvLine, CsvRow& csvRow);

   /**
    * @brief Checks if output stream is in OK state
    * @param[in] ostream - current file stream
    * @return true - success, false - failure
    */
   bool isOStreamCorrect(const std::ostream& ostream);

   /**
    * @brief	The delimiter used to separate fields in a row.
    * @details	Default =  to ";"
    */
   std::string m_fieldDelimiter;

   /**
    * @brief The delimiter  used to separate rows in a CSV file.
    * @details Default = CRLF "\r\n"
    */
   std::string m_rowDelimiter;

   /**
    * @brief Name of CSV file
    */
   std::string m_fileName;

   /**
    * Debug helpers
    */
   bool shouldShowDebugInfo() const;
   void printDebugInfo(const std::string& msg);
   bool m_isDebugEnabled;
};

}

